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
 * 1. NODE
 * 2. EDGE
 * 3. FACE
 * 4. FULL
 * 5. TWO_EDGES
 * 6. TWO_FACES
 *
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
 *
 * Hex_transition_install_3ref must be exectuted since all the hanging nodes are solved
 */

namespace cinolib{

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace Node{

    //mettere 0.333333 il cubo

    static const std::vector<double> verts = {
            0 ,       0 ,       0 ,
            1 ,       0 ,       0 ,
            0 ,       1 ,       0 ,
            1 ,       1 ,       0 ,
            0 ,       0 ,     0.5 ,
          0.5 ,       0 ,     0.5 ,
            0 ,     0.5 ,     0.5 ,
          0.5 ,     0.5 ,     0.5 ,
            0 ,       0 ,       1 ,
          0.5 ,       0 ,       1 ,
            0 ,     0.5 ,       1 ,
          0.5 ,     0.5 ,       1 ,
            1 ,       0 ,       1 ,
            0 ,       1 ,       1 ,
            1 ,       1 ,       1 ,
    };

    static const std::vector<std::vector<uint>> polys = {
         {  8,  9,  5,  4, 10, 11,  7,  6},
         {  4,  5,  1,  0,  6,  7,  3,  2},
         { 10, 11,  7,  6, 13, 14,  3,  2},
         {  9, 12,  1,  5, 11, 14,  3,  7},
    };

}




//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace Edge{

    static const std::vector<double> verts = {
            0 ,       0 ,       0 ,
            1 ,       0 ,       0 ,
            0 ,       1 ,       0 ,
            1 ,       1 ,       0 ,
            0 ,       0 , 0.33333 ,
      0.66666 ,       0 , 0.33333 ,
            0 , 0.66666 , 0.33333 ,
      0.66666 , 0.66666 , 0.33333 ,
            0 ,       0 , 0.66666 ,
      0.66666 ,       0 , 0.66666 ,
            0 , 0.66666 , 0.66666 ,
      0.66666 , 0.66666 , 0.66666 ,
            0 ,       0 ,       1 ,
            1 ,       0 ,       1 ,
            0 ,       1 ,       1 ,
            1 ,       1 ,       1 ,
    };

    static const std::vector<std::vector<uint>> polys = {
         {  4,  5,  1,  0,  6,  7,  3,  2},
         {  8,  9,  5,  4, 10, 11,  7,  6},
         { 10, 11,  7,  6, 14, 15,  3,  2},
         { 12, 13,  9,  8, 14, 15, 11, 10},
         {  9, 13,  1,  5, 11, 15,  3,  7},
    };

}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace Face{

    static const std::vector<double> verts = {
            0 ,       0 ,       0 ,
      0.33333 ,       0 ,       0 ,
      0.66666 ,       0 ,       0 ,
            1 ,       0 ,       0 ,
      0.33333 , 0.66666 ,       0 ,
      0.66666 , 0.66666 ,       0 ,
            0 ,       1 ,       0 ,
            1 ,       1 ,       0 ,
            0 ,       0 , 0.33333 ,
      0.33333 ,       0 , 0.33333 ,
      0.66666 ,       0 , 0.33333 ,
            1 ,       0 , 0.33333 ,
      0.33333 , 0.33333 , 0.33333 ,
      0.66666 , 0.33333 , 0.33333 ,
            0 , 0.66666 , 0.33333 ,
            1 , 0.66666 , 0.33333 ,
            0 ,       0 , 0.66666 ,
      0.33333 ,       0 , 0.66666 ,
      0.66666 ,       0 , 0.66666 ,
            1 ,       0 , 0.66666 ,
      0.33333 , 0.33333 , 0.66666 ,
      0.66666 , 0.33333 , 0.66666 ,
            0 , 0.66666 , 0.66666 ,
            1 , 0.66666 , 0.66666 ,
            0 ,       0 ,       1 ,
      0.33333 ,       0 ,       1 ,
      0.66666 ,       0 ,       1 ,
            1 ,       0 ,       1 ,
      0.33333 , 0.66666 ,       1 ,
      0.66666 , 0.66666 ,       1 ,
            0 ,       1 ,       1 ,
            1 ,       1 ,       1 ,
    };

    static const std::vector<std::vector<uint>> polys = {
        {  9, 10,  2,  1, 12, 13,  5,  4},
        { 17, 18, 10,  9, 20, 21, 13, 12},
        { 25, 26, 18, 17, 28, 29, 21, 20},
        { 20, 21, 13, 12, 28, 29,  5,  4},
        { 16, 17,  9,  8, 22, 20, 12, 14},
        { 24, 25, 17, 16, 30, 28, 20, 22},
        { 18, 19, 11, 10, 21, 23, 15, 13},
        { 26, 27, 19, 18, 29, 31, 23, 21},
        {  8,  9,  1,  0, 14, 12,  4,  6},
        { 10, 11,  3,  2, 13, 15,  7,  5},
        { 21, 23, 15, 13, 29, 31,  7,  5},
        { 22, 20, 12, 14, 30, 28,  4,  6},
        { 28, 29,  5,  4, 30, 31,  7,  6},
    };

}


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
namespace Two_Edges{

    static const std::vector<double> verts = {
            0 ,       0 ,       0 ,
            1 ,       0 ,       0 ,
            0 ,       1 ,       0 ,
            1 ,       1 ,       0 ,
            0 ,       0 , 0.33333 , //e1
      0.33333 ,       0 , 0.33333 , //e1
          0.5 ,       0 ,     0.3 ,
          0.7 ,       0 ,     0.3 ,
            0 , 0.66666 , 0.33333 , //e1
      0.33333 ,     0.6 , 0.33333 , //e1
          0.5 , 0.66666 ,     0.3 ,
          0.7 , 0.66666 ,     0.3 ,
            0 ,       0 , 0.66666 , //e1
      0.33333 ,       0 , 0.48333 , //e1
          0.5 ,       0 ,     0.5 ,
          0.7 ,       0 ,     0.5 ,
            0 , 0.66666 , 0.66666 , //e1
      0.33333 ,     0.6 , 0.48333 , //e1
          0.5 , 0.66666 ,     0.5 ,
          0.7 , 0.66666 ,     0.5 ,
      0.51657 ,       0 , 0.66666 , //e2
      0.66666 ,       0 , 0.66666 , //e2
      0.51657 ,     0.6 , 0.66666 , //e2
      0.66666 ,     0.6 , 0.66666 , //e2
      0.33333 ,       0 ,       1 , //e2
      0.66666 ,       0 ,       1 , //e2
      0.33333 , 0.66666 ,       1 , //e2
      0.66666 , 0.66666 ,       1 , //e2
            0 ,       0 ,       1 ,
            1 ,       0 ,       1 ,
            0 ,       1 ,       1 ,
            1 ,       1 ,       1 ,
    };

    static const std::vector<std::vector<uint>> polys = {
        {  6,  7,  1,  0, 10, 11,  3,  2},
        { 14, 15,  7,  6, 18, 19, 11, 10},
        { 18, 19, 11, 10, 30, 31,  3,  2},
        { 15, 29,  1,  7, 19, 31,  3, 11},
        { 12, 13,  5,  4, 16, 17,  9,  8}, //edge 1-2
        { 28, 14, 13, 12, 30, 18, 17, 16}, //edge 1-2
        { 13, 14,  6,  5, 17, 18, 10,  9}, //edge 1-2
        {  4,  5,  6,  0,  8,  9, 10,  2}, //edge 1-2
        { 16, 17,  9,  8, 30, 18, 10,  2}, //edge 1-2
        { 24, 25, 21, 20, 26, 27, 23, 22}, //edge 1-4
        { 20, 21, 15, 14, 22, 23, 19, 18}, //edge 1-4
        { 26, 27, 23, 22, 30, 31, 19, 18}, //edge 1-4
        { 25, 29, 15, 21, 27, 31, 19, 23}, //edge 1-4
        { 28, 24, 20, 14, 30, 26, 22, 18}, //edge 1-4
    };

}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace Two_Faces{

    static const std::vector<double> verts = {
            0 ,       0 ,       0 ,
            1 ,       0 ,       0 ,
            0 ,       1 ,       0 ,
            1 ,       1 ,       0 ,
          0.5 ,       0 ,     0.3 ,
          0.7 ,       0 ,     0.3 ,
          0.5 ,       1 ,     0.3 ,
          0.7 ,       1 ,     0.3 ,
          0.5 ,       0 ,     0.5 ,
          0.7 ,       0 ,     0.5 ,
          0.5 ,       1 ,     0.5 ,
          0.7 ,       1 ,     0.5 ,
            0 ,       0 ,       1 ,
            1 ,       0 ,       1 ,
            0 ,       1 ,       1 ,
            1 ,       1 ,       1 ,
            0 , 0.33333 ,       0 , //e1
      0.66666 , 0.33333 ,       0 , //e1
            0 , 0.66666 ,       0 , //e1
      0.66666 , 0.66666 ,       0 , //e1
      0.33333 , 0.33333 , 0.19998 , //e1
          0.6 , 0.33333 , 0.19998 , //e1
      0.33333 , 0.66666 , 0.19998 , //e1
          0.6 , 0.66666 , 0.19998 , //e1
            1 , 0.33333 , 0.33333 , //e2
            1 , 0.33333 ,       1 , //e2
            1 , 0.66666 , 0.33333 , //e2
            1 , 0.66666 ,       1 , //e2
      0.80002 , 0.33333 , 0.39999 , //e2
      0.80002 , 0.33333 , 0.66666 , //e2
      0.80002 , 0.66666 , 0.39999 , //e2
      0.80002 , 0.66666 , 0.66666 , //e2
            0 ,       1 , 0.33333 , //face1
            0 ,       1 , 0.66666 , //face1
      0.33333 ,       1 , 0.33333 , //face1
      0.33333 ,       1 , 0.48333 , //face1
            0 , 0.66666 ,       1 , //face1
            0 , 0.66666 , 0.66666 , //face1
            0 , 0.66666 , 0.33333 , //face1
      0.33333 , 0.66666 , 0.66666 , //face1
      0.16667 , 0.66666 ,     0.6 , //face1
      0.16667 , 0.66666 , 0.33333 , //face1
            0 , 0.33333 ,       1 , //face1
            0 , 0.33333 , 0.66666 , //face1
            0 , 0.33333 , 0.33333 , //face1
      0.33333 , 0.33333 , 0.66666 , //face1
      0.16667 , 0.33333 ,     0.6 , //face1
      0.16667 , 0.33333 , 0.33333 , //face1
            0 ,       0 , 0.33333 , //face1
            0 ,       0 , 0.66666 , //face1
      0.33333 ,       0 , 0.33333 , //face1
      0.33333 ,       0 , 0.48333 , //face1
      0.51667 ,       0 , 0.66666 , //face2
      0.66666 ,       0 , 0.66666 , //face2
      0.51667 ,       1 , 0.66666 , //face2
      0.66666 ,       1 , 0.66666 , //face2
      0.49997 , 0.33333 , 0.83327 , //face2
      0.66666 , 0.33333 , 0.83327 , //face2
      0.49997 , 0.66666 , 0.83327 , //face2
      0.66666 , 0.66666 , 0.83327 , //face2
      0.33333 ,       0 ,       1 , //face2
      0.66666 ,       0 ,       1 , //face2
      0.33333 , 0.33333 ,       1 , //face2
      0.66666 , 0.33333 ,       1 , //face2
      0.33333 , 0.66666 ,       1 , //face2
      0.66666 , 0.66666 ,       1 , //face2
      0.33333 ,       1 ,       1 , //face2
      0.66666 ,       1 ,       1 , //face2
    };

    static const std::vector<std::vector<uint>> polys = {
        {  8,  9,  5,  4, 10, 11,  7,  6},
        { 20, 21, 17, 16, 22, 23, 19, 18}, //edge 2-6
        {  4,  5, 21, 20,  6,  7, 23, 22}, //edge 2-6
        { 21,  5,  1, 17, 23,  7,  3, 19}, //edge 2-6
        { 22, 23, 19, 18,  6,  7,  3,  2}, //edge 2-6
        {  4,  5,  1,  0, 20, 21, 17, 16}, //edge 2-6
        { 28, 29, 25, 24, 30, 31, 27, 26}, //edge 4-8
        { 30, 31, 27, 26,  7, 11, 15,  3}, //edge 4-8
        {  5,  9, 29, 28,  7, 11, 31, 30}, //edge 4-8
        {  5,  9, 13,  1, 28, 29, 25, 24}, //edge 4-8
        {  5, 29, 24,  1,  7, 31, 26,  3}, //edge 4-8
        { 44, 47, 20, 16, 38, 41, 22, 18}, //face 1-2-5-6
        { 43, 46, 47, 44, 37, 40, 41, 38}, //face 1-2-5-6
        { 42, 45, 46, 43, 36, 39, 40, 37}, //face 1-2-5-6
        { 49, 51, 50, 48, 43, 46, 47, 44}, //face 1-2-5-6
        { 37, 40, 41, 38, 33, 35, 34, 32}, //face 1-2-5-6
        { 47, 20, 22, 41, 46, 45, 39, 40}, //face 1-2-5-6
        { 45,  8,  4, 20, 39, 10,  6, 22}, //face 1-2-5-6
        { 40, 39, 22, 41, 35, 10,  6, 34}, //face 1-2-5-6
        { 51,  8,  4, 50, 46, 45, 20, 47}, //face 1-2-5-6
        { 38, 41, 22, 18, 32, 34,  6,  2}, //face 1-2-5-6
        { 16, 20, 47, 44,  0,  4, 50, 48}, //face 1-2-5-6
        { 36, 39, 40, 37, 14, 10, 35, 33}, //face 1-2-5-6
        { 42, 45,  8, 12, 43, 46, 51, 49}, //face 1-2-5-6
        { 62, 63, 57, 56, 64, 65, 59, 58}, //face 1-4-5-8
        { 63, 25, 29, 57, 65, 27, 31, 59}, //face 1-4-5-8
        { 42, 62, 56, 45, 36, 64, 58, 39}, //face 1-4-5-8
        { 64, 65, 59, 58, 66, 67, 55, 54}, //face 1-4-5-8
        { 60, 61, 53, 52, 62, 63, 57, 56}, //face 1-4-5-8
        { 56, 57, 29, 45, 58, 59, 31, 39}, //face 1-4-5-8
        { 45, 29,  9,  8, 39, 31, 11, 10}, //face 1-4-5-8
        { 58, 59, 31, 39, 54, 55, 11, 10}, //face 1-4-5-8
        { 52, 53,  9,  8, 56, 57, 29, 45}, //face 1-4-5-8
        { 36, 64, 58, 39, 14, 66, 54, 10}, //face 1-4-5-8
        { 65, 27, 31, 59, 67, 15, 11, 55}, //face 1-4-5-8
        { 12, 60, 52,  8, 42, 62, 56, 45}, //face 1-4-5-8
        { 61, 13,  9, 53, 63, 25, 29, 57}, //face 1-4-5-8
    };

}


}


#endif // HEX_TRANSITION_SCHEMES_3REF_H
