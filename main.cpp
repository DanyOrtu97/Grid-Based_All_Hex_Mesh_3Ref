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

    int max_label = *std::max_element(poly_labels.begin(), poly_labels.end());
    std::vector<uint> adj_p2p;

    // we need a more loop to handle all difficult cases
    while(max_label > 0){
        poly_labels = mesh.vector_poly_labels();
        new_poly_labels = mesh.vector_poly_labels();
        if (weakly){
            for(uint pid=0; pid<poly_labels.size(); ++pid){
                if(poly_labels[pid] == max_label){
                    adj_p2p = mesh.adj_p2p(pid);

                    for(auto el: adj_p2p) if(poly_labels[pid] - poly_labels[el] > 1) new_poly_labels[el] = poly_labels[pid] -1;
                }

            }
        }
        else{
            std::vector<uint> neighs;
            std::vector<uint> adj_e2p;
            std::vector<uint> adj_v2p;

            for(uint pid=0; pid<poly_labels.size(); ++pid){

                if(poly_labels[pid] == max_label){
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
                        new_poly_labels[el] = poly_labels[pid] -1;

                    }
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

    mesh.poly_remove(pid);
}

}


//:::::::::::::::::::::::::::::::::::MAIN:::::::::::::::::::::::::::::::::::::

int main(int argc, char *argv[])
{
    using namespace cinolib;
    QApplication a(argc, argv);

    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/exp_4.mesh";
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

                mesh.save("cube9x9.mesh");

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

        //outputSurfaceMesh.save("bunnyref2.obj");

    }


    //outputMesh.save("bunnyref2.mesh");


    VolumeMeshControlPanel<DrawableHexmesh<>> panel_input(&mesh, &gui_input);
    VolumeMeshControlPanel<DrawableHexmesh<>> panel_output(&outputMesh, &gui_output);
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui_input), &QShortcut::activated, [&](){panel_input.show();});
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui_output), &QShortcut::activated, [&](){panel_output.show();});

    return a.exec();
}
