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


#ifndef TWSEVENTREE_H
#define TWSEVENTREE_H

#include <cinolib/geometry/spatial_data_structure_item.h>
#include <cinolib/meshes/meshes.h>
#include <queue>

namespace cinolib
{

class TwseventreeNode
{
    public:
        TwseventreeNode(const TwseventreeNode * father, const AABB & bbox) : father(father), bbox(bbox) {}
       ~TwseventreeNode();
        const TwseventreeNode *father;
        TwseventreeNode       *children[27] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                                nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                                nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
        bool              is_inner = false;
        AABB              bbox;
        std::vector<uint> item_indices; // index Twseventree::items, avoiding to store a copy of the same object multiple times in each node it appears
};


class Twseventree
{
    public:
        explicit Twseventree(const uint max_depth      = 3,
                             const uint items_per_leaf = 50);
        virtual ~Twseventree();

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void push_point      (const uint id, const vec3d & v);
        void push_sphere     (const uint id, const vec3d & c, const double r);
        void push_segment    (const uint id, const std::vector<vec3d> & v);
        void push_triangle   (const uint id, const std::vector<vec3d> & v);
        void push_tetrahedron(const uint id, const std::vector<vec3d> & v);

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void build();

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void subdivide(TwseventreeNode *node);

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        template<class M, class V, class E, class P>
        void build_from_mesh_polys(const AbstractPolygonMesh<M,V,E,P> & m)
        {
            assert(items.empty());
            items.reserve(m.num_polys());
            for(uint pid=0; pid<m.num_polys(); ++pid)
            {
                for(uint i=0; i<m.poly_tessellation(pid).size()/3; ++i)
                {
                    vec3d v0 = m.vert(m.poly_tessellation(pid).at(3*i+0));
                    vec3d v1 = m.vert(m.poly_tessellation(pid).at(3*i+1));
                    vec3d v2 = m.vert(m.poly_tessellation(pid).at(3*i+2));
                    push_triangle(pid, {v0,v1,v2});
                }
            }
            build();
        }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        template<class M, class V, class E, class P>
        void build_from_mesh_polys(const AbstractPolyhedralMesh<M,V,E,P> & m)
        {
            assert(items.empty());
            items.reserve(m.num_polys());
            for(uint pid=0; pid<m.num_polys(); ++pid)
            {
                switch(m.mesh_type())
                {
                    case TETMESH : push_tetrahedron(pid, m.poly_verts(pid)); break;
                    default: assert(false && "Unsupported element");
                }
            }
            build();
        }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void build_from_vectors(const std::vector<vec3d> & verts,
                                const std::vector<uint>  & tris)
        {
            assert(items.empty());
            items.reserve(tris.size()/3);
            for(uint i=0; i<tris.size(); i+=3)
            {
                push_triangle(i/3, { verts.at(tris.at(i  )),
                                    verts.at(tris.at(i+1)),
                                    verts.at(tris.at(i+2))});
            }
            build();
        }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        template<class M, class V, class E, class P>
        void build_from_mesh_edges(const AbstractMesh<M,V,E,P> & m)
        {
            assert(items.empty());
            items.reserve(m.num_edges());
            for(uint eid=0; eid<m.num_edges(); ++eid)
            {
                push_segment(eid, m.edge_verts(eid));
            }
            build();
        }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        uint max_items_per_leaf() const;

        // all items live here, and leaf nodes only store indices to items
        std::vector<SpatialDataStructureItem*>     items;
        TwseventreeNode                            *root = nullptr;
        std::vector<const TwseventreeNode*>        leaves;

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        protected:

        uint max_depth;      // maximum allowed depth of the tree
        uint items_per_leaf; // prescribed number of items per leaf (can't go deeper than max_depth anyways)
        uint tree_depth = 0; // actual depth of the tree
        bool print_debug_info = true;

        // SUPPORT STRUCTURES ::::::::::::::::::::::::::::::::::::::::::::::::::::

        struct Obj
        {
            double      dist  = inf_double;
            TwseventreeNode *node  = nullptr;
            int         index = -1; // note: this is the item ID, NOT necessarily the index of vector items!!
            vec3d       pos;        // closest point
        };
        struct Greater
        {
            bool operator()(const Obj & obj1, const Obj & obj2)
            {
                return obj1.dist > obj2.dist;
            }
        };
        typedef std::priority_queue<Obj,std::vector<Obj>,Greater> PrioQueue;

};

}
#ifndef  CINO_STATIC_LIB
#include "twseventree.cpp"
#endif
#endif // TWSEVENTREE_H




