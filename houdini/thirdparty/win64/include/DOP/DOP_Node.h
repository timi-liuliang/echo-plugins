/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DOP_Node.h ( DOP Library, C++)
 *
 * COMMENTS:	Dynamics OPs.  These are nodes which are used for setting
 * 		up and controlling dynamic simulations.
 */

#ifndef __DOP_Node__
#define __DOP_Node__

#include "DOP_API.h"
#include "DOP_Error.h"
#include <SIM/SIM_Utils.h>
#include <OP/OP_Network.h>
#include <OP/OP_PreDefRules.h>


class OP_OperatorTable;
class SIM_Data;
class SIM_Object;
class SIM_ObjectArray;
class SIM_RootData;
class SIM_Options;
class SIM_Relationship;
class DOP_InOutInfo;
class DOP_Output;
class DOP_Engine;

/// This declaration allows HDK users to write custom DOP operators.
extern "C" {
    SYS_VISIBILITY_EXPORT extern void       newDopOperator(OP_OperatorTable *table);
};

/// This enumeration contains all the standard DOP local variables.
/// Any DOPs which create their own variables must start their variable
/// indices at NUM_DOP_VARS. They must also call the
/// DOP_Node::evalVariableValue() in their own override of this function.
enum {
    DOP_VAR_SIMTIME,
    DOP_VAR_SIMFRAME,
    DOP_VAR_SIMFRAME1,
    DOP_VAR_SIMFRAME2,
    DOP_VAR_SIMFRAME3,
    DOP_VAR_SIMFRAME4,
    DOP_VAR_SIMFRAME5,
    DOP_VAR_SIMFRAME6,
    DOP_VAR_SIMFRAME7,
    DOP_VAR_SIMFRAME8,
    DOP_VAR_SIMFRAME9,
    DOP_VAR_SFPS,
    DOP_VAR_TIMESTEP,
    DOP_VAR_SIMNOBJ,
    DOP_VAR_OBJ,
    DOP_VAR_NOBJ,
    DOP_VAR_OBJID,
    DOP_VAR_OBJCT,
    DOP_VAR_OBJCF,
    DOP_VAR_DOPNET,
    DOP_VAR_OBJNAME,
    DOP_VAR_ALLOBJNAMES,
    DOP_VAR_ALLOBJIDS,
    DOP_VAR_RELNAME,
    DOP_VAR_RELOBJNAMES,
    DOP_VAR_RELOBJIDS,
    DOP_VAR_RELAFFOBJNAMES,
    DOP_VAR_RELAFFOBJIDS,
    DOP_NUM_DOP_VARS
};

/// This is the base class for all DOP nodes.
class DOP_API DOP_Node : public OP_Network
{
public:
    /// Constructor to allocate a new DOP_Node.
				 DOP_Node(OP_Network *dad, const char *name,
					  OP_Operator *entry);
    /// Destructor.
    virtual 			~DOP_Node();

    /// The standard DOP local variables. These get added to all DOP nodes
    /// automatically by the DOP_Operator constructor.
    static OP_VariablePair	 theVariablePair;
    static CH_LocalVariable	 theVariables[];

    /// This function builds a menu of all the rules in the
    /// DOPActivationRules file.
    static void			 buildActivationRules(void *data,
							PRM_Name* menu,
							int maxSize,
							const PRM_SpareData*,
							const PRM_Parm *);
    /// This function responds to a selection of the adtivation rule menu
    /// by setting the activation parameter.
    static int			 setActivationCallback(void *data,
							int index,
							fpreal t,
							const PRM_Template *);

    /// This function applies SIM_Data from one of our inputs to some
    /// parent data. This is done by calling applyOutputData() for the
    /// DOP_Node that is connected to the specfiied input index.
    void	 applyDataFromInput(fpreal time, int inputidx,
				    int inputidxforsuffix,
				    SIM_RootData &rootdata,
				    const char *parentdataname,
				    DOP_Engine &engine,
				    UT_StringArray *datanames,
				    bool changeuniquenamesuffix);

    /// Returns information about which DOP_Node outputs are required
    /// to process this node. This function calls getRequiredOutputsSubclass()
    /// to do the real work.
    void	 getRequiredOutputs(fpreal time, int foroutputidx,
				    UT_Array<DOP_Output> &outputs,
				    const DOP_Engine &engine);
    /// Divides incoming objects among the various outputs from this node.
    /// This function calls partitionObjectSubclass() to do the real work.
    void	 partitionObjects(fpreal time,
				  const SIM_ObjectArray &objects,
				  const UT_ValArray<SIM_ObjectArray *> &p,
				  const DOP_Engine &engine);
    /// Processes incoming objects by adding, removing, or modifying data.
    /// This function calls processObjectSubclass() to do the real work.
    void	 processObjects(fpreal time, int foroutputidx,
				const SIM_ObjectArray &objects,
				DOP_Engine &engine);
    /// Adds the data corresponding to the specified output to the object.
    /// This function calls applyOutputDataSubclass() to do real
    /// work.
    void	 applyOutputData(fpreal time, int outputidx,
				 SIM_RootData &rootdata,
				 const char *parentdataname,
				 DOP_Engine &engine,
				 UT_StringArray *datanames);
    /// This function gets called on all nodes before a timestep starts.
    /// This function calls preNetworkProcessingSubclass() on this node, and
    /// calls preNetworkProcessing() on all our child DOP_Nodes.
    void	 preNetworkProcessing(DOP_Engine &engine, bool firstframe);
    /// This function gets called on all nodes after a timestep completes.
    /// This function calls postNetworkProcessingSubclass() on this node, and
    /// calls postNetworkProcessing() on all our child DOP_Nodes.
    void	 postNetworkProcessing(DOP_Engine &engine);
    /// Gets information about the specified input.
    /// This function calls getInputInfoSubclass() to do the real work.
    void	 getInputInfo(int inputidx, DOP_InOutInfo &info) const;
    /// Gets information about the specified output.
    /// This function calls getOutputInfoSubclass() to do the real work.
    void	 getOutputInfo(int outputidx, DOP_InOutInfo &info) const;

    /// Adds an error from the DOP error file.
    void		 addError(int code, const char *msg = 0)
			 { UTaddError("DOP", code, msg);}
    /// Adds a message from the DOP error file.
    void		 addMessage(DOP_ErrorCode code, const char *msg = 0)
			 { UTaddMessage("DOP", code, msg);}
    /// Adds a warning from the DOP error file.
    void		 addWarning(DOP_ErrorCode code, const char *msg = 0)
			 { UTaddWarning("DOP", code, msg);}
    /// Adds a fatal error from the DOP error file.
    void		 addFatal(DOP_ErrorCode code, const char *msg = 0)
			 { UTaddFatal("DOP", code, msg);}

    static const char	*theChildTableName;
    /// Returns "DOP" because our child nodes will be DOP_Nodes.
    virtual const char	*getChildType() const;
    /// Returns "DOP" because we are a DOP_Node.
    virtual const char	*getOpType() const;

    /// Returns DOP_OPTYPE_ID because our child nodes will be DOP_Nodes.
    virtual OP_OpTypeId  getChildTypeID() const;
    /// Returns DOP_OPTYPE_ID because we are a DOP_Node.
    virtual OP_OpTypeId  getOpTypeID() const;

    /// Override this method to always return null.
    /// DOP_Nodes are only interested in the display node, not the render
    /// node.
    virtual OP_Node	*getRenderNodePtr();

    /// REturn text describing a particular input.
    virtual const char	*inputLabel(unsigned idx) const;

    /// Override the opChanged method. Whenever a DOP node parameter changes,
    /// we need to notify the simulation engine that it needs to recook the
    /// last timestep to reflect the changed DOP values.
    virtual void	 opChanged(OP_EventType reason, void *data=0);

    /// Handle changes to a parameter that is referenced by one of our
    /// parameters. DOP Nodes want to treat this the same as a parm change.
    virtual void	 referencedParmChanged(int pi);

    /// Handle the evaluation of standard local variables.
    virtual bool	 evalVariableValue(fpreal &v, int index, int thread);
    /// Handle the evaluation of standards string local variables.
    virtual bool	 evalVariableValue(UT_String &v, int index, int thread);

    /// Adds DOP_Operators to teh DOP operator table.
    static void		 buildOperatorTable(OP_OperatorTable &table);
    /// Adds the DOP specific expression functions.
    static void		 initializeExpressions();
    /// Adss Dop specific hscript commands.
    static void		 installCommands();

    /// Return the absolute width of the tile for this node.
    virtual fpreal       getW() const;
    /// Return the absolute height of the tile for this node.
    virtual fpreal       getH() const; 

    /// The display and render ops of a DOP Network are always equal.
    virtual int		 getDandROpsEqual();
    virtual int		 updateDandROpsEqual(int check_inputs = 1);

    // Override clearInterrupted so that we don't invalidate the whole cache
    // when the user interrupts a cook.
    virtual void	 clearInterrupted();

    /// Get the MMB info text for this node.
    virtual void	 getNodeSpecificInfoText(OP_Context &context,
				OP_NodeInfoParms &iparms);
    /// Get the info tree specific to this node.
    virtual void 	 fillInfoTreeNodeSpecific(UT_InfoTree &tree,
				const OP_NodeInfoTreeParms &parms);

    /// Return the DOP_Parent pointer associated with this node.
    /// To cast a DOP_Node to a DOP_Parent, just cast our creator node.
    virtual DOP_Parent		*castToDOPParent();
    /// Return the const DOP_Parent pointer associated with this node.
    /// To cast a DOP_Node to a DOP_Parent, just cast our creator node.
    virtual const DOP_Parent	*castToDOPParent() const;

    /// Return the number of ordered inputs by looking at our DOP_InOutInfo.
    virtual unsigned	 orderedInputs() const;

    /// Override the setInput functions so that we can send notification to
    /// the nodes that are our inputs when we disconnect from them. This lets
    /// then adjust their tile UI if necessary.
    virtual OP_ERROR	 setInput(unsigned idx, OP_Node *op,
				  unsigned outputIdx = 0);
    virtual OP_ERROR	 setInputReference(unsigned idx, const char *label,
					   int keeppos, unsigned outputIdx = 0);

    virtual OP_ERROR	 setNamedInput(const OP_ConnectorId& input_name, OP_Node *op,
	const OP_ConnectorId* output_name = NULL);

    virtual OP_ERROR	 setNamedInputReference(const OP_ConnectorId& input_name, 
	const char *label, int, const OP_ConnectorId* output_name = NULL);

    virtual bool	 hasDifferentMultiInputs() const;

    /// This functions sets the evaluation object index and the current
    /// object pointer that is used for local variable evaluation.
    /// It clears the current relationship information.
    void		 setCurrentObject(int objectindex,
					  int objectcount,
					  const SIM_Object *object);
    /// This functions sets the evaluation relationship index and the current
    /// relationship pointer that is used for local variable evaluation.
    /// It clears the current object information.
    void		 setCurrentRel(const SIM_Relationship *rel);
    /// This function copies the current object information from this
    /// node into another destination node.
    void		 copyCurrentObjectAndRelInto(DOP_Node *dest) const;
    /// Clears the current object and relationship information.
    void		 clearCurrentObjectAndRel();

protected:
    /// Cooking a DOP_Node just performs some basic error checks.
    virtual OP_ERROR	 cookMe(OP_Context &);
    /// Bypassing a DOP_Node does nothing.
    virtual OP_ERROR	 bypassMe(OP_Context &, int &);
    /// Returns the file extension to use for opsave commands on a DOP_Node.
    virtual const char	*getFileExtension(int binary) const;
    /// Handle changes to our child node display and render flags.
    virtual void	 childFlagChange(OP_Node *);
    /// Checks that the input types match the output types connected to them.
    OP_ERROR		 checkInputValidity();

    /// A DOP_Node has no data, so do nothing.
    virtual OP_DataType getCookedDataType() const;
    /// A DOP_Node has no data, so do nothing.
    virtual void	 deleteCookedData();
    /// A DOP_Node has no data, so do nothing.
    virtual int		 saveCookedData(const char *, OP_Context &);
    /// A DOP_Node has no data, so do nothing.
    virtual int		 saveCookedData(std::ostream &, OP_Context &, int);

    /// DOP Nodes always return their parent's transform, making it
    /// easy to do stuff like optransform() from within a dopnetwork.
    virtual bool	 getWorldTransform(UT_DMatrix4 &xform, OP_Context &);
    virtual bool	 getIWorldTransform(UT_DMatrix4 &xform, OP_Context &);
    virtual bool	 getTransform(TransformMode mode, UT_DMatrix4 &xform, OP_Context &);

    /// Returns whether the DOP should do any processing. This function
    /// checks the bypass flag and the "activation" parameter if there is
    /// one. This function should only be called from
    /// getRequiredOutputsSubclass(), partitionObjectsSubclass(),
    /// processObjectsSubclass(), and applyOutputDataSubclass().
    bool		 isActive(fpreal time);

    /// This default implementation indicates that all inputs are required.
    virtual void	 getRequiredOutputsSubclass(fpreal time,
					int foroutputidx,
					UT_Array<DOP_Output> &outputs,
					const DOP_Engine &engine);
    /// This default implementation sends all objects to the first output.
    virtual void	 partitionObjectsSubclass(fpreal time,
					const SIM_ObjectArray &objects,
					const UT_ValArray<SIM_ObjectArray *> &p,
					const DOP_Engine &engine);
    /// This default implementation does not affect incoming objects.
    virtual void	 processObjectsSubclass(fpreal time,
					int foroutputidx,
					const SIM_ObjectArray &objects,
					DOP_Engine &engine);
    /// This default implementation does nothing because DOP_Node doesn't
    /// have any data outputs.
    virtual void	 applyOutputDataSubclass(fpreal time, int outputidx,
					SIM_RootData &rootdata,
					const char *parentdataname,
					DOP_Engine &engine,
					UT_StringArray *datanames);
    /// This function gets called on all nodes before a timestep starts.
    /// The default implementation does nothing.
    virtual void	 preNetworkProcessingSubclass(DOP_Engine &engine, bool firstframe);
    /// This function gets called on all nodes after a timestep completes.
    /// The default implementation does nothing.
    virtual void	 postNetworkProcessingSubclass(DOP_Engine &engine);
    /// This default implementation specifies the input accepts objects.
    virtual void	 getInputInfoSubclass(int inputidx,
					DOP_InOutInfo &info) const;
    /// This default implementation specifies the output sends out objects.
    virtual void	 getOutputInfoSubclass(int outputidx,
					DOP_InOutInfo &info) const;

    /// Prefixes the supplied data name with the root name. This utility
    /// function is often used to concatenate data paths.
    void		 makeFullDataName(const char *root,
					UT_String &name) const;
    /// Prefixes & suffixes the data name to form a aunique name.
    void		 makeUniqueDataName(UT_String &dataname,
				    const char *parentdataname,
				    bool uniquename,
				    bool usenodepath) const;
    /// Appends the name and index number of the node that is applying data.
    /// This is used to generate a unique data name.
    void		 appendUniqueDataNameSuffix(UT_WorkBuffer &dataname,
					bool usenodepath) const;

private:
    class DOP_CookData
    {
	public:
	    UT_WorkBuffer	 myUniqueNamePrefix;
    };

    DOP_CookData	&getCookData() const;
    void		 handleNodeChange(OP_EventType reason, void *data);

    const SIM_ObjectArray	*myCurrentObjects;
    int				 myCurrentObjectIndex;
    int				 myCurrentObjectCount;
    const SIM_Object		*myCurrentObject;
    const SIM_Relationship	*myCurrentRel;
    bool			 myIsBusy;
    mutable UT_StringHolder	 myUnderscorePathCache;
    static UT_String		 theApplyingDataToInput;

    friend class		 DOP_Parent;
};

#endif
