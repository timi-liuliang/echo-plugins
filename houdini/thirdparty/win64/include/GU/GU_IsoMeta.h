/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_IsoMeta.h ( GU Library, C++)
 *
 * COMMENTS:	Fast meta-ball polygonization.
 *
 * CAVEATS:	The conversion can only happen once (i.e. it's not possible to
 *		convert multiple times since the data queue gets used up and
 *		needs to be re-initialized.
 */

#ifndef __GU_IsoMeta__
#define __GU_IsoMeta__

#include "GU_API.h"
#include "GU_Iso.h"
#include <GA/GA_AttributeRefMap.h>
#include <GA/GA_Handle.h>
#include <TS/TS_Expression.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_IntArray.h>

class GA_PrimitiveGroup;
class GEO_Primitive;
class GEO_MetaPrim;
class TS_MetaExpression;
class GEO_MetaExpression;

class GU_API GU_IsoMeta : public GU_IsoSurface
{
public:
	     GU_IsoMeta(GU_Detail *dest, const GU_Detail *src,
			const GA_PrimitiveGroup *grp = NULL);
	     GU_IsoMeta(GU_Detail *dest, const GU_Detail *src,
			const GEO_MetaExpression *expr);
    virtual ~GU_IsoMeta();

    const UT_BoundingBox	&getBounds() const { return myBox; }

    virtual float	evalEdge(const UT_Vector3 &pt);
    virtual bool	hasGradient() const	{ return true; }
    virtual UT_Vector3	evalGrad(const UT_Vector3 &pt);

private:
    virtual void	fillPointVertexAttributes(GU_Detail &detail,
                                                  GA_Offset vtxoff);
    virtual void	fillPrimitiveAttributes(GEO_Primitive *primitive);

private:
    void		 construct(GU_Detail *dest, const GU_Detail *src,
				   const TS_MetaExpressionPtr &expr);

private:
    GA_AttributeRefMap  myVertexHandles;
    GA_AttributeRefMap  myPrimitiveHandles;
    GA_RWHandleV3       myN;
    TS_MetaExpressionPtr myExpr;
    UT_BoundingBox      myBox;
};

#endif
