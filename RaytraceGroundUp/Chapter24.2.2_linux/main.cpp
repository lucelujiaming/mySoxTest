#include "stdafx.h"

#include "World.h"

#include <fstream>
#include <iostream>
using namespace std;
ofstream out;

#define HORIZONTAL_RESOLUTION    600
#define VERTICAL_RESOLUTION      600
int
main(void)
{
    out.open("fileppm.ppm", ios::out);
    out << "P3\n"
        << HORIZONTAL_RESOLUTION << " " << VERTICAL_RESOLUTION << "\n255\n";
    World w;
    w.vp.set_hres(HORIZONTAL_RESOLUTION);
    w.vp.set_vres(VERTICAL_RESOLUTION);
    w.build();
    // w.render_scene();
    w.camera_ptr->render_scene(w);

    out.close();
    return(0);
}

