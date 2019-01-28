/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_NetworkShape_h__
#define __HOM_NetworkShape_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"
#include "HOM_Color.h"
#include "HOM_Vector2.h"
#include "HOM_BoundingRect.h"
#include <UT/UT_Color.h>
#include <UT/UT_BoundingRect.h>
#include <UT/UT_StringHolder.h>
#include <string>

SWIGOUT(%rename(NetworkShape) HOM_NetworkShape;)
SWIGOUT(%rename(NetworkShapeLine) HOM_NetworkShapeLine;)
SWIGOUT(%rename(NetworkShapeBox) HOM_NetworkShapeBox;)
SWIGOUT(%rename(NetworkShapeNodeShape) HOM_NetworkShapeNodeShape;)
SWIGOUT(%rename(NetworkShapeConnection) HOM_NetworkShapeConnection;)

class HOM_API HOM_NetworkShape
{
public:
    HOM_NetworkShape()
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_NetworkShape()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string	 __repr__() = 0;
};

class HOM_API HOM_NetworkShapeLine : public HOM_NetworkShape
{
public:
    SWIGOUT(%kwargs HOM_NetworkShapeLine;)
    HOM_NetworkShapeLine(
	    const HOM_Vector2 &start,
	    const HOM_Vector2 &end,
	    const HOM_Color &color = UT_Color(UT_RGB, 1.0, 1.0, 1.0),
	    float alpha = 1.0,
	    float width = 1.0,
	    bool screen_space = true,
	    bool smooth = true,
	    bool dashed = false)
	: myStart(start.myVector2),
	  myEnd(end.myVector2),
	  myColor(color.myColor),
	  myAlpha(alpha),
	  myWidth(width),
	  myScreenSpace(screen_space),
	  mySmooth(smooth),
	  myDashed(dashed)
    { }

    virtual std::string	 __repr__();

    SWIGOUT(%ignore myStart;)
    UT_Vector2D		 myStart;
    SWIGOUT(%ignore myEnd;)
    UT_Vector2D		 myEnd;
    SWIGOUT(%ignore myColor;)
    UT_Color		 myColor;
    SWIGOUT(%ignore myAlpha;)
    float		 myAlpha;
    SWIGOUT(%ignore myWidth;)
    float		 myWidth;
    SWIGOUT(%ignore myScreenSpace;)
    bool		 myScreenSpace;
    SWIGOUT(%ignore mySmooth;)
    bool		 mySmooth;
    SWIGOUT(%ignore myDashed;)
    bool		 myDashed;
};

class HOM_API HOM_NetworkShapeBox : public HOM_NetworkShape
{
public:
    SWIGOUT(%kwargs HOM_NetworkShapeBox;)
    HOM_NetworkShapeBox(
	    const HOM_BoundingRect &rect,
	    const HOM_Color &color = UT_Color(UT_RGB, 1.0, 1.0, 1.0),
	    float alpha = 1.0,
	    bool fill = true,
	    bool screen_space = true,
	    bool smooth = true)
	: myRect(rect.myBoundingRect),
	  myColor(color.myColor),
	  myAlpha(alpha),
	  myFill(fill),
	  myScreenSpace(screen_space),
	  mySmooth(smooth)
    { }

    virtual std::string	 __repr__();

    SWIGOUT(%ignore myRect;)
    UT_BoundingRectD	 myRect;
    SWIGOUT(%ignore myColor;)
    UT_Color		 myColor;
    SWIGOUT(%ignore myAlpha;)
    float		 myAlpha;
    SWIGOUT(%ignore myFill;)
    bool		 myFill;
    SWIGOUT(%ignore myScreenSpace;)
    bool		 myScreenSpace;
    SWIGOUT(%ignore mySmooth;)
    bool		 mySmooth;
};

class HOM_API HOM_NetworkShapeNodeShape : public HOM_NetworkShape
{
public:
    SWIGOUT(%kwargs HOM_NetworkShapeNodeShape;)
    HOM_NetworkShapeNodeShape(
	    const HOM_BoundingRect &rect,
	    const std::string shape,
	    const HOM_Color &color = UT_Color(UT_RGB, 1.0, 1.0, 1.0),
	    float alpha = 1.0,
	    bool fill = true,
	    bool screen_space = true,
	    bool smooth = true)
	: myRect(rect.myBoundingRect),
	  myShape(shape),
	  myColor(color.myColor),
	  myAlpha(alpha),
	  myFill(fill),
	  myScreenSpace(screen_space),
	  mySmooth(smooth)
    { }

    virtual std::string	 __repr__();

    SWIGOUT(%ignore myRect;)
    UT_BoundingRectD	 myRect;
    SWIGOUT(%ignore myShape;)
    UT_StringHolder	 myShape;
    SWIGOUT(%ignore myColor;)
    UT_Color		 myColor;
    SWIGOUT(%ignore myAlpha;)
    float		 myAlpha;
    SWIGOUT(%ignore myFill;)
    bool		 myFill;
    SWIGOUT(%ignore myScreenSpace;)
    bool		 myScreenSpace;
    SWIGOUT(%ignore mySmooth;)
    bool		 mySmooth;
};

class HOM_API HOM_NetworkShapeConnection : public HOM_NetworkShape
{
public:
    SWIGOUT(%kwargs HOM_NetworkShapeConnection;)
    HOM_NetworkShapeConnection(
	    const HOM_Vector2 &output_pos,
	    const HOM_Vector2 &output_dir,
	    const HOM_Vector2 &input_pos,
	    const HOM_Vector2 &input_dir,
	    const HOM_Color &color = UT_Color(UT_RGB, 1.0, 1.0, 1.0),
	    float alpha = 1.0,
	    float fade_factor = 0.0,
	    bool smooth = true,
	    bool dashed = false)
	: myOutputPos(output_pos.myVector2),
	  myOutputDir(output_dir.myVector2),
	  myInputPos(input_pos.myVector2),
	  myInputDir(input_dir.myVector2),
	  myColor(color.myColor),
	  myAlpha(alpha),
	  myFadeFactor(fade_factor),
	  mySmooth(smooth),
	  myDashed(dashed)
    { }

    virtual std::string	 __repr__();

    SWIGOUT(%ignore myOutputPos;)
    UT_Vector2D		 myOutputPos;
    SWIGOUT(%ignore myOutputDir;)
    UT_Vector2D		 myOutputDir;
    SWIGOUT(%ignore myInputPos;)
    UT_Vector2D		 myInputPos;
    SWIGOUT(%ignore myInputDir;)
    UT_Vector2D		 myInputDir;
    SWIGOUT(%ignore myColor;)
    UT_Color		 myColor;
    SWIGOUT(%ignore myAlpha;)
    float		 myAlpha;
    SWIGOUT(%ignore myFadeFactor;)
    float		 myFadeFactor;
    SWIGOUT(%ignore mySmooth;)
    bool		 mySmooth;
    SWIGOUT(%ignore myDashed;)
    bool		 myDashed;
};

#endif

