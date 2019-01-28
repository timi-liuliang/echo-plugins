/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VEX_GeoInputs.h ( VEX Library, C++)
 *
 * COMMENTS:	A callback class to bind the geometry inputs
 */

#ifndef __VEX_GeoInputs__
#define __VEX_GeoInputs__

#include "VEX_API.h"

// Blank class for geometry inputs
// Defined at a level that knows about geometry and attached to
// the rundata to provide a callback for opinput: references.
class VEX_API VEX_GeoInputs
{
public:
    VEX_GeoInputs() {}
    virtual ~VEX_GeoInputs() {}
};

#endif


