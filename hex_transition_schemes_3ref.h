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
 * 2. FACE
 * 3. TWO_ADJ_FACES
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
        {0,  1,  5,  4},
        {1,  2,  6,  5},
        {2,  3,  7,  6},
        {4,  5,  9,  8},
        {5,  6,  10, 9},
        {6,  7,  11, 10},
        {8,  9,  13, 12},
        {9,  10, 14, 13},
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
        {3, 19, 23, 7},
        {19, 35, 39, 23},
        {35, 51, 55, 39},
        {7, 23, 27, 11},
        {23, 39, 43, 27},
        {39, 55, 59, 43},
        {11, 27, 31, 15},
        {27, 43, 47, 31},
        {43, 59, 63, 47},
        {2, 18, 22, 6},
        {18, 34, 38, 22},
        {34, 50, 54, 38},
        {6, 22, 26, 10},
        {22, 38, 42, 26},
        {38, 54, 58, 42},
        {10, 26, 30, 14},
        {26, 42, 46, 30},
        {42, 58, 62, 46},
        {1, 17, 21, 5},
        {17, 33, 37, 21},
        {33, 49, 53, 37},
        {5, 21, 25, 9},
        {21, 37, 41, 25},
        {37, 53, 57, 41},
        {9, 25, 29, 13},
        {25, 41, 45, 29},
        {41, 57, 61, 45},
        {0, 16, 20, 4},
        {16, 32, 36, 20},
        {32, 48, 52, 36},
        {4, 20, 24, 8},
        {20, 36, 40, 24},
        {36, 52, 56, 40},
        {8, 24, 28, 12},
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
        {8, 9, 25, 24},
        {9, 10, 26, 25},
        {10, 11, 27, 26},
        {24, 25, 41, 40},
        {25, 26, 42, 41},
        {26, 27, 43, 42},
        {40, 41, 57, 56},
        {41, 42, 58, 57},
        {42, 43, 59, 58},
        {4, 5, 21, 20},
        {5, 6, 22, 21},
        {6, 7, 23, 22},
        {20, 21, 37, 36},
        {21, 22, 38, 37},
        {22, 23, 39, 38},
        {36, 37, 53, 52},
        {37, 38, 54, 53},
        {38, 39, 55, 54},
        {0, 1, 17, 16},
        {1, 2, 18, 17},
        {2, 3, 19, 18},
        {16, 17, 33, 32},
        {17, 18, 34, 33},
        {18, 19, 35, 34},
        {32, 33, 49, 48},
        {33, 34, 50, 49},
        {34, 35, 51, 50},
    };

    static const std::vector<std::vector<uint>> polys = {
        {0, 54, 9, 63, 90, 99},
        {1, 45, 10, 54, 91, 100},
        {2, 36, 11, 45, 92, 101},
        {3, 57, 12, 66, 81, 90},
        {4, 48, 13, 57, 82, 91},
        {5, 39, 14, 48, 83, 92},
        {6, 60, 15, 69, 72, 81},
        {7, 51, 16, 60, 73, 82},
        {8, 42, 17, 51, 74, 83},
        {9, 55, 18, 64, 93, 102},
        {10, 46, 19, 55, 94, 103},
        {11, 37, 20, 46, 95, 104},
        {12, 58, 21, 67, 84, 93},
        {13, 49, 22, 58, 85, 94},
        {14, 40, 23, 49, 86, 95},
        {15, 61, 24, 70, 75, 84},
        {16, 52, 25, 61, 76, 85},
        {17, 43, 26, 52, 77, 86},
        {18, 56, 27, 65, 96, 105},
        {19, 47, 28, 56, 97, 106},
        {20, 38, 29, 47, 98, 107},
        {21, 59, 30, 68, 87, 96},
        {22, 50, 31, 59, 88, 97},
        {23, 41, 32, 50, 89, 98},
        {24, 62, 33, 71, 78, 87},
        {25, 53, 34, 62, 79, 88},
        {26, 44, 35, 53, 80, 89},
    };

    static const std::vector<std::vector<bool>> winding={
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
        {false, false, false, false, false, false},
    };
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace Face{

    static const std::vector<double> verts={ };

    static const std::vector<std::vector<uint>> faces={ };

    static const std::vector<std::vector<uint>> polys={ };

    static const std::vector<std::vector<bool>> winding={ };
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace Two_Adj_Faces{

    static const std::vector<double> verts={ };

    static const std::vector<std::vector<uint>> faces={ };

    static const std::vector<std::vector<uint>> polys={ };

    static const std::vector<std::vector<bool>> winding={ };
}

}

#endif // HEX_TRANSITION_SCHEMES_3REF_H
