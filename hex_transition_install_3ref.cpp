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
CINO_INLINE
uint verts_in_common(std::vector<uint> a,
                     std::vector<uint> b){
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    uint conta_verts=0;

    for(auto el : a)
        for(auto el2 : b)
            if(el == el2) conta_verts++;

    return conta_verts;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class F, class P>
CINO_INLINE
bool is_3_faces_angle(const std::vector<uint>   a,
                      const uint                 pid,
                      const Hexmesh<M,V,E,F,P> & m,
                      const std::vector<uint>  & transition_faces){
    bool is_3_faces_angle = false;

    std::vector<uint> faces;
    for (auto face_id : transition_faces){
        for(auto &fid : m.poly_faces_id(pid)){
            if(fid == face_id)
                if(verts_in_common(a, m.face_verts_id(fid)) == 4)
                    faces.push_back(fid);
        }
    }

    if(faces.size() == 3){
        if(m.faces_are_adjacent(faces[0], faces[1]) && m.faces_are_adjacent(faces[0], faces[2]) && m.faces_are_adjacent(faces[1], faces[2]))
            is_3_faces_angle=true;
    }

    return is_3_faces_angle;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class F, class P>
CINO_INLINE
bool is_angle(const std::vector<uint>   a,
              const uint                 pid,
              const Hexmesh<M,V,E,F,P> & m,
              const std::vector<uint>  & transition_faces){
    std::vector<uint> faces;
    bool is_angle = false;

    for (auto face_id : transition_faces){
        for(auto &fid : m.poly_faces_id(pid)){
            if(fid == face_id)
                if(verts_in_common(a, m.face_verts_id(fid)) == 4)
                    faces.push_back(fid);
        }
    }


    if(faces.size() == 2)
        if(m.faces_are_adjacent(faces[0], faces[1]))
            is_angle=true;

    return is_angle;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class F, class P>
CINO_INLINE
bool is_face(const std::vector<uint>   a,
             const uint                 pid,
             const Hexmesh<M,V,E,F,P> & m,
             const std::vector<uint>   transition_faces){
    std::vector<uint> faces;
    bool is_face = false;

    for (auto face_id : transition_faces){
        for(auto fid : m.poly_faces_id(pid)){
            if(fid == face_id)
                if(verts_in_common(a, m.face_verts_id(fid)) == 4)
                    faces.push_back(fid);
        }
    }


    if(faces.size() >= 1)
        is_face=true;

    return is_face;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class F, class P>
CINO_INLINE
uint how_many_faces(const std::vector<uint>   a,
                    const uint                 pid,
                    const Hexmesh<M,V,E,F,P> & m,
                    const std::vector<uint>   transition_faces){
    std::vector<uint> faces;

    for (auto face_id : transition_faces){
        for(auto fid : m.poly_faces_id(pid)){
            if(fid == face_id)
                if(verts_in_common(a, m.face_verts_id(fid)) == 4)
                    faces.push_back(fid);
        }
    }

    return faces.size();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class F, class P>
CINO_INLINE
void mark_face(const Hexmesh<M,V,E,F,P>    & m,
               const uint                    pid,
               SchemeInfo                  & info,
               const std::vector<uint>      transition_faces){

    std::vector<uint> faces;
    std::vector<vec3d> poly_verts = m.poly_verts(pid);
    vec3d min = *std::min_element(poly_verts.begin(), poly_verts.end());
    vec3d max = *std::max_element(poly_verts.begin(), poly_verts.end());

    info.type = HexTransition::FACE;
    info.scale = m.edge_length(m.adj_p2e(pid)[0]);



    for (auto face_id : transition_faces) for(auto fid : m.poly_faces_id(pid)) if(fid == face_id) faces.push_back(fid);


    //select the right orientation to apply
    int conta_min = 0, conta_max = 0, conta_left = 0, conta_right = 0, conta_back = 0, conta_front = 0;

    //maybe is needed to do more tests
    if(faces.size()>1){
        uint fid = faces[faces.size()-1];
        faces.clear();
        faces.push_back(fid);
    }

    if(faces.size() == 1){
        for(auto vid : m.face_verts_id(faces[0])){
            if(m.vector_verts()[vid].y() == min.y()){
                conta_min ++;
                if(m.vector_verts()[vid].x() == min.x()) conta_left ++;
                else if(m.vector_verts()[vid].x() == max.x()) conta_right ++;

                if(m.vector_verts()[vid].z() == min.z()) conta_front ++;
                else if(m.vector_verts()[vid].z() == max.z()) conta_back ++;

            }
            else if(m.vector_verts()[vid].y() == max.y()){
                conta_max ++;
                if(m.vector_verts()[vid].x() == min.x()) conta_left ++;
                else if(m.vector_verts()[vid].x() == max.x()) conta_right ++;

                if(m.vector_verts()[vid].z() == min.z()) conta_front ++;
                else if(m.vector_verts()[vid].z() == max.z()) conta_back ++;
            }
        }
    }



    if(conta_max == 4) info.orientations.push_back(0);
    else if (conta_min == 4) info.orientations.push_back(7);
    else /*if (conta_min == 2 && conta_max == 2)*/{
        if(conta_left == 4) info.orientations.push_back(2);
        else if (conta_right == 4) info.orientations.push_back(1);
        else if (conta_front == 4) info.orientations.push_back(6);
        else if (conta_back == 4) info.orientations.push_back(5);
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class F, class P>
CINO_INLINE
void mark_two_adj_faces(const Hexmesh<M,V,E,F,P>    & m,
                        const uint                    pid,
                        SchemeInfo                  & info,
                        const std::vector<uint>      transition_faces){

    std::vector<uint> faces;
    std::vector<vec3d> poly_verts = m.poly_verts(pid);
    vec3d min = *std::min_element(poly_verts.begin(), poly_verts.end());
    vec3d max = *std::max_element(poly_verts.begin(), poly_verts.end());


    info.type = HexTransition::TWO_ADJ_FACES;
    info.scale = m.edge_length(m.adj_p2e(pid)[0]);

    for (auto face_id : transition_faces){
        for(auto fid : m.poly_faces_id(pid)){
            if(fid == face_id)
                faces.push_back(fid);
        }
    }


    int conta_max_1 = 0, conta_max_2 = 0, conta_min_1 = 0, conta_min_2 = 0;
    int conta_left_1 = 0, conta_left_2 = 0, conta_right_1 = 0, conta_right_2 = 0;
    int conta_back_1 = 0, conta_back_2 = 0, conta_front_1 = 0, conta_front_2 = 0;

    if(faces.size() == 2){
        for(uint i=0; i<faces.size(); i++){
            for(auto v : m.face_verts(faces[i])){
                if (i == 0){
                    if(v.y() == max.y()) conta_max_1 ++;
                    else if(v.y() == min.y()) conta_min_1 ++;

                    if(v.x() == max.x()) conta_right_1 ++;
                    else if(v.x() == min.x()) conta_left_1 ++;

                    if(v.z() == max.z()) conta_back_1 ++;
                    else if(v.z() == min.z()) conta_front_1 ++;
                }
                else {
                    if(v.y() == max.y()) conta_max_2 ++;
                    else if(v.y() == min.y()) conta_min_2 ++;

                    if(v.x() == max.x()) conta_right_2 ++;
                    else if(v.x() == min.x()) conta_left_2 ++;

                    if(v.z() == max.z()) conta_back_2 ++;
                    else if(v.z() == min.z()) conta_front_2 ++;
                }
            }
        }
    }


    if( (conta_max_1 == 4 || conta_max_2 == 4) &&
        (conta_min_1 == 2 || conta_min_2 == 2) &&
        (conta_front_1 ==4 || conta_front_2 == 4) &&
        (conta_back_1 + conta_back_2) <= 2) info.orientations.push_back(0);

    if( (conta_max_1 == 2 || conta_max_2 == 2) &&
        (conta_min_1 == 4 || conta_min_2 == 4) &&
        (conta_back_1 + conta_back_2) <= 2 &&
        (conta_front_1 == 4 || conta_front_2 == 4)) info.orientations.push_back(1);

    if( (conta_max_1 == 4 || conta_max_2 == 4) &&
        (conta_min_1 == 2 || conta_min_2 == 2) &&
        (conta_back_1 == 4 || conta_back_2 == 4) &&
        (conta_front_1 + conta_front_2) <= 2) info.orientations.push_back(2);

    if( (conta_max_1 == 4 || conta_max_2 == 4) &&
        (conta_min_1 == 2 || conta_min_2 == 2) &&
        (conta_right_1 == 4 || conta_right_2 == 4) &&
        (conta_left_1 + conta_left_2) <= 2) info.orientations.push_back(3);

    if( (conta_max_1 == 4 || conta_max_2 == 4) &&
        (conta_min_1 == 2 || conta_min_2 == 2) &&
        (conta_left_1 == 4 || conta_left_2 == 4) &&
        (conta_right_1 + conta_right_2) <= 2) info.orientations.push_back(4);

    if( (conta_max_1 == 2 || conta_max_2 == 2) &&
        (conta_min_1 == 4 || conta_min_2 == 4) &&
        (conta_back_1 == 4 || conta_back_2 == 4) &&
        (conta_front_1 + conta_front_2) <= 2) info.orientations.push_back(5);

    if( (conta_max_1 == 2 || conta_max_2 == 2) &&
        (conta_min_1 == 4 || conta_min_2 == 4) &&
        (conta_right_1 == 4 || conta_right_2 == 4) &&
        (conta_left_1 + conta_left_2) <= 2) info.orientations.push_back(6);

    if( (conta_max_1 == 2 || conta_max_2 == 2) &&
        (conta_min_1 == 4 || conta_min_2 == 4) &&
        (conta_left_1 == 4 || conta_left_2 == 4) &&
        (conta_right_1 + conta_right_2) <= 2) info.orientations.push_back(7);

    if( (conta_left_1 == 4 || conta_left_2 == 4) &&
        (conta_back_1 == 4 || conta_back_2 == 4) &&
        (conta_front_1 + conta_front_2) <= 2) info.orientations.push_back(8);

    if( (conta_right_1 == 4 || conta_right_2 == 4) &&
        (conta_back_1 == 4 || conta_back_2 == 4) &&
        (conta_front_1 + conta_front_2) <= 2) info.orientations.push_back(9);

    if( (conta_right_1 == 4 || conta_right_2 == 4) &&
        (conta_front_1 == 4 || conta_front_2 == 4) &&
        (conta_back_1 + conta_back_2) <= 2) info.orientations.push_back(10);

    if( (conta_left_1 == 4 || conta_left_2 == 4) &&
        (conta_front_1 == 4 || conta_front_2 == 4) &&
        (conta_back_1 + conta_back_2) <= 2) info.orientations.push_back(11);

}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class F, class P>
CINO_INLINE
void mark_transition(const Hexmesh<M,V,E,F,P>    & m,
                     const uint                    pid,
                     SchemeInfo                  & info,
                     const std::vector<uint>     transition_faces){

    std::vector<uint> faces;
    std::vector<vec3d> poly_verts = m.poly_verts(pid);
    vec3d min = *std::min_element(poly_verts.begin(), poly_verts.end());
    vec3d max = *std::max_element(poly_verts.begin(), poly_verts.end());


    info.type = HexTransition::TRANSITION;
    info.scale = m.edge_length(m.adj_p2e(pid)[0]);

    for (auto face_id : transition_faces){
        for(auto fid : m.poly_faces_id(pid)){
            if(fid == face_id)
                faces.push_back(fid);
        }
    }

    int conta_y_s = 0, conta_y_g = 0, conta_x_s = 0, conta_x_g = 0, conta_z_s = 0, conta_z_g = 0;

    if(faces.size() == 2){
        for(auto v : m.face_verts(faces[0])){
            if(v.y() == max.y()) conta_y_s ++;
            else if(v.y() == min.y()) conta_y_g ++;

            if(v.x() == max.x()) conta_x_s ++;
            else if(v.x() == min.x()) conta_x_g ++;

            if(v.z() == max.z()) conta_z_s ++;
            else if(v.z() == min.z()) conta_z_g ++;
        }
    }

    if(conta_y_s == 4 || conta_y_g == 4) info.orientations.push_back(0);
    else if(conta_x_s == 4 || conta_x_g == 4) info.orientations.push_back(1);
    else if(conta_z_s == 4 || conta_z_g == 4) info.orientations.push_back(2);
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class F, class P>
CINO_INLINE
void mark_three_faces(const Hexmesh<M,V,E,F,P>    & m,
                      const uint                    pid,
                      SchemeInfo                  & info,
                      const std::vector<uint>       transition_faces,
                      const bool                    sharing_vertex){

    std::vector<uint> faces;
    std::vector<vec3d> poly_verts = m.poly_verts(pid);
    vec3d min = *std::min_element(poly_verts.begin(), poly_verts.end());
    vec3d max = *std::max_element(poly_verts.begin(), poly_verts.end());

    for (auto face_id : transition_faces) for(auto fid : m.poly_faces_id(pid)) if(fid == face_id) faces.push_back(fid);

    info.scale = m.edge_length(m.adj_p2e(pid)[0]);

    std::vector<uint> f1 = m.face_verts_id(faces[0]);
    std::vector<uint> f2 = m.face_verts_id(faces[1]);
    std::vector<uint> f3 = m.face_verts_id(faces[2]);


    int conta_min = 0, conta_max = 0, conta_left = 0, conta_right = 0, conta_back = 0, conta_front = 0;

    if(sharing_vertex){
        info.type = HexTransition::THREE_ADJ_FACES_SHARING_VERTEX;

        std::sort(f1.begin(), f1.end());
        std::sort(f2.begin(), f2.end());
        std::sort(f3.begin(), f3.end());

        std::vector<uint> f12;
        std::vector<uint> f123;

        vec3d common_v;

        std::set_intersection(f1.begin(), f1.end(), f2.begin(), f2.end(), std::back_inserter(f12));
        std::set_intersection(f12.begin(), f12.end(), f3.begin(), f3.end(), std::back_inserter(f123));

        if(f123.size() == 1) common_v = m.vert(f123[0]);


        if(common_v.y() == min.y()){
            conta_min ++;
            if(common_v.x() == min.x()) conta_left++;
            else if(common_v.x() == max.x()) conta_right++;

            if(common_v.z() == min.z()) conta_front++;
            else if(common_v.z() == max.z()) conta_back++;
        }

        if(common_v.y() == max.y()){
            conta_max ++;

            if(common_v.x() == min.x()) conta_left++;
            else if(common_v.x() == max.x()) conta_right++;

            if(common_v.z() == min.z()) conta_front++;
            else if(common_v.z() == max.z()) conta_back++;
        }


        if(conta_min == 1){
            if(conta_left == 1 && conta_back==1) info.orientations.push_back(0);
            else if(conta_left == 1 && conta_front==1) info.orientations.push_back(1);
            else if(conta_right == 1 && conta_back==1) info.orientations.push_back(2);
            else if(conta_right == 1 && conta_front==1) info.orientations.push_back(3);
        }
        else if(conta_max == 1){
            if(conta_left == 1 && conta_back==1) info.orientations.push_back(4);
            else if(conta_left == 1 && conta_front==1) info.orientations.push_back(5);
            else if(conta_right == 1 && conta_back==1) info.orientations.push_back(6);
            else if(conta_right == 1 && conta_front==1) info.orientations.push_back(7);
        }
    }
    else{
        info.type = HexTransition::THREE_ADJ_FACES;
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
                                 const std::vector<bool>            & transition_verts,
                                 const std::vector<uint>            & transition_faces,
                                       Hexmesh<M,V,E,F,P>           & m_out){
    m_out = m_in;

    std::unordered_map<uint, SchemeInfo> poly2scheme;
    std::vector<std::vector<uint>> verts_on_face;

    for (auto fid : transition_faces) verts_on_face.push_back(m_in.face_verts_id(fid));


    // PROFILING
    // tstart = start()
    // time = stop(tstart)
    // var_find_min_max += time

    for (uint pid=0; pid<m_in.num_polys(); pid++){
        std::vector<uint> scheme_vids;

        std::vector<std::vector<uint>> verts_face;
        for(uint vid: m_in.poly_verts_id(pid)) if(transition_verts[vid]) scheme_vids.push_back(vid);


        if(scheme_vids.size()>0) verts_face.push_back(scheme_vids);
        bool insertFace = false, insertAngle = false, insertTransition=false, insert_full=false, insert3faces = false, insert3facesv=false;


        //Select the right scheme to apply
        for (auto el2 : verts_on_face){
            for (auto el : verts_face){
                if(verts_in_common(el, el2) == 4) {
                    uint verts_common = verts_in_common(el, m_in.poly_verts_id(pid));
                    bool is_an_angle = is_angle(el, pid, m_in, transition_faces);
                    if(scheme_vids.size() > 6 && (verts_common == 8 || verts_common == 7)){
                        uint faces = how_many_faces(el, pid, m_in, transition_faces);

                        switch (faces){
                            case 2: if(is_an_angle) insertAngle = true;
                                    else insertTransition = true;
                                    break;
                            case 3: if(is_3_faces_angle(el, pid, m_in, transition_faces))insert3facesv=true;
                                    else insert3faces = true;
                                    break;
                            case 4: break;
                            case 6: if(is_an_angle) insertAngle = true;
                                    else insert_full = true;
                                    break;
                            default: insertFace = true;
                        }
                    }
                    else if(scheme_vids.size() > 5){
                        if(is_an_angle)
                            insertAngle = true;
                        else insertFace = true;
                    }
                    else insertFace = true;
                 }
            }
        }


        if (insertFace){ //FACE
            SchemeInfo info;

            mark_face(m_in, pid, info, transition_faces);

            poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
        }
        else if (insertAngle){ //TWO_ADJ_FACES
            SchemeInfo info;

            mark_two_adj_faces(m_in, pid, info, transition_faces);

            poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
        }
        else if (insertTransition){ //TRANSITION
            SchemeInfo info;

            mark_transition(m_in, pid, info, transition_faces);

            poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
        }
        else if(insert3faces){
            SchemeInfo info;

            mark_three_faces(m_in, pid, info, transition_faces, false);

            poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
        }
        else if(insert3facesv){
            SchemeInfo info;

            mark_three_faces(m_in, pid, info, transition_faces, true);

            poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
        }
        else if (insert_full){ //FULL (no need orientation)
            SchemeInfo info;
            info.type = HexTransition::FULL;
            info.scale = m_in.edge_length(m_in.adj_p2e(pid)[0]);
            poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
        }

        if (pid%500==0) std::cout<< "pid : " << pid << " [ " << (pid * 100)/ (m_in.num_polys()) << "% ]" <<std::endl;
    }

    merge_schemes_into_mesh(m_out, poly2scheme);

    std::vector<uint> polys_to_remove;

    for (auto p: poly2scheme) polys_to_remove.push_back(p.first);

    m_out.polys_remove(polys_to_remove);
}


}
