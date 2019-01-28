/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __VOPNET_Node_h__
#define __VOPNET_Node_h__

#include "VOPNET_API.h"
#include "VOPNET_Error.h"
#include <OP/OP_Network.h>
#include <VOP/VOP_CodeGenerator.h>
#include <VOP/VOP_ExportedParmsManager.h>

class UT_String;


class VOPNET_API VOPNET_Node : public OP_Network
{
public:
    static CH_LocalVariable	 myVariableList[];
    static OP_Node		*myConstructor(OP_Network* net,
					       const char* name,
					       OP_Operator* entry);

    virtual OP_OperatorFilter	*getOperatorFilter();
    virtual VOP_CodeGenerator	*getVopCodeGenerator();
    virtual bool		 hasVexShaderParameter(const char *parm_name);

    virtual const char	*getChildType() const;
    virtual const char  *getOpType() const;

    static const char	*theChildTableName;
    virtual OP_OpTypeId  getChildTypeID() const;
    virtual OP_OpTypeId  getOpTypeID() const;

    virtual void	 opChanged(OP_EventType reason, void *data=0);

    // Local variables for compiler option evaluation
    virtual bool	 evalVariableValue(UT_String &v,int index, int thread);
    virtual bool	 evalVariableValue(fpreal &v,int i, int thr)
			 { return OP_Network::evalVariableValue(v, i, thr); }

    virtual const char	*getInternalOpTable() const;
    const char		*getInternalIndexFile() const;
    bool		 getShowOpInTabMenuFlag();

    virtual OP_Operator	*getInternalOperator() const;
    void		 setInternalOperator(OP_Operator *op);
    const UT_String	&getOperatorName() const;

    // Returns true if there are any instances of any operator types
    // defined by us or any of our child nodes.
    virtual bool	 canDestroyNode();

    // VOPNET's have no outputs
    virtual unsigned	 maxOutputs() const { return 0; }
    // Absolute width
    virtual fpreal	 getW() const;
    // Absolute height
    virtual fpreal	 getH() const;

    // Handle the old parm order parameter by storing it, so it can later
    // be applied to our output node.
    virtual void	 resolveObsoleteParms(PRM_ParmList *obsolete_parms);

    // Get the management ops associated with vopnets...
    static OP_Operator	*getManagementOperator();
    static OP_Operator  *getSOPManagementOperator();
    static OP_Operator	*getPOPManagementOperator();
    static OP_Operator	*getCHOPManagementOperator();
    static OP_Operator	*getCOP2FilterManagementOperator();
    static OP_Operator	*getCOP2GenManagementOperator();
    static void		 addSHOPManagementOperators(OP_OperatorTable *table);

    static void		 buildOperatorTable (OP_OperatorTable &table);

protected:
	     VOPNET_Node(OP_Network* parent, const char* name, 
			 OP_Operator* op);
    virtual ~VOPNET_Node(void);

    virtual void	 getNodeSpecificInfoText(OP_Context &context,
				OP_NodeInfoParms &iparms);
    virtual void	 finishedLoadingNetwork(bool is_child_call=false);
    virtual void	 addNode(OP_Node *node, int notify=1, int explicitly=1);

    virtual OP_ERROR	 cookMe (OP_Context &context);
    virtual OP_ERROR	 bypassMe(OP_Context &context, int &copied_input);

    virtual OP_DataType	 getCookedDataType (void) const;
    virtual void*	 getCookedData (OP_Context &);
    virtual void	 deleteCookedData (void);
    virtual int		 saveCookedData (const char *, OP_Context &);
    virtual int		 saveCookedData (std::ostream &, OP_Context &, 
               		                 int bin = 0);

    virtual const char	*getFileExtension (int binary) const;

    void		 addError(VOPNET_ErrorCodes code, const char *msg=0)
			 { UTaddError("VOPNET", code, msg); }
    void		 addMessage(VOPNET_ErrorCodes code, const char *msg=0)
			 { UTaddMessage("VOPNET", code, msg); }
    void		 addWarning(VOPNET_ErrorCodes code, const char *msg=0)
			 { UTaddWarning("VOPNET", code, msg); }
    void		 addFatal(VOPNET_ErrorCodes code, const char *msg=0)
			 { UTaddFatal("VOPNET", code, msg); }

    virtual void	 ensureSpareParmsAreUpdatedSubclass()
			 {
			    if (getVopCodeGenerator()
				&& eventMicroNode(OP_SPAREPARM_MODIFIED)
				    .requiresUpdate(0.0))
			    {
				getVopCodeGenerator()
				    ->exportedParmsManager()
				    ->updateOwnerSpareParmLayout();
			    }
			 }

    OP_Operator		*myInternalOperator;
    VOP_CodeGenerator	*myCodeGenerator;
    UT_String		 myObsoleteParmOrder;
};

#endif
