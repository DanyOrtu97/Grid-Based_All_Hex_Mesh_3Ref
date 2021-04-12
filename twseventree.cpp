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

#include <twseventree.h>
#include <cinolib/how_many_seconds.h>
#include <cinolib/parallel_for.h>
#include <cinolib/geometry/point.h>
#include <cinolib/geometry/sphere.h>
#include <cinolib/geometry/segment.h>
#include <cinolib/geometry/triangle.h>
#include <cinolib/geometry/tetrahedron.h>
#include <stack>

namespace cinolib
{

CINO_INLINE
TwseventreeNode::~TwseventreeNode()
{
    // "in a tree's node destructor, you only need to destroy the children pointers that are manually
    //  allocated by you. You don't need to worry about the deallocation of the node itself."
    //  https://stackoverflow.com/questions/34170164/destructor-for-binary-search-tree
    for(int i=0; i<27; ++i)
    {
        if(children[i]!=nullptr)
        {
            delete children[i];
        }
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
Twseventree::Twseventree(const uint max_depth,
               const uint items_per_leaf)
: max_depth(max_depth)
, items_per_leaf(items_per_leaf)
{}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
Twseventree::~Twseventree()
{
    // delete Octree
    if(root!=nullptr) delete root;

    // delete item list
    while(!items.empty())
    {
        delete items.back();
        items.pop_back();
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void Twseventree::build()
{
    typedef std::chrono::high_resolution_clock Time;
    Time::time_point t0 = Time::now();

    if(items.empty()) return;

    // initialize root with all items, also updating its AABB
    assert(root==nullptr);
    root = new TwseventreeNode(nullptr, AABB());
    root->item_indices.resize(items.size());
    std::iota(root->item_indices.begin(),root->item_indices.end(),0);
    for(auto it : items) root->bbox.push(it->aabb);

    root->bbox.scale(1.5); // enlarge bbox to account for queries outside legal area.
                           // this should disappear eventually....

    if(root->item_indices.size()<items_per_leaf || max_depth==1)
    {
        leaves.push_back(root);
        tree_depth = 1;
    }
    else
    {
        subdivide(root);

        if(max_depth==2)
        {
            tree_depth = 2;
            for(int i=0; i<27; ++i) leaves.push_back(root->children[i]);
        }
        else
        {
            // WORK IN PARALLEL ON EACH OCTANT
            // To fully avoid syncrhonization between the threads global information
            // such as vector of leaves and tree depth are duplicated, and will be
            // merged after convergence.

            uint octant_depth[27] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
            std::vector<const TwseventreeNode*> octant_leaves[8];

            std::queue<std::pair<TwseventreeNode*,uint>> splitlist[27]; // (node, depth)
            for(int i=0; i<27; ++i)
            {
                if(root->children[i]->item_indices.size()>items_per_leaf)
                {
                    splitlist[i].push(std::make_pair(root->children[i],2));
                }
                else octant_leaves[i].push_back(root->children[i]);
            }

            PARALLEL_FOR(0,27,0,[&](uint i)
            {
                while(!splitlist[i].empty())
                {
                    auto pair  = splitlist[i].front();
                    auto node  = pair.first;
                    uint depth = pair.second + 1;
                    splitlist[i].pop();

                    subdivide(node);

                    for(int j=0; j<27; ++j)
                    {
                        if(depth<max_depth && node->children[j]->item_indices.size()>items_per_leaf)
                        {
                            splitlist[i].push(std::make_pair(node->children[j], depth));
                        }
                        else octant_leaves[i].push_back(node->children[j]);
                    }

                    octant_depth[i] = std::max(octant_depth[i], depth);
                }
            });

            // global merge of octant data
            tree_depth = *std::max_element(octant_depth, octant_depth+27);
            for(int i=0; i<27; ++i)
            {
                std::copy(octant_leaves[i].begin(), octant_leaves[i].end(), std::back_inserter(leaves));
            }
        }
    }

    if(print_debug_info)
    {
        Time::time_point t1 = Time::now();
        double t = how_many_seconds(t0,t1);
        std::cout << ":::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl;
        std::cout << "Octree created (" << t << "s)                      " << std::endl;
        std::cout << "#Items                   : " << items.size()         << std::endl;
        std::cout << "#Leaves                  : " << leaves.size()        << std::endl;
        std::cout << "Max depth                : " << max_depth            << std::endl;
        std::cout << "Depth                    : " << tree_depth           << std::endl;
        std::cout << "Prescribed items per leaf: " << items_per_leaf       << std::endl;
        std::cout << "Max items per leaf       : " << max_items_per_leaf() << std::endl;
        std::cout << ":::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl;
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void Twseventree::subdivide(TwseventreeNode * node)
{
    // create children octants
    vec3d min = node->bbox.min;
    vec3d max = node->bbox.max;
    vec3d avg1 = (max + min)/3;
    vec3d avg2 = avg1*2;


    node->children[0] = new TwseventreeNode(node, AABB(vec3d(min[0], min[1], min[2]), vec3d(avg1[0], avg1[1], avg1[2])));
    node->children[1] = new TwseventreeNode(node, AABB(vec3d(avg1[0], min[1], min[2]), vec3d(avg2[0], avg1[1], avg1[2])));
    node->children[2] = new TwseventreeNode(node, AABB(vec3d(avg2[0], min[1], min[2]), vec3d(max[0], avg1[1], avg1[2])));


    node->children[3] = new TwseventreeNode(node, AABB(vec3d(min[0], avg1[1], min[2]), vec3d(avg1[0], avg2[1], avg1[2])));
    node->children[4] = new TwseventreeNode(node, AABB(vec3d(avg1[0], avg1[1], min[2]), vec3d(avg2[0], avg2[1], avg1[2])));
    node->children[5] = new TwseventreeNode(node, AABB(vec3d(avg2[0], avg1[1], min[2]), vec3d(max[0], avg2[1], avg1[2])));


    node->children[6] = new TwseventreeNode(node, AABB(vec3d(min[0], avg2[1], min[2]), vec3d(avg1[0], max[1], avg1[2])));
    node->children[7] = new TwseventreeNode(node, AABB(vec3d(avg1[0], avg2[1], min[2]), vec3d(avg2[0], max[1], avg1[2])));
    node->children[8] = new TwseventreeNode(node, AABB(vec3d(avg2[0], avg2[1], min[2]), vec3d(max[0], max[1], avg1[2])));


    node->children[9] = new TwseventreeNode(node, AABB(vec3d(min[0], min[1], avg1[2]), vec3d(avg1[0], avg1[1], avg2[2])));
    node->children[10] = new TwseventreeNode(node, AABB(vec3d(avg1[0], min[1], avg1[2]), vec3d(avg2[0], avg1[1], avg2[2])));
    node->children[11] = new TwseventreeNode(node, AABB(vec3d(avg2[0], min[1], avg1[2]), vec3d(max[0], avg1[1], avg2[2])));


    node->children[12] = new TwseventreeNode(node, AABB(vec3d(min[0], avg1[1], avg1[2]), vec3d(avg1[0], avg2[1], avg2[2])));
    node->children[13] = new TwseventreeNode(node, AABB(vec3d(avg1[0], avg1[1], avg1[2]), vec3d(avg2[0], avg2[1], avg2[2])));
    node->children[14] = new TwseventreeNode(node, AABB(vec3d(avg2[0], avg1[1], avg1[2]), vec3d(max[0], avg2[1], avg2[2])));


    node->children[15] = new TwseventreeNode(node, AABB(vec3d(min[0], avg2[1], avg1[2]), vec3d(avg1[0], max[1], avg2[2])));
    node->children[16] = new TwseventreeNode(node, AABB(vec3d(avg1[0], avg2[1], avg1[2]), vec3d(avg2[0], max[1], avg2[2])));
    node->children[17] = new TwseventreeNode(node, AABB(vec3d(avg2[0], avg2[1], avg1[2]), vec3d(max[0], max[1], avg2[2])));


    node->children[18] = new TwseventreeNode(node, AABB(vec3d(min[0], min[1], avg2[2]), vec3d(avg1[0], avg1[1], max[2])));
    node->children[19] = new TwseventreeNode(node, AABB(vec3d(avg1[0], min[1], avg2[2]), vec3d(avg2[0], avg1[1], max[2])));
    node->children[20] = new TwseventreeNode(node, AABB(vec3d(avg2[0], min[1], avg2[2]), vec3d(max[0], avg1[1], max[2])));


    node->children[21] = new TwseventreeNode(node, AABB(vec3d(min[0], avg1[1], avg2[2]), vec3d(avg1[0], avg2[1], max[2])));
    node->children[22] = new TwseventreeNode(node, AABB(vec3d(avg1[0], avg1[1], avg2[2]), vec3d(avg2[0], avg2[1], max[2])));
    node->children[23] = new TwseventreeNode(node, AABB(vec3d(avg2[0], avg1[1], avg2[2]), vec3d(max[0], avg2[1], max[2])));


    node->children[24] = new TwseventreeNode(node, AABB(vec3d(min[0], avg2[1], avg2[2]), vec3d(avg1[0], max[1], max[2])));
    node->children[25] = new TwseventreeNode(node, AABB(vec3d(avg1[0], avg2[1], avg2[2]), vec3d(avg2[0], max[1], max[2])));
    node->children[26] = new TwseventreeNode(node, AABB(vec3d(avg2[0], avg2[1], avg2[2]), vec3d(max[0], max[1], max[2])));


    for(uint it : node->item_indices)
    {
        bool orphan = true;
        for(int i=0; i<27; ++i)
        {
            assert(node->children[i]!=nullptr);
            if(node->children[i]->bbox.intersects_box(items.at(it)->aabb))
            {
                node->children[i]->item_indices.push_back(it);
                orphan = false;
            }
        }
        assert(!orphan);
    }

    node->item_indices.clear();
    node->is_inner = true;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void Twseventree::push_point(const uint id, const vec3d & v)
{
    items.push_back(new Point(id,v));
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void Twseventree::push_sphere(const uint id, const vec3d & c, const double r)
{
    items.push_back(new Sphere(id,c,r));
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void Twseventree::push_segment(const uint id, const std::vector<vec3d> & v)
{
    items.push_back(new Segment(id,v.data()));
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void Twseventree::push_triangle(const uint id, const std::vector<vec3d> & v)
{
    items.push_back(new Triangle(id,v.data()));
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void Twseventree::push_tetrahedron(const uint id, const std::vector<vec3d> & v)
{
    items.push_back(new Tetrahedron(id,v.data()));
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
uint Twseventree::max_items_per_leaf() const
{
    uint max=0;
    for(auto l : leaves) max = std::max(max,(uint)l->item_indices.size());
    return max;
}

}
