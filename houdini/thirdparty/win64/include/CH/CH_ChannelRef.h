/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CH_ChannelRef.h ( CH Library, C++)
 *
 * COMMENTS:
 */

#ifndef __CH_ChannelRef__
#define __CH_ChannelRef__

#include "CH_API.h"
#include "CH_Types.h"
#include <UT/UT_Map.h>
#include <UT/UT_String.h>

#include <functional>
#include <iosfwd>

class UT_StringArray;

class CH_API CH_ChannelRef
{
public:

    CH_ChannelRef(const CH_ChannelRef &other)
    {
	// this is a bit faster than calling through setChannelRef,
	// since we avoid the allocString
	UT_ASSERT( other.myName );
	myNodeId = other.myNodeId;
	myIsTrack = other.myIsTrack;
	myName = other.myName;
    }

    CH_ChannelRef( int node_id, const char *name, bool is_track )
    {
	setChannelRef(node_id, name, is_track);
    }

    CH_ChannelRef()
    {
	clear();
    }

    ~CH_ChannelRef()
    {
	// do nothing for now -- maybe later free strings?
    }

    void	 setChannelRef(int node_id, const char *name, bool is_track);

    void	 clear();

    int		 getNodeId() const { return myNodeId; }

    void	 setNodeId( int new_id ) { myNodeId = new_id; }

    const char	*getName() const   { return myName; }

    bool	 isTrack() const   { return myIsTrack; }

    bool	 operator==( const CH_ChannelRef &other ) const
    {
	return myNodeId == other.myNodeId &&
	       myIsTrack == other.myIsTrack &&
	       myName == other.myName;
    }

    CH_ChannelRef &operator=( const CH_ChannelRef &other )
    {
	setChannelRef(other.myNodeId, other.myName, 
		      other.myIsTrack);
	return *this;
    }

    bool	  operator<(const CH_ChannelRef &other) const
		  {
		      if (myNodeId < other.myNodeId)
			  return true;
		      // FIXME, we should sort by path, not by id
		      if (myNodeId == other.myNodeId &&
			      isLess(myName, other.myName))
			  return true;
		      return false;
		  }

    bool	  operator>(const CH_ChannelRef &other) const
		  {
		      if (myNodeId > other.myNodeId)
			  return true;
		      // FIXME, we should sort by path, not by id
		      if (myNodeId == other.myNodeId &&
			      isGreater(myName, other.myName))
			  return true;
		      return false;
		  }

    int		  compare(const CH_ChannelRef& other) const
		  {
		      if (*this < other)
			  return -1;
		      if (*this > other)
			  return 1;
		      return 0;
		  }

    unsigned int	hash() const;

private:
    bool	 isLess(const char *str1, const char *str2) const
		 {
		     bool	rval;
		     if( !str1 )
		     {
			 if( str2 ) rval = true;// Yes, null ptr is < str2
			 else rval = false;
		     }
		     else
		     {
			 if( !str2) rval = false;// Nope, null is < str2
			 else rval = (strcmp(str1, str2) < 0) ? true : false;
		     }
		     return rval;
		 }
    bool	 isGreater(const char *str1, const char *str2) const
		 {
		     bool	rval;
		     if( !str1 ) rval = false;
		     else
		     {
			 if( !str2 ) rval = true;// Yes, str1 is > null
			 else rval = (strcmp(str1, str2) > 0) ? true : false;
		     }
		     return rval;
		 }

    int		 myNodeId:30;
    unsigned	 myIsTrack:1;
    const char	*myName;
};


inline size_t hash_value(const CH_ChannelRef &ref)
{
    return ref.hash();
}

/// CH_ChannelRefTable maintains a set of CH_ChannelRef's along with some
/// flags that are from CH_ScopeMasks (see CH_Types.h)
typedef UT_Map<CH_ChannelRef, unsigned> CH_ChannelRefTable;

typedef std::function<bool(const CH_ChannelRef&, unsigned flags)>
	CH_ChannelRefQueryCallback;
typedef std::function<void(const CH_ChannelRef&)>
	CH_ChannelRefCallback;

/// Deletes all the symbols from the table that have the given node_id,
/// returns the number of items deleted
CH_API extern int
CHdeleteRefsWithNodeId(
	CH_ChannelRefTable &ref_table,
	int node_id,
	CH_ChannelRefQueryCallback should_delete_callback = nullptr,
	CH_ChannelRefCallback pre_delete_callback = nullptr);

CH_API extern void
CHaddFullChannelPath(
	const CH_ChannelRefTable &table,
	UT_StringArray &chan_paths,
	bool use_alias);

CH_API extern void
CHaddExposedChannel(
	const CH_ChannelRefTable &table,
	CH_ChannelList &channels);

CH_API extern void
CHaddExposedChanRef(
	const CH_ChannelRefTable &table,
	CH_ChannelRefList &chanrefs,
	unsigned mask); // mask = 0 means no filtering

CH_API extern int
CHcompareChanRefs(const CH_ChannelRef*, const CH_ChannelRef*);

CH_API extern bool
CHchannelsToChanRefs(const CH_ChannelList &channels, CH_ChannelRefList &refs);

CH_API extern bool
CHchanRefsToChannels(const CH_ChannelRefList &refs, CH_ChannelList &channels);

CH_API extern void
CHdisplayChanRefs(const CH_ChannelRefList &refs);

CH_API extern std::ostream &
operator<<(std::ostream &os, const CH_ChannelRef &ref);

#endif
