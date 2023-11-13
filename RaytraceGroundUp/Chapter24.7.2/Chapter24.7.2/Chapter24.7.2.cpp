// Chapter24.7.2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "World.h"

#include <fstream>
#include <iostream>
using namespace std;
ofstream out;

#define HORIZONTAL_RESOLUTION    600
#define VERTICAL_RESOLUTION      600
int
main(char argc, char ** argv)
{
    out.open("fileppm.ppm", ios::out);
    out << "P3\n"
        << HORIZONTAL_RESOLUTION << " " << VERTICAL_RESOLUTION << "\n255\n";
    World w;
    w.vp.set_hres(HORIZONTAL_RESOLUTION);
    w.vp.set_vres(VERTICAL_RESOLUTION);
    if(argc == 2)
    {
        cout << "argv[1] == " << argv[1] << endl;
    //    return (0);
        w.build(argv[1]);
    }
    else
    {
        w.build("Bunny69K.ply");
    }
    // w.render_scene();
    w.camera_ptr->render_scene(w);

    out.close();
    return(0);
}

