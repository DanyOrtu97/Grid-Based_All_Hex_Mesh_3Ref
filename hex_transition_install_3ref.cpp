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
                         std::vector<bool>                    & transition_verts,
                         std::vector<uint>                    & poly_verts_id,
                         std::unordered_map<uint, SchemeInfo> & poly2scheme,
                         std::vector<uint>                    & changed_pid,
                         std::vector<uint>                    & changed_vid){

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
    else{ //2B or 2C
        vec3d v0 = m.vert(vertices[0]);
        vec3d v1 = m.vert(vertices[1]);

        if(v0.x() == v1.x() || v0.y() == v1.y() || v0.z() == v1.z()){ //2B

            for (auto vid: poly_verts_id){
                if(v0.x() == v1.x())
                    if (m.vert(vid).x() == v0.x() && vid != vertices[0] && vid != vertices[1]){
                        changed_vid.push_back(vid);
                        break;
                    }
                if(v0.y() == v1.y())
                    if (m.vert(vid).y() == v0.y() && vid != vertices[0] && vid != vertices[1]){
                        changed_vid.push_back(vid);
                        break;
                    }
                if(v0.z() == v1.z())
                    if (m.vert(vid).z() == v0.z() && vid != vertices[0] && vid != vertices[1]){
                        changed_vid.push_back(vid);
                        break;
                    }
            }
            changed_pid.push_back(pid);
        }
        else{ //2C
            std::vector<bool> t2_a = {true, false, false, false, false, false, true, false};
            std::vector<bool> t2_b = {false, false, false, true, false, true, false, false};
            std::vector<bool> t2_c = {false, false, true, false, true, false, false, false};
            std::vector<bool> t2_d = {false, true, false, false, false, false, false, true};
            std::vector<bool> t2;

            for (auto vid: poly_verts_id) t2.push_back(transition_verts[vid]);

            if(t2 == t2_a){
                std::vector<bool> mask_a = {true, true, true, true, false, false, true, true};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask_a[i]) changed_vid.push_back(poly_verts_id[i]);
            }
            else if(t2 == t2_b){
                std::vector<bool> mask_b = {true, true, true, true, false, true, true, false};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask_b[i]) changed_vid.push_back(poly_verts_id[i]);
            }
            else if(t2 == t2_c){
                std::vector<bool> mask_c = {true, true, true, true, true, true, false, false};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask_c[i]) changed_vid.push_back(poly_verts_id[i]);
            }
            else if(t2 == t2_d){
                std::vector<bool> mask_d = {true, true, true, true, true, false, false, true};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask_d[i]) changed_vid.push_back(poly_verts_id[i]);
            }
            changed_pid.push_back(pid);
        }
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void mark3vertices(const Hexmesh<M,V,E,F,P>                   & m,
                   const uint                                   pid,
                         std::vector<uint>                    & vertices,
                         std::vector<bool>                    & transition_verts,
                         std::vector<uint>                    & poly_verts_id,
                         std::unordered_map<uint, SchemeInfo> & poly2scheme,
                         std::vector<uint>                    & changed_pid,
                         std::vector<uint>                    & changed_vid){

    SchemeInfo info;

    bool is_3a = false;

    vec3d v0 = m.vert(vertices[0]);
    vec3d v1 = m.vert(vertices[1]);
    vec3d v2 = m.vert(vertices[2]);

    if((v0.x() == v1.x() && v1.x() == v2.x() && v2.x() == v0.x()) ||
       (v0.y() == v1.y() && v1.y() == v2.y() && v2.y() == v0.y()) ||
       (v0.z() == v1.z() && v1.z() == v2.z() && v2.z() == v0.z()))
        is_3a=true;


    if(is_3a){ //3A (maybe is better now)
        info.type = HexTransition::TWO_EDGES;
        info.scale = m.edge_length(m.adj_p2e(pid)[0]);
        setOrientationInfo3(m, info, vertices, pid);
        poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
    }
    else{ //3B, 3C
        int n_free_edge = 0;

        for(uint eid : m.adj_p2e(pid))
            if(! m.edge_contains_vert(eid, vertices[0]) &&
               ! m.edge_contains_vert(eid, vertices[1]) &&
               ! m.edge_contains_vert(eid, vertices[2]))
               n_free_edge++;

        if(n_free_edge == 4){ // 3B

            //da fareee

        }
        else{  //3C


            //da fareee

        }
    }
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void mark4vertices(const Hexmesh<M,V,E,F,P>                   & m,
                   const uint                                   pid,
                         std::vector<uint>                    & vertices,
                         std::vector<bool>                    & transition_verts,
                         std::vector<uint>                    & poly_verts_id,
                         std::unordered_map<uint, SchemeInfo> & poly2scheme,
                         std::vector<uint>                    & changed_pid,
                         std::vector<uint>                    & changed_vid){

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
    else{ // 4B, 4C, 4D, 4E or 4F
        int n_free_edge = 0;

        for(uint eid : m.adj_p2e(pid))
            if(! m.edge_contains_vert(eid, vertices[0]) &&
               ! m.edge_contains_vert(eid, vertices[1]) &&
               ! m.edge_contains_vert(eid, vertices[2]) &&
               ! m.edge_contains_vert(eid, vertices[3]))
                n_free_edge ++;

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

        if(n_free_edge == 3){ // 4B, 4C
            if(faces_3_nodes==3){ // 4B
                std::cout<<"4B"<<std::endl;
                std::vector<vec3d> poly_verts = m.poly_verts(pid);
                vec3d min_v = *std::min_element(poly_verts.begin(), poly_verts.end());
                vec3d max_v = *std::max_element(poly_verts.begin(), poly_verts.end());

                uint max=0, min=0, left=0, right=0, back=0, front=0;


                for (auto vid : vertices){
                    vec3d vert = m.vert(vid);
                    if(vert.y() == min_v.y()){
                        min ++;
                        if(vert.x() == min_v.x()) left ++;
                        else if(vert.x() == max_v.x()) right ++;
                        if(vert.z() == min_v.z()) front ++;
                        else if(vert.z() == max_v.z()) back ++;
                    }
                    else if(vert.y() == max_v.y()){
                        max ++;
                        if(vert.x() == min_v.x()) left ++;
                        else if(vert.x() == max_v.x()) right ++;
                        if(vert.z() == min_v.z()) front ++;
                        else if(vert.z() == max_v.z()) back ++;
                    }
                }

                if(min==3){
                    if((left==3 && back==3) || (right==3 && front==3)){
                        changed_vid.push_back(poly_verts_id[5]);
                        changed_vid.push_back(poly_verts_id[7]);
                    }
                    else if((back==3 && right==3) || (front==3 && left==3)){
                        changed_vid.push_back(poly_verts_id[4]);
                        changed_vid.push_back(poly_verts_id[6]);
                    }
                }
                else if(max==3){
                    if((left==3 && back==3) || (right==3 && front==3)){
                        changed_vid.push_back(poly_verts_id[1]);
                        changed_vid.push_back(poly_verts_id[3]);
                    }
                    else if((back==3 && right==3) || (front==3 && left==3)){
                        changed_vid.push_back(poly_verts_id[0]);
                        changed_vid.push_back(poly_verts_id[2]);
                    }
                }
                else if(left==3){
                    if((back==3 && max==3) || (front==3 && min==3)){
                        changed_vid.push_back(poly_verts_id[1]);
                        changed_vid.push_back(poly_verts_id[6]);
                    }
                    else if((max==3 && front==3) || (min==3 && back==3)){
                        changed_vid.push_back(poly_verts_id[2]);
                        changed_vid.push_back(poly_verts_id[5]);
                    }
                }
                else if(right==3){
                    if((min==3 && back==3) || (max==3 && front==3)){
                        changed_vid.push_back(poly_verts_id[3]);
                        changed_vid.push_back(poly_verts_id[4]);
                    }
                    else if((back==3 && max==3) || (front==3 && min==3)){
                        changed_vid.push_back(poly_verts_id[0]);
                        changed_vid.push_back(poly_verts_id[7]);
                    }
                }
                else if(back==3){
                    if((max==3 && left==3) || (min==3 && right==3)){
                        changed_vid.push_back(poly_verts_id[3]);
                        changed_vid.push_back(poly_verts_id[6]);
                    }
                    else if((left==3 && min==3) || (right==3 && max==3)){
                        changed_vid.push_back(poly_verts_id[2]);
                        changed_vid.push_back(poly_verts_id[7]);
                    }
                }
                else if(front==3){
                    if((left==3 && min==3) || (right==3 && max==3)){
                        changed_vid.push_back(poly_verts_id[1]);
                        changed_vid.push_back(poly_verts_id[4]);
                    }
                    else if((min==3 && right==3) || (max==3 && left==3)){
                        changed_vid.push_back(poly_verts_id[0]);
                        changed_vid.push_back(poly_verts_id[5]);
                    }
                }

                changed_pid.push_back(pid);

            }
            else{ //4C
                std::cout<<"4C"<<std::endl;
                std::vector<vec3d> poly_verts = m.poly_verts(pid);
                vec3d min_v = *std::min_element(poly_verts.begin(), poly_verts.end());
                vec3d max_v = *std::max_element(poly_verts.begin(), poly_verts.end());

                uint max=0, min=0, left=0, right=0, back=0, front=0;


                for (auto vid : vertices){
                    vec3d vert = m.vert(vid);
                    if(vert.y() == min_v.y()){
                        min ++;
                        if(vert.x() == min_v.x()) left ++;
                        else if(vert.x() == max_v.x()) right ++;
                        if(vert.z() == min_v.z()) front ++;
                        else if(vert.z() == max_v.z()) back ++;
                    }
                    else if(vert.y() == max_v.y()){
                        max ++;
                        if(vert.x() == min_v.x()) left ++;
                        else if(vert.x() == max_v.x()) right ++;
                        if(vert.z() == min_v.z()) front ++;
                        else if(vert.z() == max_v.z()) back ++;
                    }
                }

                if(min==3){
                    if(back==3 && right==2){
                        changed_vid.push_back(poly_verts_id[4]);
                        changed_vid.push_back(poly_verts_id[7]);
                    }
                    else if(right==3 && front==2){
                        changed_vid.push_back(poly_verts_id[4]);
                        changed_vid.push_back(poly_verts_id[5]);
                    }
                    else if(front==3 && left==2){
                        changed_vid.push_back(poly_verts_id[5]);
                        changed_vid.push_back(poly_verts_id[6]);
                    }
                    else if(left==3 && back==2){
                        changed_vid.push_back(poly_verts_id[6]);
                        changed_vid.push_back(poly_verts_id[7]);
                    }
                }
                else if(max==3){
                    if(back==3 && left==2){
                        changed_vid.push_back(poly_verts_id[1]);
                        changed_vid.push_back(poly_verts_id[2]);
                    }
                    else if(left==3 && front==2){
                        changed_vid.push_back(poly_verts_id[0]);
                        changed_vid.push_back(poly_verts_id[1]);
                    }
                    else if(front==3 && right==2){
                        changed_vid.push_back(poly_verts_id[0]);
                        changed_vid.push_back(poly_verts_id[3]);
                    }
                    else if(right==3 && back==2){
                        changed_vid.push_back(poly_verts_id[3]);
                        changed_vid.push_back(poly_verts_id[2]);
                    }
                }
                else if(left==3){
                    if(back==3 && max==2){
                        changed_vid.push_back(poly_verts_id[5]);
                        changed_vid.push_back(poly_verts_id[6]);
                    }
                    else if(max==3 && front==2){
                        changed_vid.push_back(poly_verts_id[6]);
                        changed_vid.push_back(poly_verts_id[2]);
                    }
                    else if(front==3 && min==2){
                        changed_vid.push_back(poly_verts_id[2]);
                        changed_vid.push_back(poly_verts_id[1]);
                    }
                    else if(min==3 && back==2){
                        changed_vid.push_back(poly_verts_id[1]);
                        changed_vid.push_back(poly_verts_id[5]);
                    }
                }
                else if(right==3){
                    if(min==2 && back==3){
                        changed_vid.push_back(poly_verts_id[0]);
                        changed_vid.push_back(poly_verts_id[3]);
                    }
                    else if(back==2 && max==3){
                        changed_vid.push_back(poly_verts_id[0]);
                        changed_vid.push_back(poly_verts_id[4]);
                    }
                    else if(max==2 && front==3){
                        changed_vid.push_back(poly_verts_id[4]);
                        changed_vid.push_back(poly_verts_id[7]);
                    }
                    else if(front==2 && min==3){
                        changed_vid.push_back(poly_verts_id[7]);
                        changed_vid.push_back(poly_verts_id[3]);
                    }
                }
                else if(back==3){
                    if(max==2 && left==3){
                        changed_vid.push_back(poly_verts_id[3]);
                        changed_vid.push_back(poly_verts_id[2]);
                    }
                    else if(left==2 && min==3){
                        changed_vid.push_back(poly_verts_id[2]);
                        changed_vid.push_back(poly_verts_id[6]);
                    }
                    else if(min==2 && right==3){
                        changed_vid.push_back(poly_verts_id[6]);
                        changed_vid.push_back(poly_verts_id[7]);
                    }
                    else if(right==2 && max==3){
                        changed_vid.push_back(poly_verts_id[7]);
                        changed_vid.push_back(poly_verts_id[3]);
                    }
                }
                else if(front==3){
                    if(min==3 && right==2){
                        changed_vid.push_back(poly_verts_id[0]);
                        changed_vid.push_back(poly_verts_id[4]);
                    }
                    else if(right==3 && max==2){
                        changed_vid.push_back(poly_verts_id[0]);
                        changed_vid.push_back(poly_verts_id[1]);
                    }
                    else if(max==3 && left==2){
                        changed_vid.push_back(poly_verts_id[1]);
                        changed_vid.push_back(poly_verts_id[5]);
                    }
                    else if(left==3 && min==2){
                        changed_vid.push_back(poly_verts_id[5]);
                        changed_vid.push_back(poly_verts_id[4]);
                    }
                }

                changed_pid.push_back(pid);

            }
        }
        else if(n_free_edge == 2){ // 4D, 4E
            if(faces_3_nodes==1){ // 4D


            }
            else{ //4E


            }
        }
        else{ // 4F
            for (auto vid: poly_verts_id) if(transition_verts[vid]==false) changed_vid.push_back(vid);

            changed_pid.push_back(pid);
        }
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void mark5vertices(const Hexmesh<M,V,E,F,P>                   & m,
                   const uint                                   pid,
                         std::vector<uint>                    & vertices,
                         std::vector<bool>                    & transition_verts,
                         std::vector<uint>                    & poly_verts_id,
                         std::vector<uint>                    & changed_pid,
                         std::vector<uint>                    & changed_vid){


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
            std::cout<<"5A"<<std::endl;
            std::vector<vec3d> poly_verts = m.poly_verts(pid);
            vec3d min_v = *std::min_element(poly_verts.begin(), poly_verts.end());
            vec3d max_v = *std::max_element(poly_verts.begin(), poly_verts.end());

            uint max=0, min=0, left=0, right=0, back=0, front=0;


            for (auto vid : vertices){
                vec3d vert = m.vert(vid);
                if(vert.y() == min_v.y()){
                    min ++;
                    if(vert.x() == min_v.x()) left ++;
                    else if(vert.x() == max_v.x()) right ++;
                    if(vert.z() == min_v.z()) front ++;
                    else if(vert.z() == max_v.z()) back ++;
                }
                else if(vert.y() == max_v.y()){
                    max ++;
                    if(vert.x() == min_v.x()) left ++;
                    else if(vert.x() == max_v.x()) right ++;
                    if(vert.z() == min_v.z()) front ++;
                    else if(vert.z() == max_v.z()) back ++;
                }
            }

            if(min==4){
                if(left==3 && back==3) changed_vid.push_back(poly_verts_id[7]);
                else if(back==3 && right==3) changed_vid.push_back(poly_verts_id[4]);
                else if(right==3 && front==3) changed_vid.push_back(poly_verts_id[5]);
                else if(front==3 && left==3) changed_vid.push_back(poly_verts_id[6]);
            }
            else if(max==4){
                if(left==3 && back==3) changed_vid.push_back(poly_verts_id[3]);
                else if(back==3 && right==3) changed_vid.push_back(poly_verts_id[0]);
                else if(right==3 && front==3) changed_vid.push_back(poly_verts_id[1]);
                else if(front==3 && left==3) changed_vid.push_back(poly_verts_id[2]);
            }
            else if(left==4){
                if(back==3 && max==3) changed_vid.push_back(poly_verts_id[6]);
                else if(max==3 && front==3) changed_vid.push_back(poly_verts_id[2]);
                else if(front==3 && min==3) changed_vid.push_back(poly_verts_id[1]);
                else if(min==3 && back==3) changed_vid.push_back(poly_verts_id[5]);
            }
            else if(right==4){
                if(min==3 && back==3) changed_vid.push_back(poly_verts_id[3]);
                else if(back==3 && max==3) changed_vid.push_back(poly_verts_id[0]);
                else if(max==3 && front==3) changed_vid.push_back(poly_verts_id[4]);
                else if(front==3 && min==3) changed_vid.push_back(poly_verts_id[7]);
            }
            else if(back==4){
                if(max==3 && left==3) changed_vid.push_back(poly_verts_id[3]);
                else if(left==3 && min==3) changed_vid.push_back(poly_verts_id[2]);
                else if(min==3 && right==3) changed_vid.push_back(poly_verts_id[6]);
                else if(right==3 && max==3) changed_vid.push_back(poly_verts_id[7]);
            }
            else if(front==4){
                if(left==3 && min==3) changed_vid.push_back(poly_verts_id[4]);
                else if(min==3 && right==3) changed_vid.push_back(poly_verts_id[0]);
                else if(right==3 && max==3) changed_vid.push_back(poly_verts_id[1]);
                else if(max==3 && left==3) changed_vid.push_back(poly_verts_id[5]);
            }

            changed_pid.push_back(pid);
        }
        else{ //5B
            for(auto vid: poly_verts_id){
                uint vid0 = m.edge_vert_ids(free_edge)[0];
                uint vid1 = m.edge_vert_ids(free_edge)[1];

                if(vid != vid0 && vid != vid1 && vid!=vertices[0] && vid!=vertices[1] && vid!=vertices[2] && vid!=vertices[3] && vid!=vertices[4]){
                    changed_vid.push_back(vid);
                }
            }
            changed_pid.push_back(pid);
        }
    }
    else{ // 5C
        for (auto vid: poly_verts_id) if(transition_verts[vid]==false) changed_vid.push_back(vid);

        changed_pid.push_back(pid);
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <class M, class V, class E, class F, class P>
CINO_INLINE
void mark6vertices(const Hexmesh<M,V,E,F,P>                   & m,
                   const uint                                   pid,
                         std::vector<uint>                    & vertices,
                         std::vector<bool>                    & transition_verts,
                         std::vector<uint>                    & poly_verts_id,
                         std::unordered_map<uint, SchemeInfo> & poly2scheme,
                         std::vector<uint>                    & changed_pid,
                         std::vector<uint>                    & changed_vid){

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
    else{ // 6B, 6C
        for (auto vid: poly_verts_id) if(transition_verts[vid]==false) changed_vid.push_back(vid);

        changed_pid.push_back(pid);
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
    std::vector<uint> changed_pid;
    std::vector<uint> changed_vid;

    bool added_newverts = true;

    // PROFILING
    // tstart = start()
    // time = stop(tstart)
    // var_find_min_max += time

    std::vector<uint> polys;

    for(uint i=0;i<m_in.num_polys();i++) polys.push_back(i);

    while(added_newverts){

        for (auto pid: polys){
            std::vector<uint> vertices;
            std::vector<uint> poly_verts_id = m_in.poly_verts_id(pid);

            for(uint vid: poly_verts_id) if(transition_verts[vid]) vertices.push_back(vid);

            SchemeInfo info;

            switch (vertices.size()){
                case 2: mark2vertices(m_in, pid, vertices, transition_verts, poly_verts_id, poly2scheme, changed_pid, changed_vid);
                        break;
                case 3: mark3vertices(m_in, pid, vertices, transition_verts, poly_verts_id, poly2scheme, changed_pid, changed_vid);
                        break;
                case 4: mark4vertices(m_in, pid, vertices, transition_verts, poly_verts_id, poly2scheme, changed_pid, changed_vid);
                        break;
                case 5: mark5vertices(m_in, pid, vertices, transition_verts, poly_verts_id, changed_pid, changed_vid);
                        break;
                case 6: mark6vertices(m_in, pid, vertices, transition_verts, poly_verts_id, poly2scheme, changed_pid, changed_vid);
                        break;
                case 7: for (auto vid: poly_verts_id) if(transition_verts[vid]==false) changed_vid.push_back(vid);
                        changed_pid.push_back(pid);
                        break;
                case 8: info.type = HexTransition::FULL;
                        info.scale = m_in.edge_length(m_in.adj_p2e(pid)[0]);
                        poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
                        break;
            }

        }


        std::cout<<"DIOO"<<std::endl;

        if(changed_pid.size() > 0){
            added_newverts=true;
            for(auto vid: changed_vid) transition_verts[vid] = true;

            //momentaneo
            m_out = m_in;

            //reset of the auxiliar vector
            changed_vid.clear();
            changed_pid.clear();
            poly2scheme.clear();
        }
        else{
            merge_schemes_into_mesh(m_out, poly2scheme);

            std::vector<uint> polys_to_remove;

            for (auto p: poly2scheme) polys_to_remove.push_back(p.first);

            m_out.polys_remove(polys_to_remove);

            added_newverts=false;
        }




        /*
        //insert all the neighboors of each pid
        polys.clear();
        for(auto c_pid: changed_pid){
            for(auto poly_id: m_in.adj_p2p(c_pid)){ //non sono tutti i neighbors (da aggiustare)
                polys.push_back(poly_id);
            }
        }
        */
    }
}


}
