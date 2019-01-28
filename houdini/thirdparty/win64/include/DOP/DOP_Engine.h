/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DOP_Engine.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __DOP_Engine_h__
#define __DOP_Engine_h__

#include "DOP_API.h"
#include "DOP_Output.h"
#include <SIM/SIM_Engine.h>
#include <DEP/DEP_MicroNode.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_ErrorManager.h>


class OP_Network;
class SIM_ObjectArray;
class DOP_Node;
class DOP_Output;
class DOP_OutputDependency;

typedef UT_ValArray<DOP_OutputDependency *>	     DOP_OutputArray;
typedef UT_Map<DOP_Output, DOP_OutputDependency *>   DOP_OutputMap;
typedef DOP_OutputMap::const_iterator		     DOP_OutputMapConstIter;

/// This subclass of SIM_Engine is the one used to contain simulations
/// controlled by DOP_Node networks. It serves as the glue between the
/// pure simulation library and the DOP_Node interface given to simulations
/// in Houdini.
class DOP_API DOP_Engine : public SIM_Engine
{
public:
    /// Constructor is given a pointer to the network that owns the simulation.
	     DOP_Engine(OP_Node *owner);
    /// Destructor destroys all data for the simulation.
    virtual ~DOP_Engine();

    /// Groups objects passed to a DOP_Node by the input they arrived in.
    /// Each entry in the returned UT_ValArray represents an input to
    /// the DOP_Node. If a particular input is not connected, the
    /// corresponding entry in the array will by null. Generally this
    /// function will be called from DOP_Node::processObjects() by nodes
    /// which need to have objects coming through one input interact in
    /// some way with objects coming through another input.
    void			 getObjectsAtInputs(DOP_Node *node,
					UT_ValArray<const SIM_ObjectArray *> &
					objectsatinputs) const;

    /// Gets the objects in the stream associated with the supplied node.
    /// For nodes with multiple outputs, the result is the combination of all
    /// objects on any output. This function does not do any cooking of the
    /// networks, or call partitionObjects on any nodes. It only pulls
    /// existing data from our set of outputs to process. This function
    /// is for use by expression functions that want to know what objects
    /// most recently passed through a particular node.
    void			 getObjectsForNode(DOP_Node *node,
					SIM_ConstObjectArray &objects) const;

    /// Adds the specified root data (cast to an object) as a sort of
    /// "extra object" processed by a DOP node. These extra objects don't
    /// affect cooking at all, but show up in the list of objects processed
    /// by that node (in the MMB info and dopnodeobjs expression).
    void			 addObjectForNode(DOP_Node *node,
					SIM_RootData &rootdata);

    /// Gets a pointer to the node that is currently being processed by
    /// the DOP_Engine. The return value will be null if the engine is not
    /// currently in the preSimulationStep and inside a call to
    /// DOP_Node::processObjects(). This function is used by subnet DOPs
    /// to evaluate standard DOP local variables in the context of the node
    /// that is being processed (since subnets have no context of their own).
    DOP_Node			*getDopNodeBeingProcessed() const;

    /// Explicitly dirty dependents of this simulation
    void			 propagateSimDirty(bool send_event);

    /// Pass through to our parent's notification
    void			 notifySimulationChangeSinksOfChange();

    /// Returns whether resimulations is globally disabled.
    static bool                  getWantResimulation()
                                 { return theWantResimulation; }

    /// Globally disables resimulations caused by external node changes. This
    /// is meant for internal use.
    static void                  setWantResimulation(bool want_resimulation)
                                 { theWantResimulation = want_resimulation; }

protected:
    /// Overrides the base class method to make sure that all our internal
    /// data related to the last timestep cook has been cleared away.
    virtual void	 resetSimulation();
    /// Overrides the actions that occur before doing a simulation step.
    /// This is where the network of DOP_Nodes is parsed. Each DOP_Node
    /// has a chance to alter the simulation objects in the
    /// DOP_Node::processObjects() function. Nodes can also specify
    /// which objects are sent to each output of the node.
    virtual void	 preSimulationStep();
    /// Overrides the actions that occur after a simulation step.
    virtual void	 postSimulationStep();
    /// When the most recent simulation state is being deleted or
    /// removed from memory, we have to clear all our references to
    /// it. This basically means our lists of output objects to process.
    virtual void	 clearReferencesToMostRecentState();
    /// Sets up custom data on a new object.
    /// This implementation calls the base class implementation, then it
    /// stores information about the DOP_Node that created the object.
    virtual void	 objectCreated(SIM_Object *object);
    /// Overrides the default behavior when removing a simulation object.
    /// This implementation eliminates all references to this object stored
    /// in our internal data then calls the base class implementation.
    virtual void	 objectRemoved(SIM_Object *object);
    /// Override this method to flag ourselves as requiring a resimulation
    /// when any of our external referenced nodes changes in a significant
    /// way.
    virtual void	 handleExternalNodeChangeSubclass(
				OP_Node *changednode,
				OP_EventType eventtype,
				void *data,
				bool isguide);
    /// Uses the object and data arguments to put the error message on an
    /// appropriate DOP_Node. First priority is given to the node that
    /// created the data, then the node that created the object, then the
    /// node with the display flag.
    virtual void	 addErrorSubclass(const SIM_RootData *rootdata,
				const SIM_Data *data,
				int errorcode,
				const char *errorparm,
				UT_ErrorSeverity severity) const;
    /// Returns the simulation time that corresponds to the given global time.
    virtual const SIM_Time	 getEngineTimeSubclass(const SIM_Time &t) const;
    /// Returns the global time that corresponds to the given simulation time.
    virtual const SIM_Time	 getGlobalTimeSubclass(const SIM_Time &t) const;

    /// Alerts our owner that we are simulating due to an internally
    /// generated need.  Returns the previous state
    virtual bool		 setIsSimulating(bool issimulating) const;

private:
    void	 buildObjectListForOutput(DOP_OutputDependency &output) const;
    void	 partitionObjectsForOutput(DOP_OutputDependency &output) const;
    void	 createOutputsToProcess(DOP_Node *node, int outputidx,
					const SIM_Time &optime);
    void	 clearOutputsToProcess();
    void	 assignObjectsToCreatorOutputs();
    void	 removeAllUnusedObjects(const SIM_ObjectArray &usedobjs);

    UT_ErrorSeverity			 myOwnerSeverity;
    DOP_OutputArray			 myOutputsToProcess;
    DOP_OutputMap			 myOutputsSymbolTable;
    int					 myProcessingOutput;

    static bool                         theWantResimulation;
};

#endif
