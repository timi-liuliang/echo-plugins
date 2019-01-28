/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_SubNet.h (Custom Library, C++)
 *
 * COMMENTS:    An object to describe a motion capture handle
 *
 */

#ifndef __OBJ_SubNet__
#define __OBJ_SubNet__

#include "OBJ_API.h"
#include <UT/UT_DMatrix4.h>
#include "OBJ_Node.h"
#include "OBJ_Shared.h"

class OP_Bundle;

enum OBJ_SubNetIndex
{
    I_LABEL1 = I_N_BASE_INDICES,
    I_LABEL2,
    I_LABEL3,
    I_LABEL4,

    I_OUTPUTOBJ,
    I_VISIBLEOBJECTS,

    I_N_SUBNET_INDICES
};

class OBJ_API OBJ_SubNet : public OBJ_Node
{
public:
				 OBJ_SubNet(OP_Network *net,
					   const char *name,
					   OP_Operator *op);
    virtual			~OBJ_SubNet();

    virtual const char		*getChildType() const;
    virtual OBJ_OBJECT_TYPE	 getObjectType() const;

    virtual OP_OpTypeId		 getChildTypeID() const;

    virtual bool		 getBoundingBox(UT_BoundingBox &box,
					OP_Context &ctx);

    virtual OP_Node		*getDisplayNodePtr();
    virtual OP_Node		*getRenderNodePtr();
    virtual int			 isObjectRenderable(fpreal t) const;
    OP_Bundle			*getVisibleObjectBundle(fpreal t);
    int				 isVisibleObject(const OBJ_Node *node,
						 fpreal t) const;

    virtual void		 opChanged(OP_EventType reason, void *data=0);
    virtual void		 referencedParmChanged(int parm_index);

    virtual OP_ERROR		 setInput(unsigned idx, OP_Node *op,
					  unsigned outputIdx = 0);
    virtual OP_ERROR		 setInputReference(unsigned idx,
						   const char *label,
						   int keeppos,
						   unsigned outputIdx = 0);
    virtual OP_ERROR		 setIndirectInput(unsigned idx,
						  OP_IndirectInput *in);
    virtual OP_ERROR		 setNamedInput(
					const OP_ConnectorId& input_name,
					OP_Node *op,
					const OP_ConnectorId* output_name=NULL);
    virtual OP_ERROR		 setNamedInputReference(
					const OP_ConnectorId& input_name, 
					const char *label,
					int,
					const OP_ConnectorId *output_name=NULL);
    virtual OP_ERROR		 setNamedIndirectInput(
					const OP_ConnectorId& input_name,
					OP_IndirectInput *input);

    bool			 getSubnetTransform(OP_Context &context,
						    UT_DMatrix4 &mat);

    void			 LABEL(UT_String &str, int idx);

    void			 OUTPUTOBJ(UT_String &str) const
				 {
				    evalString(str, "outputobj",
					       &getIndirect()[I_OUTPUTOBJ],
					       0, 0.0f);
				 }
    int				 parmIndexOUTPUTOBJ() const
				 {
				     return getCachedParmIndex(
					     "outputobj", I_OUTPUTOBJ);
				 }

    void			 VISIBLEOBJECTS(UT_String &str, fpreal t)
				 {
				    evalString(str, "visibleobjects",
					       &getIndirect()[I_VISIBLEOBJECTS],
					       0, t);
				 }
    void			 setVISIBLEOBJECTS(const UT_String &str,
					    CH_StringMeaning meaning, fpreal t)
				 {
				    setString(str, meaning, "visibleobjects",
					      getIndirect()[I_VISIBLEOBJECTS],
					      0, t);
				 }
    int				 parmIndexVISIBLEOBJECTS() const
				 {
				     return getCachedParmIndex(
					 "visibleobjects", I_VISIBLEOBJECTS);
				 }

    static OP_Node              *myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static const char		*theChildTableName;

    static PRM_Template		*getTemplateList(OBJ_ParmsStyle style);

    virtual OBJ_SubNet		*castToOBJSubNet() { return this; }
    
    virtual void		 getParallelInputs(OP_Context &context,
						   OP_NodeList &nodes) const;
    
    virtual int			 doDrawLit() const { return 1; }

protected:
    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const
				 { return subnetIndirect; }

    virtual OP_ERROR		 cookMyObj(OP_Context &context);
    virtual void		 childFlagChange(OP_Node *);
    virtual const char		*inputLabel(unsigned idx) const;

    virtual void		 buildParmDependency( int parm_index );

    virtual bool		 getTimeOverride();

private:
    bool			 internalDisplayDisabled(fpreal t);
    void			 propagateDisplayChangeToDescendents(
				    bool visible_children_changed);
    void			 checkInternalExtraInputTimeDependency(
							fpreal t);
    void			 updateInternalTimeDependency();

    bool			 myResetInternalTimeDependent;
    bool			 myInternalTimeDependent;
    bool			 myInternalExtraInputTimeDependent;
    int				 myLastDisplayDisabled;

    static int			*subnetIndirect;
    static int			 myLastInternalParmIndex;
};

#endif
