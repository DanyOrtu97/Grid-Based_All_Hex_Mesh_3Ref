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

#ifndef HEX_TRANSITION_INSTALL_3REF_H
#define HEX_TRANSITION_INSTALL_3REF_H

#include <cinolib/meshes/meshes.h>
#include <stdlib.h> //DA TOGLIERE
#include <map>

namespace cinolib{

/* This function installs the transitions defined in cinolib/hex_transition_schemes_3ref.h,
 * which allow to transform a strongly or weakly balanced grid into a pure hexahedral mesh.
 *
 *
 * Transition_verts is a vector having as many entries as the number of grid vertices, and
 * is set to true in correspondence of the vertices where transition schemes must be applied.
 *
 *
 */

template <class M, class V, class E, class F, class P>
CINO_INLINE
void hex_transition_install_3ref(const Hexmesh<M,V,E,F,P>           & m_in,
                                       std::vector<bool>            & transition_verts,
                                       Hexmesh<M,V,E,F,P>           & m_out);


template <class M, class V, class E, class F, class P>
CINO_INLINE
void hex_transition_install_3refDecoupling(const Hexmesh<M,V,E,F,P>           & m_in,
                                                 std::vector<bool>            & transition_verts,
                                                 Hexmesh<M,V,E,F,P>           & m_out);
}


#ifndef CINO_STATIC_LIB
#include "hex_transition_install_3ref.cpp"
#endif

#endif // HEX_TRANSITION_INSTALL_3REF_H
