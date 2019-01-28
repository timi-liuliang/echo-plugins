/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PI_OldParms.h ( PI Library, C++)
 *
 * COMMENTS:
 *
 *	This class manages the history of modifications done to parms in
 *	a PI_EditScriptedParms.
 */

#ifndef __PI_OldParms_h__
#define __PI_OldParms_h__

#include "PI_API.h"

#include <UT/UT_Array.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_String.h>
#include <UT/UT_StringArray.h>

class PRM_Parm;
class OP_Node;
class OP_Operator;
class PI_EditScriptedParm;
class PI_EditScriptedParms;
class UT_StringHolder;

// This enum must be kept in sync with the menu in $FUSE/FUSE_PrefDialog.ui.
typedef enum {
    PI_PARMDEL_UPDATEALL,
    PI_PARMDEL_UPDATESIMPLE,
    PI_PARMDEL_UPDATENONE
} PI_ParmDelBehavior;

class PI_API pi_OldParm
{
public:
    pi_OldParm() : 
	myOrigName(UT_String::ALWAYS_DEEP),
	myNewName(UT_String::ALWAYS_DEEP),
	myOrigChanSuffix(UT_String::ALWAYS_DEEP),
	myNewChanSuffix(UT_String::ALWAYS_DEEP),
	myOrigSubIdx(0),
	myDeleted(false),
	myIsRamp(false),
	myIsColorRamp(false)
    {
    }

    // Make sure you harden these as they'll be deleted.
    UT_String		myOrigName;
    UT_String		myNewName;
    UT_String		myOrigChanSuffix;
    UT_String		myNewChanSuffix;
    int			myOrigSubIdx;
    UT_StringArray	myMultiOwners;
    bool		myDeleted:1,
			myIsRamp:1,
			myIsColorRamp;
};

class PI_API PI_OldParms
{
public:
		 PI_OldParms();
		 PI_OldParms(const PI_OldParms &other);
		~PI_OldParms();

    /// Remove all parameters.
    void	 clear();

    // Build a list of parameters from the given edit parms structure.
    void	 preserveOldParms(PI_EditScriptedParms *eso);

    /// Add parameter to set.
    int		 addOldParm(const char *name, const char *chan, int subidx);

    // Keep track of changes to old parm names or deletions.
    void	 renameOldParm(const char *oldname, const char *newname);
    void	 deleteOldParm(const char *newname);
    void	 renameOldParmChannel(const char *newname, const char *oldchan,
				      const char *newchan);
    void	 deleteOldParmChannel(const char *newname, const char *newchan);
    void	 undeleteOldParm(const char *oldname);

    // This updates all parm links to make sure they use the new names.
    void	 resolveOldParmLinks(int node_id, PI_EditScriptedParms *eos,
				     UT_StringArray &errs,
				     UT_StringArray &parmdelwarnings);

    /// Return true if we have a parameter registered with the specified name.
    bool	 hasOldParm(const char *oldname) const;

    // Returns true if we have a deleted old parm with th specified name.
    bool	 hasDeletedOldParm(const char *oldname) const;

    /// Pass back a list of parameters that have been deleted.
    void	 getDeletedParms(UT_StringArray &deleted_parms) const;

    /// Pass back lists of parameters that have been renamed.
    /// `old_parms` contains the old parameter names.
    /// `new_parms` contains the new parameter names.
    /// The size of `old_parms` and `new_parms` is the same with
    /// old_parm(i) => new_parm(i) representing the parameter name change.
    void	 getRenamedParms(UT_StringArray &old_parms,
		    UT_StringArray &new_parms) const;

    /// Return the original name of the parameter before it was renamed.
    /// Return `parm_name` if no such parameter exists with that name
    /// or if the parameter was never renamed.
    const char	*getOriginalName(const char *parm_name) const;

    /// Return the new name of the parameter after it was renamed.
    /// Return `old_parm_name` if no such parameter exists with that name
    /// or if the parameter was never renamed.
    const char *getNewName(const char *old_parm_name) const;

    // Control the way we update references to parameters that are deleted.
    static int	 getParameterDeletionBehavior();
    static void	 setParameterDeletionBehavior(int behavior);

private:
    void	 initOldParmMultiOwners(
		    int idx,
		    const UT_ValArray<PI_EditScriptedParm *> &folders,
		    bool omit_last);

    // A utility function for doing generic string replacement.
    bool	 getModifiedString(const char *srcstr, UT_StringHolder &deststr,
				   const OP_Node *root, OP_Node *node,
				   const char *origname, const char *newname);

    void	 updateConditional(UT_StringHolder &cond_str,
                                   const PI_EditScriptedParm *parm,
                                   OP_Node *node, 
                                   const char *origname,
                                   const char *newname,
                                   bool deleted,
                                   const char *field_name,
                                   UT_StringArray &errs);
    
    // A utility to create all the channel names for a pi_OldParm entry
    void	 addChannelNames(OP_Node &node,
				 const pi_OldParm &old_parm,
				 const char *origname,
				 const char *newname,
				 bool whole_multiparm,
				 UT_StringArray &old_names,
				 UT_StringArray &new_names);

    /// These are helper functions are called by the main resolveOldParmLinks
    /// function defined in the public interface.
    // @{ 
    /// This responsible for changing expressions of a single pi_OldParm entry.
    void	 resolveOldParmChannels(OP_Node &node,
				   PI_EditScriptedParm *ep, int epidx,
				   const char *origname, const char *newname,
				   const pi_OldParm &old_parm,
				   UT_StringArray &errs,
				   UT_StringArray &parmdelwarnings);
    /// This responsible for changing expressions of a single PRM_Parm entry.
    void	 resolveSingleParm(OP_Node *root,
				   const char *origname, const char *newname,
				   UT_ValArray<PRM_Parm *> &parms,
				   UT_IntArray &parmsubidxs,
				   bool deleted,
				   bool autolink,
				   UT_StringArray &errs,
				   UT_StringArray &parmdelwarnings);
    /// This is for changing multiparm link info respectively.
    void	 resolveOldMultiParmInfo(OP_Node *root,
				     const char *origname, const char *newname,
				     UT_StringArray &instchans,
				     bool deleted,
				     UT_StringArray &errs,
				     UT_StringArray &parmdelwarnings);
    // @}

private:
    UT_Array<pi_OldParm>	 myOldParms;
    static PI_ParmDelBehavior	 theParmDelBehavior;
};

#endif
