/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GDT_CaptureWeight.h  (Geo Delta Library)
 *
 * COMMENTS:	This contains a full list of vectors that describe the changes
 *		to the gdp's point capture weight attributes.
 *
 */

#ifndef __GDT_CaptureWeight_h__
#define __GDT_CaptureWeight_h__

#include "GDT_API.h"
#include <UT/UT_IntArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_VectorTypes.h>
#include <GA/GA_Types.h>

#include "GDT_Token.h"

class GA_AIFEditDelta;
class GA_AIFEditSnapshot;
class GA_Attribute;
class GEO_Detail;
class GDT_TokenList;
class GU_DetailHandle;

typedef int (*GDT_CaptureRegionCB)( void *user_data,
				    char *cregion,
				    UT_Matrix4 &parent_xform,
				    GU_DetailHandle &tube_gdp );


class GDT_API GDT_CaptureWeight : public GDT_Token
{
public:
    GDT_CaptureWeight();
    virtual ~GDT_CaptureWeight();

    GDT_CaptureWeight( const GDT_CaptureWeight &other );

    virtual int64 getMemoryUsage(bool inclusive) const;

    void		    getOpPaths( UT_StringList &paths );
    void		    getOpPathsCopy( UT_ValArray<char *> &paths );
    void		    setOpPaths( const UT_ValArray<char *> &paths );
    int			    changeOpPaths( const char *old_name,
					   const char *new_name );
    void		    changeCaptureRegionRoot( const char *old_root,
						     const char *new_root );
    void		    refreshCaptureFrameInfo( 
					const GEO_Detail &gdp,
					GDT_CaptureRegionCB get_region,
					void *user_data );

    virtual void	    applyDeltas( const GDT_TokenList &token_list,
					 GEO_Detail &gdp,
					 const GA_AIFEditDeltaMap &map,
					 bool inverse=false ) const;
    virtual void	    applyScaledDelta(GEO_Detail &gdp, GA_Offset pt,
					fpreal scale,
					const GA_AIFEditDeltaMap &map) const;

    void		    mergeCaptureWeightDeltas(
				const GDT_CaptureWeight &other,
				bool inverse = false );

    virtual void	    zeroDeltas();
    
    // Begin/end a change to a point capture weight
    void		    beginCaptureWeightChange( const GA_Attribute *capt,
						      GA_Offset element );
    void		    endCaptureWeightChange(
						const GA_AIFEditDeltaMap &map);

    virtual unsigned int    changed() const;

    // I/O Methods
    virtual bool	    legacyLoad( UT_IStream &is, unsigned int numtotal );

    virtual bool	    save( UT_JSONWriter &w, const GDT_JSON &jsn ) const;
    virtual bool	    load( UT_JSONParser &p, const GDT_JSON &jsn );

private:    // methods
    void		    destroy();

private:    // data
    GA_AIFEditDelta			*myDelta;
    GA_AIFEditSnapshot			*mySnapshot;
};

#endif // __GDT_CaptureWeight_h__
