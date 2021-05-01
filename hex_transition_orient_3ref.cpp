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

#include "hex_transition_orient_3ref.h"
#include "hex_transition_schemes_3ref.h"

namespace cinolib{


namespace // anonymous
{

CINO_INLINE
void rotate(std::vector<vec3d> &verts,
            const std::string & axis,
            const double &angle){
    double rot[3][3];
    vec3d vec(0,0,0);
    if (axis == "x") vec.x() = 1;
    else if (axis == "y") vec.y() = 1;
    else vec.z() = 1;

    bake_rotation_matrix(vec, angle, rot);
    for (auto & v : verts) transform(v, rot);
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void orient_full(std::vector<vec3d>              & verts,
                 std::vector<std::vector<uint>>  & faces,
                 std::vector<std::vector<uint>>  & polys,
                 std::vector<std::vector<bool>>  & winding,
                 SchemeInfo                      & info,
                 const vec3d                     & poly_centroid){

    verts.reserve(Full::verts.size()/3);

    for (uint vid=0; vid<Full::verts.size(); vid+=3) verts.push_back(vec3d(Full::verts[vid]-0.5, Full::verts[vid+1]-0.5, Full::verts[vid+2]-0.5));

    for (auto & v: verts){
        v *= info.scale;
        v += poly_centroid;
    }

    polys = Full::polys;

    faces = Full::faces;

    winding = Full::winding;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void orient_face(std::vector<vec3d>              & verts,
                 std::vector<std::vector<uint>>  & faces,
                 std::vector<std::vector<uint>>  & polys,
                 std::vector<std::vector<bool>>  & winding,
                 SchemeInfo                      & info,
                 const vec3d                     & poly_centroid){

    verts.reserve(Face::verts.size()/3);

    for (uint vid=0; vid<Face::verts.size(); vid+=3) verts.push_back(vec3d(Face::verts[vid]-0.5, Face::verts[vid+1]-0.5, Face::verts[vid+2]-0.5));

    for (auto & v: verts){
        v *= info.scale;
        v += poly_centroid;
    }


    switch(info.orientations[0]){
        case PLUS_Y:  break;
        case PLUS_X:  rotate(verts, "z", -M_PI/2); break;
        case PLUS_Z:  rotate(verts, "x",  M_PI/2); break;
        case MINUS_X: rotate(verts, "z",  M_PI/2); break;
        case MINUS_Y: rotate(verts, "z",  M_PI);   break;
        case MINUS_Z: rotate(verts, "x", -M_PI/2); break;
    }


    polys = Face::polys;

    faces = Face::faces;

    winding = Face::winding;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void orient_transition(std::vector<vec3d>              & verts,
                       std::vector<std::vector<uint>>  & faces,
                       std::vector<std::vector<uint>>  & polys,
                       std::vector<std::vector<bool>>  & winding,
                       SchemeInfo                      & info,
                       const vec3d                     & poly_centroid){

    verts.reserve(Transition::verts.size()/3);

    for (uint vid=0; vid<Transition::verts.size(); vid+=3) verts.push_back(vec3d(Transition::verts[vid]-0.5, Transition::verts[vid+1]-0.5, Transition::verts[vid+2]-0.5));

    for (auto & v: verts){
        v *= info.scale;
        v += poly_centroid;
    }

    polys = Transition::polys;

    faces = Transition::faces;

    winding = Transition::winding;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void orient_two_adj_faces(std::vector<vec3d>              & verts,
                          std::vector<std::vector<uint>>  & faces,
                          std::vector<std::vector<uint>>  & polys,
                          std::vector<std::vector<bool>>  & winding,
                          SchemeInfo                      & info,
                          const vec3d                     & poly_centroid){

    verts.reserve(Two_Adj_Faces::verts.size()/3);

    for (uint vid=0; vid<Two_Adj_Faces::verts.size(); vid+=3) verts.push_back(vec3d(Two_Adj_Faces::verts[vid]-0.5, Two_Adj_Faces::verts[vid+1]-0.5, Two_Adj_Faces::verts[vid+2]-0.5));

    for (auto & v: verts){
        v *= info.scale;
        v += poly_centroid;
    }

    polys = Two_Adj_Faces::polys;

    faces = Two_Adj_Faces::faces;

    winding = Two_Adj_Faces::winding;
}

} // end anonymous namespace


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void hex_transition_orient_3ref(      std::vector<vec3d>              & verts,
                                      std::vector<std::vector<uint>>  & faces,
                                      std::vector<std::vector<uint>>  & polys,
                                      std::vector<std::vector<bool>>  & winding,
                                      SchemeInfo                      & info,
                                const vec3d                           & poly_centroid){


    switch(info.type){
        case HexTransition::FULL:
            orient_full(verts, faces, polys, winding, info, poly_centroid);
            break;           
        case HexTransition::TRANSITION:
            orient_transition(verts, faces, polys, winding, info, poly_centroid);
            break;
        case HexTransition::FACE:
            orient_face(verts, faces, polys, winding, info, poly_centroid);
            break;
        case HexTransition::TWO_ADJ_FACES:
            orient_two_adj_faces(verts, faces, polys, winding, info, poly_centroid);
            break;
        //case ***** ne mancano

    }

}
}


