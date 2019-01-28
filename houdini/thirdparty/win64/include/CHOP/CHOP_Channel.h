/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CHOP_Channel.h (CHOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __CHOP_CHANNEL_H__
#define __CHOP_CHANNEL_H__

#include "CHOP_API.h"
#include "CHOP_Node.h"
#include <CH/CH_Types.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_SymbolTable.h>
#include <SYS/SYS_Types.h>


class OP_Channels;
class PRM_Parm;
class UT_String;


class CHOP_API CHOP_Channel : public CHOP_Node
{
public:
    static OP_Node *	    myConstructor(OP_Network*, const char *,
					  OP_Operator *);
    static OP_TemplatePair  myTemplatePair;
    static OP_VariablePair  myVariablePair;
    static PRM_Template	    myTemplateList[];
    static CH_LocalVariable myVariableList[];


    virtual OP_ERROR	    cookMyChop(OP_Context &context);

    virtual bool	    updateParmsFlags();

    virtual void	    cookMyHandles(OP_Context &context);
    virtual fpreal	    handleChanged(CHOP_Handle *, CHOP_HandleData *);

    virtual bool	    findParmFromTrack(const OP_FollowChanRefsOptions& opt,
					      const char *trackname,
					      OP_NodeParmRefCycle &cycle_detect,
					      OP_Node *&node, PRM_Parm *&parm,
					      int &vecidx);

    /// Clear the number channel instances that we have
    void		    clearChannelInstances();

    /// Obtain the number channel instances
    int			    getNumChannelInstances();

    /// Append a new channel instance, returning instance index
    int			    appendChannelInstance();

    /// Organize the given list of channel refs so that we list them by parms
    /// and associated vector indices. Useful for getting a list parms for
    /// appendParmCopy()
    static void		    organizeChanRefs(
					const CH_ChannelRefList &chan_refs,
					OP_NodeList &nodes,
					UT_ValArray<PRM_Parm *> &parms,
					UT_IntArray &used_fields);

    /// Append a new channel instance from the given parm.
    /// copy_field is a bitfield where bit i is true if we want to copy it.
    /// The contents of src_parm is only copied if copy_value is true. In that
    /// case, entire channels are copied if they exist unless
    /// current_value_only is true. If current_value_only is true, then only
    /// the values at the given time are copied.
    /// Returns true on success. Created parms are added to new_refs.
    bool		    appendParmCopy(fpreal t, const PRM_Parm *src_parm,
					   unsigned copy_field,
					   CH_ChannelRefList &new_refs,
					   bool copy_value = true,
					   bool current_value_only = false,
					   bool lock_copy = false,
					   bool set_value_to_default = false);

    /// Remove instances matching the given channel refs. Useful for removing
    /// conflicts.
    void		    removeChannelRefs(fpreal t,
					      const CH_ChannelRefList &refs);

    /// Synchronize our value parameters to the given list of source
    /// parameters. Returns true if changes were done.
    bool		    synchSourceParms(
					fpreal t, bool force, bool synch_keys,
					const UT_StringArray &parm_paths,
					const UT_ValArray<PRM_Parm *> &parms,
					const UT_IntArray &fields);

    int			    getNumPoseKeys(fpreal t);
    bool		    addPoseKey(fpreal t, bool only_synch=false);
    bool		    copyPoseKeyToSource(fpreal t, unsigned key_idx);
    bool		    removePoseKey(fpreal t, unsigned key_idx);
    bool		    updatePoseKey(fpreal t, unsigned key_idx);

    /// Returns the list of source channel refs that we think we were created
    /// from.
    void		    getSourceChannelRefs(fpreal t,
						 CH_ChannelRefList &refs);

    /// Returns the list of source channel refs that we think we were created
    /// from.
    bool		    getSourceChannelRef(fpreal t, int track_idx,
						 CH_ChannelRef &ref);

    /// Given a list of parm paths, verifies if it matches our track export
    /// paths exactly.
    bool		    matchParmPaths(fpreal t,
					   const UT_StringArray &parm_paths);

    /// Given a list of channel refs, returns a list of possible conflicts
    void		    findConflicts(fpreal t,
					  const CH_ChannelRefList &refs,
					  CH_ChannelRefList &conflicts);

    void		    setRangeToValueAnimation(fpreal t);


    // Given a Channel Index, return the value of the matching size parm
    int			    getChannelSize(int parm_index);

    void		    getTrackParmMappings(fpreal t,
						 UT_IntArray &instances,
						 UT_IntArray &components,
						 UT_IntArray *parmindices=NULL);

    virtual bool resolveExport( const PRM_Parm* p, int subindex, CH_ChannelRef& out_export );

protected:
			    CHOP_Channel(OP_Network  *net, const char *name,
					 OP_Operator *op);
    virtual		    ~CHOP_Channel();

    virtual int		    usesScope() const	{ return false; }
    virtual int		    usesUnits();

    virtual bool	    evalVariableValue(fpreal &val, int index, 
					      int thread);
    virtual bool	    evalVariableValue(UT_String &v, int i, int thr)
			    {
				return CHOP_Node::evalVariableValue(v, i, thr);
			    }

    virtual void	    buildParmDependency(int parm_index);
    virtual void	    handleOpDependency(
				int referenced_op_id,
				const OP_Dependency &op_dep,
				OP_InterestType interest_type,
				bool &need_cook,
				const char *old_fullpath,
				const char *old_cwd,
				const char *old_chan_name);

    virtual void	 checkTimeDependencies(	int do_parms=1,
						int do_inputs=1,
						int do_extras=1 );
private:
    bool		    initializeClip(fpreal t,
					   fpreal start,
					   fpreal end,
					   bool key_per_sample,
					   UT_IntArray &instances,
					   UT_IntArray &components);

    void		    getInterval(fpreal t, fpreal &start, fpreal &end);
    
    OP_Node *		    getExportParent(UT_String *export_path,
					    bool add_trailing_slash);

    bool		    getTrackSource(OP_Node *export_parent,
					   CL_Track *track,
					   OP_Node *&node,
					   UT_String &channel_name);
    bool		    getTrackSourceParm(OP_Node *export_parent,
					       CL_Track *track,
					       PRM_Parm *&parm,
					       int &vi,
					       OP_Node **source = NULL);
    bool		    getTrackSourceRef(OP_Node *export_parent,
					      CL_Track *track,
					      CH_ChannelRef &new_ref);

    void		    getTrackPrefix(const PRM_Parm *src_parm,
					   UT_String &track_prefix);

    int			    copyToExport(fpreal t);
    static int		    copyToExportCallback(void *data, int, fpreal t,
						 const PRM_Template *);

    int			    copyFromExport(fpreal t);
    static int		    copyFromExportCallback(void *data, int, fpreal t,
	const PRM_Template *);

    int			    updateAliases(fpreal t);
    static int		    updateAliasesCallback(void *data, int, fpreal t,
						  const PRM_Template *);

    bool		    getChannelCopies(fpreal t,
					     const UT_StringArray &parm_paths,
					     CH_ChannelList &channels,
					     OP_Channels &collection);
    void		    restoreChannelCopies(fpreal t,
						 CH_ChannelList &channels);

    bool		    isChannelNameParm(int parm_index);

    int			    resetAllChannels(fpreal t);
    static int		    resetAllChannelsCallback(void *data, int, fpreal t,
						 const PRM_Template *);

    int			    sortChannels(fpreal t);
    static int		    sortChannelsCallback(void *data, int, fpreal t,
						 const PRM_Template *);
private:
    int			    my_C;		// current channel index
    int			    my_I;		// current sample index
    int			    my_NC;		// number of channels
    int			    myLastSample;	// last sample, to ignore cook if the channel CHOP uses a low frame rate.
    fpreal		    myLastTime;	// last sample, to ignore cook if the channel CHOP uses a low frame rate.
    UT_ExpandArray	    myChannelArray;
    UT_IntArray		    myTrackInstances;
    UT_IntArray		    myTrackComponents;
};

#endif // __CHOP_CHANNEL_H__
