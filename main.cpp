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

namespace cinolib
{
template<class M, class V, class E, class F, class P>
CINO_INLINE
// split the polygon of id "pid" of the input mesh into 27 cubes
void split27(const uint pid, DrawableHexmesh<M,V,E,F,P> & mesh, std::map<vec3d, uint> & vertices){

    //vector for the new polys
    //std::vector<std::vector<uint>> polys(27);

    //points for subdivision
    std::vector<vec3d> verts = mesh.poly_verts(pid);
    vec3d min = *std::min_element(verts.begin(), verts.end());
    vec3d max = *std::max_element(verts.begin(), verts.end());
    vec3d avg1 = min + ((max-min)/3);
    vec3d avg2 = avg1 + ((max-min)/3);


    //z = min
    vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], min[1], min[2]), uint(vertices.size()))); //0
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], min[1], min[2]), uint(vertices.size()))); //1
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], min[1], min[2]), uint(vertices.size()))); //2
    vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], min[1], min[2]), uint(vertices.size()))); //3

    vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], avg1[1], min[2]), uint(vertices.size()))); //4
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], avg1[1], min[2]), uint(vertices.size()))); //5
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], avg1[1], min[2]), uint(vertices.size()))); //6
    vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], avg1[1], min[2]), uint(vertices.size()))); //7

    vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], avg2[1], min[2]), uint(vertices.size()))); //8
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], avg2[1], min[2]), uint(vertices.size()))); //9
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], avg2[1], min[2]), uint(vertices.size()))); //10
    vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], avg2[1], min[2]), uint(vertices.size()))); //11

    vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], max[1], min[2]), uint(vertices.size()))); //12
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], max[1], min[2]), uint(vertices.size()))); //13
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], max[1], min[2]), uint(vertices.size()))); //14
    vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], max[1], min[2]), uint(vertices.size()))); //15

    //z = avg1
    vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], min[1], avg1[2]), uint(vertices.size()))); //16
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], min[1], avg1[2]), uint(vertices.size()))); //17
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], min[1], avg1[2]), uint(vertices.size()))); //18
    vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], min[1], avg1[2]), uint(vertices.size()))); //19

    vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], avg1[1], avg1[2]), uint(vertices.size()))); //20
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], avg1[1], avg1[2]), uint(vertices.size()))); //21
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], avg1[1], avg1[2]), uint(vertices.size()))); //22
    vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], avg1[1], avg1[2]), uint(vertices.size()))); //23

    vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], avg2[1], avg1[2]), uint(vertices.size()))); //24
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], avg2[1], avg1[2]), uint(vertices.size()))); //25
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], avg2[1], avg1[2]), uint(vertices.size()))); //26
    vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], avg2[1], avg1[2]), uint(vertices.size()))); //27

    vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], max[1], avg1[2]), uint(vertices.size()))); //28
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], max[1], avg1[2]), uint(vertices.size()))); //29
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], max[1], avg1[2]), uint(vertices.size()))); //30
    vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], max[1], avg1[2]), uint(vertices.size()))); //31

    //z = avg2
    vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], min[1], avg2[2]), uint(vertices.size()))); //32
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], min[1], avg2[2]), uint(vertices.size()))); //33
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], min[1], avg2[2]), uint(vertices.size()))); //34
    vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], min[1], avg2[2]), uint(vertices.size()))); //35

    vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], avg1[1], avg2[2]), uint(vertices.size()))); //36
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], avg1[1], avg2[2]), uint(vertices.size()))); //37
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], avg1[1], avg2[2]), uint(vertices.size()))); //38
    vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], avg1[1], avg2[2]), uint(vertices.size()))); //39

    vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], avg2[1], avg2[2]), uint(vertices.size()))); //40
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], avg2[1], avg2[2]), uint(vertices.size()))); //41
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], avg2[1], avg2[2]), uint(vertices.size()))); //42
    vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], avg2[1], avg2[2]), uint(vertices.size()))); //43

    vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], max[1], avg2[2]), uint(vertices.size()))); //44
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], max[1], avg2[2]), uint(vertices.size()))); //45
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], max[1], avg2[2]), uint(vertices.size()))); //46
    vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], max[1], avg2[2]), uint(vertices.size()))); //47

    //z = max
    vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], min[1], max[2]), uint(vertices.size()))); //48
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], min[1], max[2]), uint(vertices.size()))); //49
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], min[1], max[2]), uint(vertices.size()))); //50
    vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], min[1], max[2]), uint(vertices.size()))); //51

    vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], avg1[1], max[2]), uint(vertices.size()))); //52
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], avg1[1], max[2]), uint(vertices.size()))); //53
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], avg1[1], max[2]), uint(vertices.size()))); //54
    vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], avg1[1], max[2]), uint(vertices.size()))); //55

    vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], avg2[1], max[2]), uint(vertices.size()))); //56
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], avg2[1], max[2]), uint(vertices.size()))); //57
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], avg2[1], max[2]), uint(vertices.size()))); //58
    vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], avg2[1], max[2]), uint(vertices.size()))); //59

    vertices.insert(std::pair<vec3d, uint>(vec3d(min[0], max[1], max[2]), uint(vertices.size()))); //60
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg1[0], max[1], max[2]), uint(vertices.size()))); //61
    vertices.insert(std::pair<vec3d, uint>(vec3d(avg2[0], max[1], max[2]), uint(vertices.size()))); //62
    vertices.insert(std::pair<vec3d, uint>(vec3d(max[0], max[1], max[2]), uint(vertices.size()))); //63

    if (mesh.num_polys() == 1){ 
        std::vector<std::vector<uint>> polys(27);

        //insert polys
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

        for(auto v : vertices) mesh.vert_add(v.first);
        for(auto p : polys) mesh.poly_add(p);
    }
    else {
        std::vector<std::vector<uint>> polys(1);

        //insert polys
        polys[0] = {vertices.find(vec3d(min[0], min[1], min[2]))->second,
                    vertices.find(vec3d(avg1[0], min[1], min[2]))->second,
                    vertices.find(vec3d(avg1[0], min[1], avg1[2]))->second,
                    vertices.find(vec3d(min[0], min[1], avg1[2]))->second,
                    vertices.find(vec3d(min[0], avg1[1], min[2]))->second,
                    vertices.find(vec3d(avg1[0], avg1[1], min[2]))->second,
                    vertices.find(vec3d(avg1[0], avg1[1], avg1[2]))->second,
                    vertices.find(vec3d(min[0], avg1[1], avg1[2]))->second};
/*
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
        */

        mesh.poly_remove(pid);

        //avoid duplicate vertices in the mesh
        int start = mesh.num_verts();
        int conta = 0;
        std::map<vec3d, uint>::iterator it;
        for(it=vertices.begin(); it!=vertices.end(); ++it){
            if(conta >= start) mesh.vert_add(it->first);
            conta++;
        }


        for(auto p : polys) mesh.poly_add(p);
    }

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

    gui.push_marker(vec2i(10, gui.height()-20), "Ctrl + click to split a poly into 27 elements", Color::BLACK(), 12, 0);

    Profiler profiler;

    gui.callback_mouse_press = [&](GLcanvas *c, QMouseEvent *e)
    {
        if (e->modifiers() == Qt::ControlModifier)
        {
            vec3d p;
            vec2i click(e->x(), e->y());

            if (c->unproject(click, p)) // transform click in a 3d point
            {
                profiler.push("Pick Polys");

                uint pid = mesh.pick_poly(p);
                profiler.pop();

                split27(pid, mesh, vertices);

                mesh.updateGL();
                c->updateGL();
            }
        }
    };


    VolumeMeshControlPanel<DrawableHexmesh<>> panel(&mesh, &gui);
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});

    return a.exec();
}
