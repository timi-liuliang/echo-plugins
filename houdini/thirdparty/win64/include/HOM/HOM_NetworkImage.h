/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_NetworkImage_h__
#define __HOM_NetworkImage_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_ElemPtr.h"
#include "HOM_BoundingRect.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_BoundingRect.h>
#include <string>

SWIGOUT(%rename(NetworkImage) HOM_NetworkImage;)

class HOM_API HOM_NetworkImage
{
public:
    HOM_NetworkImage(const char *path = NULL,
	    const HOM_BoundingRect &rect = HOM_BoundingRect())
	: myPath(path),
	  myRelativeToPath(),
	  myRect(rect.myBoundingRect),
	  myBrightness(1.0)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NetworkImage(const HOM_NetworkImage &src)
	: myPath(src.myPath),
	  myRelativeToPath(src.myRelativeToPath),
	  myRect(src.myRect),
	  myBrightness(src.myBrightness)
    { HOM_CONSTRUCT_OBJECT(this) }

    ~HOM_NetworkImage()
    { HOM_DESTRUCT_OBJECT(this) }

    bool operator==(HOM_PtrOrNull<HOM_NetworkImage> image);
    bool operator!=(HOM_PtrOrNull<HOM_NetworkImage> image)
    { return !operator==(image); }

    int __hash__();
    std::string __str__();
    std::string	 __repr__();

    void		 setPath(const char *path);
    std::string		 path();

    void		 setRelativeToPath(const char *path);
    std::string		 relativeToPath();

    void		 setRect(HOM_BoundingRect &rect);
    HOM_BoundingRect	 rect();

    void		 setBrightness(double brightness);
    double		 brightness();

    SWIGOUT(%ignore myPath;)
    UT_StringHolder	 myPath;
    SWIGOUT(%ignore myRelativeToPath;)
    UT_StringHolder	 myRelativeToPath;
    SWIGOUT(%ignore myRect;)
    UT_BoundingRectD	 myRect;
    SWIGOUT(%ignore myBrightness;)
    double		 myBrightness;
};

#endif

