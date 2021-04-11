#include "mainwindow.h"
#include <QApplication>

#include <cinolib/gui/qt/qt_gui_tools.h>
#include <cinolib/meshes/meshes.h>
#include <cinolib/color.h>
#include <cinolib/geometry/drawable_aabb.h>
#include <cinolib/drawable_octree.h>

int main(int argc, char *argv[])
{
    using namespace cinolib;
    QApplication a(argc, argv);
    //std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/airplane1_input_tri.obj";

    vec3<double> v1(1., 1., 1.);
    vec3<double> v2(1., 4., 1.);
    vec3<double> v3(4., 4., 1.);
    vec3<double> v4(4., 1., 1.);
    vec3<double> v1b(1., 1., 4.);
    vec3<double> v2b(1., 4., 4.);
    vec3<double> v3b(4., 4., 4.);
    vec3<double> v4b(4., 1., 4.);

    //BB creation

    std::vector<vec3d> list(8);
    std::vector<AABB> bb;

    list[0] = v1;
    list[1] = v2;
    list[2] = v3;
    list[3] = v4;
    list[4] = v1b;
    list[5] = v2b;
    list[6] = v3b;
    list[7] = v4b;

    AABB tree(list);

    bb.push_back(tree);

    DrawableAABB b(bb);

    b.set_color(Color::RED());
    b.set_thickness(3.);


    //Octree creation
    Octree *octree = new Octree();

    //DrawablePolygonmesh<> mesh(s.c_str());
    octree->push_point(1, v1);
    octree->push_point(2, v2);
    octree->push_point(3, v3);
    octree->push_point(4, v4);
    octree->push_point(5, v1b);
    octree->push_point(6, v2b);
    octree->push_point(7, v3b);
    octree->push_point(8, v4b);

    octree->build();


    DrawableOctree *drOctree = new DrawableOctree();
    drOctree->updateGL(octree->root);

    octree->subdivide(octree->root);
    drOctree->updateGL(octree->root);
    octree->subdivide(octree->root->children[2]);
    drOctree->updateGL(octree->root);
    octree->subdivide(octree->root->children[2]->children[2]);
    drOctree->updateGL(octree->root);

    GLcanvas canvas;
    canvas.push_obj(&b); //BB
    canvas.push_obj(drOctree); //OCTREE


    canvas.show();
    // CMD+1 to show mesh controls.
    //VolumeMeshControlPanel<DrawableOctree<>> panel(drOctree, &canvas);
    //QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &canvas), &QShortcut::activated, [&](){panel.show();});


    return a.exec();
}
