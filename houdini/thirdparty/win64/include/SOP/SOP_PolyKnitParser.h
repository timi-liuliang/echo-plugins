/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS:
 *	Parser for the PolyKnit SOP.
 *
 *	Grammar for the format:
 *	Legend:
 *		/\  - empty
 *		<?> - non-terminal
 *		#   - number
 *
 *	<S> 	-> 	<T0> | <Q0> | /\
 *	<T0>	->	t###<T1> | ###<T1> | t###<Q1> | ###<Q1>
 *	<T1>	->	#<T1> | t#<T1> | #<Q1> | t#<Q1> | /\
 *	<Q0>	->	q####<T1> | ####<T1> | q####<Q1> | ####<Q1> | ##q##<Q1>
 *			| #q###<T1> | #q###<Q1>
 *	<Q1>	->	##<Q1> | q##<Q1> | ##<T1> | q##<T1> | /\
 *
 */

#ifndef __SOP_PolyKnitParser_h__
#define __SOP_PolyKnitParser_h__

#include "SOP_API.h"
#include <UT/UT_IntArray.h>

#include <GOP/GOP_Parser.h>
#include <GU/GU_Detail.h>

// Parser tokens
#define		CHAR_TRIANGLE		't'
#define		CHAR_QUAD		'q'
#define		CHAR_META_TRIANGLE	'T'
#define		CHAR_META_QUAD		'Q'

class SOP_Node;

class SOP_API SOP_PolyKnitParser : public GOP_Parser
{
public:
     SOP_PolyKnitParser(UT_String &str, GU_Detail &gdp,
			UT_IntArray &ptArray, UT_IntArray &typeArray);
    ~SOP_PolyKnitParser() {}

    bool			haveMetas() const { return myHaveMetas; }

    // Returns the token which caused the error
    const char		       *errorToken() const 
			        { return (const char *)myErrorToken; }

protected:
    virtual void		handleError();

private:
    virtual void		init();
    virtual void		finish();
    virtual bool		dispatch(char c);

    // State handlers
    virtual void		handleStart();

    void			handleTrianglePoint();
    void			handleQuadPoint();
    void			handleMetaTrianglePoint();
    void			handleMetaQuadPoint();
    void			handlePoint(int polytype, int numpoints);

    // Updates the myCanSwitch flag based on new point count
    void			updateCanSwitch();

    // Small method to make code a little cleaner
    void			switchOutput(int out);

    GU_Detail	       	       &myGdp;
    UT_IntArray		       &myPtArray;
    UT_IntArray 	       &myTypeArray;

    // Points seen so far in the current primitive being constructed.
    int				myCurrentPrimPoints;	

    // Contains the current polygon type 
    int				myCurrentPolyType;

    // Are we at a point where we can switch the poly
    bool			myCanSwitch;

    // Flag for whether or not we have meta polys
    bool			myHaveMetas;

    UT_String			myErrorToken;
    int				myErrorPoint;
};

#endif

