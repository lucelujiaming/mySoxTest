#ifndef LIGHTPROBEMAPPING
#define LIGHTPROBEMAPPING

// =============================================================================
//
//  Created by Shyam Prathish Sargunam on 03/06/16.
//  Copyright © 2015 Shyam Prathish Sargunam. All rights reserved.
//
// =============================================================================

#include "math.h"
#include "Image.h"
#include "Constants.h"
#include "Mapping.h"
#include "Texture.h"

// 光探针映射。
// 将反射球体置入某一环境中并对其拍摄多幅照片。
// 除了位于球体后方的部分区域之外，全部场景均反射可见。
enum MapType {
    light_probe = 0,
    panoramic
};

class LightProbeMapping:public Mapping
{
    public:
        
        //Constructors
        LightProbeMapping(void);                                        //Default Constructor

        //Destructor
        ~LightProbeMapping(void);

        //Member Functions
        virtual void
        get_texel_coordinates(    const     Point3D&     local_hit_point, 
                                const     int         xres, 
                                const     int         yres, 
                                        int&         row, 
                                        int&         column) const;

        //Pure virtual function to get the color at particular hit point
        RGBColor MapToImage(const ShadeRec& sr, Image* imagePtr) const;
        
        void set_map_type(MapType mapType);
    private:
        MapType map_type;
};

#endif
