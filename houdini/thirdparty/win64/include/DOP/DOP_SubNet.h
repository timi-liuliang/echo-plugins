/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DOP_SubNet.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __DOP_SubNet_h__
#define __DOP_SubNet_h__

#include "DOP_API.h"
#include "DOP_Node.h"

class DOP_API DOP_SubNet : public DOP_Node
{
public:
	     DOP_SubNet(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~DOP_SubNet();

    virtual const char	*inputLabel(unsigned idx) const;

    static OP_Node	*myConstructor(OP_Network *net, const char *name,
				       OP_Operator *entry);
    static PRM_Template	 myTemplateList[];

    virtual void	 getNodeSpecificInfoText(OP_Context &context,
						 OP_NodeInfoParms &iparms);

    /// Our number of visible outputs is always equal to the number of
    /// visible output on the display node inside us.
    virtual unsigned	 getNumVisibleOutputs() const;

    /// To evaluate local variables, we defer to the node that is
    /// currently being processed by the engine.
    virtual bool	 evalVariableValue(
			    fpreal &value, int index, int thread);
    virtual bool	 evalVariableValue(
			    UT_String &value, int index, int thread);

    void		 collectChildErrors();

    static const char	*theChildTableName;

protected:
    /// Cooking a DOP_SubNet calls cook on the child display node.
    virtual OP_ERROR	 cookMe(OP_Context &);

    virtual void	 getRequiredOutputsSubclass(fpreal time,
					int foroutputidx,
					UT_Array<DOP_Output> &outputs,
					const DOP_Engine &engine);
    virtual void	 applyOutputDataSubclass(fpreal time, int outputidx,
					SIM_RootData &rootdata,
					const char *parentdataname,
					DOP_Engine &engine,
					UT_StringArray *datanames);
    virtual void	 getInputInfoSubclass(int inputidx,
					DOP_InOutInfo &info) const;
    virtual void	 getOutputInfoSubclass(int outputidx,
					DOP_InOutInfo &info) const;
};

#endif
