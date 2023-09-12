// Chapter5.1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include "World.h"

#include <iostream>
using namespace std;


int
main(void)
{
	cout << "P3\n"
		<< 400 << " " << 400 << "\n255\n";

	World w;
	w.build();
	w.render_scene();
	return(0);
}
