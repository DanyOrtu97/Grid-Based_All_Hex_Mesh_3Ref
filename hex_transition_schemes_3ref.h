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

#ifndef HEX_TRANSITION_SCHEMES_3REF_H
#define HEX_TRANSITION_SCHEMES_3REF_H

#include <vector>
#include <sys/types.h>

/* Implementation of the convertion schemes to transform an adaptively refined grid
 * with hanging nodes into a conforming all hexahedral mesh using a 3-refinement approach.
 *
 * Schemes are defined in the primal (grid) mesh, and are meant to regularize the
 * valence of hanging nodes.
 *
 * The templates are:
 * 1. FULL
 * 2. TRANSITION
 * 3. FACE
 * 4. TWO_ADJ_FACES
 *
 *
 *
 * For each scheme we hard coded vertices, faces and cells of the polyhedral mesh.
 * Each mesh is a 1x1x1 cube, and the vertex with id 0 is always the "application point",
 * that is, the vertex that must be used for positioning the scheme in the grid.
 *
 * USAGE: Schemes are meant to be used as follows:
 *
 *  1 - locate type, size, and application point of the scheme in the adaptive grid
 *
 *  2 - position the scheme in your grid
 *
 *  3 - remove the standard grid element from your grid, substituting it with the
 *      scheme, also merging merging coincident vertices to topologically weld the
 *      two meshes
 *
 *
 * Steps (2,3) can be done with cinolib::hex_transition_install_3ref.
 */

namespace cinolib{

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace Full{

    static const std::vector<double> verts = {
            0 ,       0 ,       0 ,
      0.33333 ,       0 ,       0 ,
      0.66666 ,       0 ,       0 ,
            1 ,       0 ,       0 ,
            0 , 0.33333 ,       0 ,
      0.33333 , 0.33333 ,       0 ,
      0.66666 , 0.33333 ,       0 ,
            1 , 0.33333 ,       0 ,
            0 , 0.66666 ,       0 ,
      0.33333 , 0.66666 ,       0 ,
      0.66666 , 0.66666 ,       0 ,
            1 , 0.66666 ,       0 ,
            0 ,       1 ,       0 ,
      0.33333 ,       1 ,       0 ,
      0.66666 ,       1 ,       0 ,
            1 ,       1 ,       0 ,
            0 ,       0 , 0.33333 ,
      0.33333 ,       0 , 0.33333 ,
      0.66666 ,       0 , 0.33333 ,
            1 ,       0 , 0.33333 ,
            0 , 0.33333 , 0.33333 ,
      0.33333 , 0.33333 , 0.33333 ,
      0.66666 , 0.33333 , 0.33333 ,
            1 , 0.33333 , 0.33333 ,
            0 , 0.66666 , 0.33333 ,
      0.33333 , 0.66666 , 0.33333 ,
      0.66666 , 0.66666 , 0.33333 ,
            1 , 0.66666 , 0.33333 ,
            0 ,       1 , 0.33333 ,
      0.33333 ,       1 , 0.33333 ,
      0.66666 ,       1 , 0.33333 ,
            1 ,       1 , 0.33333 ,
            0 ,       0 , 0.66666 ,
      0.33333 ,       0 , 0.66666 ,
      0.66666 ,       0 , 0.66666 ,
            1 ,       0 , 0.66666 ,
            0 , 0.33333 , 0.66666 ,
      0.33333 , 0.33333 , 0.66666 ,
      0.66666 , 0.33333 , 0.66666 ,
            1 , 0.33333 , 0.66666 ,
            0 , 0.66666 , 0.66666 ,
      0.33333 , 0.66666 , 0.66666 ,
      0.66666 , 0.66666 , 0.66666 ,
            1 , 0.66666 , 0.66666 ,
            0 ,       1 , 0.66666 ,
      0.33333 ,       1 , 0.66666 ,
      0.66666 ,       1 , 0.66666 ,
            1 ,       1 , 0.66666 ,
            0 ,       0 ,       1 ,
      0.33333 ,       0 ,       1 ,
      0.66666 ,       0 ,       1 ,
            1 ,       0 ,       1 ,
            0 , 0.33333 ,       1 ,
      0.33333 , 0.33333 ,       1 ,
      0.66666 , 0.33333 ,       1 ,
            1 , 0.33333 ,       1 ,
            0 , 0.66666 ,       1 ,
      0.33333 , 0.66666 ,       1 ,
      0.66666 , 0.66666 ,       1 ,
            1 , 0.66666 ,       1 ,
            0 ,       1 ,       1 ,
      0.33333 ,       1 ,       1 ,
      0.66666 ,       1 ,       1 ,
            1 ,       1 ,       1 ,
    };


    static const std::vector<std::vector<uint>> polys = {
        { 16, 17,  1,  0, 20, 21,  5,  4},
        { 17, 18,  2,  1, 21, 22,  6,  5},
        { 18, 19,  3,  2, 22, 23,  7,  6},
        { 20, 21,  5,  4, 24, 25,  9,  8},
        { 21, 22,  6,  5, 25, 26, 10,  9},
        { 22, 23,  7,  6, 26, 27, 11, 10},
        { 24, 25,  9,  8, 28, 29, 13, 12},
        { 25, 26, 10,  9, 29, 30, 14, 13},
        { 26, 27, 11, 10, 30, 31, 15, 14},
        { 32, 33, 17, 16, 36, 37, 21, 20},
        { 33, 34, 18, 17, 37, 38, 22, 21},
        { 34, 35, 19, 18, 38, 39, 23, 22},
        { 36, 37, 21, 20, 40, 41, 25, 24},
        { 37, 38, 22, 21, 41, 42, 26, 25},
        { 38, 39, 23, 22, 42, 43, 27, 26},
        { 40, 41, 25, 24, 44, 45, 29, 28},
        { 41, 42, 26, 25, 45, 46, 30, 29},
        { 42, 43, 27, 26, 46, 47, 31, 30},
        { 48, 49, 33, 32, 52, 53, 37, 36},
        { 49, 50, 34, 33, 53, 54, 38, 37},
        { 50, 51, 35, 34, 54, 55, 39, 38},
        { 52, 53, 37, 36, 56, 57, 41, 40},
        { 53, 54, 38, 37, 57, 58, 42, 41},
        { 54, 55, 39, 38, 58, 59, 43, 42},
        { 56, 57, 41, 40, 60, 61, 45, 44},
        { 57, 58, 42, 41, 61, 62, 46, 45},
        { 58, 59, 43, 42, 62, 63, 47, 46},
    };
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace Transition{

    static const std::vector<double> verts = {
              0 ,       0 ,       0 ,
        0.33333 ,       0 ,       0 ,
        0.66666 ,       0 ,       0 ,
              1 ,       0 ,       0 ,
              0 ,       1 ,       0 ,
        0.33333 ,       1 ,       0 ,
        0.66666 ,       1 ,       0 ,
              1 ,       1 ,       0 ,
              0 ,       0 , 0.33333 ,
        0.33333 ,       0 , 0.33333 ,
        0.66666 ,       0 , 0.33333 ,
              1 ,       0 , 0.33333 ,
              0 ,       1 , 0.33333 ,
        0.33333 ,       1 , 0.33333 ,
        0.66666 ,       1 , 0.33333 ,
              1 ,       1 , 0.33333 ,
              0 ,       0 , 0.66666 ,
        0.33333 ,       0 , 0.66666 ,
        0.66666 ,       0 , 0.66666 ,
              1 ,       0 , 0.66666 ,
              0 ,       1 , 0.66666 ,
        0.33333 ,       1 , 0.66666 ,
        0.66666 ,       1 , 0.66666 ,
              1 ,       1 , 0.66666 ,
              0 ,       0 ,       1 ,
        0.33333 ,       0 ,       1 ,
        0.66666 ,       0 ,       1 ,
              1 ,       0 ,       1 ,
              0 ,       1 ,       1 ,
        0.33333 ,       1 ,       1 ,
        0.66666 ,       1 ,       1 ,
              1 ,       1 ,       1 ,
    };



    static const std::vector<std::vector<uint>> polys = {
        {  8,  9,  1,  0, 12, 13,  5,  4},
        {  9, 10,  2,  1, 13, 14,  6,  5},
        { 10, 11,  3,  2, 14, 15,  7,  6},
        { 16, 17,  9,  8, 20, 21, 13, 12},
        { 17, 18, 10,  9, 21, 22, 14, 13},
        { 18, 19, 11, 10, 22, 23, 15, 14},
        { 24, 25, 17, 16, 28, 29, 21, 20},
        { 25, 26, 18, 17, 29, 30, 22, 21},
        { 26, 27, 19, 18, 30, 31, 23, 22},
    };


}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace Face{

    static const std::vector<double> verts = {
            0 ,       0 ,       0 ,
            1 ,       0 ,       0 ,
      0.33333 , 0.66666 ,       0 ,
      0.66666 , 0.66666 ,       0 ,
            0 ,       1 ,       0 ,
      0.33333 ,       1 ,       0 ,
      0.66666 ,       1 ,       0 ,
            1 ,       1 ,       0 ,
            0 , 0.66666 , 0.33333 ,
      0.33333 , 0.83333 , 0.33333 ,
      0.66666 , 0.83333 , 0.33333 ,
            1 , 0.66666 , 0.33333 ,
            0 ,       1 , 0.33333 ,
      0.33333 ,       1 , 0.33333 ,
      0.66666 ,       1 , 0.33333 ,
            1 ,       1 , 0.33333 ,
            0 , 0.66666 , 0.66666 ,
      0.33333 , 0.83333 , 0.66666 ,
      0.66666 , 0.83333 , 0.66666 ,
            1 , 0.66666 , 0.66666 ,
            0 ,       1 , 0.66666 ,
      0.33333 ,       1 , 0.66666 ,
      0.66666 ,       1 , 0.66666 ,
            1 ,       1 , 0.66666 ,
            0 ,       0 ,       1 ,
            1 ,       0 ,       1 ,
      0.33333 , 0.66666 ,       1 ,
      0.66666 , 0.66666 ,       1 ,
            0 ,       1 ,       1 ,
      0.33333 ,       1 ,       1 ,
      0.66666 ,       1 ,       1 ,
            1 ,       1 ,       1 ,
    };

    static const std::vector<std::vector<uint>> polys = {
        {  8,  9,  2,  0, 12, 13,  5,  4},
        {  9, 10,  3,  2, 13, 14,  6,  5},
        { 10, 11,  1,  3, 14, 15,  7,  6},
        { 24, 26, 17, 16, 28, 29, 21, 20},
        { 26, 27, 18, 17, 29, 30, 22, 21},
        { 27, 25, 19, 18, 30, 31, 23, 22},
        { 26, 27,  3,  2, 17, 18, 10,  9},
        { 18, 19, 11, 10, 22, 23, 15, 14},
        { 16, 17,  9,  8, 20, 21, 13, 12},
        { 17, 18, 10,  9, 21, 22, 14, 13},
        { 27, 25,  1,  3, 18, 19, 11, 10},
        { 24, 26,  2,  0, 16, 17,  9,  8},
        { 24, 25,  1,  0, 26, 27,  3,  2},
    };

}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace Two_Adj_Faces{

    static const std::vector<double> verts = {
            0 ,       0 ,       0 ,
      0.33333 ,       0 ,       0 ,
      0.66666 ,       0 ,       0 ,
            1 ,       0 ,       0 ,
            0 , 0.33333 ,       0 ,
      0.33333 , 0.33333 ,       0 ,
      0.66666 , 0.33333 ,       0 ,
            1 , 0.33333 ,       0 ,
            0 , 0.66666 ,       0 ,
      0.33333 , 0.66666 ,       0 ,
      0.66666 , 0.66666 ,       0 ,
            1 , 0.66666 ,       0 ,
            0 ,       1 ,       0 ,
      0.33333 ,       1 ,       0 ,
      0.66666 ,       1 ,       0 ,
            1 ,       1 ,       0 ,
            0 , 0.66666 , 0.33333 ,
      0.33333 , 0.66666 , 0.33333 ,
      0.66666 , 0.66666 , 0.33333 ,
            1 , 0.66666 , 0.33333 ,
            0 ,       1 , 0.33333 ,
      0.33333 ,       1 , 0.33333 ,
      0.66666 ,       1 , 0.33333 ,
            1 ,       1 , 0.33333 ,
            0 , 0.33333 , 0.66666 ,
      0.33333 , 0.33333 , 0.66666 ,
      0.66666 , 0.33333 , 0.66666 ,
            1 , 0.33333 , 0.66666 ,
            0 ,       1 , 0.66666 ,
      0.33333 ,       1 , 0.66666 ,
      0.66666 ,       1 , 0.66666 ,
            1 ,       1 , 0.66666 ,
            0 ,       0 ,       1 ,
      0.33333 ,       0 ,       1 ,
      0.66666 ,       0 ,       1 ,
            1 ,       0 ,       1 ,
            0 ,       1 ,       1 ,
      0.33333 ,       1 ,       1 ,
      0.66666 ,       1 ,       1 ,
            1 ,       1 ,       1 ,
    };



    static const std::vector<std::vector<uint>> polys = {
        { 16, 17,  9,  8, 20, 21, 13, 12},
        { 17, 18, 10,  9, 21, 22, 14, 13},
        { 18, 19, 11, 10, 22, 23, 15, 14},
        { 24, 25,  5,  4, 16, 17,  9,  8},
        { 25, 26,  6,  5, 17, 18, 10,  9},
        { 26, 27,  7,  6, 18, 19, 11, 10},
        { 32, 33,  1,  0, 24, 25,  5,  4},
        { 33, 34,  2,  1, 25, 26,  6,  5},
        { 34, 35,  3,  2, 26, 27,  7,  6},
        { 24, 25, 17, 16, 28, 29, 21, 20},
        { 25, 26, 18, 17, 29, 30, 22, 21},
        { 26, 27, 19, 18, 30, 31, 23, 22},
        { 32, 33, 25, 24, 36, 37, 29, 28},
        { 33, 34, 26, 25, 37, 38, 30, 29},
        { 34, 35, 27, 26, 38, 39, 31, 30},
    };

}

}

#endif // HEX_TRANSITION_SCHEMES_3REF_H
