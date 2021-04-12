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

    /*
     * insert mesh into octree
     */
    /*
    std::string ss = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/bunny.obj";
    DrawableTrimesh<> mesh(ss.c_str());

    DrawableTwseventree octree(7, 100);
    octree.build_from_mesh_polys(mesh);
    octree.updateGL();

    GLcanvas gui;
    gui.push_obj(&mesh);
    gui.push_obj(&octree);
    gui.show();
    */


    vec3d v1(1., 1., 1.);
    vec3d v2(1., 4., 1.);
    vec3d v3(4., 4., 1.);
    vec3d v4(4., 1., 1.);
    vec3d v1b(1., 1., 4.);
    vec3d v2b(1., 4., 4.);
    vec3d v3b(4., 4., 4.);
    vec3d v4b(4., 1., 4.);


    DrawableTwseventree twseventree(7, 100);
    twseventree.push_point(1, v1);
    twseventree.push_point(2, v2);
    twseventree.push_point(3, v3);
    twseventree.push_point(4, v4);
    twseventree.push_point(5, v1b);
    twseventree.push_point(6, v2b);
    twseventree.push_point(7, v3b);
    twseventree.push_point(8, v4b);

    twseventree.build();

    twseventree.subdivide(twseventree.root);

    twseventree.updateGL();

    GLcanvas canvas;
    canvas.push_obj(&twseventree); //OCTREE

    canvas.show();


    // CMD+1 to show mesh controls.
    //SurfaceMeshControlPanel<DrawableTrimesh<>> panel(&mesh, &gui);
    //QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});


    return a.exec();
}
