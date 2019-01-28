/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GDT Library.  (Geo Delta)
 *
 * COMMENTS:	A GDT_Position contains a full list of vectors that describe
 *		the changes to the gdp's point positions.
 *
 */
#ifndef __GDT_Position_h__
#define __GDT_Position_h__

#include "GDT_API.h"
#include <UT/UT_BitArray.h>
#include <UT/UT_Vector4Array.h>
#include <GA/GA_Types.h>

#include "GDT_Token.h"

#include "GDT_CoordinateFrames.h"
#include "GDT_SymmetryTransform.h"

class GA_PointGroup;
class GA_AIFEditDelta;
class GA_AIFEditSnapshot;
class GDT_TokenList;

class GDT_API GDT_Position : public GDT_Token
{
public:
    GDT_Position();
    GDT_Position(const GDT_Position &other);
    virtual ~GDT_Position();

    virtual int64 getMemoryUsage(bool inclusive=false) const;

    virtual void	 	applyDeltas(const GDT_TokenList &list,
					    GEO_Detail &gdp,
					    const GA_AIFEditDeltaMap &map,
					    bool inverse=false) const;
    virtual void		applyScaledDelta(GEO_Detail &gdp, GA_Offset pt,
					    fpreal64 scale,
					    const GA_AIFEditDeltaMap &map) const;
    virtual void		applyScaledDeltas(GEO_Detail &gdp,
					    fpreal64 scale,
					    const GA_PointGroup *ptgroup,
					    const GA_AIFEditDeltaMap &map) const;

    void			mergePositionDeltas(const GDT_Position &other,
						    bool inverse = false);

    // Zero all the deltas non-destructively
    virtual void		zeroDeltas();
    
    // Begin/end a change to a point position
    void		 	beginPointPositionChange(const GA_Attribute *P,
							 GA_Offset element);
    void		 	beginPointPositionChange(const GA_Attribute *P,
							 const GA_Range &range);
    void		 	endPointPositionChange(
						const GA_AIFEditDeltaMap &map);

    virtual unsigned int	changed() const;

    // I/O Methods
    virtual bool	 	legacyLoad(UT_IStream &is,
					   unsigned int numtotal);

    virtual bool		save(UT_JSONWriter &w,
				     const GDT_JSON &json) const;
    virtual bool		load(UT_JSONParser &p,
				     const GDT_JSON &json);

    void			setCoordinateFrames(const GDT_CoordinateFrames *coords);

    void			createSymmetryDeltas(const GDT_Position &input,
					    GDT_SymmetryTransform *transform,
					    const GA_AIFEditDeltaMap &map);

private:
    GA_AIFEditDelta		*myDelta;
    GA_AIFEditSnapshot		*mySnapshot;

    const GDT_CoordinateFrames  *myCoordinateFrames;
};

#endif
