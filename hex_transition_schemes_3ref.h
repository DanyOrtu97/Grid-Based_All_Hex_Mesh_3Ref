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

    static const std::vector<std::vector<uint>> faces = {
        { 0,  1,  5,  4},
        { 1,  2,  6,  5},
        { 2,  3,  7,  6},
        { 4,  5,  9,  8},
        { 5,  6, 10,  9},
        { 6,  7, 11, 10},
        { 8,  9, 13, 12},
        { 9, 10, 14, 13},
        {10, 11, 15, 14},
        {16, 17, 21, 20},
        {17, 18, 22, 21},
        {18, 19, 23, 22},
        {20, 21, 25, 24},
        {21, 22, 26, 25},
        {22, 23, 27, 26},
        {24, 25, 29, 28},
        {25, 26, 30, 29},
        {26, 27, 31, 30},
        {32, 33, 37, 36},
        {33, 34, 38, 37},
        {34, 35, 39, 38},
        {36, 37, 41, 40},
        {37, 38, 42, 41},
        {38, 39, 43, 42},
        {40, 41, 45, 44},
        {41, 42, 46, 45},
        {42, 43, 47, 46},
        {48, 49, 53, 52},
        {49, 50, 54, 53},
        {50, 51, 55, 54},
        {52, 53, 57, 56},
        {53, 54, 58, 57},
        {54, 55, 59, 58},
        {56, 57, 61, 60},
        {57, 58, 62, 61},
        {58, 59, 63, 62},
        { 3, 19, 23,  7},
        {19, 35, 39, 23},
        {35, 51, 55, 39},
        { 7, 23, 27, 11},
        {23, 39, 43, 27},
        {39, 55, 59, 43},
        {11, 27, 31, 15},
        {27, 43, 47, 31},
        {43, 59, 63, 47},
        { 2, 18, 22,  6},
        {18, 34, 38, 22},
        {34, 50, 54, 38},
        { 6, 22, 26, 10},
        {22, 38, 42, 26},
        {38, 54, 58, 42},
        {10, 26, 30, 14},
        {26, 42, 46, 30},
        {42, 58, 62, 46},
        { 1, 17, 21,  5},
        {17, 33, 37, 21},
        {33, 49, 53, 37},
        { 5, 21, 25,  9},
        {21, 37, 41, 25},
        {37, 53, 57, 41},
        { 9, 25, 29, 13},
        {25, 41, 45, 29},
        {41, 57, 61, 45},
        { 0, 16, 20,  4},
        {16, 32, 36, 20},
        {32, 48, 52, 36},
        { 4, 20, 24,  8},
        {20, 36, 40, 24},
        {36, 52, 56, 40},
        { 8, 24, 28, 12},
        {24, 40, 44, 38},
        {40, 56, 60, 44},
        {12, 13, 29, 28},
        {13, 14, 30, 29},
        {14, 15, 31, 30},
        {28, 29, 45, 44},
        {29, 30, 46, 45},
        {30, 31, 47, 46},
        {44, 45, 61, 60},
        {45, 46, 62, 61},
        {46, 47, 63, 62},
        { 8,  9, 25, 24},
        { 9, 10, 26, 25},
        {10, 11, 27, 26},
        {24, 25, 41, 40},
        {25, 26, 42, 41},
        {26, 27, 43, 42},
        {40, 41, 57, 56},
        {41, 42, 58, 57},
        {42, 43, 59, 58},
        { 4,  5, 21, 20},
        { 5,  6, 22, 21},
        { 6,  7, 23, 22},
        {20, 21, 37, 36},
        {21, 22, 38, 37},
        {22, 23, 39, 38},
        {36, 37, 53, 52},
        {37, 38, 54, 53},
        {38, 39, 55, 54},
        { 0,  1, 17, 16},
        { 1,  2, 18, 17},
        { 2,  3, 19, 18},
        {16, 17, 33, 32},
        {17, 18, 34, 33},
        {18, 19, 35, 34},
        {32, 33, 49, 48},
        {33, 34, 50, 49},
        {34, 35, 51, 50},
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

    static const std::vector<std::vector<bool>> winding = {
        { true, true, true,  true, true,  true},
        { true, true, true, false, true,  true},
        { true, true, true, false, true,  true},
        { true, true, true,  true, true, false},
        { true, true, true, false, true, false},
        { true, true, true, false, true, false},
        { true, true, true,  true, true, false},
        { true, true, true, false, true, false},
        { true, true, true, false, true, false},
        {false, true, true,  true, true,  true},
        {false, true, true, false, true,  true},
        {false, true, true, false, true,  true},
        {false, true, true,  true, true, false},
        {false, true, true, false, true, false},
        {false, true, true, false, true, false},
        {false, true, true,  true, true, false},
        {false, true, true, false, true, false},
        {false, true, true, false, true, false},
        {false, true, true,  true, true,  true},
        {false, true, true, false, true,  true},
        {false, true, true, false, true,  true},
        {false, true, true,  true, true, false},
        {false, true, true, false, true, false},
        {false, true, true, false, true, false},
        {false, true, true,  true, true, false},
        {false, true, true, false, true, false},
        {false, true, true, false, true, false},
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

    static const std::vector<std::vector<uint>> faces = {
        { 0,  1,  5,  4},
        { 1,  2,  6,  5},
        { 2,  3,  7,  6},
        { 8,  9, 13, 12},
        { 9, 10, 14, 13},
        {10, 11, 15, 14},
        {16, 17, 21, 20},
        {17, 18, 22, 21},
        {18, 19, 23, 22},
        {24, 25, 29, 28},
        {25, 26, 30, 29},
        {26, 27, 31, 30},
        { 3, 11, 15,  7},
        {11, 19, 23, 15},
        {19, 27, 31, 23},
        { 2, 10, 14,  6},
        {10, 18, 22, 14},
        {18, 26, 30, 22},
        { 1,  9, 13,  5},
        { 9, 17, 21, 13},
        {17, 25, 29, 21},
        { 0,  8, 12,  4},
        { 8, 16, 20, 12},
        {16, 24, 28, 20},
        { 4,  5, 13, 12},
        { 5,  6, 14, 13},
        { 6,  7, 15, 14},
        {12, 13, 21, 20},
        {13, 14, 22, 21},
        {14, 15, 23, 22},
        {20, 21, 29, 28},
        {21, 22, 30, 29},
        {22, 23, 31, 30},
        { 0,  1,  9,  8},
        { 1,  2, 10,  9},
        { 2,  3, 11, 10},
        { 8,  9, 17, 16},
        { 9, 10, 18, 17},
        {10, 11, 19, 18},
        {16, 17, 25, 24},
        {17, 18, 26, 25},
        {18, 19, 27, 26},
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

    static const std::vector<std::vector<bool>> winding = {
        { true, true, true,  true, true, true},
        { true, true, true, false, true, true},
        { true, true, true, false, true, true},
        {false, true, true,  true, true, true},
        {false, true, true, false, true, true},
        {false, true, true, false, true, true},
        {false, true, true,  true, true, true},
        {false, true, true, false, true, true},
        {false, true, true, false, true, true},
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

    static const std::vector<std::vector<uint>> faces = {
        { 0,  2,  4,  5},
        { 0,  1,  3,  2},
        { 2,  3,  6,  5},
        { 3,  1,  7,  6},
        { 8,  9, 13, 12},
        { 8, 11, 10,  9},
        { 0,  1, 11,  8}, //obl
        { 9, 10, 14, 13},
        { 2,  3, 10,  9}, //obl
        {10, 11, 15, 14},
        {16, 17, 21, 20},
        {16, 19, 18, 17},
        {24, 25, 19, 16}, //obl
        {17, 18, 22, 21},
        {26, 27, 18, 17}, //obl
        {18, 19, 23, 22},
        {24, 26, 29, 28},
        {24, 25, 27, 26},
        {26, 27, 30, 29},
        {27, 25, 31, 30},
        { 1, 11, 15, 7},
        { 1, 25, 19, 11},
        {11, 19, 23, 15},
        {19, 25, 31, 23},
        { 3, 10, 14,  6},
        { 1, 11, 10,  3}, //obl
        {10, 18, 22, 14},
        {11, 19, 18, 10}, //obl
        {19, 25, 27, 18}, //obl
        {18, 27, 30, 22},
        { 2,  9, 13,  5},
        { 0,  8,  9,  2}, //obl
        { 9, 17, 21, 13},
        { 8, 16, 17,  9}, //obl
        {16, 24, 26, 17}, //obl
        {17, 26, 29, 21},
        { 0,  8, 12,  4},
        { 0, 24, 16,  8},
        { 8, 26, 20, 12},
        {16, 24, 28, 20},
        { 4,  5, 13, 12},
        { 5,  6, 14, 13},
        { 6,  7, 15, 14},
        {12, 13, 21, 20},
        {13, 14, 22, 21},
        {14, 15, 23, 22},
        {20, 21, 29, 28},
        {21, 22, 30, 29},
        {22, 23, 31, 30},
        { 2,  3, 10,  9},
        { 8,  9, 17, 16},
        { 9, 10, 18, 17},
        {10, 11, 19, 18},
        {17, 18, 27, 26},
        { 8, 11, 19, 16},
        { 0,  1, 25, 24},
    };

    static const std::vector<std::vector<uint>> polys = {
        { 0, 31, 30,  4, 36, 40},
        { 2, 32,  7, 38, 41, 49},
        { 1, 25, 31,  8,  5,  6},
        { 3, 20,  9, 24, 25, 42},
        { 4, 32, 10, 38, 43, 50},
        { 7, 26, 13, 32, 44, 51},
        { 9, 22, 15, 26, 45, 52},
        { 5, 27, 11, 33, 51, 54},
        {10, 35, 16, 39, 46, 34},
        {13, 29, 18, 35, 47, 53},
        {15, 23, 19, 28, 48, 29},
        {12, 11, 28, 34, 14, 17},
        { 6, 37, 12, 21, 54, 55},
    };

    static const std::vector<std::vector<bool>> winding = {
        { true,  true,  true,  true,  true, true},
        { true,  true,  true, false,  true, true},
        { true, false,  true, false, false, true},
        {false,  true,  true,  true,  true, true},
        {false,  true,  true,  true,  true, true},
        {false,  true,  true, false,  true, true},
        {false,  true,  true, false,  true, true},
        {false, false,  true, false, false, true},
        {false,  true,  true,  true,  true, true},
        {false,  true,  true, false,  true, true},
        {false,  true,  true, false,  true, true},
        { true, false, false, false, false, true},
        {false,  true, false,  true, false, true},
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

    static const std::vector<std::vector<uint>> faces = {
        { 0,  1,  5,  4},
        { 1,  2,  6,  5},
        { 2,  3,  7,  6},
        { 4,  5,  9,  8},
        { 5,  6, 10,  9},
        { 6,  7, 11, 10},
        { 8,  9, 13, 12},
        { 9, 10, 14, 13},
        {10, 11, 15, 14},
        {16, 17, 21, 20},
        {17, 18, 22, 21},
        {18, 19, 23, 22},
        {24, 25, 17, 16}, //obl
        {25, 26, 18, 17}, //obl
        {26, 27, 19, 18}, //obl
        {24, 25, 29, 28},
        {25, 26, 30, 29},
        {26, 27, 31, 30},
        {32, 33, 25, 24}, //obl
        {33, 34, 26, 25}, //obl
        {34, 35, 27, 26}, //obl
        {32, 33, 37, 36},
        {33, 34, 38, 37},
        {34, 35, 39, 38},
        { 3, 35, 27,  7},
        { 7, 27, 19, 11},
        {11, 19, 23, 15},
        {19, 27, 31, 23},
        {27, 35, 37, 29},
        { 2, 34, 26,  6},
        { 6, 26, 18, 10},
        {10, 18, 22, 14},
        {18, 26, 30, 22},
        {26, 34, 38, 30},
        { 1, 33, 25,  5},
        { 5, 25, 17,  9},
        { 9, 17, 21, 13},
        {17, 25, 29, 21},
        {25, 33, 37, 29},
        { 0, 32, 24,  4},
        { 4, 24, 16,  8},
        { 8, 16, 20, 12},
        {16, 24, 28, 20},
        {24, 32, 36, 28},
        {12, 13, 21, 20},
        {13, 14, 22, 21},
        {14, 15, 23, 22},
        {20, 21, 29, 28},
        {21, 22, 30, 29},
        {22, 23, 31, 30},
        {28, 29, 37, 26},
        {29, 30, 38, 37},
        {30, 31, 39, 38},
        { 8,  9, 17, 16},
        { 9, 10, 18, 17},
        {10, 11, 19, 18},
        { 4,  5, 25, 24},
        { 5,  6, 26, 25},
        { 6,  7, 27, 26},
        { 0,  1, 33, 32},
        { 1,  2, 34, 33},
        { 2,  3, 35, 34},
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

    static const std::vector<std::vector<bool>> winding = {
        { true, true, true,  true, true,  true},
        { true, true, true, false, true,  true},
        { true, true, true, false, true,  true},
        { true, true, true,  true, true, false},
        { true, true, true, false, true, false},
        { true, true, true, false, true, false},
        { true, true, true,  true, true, false},
        { true, true, true, false, true, false},
        { true, true, true, false, true, false},
        {false, true, true,  true, true, false},
        {false, true, true, false, true, false},
        {false, true, true, false, true, false},
        {false, true, true,  true, true, false},
        {false, true, true, false, true, false},
        {false, true, true, false, true, false},
    };
}

}

#endif // HEX_TRANSITION_SCHEMES_3REF_H
