/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __GEO_PrimMetaSQuad_H__
#define __GEO_PrimMetaSQuad_H__

#include "GEO_API.h"
#include <UT/UT_Vector3.h>
#include "GEO_MetaPrim.h"
#include "GEO_PrimSphere.h"

class TS_MetaSuperQuadric;
class UT_MemoryCounter;

class GEO_API GEO_PrimMetaSQuad : public GEO_PrimSphere, public GEO_MetaPrim
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    GEO_PrimMetaSQuad(GEO_Detail *d, GA_Offset offset = GA_INVALID_OFFSET);

    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    virtual ~GEO_PrimMetaSQuad() {}

public:
    virtual void		 copyPrimitive(const GEO_Primitive *src);
    virtual bool		 isDegenerate() const;
    virtual GEO_Primitive	*copy(int preserve_shared_pts = 0) const; 
    virtual void		 copyUnwiredForMerge(const GA_Primitive *src,
						     const GA_MergeMap &map);

//
//  Methods to handle meta interface
    void                         fillTSPrim(TS_MetaSuperQuadric &tsprim) const;
    virtual GEO_MetaPrim	*castToMetaPrim();
    virtual const GEO_MetaPrim	*castToMetaPrim() const;

    // A function that allocates a new TS_MetaSuperQuadric and returns it.
    // Used in the metaExpression parser to handle multiple instances
    // of a metaball being used in the same expression
    virtual TS_MetaPrimitive	*getNewMetaPrim() const;

    virtual float                density(const UT_Vector3 &pos) const;
    virtual void                 getBBox(UT_BoundingBox &bbox) const;

    virtual int                  getBBox(UT_BoundingBox *bbox) const
    { return GEO_PrimSphere::getBBox(bbox); }

    void		setXYexp(float v) { xyExp = v; }
    void		setZexp(float v) { zExp = v; }
    float		getXYexp(void) const { return xyExp; }
    float		getZexp(void) const { return zExp; }

    virtual const GA_PrimitiveJSON	*getJSON() const;

protected:
    /// We explicitly do not include the mask from the GEO_PrimSphere base.
    static GA_PrimitiveFamilyMask 	buildFamilyMask()
					    { return GEO_FAMILY_META; }

    // Declare methods for intrinsic attributes
    GA_DECLARE_INTRINSICS(GA_NO_OVERRIDE)

    virtual bool 	savePrivateH9(std::ostream &os, bool binary) const;
    virtual bool	loadPrivateH9(UT_IStream &is);

private:

    float xyExp;
    float zExp;
};
#endif
