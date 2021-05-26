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

template <class M, class V, class E, class F, class P>
CINO_INLINE
void setOrientationInfo2(const Hexmesh<M,V,E,F,P>    & m,
                         SchemeInfo                  & info,
                         std::vector<uint>           & vertices,
                         const uint                    pid){

    std::vector<vec3d> poly_verts = m.poly_verts(pid);
    vec3d min = *std::min_element(poly_verts.begin(), poly_verts.end());
    vec3d max = *std::max_element(poly_verts.begin(), poly_verts.end());

    uint conta_max = 0, conta_min = 0, conta_back = 0, conta_front = 0, conta_left = 0, conta_right = 0;

    for (auto vid : vertices){
        vec3d vert = m.vert(vid);
        if(vert.y() == min.y()){
            conta_min ++;
            if(vert.x() == min.x()) conta_left ++;
            else if(vert.x() == max.x()) conta_right ++;
            if(vert.z() == min.z()) conta_front ++;
            else if(vert.z() == max.z()) conta_back ++;
        }
        else if(vert.y() == max.y()){
            conta_max ++;
            if(vert.x() == min.x()) conta_left ++;
            else if(vert.x() == max.x()) conta_right ++;
            if(vert.z() == min.z()) conta_front ++;
            else if(vert.z() == max.z()) conta_back ++;
        }
    }

    if (conta_min == 2 && conta_left == 2) info.orientations.push_back(0);
    else if (conta_min == 2 && conta_right == 2) info.orientations.push_back(1);
    else if (conta_min == 2 && conta_back == 2) info.orientations.push_back(2);
    else if (conta_min == 2 && conta_front == 2) info.orientations.push_back(3);
    else if (conta_max == 2 && conta_left == 2) info.orientations.push_back(4);
    else if (conta_max == 2 && conta_right == 2) info.orientations.push_back(5);
    else if (conta_max == 2 && conta_back == 2) info.orientations.push_back(6);
    else if (conta_max == 2 && conta_front == 2) info.orientations.push_back(7);

}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void setOrientationInfo3(const Hexmesh<M,V,E,F,P>    & m,
                         SchemeInfo                  & info,
                         std::vector<uint>           & vertices,
                         const uint                    pid){

    std::vector<vec3d> poly_verts = m.poly_verts(pid);
    vec3d min = *std::min_element(poly_verts.begin(), poly_verts.end());
    vec3d max = *std::max_element(poly_verts.begin(), poly_verts.end());

    uint conta_max = 0, conta_min = 0, conta_back = 0, conta_front = 0, conta_left = 0, conta_right = 0;

    for (auto vid : vertices){
        vec3d vert = m.vert(vid);
        if(vert.y() == min.y()){
            conta_min ++;
            if(vert.x() == min.x()) conta_left ++;
            else if(vert.x() == max.x()) conta_right ++;
            if(vert.z() == min.z()) conta_front ++;
            else if(vert.z() == max.z()) conta_back ++;
        }
        else if(vert.y() == max.y()){
            conta_max ++;
            if(vert.x() == min.x()) conta_left ++;
            else if(vert.x() == max.x()) conta_right ++;
            if(vert.z() == min.z()) conta_front ++;
            else if(vert.z() == max.z()) conta_back ++;
        }
    }

    if (conta_min == 3 && conta_left == 2 && conta_front == 2) info.orientations.push_back(0);
    else if (conta_min == 3 && conta_right == 2 && conta_front == 2) info.orientations.push_back(1);
    else if (conta_min == 3 && conta_back == 2 && conta_left == 2) info.orientations.push_back(2);
    else if (conta_min == 3 && conta_front == 2 && conta_right == 2) info.orientations.push_back(3);
    else if (conta_max == 3 && conta_left == 2 && conta_front == 2) info.orientations.push_back(4);
    else if (conta_max == 3 && conta_right == 2 && conta_front == 2) info.orientations.push_back(5);
    else if (conta_max == 3 && conta_back == 2 && conta_left == 2) info.orientations.push_back(6);
    else if (conta_max == 3 && conta_front == 2 && conta_right == 2) info.orientations.push_back(7);

}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void setOrientationInfo6(const Hexmesh<M,V,E,F,P>    & m,
                         SchemeInfo                  & info,
                         std::vector<uint>           & vertices,
                         const uint                    pid){

    std::vector<vec3d> poly_verts = m.poly_verts(pid);
    vec3d min = *std::min_element(poly_verts.begin(), poly_verts.end());
    vec3d max = *std::max_element(poly_verts.begin(), poly_verts.end());

    uint conta_max = 0, conta_min = 0, conta_back = 0, conta_front = 0, conta_left = 0, conta_right = 0;

    for (auto vid : vertices){
        vec3d vert = m.vert(vid);
        if(vert.y() == min.y()){
            conta_min ++;
            if(vert.x() == min.x()) conta_left ++;
            else if(vert.x() == max.x()) conta_right ++;
            if(vert.z() == min.z()) conta_front ++;
            else if(vert.z() == max.z()) conta_back ++;
        }
        else if(vert.y() == max.y()){
            conta_max ++;
            if(vert.x() == min.x()) conta_left ++;
            else if(vert.x() == max.x()) conta_right ++;
            if(vert.z() == min.z()) conta_front ++;
            else if(vert.z() == max.z()) conta_back ++;
        }
    }

    if (conta_left == 4 && conta_back == 4) info.orientations.push_back(0);
    else if (conta_left == 4 && conta_front == 4) info.orientations.push_back(1);
    else if (conta_right == 4 && conta_back == 4) info.orientations.push_back(2);
    else if (conta_right == 4 && conta_front == 4) info.orientations.push_back(3);
    else if (conta_min == 4 && conta_back == 4) info.orientations.push_back(4);
    else if (conta_min == 4 && conta_front == 4) info.orientations.push_back(5);
    else if (conta_min == 4 && conta_left == 4) info.orientations.push_back(6);
    else if (conta_max == 4 && conta_right == 4) info.orientations.push_back(7);
    else if (conta_max == 4 && conta_back == 4) info.orientations.push_back(8);
    else if (conta_max == 4 && conta_front == 4) info.orientations.push_back(9);
    else if (conta_max == 4 && conta_left == 4) info.orientations.push_back(10);
    else if (conta_max == 4 && conta_right == 4) info.orientations.push_back(11);

}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void mark2vertices(const Hexmesh<M,V,E,F,P>                   & m,
                   const uint                                   pid,
                         std::vector<uint>                    & vertices,
                         std::vector<bool>                    & transition_verts,
                         std::vector<uint>                    & poly_verts_id,
                         std::unordered_map<uint, SchemeInfo> & poly2scheme){

    SchemeInfo info;

    int eid=-1;
    for(uint e_id : m.adj_p2e(pid))
        if (m.edge_contains_vert(e_id,vertices[0]) && m.edge_contains_vert(e_id,vertices[1])) eid = (int)e_id;


    if(eid != -1){ //2A
        info.type = HexTransition::EDGE;
        info.scale = m.edge_length(m.adj_p2e(pid)[0]);

        setOrientationInfo2(m, info, vertices, pid);

        poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
    }
    else{
        vec3d v0 = m.vert(vertices[0]);
        vec3d v1 = m.vert(vertices[1]);

        if(v0.x() == v1.x() || v0.y() == v1.y() || v0.z() == v1.z()){ //2B
            info.type = HexTransition::TWO_EDGES;
            info.scale = m.edge_length(m.adj_p2e(pid)[0]);

            for (auto vid: poly_verts_id){
                if(v0.x() == v1.x())
                    if (m.vert(vid).x() == v0.x() && vid != vertices[0] && vid != vertices[1]){
                       vertices.push_back(vid);
                       transition_verts[vid] = true;
                       break;
                    }
                if(v0.y() == v1.y())
                    if (m.vert(vid).y() == v0.y() && vid != vertices[0] && vid != vertices[1]){
                       vertices.push_back(vid);
                       transition_verts[vid] = true;
                       break;
                    }
                if(v0.z() == v1.z())
                    if (m.vert(vid).z() == v0.z() && vid != vertices[0] && vid != vertices[1]){
                       vertices.push_back(vid);
                       transition_verts[vid] = true;
                       break;
                    }
            }

            setOrientationInfo3(m, info, vertices, pid);

        }
        else{ //2C
            info.type = HexTransition::TWO_FACES;
            info.scale = m.edge_length(m.adj_p2e(pid)[0]);

            uint free_edge;

            for(uint eid : m.adj_p2e(pid))
                if (! m.edge_contains_vert(eid, vertices[0]) && ! m.edge_contains_vert(eid, vertices[1]))
                    free_edge = eid;

            for (auto vid: poly_verts_id){
                uint vid0 = m.edge_vert_ids(free_edge)[0];
                uint vid1 = m.edge_vert_ids(free_edge)[1];

                if(vid != vid0 && vid != vid1 && vid != vertices[0] && vid != vertices[1]){
                    vertices.push_back(vid);
                    transition_verts[vid] = true;
                }

            }

            setOrientationInfo6(m, info, vertices, pid);

        }
        poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
    }

}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void merge_schemes_into_mesh(Hexmesh<M,V,E,F,P>                   & m,
                             std::unordered_map<uint, SchemeInfo> & poly2scheme){

    std::map<vec3d, uint, vert_compare> v_map;

    for (uint vid=0; vid<m.num_verts(); ++vid) v_map[m.vert(vid)] = vid;

    for (const auto & p : poly2scheme){
        std::vector<vec3d>              verts;
        std::vector<std::vector<uint>>  polys;

        vec3d poly_centroid = m.poly_centroid(p.first);
        SchemeInfo info = p.second;
        hex_transition_orient_3ref(verts, polys, info, poly_centroid);

        //merge vertices
        for (auto & v : verts){
            if (v_map.find(v) == v_map.end()){
                uint fresh_vid = m.vert_add(v);
                v_map[v] = fresh_vid;
            }
        }

        //merge polys
        for (uint pid=0; pid<polys.size(); ++pid){
            auto p = polys.at(pid);

            for (auto & vid: p) vid = v_map[verts.at(vid)];

            m.poly_add(p);
        }

    }
}

} // end anonymous namespace


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void hex_transition_install_3ref(const Hexmesh<M,V,E,F,P>           & m_in,
                                       std::vector<bool>            & transition_verts,
                                       Hexmesh<M,V,E,F,P>           & m_out){
    m_out = m_in;

    std::unordered_map<uint, SchemeInfo> poly2scheme;


    // PROFILING
    // tstart = start()
    // time = stop(tstart)
    // var_find_min_max += time

    /*
     * SERVONO + CICLI PER I VERTICI AGGIUNTI PER ADATTARE I TEMPLATE
     */


    for (uint pid=0; pid<m_in.num_polys(); pid++){
        std::vector<uint> vertices;
        std::vector<uint> poly_verts_id = m_in.poly_verts_id(pid);

        for(uint vid: poly_verts_id) if(transition_verts[vid]) vertices.push_back(vid);


        SchemeInfo info;
        //Select the right scheme to apply
        switch (vertices.size()){
            case 2: mark2vertices(m_in, pid, vertices, transition_verts, poly_verts_id, poly2scheme);
                    break;
            case 3: //find_conf3vert()
                    break;
            case 4:
                    break;
            case 5:
                    break;
            case 6:
                    break;
            case 7:
                    for (auto vid: poly_verts_id){
                       if(transition_verts[vid] == false){
                            transition_verts[vid] = true;
                            vertices.push_back(vid);
                       }
                    }
                    info.type = HexTransition::FULL;
                    info.scale = m_in.edge_length(m_in.adj_p2e(pid)[0]);
                    poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
                    break;
            case 8:
                    info.type = HexTransition::FULL;
                    info.scale = m_in.edge_length(m_in.adj_p2e(pid)[0]);
                    poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
                    break;
        }

        if(pid > 500 && pid%500==0) std::cout<< "pid : " << pid << " [ " << (pid * 100)/ (m_in.num_polys()) << "% ]" <<std::endl;
    }

    merge_schemes_into_mesh(m_out, poly2scheme);

    std::vector<uint> polys_to_remove;

    for (auto p: poly2scheme) polys_to_remove.push_back(p.first);

    m_out.polys_remove(polys_to_remove);
}


}
