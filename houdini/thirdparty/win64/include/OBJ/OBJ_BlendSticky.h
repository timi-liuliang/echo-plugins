/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        OBJ_BlendSticky.h (Custom Library, C++)
 *
 * COMMENTS:    A sticky object that blends its UV transform from other objects.
 *
 */

#ifndef __OBJ_BlendSticky__
#define __OBJ_BlendSticky__

#include "OBJ_API.h"
#include "OBJ_StickyShared.h"

enum OBJ_StickyBlendIndex
{
    I_STICKYBLENDWEIGHTS = I_N_STICKYSHARED_INDICES,

    I_N_STICKYBLEND_INDICES
};

class OBJ_API OBJ_BlendSticky : public OBJ_StickyShared
{
public:
			    OBJ_BlendSticky(OP_Network *net, const char *name, 
				    OP_Operator *op);

			   ~OBJ_BlendSticky() {}

    static OP_Node	   *myConstructor(OP_Network *net, 
				    const char *name,
				    OP_Operator *entry);

    static PRM_Template	   *getTemplateList();

    virtual OP_ERROR	    setInput(unsigned idx, OP_Node *op,
				     unsigned output_idx = 0);
    virtual OP_ERROR	    setInputReference(unsigned idx, const char *label,
					      int keeppos,
					      unsigned output_idx = 0);
    virtual OP_ERROR	    setIndirectInput(unsigned idx,
					     OP_IndirectInput *in);

    virtual OP_ERROR	 setNamedInput(const OP_ConnectorId& input_name, OP_Node *op,
	const OP_ConnectorId* output_name = NULL);
    virtual OP_ERROR	 setNamedIndirectInput(const OP_ConnectorId& input_name,
	OP_IndirectInput *input);
    virtual OP_ERROR	 setNamedInputReference(const OP_ConnectorId& input_name, 
	const char *label, int, const OP_ConnectorId* output_name = NULL);

    virtual bool	    get2DWorldTransform(UT_DMatrix4 &mat,
						OP_Context &context);

    virtual void	    STICKYGEOPATH(UT_String &str, fpreal t);
    virtual void	    STICKYUV(UT_Vector2R &uv, fpreal t);
    virtual fpreal	    STICKYROT(fpreal t); 
    int			    NBLENDS()
			    { 
				return evalInt("weights", 0, 0); 
			    }
    void		    setNBLENDS(int n)
			    {
				setInt("weights", 0, 0, n);
			    }
    fpreal		    BLENDWEIGHT(int i, fpreal t) const
			    {
				if( nInputs() <= i )
				    return 0;
				else
				    return evalFloatInst("blend#", &i, 0, t);
			    }
    int			    BLENDMASK(int i)
			    {
				if( nInputs() <= i  )
				    return 0;
				else
				    return evalIntInst("mask#", &i, 0, 0);
			    }

    virtual void	    getParallelInputs(OP_Context &context,
					      OP_NodeList &nodes) const;

protected:
    virtual int		   *getIndirect() const
			    { return stickyIndirect; }
    OP_ERROR		    cookMyObj(OP_Context &context);

    bool		    verifyParentGeopaths();
    void		    verifyNumInputs();
    virtual void	    createInputDependencies(fpreal t);
    virtual OP_ERROR	    lockStickyInputs(OP_Context& context);
    virtual void	    unlockStickyInputs(OP_Context &context);

    static int		   *stickyIndirect;

};

#endif

