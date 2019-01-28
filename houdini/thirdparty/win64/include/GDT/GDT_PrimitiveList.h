/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GDT Library. (Geo Delta)
 *
 * COMMENTS:	GDT_PrimitiveList contains tokens for primitive attributes.
 *		All these tokens are not sparse.
 *
 */
#ifndef __GDT_PrimitiveList_h__
#define __GDT_PrimitiveList_h__

#include "GDT_API.h"
#include "GDT_TokenList.h"

#include <GEO/GEO_Detail.h>
#include "GDT_Transform.h"

class GA_PrimitiveGroup;


class GDT_API GDT_PrimitiveList : public GDT_TokenList
{
public:
    GDT_PrimitiveList() : myTransform(0) {}
    GDT_PrimitiveList(const GDT_PrimitiveList &other);
    virtual ~GDT_PrimitiveList();

    // Implement the pure virtual applyDeltas() from GDT_TokenList.
    virtual void	 applyDeltas(GEO_Detail &gdp, bool inverse=false) const;

    // Merge the primitive deltas from another GDT_PrimitiveList
    void		 mergePrimitiveDeltas(const GDT_PrimitiveList &other,
					      bool inverse = false);

    // Overload hiding base class method which requires a GA_AIFEditDeltaMap.
    void		 endAttributeChange(const GEO_Detail &gdp);

    // Begin and end a transform change to the primitive specified by index.
    // The delta matrix is computed and stored.
    void		 beginPrimitiveTransformChange(const UT_Matrix3 &mat);
    void		 endPrimitiveTransformChange(const UT_Matrix3 &mat,
						     unsigned int index);

    virtual const GA_AttributeDict &getAttributeDict(
						const GEO_Detail &gdp) const
						{ return gdp.primitiveAttribs(); }

    virtual bool		 hasChange() const;
    virtual bool		 haveAttributesChanged() const
				 { return GDT_TokenList::hasChange(); }
    bool			 hasTransformChanged() const
				 {
				     return (myTransform &&
					     myTransform->changed() > 0);
				 }
    
    virtual bool		 legacyLoadTrans(UT_IStream &is, uint numtotal);

    bool			 saveTrans(UT_JSONWriter &w,
					   const GDT_JSON &json) const;
    bool			 loadTrans(UT_JSONParser &p,
					   const GDT_JSON &json);

    // Zero the primitive deltas
    virtual void		 zeroDeltas();
    // Destroy the primitive deltas, but not the attribute lists
    virtual void		 clearDeltas();
    // Destroy the primitive deltas
    virtual void		 destroy();

private:
    // As with the position token, the transform token for primitives
    // is kept apart from the token list.
    GDT_Transform      *myTransform;
};
#endif
