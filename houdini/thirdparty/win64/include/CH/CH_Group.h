/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CH library (C++)
 *
 * COMMENTS:	Channel Group container
 *
 */

#ifndef __CH_Group_h__
#define __CH_Group_h__

#include "CH_API.h"
#include "CH_ChannelRef.h"
#include "CH_Collection.h"
#include <UT/UT_String.h>
#include <UT/UT_ValArray.h>

#include <iosfwd>

class UT_StringArray;
class CH_GroupEntry;
class CH_Channel;
class CH_Collection;
class CH_Manager;

/*
 * IMPORTANT:  CH_Group pointers are persistent
 * 		that means, CH_Manager functions go to great lengths to
 * 		ensure that channel groups preserve their CH_Group pointers
 * philosophy:
 * 1. a group stores channels and patterns
 * 2. channels are added through all the usual channel addition means
 * 3. channels are deleted through usual channel deletion means, or when a
 *    channel is deleted
 * 4. renaming a channel or chaning its alias has no effect on its membership
 * 5. groups are warned by the channel manager when channels get deleted or
 *    renamed (TODO if necessary)
 * 6. channel pointers are guaranteed to stay the same for the duration of the
 *    existence of the channel.
 * 	- this means that deleting and re-adding a channel is not a null
 * 	operation, as the group will lose it.  Moreover, this is bad style
 * 	because deleting a channel will send out events that may not be cheap.
 * 	- if this becomes too hard to maintain, then it is easy to go back to
 * 	storing channel names instead of pointers, by simply modifying the
 * 	channel add/remove functions
 * 7. patterns are TODO, but will remain even when the pattern no longer
 *    matches any channels
 * 8. when a channel is created, the channel manager notifies groups so that
 *    they can perform the pattern match.  Hence, patterns may be slow.
 */

/*
 * - the marking (CHM) indicates that functions in this section should only be
 * called from the channel manager.
 * - the marking (*) indicates that anyone can call this function
 * - the section labelled INTERNAL should only be called from within CH_Group
 */

class CH_API CH_Group
{
friend class CH_Manager;
friend class CH_UndoGroupAddRemoveChannel;

public:
    static char		 getGroupChar()			{ return '@'; }
    static int		 cmp( const CH_Group *a, const CH_Group *b );

private:
    // CONSTRUCTORS AND DESTRUCTOR (CHM)
     CH_Group();
    ~CH_Group();

    // (CHM)
    void		 setName(const char *name);
    CH_Group		*clone();

public:
    // INFO FUNCTIONS(*)
    const UT_String	&getName() const		{ return myName; }
    void		 getFullPath(UT_String &str) const;
    bool		 isInTree() const { return myIsInTree; }
    bool		 isRoot() const { return myIsInTree && myParent==NULL; }
    bool		 isEmpty() const;
    int			 getTotalEntries() const;
 
    void		 clear();
    void		 displayInfo(int indent = 0) const;

    // ORDERING (*)
    void		 makeLast();
    void		 changeIndex( int new_index );

    // GROUP TREE ITERATION (*)
    CH_Group		*getParentGroup() { return myParent; }
    int			 getSubGroupEntries() { return myGroups.entries(); }
    const UT_ValArray<CH_Group*> 	&getSubGroups() { return myGroups; }
    bool		 isEventualParentOf( CH_Group *other );
    int			 findIndex( CH_Group *subgroup );

    // CHANNEL QUERY FUNCTIONS (*) (no undos)
    int			 getEntries() const { return myChannels.size(); }
    bool		 strictContains( const CH_Channel *chp) const;
    bool		 strictContains( const CH_ChannelRef &chref) const;
    bool		 contains(const CH_Channel *chp) const;
    bool		 contains(const CH_ChannelRef &chref) const;
    int			 strictGetChannels(CH_ChannelList &channels) const;
    int			 strictGetChanRefs(CH_ChannelRefList &chanrefs) const;
    int			 getChannels(CH_ChannelList &channels) const;
    int			 getChanRefs(CH_ChannelRefList &chanrefs) const;
    int			 getFullChannelPaths(UT_StringArray &channel_paths,
					     bool use_alias=false) const;

    // CHANNEL FUNCTIONS (*)
    // return values: number of channels added/removed
    int			 addChannel(const CH_Channel *chp);
    int			 addChannel( const CH_ChannelRef &chref );

    int			 removeChannel( const CH_Channel *chp);
    int			 removeChannel( const CH_ChannelRef &chref );

    int			 addChannels(const CH_ChannelList &channels);
    int			 addChannels(const CH_ChannelRefList &chanrefs);

    int			 removeChannels(const CH_ChannelList &channels);
    int			 removeChannels(const CH_ChannelRefList &chanrefs);
    void		 changeReferencesToNodeId(int old_id, int new_id);
    void		 removeChannelsWithNodeId(int node_id);

    // PATTERN FUNCTIONS (*)
    //bool		 addPattern(const char *pattern); // returns exists
    //bool		 removePattern(const char *pattern); // returns found

    // FLAGS FUNCTIONS (*)
    unsigned		 getScopeFlags( unsigned flags );
    void		 dirtyScopeFlags( unsigned flags );
    void		 dirtySubtreeScopeFlags( unsigned flags );
    // for passive flags only
    // this function updates passive flags to respond to parameter state changes
    bool		 updateScopeFlags( unsigned flags );
    void		 dirtyAllScopeFlags();

private:
    // INTERNAL FUNCTIONS
    void 		 getChanRefsInternal(CH_ChannelRefTable &table) const;
    int			 addChannelInternal(const CH_ChannelRef &chref);
    int			 removeChannelInternal(const CH_ChannelRef &chref);
    void		 removeMeFromMyParent();
    CH_Group		*createGroupInternal(const char *name);
    bool		 removeGroupInternal(CH_Group *sub_group);
    void		 removeChanRefFromParents(const CH_ChannelRef &chref);

    // MESSAGE FUNCTIONS (CHM)
    void		 channelDeleted( const CH_Channel *chp );
    void		 channelCreated( const CH_Channel *chp );

    // GROUP TREE FUNCTIONS (CHM)
    void		 findValidGroupName( UT_String &name );
    CH_Group		*findGroup(const char *name, bool create);
    void		 findGroups( UT_ValArray< CH_Group* > &groups,
					const char *pattern, bool minimal );
    void		 insertGroup( CH_Group *source,
					const char *new_name=0 );
    bool		 getGroupsFromChannels(
				    const CH_ChannelRefTable& channels,
				    CH_GroupList& groups);
    void		 propagateInTreeFlag();

public:
    // IO FUNCTIONS (*)
    void		 save(std::ostream &os, int binary) const;
    bool		 load(UT_IStream &is);

private:
    UT_String				 myName;
    CH_Group				*myParent;
    unsigned				 myScopeFlags: CH_NUM_FLAGS;
    unsigned				 myDirtyScopeFlags: CH_NUM_FLAGS;
    unsigned				 myIsInTree: 1;

    CH_ChannelRefTable			 myChannels;
    //UT_ValArray<CH_GroupEntry *>	 myPatterns;
    UT_ValArray<CH_Group *>		 myGroups;
};

#endif
