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
void setOrientationInfo1(const Hexmesh<M,V,E,F,P>    & m,
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

    if (conta_min == 1 && conta_left == 1 && conta_back==1) info.orientations.push_back(0);
    else if (conta_min == 1 && conta_right == 1 && conta_back==1) info.orientations.push_back(1);
    else if (conta_min == 1 && conta_right == 1 && conta_front==1) info.orientations.push_back(2);
    else if (conta_min == 1 && conta_left == 1 && conta_front==1) info.orientations.push_back(3);
    else if (conta_max == 1 && conta_left == 1 && conta_back==1) info.orientations.push_back(4);
    else if (conta_max == 1 && conta_right == 1 && conta_back==1) info.orientations.push_back(5);
    else if (conta_max == 1 && conta_right == 1 && conta_front==1) info.orientations.push_back(6);
    else if (conta_max == 1 && conta_left == 1 && conta_front==1) info.orientations.push_back(7);


}

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
    else if (conta_left == 2 && conta_back == 2) info.orientations.push_back(8);
    else if (conta_left == 2 && conta_front == 2) info.orientations.push_back(9);
    else if (conta_right == 2 && conta_back == 2) info.orientations.push_back(10);
    else if (conta_right == 2 && conta_front == 2) info.orientations.push_back(11);


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

    if (conta_min == 3 && conta_left == 2 && conta_back == 2) info.orientations.push_back(0);
    else if (conta_min == 3 && conta_right == 2 && conta_back == 2) info.orientations.push_back(1);
    else if (conta_min == 3 && conta_left == 2 && conta_front == 2) info.orientations.push_back(2);
    else if (conta_min == 3 && conta_right == 2 && conta_front == 2) info.orientations.push_back(3);
    else if (conta_max == 3 && conta_left == 2 && conta_back == 2) info.orientations.push_back(4);
    else if (conta_max == 3 && conta_right == 2 && conta_back == 2) info.orientations.push_back(5);
    else if (conta_max == 3 && conta_left == 2 && conta_front == 2) info.orientations.push_back(6);
    else if (conta_max == 3 && conta_right == 2 && conta_front == 2) info.orientations.push_back(7);
    else if (conta_back == 3 && conta_left == 2 && conta_min == 2) info.orientations.push_back(8);
    else if (conta_back == 3 && conta_left == 2 && conta_max == 2) info.orientations.push_back(9);
    else if (conta_back == 3 && conta_right == 2 && conta_min == 2) info.orientations.push_back(10);
    else if (conta_back == 3 && conta_right == 2 && conta_max == 2) info.orientations.push_back(11);
    else if (conta_front == 3 && conta_left == 2 && conta_min == 2) info.orientations.push_back(12);
    else if (conta_front == 3 && conta_left == 2 && conta_max == 2) info.orientations.push_back(13);
    else if (conta_front == 3 && conta_right == 2 && conta_min == 2) info.orientations.push_back(14);
    else if (conta_front == 3 && conta_right == 2 && conta_max == 2) info.orientations.push_back(15);
    else if (conta_right == 3 && conta_front == 2 && conta_min == 2) info.orientations.push_back(16);
    else if (conta_right == 3 && conta_front == 2 && conta_max == 2) info.orientations.push_back(17);
    else if (conta_right == 3 && conta_back == 2 && conta_min == 2) info.orientations.push_back(18);
    else if (conta_right == 3 && conta_back == 2 && conta_max == 2) info.orientations.push_back(19);
    else if (conta_left == 3 && conta_front == 2 && conta_min == 2) info.orientations.push_back(20);
    else if (conta_left == 3 && conta_front == 2 && conta_max == 2) info.orientations.push_back(21);
    else if (conta_left == 3 && conta_back == 2 && conta_min == 2) info.orientations.push_back(22);
    else if (conta_left == 3 && conta_back == 2 && conta_max == 2) info.orientations.push_back(23);



}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void setOrientationInfo4(const Hexmesh<M,V,E,F,P>    & m,
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
    else if (conta_min == 4 && conta_right == 4) info.orientations.push_back(7);
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
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void mark3vertices(const Hexmesh<M,V,E,F,P>                   & m,
                   const uint                                   pid,
                         std::vector<uint>                    & vertices,
                         std::unordered_map<uint, SchemeInfo> & poly2scheme){

    SchemeInfo info;

    int n_free_edge = 0;

    for(uint eid : m.adj_p2e(pid))
        if(! m.edge_contains_vert(eid, vertices[0]) &&
           ! m.edge_contains_vert(eid, vertices[1]) &&
           ! m.edge_contains_vert(eid, vertices[2]))
           n_free_edge++;


    if(n_free_edge == 5){ //3A
        info.type = HexTransition::TWO_EDGES;
        info.scale = m.edge_length(m.adj_p2e(pid)[0]);
        setOrientationInfo3(m, info, vertices, pid);
        poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
    }
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void mark4vertices(const Hexmesh<M,V,E,F,P>                   & m,
                   const uint                                   pid,
                         std::vector<uint>                    & vertices,
                         std::unordered_map<uint, SchemeInfo> & poly2scheme){

    SchemeInfo info;
    int fid=-1;

    for(uint f_id : m.adj_p2f(pid))
        if (m.face_contains_vert(f_id,vertices[0]) &&
            m.face_contains_vert(f_id,vertices[1]) &&
            m.face_contains_vert(f_id,vertices[2]) &&
            m.face_contains_vert(f_id,vertices[3]))
            fid = (int)f_id;


    if(fid != -1){ // 4A
        info.type = HexTransition::FACE;
        info.scale = m.edge_length(m.adj_p2e(pid)[0]);
        setOrientationInfo4(m, info, vertices, pid);
        poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void mark4verticesD(const Hexmesh<M,V,E,F,P>                   & m,
                   const uint                                   pid,
                         std::vector<uint>                    & vertices,
                         std::vector<bool>                    & transition_verts,
                         std::vector<uint>                    & poly_verts_id,
                         std::vector<bool>                    & decoupling_nodes){


    int n_free_edge = 0;
    int free_edge=-1;

    for(uint eid : m.adj_p2e(pid))
        if(! m.edge_contains_vert(eid, vertices[0]) &&
           ! m.edge_contains_vert(eid, vertices[1]) &&
           ! m.edge_contains_vert(eid, vertices[2]) &&
           ! m.edge_contains_vert(eid, vertices[3])){
            free_edge = (int)eid;
            n_free_edge ++;
        }

    int faces_3_nodes=0;
    int conta_nodes;

    for(uint f_id : m.adj_p2f(pid)){
        conta_nodes=0;
        if (m.face_contains_vert(f_id,vertices[0])) conta_nodes++;
        if (m.face_contains_vert(f_id,vertices[1])) conta_nodes++;
        if (m.face_contains_vert(f_id,vertices[2])) conta_nodes++;
        if (m.face_contains_vert(f_id,vertices[3])) conta_nodes++;

        if(conta_nodes == 3) faces_3_nodes++;
    }

    if(n_free_edge == 3){
        if(faces_3_nodes==3){ // 4B

            std::cout<<"4B_D"<<std::endl;
            std::vector<bool> t4_a = {true, true, false, true, true, false, false, false};
            std::vector<bool> t4_b = {true, true, true, false, false, true, false, false};
            std::vector<bool> t4_c = {false, true, true, true, false, false, true, false};
            std::vector<bool> t4_d = {true, false, true, true, false, false, false, true};
            std::vector<bool> t4_e = {false, false, false, true, true, false, true, true};
            std::vector<bool> t4_f = {true, false, false, false, true, true, false, true};
            std::vector<bool> t4_g = {false, true, false, false, true, true, true, false};
            std::vector<bool> t4_h = {false, false, true, false, false, true, true, true};

            std::vector<bool> t4;

            for (auto vid: poly_verts_id) t4.push_back(transition_verts[vid]);

            if(t4 == t4_a){ //a - l - x
                std::vector<bool> mask = {false, false, false, false, false, false, true, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t4 == t4_b){ //b - o - y
                std::vector<bool> mask = {false, false, false, false, false, false, false, true};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t4 == t4_c){ //c - p - v
                std::vector<bool> mask = {false, false, false, false, true, false, false, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t4 == t4_d){ //d - m - s
                std::vector<bool> mask = {false, false, false, false, false, true, false, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t4 == t4_e){ //e - n - t
                std::vector<bool> mask = {false, true, false, false, false, false, false, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t4 == t4_f){ //f - i - w
                std::vector<bool> mask = {false, false, false, false, false, false, true, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t4 == t4_g){ //g - r - z
                std::vector<bool> mask = {false, false, false, true, false, false, false, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t4 == t4_h){ //h - q - u
                std::vector<bool> mask = {true, false, false, false, false, false, false, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
        }
    }
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void mark5verticesD(const Hexmesh<M,V,E,F,P>                   & m,
                    const uint                                   pid,
                          std::vector<uint>                    & vertices,
                          std::vector<bool>                    & transition_verts,
                          std::vector<uint>                    & poly_verts_id,
                          std::vector<bool>                    & decoupling_nodes){


    int free_edge = -1;
    int n_free_edge = 0;

    for(uint eid : m.adj_p2e(pid))
        if(! m.edge_contains_vert(eid, vertices[0]) &&
           ! m.edge_contains_vert(eid, vertices[1]) &&
           ! m.edge_contains_vert(eid, vertices[2]) &&
           ! m.edge_contains_vert(eid, vertices[3]) &&
           ! m.edge_contains_vert(eid, vertices[4])){
            free_edge = (int)eid;
            n_free_edge++;
        }


    if(free_edge != -1){ // 5A, 5B
        if(n_free_edge == 2){ // 5A
            std::vector<bool> t5_a = {true, true, true, true, true, false, false, false};
            std::vector<bool> t5_b = {true, true, true, true, false, true, false, false};
            std::vector<bool> t5_c = {true, true, true, true, false, false, true, false};
            std::vector<bool> t5_d = {true, true, true, true, false, false, false, true};
            std::vector<bool> t5_e = {false, false, false, true, true, true, true, true};
            std::vector<bool> t5_f = {false, false, true, false, true, true, true, true};
            std::vector<bool> t5_g = {false, true, false, false, true, true, true, true};
            std::vector<bool> t5_h = {true, false, false, false, true, true, true, true};
            std::vector<bool> t5_i = {true, false, false, true, true, true, false, true};
            std::vector<bool> t5_l = {true, true, false, true, true, false, false, true};
            std::vector<bool> t5_m = {true, false, true, true, true, false, false, true};
            std::vector<bool> t5_n = {true, false, false, true, true, false, true, true};
            std::vector<bool> t5_o = {true, true, true, false, false, true, true, false};
            std::vector<bool> t5_p = {false, true, true, false, true, true, true, false};
            std::vector<bool> t5_q = {false, true, true, false, false, true, true, true};
            std::vector<bool> t5_r = {false, true, true, true, false, true, true, false};
            std::vector<bool> t5_s = {true, true, false, false, true, true, false, true};
            std::vector<bool> t5_t = {true, true, false, true, true, true, false, false};
            std::vector<bool> t5_u = {true, true, true, false, true, true, false, false};
            std::vector<bool> t5_v = {true, true, false, false, true, true, true, false};
            std::vector<bool> t5_w = {true, false, true, true, false, false, true, true};
            std::vector<bool> t5_x = {false, true, true, true, false, false, true, true};
            std::vector<bool> t5_y = {false, false, true, true, false, true, true, true};
            std::vector<bool> t5_z = {false, false, true, true, true, false, true, true};

            std::vector<bool> t5;

            for (auto vid: poly_verts_id) t5.push_back(transition_verts[vid]);

            if(t5 == t5_a || t5 == t5_m){
                std::vector<bool> mask = {false, false, false, false, false, false, true, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t5 == t5_b || t5 == t5_t){
                std::vector<bool> mask = {false, false, false, false, false, false, false, true};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t5 == t5_c || t5 == t5_o){
                std::vector<bool> mask = {false, false, false, false, true, false, false, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t5 == t5_d || t5 == t5_x){
                std::vector<bool> mask = {false, false, false, false, false, true, false, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t5 == t5_e || t5 == t5_i){
                std::vector<bool> mask = {false, true, false, false, false, false, false, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t5 == t5_f || t5 == t5_z){
                std::vector<bool> mask = {true, false, false, false, false, false, false, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t5 == t5_g || t5 == t5_q){
                std::vector<bool> mask = {false, false, false, true, false, false, false, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t5 == t5_h || t5 == t5_v){
                std::vector<bool> mask = {false, false, true, false, false, false, false, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t5 == t5_l || t5 == t5_s){
                std::vector<bool> mask = {false, false, false, false, false, false, true, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t5 == t5_n || t5 == t5_w){
                std::vector<bool> mask = {false, true, false, false, false, false, false, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t5 == t5_p || t5 == t5_u){
                std::vector<bool> mask = {false, false, false, true, false, false, false, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
            else if(t5 == t5_r || t5 == t5_y){
                std::vector<bool> mask = {false, false, false, false, true, false, false, false};
                for(int i=0; i<8; i++) if(! decoupling_nodes[poly_verts_id[i]] && mask[i]) decoupling_nodes[poly_verts_id[i]] = true;
            }
        }
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void mark6vertices(const Hexmesh<M,V,E,F,P>                   & m,
                   const uint                                   pid,
                         std::vector<uint>                    & vertices,
                         std::unordered_map<uint, SchemeInfo> & poly2scheme){

    SchemeInfo info;

    int free_edge = -1;

    for(uint eid : m.adj_p2e(pid))
        if(! m.edge_contains_vert(eid, vertices[0]) &&
           ! m.edge_contains_vert(eid, vertices[1]) &&
           ! m.edge_contains_vert(eid, vertices[2]) &&
           ! m.edge_contains_vert(eid, vertices[3]) &&
           ! m.edge_contains_vert(eid, vertices[4]) &&
           ! m.edge_contains_vert(eid, vertices[5]))
           free_edge = (int)eid;


    if(free_edge != -1){ // 6A
        info.type = HexTransition::TWO_FACES;
        info.scale = m.edge_length(m.adj_p2e(pid)[0]);
        setOrientationInfo6(m, info, vertices, pid);
        poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void markTemplates(const Hexmesh<M,V,E,F,P>                   & m_in,
                         std::vector<bool>                    & transition_verts,
                         std::unordered_map<uint, SchemeInfo> & poly2scheme,
                         std::vector<uint>                    & polys){


    for (auto pid: polys){
        std::vector<uint> vertices;
        std::vector<uint> poly_verts_id = m_in.poly_verts_id(pid);

        for(uint vid: poly_verts_id) if(transition_verts[vid]) vertices.push_back(vid);

        SchemeInfo info;

        switch (vertices.size()){
            case 2: mark2vertices(m_in, pid, vertices, poly2scheme);
                    break;
            case 3: mark3vertices(m_in, pid, vertices, poly2scheme);
                    break;
            case 4: mark4vertices(m_in, pid, vertices, poly2scheme);
                    break;
            case 6: mark6vertices(m_in, pid, vertices, poly2scheme);
                    break;
            case 8: info.type = HexTransition::FULL;
                    info.scale = m_in.edge_length(m_in.adj_p2e(pid)[0]);
                    poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
                    break;
        }
    }
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void markDecoupling(const Hexmesh<M,V,E,F,P>                   & m_in,
                          std::vector<bool>                    & transition_verts,
                          std::vector<bool>                    & decoupling_nodes,
                          std::vector<uint>                    & polys){


    for (auto pid: polys){
        std::vector<uint> vertices;
        std::vector<uint> poly_verts_id = m_in.poly_verts_id(pid);

        for(uint vid: poly_verts_id) if(transition_verts[vid]) vertices.push_back(vid);


        switch (vertices.size()){
            case 4: mark4verticesD(m_in, pid, vertices, transition_verts, poly_verts_id, decoupling_nodes);
                    break;
            case 5: mark5verticesD(m_in, pid, vertices, transition_verts, poly_verts_id, decoupling_nodes);
                    break;
            case 7: for(auto vid: poly_verts_id) if(!transition_verts[vid]) decoupling_nodes[vid] = true;
                    break;
        }
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

    std::vector<uint> polys;

    for(uint i=0;i<m_in.num_polys();i++) polys.push_back(i);

    markTemplates(m_out, transition_verts, poly2scheme, polys);

    merge_schemes_into_mesh(m_out, poly2scheme);

    std::vector<uint> polys_to_remove;

    for (auto p: poly2scheme) polys_to_remove.push_back(p.first);

    m_out.polys_remove(polys_to_remove);

}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void hex_transition_install_3refDecoupling(const Hexmesh<M,V,E,F,P>           & m_in,
                                                 std::vector<bool>            & transition_verts,
                                                 Hexmesh<M,V,E,F,P>           & m_out){
    m_out = m_in;

    std::unordered_map<uint, SchemeInfo> poly2scheme;

    std::vector<bool> decoupling_nodes;

    std::vector<uint> polys;

    for(uint i=0;i<m_in.num_polys();i++) polys.push_back(i);

    for(uint vid=0; vid<m_in.num_verts(); vid++) decoupling_nodes.push_back(false);

    markDecoupling(m_in, transition_verts, decoupling_nodes, polys);

    for (auto pid : polys){
        std::vector<uint> v_lv0;
        std::vector<uint> poly_verts_id = m_in.poly_verts_id(pid);

        for(uint vid: poly_verts_id) if(decoupling_nodes[vid]) v_lv0.push_back(vid);

        SchemeInfo info;


        switch (v_lv0.size()){
            case 1: info.type = HexTransition::NODE;
                    info.scale = m_in.edge_length(m_in.adj_p2e(pid)[0]);
                    setOrientationInfo1(m_in, info, v_lv0, pid);
                    poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
                    break;
        }
    }

    merge_schemes_into_mesh(m_out, poly2scheme);

    std::vector<uint> polys_to_remove;

    for (auto p: poly2scheme) polys_to_remove.push_back(p.first);

    m_out.polys_remove(polys_to_remove);
}


}
