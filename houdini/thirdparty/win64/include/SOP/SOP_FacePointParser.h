/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS: Point parser for the curve SOP
 *
 */

#ifndef __SOP_FacePointParser_h__
#define __SOP_FacePointParser_h__

#include "SOP_API.h"
#include <GOP/GOP_Parser.h>
#include <SYS/SYS_Math.h>
#include "SOP_Curve.h"


// Special Characters recognized by the parser
#define	  CHAR_POINT		'p'
#define   CHAR_POINTCOPY        'P'
#define   CHAR_RELATIVE		'@'
#define   CHAR_BREAKPOINT       'b'
#define   CHAR_BRACKETOPEN	'['
#define   CHAR_BRACKETCLOSE	']'
#define   CHAR_XYZW_DELIM	','
#define   CHAR_UV_DELIM		','

class SOP_API SOP_FacePointParser : public GOP_Parser
{
public:
    SOP_FacePointParser(const GU_Detail *gdp, UT_String &str);
    SOP_FacePointParser(const GU_Detail *gdp, const UT_StringHolder &str);

    virtual ~SOP_FacePointParser() {}

    enum RefType
    {
	RefTypeNone = -1,
	RefTypePoint = 0,
	RefTypeBreakpoint = 1,
	RefTypeRelative = 2
    };

protected:
    // The following methods should be handled by the child classes, depending
    // on how they store the point list.
    virtual void		 handleError();
    virtual void		 resetPointList() = 0;
    virtual void		 addPoint(const UT_Vector4R &v, bool relative,
					bool use_w, const UT_Vector4R &last,
					RefType reftype = RefTypeNone,
					int pid = -1) = 0;
    virtual void		 referencePoint(GA_Offset point) = 0;

    // this is where we know what error occurred, but the actual message error
    // display happens somewhere else; therefore have an integer which keeps
    // track of error messages (negative for generic, index of error message
    // otherwise)
    int				 myErrorIndex;

    const GU_Detail		*myRefGdp; // gdp to use for references

private:
    virtual void		 init();
    virtual void		 finish();
    virtual bool 	 	 dispatch(char c);
    
    // State handlers
    virtual void 		 handleStart();
    void			 handleCollectX();
    void			 handleCollectY();
    void			 handleCollectZ();
    void			 handleCollectW();
    void			 handleCollectP();
    void			 handleCollectBU();
    void			 handleCollectBV();

    // Puts the position of the breakpoint from primitive at index primidx
    // specified by u and v.  (Set v to -1 for curves).
    // Returns true if the u and v and primidx are valid, false otherwise
    bool 			 getPosFromBreakpoint(int u, int v,
					GA_Index primidx,
					UT_Vector4R &pos);

    UT_Vector4R	 		 myLastPos;
    UT_Vector4R	 		 myCurrPos;

    bool	 		 myRelative;
    bool         		 myCopyPoint;
    bool	 		 myAllSpaces;
    bool 	 		 myFirstBracket;
    bool	 		 myOpenBracket;
    bool	 		 myZDone;
    int		 		 myIndex;
    int		 		 myU, myV;
};

#endif
