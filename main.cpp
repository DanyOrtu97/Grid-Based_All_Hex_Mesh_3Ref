#include "mainwindow.h"
#include <QApplication>

#include <cinolib/gui/qt/qt_gui_tools.h>
#include <cinolib/meshes/meshes.h>
#include <cinolib/color.h>
#include <cinolib/drawable_octree.h>
#include <cinolib/meshes/hexmesh.h>
#include <cinolib/profiler.h>
#include <map>
#include <algorithm>
#include <cinolib/how_many_seconds.h>
#include <hex_transition_install_3ref.h>

namespace cinolib
{

//Custom comparator operator for maps of vec3d
struct vert_compare
{
    bool operator()(const vec3d & a,
                    const vec3d & b) const
    {
       double eps = 1e-6;
       if(a.x()-b.x() < 0.0 && abs(a.x()-b.x()) > eps)
       {
           return true;
       }
       else if(abs(a.x()-b.x()) < eps)
       {
           if(a.y()-b.y() < 0.0 && abs(a.y()-b.y()) > eps)
           {
               return true;
           }
           else if(abs(a.y()-b.y()) < eps)
           {
               if(a.z()-b.z() < 0.0 && abs(a.z()-b.z()) > eps)
               {
                   return true;
               }
           }
       }
       return false;
    }
};


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class F, class P>
CINO_INLINE
// split the polygon of id "pid" of the input mesh into 27 cubes
void split27(const uint pid, DrawableHexmesh<M,V,E,F,P> & mesh, std::map<vec3d, uint> & vertices, std::vector<bool> & transition_verts){

    //vector for the new polys
    std::vector<std::vector<uint>> polys(27);

    //points for subdivision
    std::vector<vec3d> verts = mesh.poly_verts(pid);
    vec3d min = *std::min_element(verts.begin(), verts.end());
    vec3d max = *std::max_element(verts.begin(), verts.end());
    vec3d avg1 = min + ((max-min)/3);
    vec3d avg2 = avg1 + ((max-min)/3);

    std::vector<vec3d> newverts(64);

    if(mesh.num_polys() <= 1) for(auto v: verts){
        vertices.insert(std::pair<vec3d, uint>(v, uint(vertices.size())));
        transition_verts.push_back(false);
    }

    //z = min
    newverts[0] = vec3d(min[0], min[1], min[2]); //
    newverts[1] = vec3d(avg1[0], min[1], min[2]);
    newverts[2] = vec3d(avg2[0], min[1], min[2]);
    newverts[3] = vec3d(max[0], min[1], min[2]); //

    newverts[4] = vec3d(min[0], avg1[1], min[2]);
    newverts[5] = vec3d(avg1[0], avg1[1], min[2]);
    newverts[6] = vec3d(avg2[0], avg1[1], min[2]);
    newverts[7] = vec3d(max[0], avg1[1], min[2]);

    newverts[8] = vec3d(min[0], avg2[1], min[2]);
    newverts[9] = vec3d(avg1[0], avg2[1], min[2]);
    newverts[10] = vec3d(avg2[0], avg2[1], min[2]);
    newverts[11] = vec3d(max[0], avg2[1], min[2]);

    newverts[12] = vec3d(min[0], max[1], min[2]); //
    newverts[13] = vec3d(avg1[0], max[1], min[2]);
    newverts[14] = vec3d(avg2[0], max[1], min[2]);
    newverts[15] = vec3d(max[0], max[1], min[2]); //

    //z = avg1
    newverts[16] = vec3d(min[0], min[1], avg1[2]);
    newverts[17] = vec3d(avg1[0], min[1], avg1[2]);
    newverts[18] = vec3d(avg2[0], min[1], avg1[2]);
    newverts[19] = vec3d(max[0], min[1], avg1[2]);

    newverts[20] = vec3d(min[0], avg1[1], avg1[2]);
    newverts[21] = vec3d(avg1[0], avg1[1], avg1[2]);
    newverts[22] = vec3d(avg2[0], avg1[1], avg1[2]);
    newverts[23] = vec3d(max[0], avg1[1], avg1[2]);

    newverts[24] = vec3d(min[0], avg2[1], avg1[2]);
    newverts[25] = vec3d(avg1[0], avg2[1], avg1[2]);
    newverts[26] = vec3d(avg2[0], avg2[1], avg1[2]);
    newverts[27] = vec3d(max[0], avg2[1], avg1[2]);

    newverts[28] = vec3d(min[0], max[1], avg1[2]);
    newverts[29] = vec3d(avg1[0], max[1], avg1[2]);
    newverts[30] = vec3d(avg2[0], max[1], avg1[2]);
    newverts[31] = vec3d(max[0], max[1], avg1[2]);

    //z = avg2
    newverts[32] = vec3d(min[0], min[1], avg2[2]);
    newverts[33] = vec3d(avg1[0], min[1], avg2[2]);
    newverts[34] = vec3d(avg2[0], min[1], avg2[2]);
    newverts[35] = vec3d(max[0], min[1], avg2[2]);

    newverts[36] = vec3d(min[0], avg1[1], avg2[2]);
    newverts[37] = vec3d(avg1[0], avg1[1], avg2[2]);
    newverts[38] = vec3d(avg2[0], avg1[1], avg2[2]);
    newverts[39] = vec3d(max[0], avg1[1], avg2[2]);

    newverts[40] = vec3d(min[0], avg2[1], avg2[2]);
    newverts[41] = vec3d(avg1[0], avg2[1], avg2[2]);
    newverts[42] = vec3d(avg2[0], avg2[1], avg2[2]);
    newverts[43] = vec3d(max[0], avg2[1], avg2[2]);

    newverts[44] = vec3d(min[0], max[1], avg2[2]);
    newverts[45] = vec3d(avg1[0], max[1], avg2[2]);
    newverts[46] = vec3d(avg2[0], max[1], avg2[2]);
    newverts[47] = vec3d(max[0], max[1], avg2[2]);

    //z = max
    newverts[48] = vec3d(min[0], min[1], max[2]); //
    newverts[49] = vec3d(avg1[0], min[1], max[2]);
    newverts[50] = vec3d(avg2[0], min[1], max[2]);
    newverts[51] = vec3d(max[0], min[1], max[2]); //

    newverts[52] = vec3d(min[0], avg1[1], max[2]);
    newverts[53] = vec3d(avg1[0], avg1[1], max[2]);
    newverts[54] = vec3d(avg2[0], avg1[1], max[2]);
    newverts[55] = vec3d(max[0], avg1[1], max[2]);

    newverts[56] = vec3d(min[0], avg2[1], max[2]);
    newverts[57] = vec3d(avg1[0], avg2[1], max[2]);
    newverts[58] = vec3d(avg2[0], avg2[1], max[2]);
    newverts[59] = vec3d(max[0], avg2[1], max[2]);

    newverts[60] = vec3d(min[0], max[1], max[2]); //
    newverts[61] = vec3d(avg1[0], max[1], max[2]);
    newverts[62] = vec3d(avg2[0], max[1], max[2]);
    newverts[63] = vec3d(max[0], max[1], max[2]); //

    uint conta = 0;
    std::vector<uint> vert_to_false;

    //insert vertices in map and mesh
    for (auto v : newverts){
        if (vertices.find(v) == vertices.end()){
            uint fresh_vid = mesh.vert_add(v);
            vertices[v] = fresh_vid;
            transition_verts.push_back(false);
        }

    }


    //find vertices to set a false in the transition_verts vector
    for(auto vert: mesh.poly_verts_id(pid)){
        if(transition_verts[vert]){
            conta++;
            vert_to_false.push_back(vert);
        }
    }

    //calculate vertices to apply templates
    if(mesh.num_verts() > 64){
        if(conta == 4 || conta == 8){
            for (auto v: vert_to_false) transition_verts[v] = ! transition_verts[v];
        }
        else{
            for(auto vert: mesh.poly_verts_id(pid)) transition_verts[vert] = true;
        }
    }

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    //Insert polys
    polys[0] = {vertices.find(vec3d(min[0], min[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], min[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(min[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(min[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(min[0], avg1[1], avg1[2]))->second};

    polys[1] = {vertices.find(vec3d(avg1[0], min[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], min[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg1[2]))->second};

    polys[2] = {vertices.find(vec3d(avg2[0], min[1], min[2]))->second,
                vertices.find(vec3d(max[0], min[1], min[2]))->second,
                vertices.find(vec3d(max[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(max[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(max[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], avg1[2]))->second};

    polys[3] = {vertices.find(vec3d(min[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(min[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(min[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(min[0], avg2[1], avg1[2]))->second};

    polys[4] = {vertices.find(vec3d(avg1[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], avg1[2]))->second};

    polys[5] = {vertices.find(vec3d(avg2[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(max[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(max[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(max[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(max[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], avg1[2]))->second};

    polys[6] = {vertices.find(vec3d(min[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(min[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(min[0], max[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], max[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], max[1], avg1[2]))->second,
                vertices.find(vec3d(min[0], max[1], avg1[2]))->second};

    polys[7] = {vertices.find(vec3d(avg1[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], max[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], max[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], max[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], max[1], avg1[2]))->second};

    polys[8] = {vertices.find(vec3d(avg2[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(max[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(max[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], max[1], min[2]))->second,
                vertices.find(vec3d(max[0], max[1], min[2]))->second,
                vertices.find(vec3d(max[0], max[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], max[1], avg1[2]))->second};

    polys[9] = {vertices.find(vec3d(min[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], min[1], avg2[2]))->second,
                vertices.find(vec3d(min[0], min[1], avg2[2]))->second,
                vertices.find(vec3d(min[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(min[0], avg1[1], avg2[2]))->second};

    polys[10] = {vertices.find(vec3d(avg1[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], min[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], min[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg2[2]))->second};

    polys[11] = {vertices.find(vec3d(avg2[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], min[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], min[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], avg2[2]))->second};

    polys[12] = {vertices.find(vec3d(min[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(min[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(min[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(min[0], avg2[1], avg2[2]))->second};

    polys[13] = {vertices.find(vec3d(avg1[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], avg2[2]))->second};

    polys[14] = {vertices.find(vec3d(avg2[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], avg2[2]))->second};

    polys[15] = {vertices.find(vec3d(min[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(min[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(min[0], max[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], max[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], max[1], avg2[2]))->second,
                vertices.find(vec3d(min[0], max[1], avg2[2]))->second};

    polys[16] = {vertices.find(vec3d(avg1[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], max[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], max[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], max[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], max[1], avg2[2]))->second};

    polys[17] = {vertices.find(vec3d(avg2[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], max[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], max[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], max[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], max[1], avg2[2]))->second};

    polys[18] = {vertices.find(vec3d(min[0], min[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], min[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], min[1], max[2]))->second,
                vertices.find(vec3d(min[0], min[1], max[2]))->second,
                vertices.find(vec3d(min[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], max[2]))->second,
                vertices.find(vec3d(min[0], avg1[1], max[2]))->second};

    polys[19] = {vertices.find(vec3d(avg1[0], min[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], min[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], min[1], max[2]))->second,
                vertices.find(vec3d(avg1[0], min[1], max[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], max[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], max[2]))->second};

    polys[20] = {vertices.find(vec3d(avg2[0], min[1], avg2[2]))->second,
                vertices.find(vec3d(max[0], min[1], avg2[2]))->second,
                vertices.find(vec3d(max[0], min[1], max[2]))->second,
                vertices.find(vec3d(avg2[0], min[1], max[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(max[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(max[0], avg1[1], max[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], max[2]))->second};

    polys[21] = {vertices.find(vec3d(min[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], max[2]))->second,
                vertices.find(vec3d(min[0], avg1[1], max[2]))->second,
                vertices.find(vec3d(min[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], max[2]))->second,
                vertices.find(vec3d(min[0], avg2[1], max[2]))->second};

    polys[22] = {vertices.find(vec3d(avg1[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], max[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], max[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], max[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], max[2]))->second};

    polys[23] = {vertices.find(vec3d(avg2[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(max[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(max[0], avg1[1], max[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], max[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(max[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(max[0], avg2[1], max[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], max[2]))->second};

    polys[24] = {vertices.find(vec3d(min[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], max[2]))->second,
                vertices.find(vec3d(min[0], avg2[1], max[2]))->second,
                vertices.find(vec3d(min[0], max[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], max[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], max[1], max[2]))->second,
                vertices.find(vec3d(min[0], max[1], max[2]))->second};

    polys[25] = {vertices.find(vec3d(avg1[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], max[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], max[2]))->second,
                vertices.find(vec3d(avg1[0], max[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], max[1], avg2[2]))->second,
                vertices.find(vec3d(avg2[0], max[1], max[2]))->second,
                vertices.find(vec3d(avg1[0], max[1], max[2]))->second};

    polys[26] = {vertices.find(vec3d(avg2[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(max[0], avg2[1], avg2[2]))->second,
                vertices.find(vec3d(max[0], avg2[1], max[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], max[2]))->second,
                vertices.find(vec3d(avg2[0], max[1], avg2[2]))->second,
                vertices.find(vec3d(max[0], max[1], avg2[2]))->second,
                vertices.find(vec3d(max[0], max[1], max[2]))->second,
                vertices.find(vec3d(avg2[0], max[1], max[2]))->second};


    for(auto p : polys) mesh.poly_add(p);

    mesh.poly_remove(pid);

    mesh.updateGL();
}

}



int main(int argc, char *argv[])
{
    using namespace cinolib;
    QApplication a(argc, argv);

    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/cube.mesh";
    DrawableHexmesh<> mesh(s.c_str());

    //vertices
    std::map<vec3d, uint> vertices;

    GLcanvas gui;

     /*
    gui.push_obj(&mesh);
    gui.show();


    Profiler profiler;

    gui.push_marker(vec2i(10, gui.height()-20), "Ctrl + click to split a poly into 27 elements", Color::BLACK(), 12, 0);

    gui.callback_mouse_press = [&](GLcanvas *c, QMouseEvent *e)
    {
        if (e->modifiers() == Qt::ControlModifier)
        {
            vec3d p;
            vec2i click(e->x(), e->y());

            if (c->unproject(click, p))
            {
                profiler.push("Pick Polys");

                uint pid = mesh.pick_poly(p);
                profiler.pop();

                std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();

                split27(pid, mesh, vertices);

                std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

                std::cout<<"Subdivide Poly " << pid << " into 27 Polys [" << how_many_seconds(t0, t1) << "]" << std::endl;

                mesh.updateGL();
                c->updateGL();
            }
        }
    };
    */
    std::vector<bool> transition_verts;


    split27(0, mesh, vertices, transition_verts);

    split27(1, mesh, vertices, transition_verts);

    split27(3, mesh, vertices, transition_verts);



    std::vector<std::vector<bool>> polys_face_winding(mesh.num_polys());
    for (uint pid=0; pid<mesh.num_polys(); pid++) polys_face_winding[pid] = mesh.poly_faces_winding(pid);

    Polyhedralmesh<> inputMesh(mesh.vector_verts(), mesh.vector_faces(), mesh.vector_polys(), polys_face_winding);
    Polyhedralmesh<> outputMesh;


    hex_transition_install_3ref(inputMesh, transition_verts, outputMesh);


    std::vector<std::vector<bool>> polys_face_winding2(outputMesh.num_polys());
    for (uint pid=0; pid<outputMesh.num_polys(); pid++) polys_face_winding2[pid] = outputMesh.poly_faces_winding(pid);
    DrawablePolyhedralmesh<> mesh2(outputMesh.vector_verts(), outputMesh.vector_faces(), outputMesh.vector_polys(), polys_face_winding2);


    gui.push_obj(&mesh2);
    gui.show();


    VolumeMeshControlPanel<DrawablePolyhedralmesh<>> panel(&mesh2, &gui);
    //VolumeMeshControlPanel<DrawableHexmesh<>> panel(&mesh, &gui);
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});

    return a.exec();
}
