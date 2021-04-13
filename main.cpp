#include "mainwindow.h"
#include <QApplication>

#include <cinolib/gui/qt/qt_gui_tools.h>
#include <cinolib/meshes/meshes.h>
#include <cinolib/color.h>
#include <cinolib/drawable_octree.h>
#include <drawable_twseventree.h>


int main(int argc, char *argv[])
{
    using namespace cinolib;
    QApplication a(argc, argv);

    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/cube.mesh";
    DrawableHexmesh<> mesh(s.c_str());

    std::vector<vec3d>  verts;
    std::vector<std::vector<uint>> polys;


    /*
    uint nv = verts.size();
    uint np = polys.size();
    this->verts.reserve(nv);
    this->polys.reserve(np);
    this->v2v.reserve(nv);
    this->v2e.reserve(nv);
    this->v2f.reserve(nv);
    this->v2p.reserve(nv);
    this->p2v.reserve(np);
    this->p2e.reserve(np);
    this->p2p.reserve(np);
    this->v_data.reserve(nv);
    this->p_data.reserve(np);
    this->polys_face_winding.reserve(np);
    */
    for(auto v : verts) mesh.vert_add(v);
    for(auto p : polys) mesh.poly_add(p);


    GLcanvas gui;
    gui.push_obj(&mesh);
    gui.show();



    //SurfaceMeshControlPanel<DrawableTrimesh<>> panel(&mesh, &gui);
    //QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});

    return a.exec();
}
