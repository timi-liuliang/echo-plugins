/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO library (C++)
 *
 * COMMENTS:	Particle Render Options
 *
 */

#ifndef __GEO_PartRender_h__
#define __GEO_PartRender_h__

#include "GEO_API.h"
#include <SYS/SYS_Math.h>
#include <iosfwd>
class UT_IStream;
class UT_JSONWriter;
class UT_JSONParser;
class UT_JSONValue;

/// Geometry type to use when rendering particles.
enum GEO_PART_TYPE
{
    GEO_PARTICLE_SPHERE,		///< Render as spheres
    GEO_PARTICLE_CIRCLE,		///< Render as oriented circles
    GEO_PARTICLE_LINE,			///< Render as lines
    GEO_PARTICLE_TUBE,			///< Render as open tubes
    GEO_PARTICLE_TUBEC,			///< Render as capped tubes
    GEO_PARTICLE_TUBES			///< Render as rounded tubes
};

#define GEO_PART_MOTIONBLUR	0x0001	///< Particles are motion blurred
#define GEO_PART_VIRTUAL	0x0004	///< Particles not to be rendered

/// A class to manage the various settings that control how a particle system
/// should be rendered.
class GEO_API GEO_PartRender
{
public:
     GEO_PartRender();
    ~GEO_PartRender();

    void		setDefaults();

    bool		saveH9(std::ostream &os, bool binary) const;
    bool		loadH9(UT_IStream &is);

    GEO_PartRender	&operator=(const GEO_PartRender &src)
			{
			    myFlag = src.myFlag;
			    mySize = src.mySize;
			    myBlurTime = src.myBlurTime;
			    myType = src.myType;
			    return *this;
			}
    bool		isEqual(const GEO_PartRender &src,
				fpreal tol=SYS_FTOLERANCE) const
			{
			    return myFlag == src.myFlag
				&& SYSisEqual(mySize, src.mySize, tol)
				&& SYSisEqual(myBlurTime, src.myBlurTime, tol)
				&& myType == src.myType;
			}

    bool		operator==(const GEO_PartRender &src) const
			{
			    return isEqual(src);
			}

    bool		saveJSON(UT_JSONWriter &w) const;
    bool		saveJSON(UT_JSONValue &v) const;
    bool		loadJSON(UT_JSONParser &p);
    bool		loadJSON(UT_JSONParser &p, const UT_JSONValue &v);

    void		setSize(float radius)	{ mySize = radius; }
    void		setBlurTime(float bt)	{ myBlurTime = bt; }
    void		setType(GEO_PART_TYPE t){ myType = t; }
    void		setMotionBlur(int onOff)
			{
			    if (onOff)	myFlag |=  GEO_PART_MOTIONBLUR;
			    else	myFlag &= ~GEO_PART_MOTIONBLUR;
			}
    void		setVirtual(int onOff)
			{
			    if (onOff)	myFlag |=  GEO_PART_VIRTUAL;
			    else	myFlag &= ~GEO_PART_VIRTUAL;
			}


    float		getSize() const		{ return mySize; }
    float		getBlurTime() const	{ return myBlurTime; }
    GEO_PART_TYPE	getType() const		{ return myType; }
    int			isMotionBlur()	const
			{ return myFlag & GEO_PART_MOTIONBLUR; }
    int			isVirtual() const
			{ return myFlag & GEO_PART_VIRTUAL; }

    /// @{
    /// Get/Set type by name (rather than enum)
    const char		*getTypeName() const;
    void		 setTypeName(const char *type);
    /// @}

private:
    int			myFlag;
    float		mySize;
    float		myBlurTime;
    GEO_PART_TYPE	myType;
};

#endif
