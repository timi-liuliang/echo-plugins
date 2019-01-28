/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GDT Library.  (Geo Delta)
 *
 * COMMENTS:	A GDT_Transform contains a list of matrices that describe
 *		the changes to the gdp's primitive transforms.
 *
 */
#ifndef __GDT_Transform_h__
#define __GDT_Transform_h__

#include "GDT_API.h"
#include <UT/UT_BitArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Matrix3.h>

#include "GDT_Token.h"

class GA_PrimitiveGroup;
class GDT_TokenList;

class GDT_API GDT_Transform : public GDT_Token
{
public:
    GDT_Transform(unsigned int size) : myUsedEntries(size) {}
    GDT_Transform(const GDT_Transform &other);

    virtual ~GDT_Transform();

    virtual int64   getMemoryUsage(bool inclusive=false) const;

    virtual void	 	applyDeltas(const GDT_TokenList &list,
					    GEO_Detail &gdp,
					    const GA_AIFEditDeltaMap &map,
					    bool inverse=false) const;

    void			mergeTransformDeltas(
					    const GDT_Transform &other,
					    bool inverse = false);

    // Zero all the deltas non-destructively
    virtual void		zeroDeltas();
    
    // Begin/end a change to a point position
    void		 	beginPrimitiveTransformChange(
						    const UT_Matrix3 &mat);
    void		 	endPrimitiveTransformChange(
						    const UT_Matrix3 &endmat,
						    unsigned int index);

    virtual void		resize(unsigned int size);

    virtual unsigned int	changed() const
				{ return myUsedEntries.numBitsSet(); }
    virtual unsigned int	entries() const 
				{ return myUsedEntries.size(); }

    // I/O Methods
    virtual bool	 	legacyLoad(UT_IStream &is,
					   unsigned int numtotal);

    virtual bool		save(UT_JSONWriter &w,
				     const GDT_JSON &json) const;
    virtual bool	 	load(UT_JSONParser &p,
				     const GDT_JSON &json);

private:
    UT_Matrix3			 mySavedMat;

    UT_ValArray<UT_Matrix3 *> 	 myValues;
    UT_BitArray			 myUsedEntries;
};

#endif
