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
void set_orientation_Dnode(const Hexmesh<M,V,E,F,P>    & m,
                           SchemeInfo                  & info,
                           std::vector<uint>           & vertices,
                           const uint                    pid){



}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void set_orientation_Dedge(const Hexmesh<M,V,E,F,P>    & m,
                           SchemeInfo                  & info,
                           std::vector<uint>           & vertices,
                           const uint                    pid){


}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void set_orientation_edge(const Hexmesh<M,V,E,F,P>    & m,
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
    else if (conta_left == 2 && conta_back == 2) info.orientations.push_back(8);
    else if (conta_left == 2 && conta_front == 2) info.orientations.push_back(9);
    else if (conta_right == 2 && conta_back == 2) info.orientations.push_back(10);
    else if (conta_right == 2 && conta_front == 2) info.orientations.push_back(11);


}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void set_orientation_face(const Hexmesh<M,V,E,F,P>    & m,
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

    if (conta_min == 4) info.orientations.push_back(0);
    else if (conta_right == 4) info.orientations.push_back(1);
    else if (conta_left == 4) info.orientations.push_back(2);
    else if (conta_back == 4) info.orientations.push_back(3);
    else if (conta_front == 4) info.orientations.push_back(4);
    else if (conta_max == 4) info.orientations.push_back(5);

}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
std::set<uint> decoupling_node_analysis(Hexmesh<M,V,E,F,P> & m){


    std::set<uint> L;

    for(int pid=0; pid < m.num_polys(); pid++){
        //if pid matches a pattern from figure 9
            //mark the node as level0
        //if pid matches a pattern from figure 10
            //mark the node as level1
    }

    //for auto leaf h
        //if h contains decoupling nodes && h does not match a pattern from figure 11
            //for auto pid in sibling(h)
                //if pid is a leaf && pid is not marked for refinement
                    //L.insert(pid);

    return L;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void sibling_refining(Hexmesh<M,V,E,F,P>                   & m,
                      std::unordered_map<uint, SchemeInfo> & poly2scheme){


    SchemeInfo info;


    //for auto h: internal
        //for auto pid: siblings(h)
            //if pid is a leaf && pid is not marked for refinement
                //mark pid for ref

    //if(mark)
        //for auto pid
            //info.type = HexTransition::FULL;
            //info.scale = m.edge_length(m.adj_p2e(pid)[0];
            //poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
            //for(auto vid: m.poly_verts_id(pid)) transition_verts[vid]=true;

}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void gap_filling(Hexmesh<M,V,E,F,P>                   & m,
                 std::unordered_map<uint, SchemeInfo> & poly2scheme){


    SchemeInfo info;

    bool mesh_changed=true;


    while(mesh_changed){
        //mark h for level 0 ref if matches a pattern from figure 10

        //for auto h: central child node
            //if h matches a pattern from figure 20
                //for auto pid: siblings(h)
                    //if pid is a leaf && pid is not marked for refinement
                        //mark pid for ref

        //if(mark)
            //for auto pid
                //info.type = HexTransition::FULL;
                //info.scale = m.edge_length(m.adj_p2e(pid)[0];
                //poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
                //for(auto vid: m.poly_verts_id(pid)) transition_verts[vid]=true;
        //else
            //mesh_changed=false;

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

    std::unordered_map<uint, bool> decoupling_verts_l0;
    std::unordered_map<uint, bool> decoupling_verts_l1;

    SchemeInfo info;

    sibling_refining(m_in, poly2scheme); //octree (marked pid)

    while(true){

        gap_filling(m_in, poly2scheme); //octree (marked pid)

        std::set<uint> L = decoupling_node_analysis(m_in); //octree (marked pid)

        if(L.size() == 0){
            break;
        }
        else{
            for(auto pid: L){
                info.type = HexTransition::FULL;
                info.scale = m_in.edge_length(m_in.adj_p2e(pid)[0];
                poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));

                for(auto vid: m_in.poly_verts_id(pid)) transition_verts[vid]=true;
            }
        }
    }

    //apply level 0 node decoupling
    for(int pid=0; pid<m_in.num_polys();pid++){
        std::vector<uint> vertices;
        std::vector<uint> poly_verts_id = m_in.poly_verts_id(pid);

        for(uint vid: poly_verts_id) vertices.push_back(decoupling_verts_l0.find(vid));

        int pattern; //funzione per trovare il pattern dati i vertici a true

        switch(pattern){

        }
    }


    //apply level 1 node decoupling
    for(int pid=0; pid<m_in.num_polys();pid++){
        std::vector<uint> vertices;
        std::vector<uint> poly_verts_id = m_in.poly_verts_id(pid);

        for(uint vid: poly_verts_id) vertices.push_back(decoupling_verts_l1.find(vid));

        int pattern; //funzione per trovare il pattern dati i vertici a true

        switch(pattern){

        }
    }


    //apply refinement templates (edge controlled)
    for(int pid=0; pid<m_in.num_polys();pid++){
        std::vector<uint> vertices;
        std::vector<uint> poly_verts_id = m_in.poly_verts_id(pid);

        for(uint vid: poly_verts_id) if(transition_verts[vid]) vertices.push_back(vid);

        int pattern; //funzione per trovare il pattern dati i vertici a true

        switch(pattern){

        }
    }



    merge_schemes_into_mesh(m_out, poly2scheme);

    std::vector<uint> polys_to_remove;

    for (auto p: poly2scheme) polys_to_remove.push_back(p.first);

    m_out.polys_remove(polys_to_remove);

}


}
