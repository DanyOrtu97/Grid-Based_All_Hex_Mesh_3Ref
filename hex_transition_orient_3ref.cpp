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

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void rotate(std::vector<vec3d> & verts,
            const std::string  & axis,
            const double       & angle)
{
    double rot[3][3];
    vec3d vec(0,0,0);
         if(axis == "x") vec.x() = 1;
    else if(axis == "y") vec.y() = 1;
    else                 vec.z() = 1;

    bake_rotation_matrix(vec, angle, rot);
    for(auto & v : verts) transform(v, rot);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void reflect(std::vector<vec3d> & verts,
             const std::string  & axis)
{
    double m[3][3];
    m[0][0] = -1; m[0][1] =  0; m[0][2] =  0;
    m[1][0] =  0; m[1][1] = -1; m[1][2] =  0;
    m[2][0] =  0; m[2][1] =  0; m[2][2] = -1;

    if(axis.find('x') != std::string::npos) m[0][0] = 1;
    if(axis.find('y') != std::string::npos) m[1][1] = 1;
    if(axis.find('z') != std::string::npos) m[2][2] = 1;

    for(auto & v : verts) transform(v, m);
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void orient_node(std::vector<vec3d>              & verts,
                 std::vector<std::vector<uint>>  & polys,
                 SchemeInfo                      & info,
                 const vec3d                     & poly_centroid){

    verts.reserve(Node::verts.size()/3);

    for (uint vid=0; vid<Node::verts.size(); vid+=3) verts.push_back(vec3d(Node::verts[vid]-0.5, Node::verts[vid+1]-0.5, Node::verts[vid+2]-0.5));


   switch(info.orientations[0])
    {
        case 0:  break; //DEFAULT
        case 1:  rotate(verts, "y",  M_PI/2); break;
        case 2:  rotate(verts, "y",  M_PI); break;
        case 3:  rotate(verts, "y", -M_PI/2); break;
        case 4:  rotate(verts, "x", -M_PI/2); break;
        case 5:  rotate(verts, "x", -M_PI/2);
                 rotate(verts, "y",  M_PI/2); break;
        case 6:  rotate(verts, "x", -M_PI/2);
                 rotate(verts, "y",  M_PI); break;
        case 7:  rotate(verts, "x", -M_PI); break;
    }

    for (auto & v: verts){
        v *= info.scale;
        v += poly_centroid;
    }

    polys = Node::polys;
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void orient_edge(std::vector<vec3d>              & verts,
                 std::vector<std::vector<uint>>  & polys,
                 SchemeInfo                      & info,
                 const vec3d                     & poly_centroid,
                 const std::vector<vec3d>        & poly_verts){

    verts.reserve(Edge::verts.size()/3);

    for (uint vid=0; vid<Edge::verts.size(); vid+=3) verts.push_back(vec3d(Edge::verts[vid]-0.5, Edge::verts[vid+1]-0.5, Edge::verts[vid+2]-0.5));


    verts[0] = poly_verts[3];
    verts[1] = poly_verts[2];
    verts[2] = poly_verts[7];
    verts[3] = poly_verts[6];
    verts[12] = poly_verts[0];
    verts[13] = poly_verts[1];
    verts[14] = poly_verts[4];
    verts[15] = poly_verts[5];

    verts[4] = verts[12];
    verts[4].z() -= ((verts[12].z() - verts[0].z()) / 3 )*2;
    verts[8] = verts[12];
    verts[8].z() -= (verts[12].z() - verts[0].z()) / 3;
    verts[6] = verts[12];
    verts[6].z() -= ((verts[12].z() - verts[0].z()) / 3 )*2;
    verts[6].y() += ((verts[2].y() - verts[0].y()) / 3 )*2;
    verts[10] = verts[12];
    verts[10].z() -= (verts[12].z() - verts[0].z()) / 3;
    verts[10].y() += ((verts[2].y() - verts[0].y()) / 3 )*2;
    verts[5] = verts[12];
    verts[5].z() -= ((verts[12].z() - verts[0].z()) / 3 )*2;
    verts[5].x() += ((verts[1].x() - verts[0].x()) / 3 )*2;
    verts[9] = verts[12];
    verts[9].z() -= (verts[12].z() - verts[0].z()) / 3;
    verts[9].x() += ((verts[1].x() - verts[0].x()) / 3 )*2;
    verts[7] = verts[12];
    verts[7].z() -= ((verts[12].z() - verts[0].z()) / 3 )*2;
    verts[7].x() += ((verts[1].x() - verts[0].x()) / 3 )*2;
    verts[7].y() += ((verts[2].y() - verts[0].y()) / 3 )*2;
    verts[11] = verts[12];
    verts[11].z() -= (verts[12].z() - verts[0].z()) / 3;
    verts[11].x() += ((verts[1].x() - verts[0].x()) / 3 )*2;
    verts[11].y() += ((verts[2].y() - verts[0].y()) / 3 )*2;


    for (auto & v: verts){
        v += poly_centroid;
    }

    switch(info.orientations[0])
    {
        case 0:  break; //DEFAULT
        case 1:  rotate(verts, "z",  M_PI/2); break;
        case 2:  rotate(verts, "y",  M_PI/2); break;
        case 3:  rotate(verts, "y", -M_PI/2); break;
        case 4:  rotate(verts, "z", -M_PI/2); break;
        case 5:  rotate(verts, "z",  M_PI); break;
        case 6:  rotate(verts, "y",  M_PI/2);
                 rotate(verts, "x", -M_PI/2); break;
        case 7:  rotate(verts, "y", -M_PI/2);
                 rotate(verts, "x",  M_PI/2); break;
        case 8:  rotate(verts, "x", -M_PI/2); break;
        case 9:  rotate(verts, "x",  M_PI/2); break;
        case 10: rotate(verts, "x", -M_PI/2);
                 rotate(verts, "y",  M_PI/2); break;
        case 11: rotate(verts, "y", -M_PI/2);
                 rotate(verts, "z",  M_PI/2); break;
    }



    polys = Edge::polys;

}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void orient_face(std::vector<vec3d>              & verts,
                 std::vector<std::vector<uint>>  & polys,
                 SchemeInfo                      & info,
                 const vec3d                     & poly_centroid){

    verts.reserve(Face::verts.size()/3);

    for (uint vid=0; vid<Face::verts.size(); vid+=3) verts.push_back(vec3d(Face::verts[vid]-0.5, Face::verts[vid+1]-0.5, Face::verts[vid+2]-0.5));

    switch(info.orientations[0])
    {
        case 0:  break; //DEFAULT
        case 1:  rotate(verts, "z",  M_PI/2); break;
        case 2:  rotate(verts, "z", -M_PI/2); break;
        case 3:  rotate(verts, "x", -M_PI/2); break;
        case 4:  rotate(verts, "x",  M_PI/2); break;
        case 5:  reflect(verts, "x"); break;
    }

    for (auto & v: verts){
        v *= info.scale;
        v += poly_centroid;
    }

    polys = Face::polys;

}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void orient_full(std::vector<vec3d>              & verts,
                 std::vector<std::vector<uint>>  & polys,
                 SchemeInfo                      & info,
                 const vec3d                     & poly_centroid){

    verts.reserve(Full::verts.size()/3);

    for (uint vid=0; vid<Full::verts.size(); vid+=3) verts.push_back(vec3d(Full::verts[vid]-0.5, Full::verts[vid+1]-0.5, Full::verts[vid+2]-0.5));

    for (auto & v: verts){
        v *= info.scale;
        v += poly_centroid;
    }

    polys = Full::polys;

}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void orient_two_edges(std::vector<vec3d>              & verts,
                      std::vector<std::vector<uint>>  & polys,
                      SchemeInfo                      & info,
                      const vec3d                     & poly_centroid){

    verts.reserve(Two_Edges::verts.size()/3);

    for (uint vid=0; vid<Two_Edges::verts.size(); vid+=3) verts.push_back(vec3d(Two_Edges::verts[vid]-0.5, Two_Edges::verts[vid+1]-0.5, Two_Edges::verts[vid+2]-0.5));

    switch(info.orientations[0])
    {
        case 0:  break; //DEFAULT
        case 1:  rotate(verts, "y",  M_PI/2); break;
        case 2:  rotate(verts, "y", -M_PI/2); break;
        case 3:  rotate(verts, "y",  M_PI); break;
        case 4:  rotate(verts, "x", -M_PI);
                 rotate(verts, "y",  M_PI/2); break;
        case 5:  rotate(verts, "x", -M_PI);
                 rotate(verts, "y",  M_PI); break;
        case 6:  rotate(verts, "x", -M_PI); break;
        case 7:  rotate(verts, "x", -M_PI);
                 rotate(verts, "y", -M_PI/2); break;
        case 8:  rotate(verts, "x", -M_PI/2);
                 rotate(verts, "z",  M_PI/2); break;
        case 9:  rotate(verts, "x", -M_PI/2); break;
        case 10: rotate(verts, "z", -M_PI/2);
                 rotate(verts, "y",  M_PI/2);
                 rotate(verts, "z", -M_PI/2); break;
        case 11: rotate(verts, "z", -M_PI/2);
                 rotate(verts, "y",  M_PI/2); break;
        case 12: rotate(verts, "x",  M_PI/2); break;
        case 13: rotate(verts, "x",  M_PI/2);
                 rotate(verts, "z", -M_PI/2); break;
        case 14: rotate(verts, "x",  M_PI/2);
                 rotate(verts, "z",  M_PI/2); break;
        case 15: rotate(verts, "x",  M_PI/2);
                 rotate(verts, "z",  M_PI); break;
        case 16: rotate(verts, "z",  M_PI/2);
                 rotate(verts, "x",  M_PI/2); break;
        case 17: rotate(verts, "z",  M_PI/2);
                 rotate(verts, "x",  M_PI); break;
        case 18: rotate(verts, "z",  M_PI/2); break;
        case 19: rotate(verts, "z",  M_PI/2);
                 rotate(verts, "x", -M_PI/2); break;
        case 20: rotate(verts, "z", -M_PI/2);
                 rotate(verts, "x",  M_PI); break;
        case 21: rotate(verts, "z", -M_PI/2);
                 rotate(verts, "x", -M_PI/2); break;
        case 22: rotate(verts, "z", -M_PI/2);
                 rotate(verts, "x",  M_PI/2); break;
        case 23: rotate(verts, "z", -M_PI/2); break;

    }

    for (auto & v: verts){
        v *= info.scale;
        v += poly_centroid;
    }

    polys = Two_Edges::polys;

}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void orient_two_faces(std::vector<vec3d>              & verts,
                      std::vector<std::vector<uint>>  & polys,
                      SchemeInfo                      & info,
                      const vec3d                     & poly_centroid){

    verts.reserve(Two_Faces::verts.size()/3);

    for (uint vid=0; vid<Two_Faces::verts.size(); vid+=3) verts.push_back(vec3d(Two_Faces::verts[vid]-0.5, Two_Faces::verts[vid+1]-0.5, Two_Faces::verts[vid+2]-0.5));

    switch(info.orientations[0])
    {
        case 0:  break; //DEFAULT
        case 1:  rotate(verts, "y", -M_PI/2); break;
        case 2:  rotate(verts, "y",  M_PI/2); break;
        case 3:  rotate(verts, "y",  M_PI); break;
        case 4:  rotate(verts, "z",  M_PI/2); break;
        case 5:  rotate(verts, "z",  M_PI/2);
                 rotate(verts, "x",  M_PI/2); break;
        case 6:  rotate(verts, "x",  M_PI/2); break;
        case 7:  rotate(verts, "x",  M_PI/2);
                 rotate(verts, "z",  M_PI/2); break;
        case 8:  rotate(verts, "z", -M_PI/2); break;
        case 9:  rotate(verts, "z",  M_PI/2);
                 rotate(verts, "x", -M_PI/2); break;
        case 10: rotate(verts, "x", -M_PI/2); break;
        case 11: rotate(verts, "x", -M_PI/2);
                 rotate(verts, "z", -M_PI/2); break;
    }

    for (auto & v: verts){
        v *= info.scale;
        v += poly_centroid;
    }

    polys = Two_Faces::polys;

}

} // end anonymous namespace



//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void hex_transition_orient_3ref(      std::vector<vec3d>              & verts,
                                      std::vector<std::vector<uint>>  & polys,
                                      SchemeInfo                      & info,
                                const vec3d                           & poly_centroid,
                                const std::vector<vec3d>              & poly_verts){


    switch(info.type){
        case HexTransition::NODE:
            orient_node(verts, polys, info, poly_centroid);
            break;
        case HexTransition::EDGE:
            orient_edge(verts, polys, info, poly_centroid, poly_verts);
            break;
        case HexTransition::FACE:
            orient_face(verts, polys, info, poly_centroid);
            break;           
        case HexTransition::FULL:
            orient_full(verts, polys, info, poly_centroid);
            break;
        case HexTransition::TWO_EDGES:
            orient_two_edges(verts, polys, info, poly_centroid);
            break;
        case HexTransition::TWO_FACES:
            orient_two_faces(verts, polys, info, poly_centroid);
            break;
     }

}
}


