/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_BlendShapes.h (C++)
 *
 * COMMENTS:	dynamic blend SOP
 *
 */


#ifndef __SOP_BlendShapes_h__
#define __SOP_BlendShapes_h__

#include "SOP_API.h"
#include "SOP_Node.h"

class SOP_API SOP_BlendShapes : public SOP_Node
{
public:
    SOP_BlendShapes(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_BlendShapes();

    virtual OP_ERROR	 cookInputGroups(OP_Context &context,
				int alone = 0) override;

    static OP_Node	*myConstructor(OP_Network *net,
				const char *name, OP_Operator *entry);
    static PRM_Template *buildTemplates();
    virtual bool	 updateParmsFlags() override;

    virtual const SOP_NodeVerb* cookVerb() const override;

    void	setSelectionOverride(bool selection_override);
    void	setBlendValue(int i, float value, fpreal t);
    void	setBlendAlias(int i, const char *alias);
    void	removeBlend(int i);

protected:
    virtual bool		 load(UT_IStream &is, const char *extension,
				      const char *path=0) override;
    virtual OP_ERROR		 cookMySop(OP_Context &context) override;
    virtual const char		*inputLabel(unsigned) const override;

    OP_ERROR			 lockMyInputs(OP_Context &context);
    void			 unlockMyInputs();

private:

    static int	updateChannelNames(void *data, int index, fpreal t,
				   const PRM_Template * );
};

#endif

