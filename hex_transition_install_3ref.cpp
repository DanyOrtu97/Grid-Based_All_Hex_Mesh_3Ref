/********************************************************************************
*  This file is part of CinoLib                                                 *
*  Copyright(C) 2016: Marco Livesu                                              *
*                                                                               *
*  The MIT License                                                              *
*                                                                               *
*  Permission is hereby granted, free of charge, to any person obtaining a      *
*  copy of this software and associated documentation files (the "Software"),   *
*  to deal in the Software without restriction, including without limitation    *
*  the rights to use, copy, modify, merge, publish, distribute, sublicense,     *
*  and/or sell copies of the Software, and to permit persons to whom the        *
*  Software is furnished to do so, subject to the following conditions:         *
*                                                                               *
*  The above copyright notice and this permission notice shall be included in   *
*  all copies or substantial portions of the Software.                          *
*                                                                               *
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR   *
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,     *
*  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE *
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER       *
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      *
*  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS *
*  IN THE SOFTWARE.                                                             *
*                                                                               *
*  Author(s):                                                                   *
*                                                                               *
*     Daniele Ortu                                                              *
*********************************************************************************/

#include "hex_transition_install_3ref.h"
#include "hex_transition_orient_3ref.h"

namespace cinolib{


namespace // anonymous
{

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//Custom comparator operator for maps of vec3d
struct vert_compare
{
    bool operator()(const vec3d & a,
                    const vec3d & b) const
    {
       double eps = 1e-6;
       if(a.x()-b.x() < 0.0 && abs(a.x()-b.x()) > eps)
       {
           return true;
       }
       else if(abs(a.x()-b.x()) < eps)
       {
           if(a.y()-b.y() < 0.0 && abs(a.y()-b.y()) > eps)
           {
               return true;
           }
           else if(abs(a.y()-b.y()) < eps)
           {
               if(a.z()-b.z() < 0.0 && abs(a.z()-b.z()) > eps)
               {
                   return true;
               }
           }
       }
       return false;
    }
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class F, class P>
CINO_INLINE
uint edge_orientation(const Polyhedralmesh<M,V,E,F,P> & m, const uint vid, const uint eid)
{
    vec3d v1 = m.vert(vid);
    vec3d v2 = m.edge_vert_id(eid, 0) == vid ? m.edge_vert(eid, 1) : m.edge_vert(eid, 0);
    vec3d n  = v2 - v1; n.normalize();
    for(uint i=0; i<3; i++) n[i] = std::round(n[i]);
    if(n.x() ==  1)  return PLUS_X;
    if(n.x() == -1)  return MINUS_X;
    if(n.y() ==  1)  return PLUS_Y;
    if(n.y() == -1)  return MINUS_Y;
    if(n.z() ==  1)  return PLUS_Z;
    if(n.z() == -1)  return MINUS_Z;
    assert(false);
    return 0;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class F, class P>
CINO_INLINE
void get_transition_verts_direction(const Polyhedralmesh<M,V,E,F,P> & m,
                                    const std::vector<bool>         & transition_verts,
                                          std::vector<uint>         & transition_verts_direction)
{
    transition_verts_direction = std::vector<uint>(m.num_verts());

    std::map<vec3d, uint, vert_compare> v_map;
    for(uint vid=0; vid<m.num_verts(); vid++) v_map[m.vert(vid)] = vid;

    for(uint vid=0; vid<m.num_verts(); vid++)
    {
        if(transition_verts[vid] && m.adj_v2p(vid).size() == 8)
        {
            double length = 0;
            for(uint eid : m.adj_v2e(vid))
            {
                if(m.adj_e2p(eid).size() == 4 && m.edge_length(eid) > length)
                {
                    length = m.edge_length(eid);
                    transition_verts_direction[vid] = edge_orientation(m, vid, eid);
                }
            }
        }
        else if(transition_verts[vid])
        {
            double length = 0;
            for(uint eid : m.adj_v2e(vid))
            {
                if(m.edge_length(eid) > length && v_map.find(m.edge_sample_at(eid, 0.5)) == v_map.end())
                {
                    length = m.edge_length(eid);
                    transition_verts_direction[vid] = edge_orientation(m, vid, eid);
                }
            }
        }
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void merge_schemes_into_mesh(Polyhedralmesh<M,V,E,F,P>            & m,
                             std::unordered_map<uint, SchemeInfo> & poly2scheme){

    std::map<vec3d, uint, vert_compare> v_map;
    std::map<uint, uint> f_map;

    for (uint vid=0; vid<m.num_verts(); ++vid) v_map[m.vert(vid)] = vid;

    for (const auto &p : poly2scheme){
        std::vector<vec3d>              verts;
        std::vector<std::vector<uint>>  faces;
        std::vector<std::vector<uint>>  polys;
        std::vector<std::vector<bool>>  winding;

        vec3d poly_centroid = m.poly_centroid(p.first);
        SchemeInfo info = p.second;
        hex_transition_orient_3ref(verts, faces, polys, winding, info, poly_centroid);

        //merge vertices
        for (auto & v : verts){
            if (v_map.find(v) == v_map.end()){
                uint fresh_vid = m.vert_add(v);
                v_map[v] = fresh_vid;
            }
        }

        //merge faces
        for (uint fid=0; fid<faces.size(); fid++){
            auto f = faces[fid];
            for (uint &v : f) v = v_map[verts.at(v)];

            int test_id = m.face_id(f);

            if(test_id>0){
                f_map[fid] = test_id;
            }
            else{
                uint fresh_fid = m.face_add(f);
                f_map[fid] = fresh_fid;
            }
        }

        //merge polys
        for (uint pid=0; pid<polys.size(); ++pid){
            auto p = polys.at(pid);
            for (auto & fid: p) fid = f_map.at(fid);

            int test_id = m.poly_id(p);

            if (test_id == -1) m.poly_add(p, winding[pid]);
        }
    }
}

} // end anonymous namespace

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void hex_transition_install_3ref(const Polyhedralmesh<M,V,E,F,P>    & m_in,
                                 const std::vector<bool>            & transition_verts,
                                       Polyhedralmesh<M,V,E,F,P>    & m_out){
    m_out = m_in;

    std::vector<uint> transition_verts_direction;
    get_transition_verts_direction(m_in, transition_verts, transition_verts_direction);

    std::unordered_map<uint, SchemeInfo> poly2scheme;

    for (uint pid=0; pid<m_in.num_polys(); pid++){
        std::vector<uint> scheme_vids;

        for(uint vid: m_in.poly_verts_id(pid)){
            if(transition_verts[vid])
                scheme_vids.push_back(vid);
        }
        /*
         * Massimo abbiamo 8 vertici per poly, quindi in questo modo si cade in errore
         * Serve un modo per associare ad ogni poly il numero di hanging nodes (che derivano dai poly vicini)
         */

        if (scheme_vids.size() == 4){ //FACE
            SchemeInfo info;
            info.type = HexTransition::FACE;
            info.scale = m_in.edge_length(m_in.adj_p2e(pid)[0]);
            info.orientations.push_back(transition_verts_direction[scheme_vids[1]]);

            poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
        }
        else if (scheme_vids.size() == 8){ //TRANSITION OR TWO_ADJ_FACES OR FULL
            SchemeInfo info;
            info.type = HexTransition::TRANSITION;
            info.scale = m_in.edge_length(m_in.adj_p2e(pid)[0]);

            //others info about scheme to catch the orientation ****

            poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
        }
    }

    merge_schemes_into_mesh(m_out, poly2scheme);

    std::vector<uint> polys_to_remove;

    for (auto p: poly2scheme) polys_to_remove.push_back(p.first);

    m_out.polys_remove(polys_to_remove);
}


}
