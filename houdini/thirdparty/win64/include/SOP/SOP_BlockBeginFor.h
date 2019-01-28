/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_BlockBeginFor.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_BlockBeginFor__
#define __SOP_BlockBeginFor__

#include "SOP_API.h"
#include "SOP_BlockBegin.h"

class SOP_API SOP_BlockBeginFor : public SOP_BlockBegin
{
public:
	     SOP_BlockBeginFor(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_BlockBeginFor();

    virtual bool		 updateParmsFlags();

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    static PRM_Template		 myTemplateList[];

    int			 NEXTMETHOD(fpreal t) const
			 { return evalInt("method", 0, t); }
    virtual void	 FORPATH(UT_String &str, fpreal t) const
			 { evalString(str, "blockpath", 0, t); }

    static int			 buildMetaBlockStatic(void *op, int idx, 
					fpreal t, const PRM_Template *);
    static int			 resetCookPassStatic(void *op, int idx, fpreal t,
					const PRM_Template *);
    void			 resetCookPass(fpreal t);

protected:
    virtual OP_ERROR	 cookMySop(OP_Context &context);

    virtual const char	*inputLabel(unsigned idx) const;
    virtual void	 getDescriptiveName(UT_String &name) const;

    // Id of the forsop we were last registered to so we can
    // unregister.
    int			 myLastForSOP;

private:
};


#endif

