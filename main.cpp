#include "mainwindow.h"
#include <QApplication>

#include <cinolib/gui/qt/qt_gui_tools.h>
#include <cinolib/meshes/meshes.h>
#include <cinolib/color.h>
#include <cinolib/geometry/drawable_aabb.h>


int main(int argc, char *argv[])
{


    using namespace cinolib;
    QApplication a(argc, argv);
    //std::string s = (argc>1) ? std::string(argv[1]) : std::string(DATA_PATH) + "/airplane1_input_tri.obj";

    std::vector<vec3d> list(4);
    std::vector<AABB> bb;

    vec3<double> v1(1., 1., 1.);
    vec3<double> v2(1., 4., 1.);
    vec3<double> v3(4., 4., 1.);
    vec3<double> v4(4., 1., 1.);

    list[0] = v1;
    list[1] = v2;
    list[2] = v3;
    list[3] = v4;

    AABB tree(list);

    bb.push_back(tree);

    DrawableAABB m(bb);

    GLcanvas canvas;
    canvas.push_obj(&m);
    canvas.show();


    // CMD+1 to show mesh controls.
    //VolumeMeshControlPanel<DrawableOctree<>> panel(&m, &gui);
    //QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});


    return a.exec();
}
