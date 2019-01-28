/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_BlockBegin.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_BlockBegin__
#define __SOP_BlockBegin__

#include "SOP_API.h"
#include "SOP_Node.h"

class SOP_API SOP_BlockBegin : public SOP_Node
{
public:
	     SOP_BlockBegin(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_BlockBegin();

    /// Returns true if end is what our FORPATH points to.
    bool		 isPaired(const OP_Node *end) const;

    virtual void	 FORPATH(UT_String &str, fpreal t) const = 0;

    // We enforce that blocked pairs have the same color
    virtual bool	    	 setColor(const UT_Color &col);

protected:
private:
};


#endif
