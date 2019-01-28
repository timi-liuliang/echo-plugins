/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO library (C++)
 *
 * COMMENTS:	Element List
 *
 */

#ifndef __GEO_PrimList_h__
#define __GEO_PrimList_h__

#include "GEO_API.h"
#include <GA/GA_GBPrimitiveList.h>
#include <GA/GA_PrimCompat.h>
class GEO_Primitive;

SYS_DEPRECATED_PUSH_DISABLE()

class GEO_API GEO_PrimList : public GA_GBPrimitiveListWMixT<GEO_Primitive,GA_Primitive>
{
public:
    SYS_DEPRECATED_HDK(13.0)
	     GEO_PrimList(const GA_IndexMap &index)
                 : GA_GBPrimitiveListWMixT<GEO_Primitive,GA_Primitive>(index) {}
    SYS_DEPRECATED_HDK(13.0)
    virtual ~GEO_PrimList();

    // Prevent local overloads from hiding base class methods.
    using GA_GBPrimitiveListWMixT<GEO_Primitive,GA_Primitive>::head;
    using GA_GBPrimitiveListWMixT<GEO_Primitive,GA_Primitive>::tail;
    using GA_GBPrimitiveListWMixT<GEO_Primitive,GA_Primitive>::next;
    using GA_GBPrimitiveListWMixT<GEO_Primitive,GA_Primitive>::prev;

    SYS_DEPRECATED_HDK(13.0)
    const GEO_Primitive	*head(const GA_PrimCompat::TypeMask &mask) const;
    SYS_DEPRECATED_HDK(13.0)
    const GEO_Primitive	*tail(const GA_PrimCompat::TypeMask &mask) const;
    SYS_DEPRECATED_HDK(13.0)
    const GEO_Primitive	*next(const GEO_Primitive *,
                              const GA_PrimCompat::TypeMask &mask) const;
    SYS_DEPRECATED_HDK(13.0)
    const GEO_Primitive	*prev(const GEO_Primitive *,
                              const GA_PrimCompat::TypeMask &mask) const;

    SYS_DEPRECATED_HDK(13.0)
    GEO_Primitive	*head(const GA_PrimCompat::TypeMask &mask);
    SYS_DEPRECATED_HDK(13.0)
    GEO_Primitive	*tail(const GA_PrimCompat::TypeMask &mask);
    SYS_DEPRECATED_HDK(13.0)
    GEO_Primitive	*next(GEO_Primitive *,
                              const GA_PrimCompat::TypeMask &mask);
    SYS_DEPRECATED_HDK(13.0)
    GEO_Primitive	*prev(GEO_Primitive *,
                              const GA_PrimCompat::TypeMask &mask);

protected:
};

SYS_DEPRECATED_POP_DISABLE()

#endif
