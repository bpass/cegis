/* 
 * Programmer: David Mattli 
 */

#ifndef RESAMPLE_HH
#define RESAMPLE_HH

#include <math.h>

#include "image.hh"

/* Class Name: resample
 *
 * Description:
 *   Takes a point and an image and returns the value at the point using
 *   interpolation.
 */

/* V is pixel value type and C is coordinate value */
template <typename V, typename C = double>
class resample
{
public:
    resample();
    virtual ~resample();
    
    virtual V getpointval(C inx, C iny, image::image<V> &img);
    
};

template <typename V, typename C>
resample<V, C>::resample()
{
    return;
}

template <typename V, typename C>
resample<V, C>::~resample()
{
    
    return;
}

template <typename V, typename C>
V resample<V, C>::nearest_neighbor(C inx, C iny, image::image<V> &img)
{
    /* Perform Nearest Neighbor resampling */
    C newx = static_cast<C>(round(inx));
    C newy = static_cast<C>(round(iny));
    
    return img.getPoint(newx, newy);
}

template <typename V, typename C>
V resample<V, C>::bilinear(C inx, C iny, image::image<V> &img)
{
    /* lower-left, upper-left, lower-right, upper-right coordinate values */
    V ll_v, ul_v, lr_v, ur_v;
    V r1, r2, p;   /* Temp values */
    
    
    C fl_x = floor(inx);
    C cl_x = ceil(inx);
    C fl_y = floor(iny);
    C cl_y = ceil(iny);
    
    /* Find values at surrounding corners */
    ll_v = img.getPoint(fl_x, fl_y);
    ul_v = img.getPoint(fl_x, cl_y);
    lr_v = img.getPoint(cl_x, fl_y);
    ur_v = img.getPoint(cl_x, cl_y);

    /* Interpolate x-values */
    r1 = ((cl_x - inx)/(cl_x - fl_x) * ll_v) + ((inx - fl_x)/(cl_x - fl_x) * lr_v);
    r2 = ((cl_x - inx)/(cl_x - fl_x) * ul_v) + ((inx - fl_x)/(cl_x - fl_x) * ur_v);

    /* Interpolate y-value */
    p = ((cl_y - iny)/(cl_y - fl_y) * r1) + ((iny - fl_y)/(cl_y - fl_y) * r2);

    return p;
}

#endif  
  
  
