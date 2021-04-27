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
void orient_full(std::vector<vec3d>              & verts,
                 std::vector<std::vector<uint>>  & faces,
                 std::vector<std::vector<uint>>  & polys,
                 std::vector<std::vector<bool>>  & winding){

    verts.reserve(Full::verts.size()/3);

    for (uint vid=0; vid<Full::verts.size(); vid+=3) verts.push_back(vec3d(Full::verts[vid], Full::verts[vid+1], Full::verts[vid+2]));

    polys = Full::polys;

    faces = Full::faces;

    winding = Full::winding;
}

CINO_INLINE
void orient_fake(std::vector<vec3d>              & verts,
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

    polys = Face::polys;

    faces = Face::faces;

    winding = Face::winding;
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
            orient_full(verts, faces, polys, winding);
            break;           
        case HexTransition::TRANSITION:
            orient_fake(verts, faces, polys, winding, info, poly_centroid);
            break;
        case HexTransition::FACE:
            orient_fake(verts, faces, polys, winding, info, poly_centroid);
            break;
        case HexTransition::TWO_ADJ_FACES:
            orient_fake(verts, faces, polys, winding, info, poly_centroid);
            break;
        //case ***** ne mancano

    }

}
}


