// Chapter23.5.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "World.h"

#include <fstream>
#include <iostream>
using namespace std;
ofstream out;

int
main(void)
{
	out.open("fileppm.ppm", ios::out);
	out << "P3\n"
		<< 400 << " " << 400 << "\n255\n";
	World w;
	w.build();
	// w.render_scene();
	w.camera_ptr->render_scene(w);

	out.close();
	return(0);
}
