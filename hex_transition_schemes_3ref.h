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

/*
 *  Spiegazione di cosa è e come si usa
 *
 *
 *
 */

namespace cinolib{

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace Full{

    static const std::vector<double> verts={ };

    static const std::vector<std::vector<uint>> faces={ };

    static const std::vector<std::vector<uint>> polys={ };

    static const std::vector<std::vector<bool>> winding={ };
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
