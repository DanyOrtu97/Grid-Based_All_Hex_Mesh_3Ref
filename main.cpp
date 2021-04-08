#include "mainwindow.h"
#include <QApplication>

#include <cinolib/gui/qt/qt_gui_tools.h>
#include <cinolib/meshes/meshes.h>
#include <cinolib/color.h>


int main(int argc, char *argv[])
{

    using namespace cinolib;
    QApplication a(argc, argv);

    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/rockerarm.mesh";

    DrawableHexmesh<> hex(s.c_str());

    hex.edge_set_color(Color::BLUE());
    GLcanvas gui;
    gui.push_obj(&hex);
    gui.show();


    // CMD+1 to show mesh controls.
    //VolumeMeshControlPanel<DrawableOctree<>> panel(&m, &gui);
    //QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});


    return a.exec();
}
