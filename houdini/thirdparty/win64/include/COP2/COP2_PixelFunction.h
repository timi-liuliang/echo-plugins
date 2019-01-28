/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_PixelFunction.h
 *
 * COMMENTS:
 *	Data class which manages pixel function lists between COPs.
 */
#ifndef _COP2_PIXEL_FUNCTION_H_
#define _COP2_PIXEL_FUNCTION_H_

#include "COP2_API.h"
#include <UT/UT_String.h>

class COP2_Node;
class RU_PixelFunction;

class COP2_API COP2_PixelFunction
{
public:
    // Set up with specific function conditions.
    COP2_PixelFunction(COP2_Node	*masknode,
		       int		 maskplane,
		       int		 maskindex,
		       bool		 in_unpremult,
		       float		 effect);
    ~COP2_PixelFunction();

    // returns true if the scope & mask COP are the same.
    bool	 isCompatibleWith(COP2_Node     *masknode,
				  int		 maskplane,
				  int		 maskindex,
				  bool		 inunpremult,
				  float		 effect) const;

    // prepends the function to the function list. Only do if the above
    // function returns true!
    void	 addFunction(RU_PixelFunction	*func,
			     COP2_Node		*fornode,
			     int		 input_index,
			     bool		 mask[4]);

    // Needed for evaluation when all functions are added.
    COP2_Node		*getFirstNode()	const { return myFirstNode; }
    int			 getNodeInput() const { return myInputIndex; }
    RU_PixelFunction	*getFunction()  const { return myFunction; }
    bool		 isInUnpremult() const { return myInUnpremult; }
    
private:
    // Pixel function conditions
    COP2_Node		*myMask;
    int			 myMaskPlane;
    int			 myMaskIndex;

    // Pixel function information
    COP2_Node		*myFirstNode;
    int			 myInputIndex;
    RU_PixelFunction	*myFunction;
    bool		 myInUnpremult;
    float		 myEffect;
};

#endif
