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
struct vert_compare {
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
void setOrientationInfo1(SchemeInfo                  & info,
                         std::vector<bool>           & transition_verts,
                         std::vector<uint>           & poly_verts_id){


    std::vector<bool> mask_a = {true, false, false, false, false, false, false, false};
    std::vector<bool> mask_b = {false, true, false, false, false, false, false, false};
    std::vector<bool> mask_c = {false, false, true, false, false, false, false, false};
    std::vector<bool> mask_d = {false, false, false, true, false, false, false, false};
    std::vector<bool> mask_e = {false, false, false, false, true, false, false, false};
    std::vector<bool> mask_f = {false, false, false, false, false, true, false, false};
    std::vector<bool> mask_g = {false, false, false, false, false, false, true, false};
    std::vector<bool> mask_h = {false, false, false, false, false, false, false, true};

    std::vector<bool> mask;

    for (auto vid: poly_verts_id) mask.push_back(transition_verts[vid]);

    if (mask == mask_a) info.orientations.push_back(0);
    else if (mask == mask_b) info.orientations.push_back(1);
    else if (mask == mask_c) info.orientations.push_back(2);
    else if (mask == mask_d) info.orientations.push_back(3);
    else if (mask == mask_e) info.orientations.push_back(4);
    else if (mask == mask_f) info.orientations.push_back(5);
    else if (mask == mask_g) info.orientations.push_back(6);
    else if (mask == mask_h) info.orientations.push_back(7);
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
CINO_INLINE
void setOrientationInfo2(SchemeInfo                  & info,
                         std::vector<bool>           & transition_verts,
                         std::vector<uint>           & poly_verts_id){


    std::vector<bool> mask_a = {true, false, false, true, false, false, false, false};
    std::vector<bool> mask_b = {false, true, true, false, false, false, false, false};
    std::vector<bool> mask_c = {true, true, false, false, false, false, false, false};
    std::vector<bool> mask_d = {false, false, true, true, false, false, false, false};
    std::vector<bool> mask_e = {false, false, false, false, true, false, false, true};
    std::vector<bool> mask_f = {false, false, false, false, false, true, true, false};
    std::vector<bool> mask_g = {false, false, false, false, true, true, false, false};
    std::vector<bool> mask_h = {false, false, false, false, false, false, true, true};
    std::vector<bool> mask_i = {true, false, false, false, true, false, false, false};
    std::vector<bool> mask_l = {false, false, false, true, false, false, false, true};
    std::vector<bool> mask_m = {false, true, false, false, false, true, false, false};
    std::vector<bool> mask_n = {false, false, true, false, false, false, true, false};

    std::vector<bool> mask;

    for (auto vid: poly_verts_id) mask.push_back(transition_verts[vid]);

    if (mask == mask_a) info.orientations.push_back(0);
    else if (mask == mask_b) info.orientations.push_back(1);
    else if (mask == mask_c) info.orientations.push_back(2);
    else if (mask == mask_d) info.orientations.push_back(3);
    else if (mask == mask_e) info.orientations.push_back(4);
    else if (mask == mask_f) info.orientations.push_back(5);
    else if (mask == mask_g) info.orientations.push_back(6);
    else if (mask == mask_h) info.orientations.push_back(7);
    else if (mask == mask_i) info.orientations.push_back(8);
    else if (mask == mask_l) info.orientations.push_back(9);
    else if (mask == mask_m) info.orientations.push_back(10);
    else if (mask == mask_n) info.orientations.push_back(11);

}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
CINO_INLINE
void setOrientationInfo3(SchemeInfo                  & info,
                         std::vector<bool>           & transition_verts,
                         std::vector<uint>           & poly_verts_id){


    std::vector<bool> mask_a = {true, true, false, true, false, false, false, false};
    std::vector<bool> mask_b = {true, true, true, false, false, false, false, false};
    std::vector<bool> mask_c = {true, false, true, true, false, false, false, false};
    std::vector<bool> mask_d = {false, true, true, true, false, false, false, false};
    std::vector<bool> mask_e = {false, false, false, false, true, true, false, true};
    std::vector<bool> mask_f = {false, false, false, false, true, true, true, false};
    std::vector<bool> mask_g = {false, false, false, false, true, false, true, true};
    std::vector<bool> mask_h = {false, false, false, false, false, true, true, true};
    std::vector<bool> mask_i = {true, true, false, false, true, false, false, false};
    std::vector<bool> mask_j = {true, false, false, false, true, true, false, false};
    std::vector<bool> mask_k = {true, true, false, false, false, true, false, false};
    std::vector<bool> mask_l = {false, true, false, false, true, true, false, false};
    std::vector<bool> mask_m = {false, false, true, true, false, false, false, true};
    std::vector<bool> mask_n = {false, false, false, true, false, false, true, true};
    std::vector<bool> mask_o = {false, false, true, true, false, false, true, false};
    std::vector<bool> mask_p = {false, false, true, false, false, false, true, true};
    std::vector<bool> mask_q = {false, true, true, false, false, false, true, false};
    std::vector<bool> mask_r = {false, false, true, false, false, true, true, false};
    std::vector<bool> mask_s = {false, true, true, false, false, true, false, false};
    std::vector<bool> mask_t = {false, true, false, false, false, true, true, false};
    std::vector<bool> mask_u = {true, false, false, true, false, false, false, true};
    std::vector<bool> mask_v = {false, false, false, true, true, false, false, true};
    std::vector<bool> mask_w = {true, false, false, true, true, false, false, false};
    std::vector<bool> mask_x = {true, false, false, false, true, false, false, true};

    std::vector<bool> mask;

    for (auto vid: poly_verts_id) mask.push_back(transition_verts[vid]);

    if (mask == mask_a) info.orientations.push_back(0);
    else if (mask == mask_b) info.orientations.push_back(1);
    else if (mask == mask_c) info.orientations.push_back(2);
    else if (mask == mask_d) info.orientations.push_back(3);
    else if (mask == mask_e) info.orientations.push_back(4);
    else if (mask == mask_f) info.orientations.push_back(5);
    else if (mask == mask_g) info.orientations.push_back(6);
    else if (mask == mask_h) info.orientations.push_back(7);
    else if (mask == mask_i) info.orientations.push_back(8);
    else if (mask == mask_j) info.orientations.push_back(9);
    else if (mask == mask_k) info.orientations.push_back(10);
    else if (mask == mask_l) info.orientations.push_back(11);
    else if (mask == mask_m) info.orientations.push_back(12);
    else if (mask == mask_n) info.orientations.push_back(13);
    else if (mask == mask_o) info.orientations.push_back(14);
    else if (mask == mask_p) info.orientations.push_back(15);
    else if (mask == mask_q) info.orientations.push_back(16);
    else if (mask == mask_r) info.orientations.push_back(17);
    else if (mask == mask_s) info.orientations.push_back(18);
    else if (mask == mask_t) info.orientations.push_back(19);
    else if (mask == mask_u) info.orientations.push_back(20);
    else if (mask == mask_v) info.orientations.push_back(21);
    else if (mask == mask_w) info.orientations.push_back(22);
    else if (mask == mask_x) info.orientations.push_back(23);

}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
CINO_INLINE
void setOrientationInfo4A(SchemeInfo                  & info,
                          std::vector<bool>           & transition_verts,
                          std::vector<uint>           & poly_verts_id){


    std::vector<bool> mask_a = {true, true, true, true, false, false, false, false};
    std::vector<bool> mask_b = {false, true, true, false, false, true, true, false};
    std::vector<bool> mask_c = {true, false, false, true, true, false, false, true};
    std::vector<bool> mask_d = {true, true, false, false, true, true, false, false};
    std::vector<bool> mask_e = {false, false, true, true, false, false, true, true};
    std::vector<bool> mask_f = {false, false, false, false, true, true, true, true};


    std::vector<bool> mask;

    for (auto vid: poly_verts_id) mask.push_back(transition_verts[vid]);

    if (mask == mask_a) info.orientations.push_back(0);
    else if (mask == mask_b) info.orientations.push_back(1);
    else if (mask == mask_c) info.orientations.push_back(2);
    else if (mask == mask_d) info.orientations.push_back(3);
    else if (mask == mask_e) info.orientations.push_back(4);
    else if (mask == mask_f) info.orientations.push_back(5);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
CINO_INLINE
void setOrientationInfo4B(SchemeInfo                  & info,
                          std::vector<bool>           & transition_verts,
                          std::vector<uint>           & poly_verts_id){


    std::vector<bool> mask_a = {true, true, false, true, true, false, false, false};
    std::vector<bool> mask_b = {true, true, true, false, false, true, false, false};
    std::vector<bool> mask_c = {false, true, true, true, false, false, true, false};
    std::vector<bool> mask_d = {true, false, true, true, false, false, false, true};
    std::vector<bool> mask_e = {false, false, false, true, true, false, true, true};
    std::vector<bool> mask_f = {true, false, false, false, true, true, false, true};
    std::vector<bool> mask_g = {false, true, false, false, true, true, true, false};
    std::vector<bool> mask_h = {false, false, true, false, false, true, true, true};

    std::vector<bool> mask;

    for (auto vid: poly_verts_id) mask.push_back(transition_verts[vid]);

    if (mask == mask_a) info.orientations.push_back(0);
    else if (mask == mask_b) info.orientations.push_back(1);
    else if (mask == mask_c) info.orientations.push_back(2);
    else if (mask == mask_d) info.orientations.push_back(3);
    else if (mask == mask_e) info.orientations.push_back(4);
    else if (mask == mask_f) info.orientations.push_back(5);
    else if (mask == mask_g) info.orientations.push_back(6);
    else if (mask == mask_h) info.orientations.push_back(7);

}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
CINO_INLINE
void setOrientationInfo4C(SchemeInfo                  & info,
                          std::vector<bool>           & transition_verts,
                          std::vector<uint>           & poly_verts_id){

    std::vector<bool> mask_a = {true, true, false, true, false, true, false, false};
    std::vector<bool> mask_b = {true, true, true, false, false, false, true, false};
    std::vector<bool> mask_c = {false, true, true, true, false, false, false, true};
    std::vector<bool> mask_d = {true, false, true, true, true, false, false, false};
    std::vector<bool> mask_e = {true, false, false, false, true, true, true, false};
    std::vector<bool> mask_f = {false, true, false, false, false, true, true, true};
    std::vector<bool> mask_g = {false, false, true, false, true, false, true, true};
    std::vector<bool> mask_h = {false, false, false, true, true, true, false, true};
    std::vector<bool> mask_i = {true, true, false, false, true, false, false, true};
    std::vector<bool> mask_l = {false, false, false, true, true, true, false, true};
    std::vector<bool> mask_m = {true, false, false, true, false, false, true, true};
    std::vector<bool> mask_n = {true, false, true, true, true, false, false, false};
    std::vector<bool> mask_o = {false, true, true, false, true, true, false, false};
    std::vector<bool> mask_p = {false, true, false, false, false, true, true, true};
    std::vector<bool> mask_q = {false, false, true, true, false, true, true, false};
    std::vector<bool> mask_r = {true, true, true, false, false, false, true, false};
    std::vector<bool> mask_s = {true, false, false, false, true, true, true, false};
    std::vector<bool> mask_t = {false, true, true, false, true, true, false, false};
    std::vector<bool> mask_u = {true, true, false, true, false, true, false, false};
    std::vector<bool> mask_v = {true, true, false, false, true, false, false, true};
    std::vector<bool> mask_w = {false, true, true, true, false, false, false, true};
    std::vector<bool> mask_x = {false, false, true, true, false, true, true, false};
    std::vector<bool> mask_y = {false, false, true, false, true, false, true, true};
    std::vector<bool> mask_z = {true, false, false, true, false, false, true, true};
    std::vector<bool> mask_a1 = {true, true, false, true, false, false, false, true};
    std::vector<bool> mask_b1 = {true, true, true, false, true, false, false, false};
    std::vector<bool> mask_c1 = {false, true, true, true, false, true, false, false};
    std::vector<bool> mask_d1 = {true, false, true, true, false, false, true, false};
    std::vector<bool> mask_e1 = {true, false, false, true, true, true, false, false};
    std::vector<bool> mask_f1 = {true, true, false, false, false, true, true, false};
    std::vector<bool> mask_g1 = {false, true, true, false, false, false, true, true};
    std::vector<bool> mask_h1 = {false, false, true, true, true, false, false, true};
    std::vector<bool> mask_i1 = {true, false, false, false, true, false, true, true};
    std::vector<bool> mask_l1 = {false, false, true, true, true, false, false, true};
    std::vector<bool> mask_m1 = {true, true, false, true, false, false, false, true};
    std::vector<bool> mask_n1 = {true, false, false, true, true, true, false, false};
    std::vector<bool> mask_o1 = {false, true, true, true, false, true, false, false};
    std::vector<bool> mask_p1 = {true, true, false, false, false, true, true, false};
    std::vector<bool> mask_q1 = {false, false, true, false, true, true, true, false};
    std::vector<bool> mask_r1 = {false, true, true, false, false, false, true, true};
    std::vector<bool> mask_s1 = {true, false, false, true, true, true, false, false};
    std::vector<bool> mask_t1 = {true, true, true, false, true, false, false, false};
    std::vector<bool> mask_u1 = {true, true, false, false, false, true, true, false};
    std::vector<bool> mask_v1 = {false, true, false, false, true, true, false, true};
    std::vector<bool> mask_w1 = {false, false, true, true, true, false, false, true};
    std::vector<bool> mask_x1 = {false, false, false, true, false, true, true, true};
    std::vector<bool> mask_y1 = {false, true, true, false, false, false, true, true};
    std::vector<bool> mask_z1 = {true, false, true, true, false, false, true, false};


    std::vector<bool> mask;

    for (auto vid: poly_verts_id) mask.push_back(transition_verts[vid]);


    if(mask == mask_a || mask == mask_b || mask == mask_c || mask == mask_d || mask == mask_e || mask == mask_f ||
       mask == mask_g || mask == mask_h || mask == mask_i || mask == mask_l || mask == mask_m || mask == mask_n ||
       mask == mask_o || mask == mask_p || mask == mask_q || mask == mask_r || mask == mask_s || mask == mask_t ||
       mask == mask_u || mask == mask_v || mask == mask_w || mask == mask_x || mask == mask_y || mask == mask_z){

        info.type = HexTransition::CORNER_4CA;
        info.flag = 0;

        if (mask == mask_a) info.orientations.push_back(0);
        else if (mask == mask_b) info.orientations.push_back(1);
        else if (mask == mask_c) info.orientations.push_back(2);
        else if (mask == mask_d) info.orientations.push_back(3);
        else if (mask == mask_e) info.orientations.push_back(4);
        else if (mask == mask_f) info.orientations.push_back(5);
        else if (mask == mask_g) info.orientations.push_back(6);
        else if (mask == mask_h) info.orientations.push_back(7);
        else if (mask == mask_i) info.orientations.push_back(8);
        else if (mask == mask_l) info.orientations.push_back(9);
        else if (mask == mask_m) info.orientations.push_back(10);
        else if (mask == mask_n) info.orientations.push_back(11);
        else if (mask == mask_o) info.orientations.push_back(12);
        else if (mask == mask_p) info.orientations.push_back(13);
        else if (mask == mask_q) info.orientations.push_back(14);
        else if (mask == mask_r) info.orientations.push_back(15);
        else if (mask == mask_s) info.orientations.push_back(16);
        else if (mask == mask_t) info.orientations.push_back(17);
        else if (mask == mask_u) info.orientations.push_back(18);
        else if (mask == mask_v) info.orientations.push_back(19);
        else if (mask == mask_w) info.orientations.push_back(20);
        else if (mask == mask_x) info.orientations.push_back(21);
        else if (mask == mask_y) info.orientations.push_back(22);
        else if (mask == mask_z) info.orientations.push_back(23);
    }
    else{
        info.type = HexTransition::CORNER_4CB;
        info.flag = 1;

        if (mask == mask_a1) info.orientations.push_back(0);
        else if (mask == mask_b1) info.orientations.push_back(1);
        else if (mask == mask_c1) info.orientations.push_back(2);
        else if (mask == mask_d1) info.orientations.push_back(3);
        else if (mask == mask_e1) info.orientations.push_back(4);
        else if (mask == mask_f1) info.orientations.push_back(5);
        else if (mask == mask_g1) info.orientations.push_back(6);
        else if (mask == mask_h1) info.orientations.push_back(7);
        else if (mask == mask_i1) info.orientations.push_back(8);
        else if (mask == mask_l1) info.orientations.push_back(9);
        else if (mask == mask_m1) info.orientations.push_back(10);
        else if (mask == mask_n1) info.orientations.push_back(11);
        else if (mask == mask_o1) info.orientations.push_back(12);
        else if (mask == mask_p1) info.orientations.push_back(13);
        else if (mask == mask_q1) info.orientations.push_back(14);
        else if (mask == mask_r1) info.orientations.push_back(15);
        else if (mask == mask_s1) info.orientations.push_back(16);
        else if (mask == mask_t1) info.orientations.push_back(17);
        else if (mask == mask_u1) info.orientations.push_back(18);
        else if (mask == mask_v1) info.orientations.push_back(19);
        else if (mask == mask_w1) info.orientations.push_back(20);
        else if (mask == mask_x1) info.orientations.push_back(21);
        else if (mask == mask_y1) info.orientations.push_back(22);
        else if (mask == mask_z1) info.orientations.push_back(23);
    }
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
CINO_INLINE
void setOrientationInfo5A(SchemeInfo                  & info,
                          std::vector<bool>           & transition_verts,
                          std::vector<uint>           & poly_verts_id){


    std::vector<bool> mask_a = {true, true, true, true, true, false, false, false};
    std::vector<bool> mask_b = {true, true, true, true, false, true, false, false};
    std::vector<bool> mask_c = {true, true, true, true, false, false, true, false};
    std::vector<bool> mask_d = {true, true, true, true, false, false, false, true};
    std::vector<bool> mask_e = {false, false, false, true, true, true, true, true};
    std::vector<bool> mask_f = {false, false, true, false, true, true, true, true};
    std::vector<bool> mask_g = {false, true, false, false, true, true, true, true};
    std::vector<bool> mask_h = {true, false, false, false, true, true, true, true};
    std::vector<bool> mask_i = {true, false, false, true, true, true, false, true};
    std::vector<bool> mask_l = {true, true, false, true, true, false, false, true};
    std::vector<bool> mask_m = {true, false, true, true, true, false, false, true};
    std::vector<bool> mask_n = {true, false, false, true, true, false, true, true};
    std::vector<bool> mask_o = {true, true, true, false, false, true, true, false};
    std::vector<bool> mask_p = {false, true, true, false, true, true, true, false};
    std::vector<bool> mask_q = {false, true, true, false, false, true, true, true};
    std::vector<bool> mask_r = {false, true, true, true, false, true, true, false};
    std::vector<bool> mask_s = {true, true, false, false, true, true, false, true};
    std::vector<bool> mask_t = {true, true, false, true, true, true, false, false};
    std::vector<bool> mask_u = {true, true, true, false, true, true, false, false};
    std::vector<bool> mask_v = {true, true, false, false, true, true, true, false};
    std::vector<bool> mask_w = {true, false, true, true, false, false, true, true};
    std::vector<bool> mask_x = {false, true, true, true, false, false, true, true};
    std::vector<bool> mask_y = {false, false, true, true, false, true, true, true};
    std::vector<bool> mask_z = {false, false, true, true, true, false, true, true};

    std::vector<bool> mask;

    for (auto vid: poly_verts_id) mask.push_back(transition_verts[vid]);

    if (mask == mask_a) info.orientations.push_back(0);
    else if (mask == mask_b) info.orientations.push_back(1);
    else if (mask == mask_c) info.orientations.push_back(2);
    else if (mask == mask_d) info.orientations.push_back(3);
    else if (mask == mask_e) info.orientations.push_back(4);
    else if (mask == mask_f) info.orientations.push_back(5);
    else if (mask == mask_g) info.orientations.push_back(6);
    else if (mask == mask_h) info.orientations.push_back(7);
    else if (mask == mask_i) info.orientations.push_back(8);
    else if (mask == mask_l) info.orientations.push_back(9);
    else if (mask == mask_m) info.orientations.push_back(10);
    else if (mask == mask_n) info.orientations.push_back(11);
    else if (mask == mask_o) info.orientations.push_back(12);
    else if (mask == mask_p) info.orientations.push_back(13);
    else if (mask == mask_q) info.orientations.push_back(14);
    else if (mask == mask_r) info.orientations.push_back(15);
    else if (mask == mask_s) info.orientations.push_back(16);
    else if (mask == mask_t) info.orientations.push_back(17);
    else if (mask == mask_u) info.orientations.push_back(18);
    else if (mask == mask_v) info.orientations.push_back(19);
    else if (mask == mask_w) info.orientations.push_back(20);
    else if (mask == mask_x) info.orientations.push_back(21);
    else if (mask == mask_y) info.orientations.push_back(22);
    else if (mask == mask_z) info.orientations.push_back(23);
}



//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
CINO_INLINE
void setOrientationInfo6(SchemeInfo                  & info,
                         std::vector<bool>           & transition_verts,
                         std::vector<uint>           & poly_verts_id){


    std::vector<bool> mask_a = {true, true, false, true, true, true, false, true};
    std::vector<bool> mask_b = {true, false, true, true, true, false, true, true};
    std::vector<bool> mask_c = {true, true, true, false, true, true, true, false};
    std::vector<bool> mask_d = {false, true, true, true, false, true, true, true};
    std::vector<bool> mask_e = {true, true, true, true, true, true, false, false};
    std::vector<bool> mask_f = {true, true, true, true, false, false, true, true};
    std::vector<bool> mask_g = {true, true, true, true, true, false, false, true};
    std::vector<bool> mask_h = {true, true, true, true, false, true, true, false};
    std::vector<bool> mask_i = {true, true, false, false, true, true, true, true};
    std::vector<bool> mask_l = {false, false, true, true, true, true, true, true};
    std::vector<bool> mask_m = {true, false, false, true, true, true, true, true};
    std::vector<bool> mask_n = {false, true, true, false, true, true, true, true};

    std::vector<bool> mask;

    for (auto vid: poly_verts_id) mask.push_back(transition_verts[vid]);

    if (mask == mask_a) info.orientations.push_back(0);
    else if (mask == mask_b) info.orientations.push_back(1);
    else if (mask == mask_c) info.orientations.push_back(2);
    else if (mask == mask_d) info.orientations.push_back(3);
    else if (mask == mask_e) info.orientations.push_back(4);
    else if (mask == mask_f) info.orientations.push_back(5);
    else if (mask == mask_g) info.orientations.push_back(6);
    else if (mask == mask_h) info.orientations.push_back(7);
    else if (mask == mask_i) info.orientations.push_back(8);
    else if (mask == mask_l) info.orientations.push_back(9);
    else if (mask == mask_m) info.orientations.push_back(10);
    else if (mask == mask_n) info.orientations.push_back(11);

}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
CINO_INLINE
void setOrientationInfo7(SchemeInfo                  & info,
                         std::vector<bool>           & transition_verts,
                         std::vector<uint>           & poly_verts_id){


    std::vector<bool> mask_a = {true, true, true, true, true, true, false, true};
    std::vector<bool> mask_b = {true, true, true, true, true, true, true, false};
    std::vector<bool> mask_c = {true, true, true, true, false, true, true, true};
    std::vector<bool> mask_d = {true, true, true, true, true, false, true, true};
    std::vector<bool> mask_e = {true, true, false, true, true, true, true, true};
    std::vector<bool> mask_f = {true, true, true, false, true, true, true, true};
    std::vector<bool> mask_g = {false, true, true, true, true, true, true, true};
    std::vector<bool> mask_h = {true, false, true, true, true, true, true, true};


    std::vector<bool> mask;

    for (auto vid: poly_verts_id) mask.push_back(transition_verts[vid]);

    if (mask == mask_a) info.orientations.push_back(0);
    else if (mask == mask_b) info.orientations.push_back(1);
    else if (mask == mask_c) info.orientations.push_back(2);
    else if (mask == mask_d) info.orientations.push_back(3);
    else if (mask == mask_e) info.orientations.push_back(4);
    else if (mask == mask_f) info.orientations.push_back(5);
    else if (mask == mask_g) info.orientations.push_back(6);
    else if (mask == mask_h) info.orientations.push_back(7);

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
                         SchemeInfo                           & info){


    int eid=-1;
    for(uint e_id : m.adj_p2e(pid))
        if (m.edge_contains_vert(e_id,vertices[0]) && m.edge_contains_vert(e_id,vertices[1])) eid = (int)e_id;


    if(eid != -1){ //2A
        info.type = HexTransition::EDGE;
        info.scale = m.edge_length(m.adj_p2e(pid)[0]);
        setOrientationInfo2(info, transition_verts, poly_verts_id);
        poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
    }
    else{ //2B or 2C

        vec3d v0 = m.vert(vertices[0]);
        vec3d v1 = m.vert(vertices[1]);

        if(v0.x() == v1.x() || v0.y() == v1.y() || v0.z() == v1.z()){ //2B
            std::cout<<"2B"<<std::endl;
            for (auto vid: poly_verts_id){
                if(v0.x() == v1.x())
                    if (m.vert(vid).x() == v0.x() && vid != vertices[0] && vid != vertices[1]){
                        transition_verts[vid] = true;
                        break;
                    }
                if(v0.y() == v1.y())
                    if (m.vert(vid).y() == v0.y() && vid != vertices[0] && vid != vertices[1]){
                        transition_verts[vid] = true;
                        break;
                    }
                if(v0.z() == v1.z())
                    if (m.vert(vid).z() == v0.z() && vid != vertices[0] && vid != vertices[1]){
                        transition_verts[vid] = true;
                        break;
                    }
            }
            changed_pid.push_back(pid);
        }
        else{ //2C
            /*std::cout<<"2C"<<std::endl;
            std::vector<bool> t2_a = {true, false, false, false, false, false, true, false};
            std::vector<bool> t2_b = {false, false, false, true, false, true, false, false};
            std::vector<bool> t2_c = {false, false, true, false, true, false, false, false};
            std::vector<bool> t2_d = {false, true, false, false, false, false, false, true};
            std::vector<bool> t2;

            for (auto vid: poly_verts_id) t2.push_back(transition_verts[vid]);

            if(t2 == t2_a){
                std::vector<bool> mask_a = {true, true, true, true, false, false, true, true};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask_a[i]) transition_verts[poly_verts_id[i]] = true;
            }
            else if(t2 == t2_b){
                std::vector<bool> mask_b = {true, true, true, true, false, true, true, false};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask_b[i]) transition_verts[poly_verts_id[i]] = true;
            }
            else if(t2 == t2_c){
                std::vector<bool> mask_c = {true, true, true, true, true, true, false, false};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask_c[i]) transition_verts[poly_verts_id[i]] = true;
            }
            else if(t2 == t2_d){
                std::vector<bool> mask_d = {true, true, true, true, true, false, false, true};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask_d[i]) transition_verts[poly_verts_id[i]] = true;
            }
            changed_pid.push_back(pid);*/
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
                         SchemeInfo                           & info){


    bool is_3a = false;

    vec3d v0 = m.vert(vertices[0]);
    vec3d v1 = m.vert(vertices[1]);
    vec3d v2 = m.vert(vertices[2]);


    if(((round(v0.x()*10000.0)/10000.0) == (round(v1.x()*10000.0)/10000.0) && (round(v1.x()*10000.0)/10000.0) == (round(v2.x()*10000.0)/10000.0) && (round(v2.x()*10000.0)/10000.0) == (round(v0.x()*10000.0)/10000.0)) ||
       ((round(v0.y()*10000.0)/10000.0) == (round(v1.y()*10000.0)/10000.0) && (round(v1.y()*10000.0)/10000.0) == (round(v2.y()*10000.0)/10000.0) && (round(v2.y()*10000.0)/10000.0) == (round(v0.y()*10000.0)/10000.0)) ||
       ((round(v0.z()*10000.0)/10000.0) == (round(v1.z()*10000.0)/10000.0) && (round(v1.z()*10000.0)/10000.0) == (round(v2.z()*10000.0)/10000.0) && (round(v2.z()*10000.0)/10000.0) == (round(v0.z()*10000.0)/10000.0))) is_3a=true;



    if(is_3a){ //3A
        info.type = HexTransition::TWO_EDGES;
        info.scale = m.edge_length(m.adj_p2e(pid)[0]);
        setOrientationInfo3(info, transition_verts, poly_verts_id);
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
            std::cout<<"3B"<<std::endl;
            std::vector<bool> t3_a = {true, false, false, true, false, true, false, false};
            std::vector<bool> t3_b = {false, true, false, false, true, false, false, true};
            std::vector<bool> t3_c = {true, false, false, false, false, true, true, false};
            std::vector<bool> t3_d = {false, true, true, false, true, false, false, false};
            std::vector<bool> t3_e = {true, true, false, false, false, false, true, false};
            std::vector<bool> t3_f = {false, false, true, false, true, true, false, false};
            std::vector<bool> t3_g = {false, true, false, false, false, false, true, true};
            std::vector<bool> t3_h = {false, false, true, true, false, true, false, false};
            std::vector<bool> t3_i = {false, false, true, true, true, false, false, false};
            std::vector<bool> t3_l = {true, false, false, false, false, false, true, true};
            std::vector<bool> t3_m = {false, false, false, true, true, true, false, false};
            std::vector<bool> t3_n = {true, true, false, false, false, false, false, true};
            std::vector<bool> t3_o = {false, true, true, false, false, false, false, true};
            std::vector<bool> t3_p = {true, false, false, true, false, false, true, false};
            std::vector<bool> t3_q = {false, false, true, false, true, false, false, true};
            std::vector<bool> t3_r = {false, false, false, true, false, true, true, false};
            std::vector<bool> t3_s = {true, false, false, false, true, false, true, false};
            std::vector<bool> t3_t = {false, false, false, true, false, true, false, true};
            std::vector<bool> t3_u = {false, false, true, false, true, false, true, false};
            std::vector<bool> t3_v = {false, true, false, false, false, true, false, true};
            std::vector<bool> t3_w = {false, true, false, true, false, false, false, true};
            std::vector<bool> t3_j = {true, false, true, false, false, false, true, false};
            std::vector<bool> t3_x = {false, true, false, true, false, true, false, false};
            std::vector<bool> t3_z = {true, false, true, false, true, false, false, false};

            std::vector<bool> t3;

            for (auto vid: poly_verts_id) t3.push_back(transition_verts[vid]);

            if(t3 == t3_a || t3 == t3_x){
                std::vector<bool> mask = {true, true, false, true, false, true, false, false};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
            }
            else if(t3 == t3_b || t3 == t3_n){
                std::vector<bool> mask = {true, true, false, false, true, false, false, true};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
            }
            else if(t3 == t3_c || t3 == t3_s){
                std::vector<bool> mask = {true, false, false, false, true, true, true, false};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
            }
            else if(t3 == t3_d || t3 == t3_f){
                std::vector<bool> mask = {false, true, true, false, true, true, false, false};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
            }
            else if(t3 == t3_e || t3 == t3_j){
                std::vector<bool> mask = {true, true, true, false, false, false, true, false};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
            }
            else if(t3 == t3_g || t3 == t3_v){
                std::vector<bool> mask = {false, true, false, false, false, true, true, true};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
            }
            else if(t3 == t3_h || t3 == t3_r){
                std::vector<bool> mask = {false, false, true, true, false, true, true, false};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
            }
            else if(t3 == t3_i || t3 == t3_z){
                std::vector<bool> mask = {true, false, true, true, true, false, false, false};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
            }
            else if(t3 == t3_l || t3 == t3_p){
                std::vector<bool> mask = {true, false, false, true, false, false, true, true};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
            }
            else if(t3 == t3_m || t3 == t3_t){
                std::vector<bool> mask = {false, false, false, true, true, true, false, true};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
            }
            else if(t3 == t3_o || t3 == t3_w){
                std::vector<bool> mask = {false, true, true, true, false, false, false, true};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
            }
            else if(t3 == t3_q || t3 == t3_u){
                std::vector<bool> mask = {false, false, true, false, true, false, true, true};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
            }

            changed_pid.push_back(pid);

        }
        else{  //3C
            /*std::cout<<"3C"<<std::endl;
            std::vector<bool> t3_a = {true, false, false, false, false, true, false, true};
            std::vector<bool> t3_b = {false, true, false, false, true, false, true, false};
            std::vector<bool> t3_c = {false, false, true, false, false, true, false, true};
            std::vector<bool> t3_d = {false, false, false, true, true, false, true, false};
            std::vector<bool> t3_e = {false, true, false, true, true, false, false, false};
            std::vector<bool> t3_f = {true, false, true, false, false, true, false, false};
            std::vector<bool> t3_g = {false, true, false, true, false, false, true, false};
            std::vector<bool> t3_h = {true, false, true, false, false, false, false, true};

            std::vector<bool> t3;

            for (auto vid: poly_verts_id) t3.push_back(transition_verts[vid]);

            if(t3 == t3_a || t3 == t3_e){
                std::vector<bool> mask = {true, true, false, true, true, true, false, true};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
            }
            else if(t3 == t3_b || t3 == t3_f){
                std::vector<bool> mask = {true, true, true, false, true, true, true, false};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
            }
            else if(t3 == t3_c || t3 == t3_g){
                std::vector<bool> mask = {false, true, true, true, false, true, true, true};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
            }
            else if(t3 == t3_d || t3 == t3_h){
                std::vector<bool> mask = {true, false, true, true, true, false, true, true};
                for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
            }

            changed_pid.push_back(pid);*/
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
                         SchemeInfo                           & info){

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
        setOrientationInfo4A(info, transition_verts, poly_verts_id);
        poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
    }
    else{ // 4B, 4C, 4D, 4E or 4F
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

        if(n_free_edge == 3){ // 4B, 4C
            if(faces_3_nodes==3){ // 4B
                info.type = HexTransition::CORNER_4B;
                info.scale = m.edge_length(m.adj_p2e(pid)[0]);
                setOrientationInfo4B(info, transition_verts, poly_verts_id);
                poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
            }
            else{ //4C
                info.scale = m.edge_length(m.adj_p2e(pid)[0]);
                setOrientationInfo4C(info, transition_verts, poly_verts_id);
                poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
            }
        }
        else if(n_free_edge == 2){ // 4D, 4E
            if(faces_3_nodes==1){ // 4D
                std::cout<<"4D"<<std::endl;
                std::vector<bool> t4_a = {true, true, false, true, false, false, true, false};
                std::vector<bool> t4_b = {true, false, true, true, false, true, false, false};
                std::vector<bool> t4_c = {false, true, true, true, true, false, false, false};
                std::vector<bool> t4_d = {true, true, true, false, false, false, false, true};
                std::vector<bool> t4_e = {true, false, false, false, false, true, true, true};
                std::vector<bool> t4_f = {false, true, false, false, true, false, true, true};
                std::vector<bool> t4_g = {false, false, true, false, true, false, true, true};
                std::vector<bool> t4_h = {false, false, false, true, true, true, true, false};
                std::vector<bool> t4_i = {true, false, true, false, true, false, false, true};
                std::vector<bool> t4_l = {true, false, false, true, true, false, true, false};
                std::vector<bool> t4_m = {true, false, false, true, false, true, false, true};
                std::vector<bool> t4_n = {false, true, false, true, true, false, false, true};
                std::vector<bool> t4_o = {false, true, true, false, false, true, false, true};
                std::vector<bool> t4_p = {false, true, true, false, true, false, true, false};
                std::vector<bool> t4_q = {true, false, true, false, true, false, false, true};
                std::vector<bool> t4_r = {false, true, false, true, false, true, true, false};
                std::vector<bool> t4_s = {true, false, true, false, true, true, false, false};
                std::vector<bool> t4_t = {false, true, false, true, true, true, false, false};
                std::vector<bool> t4_u = {true, true, false, false, false, true, false, true};
                std::vector<bool> t4_v = {true, true, false, false, true, false, true, false};
                std::vector<bool> t4_w = {false, false, true, true, false, true, false, true};
                std::vector<bool> t4_x = {false, true, false, true, false, false, true, true};
                std::vector<bool> t4_j = {true, false, true, false, false, false, true, true};
                std::vector<bool> t4_z = {false, false, true, true, true, false, true, false};

                std::vector<bool> t4;

                for (auto vid: poly_verts_id) t4.push_back(transition_verts[vid]);

                if(t4 == t4_a || t4 == t4_j){
                    std::vector<bool> mask = {true, true, true, true, false, false, true, true};
                    for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
                }
                else if(t4 == t4_b || t4 == t4_r){
                    std::vector<bool> mask = {true, true, true, true, false, true, true, false};
                    for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
                }
                else if(t4 == t4_c || t4 == t4_s){
                    std::vector<bool> mask = {true, true, true, true, true, true, false, false};
                    for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
                }
                else if(t4 == t4_d || t4 == t4_n){
                    std::vector<bool> mask = {true, true, true, true, true, false, false, true};
                    for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
                }
                else if(t4 == t4_e || t4 == t4_l){
                    std::vector<bool> mask = {true, false, false, true, true, true, true, true};
                    for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
                }
                else if(t4 == t4_f || t4 == t4_u){
                    std::vector<bool> mask = {true, true, false, false, true, true, true, true};
                    for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
                }
                else if(t4 == t4_g || t4 == t4_p){
                    std::vector<bool> mask = {false, true, true, false, true, true, true, true};
                    for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
                }
                else if(t4 == t4_h || t4 == t4_w){
                    std::vector<bool> mask = {false, false, true, true, true, true, true, true};
                    for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
                }
                else if(t4 == t4_i || t4 == t4_z){
                    std::vector<bool> mask = {true, false, true, true, true, false, true, true};
                    for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
                }
                else if(t4 == t4_m || t4 == t4_t){
                    std::vector<bool> mask = {true, true, false, true, true, true, false, true};
                    for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
                }
                else if(t4 == t4_o || t4 == t4_x){
                    std::vector<bool> mask = {false, true, true, true, false, true, true, true};
                    for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
                }
                else if(t4 == t4_q || t4 == t4_v){
                    std::vector<bool> mask = {true, true, true, false, true, true, true, false};
                    for(int i=0; i<8; i++) if(! transition_verts[poly_verts_id[i]] && mask[i]) transition_verts[poly_verts_id[i]] = true;
                }

                changed_pid.push_back(pid);


            }
            else{ //4E
                std::cout<<"4E"<<std::endl;
                for(auto vid: poly_verts_id){
                    uint vid0 = m.edge_vert_ids(free_edge)[0];
                    uint vid1 = m.edge_vert_ids(free_edge)[1];

                    if(vid != vid0 && vid != vid1 && vid!=vertices[0] && vid!=vertices[1] && vid!=vertices[2] && vid!=vertices[3]){
                        transition_verts[vid] = true;
                    }
                }
                changed_pid.push_back(pid);
            }
        }
        else{ // 4F
            for (auto vid: poly_verts_id) if(transition_verts[vid]==false) transition_verts[vid] = true;
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
                         std::unordered_map<uint, SchemeInfo> & poly2scheme,
                         std::vector<uint>                    & changed_pid,
                         SchemeInfo                           & info){

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
            info.type = HexTransition::CORNER_5A;
            info.scale = m.edge_length(m.adj_p2e(pid)[0]);
            setOrientationInfo5A(info, transition_verts, poly_verts_id);
            poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
        }
        else{ //5B
            std::cout<<"5B"<<std::endl;
            for(auto vid: poly_verts_id){
                uint vid0 = m.edge_vert_ids(free_edge)[0];
                uint vid1 = m.edge_vert_ids(free_edge)[1];

                if(vid != vid0 && vid != vid1 && vid!=vertices[0] && vid!=vertices[1] && vid!=vertices[2] && vid!=vertices[3] && vid!=vertices[4]){
                    transition_verts[vid] = true;
                }
            }
            changed_pid.push_back(pid);
        }
    }
    else{ // 5C
        for (auto vid: poly_verts_id) if(! transition_verts[vid]) transition_verts[vid] = true;
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
                         SchemeInfo                           & info){


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
        setOrientationInfo6(info, transition_verts, poly_verts_id);
        poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
    }
    else{ // 6B, 6C
        for (auto vid: poly_verts_id) if(! transition_verts[vid]) transition_verts[vid] = true;
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
    std::vector<uint> changed_pid; //controls the changed vertices for each pid

    bool added_newverts = true;

    // PROFILING
    // tstart = start()
    // time = stop(tstart)
    // var_find_min_max += time

    std::vector<uint> polys;

    for(uint i=0;i<m_in.num_polys();i++) polys.push_back(i);

    while(added_newverts){
        for (auto pid: polys){
            std::vector<uint> vertices; //controls the number of "true" vertices for each poly
            std::vector<vec3d> poly_vec3d; //controls the orientation of the input mesh cubes
            std::vector<uint> poly_verts_id = m_in.poly_verts_id(pid);

            for(uint vid: poly_verts_id){
                if(transition_verts[vid]) vertices.push_back(vid);
                poly_vec3d.push_back(m_in.vert(vid));
            }

            SchemeInfo info;


            if(poly_vec3d[0].z() > poly_vec3d[3].z() && poly_vec3d[0].x() < poly_vec3d[1].x()){
                info.mask_type=0;
            }
            else if(poly_vec3d[0].x() < poly_vec3d[3].x() && poly_vec3d[0].z() < poly_vec3d[1].z()){
                info.mask_type=1;
            }
            else if(poly_vec3d[0].z() < poly_vec3d[3].z() && poly_vec3d[0].x() > poly_vec3d[1].x()){
                info.mask_type=2;
            }
            else if(poly_vec3d[0].x() > poly_vec3d[3].x() && poly_vec3d[0].z() > poly_vec3d[1].z()){
                info.mask_type=3;
            }
            else if(poly_vec3d[0].y() < poly_vec3d[3].y() && poly_vec3d[0].x() < poly_vec3d[1].x()){
                info.mask_type=4;
            }

            //MAYBE WE NEED OTHER CONFIGURATIONS FOR MASK_TYPE

            switch (vertices.size()){
                case 2: mark2vertices(m_in, pid, vertices, transition_verts, poly_verts_id, poly2scheme, changed_pid, info);
                        break;
                case 3: mark3vertices(m_in, pid, vertices, transition_verts, poly_verts_id, poly2scheme, changed_pid, info);
                        break;
                case 4: mark4vertices(m_in, pid, vertices, transition_verts, poly_verts_id, poly2scheme, changed_pid, info);
                        break;
                case 5: mark5vertices(m_in, pid, vertices, transition_verts, poly_verts_id, poly2scheme, changed_pid, info);
                        break;
                case 6: mark6vertices(m_in, pid, vertices, transition_verts, poly_verts_id, poly2scheme, changed_pid, info);
                        break;
                case 7: info.type = HexTransition::CORNER_7A;
                        info.scale = m_in.edge_length(m_in.adj_p2e(pid)[0]);
                        setOrientationInfo7(info, transition_verts, poly_verts_id);
                        poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
                        break;
                case 8: info.type = HexTransition::FULL;
                        info.scale = m_in.edge_length(m_in.adj_p2e(pid)[0]);
                        poly2scheme.insert(std::pair<uint, SchemeInfo>(pid, info));
                        break;
            }

        }

        if(changed_pid.size() > 0){
            added_newverts=true;
            //reset of the auxiliar vector
            changed_pid.clear();
            std::cout<<"sdfnsld"<<std::endl;
            poly2scheme.clear();
        }
        else{
            merge_schemes_into_mesh(m_out, poly2scheme);

            std::vector<uint> polys_to_remove;

            for (auto p: poly2scheme) polys_to_remove.push_back(p.first);

            m_out.polys_remove(polys_to_remove);

            added_newverts=false;
        }
    }
}


}
