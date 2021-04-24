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



} // end anonymous namespace


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void hex_transition_orient_3ref(      std::vector<vec3d>              &verts,
                                      std::vector<std::vector<uint>>  &faces,
                                      std::vector<std::vector<uint>>  &polys,
                                      std::vector<std::vector<bool>>  &winding,
                                const SchemeInfo                      &info,
                                const vec3d                            poly_centroid){


    switch(info.type){
        case HexTransition::FULL:
            break;
        case HexTransition::TRANSITION:
            break;
        case HexTransition::FACE:
            break;
        case HexTransition::TWO_ADJ_FACES:
            break;
        //case ***** ne mancano

    }

}
}


