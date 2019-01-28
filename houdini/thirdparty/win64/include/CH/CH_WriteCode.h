/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CH_WriteCode.h ( CH Library, C++)
 *
 * COMMENTS:
 *	Writes the C++ code to implement a channel...
 *	This is designed to be overloaded where necessary to customize
 *	it for specific environments (such as inside a POP)
 */

#ifndef __CH_WriteCode__
#define __CH_WriteCode__

#include "CH_API.h"
#include <SYS/SYS_Types.h>
#include <iosfwd>

class CH_API CH_WriteCode
{
public:
    CH_WriteCode();
    virtual ~CH_WriteCode();

    // os is output stream to write the new function call.  gs is
    // the global stream to put the data.
    // prefix is the file level prefix, a channel specific name
    // will be appended.
    // Force type is one of CH_ChannelBehavior.  -1 doesn't cause
    // any forcing to occur.
    int			writeCode(CH_Channel *chan, std::ostream &os, 
       			          std::ostream &gs,
       			          const UT_String &prefix, 
       			          const UT_String &mainprefix, int isstatic = 1,
       			          int forcetype = -1);
    
    // This function expands expressions:			
    virtual void	expandExpression(UT_String &expr);

    // These mark the start and end of each call back functions expandExpression
    // region.  They both write BEFORE the result of expandExpression
    // is written.
    virtual void	startExpressionBlock(std::ostream &gs);
    virtual void	stopExpressionBlock(std::ostream &gs);

    static void		writeChannelType(std::ostream &os, CH_ChannelBehavior type);

    // Writes a proper C++ fpreal with the "F" qualifier.
    static void		writeFloat(std::ostream &os, fpreal val);

protected:
    // Duplicate from OP_Node :<
    int			myTabSpace;
    void		writeIndent(std::ostream &os, int level);
    void		writeSpaces(std::ostream &os, int spaces);
};

#endif

