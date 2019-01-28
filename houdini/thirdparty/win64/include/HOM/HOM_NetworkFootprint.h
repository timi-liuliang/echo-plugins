/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_NetworkFootprint_h__
#define __HOM_NetworkFootprint_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_Color.h"
#include "HOM_EnumValue.h"
#include <UT/UT_Color.h>
#include <UT/UT_StringHolder.h>
#include <string>

SWIGOUT(%rename(NetworkFootprint) HOM_NetworkFootprint;)

class HOM_API HOM_NetworkFootprint
{
public:
    HOM_NetworkFootprint(
	    HOM_EnumValue *flag,
	    const HOM_Color &color,
	    int ring,
	    bool use_minimum_size)
	: myFlag(flag),
	  myNodeType(),
	  myColor(color.myColor),
	  myRing(ring),
	  myUseMinimumSize(use_minimum_size)
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_NetworkFootprint(
	    const char *node_type,
	    const HOM_Color &color,
	    int ring,
	    bool use_minimum_size)
	: myFlag(NULL),
	  myNodeType(node_type),
	  myColor(color.myColor),
	  myRing(ring),
	  myUseMinimumSize(use_minimum_size)
    { HOM_CONSTRUCT_OBJECT(this) }

    ~HOM_NetworkFootprint()
    { HOM_DESTRUCT_OBJECT(this) }

    std::string	 __repr__();

    SWIGOUT(%ignore myFlag;)
    HOM_EnumValue	*myFlag;
    SWIGOUT(%ignore myNodeType;)
    UT_StringHolder	 myNodeType;
    SWIGOUT(%ignore myColor;)
    UT_Color		 myColor;
    SWIGOUT(%ignore myRing;)
    int			 myRing;
    SWIGOUT(%ignore myUseMinimumSize;)
    bool		 myUseMinimumSize;
};

#endif

