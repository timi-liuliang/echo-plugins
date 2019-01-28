/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GDT Library. (Geo Delta)
 *
 * COMMENTS:	GDT_PointList contains tokens for point attributes and
 *		a position token.  All these tokens are not sparse.
 *
 */
#ifndef __GDT_PointList_h__
#define __GDT_PointList_h__

#include "GDT_API.h"
#include "GDT_TokenList.h"

#include <GA/GA_Types.h>
#include <GEO/GEO_Detail.h>
#include "GDT_CaptureWeight.h"
#include "GDT_Position.h"
#include "GDT_CoordinateFrames.h"

class GA_PointGroup;

class GDT_API GDT_PointList : public GDT_TokenList
{
public:
	    GDT_PointList() : myPosition(0), myCaptureWeight(0), myCoordinateFrames(0) {}
	    GDT_PointList(const GDT_PointList &other);
    virtual ~GDT_PointList();

    // Implement the pure virtual applyDeltas() from GDT_TokenList.
    virtual void	 applyDeltas(GEO_Detail &gdp, bool inverse=false) const;

    virtual void	 applyScaledDelta(GEO_Detail &gdp,
					GA_Offset pt, float scale) const;
    virtual void	 applyScaledPosDeltas(GEO_Detail &gdp,
					      float scale,
					      const GA_PointGroup *ptgroup = 0) const;

    // Merge the point deltas from another GDT_PointList into this one
    void	 	 mergePointDeltas(const GDT_PointList &other,
					  bool inverse = false);

    // Overload hiding base class method which requires a GA_AIFEditDeltaMap.
    void		 endAttributeChange(const GEO_Detail &gdp);

    // Begin/end a change to a point position
    void		 beginPointPositionChange(const GA_Attribute *P,
						  GA_Offset element);
    void		 beginPointPositionChange(const GA_Attribute *P,
						  const GA_Range &range);
    void		 endPointPositionChange(const GEO_Detail &gdp);

    void		 getOpPaths( UT_StringList &paths );
    void		 getOpPathsCopy( UT_ValArray<char *> &paths );
    void		 setOpPaths( const UT_ValArray<char *> &paths );
    int			 changeOpPaths( const char *old_name,
				        const char *new_name );
    bool		 changeCaptureRegionRoot( const char *old_root,
						  const char *new_root );
    void		 refreshCaptureFrameInfo( 
				const GEO_Detail &gdp,
				GDT_CaptureRegionCB get_region,
				void *user_data );
    void		 initCaptureWeightChange( const GEO_Detail &gdp );
    void		 beginCaptureWeightChange(
				GA_Offset element );
    void		 endCaptureWeightChange(const GEO_Detail &gdp);

    virtual const GA_AttributeDict &getAttributeDict(
						const GEO_Detail &gdp) const
						{ return gdp.pointAttribs(); }

    virtual bool		 hasChange() const;
    virtual bool		 haveAttributesChanged() const
				 { return GDT_TokenList::hasChange(); }
    // Returns true if point position data is present, return false otherwise
    bool			 hasPositionChanged() const 
				 { 
				     return (myPosition && 
				 	     myPosition->changed() > 0); 
				 }
    bool			 hasCaptureWeightChanged() const
				 {
				     return (myCaptureWeight && 
				 	     myCaptureWeight->changed() > 0); 
				 }

    // If myPosition has been blanked out, then loadPos will pre-allocate
    // numtotal slots in the position array.  If myPosition exists, then this
    // number is ignored and the data is loaded and merged with what already
    // exists.
    bool			 legacyLoadPos(UT_IStream &is, uint numtotal);

    bool			 savePos(UT_JSONWriter &w,
					 const GDT_JSON &json) const;
    bool			 loadPos(UT_JSONParser &p,
					 const GDT_JSON &json);

    bool			 legacyLoadCaptureWeight(UT_IStream &is,
						   unsigned int numtotal);

    bool			 saveCaptureWeight(UT_JSONWriter &w,
						   const GDT_JSON &json) const;
    bool			 loadCaptureWeight(UT_JSONParser &p,
						   const GDT_JSON &json);

    // Zero the point deltas
    virtual void		 zeroDeltas();
    // Destroy the point deltas, but not the attribute lists
    virtual void		 clearDeltas();
    // Destroy the point deltas
    virtual void		 destroy();

    void			 setCoordinateFrames(const GDT_CoordinateFrames *coords);
    const GDT_CoordinateFrames	*getCoordinateFrames();

    void			 createSymmetryDeltas(const GDT_PointList &input, GDT_SymmetryTransform *transform);

protected:
    GDT_Position	*newPosition(const GDT_Position &p);
    GDT_Position	*newPosition();

private:
    // The position token is kept apart from the token list, so that
    // we can make the token list general and not have to check extra cases.
    GDT_Position	*myPosition;

    GDT_CaptureWeight	*myCaptureWeight;

    const GDT_CoordinateFrames  *myCoordinateFrames;
};
#endif
