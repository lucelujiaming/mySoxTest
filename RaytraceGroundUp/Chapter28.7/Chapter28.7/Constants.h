#ifndef __CONSTANTS__
#define __CONSTANTS__

#include <stdlib.h>
#include "RGBColor.h"

const double     PI             = 3.1415926535897932384;
const double     TWO_PI         = 6.2831853071795864769;
const double     PI_ON_180     = 0.0174532925199432957;
const double     invPI         = 0.3183098861837906715;
const double     invTWO_PI     = 0.1591549430918953358;

const double     kEpsilon     = 0.0001; 
const double    kHugeValue    = 1.0E10;

const RGBColor    black(0.0);
const RGBColor    white(1.0);
const RGBColor    red(1.0, 0.0, 0.0);
const RGBColor    green(0.0, 1.0, 0.0);
const RGBColor	  blue(0.0, 0.0, 1.0);

const RGBColor	  yellow(1.0, 1.0, 0.0);
const RGBColor	  olive(0.5, 0.5, 0.0);   // 橄榄色
const RGBColor	  purple(0.5, 0.0, 0.5);  // 紫色
const RGBColor	  teal(0.0, 0.5, 0.5);    // Teal为蓝绿色,其实就是青色

const RGBColor	  orange(1.0, 0.64, 0.0);
const RGBColor	  lightblue(0.68, 0.84, 0.9);


const float     invRAND_MAX = 1.0 / (float)RAND_MAX;

#endif
