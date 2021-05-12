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
#include <cinolib/connected_components.h>
#include <cinolib/io/io_utilities.h>


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
void read_balancing(const bool                         weakly,
                          DrawableHexmesh<M,V,E,F,P> & mesh,
                          std::map<vec3d, uint>      & vertices,
                          std::vector<bool>          & transition_verts,
                          std::vector<uint>          & transition_faces){

    std::vector<int> poly_labels = mesh.vector_poly_labels();

    std::vector<uint> adj_p2p;

    if (weakly){
        for(uint pid=0; pid<poly_labels.size(); ++pid){
            adj_p2p = mesh.adj_p2p(pid);

            if(poly_labels[pid] == 2){
                for(auto el: adj_p2p) if(poly_labels[el] == 0) poly_labels[el] = 1;
            }
        }
    }
    else{
        // still to do
    }

    mesh.poly_apply_labels(poly_labels);
    mesh.poly_color_wrt_label();
    mesh.updateGL();
    read(mesh, vertices, transition_verts, transition_faces);
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
template<class M, class V, class E, class F, class P>
CINO_INLINE
void read(DrawableHexmesh<M,V,E,F,P> & mesh,
          std::map<vec3d, uint>      & vertices,
          std::vector<bool>          & transition_verts,
          std::vector<uint>          & transition_faces){


    std::vector<int> poly_labels = mesh.vector_poly_labels();
    int max = *std::max_element(poly_labels.begin(), poly_labels.end());

    std::vector<uint> vector_pid;

    if(max > 0){
        std::cout<< "max = " << max << std::endl;

        for(uint pid=0; pid<poly_labels.size(); ++pid){
            /*
             * Add controls for fill the 1x1x1 holes
             */

            if(poly_labels[pid] >= 1){

                split27(pid, mesh, vertices, transition_verts, transition_faces);

                vector_pid.push_back(pid);

            }
            std::cout<< "pid : " << pid << " [ " << (pid * 100)/(poly_labels.size()) << "% ]" <<std::endl;
        }

        std::vector<int> new_poly_labels = mesh.vector_poly_labels();

        for (auto el: vector_pid) new_poly_labels[el] = poly_labels[el] - 1;

        int change=-1;
        for (uint p= (new_poly_labels.size() - (vector_pid.size()*26)); p<new_poly_labels.size(); p++){
            if(new_poly_labels[p] == -1){
                if((new_poly_labels.size() - p) % 26 == 0) change ++;
                new_poly_labels[p] = poly_labels[vector_pid[change]] - 1;
            }
        }

        mesh.poly_apply_labels(new_poly_labels);
        mesh.updateGL();

        read(mesh, vertices, transition_verts, transition_faces);
    }
    else return;
}




//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//Split the polygon of id "pid" of the input mesh into 27 cubes
template<class M, class V, class E, class F, class P>
CINO_INLINE
void split27(const uint                                   pid,
             DrawableHexmesh<M,V,E,F,P>                 & mesh,
             std::map<vec3d, uint>                      & vertices,
             std::vector<bool>                          & transition_verts,
             std::vector<uint>                          & transition_faces){

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


    //insert vertices in map and mesh
    for (auto v : newverts){
        if (vertices.find(v) == vertices.end()){
            uint fresh_vid = mesh.vert_add(v);
            vertices[v] = fresh_vid;
            transition_verts.push_back(false);
        }

    }

    //calculate vertices and faces to apply templates
    if(mesh.num_verts() > 64){
        for(auto vid: mesh.poly_verts_id(pid)) transition_verts[vid] = true;
        for(auto fid: mesh.poly_faces_id(pid)) transition_faces.push_back(fid);

    }

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    //Insert polys
    polys[0] = {vertices.find(vec3d(min[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], min[1], min[2]))->second,
                vertices.find(vec3d(min[0], min[1], min[2]))->second,
                vertices.find(vec3d(min[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(min[0], avg1[1], min[2]))->second};

    polys[1] = {vertices.find(vec3d(avg1[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], min[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], min[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], min[2]))->second};

    polys[2] = {vertices.find(vec3d(avg2[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], min[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], min[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], min[2]))->second};

    polys[3] = {vertices.find(vec3d(min[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(min[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(min[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(min[0], avg2[1], min[2]))->second};

    polys[4] = {vertices.find(vec3d(avg1[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], min[2]))->second};

    polys[5] = {vertices.find(vec3d(avg2[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], avg1[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], min[2]))->second};

    polys[6] = {vertices.find(vec3d(min[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(min[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(min[0], max[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], max[1], avg1[2]))->second,
                vertices.find(vec3d(avg1[0], max[1], min[2]))->second,
                vertices.find(vec3d(min[0], max[1], min[2]))->second};

    polys[7] = {vertices.find(vec3d(avg1[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], max[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], max[1], avg1[2]))->second,
                vertices.find(vec3d(avg2[0], max[1], min[2]))->second,
                vertices.find(vec3d(avg1[0], max[1], min[2]))->second};

    polys[8] = {vertices.find(vec3d(avg2[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], avg2[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], avg2[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], max[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], max[1], avg1[2]))->second,
                vertices.find(vec3d(max[0], max[1], min[2]))->second,
                vertices.find(vec3d(avg2[0], max[1], min[2]))->second};

    polys[9] = {vertices.find(vec3d(min[0], min[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], min[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(min[0], min[1], avg1[2]))->second,
                vertices.find(vec3d(min[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg2[2]))->second,
                vertices.find(vec3d(avg1[0], avg1[1], avg1[2]))->second,
                vertices.find(vec3d(min[0], avg1[1], avg1[2]))->second};

    polys[10] = {vertices.find(vec3d(avg1[0], min[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], min[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], min[1], avg1[2]))->second,
                 vertices.find(vec3d(avg1[0], min[1], avg1[2]))->second,
                 vertices.find(vec3d(avg1[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], avg1[1], avg1[2]))->second,
                 vertices.find(vec3d(avg1[0], avg1[1], avg1[2]))->second};

    polys[11] = {vertices.find(vec3d(avg2[0], min[1], avg2[2]))->second,
                 vertices.find(vec3d(max[0], min[1], avg2[2]))->second,
                 vertices.find(vec3d(max[0], min[1], avg1[2]))->second,
                 vertices.find(vec3d(avg2[0], min[1], avg1[2]))->second,
                 vertices.find(vec3d(avg2[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(max[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(max[0], avg1[1], avg1[2]))->second,
                 vertices.find(vec3d(avg2[0], avg1[1], avg1[2]))->second};

    polys[12] = {vertices.find(vec3d(min[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(avg1[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(avg1[0], avg1[1], avg1[2]))->second,
                 vertices.find(vec3d(min[0], avg1[1], avg1[2]))->second,
                 vertices.find(vec3d(min[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(avg1[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(avg1[0], avg2[1], avg1[2]))->second,
                 vertices.find(vec3d(min[0], avg2[1], avg1[2]))->second};

    polys[13] = {vertices.find(vec3d(avg1[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], avg1[1], avg1[2]))->second,
                 vertices.find(vec3d(avg1[0], avg1[1], avg1[2]))->second,
                 vertices.find(vec3d(avg1[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], avg2[1], avg1[2]))->second,
                 vertices.find(vec3d(avg1[0], avg2[1], avg1[2]))->second};

    polys[14] = {vertices.find(vec3d(avg2[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(max[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(max[0], avg1[1], avg1[2]))->second,
                 vertices.find(vec3d(avg2[0], avg1[1], avg1[2]))->second,
                 vertices.find(vec3d(avg2[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(max[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(max[0], avg2[1], avg1[2]))->second,
                 vertices.find(vec3d(avg2[0], avg2[1], avg1[2]))->second};

    polys[15] = {vertices.find(vec3d(min[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(avg1[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(avg1[0], avg2[1], avg1[2]))->second,
                 vertices.find(vec3d(min[0], avg2[1], avg1[2]))->second,
                 vertices.find(vec3d(min[0], max[1], avg2[2]))->second,
                 vertices.find(vec3d(avg1[0], max[1], avg2[2]))->second,
                 vertices.find(vec3d(avg1[0], max[1], avg1[2]))->second,
                 vertices.find(vec3d(min[0], max[1], avg1[2]))->second};

    polys[16] = {vertices.find(vec3d(avg1[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], avg2[1], avg1[2]))->second,
                 vertices.find(vec3d(avg1[0], avg2[1], avg1[2]))->second,
                 vertices.find(vec3d(avg1[0], max[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], max[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], max[1], avg1[2]))->second,
                 vertices.find(vec3d(avg1[0], max[1], avg1[2]))->second};

    polys[17] = {vertices.find(vec3d(avg2[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(max[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(max[0], avg2[1], avg1[2]))->second,
                 vertices.find(vec3d(avg2[0], avg2[1], avg1[2]))->second,
                 vertices.find(vec3d(avg2[0], max[1], avg2[2]))->second,
                 vertices.find(vec3d(max[0], max[1], avg2[2]))->second,
                 vertices.find(vec3d(max[0], max[1], avg1[2]))->second,
                 vertices.find(vec3d(avg2[0], max[1], avg1[2]))->second};

    polys[18] = {vertices.find(vec3d(min[0], min[1], max[2]))->second,
                 vertices.find(vec3d(avg1[0], min[1], max[2]))->second,
                 vertices.find(vec3d(avg1[0], min[1], avg2[2]))->second,
                 vertices.find(vec3d(min[0], min[1], avg2[2]))->second,
                 vertices.find(vec3d(min[0], avg1[1], max[2]))->second,
                 vertices.find(vec3d(avg1[0], avg1[1], max[2]))->second,
                 vertices.find(vec3d(avg1[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(min[0], avg1[1], avg2[2]))->second};

    polys[19] = {vertices.find(vec3d(avg1[0], min[1], max[2]))->second,
                 vertices.find(vec3d(avg2[0], min[1], max[2]))->second,
                 vertices.find(vec3d(avg2[0], min[1], avg2[2]))->second,
                 vertices.find(vec3d(avg1[0], min[1], avg2[2]))->second,
                 vertices.find(vec3d(avg1[0], avg1[1], max[2]))->second,
                 vertices.find(vec3d(avg2[0], avg1[1], max[2]))->second,
                 vertices.find(vec3d(avg2[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(avg1[0], avg1[1], avg2[2]))->second};

    polys[20] = {vertices.find(vec3d(avg2[0], min[1], max[2]))->second,
                 vertices.find(vec3d(max[0], min[1], max[2]))->second,
                 vertices.find(vec3d(max[0], min[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], min[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], avg1[1], max[2]))->second,
                 vertices.find(vec3d(max[0], avg1[1], max[2]))->second,
                 vertices.find(vec3d(max[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], avg1[1], avg2[2]))->second};

    polys[21] = {vertices.find(vec3d(min[0], avg1[1], max[2]))->second,
                 vertices.find(vec3d(avg1[0], avg1[1], max[2]))->second,
                 vertices.find(vec3d(avg1[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(min[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(min[0], avg2[1], max[2]))->second,
                 vertices.find(vec3d(avg1[0], avg2[1], max[2]))->second,
                 vertices.find(vec3d(avg1[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(min[0], avg2[1], avg2[2]))->second};

    polys[22] = {vertices.find(vec3d(avg1[0], avg1[1], max[2]))->second,
                 vertices.find(vec3d(avg2[0], avg1[1], max[2]))->second,
                 vertices.find(vec3d(avg2[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(avg1[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(avg1[0], avg2[1], max[2]))->second,
                 vertices.find(vec3d(avg2[0], avg2[1], max[2]))->second,
                 vertices.find(vec3d(avg2[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(avg1[0], avg2[1], avg2[2]))->second};

    polys[23] = {vertices.find(vec3d(avg2[0], avg1[1], max[2]))->second,
                 vertices.find(vec3d(max[0], avg1[1], max[2]))->second,
                 vertices.find(vec3d(max[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], avg1[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], avg2[1], max[2]))->second,
                 vertices.find(vec3d(max[0], avg2[1], max[2]))->second,
                 vertices.find(vec3d(max[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], avg2[1], avg2[2]))->second};

    polys[24] = {vertices.find(vec3d(min[0], avg2[1], max[2]))->second,
                 vertices.find(vec3d(avg1[0], avg2[1], max[2]))->second,
                 vertices.find(vec3d(avg1[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(min[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(min[0], max[1], max[2]))->second,
                 vertices.find(vec3d(avg1[0], max[1], max[2]))->second,
                 vertices.find(vec3d(avg1[0], max[1], avg2[2]))->second,
                 vertices.find(vec3d(min[0], max[1], avg2[2]))->second};

    polys[25] = {vertices.find(vec3d(avg1[0], avg2[1], max[2]))->second,
                 vertices.find(vec3d(avg2[0], avg2[1], max[2]))->second,
                 vertices.find(vec3d(avg2[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(avg1[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(avg1[0], max[1], max[2]))->second,
                 vertices.find(vec3d(avg2[0], max[1], max[2]))->second,
                 vertices.find(vec3d(avg2[0], max[1], avg2[2]))->second,
                 vertices.find(vec3d(avg1[0], max[1], avg2[2]))->second};

    polys[26] = {vertices.find(vec3d(avg2[0], avg2[1], max[2]))->second,
                 vertices.find(vec3d(max[0], avg2[1], max[2]))->second,
                 vertices.find(vec3d(max[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], avg2[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], max[1], max[2]))->second,
                 vertices.find(vec3d(max[0], max[1], max[2]))->second,
                 vertices.find(vec3d(max[0], max[1], avg2[2]))->second,
                 vertices.find(vec3d(avg2[0], max[1], avg2[2]))->second};


    for(auto p : polys) mesh.poly_add(p);

    mesh.poly_remove(pid);

    mesh.updateGL();
}

}

int main(int argc, char *argv[])
{
    using namespace cinolib;
    QApplication a(argc, argv);

    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/exp_4.mesh";
    DrawableHexmesh<> mesh(s.c_str());


    DrawableHexmesh<> outputMesh;

    //vertices
    std::map<vec3d, uint> vertices;


    //vectors for templates application
    std::vector<bool> transition_verts;
    std::vector<uint> transition_faces;


    //when the mesh has numverts > 8
    for (auto v: mesh.vector_verts()){
        vertices.insert(std::pair<vec3d, uint>(v, vertices.size()));
        transition_verts.push_back(false);
    }

    //mesh.poly_fix_orientation();
    read_balancing(true, mesh, vertices, transition_verts, transition_faces);


    QWidget window;
    GLcanvas gui_input, gui_output;
    QHBoxLayout layout;
    layout.addWidget(&gui_input);
    layout.addWidget(&gui_output);
    window.setLayout(&layout);
    window.resize(1000,600);
    window.show();

    gui_input.push_marker(vec2i(10, gui_input.height()-20), "Hexmesh before templates application", Color::BLACK(), 12, 0);
    gui_output.push_marker(vec2i(10, gui_input.height()-20), "Hexmesh after templates application (hanging nodes solved)", Color::BLACK(), 12, 0);
    gui_input.push_obj(&mesh);

    /*
     * Tool for creating new polys by mouse click
     */

    /*
    Profiler profiler;

    gui_input.push_marker(vec2i(10, gui_input.height()-20), "Ctrl + click to split a poly into 27 elements", Color::BLACK(), 12, 0);

    gui_input.callback_mouse_press = [&](GLcanvas *c, QMouseEvent *e)
    {
        if (e->modifiers() == Qt::ControlModifier)
        {
            vec3d p;
            vec2i click(e->x(), e->y());

            if (c->unproject(click, p))
            {
                std::cout<< "###############################" <<std::endl;

                profiler.push("Pick Polys");

                uint pid = mesh.pick_poly(p);
                profiler.pop();


                //chrono for subdivision
                std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();

                split27(pid, mesh, vertices, transition_verts, transition_faces);

                std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

                std::cout<<"Subdivide Poly " << pid << " into 27 Polys [" << how_many_seconds(t0, t1) << "]" << std::endl;

                mesh.updateGL();


                //chrono for template's application
                std::chrono::high_resolution_clock::time_point t0o = std::chrono::high_resolution_clock::now();

                if(mesh.num_verts() >= 64) hex_transition_install_3ref(mesh, transition_verts, transition_faces, outputMesh);

                std::chrono::high_resolution_clock::time_point t1o = std::chrono::high_resolution_clock::now();

                std::cout << std::endl;
                std::cout << "Applied 3 refinement templates into the mesh : " << outputMesh.num_verts() << "V / " <<
                                                                                  outputMesh.num_edges() << "E / " <<
                                                                                  outputMesh.num_faces() << "F / " <<
                                                                                  outputMesh.num_polys() << "P  [" <<
                                                                                  how_many_seconds(t0o,t1o) << "s]" << std::endl;

                gui_output.push_obj(&outputMesh);


                outputMesh.updateGL();
                outputMesh.print_quality(); //scaled jacobian

                std::cout<< "N° componenti connesse: " << connected_components(outputMesh) <<std::endl;

                c->updateGL();

            }
        }
    };
    */



    mesh.print_quality();

    //chrono for template's application
    std::chrono::high_resolution_clock::time_point t0o = std::chrono::high_resolution_clock::now();

    if(mesh.num_verts() >= 64) hex_transition_install_3ref(mesh, transition_verts, transition_faces, outputMesh);

    std::chrono::high_resolution_clock::time_point t1o = std::chrono::high_resolution_clock::now();

    std::cout << std::endl;
    std::cout << "Applied 3 refinement templates into the mesh : " << outputMesh.num_verts() << "V / " <<
                                                                      outputMesh.num_edges() << "E / " <<
                                                                      outputMesh.num_faces() << "F / " <<
                                                                      outputMesh.num_polys() << "P  [" <<
                                                                      how_many_seconds(t0o,t1o) << "s]" << std::endl;

    gui_output.push_obj(&outputMesh);


    outputMesh.updateGL();
    outputMesh.print_quality(); //scaled jacobian

    std::cout<< "N° componenti connesse: " << connected_components(outputMesh) <<std::endl;



    VolumeMeshControlPanel<DrawableHexmesh<>> panel_input(&mesh, &gui_input);
    VolumeMeshControlPanel<DrawableHexmesh<>> panel_output(&outputMesh, &gui_output);
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui_input), &QShortcut::activated, [&](){panel_input.show();});
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui_output), &QShortcut::activated, [&](){panel_output.show();});

    return a.exec();
}
