/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CHOP_Fitter.h ( CHOP Library, C++)
 *
 * COMMENTS:
 *
 *	Utilities to convert between raw tracks and channels
 */

#ifndef __CHOP_Fitter__
#define __CHOP_Fitter__

#include "CHOP_API.h"

#include <CH/CH_Filters.h>
#include <CL/CL_Clip.h>
#include <OP/OP_Channels.h>
#include <UT/UT_StringArray.h>

class CHOP_Node;

class CHOP_API CHOP_Fitter
{

public:

     CHOP_Fitter();
    ~CHOP_Fitter();

    int		 convertToCollection(const CL_Clip &clip,
				     OP_Channels &coll,
				     fpreal error2, 
				     int start, int end);

    void	 convertToTrack(CH_Channel *channel, CL_Track &track,
	    			int istart, int iend);

    void	 setScope(OP_Channels &coll, const UT_String &scope);

    static const char *fixName(const char *input);

private:

    UT_Vector2R		*myPoints;      // simple array
    fpreal		*myValues;
    int			 myPointSize;

    int			 growArray(int size);

};

// Implementation of CH_FilterRangeBuilder
// begin, end and copyChopChannels.
// Needed to be in a shared library other than CH because 
// it accesses OP_Node and CHOP.
class CHOP_API CHOP_CopyChannels
{
public:
    CHOP_CopyChannels();
    virtual ~CHOP_CopyChannels();

    virtual void begin(CH_BakeChop mode);
    virtual void end();
    virtual void copy(CH_Channel *chp, fpreal global_start, fpreal global_end);

private:
    CH_BakeChop myMode;  // Bake mode
    int		myCount; // to check for balanced calls

    typedef UT_Map<CHOP_Node*,UT_StringArray> chopExportsMap;
    chopExportsMap myExports; // Baked Exports
    void append( OP_Node *chop, const UT_String &track );
};

// Create a channel with an expression pointing back to the chop export channel.
// It creates the channel only if a chop export is in effect.
// The returned CH_Channel won't be added to its parm.
//
// For example, if sphere_object1 tx parameter is overriden by a chop export from a motionfx,
// a new channel will be created and it will have a segment with the following expression.
//  `channel('/obj/motionfx/add1/sphere_object:tx`
//
// This is used by the Animation Editor to create temporary channels to visualize CHOP exports.
// It's also used by HOMF_Parm::keyframesRefit() to bake chop exports.
CHOP_API extern CH_Channel *
CHOPcreateExpressionChannel( const CH_ChannelRef &ref, fpreal t, int thread );

// Create a channel with a given expression.
// The returned CH_Channel won't be added to its parm.
// This is used by HOMF_Parm::keyframesRefit() to bake expressions.
CHOP_API extern CH_Channel *
CHOPcreateExpressionChannel( const CH_ChannelRef &ref, const char *expr );

#endif

