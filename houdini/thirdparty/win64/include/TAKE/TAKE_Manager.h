/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TAKE_Manager.h ( TAK Library, C++)
 *
 * COMMENTS:	Manage all layers
 */

#ifndef __TAKE_Manager__
#define __TAKE_Manager__

#include "TAKE_API.h"
#include "TAKE_Event.h"
#include <UT/UT_Array.h>
#include <UT/UT_String.h>
#include <UT/UT_SymbolTable.h>
#include <iosfwd>

class UT_String;
class UT_StringArray;
class TAKE_Data;
class TAKE_DataId;
class TAKE_Take;
class TAKE_StringSaver;

class TAKE_API TAKE_Manager {
public:
	     TAKE_Manager();
    virtual ~TAKE_Manager();

    static TAKE_Manager	*getCurrentManager();
    static void		 setCurrentManager(TAKE_Manager *mgr);

    static bool		 getAutoIncludeModeFlag()
				    { return ourAutoIncludeModeFlag; }
    static void		 setAutoIncludeModeFlag(bool state) 
				    { ourAutoIncludeModeFlag = state; }

    static bool		 getAutoIncludeLockedAssetsFlag()
				    { return ourAutoIncludeLockedAssetsFlag; }
    static void		 setAutoIncludeLockedAssetsFlag(bool state) 
				    { ourAutoIncludeLockedAssetsFlag = state; }

    TAKE_Take	*getMasterTake()	{ return myMasterTake; }
    // Destroy the specified take, and, if cleaned_dead_data_from_master_take
    // is provided, set it to true if we've cleaned dead data from the master
    // take.
    void	 destroyTake(TAKE_Take *take,
			     bool *cleaned_dead_data_from_master_take = 0);

    void	 clear();

    TAKE_Take	*findTake(const char *name);

    void	 getTakes(UT_Array<TAKE_Take *> &takes) const;

    virtual void	 switchToTake(TAKE_Take *take,
				      bool force_even_if_current=false);

    bool		 isSwitching() const	{ return mySwitching; }

    // Re-apply all the take data for a particular take data id
    void	 reApplyTake(const TAKE_DataId &id);

    // Insert a layer with the current layer as a parent layer.  All children
    // of the current layer will inherit values from the new layer.  If the
    // parent is null, then the take will be added as a child of the current
    // take.
    TAKE_Take	*insertTake(const char *name, const char *parent = 0);

    TAKE_Take	*getCurrentTake()	{ return myCurrentTake; }
    bool	 isMasterTake()		{ return myCurrentTake==myMasterTake; }

    int		 globTakes(UT_Array<TAKE_Take *> &list, const char *pattern);

    virtual void renameTake(TAKE_Take *take, const char *newname);
    bool	 reparentTake(TAKE_Take *take, TAKE_Take *parent);

    // Find the take in the tree which defines the data entity.  If from_which
    // is 0, the definition will be found by scanning from the current take.
    // If a null ptr is returned, the parameter is defined in the take
    // specified.
    TAKE_Take	*findDefinition(const TAKE_DataId &id, TAKE_Take *from_which=0);

    int		 getListSerial() const	{ return myListSerial; }

    const char	*getDefaultTakeName()	{ return myTakeName; }
    void	 setDefaultTakeName(const char *s);

    //
    // Merge the take data from the source take into the destination take.  If
    // the override is set, then any data in the destination will be
    // overwritten by the source take.
    bool	 mergeTake(TAKE_Take *dest, TAKE_Take *src,
			    bool overwrite_dest=false);

    // Saving and loading the master take.
    virtual int		 save(std::ostream &os, int binary) const;
    virtual bool	 load(UT_IStream &is);

    int			 saveTake(TAKE_StringSaver &strings, TAKE_Take *take,
       			   std::ostream &os, int bin, bool recurse) const;
    bool		 loadTake(UT_StringArray &strings, TAKE_Take *parent,
				    UT_IStream &is,
				    UT_Array<TAKE_Take *> *createdTakes);

    int			 copyTake(std::ostream &os, TAKE_Take *take,
				    bool binary, bool recurse) const;
    bool		 pasteTake(UT_IStream &is,
				    UT_Array<TAKE_Take *> &createdTakes,
				    TAKE_Take *under = 0);

    // Stashes the specified layer and all the inherited layers
    int			 stashTakeForUndo(TAKE_Take *layer,
       			           std::ostream &os, int binary) const;
    // Restore a layer from a stash
    bool		 restoreTakeForUndo(UT_IStream &is);

    // During the loading process, we need to create data of arbitrary types.
    // The dataFactory() us used to build the data for us.  The path/label of
    // the data id must be sufficient information to construct the data.
    virtual TAKE_Data	*dataFactory(const char *oppath, const char *label) = 0;

    virtual void	notifyObservers(TAKE_Event::TAKE_EventType type,
					const char *name,
					const TAKE_Take *take=0);

    UT_NotifierImpl<TAKE_Event &>   &getEventNotifier(){return myEventNotifier;}

protected:
    // Build a list of all the takes (in reverse order) from the specified take
    // to the root.
    int			 buildTakeTree(TAKE_Take *take,
					UT_Array<TAKE_Take *> &list);
    bool		 generateName(const char *name, UT_String &result);
    bool		 generateUniquePrefix(const char *name, UT_String &result);

    UT_SymbolMap<TAKE_Take *> myNameSpace;
    TAKE_Take		*myMasterTake;
    TAKE_Take		*myCurrentTake;

    // A method to allow a derived class to query whether the current take
    // is pending activation.
    bool		 isCurrentTakeActivationPending() const
				    { return myCurrentActivationPendingFlag; }

    // A method to allow a derived class to perform the pending activation of
    // the current take.
    void		 handlePendingTakeActivation();

    // Absorb the specified external take hierachy, rooting it at a new top
    // level child take.  The return value is the newly created root take
    // for the merged hierarchy, or 0 if the external hierarchy is empty, as
    // no root take is created.
    //
    // A common (unique) prefix, derived from prefix_base, is used for this
    // new root and all merged takes.  The current_take_to_require_activation
    // argument, if true, indicates that if the next processed take switch is
    // to the current take (i.e. a null op), we must still activate the current
    // take.  This will typically only be necessary if the current take is the
    // master take as we're otherwise only adding new takes, not modifying
    // existing take data.
    // TODO: src argument is not const because of TAKE_Take::getChild().
    TAKE_Take	 	*mergeExternal(TAKE_Manager &src,
				       const char *prefix_base,
				       bool current_take_to_require_activation,
				       UT_String &prefix);

private:
    TAKE_Take		*createTake(TAKE_Take *parent, const char *name);
    void		 createMasterTake();
    void		 destroyMasterTake();
    bool		 saveMagic(std::ostream &os) const;
    bool		 loadMagic(UT_IStream &is,
				    int &vmajor, int &vminor) const;

    void		 mergeExternal(TAKE_Take *parent, TAKE_Take *src,
				       const char *prefix, bool recurse);

    int					myListSerial;
    UT_NotifierImpl<TAKE_Event &>	myEventNotifier;
    UT_String				myTakeName;
    bool				mySwitching;

    // This flag is used to indicate that, while we may have a current take
    // set, it has not been properly activated.
    bool				myCurrentActivationPendingFlag;

    static bool				ourAutoIncludeModeFlag;
    static bool				ourAutoIncludeLockedAssetsFlag;
};

#endif
