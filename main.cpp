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

namespace cinolib
{
template<class M, class V, class E, class F, class P>
CINO_INLINE
// split the polygon of id "pid" of the input mesh into 27 cubes
void split27(const uint pid, DrawableHexmesh<M,V,E,F,P> & mesh, std::map<vec3d, uint> & vertices){

    //vector for the new polys
    std::vector<std::vector<uint>> polys(27);

    //points for subdivision
    std::vector<vec3d> verts = mesh.poly_verts(pid);
    vec3d min = *std::min_element(verts.begin(), verts.end());
    vec3d max = *std::max_element(verts.begin(), verts.end());
    vec3d avg1 = min + ((max-min)/3);
    vec3d avg2 = avg1 + ((max-min)/3);

    if(mesh.num_polys() <= 1) for(auto v: verts) vertices.insert(std::pair<vec3d, uint>(v, uint(vertices.size())));

    //z = min
    if (vertices.find(vec3d(min[0], min[1], min[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], min[1], min[2]), uint(vertices.size()))); //0
        mesh.vert_add(vec3d(min[0], min[1], min[2]));
    }
    if (vertices.find(vec3d(avg1[0], min[1], min[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], min[1], min[2]), uint(vertices.size()))); //1
        mesh.vert_add(vec3d(avg1[0], min[1], min[2]));
    }
    if (vertices.find(vec3d(avg2[0], min[1], min[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], min[1], min[2]), uint(vertices.size()))); //2
        mesh.vert_add(vec3d(avg2[0], min[1], min[2]));
    }
    if (vertices.find(vec3d(max[0], min[1], min[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], min[1], min[2]), uint(vertices.size()))); //3
        mesh.vert_add(vec3d(max[0], min[1], min[2]));
    }
    if (vertices.find(vec3d(min[0], avg1[1], min[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], avg1[1], min[2]), uint(vertices.size()))); //4
        mesh.vert_add(vec3d(min[0], avg1[1], min[2]));
    }
    if (vertices.find(vec3d(avg1[0], avg1[1], min[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], avg1[1], min[2]), uint(vertices.size()))); //5
        mesh.vert_add(vec3d(avg1[0], avg1[1], min[2]));
    }
    if (vertices.find(vec3d(avg2[0], avg1[1], min[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], avg1[1], min[2]), uint(vertices.size()))); //6
        mesh.vert_add(vec3d(avg2[0], avg1[1], min[2]));
    }
    if (vertices.find(vec3d(max[0], avg1[1], min[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], avg1[1], min[2]), uint(vertices.size()))); //7
        mesh.vert_add(vec3d(max[0], avg1[1], min[2]));
    }
    if (vertices.find(vec3d(min[0], avg2[1], min[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], avg2[1], min[2]), uint(vertices.size()))); //8
        mesh.vert_add(vec3d(min[0], avg2[1], min[2]));
    }
    if (vertices.find(vec3d(avg1[0], avg2[1], min[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], avg2[1], min[2]), uint(vertices.size()))); //9
        mesh.vert_add(vec3d(avg1[0], avg2[1], min[2]));
    }
    if (vertices.find(vec3d(avg2[0], avg2[1], min[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], avg2[1], min[2]), uint(vertices.size()))); //10
        mesh.vert_add(vec3d(avg2[0], avg2[1], min[2]));
    }
    if (vertices.find(vec3d(max[0], avg2[1], min[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], avg2[1], min[2]), uint(vertices.size()))); //11
        mesh.vert_add(vec3d(max[0], avg2[1], min[2]));
    }
    if (vertices.find(vec3d(min[0], max[1], min[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], max[1], min[2]), uint(vertices.size()))); //12
        mesh.vert_add(vec3d(min[0], max[1], min[2]));
    }
    if (vertices.find(vec3d(avg1[0], max[1], min[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], max[1], min[2]), uint(vertices.size()))); //13
        mesh.vert_add(vec3d(avg1[0], max[1], min[2]));
    }
    if (vertices.find(vec3d(avg2[0], max[1], min[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], max[1], min[2]), uint(vertices.size()))); //14
        mesh.vert_add(vec3d(avg2[0], max[1], min[2]));
    }
    if (vertices.find(vec3d(max[0], max[1], min[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], max[1], min[2]), uint(vertices.size()))); //15
        mesh.vert_add(vec3d(max[0], max[1], min[2]));
    }

    //z = avg1
    if (vertices.find(vec3d(min[0], min[1], avg1[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], min[1], avg1[2]), uint(vertices.size()))); //16
        mesh.vert_add(vec3d(min[0], min[1], avg1[2]));
    }
    if (vertices.find(vec3d(avg1[0], min[1], avg1[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], min[1], avg1[2]), uint(vertices.size()))); //17
        mesh.vert_add(vec3d(avg1[0], min[1], avg1[2]));
    }
    if (vertices.find(vec3d(avg2[0], min[1], avg1[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], min[1], avg1[2]), uint(vertices.size()))); //18
        mesh.vert_add(vec3d(avg2[0], min[1], avg1[2]));
    }
    if (vertices.find(vec3d(max[0], min[1], avg1[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], min[1], avg1[2]), uint(vertices.size()))); //19
        mesh.vert_add(vec3d(max[0], min[1], avg1[2]));
    }
    if (vertices.find(vec3d(min[0], avg1[1], avg1[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], avg1[1], avg1[2]), uint(vertices.size()))); //20
        mesh.vert_add(vec3d(min[0], avg1[1], avg1[2]));
    }
    if (vertices.find(vec3d(avg1[0], avg1[1], avg1[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], avg1[1], avg1[2]), uint(vertices.size()))); //21
        mesh.vert_add(vec3d(avg1[0], avg1[1], avg1[2]));
    }
    if (vertices.find(vec3d(avg2[0], avg1[1], avg1[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], avg1[1], avg1[2]), uint(vertices.size()))); //22
        mesh.vert_add(vec3d(avg2[0], avg1[1], avg1[2]));
    }
    if (vertices.find(vec3d(max[0], avg1[1], avg1[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], avg1[1], avg1[2]), uint(vertices.size()))); //23
        mesh.vert_add(vec3d(max[0], avg1[1], avg1[2]));
    }
    if (vertices.find(vec3d(min[0], avg2[1], avg1[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], avg2[1], avg1[2]), uint(vertices.size()))); //24
        mesh.vert_add(vec3d(min[0], avg2[1], avg1[2]));
    }
    if (vertices.find(vec3d(avg1[0], avg2[1], avg1[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], avg2[1], avg1[2]), uint(vertices.size()))); //25
        mesh.vert_add(vec3d(avg1[0], avg2[1], avg1[2]));
    }
    if (vertices.find(vec3d(avg2[0], avg2[1], avg1[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], avg2[1], avg1[2]), uint(vertices.size()))); //26
        mesh.vert_add(vec3d(avg2[0], avg2[1], avg1[2]));
    }
    if (vertices.find(vec3d(max[0], avg2[1], avg1[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], avg2[1], avg1[2]), uint(vertices.size()))); //27
        mesh.vert_add(vec3d(max[0], avg2[1], avg1[2]));
    }
    if (vertices.find(vec3d(min[0], max[1], avg1[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], max[1], avg1[2]), uint(vertices.size()))); //28
        mesh.vert_add(vec3d(min[0], max[1], avg1[2]));
    }
    if (vertices.find(vec3d(avg1[0], max[1], avg1[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], max[1], avg1[2]), uint(vertices.size()))); //29
        mesh.vert_add(vec3d(avg1[0], max[1], avg1[2]));
    }
    if (vertices.find(vec3d(avg2[0], max[1], avg1[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], max[1], avg1[2]), uint(vertices.size()))); //30
        mesh.vert_add(vec3d(avg2[0], max[1], avg1[2]));
    }
    if (vertices.find(vec3d(max[0], max[1], avg1[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], max[1], avg1[2]), uint(vertices.size()))); //31
        mesh.vert_add(vec3d(max[0], max[1], avg1[2]));
    }

    //z = avg2
    if (vertices.find(vec3d(min[0], min[1], avg2[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], min[1], avg2[2]), uint(vertices.size()))); //32
        mesh.vert_add(vec3d(min[0], min[1], avg2[2]));
    }
    if (vertices.find(vec3d(avg1[0], min[1], avg2[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], min[1], avg2[2]), uint(vertices.size()))); //33
        mesh.vert_add(vec3d(avg1[0], min[1], avg2[2]));
    }
    if (vertices.find(vec3d(avg2[0], min[1], avg2[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], min[1], avg2[2]), uint(vertices.size()))); //34
        mesh.vert_add(vec3d(avg2[0], min[1], avg2[2]));
    }
    if (vertices.find(vec3d(max[0], min[1], avg2[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], min[1], avg2[2]), uint(vertices.size()))); //35
        mesh.vert_add(vec3d(max[0], min[1], avg2[2]));
    }
    if (vertices.find(vec3d(min[0], avg1[1], avg2[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], avg1[1], avg2[2]), uint(vertices.size()))); //36
        mesh.vert_add(vec3d(min[0], avg1[1], avg2[2]));
    }
    if (vertices.find(vec3d(avg1[0], avg1[1], avg2[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], avg1[1], avg2[2]), uint(vertices.size()))); //37
        mesh.vert_add(vec3d(avg1[0], avg1[1], avg2[2]));
    }
    if (vertices.find(vec3d(avg2[0], avg1[1], avg2[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], avg1[1], avg2[2]), uint(vertices.size()))); //38
        mesh.vert_add(vec3d(avg2[0], avg1[1], avg2[2]));
    }
    if (vertices.find(vec3d(max[0], avg1[1], avg2[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], avg1[1], avg2[2]), uint(vertices.size()))); //39
        mesh.vert_add(vec3d(max[0], avg1[1], avg2[2]));
    }
    if (vertices.find(vec3d(min[0], avg2[1], avg2[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], avg2[1], avg2[2]), uint(vertices.size()))); //40
        mesh.vert_add(vec3d(min[0], avg2[1], avg2[2]));
    }
    if (vertices.find(vec3d(avg1[0], avg2[1], avg2[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], avg2[1], avg2[2]), uint(vertices.size()))); //41
        mesh.vert_add(vec3d(avg1[0], avg2[1], avg2[2]));
    }
    if (vertices.find(vec3d(avg2[0], avg2[1], avg2[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], avg2[1], avg2[2]), uint(vertices.size()))); //42
        mesh.vert_add(vec3d(avg2[0], avg2[1], avg2[2]));
    }
    if (vertices.find(vec3d(max[0], avg2[1], avg2[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], avg2[1], avg2[2]), uint(vertices.size()))); //43
        mesh.vert_add(vec3d(max[0], avg2[1], avg2[2]));
    }
    if (vertices.find(vec3d(min[0], max[1], avg2[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], max[1], avg2[2]), uint(vertices.size()))); //44
        mesh.vert_add(vec3d(min[0], max[1], avg2[2]));
    }
    if (vertices.find(vec3d(avg1[0], max[1], avg2[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], max[1], avg2[2]), uint(vertices.size()))); //45
        mesh.vert_add(vec3d(avg1[0], max[1], avg2[2]));
    }
    if (vertices.find(vec3d(avg2[0], max[1], avg2[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], max[1], avg2[2]), uint(vertices.size()))); //46
        mesh.vert_add(vec3d(avg2[0], max[1], avg2[2]));
    }
    if (vertices.find(vec3d(max[0], max[1], avg2[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], max[1], avg2[2]), uint(vertices.size()))); //47
        mesh.vert_add(vec3d(max[0], max[1], avg2[2]));
    }

    //z = max
    if (vertices.find(vec3d(min[0], min[1], max[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], min[1], max[2]), uint(vertices.size()))); //48
        mesh.vert_add(vec3d(min[0], min[1], max[2]));
    }
    if (vertices.find(vec3d(avg1[0], min[1], max[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], min[1], max[2]), uint(vertices.size()))); //49
        mesh.vert_add(vec3d(avg1[0], min[1], max[2]));
    }
    if (vertices.find(vec3d(avg2[0], min[1], max[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], min[1], max[2]), uint(vertices.size()))); //50
        mesh.vert_add(vec3d(avg2[0], min[1], max[2]));
    }
    if (vertices.find(vec3d(max[0], min[1], max[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], min[1], max[2]), uint(vertices.size()))); //51
        mesh.vert_add(vec3d(max[0], min[1], max[2]));
    }
    if (vertices.find(vec3d(min[0], avg1[1], max[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], avg1[1], max[2]), uint(vertices.size()))); //52
        mesh.vert_add(vec3d(min[0], avg1[1], max[2]));
    }
    if (vertices.find(vec3d(avg1[0], avg1[1], max[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], avg1[1], max[2]), uint(vertices.size()))); //53
        mesh.vert_add(vec3d(avg1[0], avg1[1], max[2]));
    }
    if (vertices.find(vec3d(avg2[0], avg1[1], max[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], avg1[1], max[2]), uint(vertices.size()))); //54
        mesh.vert_add(vec3d(avg2[0], avg1[1], max[2]));
    }
    if (vertices.find(vec3d(max[0], avg1[1], max[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], avg1[1], max[2]), uint(vertices.size()))); //55
        mesh.vert_add(vec3d(max[0], avg1[1], max[2]));
    }
    if (vertices.find(vec3d(min[0], avg2[1], max[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], avg2[1], max[2]), uint(vertices.size()))); //56
        mesh.vert_add(vec3d(min[0], avg2[1], max[2]));
    }
    if (vertices.find(vec3d(avg1[0], avg2[1], max[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], avg2[1], max[2]), uint(vertices.size()))); //57
        mesh.vert_add(vec3d(avg1[0], avg2[1], max[2]));
    }
    if (vertices.find(vec3d(avg2[0], avg2[1], max[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], avg2[1], max[2]), uint(vertices.size()))); //58
        mesh.vert_add(vec3d(avg2[0], avg2[1], max[2]));
    }
    if (vertices.find(vec3d(max[0], avg2[1], max[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], avg2[1], max[2]), uint(vertices.size()))); //59
        mesh.vert_add(vec3d(max[0], avg2[1], max[2]));
    }
    if (vertices.find(vec3d(min[0], max[1], max[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], max[1], max[2]), uint(vertices.size()))); //60
        mesh.vert_add(vec3d(min[0], max[1], max[2]));
    }
    if (vertices.find(vec3d(avg1[0], max[1], max[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], max[1], max[2]), uint(vertices.size()))); //61
        mesh.vert_add(vec3d(avg1[0], max[1], max[2]));
    }
    if (vertices.find(vec3d(avg2[0], max[1], max[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], max[1], max[2]), uint(vertices.size()))); //62
        mesh.vert_add(vec3d(avg2[0], max[1], max[2]));
    }
    if (vertices.find(vec3d(max[0], max[1], max[2])) == vertices.end()){
        vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], max[1], max[2]), uint(vertices.size()))); //63
        mesh.vert_add(vec3d(max[0], max[1], max[2]));
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


    VolumeMeshControlPanel<DrawableHexmesh<>> panel(&mesh, &gui);
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});

    return a.exec();
}
