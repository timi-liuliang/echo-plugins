/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_BlockEndCompile.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_BlockEndCompile_h__
#define __SOP_BlockEndCompile_h__

#include "SOP_API.h"
#include "SOP_BlockEnd.h"
#include "SOP_Compiled.h"

class SOP_API SOP_BlockEndCompile : public SOP_BlockEnd
{
public:
	     SOP_BlockEndCompile(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_BlockEndCompile();

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    static PRM_Template		*buildTemplates();

    // Should all inputs be treated as enclosed?
    virtual bool		 encloseInputs() const { return true; }

    // Remove all of our cached data, including our compile graph.
    virtual bool		 unloadData();

protected:
    virtual OP_ERROR	 cookMySop(OP_Context &context);

    virtual const char	*inputLabel(unsigned idx) const;

    virtual void		 getNodeSpecificInfoText(OP_Context &context,
					OP_NodeInfoParms &iparms);

    // Fill in tree with this node's details
    virtual void		 fillInfoTreeNodeSpecific(UT_InfoTree &tree, 
					const OP_NodeInfoTreeParms &parms);


    bool		 COMPILE(fpreal t) const
			 { return evalInt("docompile", 0, t); }
    int		 	 UNLOAD(fpreal t) const
			 { return evalInt("unload", 0, t); }

    virtual void	 PRIMARYPATH(UT_String &str, fpreal t) const
			 { evalString(str, "primarypath", 0, t); }

    void		 forceCompile();
    static int		 forceCompile(void *d, int i, fpreal t,
					const PRM_Template *);
private:

    // These should all be bundled into a single structure to represent
    // a compiled chunk of nodes.
    SOP_Compiled	*myCompiled;
    SOP_Compiled::TaskStateArray	myTaskStates;
};

#endif


