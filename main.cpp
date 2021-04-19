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
// split the polygon of id "pid" of the input mesh into 27 cubes
void split27(const uint pid, DrawableHexmesh<M,V,E,F,P> & mesh){
    std::vector<vec3d> verts = mesh.poly_verts(pid);
    std::vector<std::vector<uint>> polys(27);

    //points for subdivision
    vec3d min = *std::min_element(verts.begin(), verts.end());
    vec3d max = *std::max_element(verts.begin(), verts.end());
    vec3d avg1 = min + ((max-min)/3);
    vec3d avg2 = avg1 + ((max-min)/3);


    if (mesh.num_polys() == 1){
        std::vector<vec3d> newverts(64);

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

        for(auto v : newverts) mesh.vert_add(v);
        for(auto p : polys) mesh.poly_add(p);
    }
    else {
        std::vector<std::vector<uint>> v2f_number(8);

        std::vector<uint> vertsId = mesh.poly_verts_id(pid);

        int id_angle = 0;
        bool angle = false;
        for (int c=0; c<8; c++) {
            v2f_number[c] = mesh.adj_v2p(vertsId[c]);
            if (v2f_number[c].size() < 2){
                id_angle = c;
                angle = true;

            }
        }

        mesh.poly_remove(pid);
        uint start = mesh.num_verts();

        if (angle){
            std::vector<vec3d> newverts(57); // 57 SOLO SE SIAMO IN UN ANGOLO

            //z = min
            newverts[0] = vec3d(avg1[0], min[1], min[2]);
            newverts[1] = vec3d(avg2[0], min[1], min[2]);

            newverts[2] = vec3d(min[0], avg1[1], min[2]);
            newverts[3] = vec3d(avg1[0], avg1[1], min[2]);
            newverts[4] = vec3d(avg2[0], avg1[1], min[2]);
            newverts[5] = vec3d(max[0], avg1[1], min[2]);

            newverts[6] = vec3d(min[0], avg2[1], min[2]);
            newverts[7] = vec3d(avg1[0], avg2[1], min[2]);
            newverts[8] = vec3d(avg2[0], avg2[1], min[2]);
            newverts[9] = vec3d(max[0], avg2[1], min[2]);

            newverts[10] = vec3d(avg1[0], max[1], min[2]);
            newverts[11] = vec3d(avg2[0], max[1], min[2]);

            //z = avg1
            newverts[12] = vec3d(min[0], min[1], avg1[2]);
            newverts[13] = vec3d(avg1[0], min[1], avg1[2]);
            newverts[14] = vec3d(avg2[0], min[1], avg1[2]);
            newverts[15] = vec3d(max[0], min[1], avg1[2]);

            newverts[16] = vec3d(min[0], avg1[1], avg1[2]);
            newverts[17] = vec3d(avg1[0], avg1[1], avg1[2]);
            newverts[18] = vec3d(avg2[0], avg1[1], avg1[2]);
            newverts[19] = vec3d(max[0], avg1[1], avg1[2]);

            newverts[20] = vec3d(min[0], avg2[1], avg1[2]);
            newverts[21] = vec3d(avg1[0], avg2[1], avg1[2]);
            newverts[22] = vec3d(avg2[0], avg2[1], avg1[2]);
            newverts[23] = vec3d(max[0], avg2[1], avg1[2]);

            newverts[24] = vec3d(min[0], max[1], avg1[2]);
            newverts[25] = vec3d(avg1[0], max[1], avg1[2]);
            newverts[26] = vec3d(avg2[0], max[1], avg1[2]);
            newverts[27] = vec3d(max[0], max[1], avg1[2]);

            //z = avg2
            newverts[28] = vec3d(min[0], min[1], avg2[2]);
            newverts[29] = vec3d(avg1[0], min[1], avg2[2]);
            newverts[30] = vec3d(avg2[0], min[1], avg2[2]);
            newverts[31] = vec3d(max[0], min[1], avg2[2]);

            newverts[32] = vec3d(min[0], avg1[1], avg2[2]);
            newverts[33] = vec3d(avg1[0], avg1[1], avg2[2]);
            newverts[34] = vec3d(avg2[0], avg1[1], avg2[2]);
            newverts[35] = vec3d(max[0], avg1[1], avg2[2]);

            newverts[36] = vec3d(min[0], avg2[1], avg2[2]);
            newverts[37] = vec3d(avg1[0], avg2[1], avg2[2]);
            newverts[38] = vec3d(avg2[0], avg2[1], avg2[2]);
            newverts[39] = vec3d(max[0], avg2[1], avg2[2]);

            newverts[40] = vec3d(min[0], max[1], avg2[2]);
            newverts[41] = vec3d(avg1[0], max[1], avg2[2]);
            newverts[42] = vec3d(avg2[0], max[1], avg2[2]);
            newverts[43] = vec3d(max[0], max[1], avg2[2]);

            //z = max
            newverts[44] = vec3d(avg1[0], min[1], max[2]);
            newverts[45] = vec3d(avg2[0], min[1], max[2]);

            newverts[46] = vec3d(min[0], avg1[1], max[2]);
            newverts[47] = vec3d(avg1[0], avg1[1], max[2]);
            newverts[48] = vec3d(avg2[0], avg1[1], max[2]);
            newverts[49] = vec3d(max[0], avg1[1], max[2]);

            newverts[50] = vec3d(min[0], avg2[1], max[2]);
            newverts[51] = vec3d(avg1[0], avg2[1], max[2]);
            newverts[52] = vec3d(avg2[0], avg2[1], max[2]);
            newverts[53] = vec3d(max[0], avg2[1], max[2]);

            newverts[54] = vec3d(avg1[0], max[1], max[2]);
            newverts[55] = vec3d(avg2[0], max[1], max[2]);

            newverts[56] = vec3d(verts[id_angle].x(), verts[id_angle].y(), verts[id_angle].z());

            for(auto v : newverts) mesh.vert_add(v);
        }
        else{
            std::vector<vec3d> newverts(56);
            //z = min
            newverts[0] = vec3d(avg1[0], min[1], min[2]);
            newverts[1] = vec3d(avg2[0], min[1], min[2]);

            newverts[2] = vec3d(min[0], avg1[1], min[2]);
            newverts[3] = vec3d(avg1[0], avg1[1], min[2]);
            newverts[4] = vec3d(avg2[0], avg1[1], min[2]);
            newverts[5] = vec3d(max[0], avg1[1], min[2]);

            newverts[6] = vec3d(min[0], avg2[1], min[2]);
            newverts[7] = vec3d(avg1[0], avg2[1], min[2]);
            newverts[8] = vec3d(avg2[0], avg2[1], min[2]);
            newverts[9] = vec3d(max[0], avg2[1], min[2]);

            newverts[10] = vec3d(avg1[0], max[1], min[2]);
            newverts[11] = vec3d(avg2[0], max[1], min[2]);

            //z = avg1
            newverts[12] = vec3d(min[0], min[1], avg1[2]);
            newverts[13] = vec3d(avg1[0], min[1], avg1[2]);
            newverts[14] = vec3d(avg2[0], min[1], avg1[2]);
            newverts[15] = vec3d(max[0], min[1], avg1[2]);

            newverts[16] = vec3d(min[0], avg1[1], avg1[2]);
            newverts[17] = vec3d(avg1[0], avg1[1], avg1[2]);
            newverts[18] = vec3d(avg2[0], avg1[1], avg1[2]);
            newverts[19] = vec3d(max[0], avg1[1], avg1[2]);

            newverts[20] = vec3d(min[0], avg2[1], avg1[2]);
            newverts[21] = vec3d(avg1[0], avg2[1], avg1[2]);
            newverts[22] = vec3d(avg2[0], avg2[1], avg1[2]);
            newverts[23] = vec3d(max[0], avg2[1], avg1[2]);

            newverts[24] = vec3d(min[0], max[1], avg1[2]);
            newverts[25] = vec3d(avg1[0], max[1], avg1[2]);
            newverts[26] = vec3d(avg2[0], max[1], avg1[2]);
            newverts[27] = vec3d(max[0], max[1], avg1[2]);

            //z = avg2
            newverts[28] = vec3d(min[0], min[1], avg2[2]);
            newverts[29] = vec3d(avg1[0], min[1], avg2[2]);
            newverts[30] = vec3d(avg2[0], min[1], avg2[2]);
            newverts[31] = vec3d(max[0], min[1], avg2[2]);

            newverts[32] = vec3d(min[0], avg1[1], avg2[2]);
            newverts[33] = vec3d(avg1[0], avg1[1], avg2[2]);
            newverts[34] = vec3d(avg2[0], avg1[1], avg2[2]);
            newverts[35] = vec3d(max[0], avg1[1], avg2[2]);

            newverts[36] = vec3d(min[0], avg2[1], avg2[2]);
            newverts[37] = vec3d(avg1[0], avg2[1], avg2[2]);
            newverts[38] = vec3d(avg2[0], avg2[1], avg2[2]);
            newverts[39] = vec3d(max[0], avg2[1], avg2[2]);

            newverts[40] = vec3d(min[0], max[1], avg2[2]);
            newverts[41] = vec3d(avg1[0], max[1], avg2[2]);
            newverts[42] = vec3d(avg2[0], max[1], avg2[2]);
            newverts[43] = vec3d(max[0], max[1], avg2[2]);

            //z = max
            newverts[44] = vec3d(avg1[0], min[1], max[2]);
            newverts[45] = vec3d(avg2[0], min[1], max[2]);

            newverts[46] = vec3d(min[0], avg1[1], max[2]);
            newverts[47] = vec3d(avg1[0], avg1[1], max[2]);
            newverts[48] = vec3d(avg2[0], avg1[1], max[2]);
            newverts[49] = vec3d(max[0], avg1[1], max[2]);

            newverts[50] = vec3d(min[0], avg2[1], max[2]);
            newverts[51] = vec3d(avg1[0], avg2[1], max[2]);
            newverts[52] = vec3d(avg2[0], avg2[1], max[2]);
            newverts[53] = vec3d(max[0], avg2[1], max[2]);

            newverts[54] = vec3d(avg1[0], max[1], max[2]);
            newverts[55] = vec3d(avg2[0], max[1], max[2]);
            for(auto v : newverts) mesh.vert_add(v);
        }
        if(angle) {
            switch (id_angle){
                case 0:
                    polys[0] = {start+56,start+0,start+13,start+12,start+2,start+3,start+17,start+16};
                    polys[2] = {start+1,vertsId[1],start+15,start+14,start+4,start+5,start+19,start+18};
                    polys[6] = {start+6,start+7,start+21,start+20,vertsId[4],start+10,start+25,start+24};
                    polys[8] = {start+8,start+9,start+23,start+22,start+11,vertsId[5],start+27,start+26};
                    polys[18] = {start+28,start+29,start+44,vertsId[3],start+32,start+33,start+47,start+46};
                    polys[20] = {start+30,start+31,vertsId[2],start+45,start+34,start+35,start+49,start+48};
                    polys[24] = {start+36,start+37,start+51,start+50,start+40,start+41,start+54,vertsId[7]};
                    polys[26] = {start+38,start+39,start+53,start+52,start+42,start+43,vertsId[6],start+55};
                    break;
                case 1:
                    polys[0] = {vertsId[0],start+0,start+13,start+12,start+2,start+3,start+17,start+16};
                    polys[2] = {start+1,start+56,start+15,start+14,start+4,start+5,start+19,start+18};
                    polys[6] = {start+6,start+7,start+21,start+20,vertsId[4],start+10,start+25,start+24};
                    polys[8] = {start+8,start+9,start+23,start+22,start+11,vertsId[5],start+27,start+26};
                    polys[18] = {start+28,start+29,start+44,vertsId[3],start+32,start+33,start+47,start+46};
                    polys[20] = {start+30,start+31,vertsId[2],start+45,start+34,start+35,start+49,start+48};
                    polys[24] = {start+36,start+37,start+51,start+50,start+40,start+41,start+54,vertsId[7]};
                    polys[26] = {start+38,start+39,start+53,start+52,start+42,start+43,vertsId[6],start+55};
                    break;
                case 2:
                    polys[0] = {vertsId[0],start+0,start+13,start+12,start+2,start+3,start+17,start+16};
                    polys[2] = {start+1,vertsId[1],start+15,start+14,start+4,start+5,start+19,start+18};
                    polys[6] = {start+6,start+7,start+21,start+20,vertsId[4],start+10,start+25,start+24};
                    polys[8] = {start+8,start+9,start+23,start+22,start+11,vertsId[5],start+27,start+26};
                    polys[18] = {start+28,start+29,start+44,vertsId[3],start+32,start+33,start+47,start+46};
                    polys[20] = {start+30,start+31,start+56,start+45,start+34,start+35,start+49,start+48};
                    polys[24] = {start+36,start+37,start+51,start+50,start+40,start+41,start+54,vertsId[7]};
                    polys[26] = {start+38,start+39,start+53,start+52,start+42,start+43,vertsId[6],start+55};
                    break;
                case 3:
                    polys[0] = {vertsId[0],start+0,start+13,start+12,start+2,start+3,start+17,start+16};
                    polys[2] = {start+1,vertsId[1],start+15,start+14,start+4,start+5,start+19,start+18};
                    polys[6] = {start+6,start+7,start+21,start+20,vertsId[4],start+10,start+25,start+24};
                    polys[8] = {start+8,start+9,start+23,start+22,start+11,vertsId[5],start+27,start+26};
                    polys[18] = {start+28,start+29,start+44,start+56,start+32,start+33,start+47,start+46};
                    polys[20] = {start+30,start+31,vertsId[2],start+45,start+34,start+35,start+49,start+48};
                    polys[24] = {start+36,start+37,start+51,start+50,start+40,start+41,start+54,vertsId[7]};
                    polys[26] = {start+38,start+39,start+53,start+52,start+42,start+43,vertsId[6],start+55};
                    break;
                case 4:
                    polys[0] = {vertsId[0],start+0,start+13,start+12,start+2,start+3,start+17,start+16};
                    polys[2] = {start+1,vertsId[1],start+15,start+14,start+4,start+5,start+19,start+18};
                    polys[6] = {start+6,start+7,start+21,start+20,start+56,start+10,start+25,start+24};
                    polys[8] = {start+8,start+9,start+23,start+22,start+11,vertsId[5],start+27,start+26};
                    polys[18] = {start+28,start+29,start+44,vertsId[3],start+32,start+33,start+47,start+46};
                    polys[20] = {start+30,start+31,vertsId[2],start+45,start+34,start+35,start+49,start+48};
                    polys[24] = {start+36,start+37,start+51,start+50,start+40,start+41,start+54,vertsId[7]};
                    polys[26] = {start+38,start+39,start+53,start+52,start+42,start+43,vertsId[6],start+55};
                    break;
                case 5:
                    polys[0] = {vertsId[0],start+0,start+13,start+12,start+2,start+3,start+17,start+16};
                    polys[2] = {start+1,vertsId[1],start+15,start+14,start+4,start+5,start+19,start+18};
                    polys[6] = {start+6,start+7,start+21,start+20,vertsId[4],start+10,start+25,start+24};
                    polys[8] = {start+8,start+9,start+23,start+22,start+11,start+56,start+27,start+26};
                    polys[18] = {start+28,start+29,start+44,vertsId[3],start+32,start+33,start+47,start+46};
                    polys[20] = {start+30,start+31,vertsId[2],start+45,start+34,start+35,start+49,start+48};
                    polys[24] = {start+36,start+37,start+51,start+50,start+40,start+41,start+54,vertsId[7]};
                    polys[26] = {start+38,start+39,start+53,start+52,start+42,start+43,vertsId[6],start+55};
                    break;
                case 6:
                    polys[0] = {vertsId[0],start+0,start+13,start+12,start+2,start+3,start+17,start+16};
                    polys[2] = {start+1,vertsId[1],start+15,start+14,start+4,start+5,start+19,start+18};
                    polys[6] = {start+6,start+7,start+21,start+20,vertsId[4],start+10,start+25,start+24};
                    polys[8] = {start+8,start+9,start+23,start+22,start+11,vertsId[5],start+27,start+26};
                    polys[18] = {start+28,start+29,start+44,vertsId[3],start+32,start+33,start+47,start+46};
                    polys[20] = {start+30,start+31,vertsId[2],start+45,start+34,start+35,start+49,start+48};
                    polys[24] = {start+36,start+37,start+51,start+50,start+40,start+41,start+54,vertsId[7]};
                    polys[26] = {start+38,start+39,start+53,start+52,start+42,start+43,start+56,start+55};
                    break;
                case 7:
                    polys[0] = {vertsId[0],start+0,start+13,start+12,start+2,start+3,start+17,start+16};
                    polys[2] = {start+1,vertsId[1],start+15,start+14,start+4,start+5,start+19,start+18};
                    polys[6] = {start+6,start+7,start+21,start+20,vertsId[4],start+10,start+25,start+24};
                    polys[8] = {start+8,start+9,start+23,start+22,start+11,vertsId[5],start+27,start+26};
                    polys[18] = {start+28,start+29,start+44,vertsId[3],start+32,start+33,start+47,start+46};
                    polys[20] = {start+30,start+31,vertsId[2],start+45,start+34,start+35,start+49,start+48};
                    polys[24] = {start+36,start+37,start+51,start+50,start+40,start+41,start+54,start+56};
                    polys[26] = {start+38,start+39,start+53,start+52,start+42,start+43,vertsId[6],start+55};
                    break;
                default:
                    printf("%s  ", "Error in the vertex-angle selection!!");
                    break;
            }
        }
        else{
            polys[0] = {vertsId[0],start+0,start+13,start+12,start+2,start+3,start+17,start+16};
            polys[2] = {start+1,vertsId[1],start+15,start+14,start+4,start+5,start+19,start+18};
            polys[6] = {start+6,start+7,start+21,start+20,vertsId[4],start+10,start+25,start+24};
            polys[8] = {start+8,start+9,start+23,start+22,start+11,vertsId[5],start+27,start+26};
            polys[18] = {start+28,start+29,start+44,vertsId[3],start+32,start+33,start+47,start+46};
            polys[20] = {start+30,start+31,vertsId[2],start+45,start+34,start+35,start+49,start+48};
            polys[24] = {start+36,start+37,start+51,start+50,start+40,start+41,start+54,vertsId[7]};
            polys[26] = {start+38,start+39,start+53,start+52,start+42,start+43,vertsId[6],start+55};
        }

        //insert polys

        polys[1] = {start+0,start+1,start+14,start+13,start+3,start+4,start+18,start+17};


        polys[3] = {start+2,start+3,start+17,start+16,start+6,start+7,start+21,start+20};
        polys[4] = {start+3,start+4,start+18,start+17,start+7,start+8,start+22,start+21};
        polys[5] = {start+4,start+5,start+19,start+18,start+8,start+9,start+23,start+22};


        polys[7] = {start+7,start+8,start+22,start+21,start+10,start+11,start+26,start+25};



        polys[9] = {start+12,start+13,start+29,start+28,start+16,start+17,start+33,start+32};
        polys[10] = {start+13,start+14,start+30,start+29,start+17,start+18,start+34,start+33};
        polys[11] = {start+14,start+15,start+31,start+30,start+18,start+19,start+35,start+34};

        polys[12] = {start+16,start+17,start+33,start+32,start+20,start+21,start+37,start+36};
        polys[13] = {start+17,start+18,start+34,start+33,start+21,start+22,start+38,start+37};
        polys[14] = {start+18,start+19,start+35,start+34,start+22,start+23,start+39,start+38};

        polys[15] = {start+20,start+21,start+37,start+36,start+24,start+25,start+41,start+40};
        polys[16] = {start+21,start+22,start+38,start+37,start+25,start+26,start+42,start+41};
        polys[17] = {start+22,start+23,start+39,start+38,start+26,start+27,start+43,start+42};



        polys[19] = {start+29,start+30,start+45,start+44,start+33,start+34,start+48,start+47};


        polys[21] = {start+32,start+33,start+47,start+46,start+36,start+37,start+51,start+50};
        polys[22] = {start+33,start+34,start+48,start+47,start+37,start+38,start+52,start+51};
        polys[23] = {start+34,start+35,start+49,start+48,start+38,start+39,start+53,start+52};


        polys[25] = {start+37,start+38,start+52,start+51,start+41,start+42,start+55,start+54};

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

    split27(0, mesh);

    for (int i=0; i<27;i++) split27(i, mesh);





    GLcanvas gui;
    gui.push_obj(&mesh);
    gui.show();



    VolumeMeshControlPanel<DrawableHexmesh<>> panel(&mesh, &gui);
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});

    return a.exec();
}
