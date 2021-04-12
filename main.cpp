#include "mainwindow.h"
#include <QApplication>

#include <cinolib/gui/qt/qt_gui_tools.h>
#include <cinolib/meshes/meshes.h>
#include <cinolib/color.h>
#include <cinolib/drawable_octree.h>



int main(int argc, char *argv[])
{
    using namespace cinolib;
    QApplication a(argc, argv);

    /*
     * insert mesh into octree
     *
    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/airplane1_input_tri.obj";
    DrawableTrimesh<> mesh(s.c_str());

    DrawableOctree octree(7, 100);
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


    DrawableOctree octree(7, 100);
    octree.push_point(1, v1);
    octree.push_point(2, v2);
    octree.push_point(3, v3);
    octree.push_point(4, v4);
    octree.push_point(5, v1b);
    octree.push_point(6, v2b);
    octree.push_point(7, v3b);
    octree.push_point(8, v4b);

    octree.build();

    OctreeNode *node = octree.root;


    //split into 27 cubes
    vec3d min = node->bbox.min;
    vec3d max = node->bbox.max;
    vec3d avg = node->bbox.center();

    //maybe vec3d avg1 = (max - min)/3
    // and vec3d avg2 = avg1*2

    node->children[0] = new OctreeNode(node, AABB(vec3d(min[0], min[1], min[2]), vec3d(avg[0], avg[1], avg[2])));
    node->children[1] = new OctreeNode(node, AABB(vec3d(avg[0], min[1], min[2]), vec3d(max[0], avg[1], avg[2])));
    node->children[2] = new OctreeNode(node, AABB(vec3d(avg[0], avg[1], min[2]), vec3d(max[0], max[1], avg[2])));
    node->children[3] = new OctreeNode(node, AABB(vec3d(min[0], avg[1], min[2]), vec3d(avg[0], max[1], avg[2])));
    node->children[4] = new OctreeNode(node, AABB(vec3d(min[0], min[1], avg[2]), vec3d(avg[0], avg[1], max[2])));
    node->children[5] = new OctreeNode(node, AABB(vec3d(avg[0], min[1], avg[2]), vec3d(max[0], avg[1], max[2])));
    node->children[6] = new OctreeNode(node, AABB(vec3d(avg[0], avg[1], avg[2]), vec3d(max[0], max[1], max[2])));
    node->children[7] = new OctreeNode(node, AABB(vec3d(min[0], avg[1], avg[2]), vec3d(avg[0], max[1], max[2])));
    /*
    node->children[8] = new OctreeNode(node, AABB(vec3d(min[0], min[1], min[2]), vec3d(avg[0], avg[1], avg[2])));
    node->children[9] = new OctreeNode(node, AABB(vec3d(avg[0], min[1], min[2]), vec3d(max[0], avg[1], avg[2])));
    node->children[10] = new OctreeNode(node, AABB(vec3d(avg[0], avg[1], min[2]), vec3d(max[0], max[1], avg[2])));
    node->children[11] = new OctreeNode(node, AABB(vec3d(min[0], avg[1], min[2]), vec3d(avg[0], max[1], avg[2])));
    node->children[12] = new OctreeNode(node, AABB(vec3d(min[0], min[1], avg[2]), vec3d(avg[0], avg[1], max[2])));
    node->children[13] = new OctreeNode(node, AABB(vec3d(avg[0], min[1], avg[2]), vec3d(max[0], avg[1], max[2])));
    node->children[14] = new OctreeNode(node, AABB(vec3d(avg[0], avg[1], avg[2]), vec3d(max[0], max[1], max[2])));
    node->children[15] = new OctreeNode(node, AABB(vec3d(min[0], avg[1], avg[2]), vec3d(avg[0], max[1], max[2])));
    node->children[16] = new OctreeNode(node, AABB(vec3d(min[0], min[1], min[2]), vec3d(avg[0], avg[1], avg[2])));
    node->children[17] = new OctreeNode(node, AABB(vec3d(avg[0], min[1], min[2]), vec3d(max[0], avg[1], avg[2])));
    node->children[18] = new OctreeNode(node, AABB(vec3d(avg[0], avg[1], min[2]), vec3d(max[0], max[1], avg[2])));
    node->children[19] = new OctreeNode(node, AABB(vec3d(min[0], avg[1], min[2]), vec3d(avg[0], max[1], avg[2])));
    node->children[20] = new OctreeNode(node, AABB(vec3d(min[0], min[1], avg[2]), vec3d(avg[0], avg[1], max[2])));
    node->children[21] = new OctreeNode(node, AABB(vec3d(avg[0], min[1], avg[2]), vec3d(max[0], avg[1], max[2])));
    node->children[22] = new OctreeNode(node, AABB(vec3d(avg[0], avg[1], avg[2]), vec3d(max[0], max[1], max[2])));
    node->children[23] = new OctreeNode(node, AABB(vec3d(min[0], avg[1], avg[2]), vec3d(avg[0], max[1], max[2])));
    node->children[24] = new OctreeNode(node, AABB(vec3d(min[0], avg[1], avg[2]), vec3d(avg[0], max[1], max[2])));
    node->children[25] = new OctreeNode(node, AABB(vec3d(min[0], avg[1], avg[2]), vec3d(avg[0], max[1], max[2])));
    node->children[26] = new OctreeNode(node, AABB(vec3d(min[0], avg[1], avg[2]), vec3d(avg[0], max[1], max[2])));
    */

    for(uint it : node->item_indices)
    {
        bool orphan = true;
        for(int i=0; i<8; ++i)
        {
            assert(node->children[i]!=nullptr);
            if(node->children[i]->bbox.intersects_box(octree.items.at(it)->aabb))
            {
                node->children[i]->item_indices.push_back(it);
                orphan = false;
            }
        }
        assert(!orphan);
    }

    node->item_indices.clear();
    node->is_inner = true;

    octree.updateGL();

    GLcanvas canvas;
    canvas.push_obj(&octree); //OCTREE

    canvas.show();
    // CMD+1 to show mesh controls.
    //SurfaceMeshControlPanel<DrawableTrimesh<>> panel(&mesh, &gui);
    //QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});


    return a.exec();
}
