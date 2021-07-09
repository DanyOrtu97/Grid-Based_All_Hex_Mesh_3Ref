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
#include <sys/types.h>
#include <cinolib/drawable_sphere.h>
#include <cinolib/how_many_seconds.h>
#include <hex_transition_install_3ref.h>
#include <cinolib/connected_components.h>
#include <cinolib/io/io_utilities.h>
#include <cinolib/export_surface.h>
#include <numeric>


namespace cinolib
{


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
template<class M, class V, class E, class F, class P>
CINO_INLINE
void balancing(const bool                         weakly,
                     Hexmesh<M,V,E,F,P>         & mesh){

    std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();

    std::vector<int> poly_labels = mesh.vector_poly_labels();
    std::vector<int> new_poly_labels = mesh.vector_poly_labels();

    std::vector<uint> neighs;

    std::vector<uint> adj_p2p;
    std::vector<uint> adj_e2p;
    std::vector<uint> adj_v2p;

    int max_label = *std::max_element(poly_labels.begin(), poly_labels.end());

    // we need a more loop to handle all difficult cases
    while(max_label > 0){
        poly_labels = mesh.vector_poly_labels();
        new_poly_labels = mesh.vector_poly_labels();
        if (weakly){
            for(uint pid=0; pid<poly_labels.size(); ++pid){
                adj_p2p = mesh.adj_p2p(pid);

                for(auto el: adj_p2p) if(poly_labels[pid] - poly_labels[el] > 1) new_poly_labels[el] = poly_labels[pid] -1;

            }
        }
        else{
            for(uint pid=0; pid<poly_labels.size(); ++pid){
                neighs.clear();
                adj_p2p = mesh.adj_p2p(pid);

                for(auto poly: adj_p2p) neighs.push_back(poly);


                for(auto edge: mesh.adj_p2e(pid)){
                    adj_e2p = mesh.adj_e2p(edge);
                    for(auto poly: adj_e2p) neighs.push_back(poly);
                }


                for(auto vertex: mesh.adj_p2v(pid)){
                    adj_v2p = mesh.adj_v2p(vertex);
                    for(auto poly: adj_v2p) neighs.push_back(poly);
                }

                std::sort(neighs.begin(), neighs.end());
                neighs.erase(std::unique(neighs.begin(), neighs.end()), neighs.end());

                for(auto el: neighs) if(poly_labels[pid] - poly_labels[el] > 1){
                    std::cout<<"Modificato"<<std::endl;
                    new_poly_labels[el] = poly_labels[pid] -1;

                }

            }

        }
        max_label--;
        mesh.poly_apply_labels(new_poly_labels);
    }


    mesh.poly_color_wrt_label();

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    std::cout << std::endl;
    std::cout << "Balancing of the mesh [" << how_many_seconds(t0,t1) << "s]" << std::endl;

}




//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
template<class M, class V, class E, class F, class P>
CINO_INLINE
void fill_holes(Hexmesh<M,V,E,F,P> & mesh){

    std::vector<int> poly_labels = mesh.vector_poly_labels();
    std::vector<std::pair<uint, int>> vec_conta_adj;

    std::vector<uint> adj_p2p;

    int conta_adj;

    for(uint pid=0; pid<poly_labels.size(); ++pid){
        conta_adj = 0;
        adj_p2p = mesh.adj_p2p(pid);
        for(auto el: adj_p2p) if(poly_labels[el] == poly_labels[pid] + 1) conta_adj++;

        vec_conta_adj.push_back(std::pair<uint, int>(pid, conta_adj));
    }

    for (auto el: vec_conta_adj) if(el.second == 5) poly_labels[el.first] ++;

    mesh.poly_apply_labels(poly_labels);
    mesh.poly_color_wrt_label();

}



//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
template<class M, class V, class E, class F, class P>
CINO_INLINE
void apply_refinements(Hexmesh<M,V,E,F,P>                       & mesh,
                       std::map<vec3d, uint, vert_compare>      & vertices,
                       std::vector<bool>                        & transition_verts){

    std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();

    std::vector<int> poly_labels = mesh.vector_poly_labels();
    int max = *std::max_element(poly_labels.begin(), poly_labels.end());
    std::vector<uint> vector_pid;


    for(int i = 0; i < max-1; i++){
        vector_pid.clear();
        std::cout << std::endl;
        std::cout<< "Refinements of level " << i+1 << std::endl;

        for(uint pid=0; pid<poly_labels.size(); ++pid){

            if(poly_labels[pid] >= 1){

                split27(pid, mesh, vertices, transition_verts);

                vector_pid.push_back(pid);

            }
            if (pid%1000==0) std::cout<< "pid : " << pid << " [ " << (pid * 100)/ (poly_labels.size()) << "% ]" <<std::endl;
        }


        //recalculate poly labels
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
        poly_labels = mesh.vector_poly_labels();

    }

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    std::cout << std::endl;
    std::cout << "Applied refinements [" << how_many_seconds(t0,t1) << "s]" << std::endl;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
template<class M, class V, class E, class F, class P>
CINO_INLINE
void split27(const uint                                   pid,
             Hexmesh<M,V,E,F,P>                         & mesh,
             std::map<vec3d, uint, vert_compare>        & vertices,
             std::vector<bool>                          & transition_verts){


    SchemeInfo info;

    info.type = HexTransition::FULL;
    info.scale = mesh.edge_length(mesh.adj_p2e(pid)[0]);


    std::vector<vec3d>              verts;
    std::vector<std::vector<uint>>  polys;

    vec3d poly_centroid = mesh.poly_centroid(pid);

    hex_transition_orient_3ref(verts, polys, info, poly_centroid);

    //merge vertices
    for (auto & v : verts){
        if (vertices.find(v) == vertices.end()){
            uint fresh_vid = mesh.vert_add(v);
            vertices[v] = fresh_vid;
            transition_verts.push_back(false);
        }
    }

    if(mesh.num_verts() > 64) for(auto vid: mesh.poly_verts_id(pid)) transition_verts[vid] = true;

    //merge polys
    for (uint poly=0; poly<polys.size(); ++poly){
        auto p = polys.at(poly);

        for (auto & vid: p) vid = vertices[verts.at(vid)];

        mesh.poly_add(p);
    }
    /*


    //vector for the new polys
    std::vector<std::vector<uint>> polys(27);

    //points for subdivision
    std::vector<vec3d> verts = mesh.poly_verts(pid);

    vec3d min = *std::min_element(verts.begin(), verts.end());
    vec3d max = *std::max_element(verts.begin(), verts.end());
    vec3d avg1 = min + ((max-min)/3);
    vec3d avg2 = avg1 + ((max-min)/3);

    std::vector<vec3d> newverts(64);
    std::vector<uint> newvertsid(64);

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
    int conta = 0;

    for (auto & v : newverts){
        std::map<vec3d, uint, vert_compare>::iterator pair = vertices.find(v);
        if (pair == vertices.end()){
            uint fresh_vid = mesh.vert_add(v);
            vertices[v] = fresh_vid;
            newvertsid[conta] = fresh_vid;
            transition_verts.push_back(false);
        }
        else newvertsid[conta] = pair->second;
        conta++;
    }

    //update vertices to apply templates
    if(mesh.num_verts() > 64) for(auto vid: mesh.poly_verts_id(pid)) transition_verts[vid] = true;


    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    //Insert polys
    polys[0] =  {newvertsid[16], newvertsid[17],  newvertsid[1],  newvertsid[0], newvertsid[20], newvertsid[21],  newvertsid[5],  newvertsid[4]};
    polys[1] =  {newvertsid[17], newvertsid[18],  newvertsid[2],  newvertsid[1], newvertsid[21], newvertsid[22],  newvertsid[6],  newvertsid[5]};
    polys[2] =  {newvertsid[18], newvertsid[19],  newvertsid[3],  newvertsid[2], newvertsid[22], newvertsid[23],  newvertsid[7],  newvertsid[6]};
    polys[3] =  {newvertsid[20], newvertsid[21],  newvertsid[5],  newvertsid[4], newvertsid[24], newvertsid[25],  newvertsid[9],  newvertsid[8]};
    polys[4] =  {newvertsid[21], newvertsid[22],  newvertsid[6],  newvertsid[5], newvertsid[25], newvertsid[26], newvertsid[10],  newvertsid[9]};
    polys[5] =  {newvertsid[22], newvertsid[23],  newvertsid[7],  newvertsid[6], newvertsid[26], newvertsid[27], newvertsid[11], newvertsid[10]};
    polys[6] =  {newvertsid[24], newvertsid[25],  newvertsid[9],  newvertsid[8], newvertsid[28], newvertsid[29], newvertsid[13], newvertsid[12]};
    polys[7] =  {newvertsid[25], newvertsid[26], newvertsid[10],  newvertsid[9], newvertsid[29], newvertsid[30], newvertsid[14], newvertsid[13]};
    polys[8] =  {newvertsid[26], newvertsid[27], newvertsid[11], newvertsid[10], newvertsid[30], newvertsid[31], newvertsid[15], newvertsid[14]};
    polys[9] =  {newvertsid[32], newvertsid[33], newvertsid[17], newvertsid[16], newvertsid[36], newvertsid[37], newvertsid[21], newvertsid[20]};
    polys[10] = {newvertsid[33], newvertsid[34], newvertsid[18], newvertsid[17], newvertsid[37], newvertsid[38], newvertsid[22], newvertsid[21]};
    polys[11] = {newvertsid[34], newvertsid[35], newvertsid[19], newvertsid[18], newvertsid[38], newvertsid[39], newvertsid[23], newvertsid[22]};
    polys[12] = {newvertsid[36], newvertsid[37], newvertsid[21], newvertsid[20], newvertsid[40], newvertsid[41], newvertsid[25], newvertsid[24]};
    polys[13] = {newvertsid[37], newvertsid[38], newvertsid[22], newvertsid[21], newvertsid[41], newvertsid[42], newvertsid[26], newvertsid[25]};
    polys[14] = {newvertsid[38], newvertsid[39], newvertsid[23], newvertsid[22], newvertsid[42], newvertsid[43], newvertsid[27], newvertsid[26]};
    polys[15] = {newvertsid[40], newvertsid[41], newvertsid[25], newvertsid[24], newvertsid[44], newvertsid[45], newvertsid[29], newvertsid[28]};
    polys[16] = {newvertsid[41], newvertsid[42], newvertsid[26], newvertsid[25], newvertsid[45], newvertsid[46], newvertsid[30], newvertsid[29]};
    polys[17] = {newvertsid[42], newvertsid[43], newvertsid[27], newvertsid[26], newvertsid[46], newvertsid[47], newvertsid[31], newvertsid[30]};
    polys[18] = {newvertsid[48], newvertsid[49], newvertsid[33], newvertsid[32], newvertsid[52], newvertsid[53], newvertsid[37], newvertsid[36]};
    polys[19] = {newvertsid[49], newvertsid[50], newvertsid[34], newvertsid[33], newvertsid[53], newvertsid[54], newvertsid[38], newvertsid[37]};
    polys[20] = {newvertsid[50], newvertsid[51], newvertsid[35], newvertsid[34], newvertsid[54], newvertsid[55], newvertsid[39], newvertsid[38]};
    polys[21] = {newvertsid[52], newvertsid[53], newvertsid[37], newvertsid[36], newvertsid[56], newvertsid[57], newvertsid[41], newvertsid[40]};
    polys[22] = {newvertsid[53], newvertsid[54], newvertsid[38], newvertsid[37], newvertsid[57], newvertsid[58], newvertsid[42], newvertsid[41]};
    polys[23] = {newvertsid[54], newvertsid[55], newvertsid[39], newvertsid[38], newvertsid[58], newvertsid[59], newvertsid[43], newvertsid[42]};
    polys[24] = {newvertsid[56], newvertsid[57], newvertsid[41], newvertsid[40], newvertsid[60], newvertsid[61], newvertsid[45], newvertsid[44]};
    polys[25] = {newvertsid[57], newvertsid[58], newvertsid[42], newvertsid[41], newvertsid[61], newvertsid[62], newvertsid[46], newvertsid[45]};
    polys[26] = {newvertsid[58], newvertsid[59], newvertsid[43], newvertsid[42], newvertsid[62], newvertsid[63], newvertsid[47], newvertsid[46]};


    for(auto p : polys) mesh.poly_add(p);

*/
    mesh.poly_remove(pid);
}

}


//:::::::::::::::::::::::::::::::::::MAIN:::::::::::::::::::::::::::::::::::::

int main(int argc, char *argv[])
{
    using namespace cinolib;
    QApplication a(argc, argv);

    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/bunnygrid.mesh";
    DrawableHexmesh<> mesh(s.c_str());
    DrawableHexmesh<> outputMesh;

    GLcanvas gui_input, gui_output;

    gui_input.push_marker(vec2i(10, gui_input.height()-20), "Hexmesh before templates application", Color::BLACK(), 12, 0);
    gui_output.push_marker(vec2i(10, gui_input.height()-20), "Hexmesh after templates application (hanging nodes solved)", Color::BLACK(), 12, 0);
    gui_input.show();
    gui_output.show();


    std::map<vec3d, uint, vert_compare> vertices;
    std::vector<bool> transition_verts;


    for (uint vid=0; vid<mesh.num_verts(); ++vid){
        vertices[mesh.vert(vid)] = vid;
        transition_verts.push_back(false);
    }


    balancing(false, mesh);
    mesh.updateGL();

    apply_refinements(mesh, vertices, transition_verts);


    mesh.print_quality();
    gui_output.push_obj(&outputMesh);
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

                split27(pid, mesh, vertices, transition_verts);

                std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

                std::cout<<"Subdivide Poly " << pid << " into 27 Polys [" << how_many_seconds(t0, t1) << "]" << std::endl;

                //mesh.save("cube6x6.mesh");

                //chrono for template's application
                std::chrono::high_resolution_clock::time_point t0o = std::chrono::high_resolution_clock::now();

                std::cout<< "Template application in progress ...." <<std::endl;

                hex_transition_install_3ref(mesh, transition_verts, outputMesh);


                for(uint iii=0; iii<outputMesh.num_verts(); iii++){
                    if(transition_verts[iii])
                        outputMesh.vert_data(iii).color = Color::RED();
                }


                std::chrono::high_resolution_clock::time_point t1o = std::chrono::high_resolution_clock::now();

                std::cout << std::endl;
                std::cout << "Applied 3 refinement templates into the mesh : " << outputMesh.num_verts() << "V / " <<
                                                                                  outputMesh.num_edges() << "E / " <<
                                                                                  outputMesh.num_faces() << "F / " <<
                                                                                  outputMesh.num_polys() << "P  [" <<
                                                                                  how_many_seconds(t0o,t1o) << "s]" << std::endl;

                gui_output.push_obj(&outputMesh);
                mesh.updateGL();
                outputMesh.updateGL();
                outputMesh.print_quality(); //scaled jacobian

                if(outputMesh.num_polys() > 0 ){
                    Quadmesh<> outputSurfaceMesh;

                    export_surface(outputMesh, outputSurfaceMesh);

                    std::cout<< "N° componenti connesse: " << connected_components(outputSurfaceMesh) <<std::endl;

                    bool manifold = true;

                    for (auto eid: outputSurfaceMesh.vector_edges()){
                        if(! outputSurfaceMesh.edge_is_manifold(eid))
                            manifold = false;
                    }

                    std::cout<< "Surface is manifold (must to be true/1): "<< manifold <<std::endl; //must to be true or 1
                    //outputSurfaceMesh.save("cube2manifold.obj");

                }

                c->updateGL();
            }
        }
    };



*/
    std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
    std::cout<<std::endl;
    std::cout<< "Template application in progress ...." <<std::endl;

    hex_transition_install_3ref(mesh, transition_verts, outputMesh);

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    std::cout << std::endl;
    std::cout << "Applied 3 refinement templates into the mesh : " << outputMesh.num_verts() << "V / " <<
                                                                      outputMesh.num_edges() << "E / " <<
                                                                      outputMesh.num_faces() << "F / " <<
                                                                      outputMesh.num_polys() << "P  [" <<
                                                                      how_many_seconds(t0,t1) << "s]" << std::endl;

    gui_output.push_obj(&outputMesh);
    gui_input.push_obj(&mesh);

    for(uint iii=0; iii<outputMesh.num_verts(); iii++){
        if(transition_verts[iii])
            outputMesh.vert_data(iii).color = Color::RED();
    }




    mesh.updateGL();
    outputMesh.updateGL();

    outputMesh.print_quality(); //scaled jacobian


    if(outputMesh.num_polys() > 0 ){
        Quadmesh<> outputSurfaceMesh;

        export_surface(outputMesh, outputSurfaceMesh);

        std::cout<< "N° componenti connesse: " << connected_components(outputSurfaceMesh) <<std::endl;


        bool manifold = true;

        for (auto eid: outputSurfaceMesh.vector_edges()){
            if(! outputSurfaceMesh.edge_is_manifold(eid))
                manifold = false;
        }

        std::cout<< "Surface is manifold (must to be true/1): "<< manifold <<std::endl; //must to be true or 1

        outputSurfaceMesh.save("bunnyref2.obj");

    }


    outputMesh.save("bunnyref2.mesh");

    VolumeMeshControlPanel<DrawableHexmesh<>> panel_input(&mesh, &gui_input);
    VolumeMeshControlPanel<DrawableHexmesh<>> panel_output(&outputMesh, &gui_output);
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui_input), &QShortcut::activated, [&](){panel_input.show();});
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui_output), &QShortcut::activated, [&](){panel_output.show();});

    return a.exec();
}
