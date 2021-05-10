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
 * ******** and so on
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
        { 0, 54,  9, 63, 90,  99},
        { 1, 45, 10, 54, 91, 100},
        { 2, 36, 11, 45, 92, 101},
        { 3, 57, 12, 66, 81,  90},
        { 4, 48, 13, 57, 82,  91},
        { 5, 39, 14, 48, 83,  92},
        { 6, 60, 15, 69, 72,  81},
        { 7, 51, 16, 60, 73,  82},
        { 8, 42, 17, 51, 74,  83},
        { 9, 55, 18, 64, 93, 102},
        {10, 46, 19, 55, 94, 103},
        {11, 37, 20, 46, 95, 104},
        {12, 58, 21, 67, 84,  93},
        {13, 49, 22, 58, 85,  94},
        {14, 40, 23, 49, 86,  95},
        {15, 61, 24, 70, 75,  84},
        {16, 52, 25, 61, 76,  85},
        {17, 43, 26, 52, 77,  86},
        {18, 56, 27, 65, 96, 105},
        {19, 47, 28, 56, 97, 106},
        {20, 38, 29, 47, 98, 107},
        {21, 59, 30, 68, 87,  96},
        {22, 50, 31, 59, 88,  97},
        {23, 41, 32, 50, 89,  98},
        {24, 62, 33, 71, 78,  87},
        {25, 53, 34, 62, 79,  88},
        {26, 44, 35, 53, 80,  89},
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
        { 0, 18,  3, 21, 24, 33},
        { 1, 15,  4, 18, 25, 34},
        { 2, 12,  5, 15, 26, 35},
        { 3, 19,  6, 22, 27, 36},
        { 4, 16,  7, 19, 28, 37},
        { 5, 13,  8, 16, 29, 38},
        { 6, 20,  9, 23, 30, 39},
        { 7, 17, 10, 20, 31, 40},
        { 8, 14, 11, 17, 32, 41},
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
        { 8, 9, 2, 0, 12, 13, 5, 4},

        /*{ 0, 2, 9, 8, 4, 5, 13, 12},*/
        { 2, 3, 10, 9, 5, 6, 14, 13},
        { 3, 1, 11, 10, 6, 7, 15, 14},
        { 16, 17, 26, 24, 20, 21, 29, 28},
        { 17, 18, 27, 26, 21, 22, 30, 29},
        { 18, 19, 25, 27, 22, 23, 31, 30},
        { 2, 3, 27, 26, 9, 10, 18, 17},
        { 10, 11, 19, 18, 14, 15, 23, 22},
        { 8, 9, 17, 16, 12, 13, 21, 20},
        { 9, 10, 18, 17, 13, 14, 22, 21},
        { 3, 1, 25, 27, 10, 11, 19, 18},
        { 0, 2, 26, 24, 8, 9, 17, 16},
        { 0, 1, 25, 24, 2, 3, 27, 26},
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
        { 0, 34, 18, 39, 56, 59},
        { 1, 29, 19, 34, 57, 60},
        { 2, 24, 20, 29, 58, 61},
        { 3, 35, 12, 40, 53, 56},
        { 4, 30, 13, 35, 54, 57},
        { 5, 25, 14, 30, 55, 58},
        { 6, 36,  9, 41, 44, 53},
        { 7, 31, 10, 36, 45, 54},
        { 8, 26, 11, 31, 46, 55},
        { 9, 37, 15, 42, 47, 12},
        {10, 32, 16, 37, 48, 13},
        {11, 27, 17, 32, 49, 14},
        {15, 38, 21, 43, 50, 18},
        {16, 33, 22, 38, 51, 19},
        {17, 28, 23, 33, 52, 20},
    };

}

}

#endif // HEX_TRANSITION_SCHEMES_3REF_H
