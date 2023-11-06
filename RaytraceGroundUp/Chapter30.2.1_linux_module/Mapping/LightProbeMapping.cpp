// =============================================================================
//
//  Created by Shyam Prathish Sargunam on 03/06/16.
//  Copyright © 2015 Shyam Prathish Sargunam. All rights reserved.
//
// =============================================================================

#include "LightProbeMapping.h"

//Default Constructor
LightProbeMapping::LightProbeMapping(void)
{
    map_type = light_probe;
}

//Destructor
LightProbeMapping::~LightProbeMapping(void)
{

}

void
LightProbeMapping::get_texel_coordinates(    const     Point3D&     hit_point, 
                                        const     int         xres, 
                                        const     int         yres, 
                                                int&         row, 
                                                int&         column) const {

    float x = hit_point.x; float y = hit_point.y; float z = hit_point.z;
    
    // 参见公式29.2
    float d = sqrt(x*x + y*y);
    float sin_beta = y/d;
    float cos_beta = x/d;
    float alpha;
    
    // 参见公式29.2
    // first, compute theta and phi
    if(map_type == light_probe)    //the default
    {
        alpha = acos(z);
    }
    if(map_type == panoramic)
    {
        alpha=acos(-z);
    }
    float r = alpha * invPI;
    
    // 参见公式29.3
    // next, map theta and phi to (u, v) in [0, 1] X [0, 1]
    float u = (1.0 + r * cos_beta) * 0.5;
    float v = (1.0 + r * sin_beta) * 0.5;
    
    // finally, map u and v to the texel coordinates
    column = (int)((xres - 1) * u);       // column is across
    row    = (int)((yres - 1) * v);        // row is up
}

void LightProbeMapping::set_map_type(MapType mapType)
{
    map_type = map_type;
}

RGBColor LightProbeMapping::MapToImage(const ShadeRec& sr, Image* imagePtr) const
{
    Vector3D localHitPoint = sr.local_hit_point;

    // 参见公式29.2
    float alpha;
    if(map_type == light_probe)    //the default
    {
        alpha = acos(localHitPoint.z);
    }
    if(map_type == panoramic)
    {
        alpha=acos(-localHitPoint.z);
    }
    float invDenominaotr = 1 / sqrt(localHitPoint.x * localHitPoint.x
                                  + localHitPoint.y * localHitPoint.y);
    float sinBeta = localHitPoint.y * invDenominaotr;
    float cosBeta = localHitPoint.x * invDenominaotr;

    // 参见公式29.3
    float u = (1 + (alpha * invPI)*cosBeta) * 0.5;
    float v = (1 + (alpha * invPI)*sinBeta) * 0.5;

    // finally, map u and v to the texel coordinates
    int i = (int)((imagePtr->hres - 1) * u);
    int j = (int)((imagePtr->vres - 1) * v);
    // 算出贴图坐标在图片信息中的下标。
    int tempIndex = j*(imagePtr->hres) + i;
    // 返回图片信息中对应下标的像素。
    if (tempIndex >= 0 && tempIndex <= imagePtr->hres * imagePtr->vres)
        return imagePtr->pixels[tempIndex];
    else
        return RGBColor(1.0,0.0,0.0);
}

