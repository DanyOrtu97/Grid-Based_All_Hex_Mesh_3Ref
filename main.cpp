#include "mainwindow.h"
#include <QApplication>

#include <cinolib/gui/qt/qt_gui_tools.h>
#include <cinolib/meshes/meshes.h>
#include <cinolib/color.h>
#include <cinolib/drawable_octree.h>
#include <cinolib/meshes/hexmesh.h>


namespace cinolib
{
template<class M, class V, class E, class F, class P>
CINO_INLINE
void split27(const uint pid, DrawableHexmesh<M,V,E,F,P> & mesh){
    std::vector<vec3d> verts = mesh.poly_verts(pid);
    std::vector<vec3d> newverts(64);

    vec3d min = *std::min_element(verts.begin(), verts.end());
    vec3d max = *std::max_element(verts.begin(), verts.end());

    vec3d avg1 = min + ((max-min)/3);
    vec3d avg2 = avg1 + ((max-min)/3);

    //do improvements for multiple insertion (avoid vertices duplicate)

    //z = min
    newverts[0] = vec3d(min[0], min[1], min[2]);
    newverts[1] = vec3d(avg1[0], min[1], min[2]);
    newverts[2] = vec3d(avg2[0], min[1], min[2]);
    newverts[3] = vec3d(max[0], min[1], min[2]);

    newverts[4] = vec3d(min[0], avg1[1], min[2]);
    newverts[5] = vec3d(avg1[0], avg1[1], min[2]);
    newverts[6] = vec3d(avg2[0], avg1[1], min[2]);
    newverts[7] = vec3d(max[0], avg1[1], min[2]);

    newverts[8] = vec3d(min[0], avg2[1], min[2]);
    newverts[9] = vec3d(avg1[0], avg2[1], min[2]);
    newverts[10] = vec3d(avg2[0], avg2[1], min[2]);
    newverts[11] = vec3d(max[0], avg2[1], min[2]);

    newverts[12] = vec3d(min[0], max[1], min[2]);
    newverts[13] = vec3d(avg1[0], max[1], min[2]);
    newverts[14] = vec3d(avg2[0], max[1], min[2]);
    newverts[15] = vec3d(max[0], max[1], min[2]);

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
    newverts[48] = vec3d(min[0], min[1], max[2]);
    newverts[49] = vec3d(avg1[0], min[1], max[2]);
    newverts[50] = vec3d(avg2[0], min[1], max[2]);
    newverts[51] = vec3d(max[0], min[1], max[2]);

    newverts[52] = vec3d(min[0], avg1[1], max[2]);
    newverts[53] = vec3d(avg1[0], avg1[1], max[2]);
    newverts[54] = vec3d(avg2[0], avg1[1], max[2]);
    newverts[55] = vec3d(max[0], avg1[1], max[2]);

    newverts[56] = vec3d(min[0], avg2[1], max[2]);
    newverts[57] = vec3d(avg1[0], avg2[1], max[2]);
    newverts[58] = vec3d(avg2[0], avg2[1], max[2]);
    newverts[59] = vec3d(max[0], avg2[1], max[2]);

    newverts[60] = vec3d(min[0], max[1], max[2]);
    newverts[61] = vec3d(avg1[0], max[1], max[2]);
    newverts[62] = vec3d(avg2[0], max[1], max[2]);
    newverts[63] = vec3d(max[0], max[1], max[2]);

    //insert polys
    std::vector<std::vector<uint>> polys(27);
    polys[0] = {0,1,17,16,4,5,21,20};
    polys[1] = {1,2,18,17,5,6,22,21};
    polys[2] = {2,3,19,18,6,7,23,22};

    polys[3] = {4,5,21,20,8,9,25,24};
    polys[4] = {5,6,22,21,9,10,26,25};
    polys[5] = {6,7,23,22,10,11,27,26};

    polys[6] = {8,9,25,24,12,13,29,28};
    polys[7] = {9,10,26,25,13,14,30,29};
    polys[8] = {10,11,27,26,14,15,31,30};


    polys[9] = {16,17,33,32,20,21,37,36};
    polys[10] = {17,18,34,33,21,22,38,37};
    polys[11] = {18,19,35,34,22,23,39,38};

    polys[12] = {20,21,37,36,24,25,41,40};
    polys[13] = {21,22,38,37,25,26,42,41};
    polys[14] = {22,23,39,38,26,27,43,42};

    polys[15] = {24,25,41,40,28,29,45,44};
    polys[16] = {25,26,42,41,29,30,46,45};
    polys[17] = {26,27,43,42,30,31,47,46};


    polys[18] = {32,33,49,48,36,37,53,52};
    polys[19] = {33,34,50,49,37,38,54,53};
    polys[20] = {34,35,51,50,38,39,55,54};

    polys[21] = {36,37,53,52,40,41,57,56};
    polys[22] = {37,38,54,53,41,42,58,57};
    polys[23] = {38,39,55,54,42,43,59,58};

    polys[24] = {40,41,57,56,44,45,61,60};
    polys[25] = {41,42,58,57,45,46,62,61};
    polys[26] = {42,43,59,58,46,47,63,62};

    mesh.poly_remove(pid);

    for(auto v : newverts) mesh.vert_add(v);
    for(auto p : polys) mesh.poly_add(p);

    mesh.updateGL();
}
}

int main(int argc, char *argv[])
{
    using namespace cinolib;
    QApplication a(argc, argv);

    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/cube.mesh";
    DrawableHexmesh<> mesh(s.c_str());

    split27(0, mesh);

    //split27(8, mesh);

    GLcanvas gui;
    gui.push_obj(&mesh);
    gui.show();



    VolumeMeshControlPanel<DrawableHexmesh<>> panel(&mesh, &gui);
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});

    return a.exec();
}
