#ifndef __TEXTURE__
#define __TEXTURE__

//     Copyright (C) Kevin Suffern 2000-2007.
//    This C++ code is for non-commercial purposes only.
//    This C++ code is licensed under the GNU General Public License Version 2.
//    See the file COPYING.txt for the full license.


#include <math.h>

#include "ShadeRec.h"
#include "RGBColor.h"
// 纹理的具体应用操作非常简单。所需工作只是返回碰撞点处的RGB值。
// 考虑到纹理将执行颜色值的返回操作，因而主要用于调整材质颜色值cd、cs、cr以及Ce，
// 还可用于调整反射系数ka、ka、ks以及kr或kt。
// 本章将主要讨论纹理与漫反射颜色值ca之间的调整操作。
class Texture {    
    public:
    
        Texture(void);                                        
        
        Texture(const Texture& texture);                     
                
        virtual Texture*                                    
        clone(void) const = 0;    

        virtual                                             
        ~Texture(void);
        // 返回碰撞点处的RGB值。函数将会用到碰撞点处的纹理坐标值(u， v)，
        // 且该坐标值存储于Shade Rec对象中， 因而函数将包含一个Shade Rec引用参数。
        // 此处将使用纹理及其纹理坐标渲染三角形网格，这也是纹理的主要用途之一。
        virtual RGBColor                                                                            
        get_color(const ShadeRec& sr) const = 0;
                
    protected:
    
        Texture&                                             
        operator= (const Texture& rhs);
};

#endif
        