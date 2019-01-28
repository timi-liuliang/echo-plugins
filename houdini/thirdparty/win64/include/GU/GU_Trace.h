
#ifndef __GU_Trace_h__
#define __GU_Trace_h__

#include "GU_API.h"
class IMG_Raster;

class GU_API GU_TraceParms
{
public:

    IMG_Raster *raster; // image to be traced
    int thresh;         // image cut-off value

    bool doResample;    // resample points
    float stepsize;     // used in resampling

    bool doSmooth;      // smooth object
    float cornerDelta;  // used in finding corners

    bool removeBorder;  // del image boundaries edges
    int  borderWidth;   // how far border extends

    /// Use pixel (0,0) as a boundary value
    /// NOTE: The true default is just for HDK backward compatibility.
    ///       You probably want to set this to false and set boundaryValue to 0 or 255.
    bool firstPixelValueBoundary = true;
    int boundaryValue;  // If !firstPixelValueBoundary, use this as a boundary value.
private:

};

#endif

