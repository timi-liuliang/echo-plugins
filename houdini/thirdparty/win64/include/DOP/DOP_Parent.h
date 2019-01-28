/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DOP_Parent.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __DOP_Parent_h__
#define __DOP_Parent_h__

#include "DOP_API.h"
#include "DOP_FullPathData.h"
#include "DOP_Node.h"

#include <SIM/SIM_Utils.h>
#include <UT/UT_ValArray.h>
#include <OP/OP_DopParent.h>

class DEP_MicroNode;
class DOP_Parent;

typedef enum {
    DOP_PARENT_CREATED,
    DOP_PARENT_DELETED,
    DOP_PARENT_REORDERED
} DOP_ParentEventType;

typedef UT_ValArray<DOP_Parent *>		 DOP_ParentList;

class DOP_API DOP_Parent : public OP_DopParent
{
public:
	     DOP_Parent();
    virtual ~DOP_Parent();

    const DOP_Engine		&getEngine() const;
    DOP_Engine			&getNonConstEngine();

    int				 dopparentUniqueId() const { return myDopParentUniqueId; }

    void		 	 setNeedsToResimulateLastTimestep();
    void			 setNeedsToFilterData();
    virtual void		 setNeedsToCompletelyReset();

    /// Sets the current simulation time, solving all intermediate steps.
    /// Note that the time t passed in to this function is a global
    /// Houdini time, and may not match the simulation time.
    void			 setDOPTime(const SIM_Time &t);
    /// Returns the simulation time that corresponds to a given global
    /// Houdini time.
    const SIM_Time		 getDOPTime(const SIM_Time &t) const;
    /// Returns the global Houdini time that corresponds to a given
    /// simulation time.
    const SIM_Time		 getGlobalTime(const SIM_Time &t) const;
    void			 resimulate();
    /// Resets the simulation and cooks to a specific DOP time, this
    /// avoids double cooking if you are resetting to frame 0.
    void			 resimulateToTime(const SIM_Time &t);

    /// Returns true if the DOP network has changed such that the
    /// simulation needs to be reset.
    bool			 getNeedsResimulation() const;

    /// Attributes for controlling the simulation behavior.
    const SIM_Time		&getOffsetTime() const;
    void			 setOffsetTime(const SIM_Time &t);
    fpreal			 getTimeScale() const;
    void			 setTimeScale(fpreal scale);
    bool			 getAutoResimulation() const;
    void			 setAutoResimulation(bool autoreset);
    bool			 getInterpolateData(const SIM_Time &t) const;
    void			 setInterpolateData(bool interpolate);
    bool			 getIsSimulating() const;
    virtual bool		 getIsTimeless() const { return false; }

    /// Used by SIM_Engine to flag this privately if it is during
    /// an internally generated simulation pass
    void			 setIsSimulating(bool val) { mySimulating = val; }

    /// Used externally to flag that we do not want to resimulate
    /// due to our setDOPTime() invocations.
    bool			 getDisableSimulation() const { return myDisableSimulation; }
    void			 setDisableSimulation(bool disable) { myDisableSimulation = disable; }

    /// Get all objects that match a particular string. This calls the
    /// SIM_Engine::findAllObjectsFromString() function, but converts the
    /// time value from global time to simulation time, and passes our
    /// getInterpolateData() value as the interpolate parameter.
    void			 findAllObjectsFromString(const char *objspec,
					SIM_ConstObjectArray &objects,
					const SIM_Time &t) const;
    /// Get the object that matches a particular string. This calls the
    /// SIM_Engine::findObjectFromString() function, but converts the
    /// time value from global time to simulation time, and passes our
    /// getInterpolateData() value as the interpolate parameter.
    const SIM_Object		*findObjectFromString(const char *objspec,
					int whichmatch, int *nummatch,
					const SIM_Time &t) const;
    /// Get all relationships that match a particular string. This calls the
    /// SIM_Engine::findAllRelationshipsFromString() function, but converts the
    /// time value from global time to simulation time, and passes our
    /// getInterpolateData() value as the interpolate parameter.
    void			 findAllRelationshipsFromString(
					const char *relspec,
					SIM_ConstDataArray &relationships,
					const SIM_Time &t) const;
    /// Get the relationship that matches a particular string. This calls the
    /// SIM_Engine::findRelationshipFromString() function, but converts the
    /// time value from global time to simulation time, and passes our
    /// getInterpolateData() value as the interpolate parameter.
    const SIM_Relationship	*findRelationshipFromString(
					const char *relspec,
					int whichmatch, int *nummatch,
					const SIM_Time &t) const;

    /// Returns data used by DOP_Nodes during their cook process. This is
    /// information that is global to the DOP Network as it cooks.
    DOP_Node::DOP_CookData	&getDopNodeCookData();

    /// Sends out a parm changed message so that anyone interested in the
    /// simulation will know to refresh their data. This call also protects
    /// the DOP Network from causing infinite recursion when the DOP Network
    /// is interested in a SOP node which is interested in the DOP Network
    /// (a common and legal situation when using a SOP Solver, for example).
    void			 sendResimulateNotification(
					bool callnotify,
					bool changingguideparm);

    DEP_MicroNode &		 simMicroNode()
	{ return getNonConstEngineSubclass().simMicroNode(); }
    DEP_MicroNode &		 simInputMicroNode()
	{ return getNonConstEngineSubclass().simInputMicroNode(); }
    DEP_MicroNode &		 resimParmsMicroNode()
	{ return getNonConstEngineSubclass().resimParmsMicroNode(); }
    const DEP_MicroNode &		 resimParmsMicroNode() const
	{ return getEngineSubclass().resimParmsMicroNode(); }
    DEP_MicroNode &		 refilterParmsMicroNode()
	{ return getNonConstEngineSubclass().refilterParmsMicroNode(); }
    const DEP_MicroNode &		 refilterParmsMicroNode() const
	{ return getEngineSubclass().refilterParmsMicroNode(); }

    /// This static function returns an array of all DOP_Parents.
    static const DOP_ParentList	&getAllDopParents();
    /// This function moves a particular DOP_Parent to the front of the list
    /// of all DOP_Parents. This lets us treat this list as a "most recently
    /// used" stack.
    static void			 setMostRecentDopParent(DOP_Parent *dopparent);

    /// The current dopnet may not be the top of the recently used.
    /// Dopnets that are inside locked assets, for example, are rather
    /// useless as destinations.
    /// Except we do want access to those for tracking cache settings,
    /// reset flags, etc.
    static OP_Node		*getMostRecentDopOwner();
    static OP_Node		*getMostRecentWriteableDopOwner();

    virtual bool		 opGetNeedsResimulation() const
    { return getNeedsResimulation(); }
    virtual bool		 opGetDisableSimulation() const;
    /// Query cache size.
    virtual void		 opGetDiskCache(UT_IntArray &frames) const;
    virtual void		 opGetMemoryCache(UT_IntArray &frames) const;

    /// Update re-simulation parameters, ensuring getOffsetTime and the
    /// ilk are valid.  This is required before getDOPTime will work
    /// as expected (but note setDOPTime() implicitly invokes this)
    void			 updateResimParms(const SIM_Time &t);

protected:
    const SIM_Time		&getLastSetTime() const;
    void			 resimulateLastTimestep();
    /// Sets a flag that says we need to re-filter our simulation data the
    /// next time a request for the data is made.
    void			 setNeedsToFilterData(const SIM_Time &t);
    /// Check if we need to filter our simulation data, and do so if required.
    void			 filterDataIfRequired() const;
    /// This function calls filterDataSubclass() to cache pointers to the
    /// simulation data that is of interest to us.
    void			 filterData(const SIM_Time &t);

    virtual const DOP_Engine	&getEngineSubclass() const = 0;
    virtual void		 resimulateLastTimestepSubclass();
    virtual void		 setDOPTimeSubclass(const SIM_Time &t);
    virtual DOP_Engine		&getNonConstEngineSubclass() = 0;
    virtual void		 filterDataSubclass(const SIM_Time &t) = 0;

    virtual void		 handleRefilterParms(const SIM_Time &t) {}
    virtual void		 handleResimParms(const SIM_Time &t) {}

    void			 notifySimulationChangeSinksOfChange();
    void			 notifySimulationChangeSinksOfDelete();

private:
    UT_ValArray<DOP_SimulationChangeSink *>	 mySimulationChangeSinks;
    DOP_Node::DOP_CookData	 myDopNodeCookData;
    SIM_Time			 myLastSetTime;
    SIM_Time			 myOffsetTime;
    SIM_Time			 myNeedsToFilterDataTime;
    fpreal			 myTimeScale;
    bool			 myNeedsToFilterData;
    bool			 mySimulating;
    bool			 myNeedsToResimulateFrame;
    bool			 myNeedsToResimulateCompletely;
    bool			 myAutoResimulation;
    bool			 myInterpolateData;
    bool			 mySendingResimulateNotification;
    bool			 myDisableSimulation;
    int				 myDopParentUniqueId;

    static DOP_ParentList	 theDopParents;

    friend class		 DOP_SimulationChangeSink;
    friend class		 DOP_Engine;
};

class DOP_API DOP_SimulationChangeSink
{
public:
		 DOP_SimulationChangeSink()
		 { }
     virtual	~DOP_SimulationChangeSink()
		 { removeAllSimulationChangeSinks(); }

     virtual void simulationChanged(DOP_Parent *dopparent) = 0;
     virtual void simulationDeleted(DOP_Parent *dopparent)
		  { removeSimulationChangeSink(dopparent); }

protected:
     void	 addSimulationChangeSink(DOP_Parent *dopparent)
		 {
		     if( !dopparent ) return;
		     dopparent->mySimulationChangeSinks.append(this, 1);
		     myDopParents.append(dopparent, 1);
		 }
     void	 removeSimulationChangeSink(DOP_Parent *dopparent)
		 {
		     if( !dopparent ) return;
		     dopparent->mySimulationChangeSinks.findAndRemove(this);
		     myDopParents.findAndRemove(dopparent);
		 }
     void	 removeAllSimulationChangeSinks()
		 {
		     for( int i = myDopParents.entries(); i --> 0; )
			 removeSimulationChangeSink(myDopParents(i));
		 }

private:
     UT_ValArray<DOP_Parent *>	 myDopParents;
};

#endif
