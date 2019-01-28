/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO library (C++)
 *
 * COMMENTS:	base class for interface to Tesseara (meta) library
 *
 */

#ifndef __GEO_MetaPrim_h__
#define __GEO_MetaPrim_h__

#include "GEO_API.h"
#include <iosfwd>
#include <UT/UT_Vector3.h>
#include <UT/UT_BoundingBox.h>
#include <GA/GA_IntrinsicManager.h>	// For the inner Registrar class

class TS_MetaPrimitive;		// Primitive
class TS_MetaKernel;
class GA_PrimitiveDefinition;

class GEO_API GEO_MetaPrim
{
public:
    GEO_MetaPrim();
    virtual ~GEO_MetaPrim() {}

    virtual TS_MetaPrimitive	*getNewMetaPrim() const = 0;
    virtual float                density(const UT_Vector3 &pos) const = 0;
    virtual void                 getBBox(UT_BoundingBox &bbox) const = 0;

    bool			 setMetaKernel(const char *type);
    const char			*getMetaKernel() const;
    void			 setWeight(float w) { weight = w; }
    float			 getWeight() const { return weight; }

protected:
    bool		saveMetaH9(std::ostream &os, bool binary) const;
    bool		loadMetaH9(UT_IStream &is);

    float		 weight;
    const TS_MetaKernel	*kernel;
};

#endif
