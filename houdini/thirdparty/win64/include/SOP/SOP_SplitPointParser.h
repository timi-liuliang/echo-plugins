/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS:
 *	Parser for the PolySplit SOP.
 */

#ifndef __SOP_SplitPointParser_h__
#define __SOP_SplitPointParser_h__

#include "SOP_API.h"
#include <GU/GU_EdgeSplit.h>
#include <GOP/GOP_Parser.h>
#include "SOP_PolySplit2.h"

// Parser tokens
#define		CHAR_EDGE		'e'
#define 	CHAR_VERTEX		'v'
#define         CHAR_POINT		'p'
#define		CHAR_FACE_VERTEX	'f'
#define		CHAR_T			':'
#define         CHAR_POINT_DELIM	'-'
#define		CHAR_COMMA_DELIM	','

class SOP_API SOP_SplitPointParser : public GOP_Parser
{
public:
     SOP_SplitPointParser(UT_String &str, float forcedt, GU_Detail &gdp);
    ~SOP_SplitPointParser() {}

protected:
    virtual void		addPoint(GU_SplitLoc *slvert) = 0;
    virtual void		handleError();

    int				getPrim() const { return myPrim; }
    int				getIndex() const { return myIndex; }

private:
    virtual void		init();
    virtual void		finish();
    virtual bool		dispatch(char c);

    // State handlers
    virtual void		handleStart();
    void			handleCollectPrim();
    void			handleCollectVert();
    void			handleCollectEdge();
    void			handleCollectFaceVert();
    void			handleCollectT();
    void			handleCollectU();
    void			handleCollectV();
    void			handleCollectP0();
    void			handleCollectP1();
    void			handleCollectPT();

    // addSplitLocVert and addSplitLocEdge expect the member variables
    // to be properly setup before they are called.
    bool			addSplitLocVert();
    bool			addSplitLocEdge();
    bool			addSplitLocFaceVert();
    bool			addSplitLocPoints();

    
    GU_Detail	       	       &myGdp;

    float			myT;			// parametric t
    float			myT2;			// parametric t2
    int				myPrim;			// prim index
    int				myIndex;		// edge or vertex index
    int				myP0;			// point index
    int				myP1;			// point index

    float			myForcedT;
};

#endif

