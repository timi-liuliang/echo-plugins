/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CH library (C++)
 *
 * COMMENTS:	Manager of all channels for a world.
 *
 */

#ifndef __CH_Manager_h__
#define __CH_Manager_h__

#include "CH_API.h"
#include "CH_ChannelRef.h"
#include "CH_EvalContextFwd.h"
#include "CH_ExprLanguage.h"
#include "CH_KeyState.h"
#include "CH_Types.h"

#include <DEP/DEP_ContextOptions.h>
#include <UT/UT_Assert.h>
#include <UT/UT_Color.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Map.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_Set.h>
#include <UT/UT_String.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_SuperInterval.h>
#include <UT/UT_Thread.h>
#include <UT/UT_ThreadSpecificValue.h>
#include <UT/UT_ValArray.h>
#include <SYS/SYS_Floor.h>
#include <SYS/SYS_Hash.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>

class CH_Channel;
class CH_ChannelRef;
class CH_Collection;
class CH_ExprDefine;
class CH_Group;
class CH_LocalVariable;
class CH_Manager;
class chParamData;
class CH_ScopedChannels;
class CH_ScriptAccessManager;
class CH_Segment;
class CH_TimeGroup;
class DEP_MicroNode;
class UT_WorkBuffer;

typedef bool (*CH_StringVarEvalFunc)(UT_String &val, int varid, int thread);
typedef bool (*CH_FloatVarEvalFunc)(fpreal &val, int varid, int thread);
typedef bool (*CH_IntVarEvalFunc)(int &val, int varid, int thread);

class CH_API CH_ChannelColorManager
{
public:
    virtual ~CH_ChannelColorManager(){}
    virtual UT_Color getChannelColor(const CH_Channel *channel) = 0;
    virtual UT_Color getChannelColor(const CH_ChannelRef &channel_ref) = 0;
};

class CH_API CH_ChannelChanged
{
public:
    virtual ~CH_ChannelChanged(){}
    virtual void changed() = 0;
};

class CH_API CH_FilterChanged
{
public:
    virtual ~CH_FilterChanged(){}
    virtual void changed(int type, void* fsc) = 0;
    virtual void removed(void* fsc) = 0;
};

class CH_API CH_LoadInfo
{
public:
    bool	use_load_range;
    fpreal64	load_start;
    fpreal64	load_end;
    fpreal64	file_start;
    fpreal64	file_end;
    bool	full_replace;
};


// CH_ScopedChannels holds the scope/display/layer/pinned flags
// for all the channels.
// The CH_Manager holds a list of scoped channels and we refer to that list
// as the Global Scoped Channels.
// The Animation Editor can pin its state to create and act on a disconnected
// list of channels.
class CH_API CH_ScopedChannels
{
public:
    CH_ScopedChannels( CH_Manager *mgr, bool global=false );
    virtual ~CH_ScopedChannels();


    // these next two methods can be used to manipulate channel scope state
    // using the full path of the channel.  The correct behaviour will take 
    // place if the channel doesn't yet exist and so forth.
    bool	 scopeChannelInternal(const CH_Channel *chp, bool on_off);
    bool	 scopeChannelInternal(const CH_ChannelRef &chref, bool on_off);
    void	 scopeChannel(const CH_ChannelRef &chref, bool on_off);
    void	 scopeChannel(const char *full_chan_path, bool on_off);
    void	 unscopeChannelsWithNodeId(int node_id, bool force=false);

    // this function should only be used by CH_Channel, CH_Collection, and here
    void	 scopeChanged( unsigned flags, bool propagate=true );

    // these functions can be used by anyone
    bool	 isChannelScoped(CH_Channel *chp) const;
    bool	 isChannelScoped(const char *full_chan_path) const;
    bool	 isChannelScoped(const CH_ChannelRef &chref) const;

    //
    // Displayed Channels (within the scoped channels)
    //
    bool	 isChannelDisplayed(CH_Channel *chp) const;
    bool	 isChannelDisplayed( const CH_ChannelRef &chref ) const;
    bool	 isChannelDisplayed( const char *full_chan_path ) const;
    void	 clearDisplayedChannels( bool propagate = true );
    void	 displayAllScopedChannels( bool propagate = true );
    void	 displaySelectedScopedChannels( bool propagate = true );

    void	 displayChannel( const CH_ChannelRef &chref,
				 bool propagate = true );
    void	 displayChannel( const char *full_chan_path,
				 bool propagate = true );
    void	 undisplayChannel( const CH_ChannelRef &chref,
				   bool propagate = true );
    void	 undisplayChannel( const char *full_chan_path,
				   bool propagate = true );
    void	 undisplayChannelsWithNodeId(int node_id, bool force=false);
    //
    // Selected Channels (within the scoped channels)
    //
    bool	 isChannelSelected( const CH_ChannelRef &chref ) const;
    bool	 isChannelSelected( const char *full_chan_path ) const;
    void	 clearSelectedChannels( bool propagate = true );
    void	 selectAllScopedChannels(bool propagate = true );

    void	 selectChannel( const CH_ChannelRef &chref,
				bool propagate = true );
    void	 selectChannel( const char *full_chan_path,
				bool propagate = true );
    void	 deselectChannel( const CH_ChannelRef &chref,
				  bool propagate = true );
    void	 deselectChannel( const char *full_chan_path,
				  bool propagate = true );

    //
    // Pinned Channels (within the scoped channels)
    //
    void	 togglePinAllScopedChannels();
    void	 togglePinScopedChannels( const CH_ChannelRefList &chanrefs );
    void	 pinAllScopedChannels( bool propagate = true );
    void	 unpinAllScopedChannels( bool propagate = true );
    void	 pinSelectedChannels( bool propagate = true, 
					int operateOn = CH_SELECTED );
    void	 unpinSelectedChannels( bool propagate = true,
					int operateOn = CH_SELECTED );
    void	 pinChannels( const CH_ChannelRefList &chanrefs, 
					bool propagate = true );
    void	 unpinChannels( const CH_ChannelRefList &chanrefs,
					bool propagate = true );
    void	 pinChannel( const CH_ChannelRef &ref, 
					bool propagate = true );
    void	 unpinChannel( const CH_ChannelRef &ref,
					bool propagate = true );
    void	 unpinChannelsWithNodeId(int node_id, bool force=false);
    void	 clearPinnedChannels( bool propagate = true );

    //
    // Scopped Layers
    //
    void	 clearLayerScope();
    bool	 isLayerScoped(CH_Channel *chp) const;
    bool	 isLayerScoped(const char *full_chan_path) const;
    bool	 isLayerScoped(const CH_ChannelRef &chref) const;
    void	 scopeLayer(const CH_ChannelRef &chref, bool on_off);
    void	 scopeLayer(const char *full_chan_path, bool on_off);
    bool	 scopeLayerInternal(CH_Channel *chp, bool on_off);
    bool	 scopeLayerInternal(const CH_ChannelRef &chref, bool on_off);

    //
    // Selected Layers (within the scoped layers)
    //
    bool	 isLayerSelected(const CH_ChannelRef &chref) const;
    bool	 isLayerSelected(const char *full_chan_path) const;
    void	 clearSelectedLayers(bool propagate = true);
    void	 selectAllScopedLayers(bool propagate = true);

    void	 selectLayer(const CH_ChannelRef &chref, bool propagate=true);
    void	 selectLayer(const char *full_chan_path, bool propagate=true);
    void	 deselectLayer(const CH_ChannelRef &chref, bool propagate=true);
    void	 deselectLayer(const char *full_chan_path, bool propagate=true);

    //
    // Pinned Layers (within the scoped layers)
    //
    void	 togglePinAllScopedLayers();
    void	 togglePinScopedLayers( const CH_ChannelRefList& chanrefs );
    void	 pinAllScopedLayers( bool propagate = true );
    void	 unpinAllScopedLayers( bool propagate = true );
    void	 pinSelectedLayers( bool propagate = true,
			int operateOn = CH_LAYER_SELECTED );
    void	 unpinSelectedLayers( bool propagate = true,
			int operateOn = CH_LAYER_SELECTED );
    void	 pinLayer( const CH_ChannelRef& ref,
			bool propagate = true );
    void	 unpinLayer( const CH_ChannelRef& ref,
			bool propagate = true );
    void	 pinLayers( const CH_ChannelRefList& chanrefs,
			bool propagate = true );
    void	 unpinLayers( const CH_ChannelRefList& chanrefs,
			bool propagate = true );
    void	 clearPinnedLayers( bool propagate = true );

    //
    // Displayed Layers (within the scoped layers)
    bool	 isLayerDisplayed(CH_Channel *chp) const;
    bool	 isLayerDisplayed( const CH_ChannelRef &chref ) const;
    bool	 isLayerDisplayed( const char *full_chan_path ) const;
    void	 clearDisplayedLayers( bool propagate = true );
    void	 displayAllScopedLayers( bool propagate = true );
    void	 displaySelectedScopedLayers( bool propagate = true );

    void	 displayLayer( const CH_ChannelRef &chref,
				 bool propagate = true );
    void	 displayLayer( const char *full_chan_path,
				 bool propagate = true );
    void	 undisplayLayer( const CH_ChannelRef &chref,
				   bool propagate = true );
    void	 undisplayLayer( const char *full_chan_path,
				   bool propagate = true );

    // Graph Selected flag
    bool	 isGraphSelected(CH_Channel *chp) const;
    bool	 isGraphSelected( const CH_ChannelRef &chref ) const;
    bool	 isGraphSelected( const char *full_chan_path ) const;
    void	 clearGraphSelected( bool propagate = true );
    void	 selectGraph( const CH_ChannelRef &chref,
				 bool propagate = true );
    void	 selectGraph( const char *full_chan_path,
				 bool propagate = true );
    void	 deselectGraph( const CH_ChannelRef &chref,
				 bool propagate = true );
    void	 deselectGraph( const char *full_chan_path,
				 bool propagate = true );

    //
    // Generic channel flag manipulation methods
    //
    void	 setChannelFlags( const CH_ChannelRef &chref,
				    unsigned flags,
				    bool on_off,
				    bool propagate = true );
    unsigned	 getChannelFlags( const CH_ChannelRef &chref ) const;

    //
    // Scoping functions
    //
    void	 clearChannelScope( bool clear_all_tables=false);

    // Returns scoped channels only (not scoped parameters)
    void	 getScopedChannels(CH_ChannelList &channels, bool sorted=false);

    // Returns count of scoped channels AND scoped parameters
    int		 getNumScopedChanRefs();

    // mask corresponds to flags in CH_ScopeMasks. Use 0 for mask to mean all
    // scoped channel refs.
    // Returns scoped channel AND scoped parameter refs
    void	 getScopedChanRefs(CH_ChannelRefList &chanrefs,
				    unsigned mask = 0,
				    bool sorted=false) const;
    void	 getSelectedScopedChanRefs(CH_ChannelRefList &chanrefs,
		    bool sorted=false, int operateOn=CH_SELECTED);

    void	 getScopedChannelsKeyFrames( UT_IntArray &frames,
				     int minframe, int maxframe);

    bool	 hasScopedChannels();

    bool	 getResetChannelListSelection() const
		    { return myResetChannelSelection; }
    void	 setResetChannelListSelection(bool b)
		    { myResetChannelSelection = b;}

    enum FilterType
    {
	FilterChannels        = 0,
	FilterAnimationLayers = 1,
    };
    enum
    {
	NumFilterType = 2,
    };

    void	 setEnableFilter( FilterType filter_type, bool b)
		    { myEnableFilter[filter_type] = b; }
    bool	 getEnableFilter( FilterType filter_type ) const
		    { return myEnableFilter[filter_type]; }

    void	 setFilter( FilterType filter_type, const UT_StringHolder &f)
		    { myFilter[filter_type] = f;}
    const UT_StringHolder& getFilter(FilterType filter_type) const
		    { return myFilter[filter_type]; }

    void	 setFilterTranslates( bool b)
		    { myFilterTranlates = b;}
    bool	 getFilterTranslates() const
		    { return myFilterTranlates; }

    void	 setFilterRotates( bool b)
		    { myFilterRotates = b;}
    bool	 getFilterRotates() const
		    { return myFilterRotates; }

    void	 setFilterScales( bool b)
		    { myFilterScales = b;}
    bool	 getFilterScales() const
		    { return myFilterScales; }

    void	 setFilterParmLabels( FilterType filter_type, bool b)
		    { myFilterParmLabels[filter_type] = b;}
    bool	 getFilterParmLabels(FilterType filter_type ) const
		    { return myFilterParmLabels[filter_type]; }

    void	 setFilterFullNames( FilterType filter_type, bool b)
		    { myFilterFullNames[filter_type] = b;}
    bool	 getFilterFullNames(FilterType filter_type) const
		    { return myFilterFullNames[filter_type]; }

    void	 setFilterDirty(FilterType filter_type);


    void	 addDependentFilter(CH_FilterChanged *f);
    void	 removeDependentFilter(CH_FilterChanged *f);
    void	 triggerDependentFilters(int type);
private:
    friend class CH_Manager;

    // Filtering functions
    void filterChannels(CH_ChannelList &chns, FilterType filter_type) const;
    void filterChannelRefs(CH_ChannelRefList &refs, FilterType filter_type) const;
    void setupFilter(UT_String &ret, UT_StringArray &filter_tokens, FilterType filter_type) const;
    bool filterChannelRef( const CH_ChannelRef &chref, FilterType filter_type, const UT_String &prefilter, const UT_StringArray &filter_tokens ) const;

    CH_ChannelRefTable		myTable;
    CH_ChannelRefTable		myTableDisplayed;
    CH_ChannelList		mySorted;
    bool			myIsSorted;
    bool                        myResetChannelSelection;
    mutable int			myNumScopedChannels;

    // Filter for Channels and Animation Layers
    CH_ChannelList		myFiltered[NumFilterType];
    UT_StringHolder		myFilter[NumFilterType];
    bool			myEnableFilter[NumFilterType];
    bool			myFilterParmLabels[NumFilterType];
    bool			myFilterFullNames[NumFilterType];
    bool			myIsFiltered[NumFilterType];
    bool			myFilterTranlates;
    bool			myFilterRotates;
    bool			myFilterScales;
    bool			myIsGlobal;

    CH_Manager			*myManager;

    UT_Array<CH_FilterChanged*> myDependentFilters;
};

class CH_API CH_Manager
{
public:
    typedef enum {
	//CH_ON_COLLISION_FAIL,
	CH_ON_COLLISION_RENAME,
	CH_ON_COLLISION_MERGE,
	CH_ON_COLLISION_REPLACE,
	CH_ON_COLLISION_ASSERT
    } CollisionBehaviour;

    typedef enum {
    	CH_EXPANSION_TYPE_EXPRESSION	= 1,
	CH_EXPANSION_TYPE_VARIABLE	= 1 << 1,
	CH_EXPANSION_TYPE_DEFAULT	= CH_EXPANSION_TYPE_EXPRESSION |
					  CH_EXPANSION_TYPE_VARIABLE,
    } ExpansionType;

    typedef enum {
	CH_SLOPE_MANUAL,
	CH_SLOPE_MANUAL_KEEP,
	CH_SLOPE_AUTOMATIC,
    } SlopeMode;

    typedef CollisionBehaviour (*CBCallback)( CH_Group *source,
						CH_Group *target );

	     CH_Manager(int initTheExpressionLibrary = 1);
    virtual ~CH_Manager();

    /// @private
    /// Called just before the CH_Manger will be deleted
    virtual void	 aboutToDelete();

    //  The following method is used to set the context to this manager.
    static CH_Manager	*getContext()
			 {
			    UT_ASSERT( theManager );
			    return theManager;
			 }
    static bool		 getContextExists() { return (bool)theManager; }
    virtual void	 setContext();

    int			 condenseCommon(UT_String &path);

    void		 resetPlayback();

    void		 resetChannelOptions();
    bool		 loadChannelOptions( const char *filename );
    bool		 saveChannelOptions( const char *filename );

    void		 setDefaultExpression(const char *s)
			    {
				if( s && *s )
				    myDefaultExpression.harden( s );
			    }
    const char		*getDefaultExpression() const
			    { return (const char *)myDefaultExpression; }
    void		 setDefaultRotationExpression(const char *s)
			    {
				if (s)
				    myDefaultRotationExpression.harden(s);
			    }
    const char		*getDefaultRotationExpression() const
			    { return (const char *)myDefaultRotationExpression;}
    void		 setAutoSlope(SlopeMode m)
			    { myAutoSlope = m; }
    SlopeMode		 getAutoSlope() const
			    { return myAutoSlope; }
    void		 setDefaultSlope(fpreal slope)
			    { myDefaultSlope = slope; }
    fpreal		 getDefaultSlope() const
			    { return myDefaultSlope; }
    void		 setAutoSplit(bool onoff)
			    { myAutoSplit = onoff; }
    bool		 getAutoSplit() const
			    { return myAutoSplit; }
    void		 setDefaultSplitExpression(const char *expr)
			    { myDefaultSplitExpression.harden(
							expr ? expr : "" ); }
    const char		*getDefaultSplitExpression() const
			    { return (const char *)myDefaultSplitExpression; }

    void		 setHoldLastKey(bool onoff)
			    { myHoldLastKey = onoff; }
    int			 getHoldLastKey() const
			    { return myHoldLastKey; }
    const char		*getChanCreateKeyExpression() const
			    { return (const char *)myChanCreateKeyExpression; }
    void		 setChanCreateKeyExpression(const char *s)
			    { myChanCreateKeyExpression.harden(s ? s : ""); }
    bool		 getAlwaysCommitKeyChange() const
			    { return myAlwaysCommitKey; }
    void		 setAlwaysCommitKeyChange( bool onoff )
			    { myAlwaysCommitKey = onoff; } 

    // Scoping functions
    // Deprecated and moved to CH_ScopedChannels
    // With H16, we introduced the ability to lock animation editor scoped
    // channels.  The embedded animation layers and scoped channel listers can
    // take a copy of CH_ScopedChannels and acts independtly of the global
    // list.  ChgetScopedChannels() returns the list of globally scoped
    // channels.
    SYS_DEPRECATED(16.0)
    void	 clearChannelScope();

    // Returns scoped channels only (not scoped parameters)
    SYS_DEPRECATED(16.0)
    void	 getScopedChannels(CH_ChannelList &channels, bool sorted=false);

    // Returns count of scoped channels AND scoped parameters
    SYS_DEPRECATED(16.0)
    int		 getNumScopedChanRefs()
		    { return myScopedChannels.getNumScopedChanRefs(); }

    // mask corresponds to flags in CH_ScopeMasks. Use 0 for mask to mean all
    // scoped channel refs.
    // Returns scoped channel AND scoped parameter refs
    SYS_DEPRECATED(16.0)
    void	 getScopedChanRefs(CH_ChannelRefList &chanrefs,
				    unsigned mask = 0,
				    bool sorted=false) const;
    SYS_DEPRECATED(16.0)
    void	 getSelectedScopedChanRefs(CH_ChannelRefList &chanrefs,
		    bool sorted=false, int operateOn=CH_SELECTED);

    // utility functions for getting keyframe numbers:
    // they return array of keyframes in the or scoped channes
    // between minframe and maxframe, indexed relative to the start index
    // The frames are unique and sorted in a ascending order.
    SYS_DEPRECATED(16.0)
    void	 getScopedChannelsKeyFrames( UT_IntArray &frames,
				     int minframe, int maxframe);

    // Useful method to group multiple scope change event notifications
    // together.
    void	 blockModifyScope(int on_off);

    // these next two methods can be used to manipulate channel scope state
    // using the full path of the channel.  The correct behaviour will take 
    // place if the channel doesn't yet exist and so forth.
    SYS_DEPRECATED(16.0)
    void	 scopeChannel(const CH_ChannelRef &chref, bool on_off);
    SYS_DEPRECATED(16.0)
    void	 scopeChannel(const char *full_chan_path, bool on_off);
    SYS_DEPRECATED(16.0)
    bool	 scopeChannelInternal(const CH_Channel *chp, bool on_off);
    SYS_DEPRECATED(16.0)
    bool	 scopeChannelInternal(const CH_ChannelRef &chref, bool on_off);

    virtual void getChannelsWithNodeId(CH_ChannelList &chans, int node_id) const;

    // this function should only be used by CH_Channel, CH_Collection, and here
    SYS_DEPRECATED(16.0)
    void	 scopeChanged( unsigned flags, bool propagate=true );

    // these functions can be used by anyone
    SYS_DEPRECATED(16.0)
    bool	 isChannelScoped(CH_Channel *chp) const;
    SYS_DEPRECATED(16.0)
    bool	 isChannelScoped(const char *full_chan_path) const;
    SYS_DEPRECATED(16.0)
    bool	 isChannelScoped(const CH_ChannelRef &chref) const;

    //
    // Pending Channels
    //
    void	 clearPendingChannels();
    void	 getPendingChannels(CH_ChannelList &channels) const;
    bool	 hasPendingChannels() const;

    // addPendingChannel is intended to be used by CH_Channel/CH_Collection!
    void	 addPendingChannel( CH_Channel *chp );
    void	 removePendingChannel( CH_Channel *chp );

    //
    // Displayed Channels (within the scoped channels)
    //
    SYS_DEPRECATED(16.0)
    bool	 isChannelDisplayed(CH_Channel *chp) const;
    SYS_DEPRECATED(16.0)
    bool	 isChannelDisplayed( const CH_ChannelRef &chref ) const;
    SYS_DEPRECATED(16.0)
    bool	 isChannelDisplayed( const char *full_chan_path ) const;
    SYS_DEPRECATED(16.0)
    void	 clearDisplayedChannels( bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 displayAllScopedChannels( bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 displaySelectedScopedChannels( bool propagate = true );

    SYS_DEPRECATED(16.0)
    void	 displayChannel( const CH_ChannelRef &chref,
				 bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 displayChannel( const char *full_chan_path,
				 bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 undisplayChannel( const CH_ChannelRef &chref,
				   bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 undisplayChannel( const char *full_chan_path,
				   bool propagate = true );

    //
    // Selected Channels (within the scoped channels)
    //
    SYS_DEPRECATED(16.0)
    bool	 isChannelSelected( const CH_ChannelRef &chref ) const;
    SYS_DEPRECATED(16.0)
    bool	 isChannelSelected( const char *full_chan_path ) const;
    SYS_DEPRECATED(16.0)
    void	 clearSelectedChannels( bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 selectAllScopedChannels(bool propagate = true );

    SYS_DEPRECATED(16.0)
    void	 selectChannel( const CH_ChannelRef &chref,
				bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 selectChannel( const char *full_chan_path,
				bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 deselectChannel( const CH_ChannelRef &chref,
				  bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 deselectChannel( const char *full_chan_path,
				  bool propagate = true );

    //
    // Pinned Channels (within the scoped channels)
    //
    SYS_DEPRECATED(16.0)
    void	 togglePinAllScopedChannels();
    SYS_DEPRECATED(16.0)
    void	 togglePinScopedChannels( const CH_ChannelRefList &chanrefs );
    SYS_DEPRECATED(16.0)
    void	 pinAllScopedChannels( bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 unpinAllScopedChannels( bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 pinSelectedChannels( bool propagate = true, 
					int operateOn = CH_SELECTED );
    SYS_DEPRECATED(16.0)
    void	 unpinSelectedChannels( bool propagate = true,
					int operateOn = CH_SELECTED );
    SYS_DEPRECATED(16.0)
    void	 pinChannels( const CH_ChannelRefList &chanrefs, 
					bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 unpinChannels( const CH_ChannelRefList &chanrefs,
					bool propagate = true );
    //
    // Scopped Layers
    //
    SYS_DEPRECATED(16.0)
    void	 clearLayerScope();
    SYS_DEPRECATED(16.0)
    bool	 isLayerScoped(CH_Channel *chp) const;
    SYS_DEPRECATED(16.0)
    bool	 isLayerScoped(const char *full_chan_path) const;
    SYS_DEPRECATED(16.0)
    bool	 isLayerScoped(const CH_ChannelRef &chref) const;
    SYS_DEPRECATED(16.0)
    void	 scopeLayer(const CH_ChannelRef &chref, bool on_off);
    SYS_DEPRECATED(16.0)
    void	 scopeLayer(const char *full_chan_path, bool on_off);
    SYS_DEPRECATED(16.0)
    bool	 scopeLayerInternal(CH_Channel *chp, bool on_off);
    SYS_DEPRECATED(16.0)
    bool	 scopeLayerInternal(const CH_ChannelRef &chref, bool on_off);

    //
    // Selected Layers (within the scoped layers)
    //
    SYS_DEPRECATED(16.0)
    bool	 isLayerSelected( const CH_ChannelRef &chref ) const;
    SYS_DEPRECATED(16.0)
    bool	 isLayerSelected( const char *full_chan_path ) const;
    SYS_DEPRECATED(16.0)
    void	 clearSelectedLayers( bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 selectAllScopedLayers(bool propagate = true );

    SYS_DEPRECATED(16.0)
    void	 selectLayer( const CH_ChannelRef &chref, bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 selectLayer( const char *full_chan_path, bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 deselectLayer( const CH_ChannelRef &chref, bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 deselectLayer( const char *full_chan_path, bool propagate = true );

    //
    // Pinned Layers (within the scoped layers)
    //
    SYS_DEPRECATED(16.0)
    void	 togglePinAllScopedLayers();
    SYS_DEPRECATED(16.0)
    void	 togglePinScopedLayers( const CH_ChannelRefList &chanrefs );
    SYS_DEPRECATED(16.0)
    void	 pinAllScopedLayers( bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 unpinAllScopedLayers( bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 pinSelectedLayers( bool propagate = true, int operateOn = CH_LAYER_SELECTED );
    SYS_DEPRECATED(16.0)
    void	 unpinSelectedLayers( bool propagate = true, int operateOn = CH_LAYER_SELECTED );
    SYS_DEPRECATED(16.0)
    void	 pinLayers( const CH_ChannelRefList &chanrefs, bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 unpinLayers( const CH_ChannelRefList &chanrefs, bool propagate = true );

    //
    // Displayed Layers (within the scoped layers)
    SYS_DEPRECATED(16.0)
    bool	 isLayerDisplayed(CH_Channel *chp) const;
    SYS_DEPRECATED(16.0)
    bool	 isLayerDisplayed( const CH_ChannelRef &chref ) const;
    SYS_DEPRECATED(16.0)
    bool	 isLayerDisplayed( const char *full_chan_path ) const;
    SYS_DEPRECATED(16.0)
    void	 clearDisplayedLayers( bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 displayAllScopedLayers( bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 displaySelectedScopedLayers( bool propagate = true );

    SYS_DEPRECATED(16.0)
    void	 displayLayer( const CH_ChannelRef &chref,
				 bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 displayLayer( const char *full_chan_path,
				 bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 undisplayLayer( const CH_ChannelRef &chref,
				   bool propagate = true );
    SYS_DEPRECATED(16.0)
    void	 undisplayLayer( const char *full_chan_path,
				   bool propagate = true );
    // Generic channel flag manipulation methods
    SYS_DEPRECATED(16.0)
    void	 setChannelFlags( const CH_ChannelRef &chref,
				    unsigned flags,
				    bool on_off,
				    bool propagate = true );
    SYS_DEPRECATED(16.0)
    unsigned	 getChannelFlags( const CH_ChannelRef &chref ) const;

    // Expand a string given a specific channel set (for locals).  It's
    // possible that the channel collection can be null, meaning local
    // variables won't be picked up.  The input string is expected to be an
    // unexpanded string value using the old improper expression syntax.  The
    // method returns a mask with CH_EXPRTIME set if the string uses time
    // dependent variables.
    int		expandString(const char *string, UT_String &expanded,
			     fpreal time, CH_Collection *local = 0,
			     int thread = 0,
			     int expansion_type = CH_EXPANSION_TYPE_DEFAULT,
			     const DEP_ContextOptionsStack *
				context_options_stack = nullptr,
			     DEP_ContextOptionsReadHandle context_options =
				DEP_ContextOptionsReadHandle());

    void	buildStringOpDependencies( const char *str, void *ref_id,
					   CH_Collection *locals, int thread );

    /// Return true if the variable exists in the given string.  The variable
    /// may be in the form @c "$NAME" or "NAME".  If @c expand_expressions is
    /// true, any back-tick expressions will be expanded and the result scanned
    /// for the variable.
    bool	scanForVariableInString(const char *variable,
				const char *string) const;
    bool	changeStringOpRef( UT_String &str,
				   const char *new_fullpath,
				   const char *old_fullpath,
				   const char *old_cwd,
				   const char *chan_name,
				   const char *old_chan_name,
				   CH_Collection *locals,
				   int thread );

    // These functions evaluate expressions that have been specified using the
    // old proper expression syntax.
    fpreal	evaluate(const char *expr, fpreal now,
			CH_Collection *local=0, int *frame_dep=0,
			int thread = 0,
			CH_ExprLanguage language = CH_OLD_EXPR_LANGUAGE,
			     const DEP_ContextOptionsStack *
				context_options_stack = nullptr,
			DEP_ContextOptionsReadHandle context_options =
			    DEP_ContextOptionsReadHandle());
    int		evaluateString(const char *expr, UT_String &result,
			fpreal now, CH_Collection *local=0,
			int thread = 0,
			CH_ExprLanguage language = CH_OLD_EXPR_LANGUAGE,
			const DEP_ContextOptionsStack *
			    context_options_stack = nullptr,
			DEP_ContextOptionsReadHandle context_options =
			    DEP_ContextOptionsReadHandle());

    // Issue an expression evaluation error. 'err' is an index used for
    // ev_GetErrorText(). NOTE: This will call ev_SetErrNo(EV_NOERROR, thread)!
    static void	evaluationError(int err, int thread, const char *path);

    //  Global parameter management

    // This function tells us if we are in the process of setting a global
    // param value. This is useful for differentiating between real node
    // change events, and apparent node changes caused by setting a global
    // param.
    // If add_dependencies is set, the currently evaluating operator will have
    // dependencies added for the global parameter.  Disable this if you're
    // just getting values for push/pop of values.
    bool	getIsSettingGlobalParam() const
		    { return (mySettingGlobalParam > 0); }
    void	getGlobalParamF(const char *token, CH_Collection *scope,
				fpreal &result, fpreal def, int thread,
				bool add_dependencies /*=true*/);
    void	getGlobalParamS(const char *token, CH_Collection *scope,
				UT_String &result, const char *def,
				int thread, bool add_dependencies /*=true*/);
			    
    int		setGlobalFloatParam(const char *token,
			       fpreal val,
			       CH_Collection *&owner);
    int		setGlobalStringParam(const char *token,
			       const char *strval,
			       CH_Collection *&owner);

    int		removeGlobalParamOwner(CH_Collection *owner);
    void	dumpGlobalParams(std::ostream &out) const;

    // The optional dependents set, if specified, will be populated with
    // a set of the dependent CH_Collection(s).
    void	dirtyOrphanGlobalParamDependents(const char *token,
			CH_Collection *scope,
			bool recurse,
			UT_Set<CH_Collection *> *dependents);

    // Like global params, simulation guide parameters need a flag so we
    // know not to destroy interests and extra inputs when they are changed.
    bool	getIsSettingGuideParam() const
		    { return (mySettingGuideParam > 0); }

    static CH_KeyState	 getKeyState(fpreal gtime,
				     const CH_ChannelList &channels,
				     bool loose=false);

    static void		 getUniqueCollections(
				     const CH_ChannelList &channels,
				     CH_CollectionList &collections);

    // Channel Group Management
    CH_Group		*getTopGroup() const;
    CH_Group		*findGroup(const char *const_path, bool create = false,
				    CH_Group *head=0);
    void		 findGroups( CH_GroupList &groups,
					const char *pattern, bool minimal );
    void		 findChannelsFromGroups( CH_ChannelList &channels,
						    const char *pattern );
    void		 findChanRefsFromGroups( CH_ChannelRefList &chanrefs,
						    const char *pattern );
    static bool		 makeGroupPathValid( UT_String &path );
    static bool		 makeGroupNameValid( UT_String &path );

    CH_GroupList	 getSelectedGroups();
    CH_Group		*getCurrentGroup();
    void		 updateGroupSelection(const CH_GroupList &groups,
						void *by,
						bool propagate = true);
    void		 updateGroupSelectionFromScoped(void *by,
						bool propagate = true);
    void		 updateCurrentGroup(CH_Group *group,
						void *by,
						bool propagate = true);
    void		 enforceLeafOnlyChannels(CH_Group *group,
						 const char *name="new_group");
    
    // RMB Group functions
    CH_Group		*createGroup(const char *const_path,
					bool doRename = true, CH_Group *head=0);
    void		 renameGroup(CH_Group *group, const char *newname);
    void		 deleteGroup(CH_Group *group);

    // RMB Group* functions
    void		 extractGroup( CH_Group *group, CollisionBehaviour cb );
    CH_Group		*collapseGroups( CH_GroupList const& groups,
					    const char *name );
    void		 collapseChildren( CH_Group *parent );

    // TODO merge selected

    // Group -> Group functions
    void		 copyIntoGroup( CH_Group *source, CH_Group *target,
					CollisionBehaviour cb,
					const char *new_name = 0 );
    void		 moveIntoGroup( CH_Group *source, CH_Group *target,
					CollisionBehaviour cb,
					const char *new_name = 0 );
    void		 addToGroup( CH_Group *source, CH_Group *target,
					CollisionBehaviour cb );
    void		 mergeWithGroup( CH_Group *source, CH_Group *target,
					    CollisionBehaviour cb );
    // TODO remove shared channels?
    // TODO replace group?

    // Channels -> Group functions implemented via G -> G
    void		channelCreated( const CH_Channel *chp );
    void		channelDeleted( const CH_Channel *chp );

    // save, load functions
    void		  saveGroup(CH_Group *g, std::ostream &os, int binary);
    CH_Group		 *loadGroup(UT_IStream &is);


    // Returns true if there exists a situation in which the string can
    // be expanded.  Returns false if there is nothing in the string that
    // indicates that it could be expanded.
    static bool		 couldExpandString(const char *str);

    // Gets/Sets the default expression language.
    CH_ExprLanguage	 getDefaultExprLanguage()
				{ return myDefaultExprLanguage; }
    void		 setDefaultExprLanguage(CH_ExprLanguage language)
				{ myDefaultExprLanguage = language; }

    // Checks if the given scoped channels list is the global one
    bool isGlobalScopedChannels(CH_ScopedChannels *p)
				{ return p == &myScopedChannels; }

    // Access the global scoped channels
    CH_ScopedChannels& getGlobalScopedChannels() 
				{ return myScopedChannels; }
    const CH_ScopedChannels& getGlobalScopedChannels() const
				{ return myScopedChannels; }

    // Time Group Management
    void		 timeGroupsChanged();
    void		 addTimeGroup(CH_TimeGroup *group,
					bool doRename=true,
					bool force=false);	
    void		 destroyTimeGroup(const CH_TimeGroup *group);
    void		 destroyTimeGroup(int i);
    void		 renameTimeGroup(CH_TimeGroup *group, 
	    				 const char *newname,
					 bool force=false);
    int			 getNTimeGroups() const	{return myTimeGroups.entries();}
    CH_TimeGroup	*getTimeGroup(int i)	   { return myTimeGroups(i); }
    const CH_TimeGroup	*getTimeGroup(int i) const { return myTimeGroups(i); }
    CH_TimeGroup	*getTimeGroup(const char *name);
    const CH_TimeGroup	*getTimeGroup(const char *name) const;
    int			 getTimeGroupIdx( const char *name ) const;

    /// Utility function to make a floating point number "nice" - i.e. only /
    /// use N decimal places.  Should only be used for frames/samples
    /// (not time).
    ///
    /// Note that @c digits does @b not specify precision. It specifies
    /// precision of decimals. The integer portion will be left at full
    /// precision. For example: @code
    ///	niceNumber(12345.2, 1) == 12345;
    ///	niceNumber(1.23452e-14, 3) == 1.23e-14;
    /// @endcode
    /// Note that 6 digits is enough to hold a single precision float.
    static fpreal	 niceNumber(fpreal num, int digits=6)
			    { return SYSniceNumber(num, digits); }

    // Utility function to find a frame separation that is an intelligent
    // multiple/subdivision of the current frame rate, close to the
    // initialSep parameter.
    int			 niceFrameSeparation(fpreal initialSep,
					     int minimumSep = 1) const;

    // Stretching collections will stretch every channel of every collection
    // based on the old time passed to us.
    // The channels will be stretched to fit the new time
    void	stretchCollections(CH_Collection *from, fpreal ostart,
					fpreal oend);

    static fpreal getDefaultFPS();
    static fpreal getDefaultLength(fpreal fps);
    static fpreal getDefaultLengthFrames(fpreal fps);
    static int	  getDefaultChopMotionSamples();

    fpreal	getNumSamples() const		{ return mySampleCount; }
    void	setRawNumSamples(int n);
    fpreal	getSamplesPerSec() const	{ return myFPS; }
    void	setSamplesPerSec(fpreal fps);
    fpreal	getSecsPerSample() const	{ return 1.0 / myFPS; }
    void	setRawSamplesPerSec(fpreal fps);
    int		getChopMotionSamples() const	{ return myChopMotionSamples; }
    void	setChopMotionSamples(int n);

    fpreal	getGlobalStart() const		{ return myStart; }
    fpreal	getGlobalEnd() const		{ return myEnd; }
    int		getGlobalStartFrame() const
		{ return getFrame( myStart ); }
    int		getGlobalEndFrame() const
		{ return getFrame( myEnd ); }
    
    void	setGlobalTime(fpreal start, fpreal end);

    void	setRawInterpolation(int state)	{ myRawInterpFlag = state; }
    int		getRawInterpolation()		{ return myRawInterpFlag; }

    // CONVERSION
    fpreal	getTolerance() const { return myTimeTolerance; }
    // sample -> time
    fpreal	getTime(fpreal sample) const
		{
		    return ((sample-1.0)/myFPS);
		}
    // time -> sample (for calculation)
    fpreal	getSampleRaw(fpreal t) const
	        {
		    return t*myFPS+1.0;
		}
    // time -> sample (for display)
    fpreal	getSample(fpreal t) const
	        {
		    return niceNumber(getSampleRaw(t));
		}
    // time -> sample (integer)
    int		getFrame(fpreal t) const
		{
		    return (int)SYSfloor(getSampleRaw(t+getTolerance()));
		}
    // sample-delta -> time-delta
    fpreal	getTimeDelta(fpreal sample_delta) const
		{
		    return sample_delta/myFPS;
		}
    // time-delta -> sample-delta
    fpreal	getSampleDelta(fpreal time_delta) const
		{
		    return time_delta*myFPS;
		}
    // time -> time (snapped to an integer frame)
    fpreal	getSnapToFrameTime(fpreal t) const
		{
		    return getTime( SYSrint( getSampleRaw(t) ) );
		}

    // frame -> frame (snapped to an integer frame)
    fpreal	getSnapSample(fpreal frame) const
		{
		    return SYSrint(frame);
		}
    // say you have a key at frame 0.6 (base_time) and you are moving it with
    // snap to frame on, so you want to snap the amount you move the key so
    // that the key lands on the nearest frame.  In this example, time_delta
    // would be rounded to numbers like: -1.6, -0.6, 0.4, 1.4. This function
    // does this.  Note that the parameters and return value are in time not
    // frames as they were in the example.
    fpreal	getSnapToFrameDelta(fpreal base_time, fpreal time_delta) const;
    // time -> bool (true if time is at an integer frame boundary)
    bool	getIsAtFrame(fpreal t) const
		{
		    fpreal	snaptime;

		    snaptime = getSnapToFrameTime( t );
		    return SYSisEqual(t, snaptime, getTolerance());
		}
    
    // Get code in buf
    void	getTimeCode(char *buf, fpreal t);
    // Return time from buf
    int		setTimeCode(const char *buf, fpreal &t);

    void	setTimeCodeOffset(fpreal t)	 { myTimeCodeOffset = t; }
    fpreal	getTimeCodeOffset(fpreal ) const { return myTimeCodeOffset; }

    int		getIntegerFrameFlag() const	{ return myIntegerFrames; }
    void	setIntegerFrameFlag(int flag)	{ myIntegerFrames = flag; }

    int		getRestrictRangeFlag() const	{ return myRestrictRange; }
    void	setRestrictRangeFlag(int flag)	{ myRestrictRange = flag; }

    int		getShowRangeFlag() const	{ return myShowRange; }
    void	setShowRangeFlag(int flag)	{ myShowRange = flag; }

    int		getShowTicksFlag() const	{ return myShowTicks; }
    void	setShowTicksFlag(int flag)	{ myShowTicks = flag; }

    int		getShowAudioFlag() const	{ return myShowAudio; }
    void	setShowAudioFlag(int flag)	{ myShowAudio = flag; }

    int		getShowKeysFlag() const		{ return myShowKeys; }
    void	setShowKeysFlag(int flag)	{ myShowKeys = flag; }

    int		getShowSimCacheFlag() const	{ return myShowSimCache; }
    void	setShowSimCacheFlag(int flag)	{ myShowSimCache = flag; }

    fpreal	getAudioLevel() const		{ return myAudioLevel; }
    void	setAudioLevel(fpreal lvl)	{ myAudioLevel = lvl; }

    fpreal	getUnitLength() const		{ return myUnitLength; }
    void	setUnitLength(fpreal unitlength){ myUnitLength = unitlength; }
    fpreal	getUnitMass() const		{ return myUnitMass; }
    void	setUnitMass(fpreal unitmass)	{ myUnitMass = unitmass; }
    void	parseUnitsString(const char *units,
				 fpreal &distexp,
				 fpreal &massexp) const;
    fpreal	scaleToMKS(const char *fromunits) const;
    fpreal	scaleFromMKS(const char *tounits) const;

    int		getRealTimeFlag() const		{ return myRealTimePlay; }
    void	setRealTimeFlag(int flag)	{ myRealTimePlay = flag; }
    fpreal	getRealTimeFactor() const	{ return myRealTimeFactor; }
    void	setRealTimeFactor(fpreal f)	{ myRealTimeFactor = f; }
    bool	getRealTimeDisableSkip() const { return myRealTimeDisableSkip; }
    void	setRealTimeDisableSkip(bool b) { myRealTimeDisableSkip = b; }
    fpreal	getSampleStep() const		{ return mySampleStep; }
    void	setSampleStep(fpreal s)		{ mySampleStep = s; }

    void	getSampleRange(fpreal &start, fpreal &end)
		{
		    start = myRangeStart;
		    end   = myRangeEnd;
		}
    void	setSampleRange(fpreal start, fpreal end)
		{
		    if (start > end) { myRangeEnd = start; myRangeStart = end; }
		    else	     { myRangeEnd = end; myRangeStart = start; }
		}

    int		getHoldFlag() const	{ return myHoldFlag; }
    void	setHoldFlag( int flag )	{ myHoldFlag = flag; }

    // I/O methods
    int		saveChannels(const char *filename, bool binary, bool compiled,
			     const CH_ChannelList &channels, 
                             bool displayed = false) const;
    int		loadChannels(const char *filename,
			     const char *src_pattern,
			     const char *dest_pattern,
			     bool use_range,
			     const fpreal *tstart,
			     const fpreal *tend,
			     CH_CollectionList &parents,
			     bool delete_keys = false,
                             CH_ChannelRefList *displayed_parms = NULL);
    static bool	readCHNFileHeader(UT_IStream &is, bool quiet,
				  fpreal32 &version, int &binary,
				  bool &is_fp64,
				  fpreal64 &tstart, fpreal64 &tend);
    static bool	skipCHNFileHeaderForCollection(UT_IStream &is, bool &is_fp64);
    static bool	getFileType( const char *filename, int &raw, int &binary );
    static bool	printFileInfo(std::ostream &os, const char *filename,
			      CH_Collection *load_coll,
			      bool print_segments=true );

    // Expression glue.  Only call this if you know what you're really doing.
    void	 addExpressionDefine(CH_ExprDefine *def);

    // The following method should only be called by CH_Collection, so please
    // don't mess me up.
    void		 setEvalCollection(CH_Collection *g, int thread)
			    {
				// We use thread-local storage for the eval
				// collection.  This isn't because two
				// different collections can be evaluating
				// paramaters in different threads at the same
				// time, though it would be nice for that to be
				// possible one day.  Instead, it's so that if
				// Houdini is evaluating a parameter at the
				// same time as a python script is running code
				// that checks for the currently evaluating
				// parameter (so it can add the appropriate
				// parm interests), that we don't add parm
				// interests when we shouldn't.
				evalContext(thread).myCollection = g;
				// Reset the other members to avoid stale ptrs
				evalContext(thread).myChannel = 0;
				evalContext(thread).mySegment = 0;
				evalContext(thread).myChannelName = 0;
			    }
    // TODO: Make getEvalCollection() return a const pointer
    CH_Collection	*getEvalCollection(int thread) const
			    {
				// We use thread-local storage the eval
				// collection.
				//
				// If this is the first time this thread has
				// accessed the pointer it will be null.
				// That's exactly the behaviour we want.
				return evalContext(thread).myCollection;
			    }

    // The CH_Manager needs a pointer to the channel collection for the
    // OP_Director. This is used to trigger events when channel and time
    // groups are modified.
    void		 setRootCollection(CH_Collection *root)
    			 { myRootCollection = root; }
    CH_Collection	*getRootCollection() const
			 { return myRootCollection; }

    virtual CH_ScriptAccessManager *getAccessManager(int /*thread*/)
			 { return 0; }

    inline const CH_EvalContext &
			 evalContext(int thread) const
			 { return myEvalContext.getValueForThread(thread); }

    inline CH_EvalContext &
			 evalContext(int thread)
			 { return myEvalContext.getValueForThread(thread); }

    inline const CH_Channel *getEvalChannel(int thread) const
			 { return evalContext(thread).myChannel; }
    inline const CH_Segment *getEvalSegment(int thread) const
			 { return evalContext(thread).mySegment; }
    inline const char	    *getEvalChannelName(int thread) const
			 { return evalContext(thread).myChannelName; }
    inline const DEP_ContextOptionsStack *
			 getEvalChannelContextOptionsStack(int thread) const
			 { return evalContext(thread).myContextOptionsStack; }
    inline DEP_ContextOptionsReadHandle
			 getEvalChannelContextOptions(int thread) const
			 { return evalContext(thread).myContextOptions; }

    /// Functions for obtaining and setting the current evaluation time
    // @{
    inline fpreal	 getEvaluateTime(int thread) const
			    { return evalContext(thread).myTime; }
    inline void		 setEvaluateTime(fpreal time, int thread)
			    { evalContext(thread).myTime = time; }
    // @}

    /// The following function only remains for backwards compatibility of
    /// HDK users. Calls to this function should be replaced with one of the
    /// following forms:
    ///    getEvaluateTime(context.getThread())	-> inside cooking code path
    ///    getEvaluateTime(thread)		-> in expression functions
    ///    CHgetEvalTime()			-> else without thread variable
    SYS_DEPRECATED(11.1) fpreal getEvaluateTime() const
				{ return getEvaluateTime(SYSgetSTID()); }

    //
    //  These methods are usually only used by CH_Collection to evaluate
    //	variables belonging to me...
    CH_LocalVariable	*getLocalVariableTable() const;
    bool		 getVariableValue(UT_String &str, int i,
					  int thread) const;
    bool		 getVariableValue(fpreal &val, int i,
					  int thread) const;

    // Should only be called if the variable has flagged as CH_VARIABLE_INTVAL
    bool		 getVariableValue(int &val, int i, int thread) const;

    // getVariableString should really be protected, but can't do it.
    virtual bool	 getVariableString(const char *name,
					   UT_String &value,
					   int &timeDepend,
					   int thread);

    // Function for adding new global variables with the HDK.
    int			 addVariable(const CH_LocalVariable &var,
				CH_StringVarEvalFunc strfunc,
				CH_FloatVarEvalFunc floatfunc,
				CH_IntVarEvalFunc intfunc) const;

    static void		 lookupVariable(const char *name, UT_String &val,
					int thread);
    static void		 lookupVariableNoThread(const char *name,
						UT_String &val);
    static void		 lookupExpression(const char *name, UT_String &val,
					  int thread);
    static void		 lookupExpressionNoThread(const char *name,
						  UT_String &val);
    
    // This should only be called by CH_Collection
    void		 removeParamDependency(CH_Collection *coll);

    /// Use this method to translate a channel reference to a channel pointer.
    /// May return NULL if the channel reference does not reference a valid
    /// channel.
    virtual CH_Channel	*getChRefChannel(const CH_ChannelRef &r) const;

    /// Use this method to determine if the owner of the channel reference
    /// is exposed or not.
    virtual bool	 isOwnerExposed(const CH_ChannelRef &r) const;

    /// Use this method to build a channel reference from a channel pointer.
    virtual bool	 buildChanRef(CH_ChannelRef &r,
				const CH_Channel *chp) const;

    /// Use this method to build a channel reference from a channel path.
    virtual bool	 buildChanRef(CH_ChannelRef &r,
				const char *path) const;

    bool		 buildChannelRefs( CH_ChannelRefList &reflist,
				const CH_ChannelList &chlist );

    bool		 buildChannelRefs( CH_ChannelRefList &reflist,
				const UT_StringArray &chlist );

    /// Returns 0 if r1 and r2 are equal, returns -1 if r1 < r2 and 
    /// returns 1 if r1 > r2.
    virtual int		 compareChanRefs(const CH_ChannelRef &r1,
				const CH_ChannelRef &r2) const;

    /// Returns 0 if r1 and r2 are equal, returns -1 if r1 < r2 and 
    /// returns 1 if r1 > r2.
    virtual int		 compareLayerRefs(const CH_ChannelRef &r1,
				const CH_ChannelRef &r2) const;

    /// Use this method to get the full path of the channel referenced by r.
    virtual void	 getChanRefPath(const CH_ChannelRef &r,
				UT_String &path,
				bool use_alias=false) const;

    void		 changeReferencesToNodeId(int old_id, int new_id);

    /// This method will remove all references that this manager knows about
    /// channels belonging to node 'node_id'.  After this method, scoped
    /// channels belonging to node_id will be removed, as will such channels
    /// in channel groups.
    void		 removeReferencesToNodeId(int node_id);

    void		 displayScope() const;

    // Find ChannelRef export location
    // Channel CHOP nodes have inputs that match directly to the exported
    // tracks.  Given a Channel CHOP value param and index, find the export
    // location in the scene and return its ChannelRef If the input 'r' channel
    // ref is from a CHOP Channel Node, return the corresponding export in
    // 'out_export'.
    // Returns true if a valid Export was found
    virtual bool	 resolveChanRefExport(const CH_ChannelRef& r,
				CH_ChannelRef& out_export );

    /// Auto-Key all Tuples is a preference to key tuples together.
    bool		 getAutoKeyTuples() const
			    { return myAutoKeyTuples; }
    void		 setAutoKeyTuples(bool b)
			    { myAutoKeyTuples=b; }

    /// Auto-Scope when Creating Channel is a preference to set the auto-scope
    /// flag when creating a channel.
    bool		 getAutoScopeOnChannelCreate() const
			    { return mySetAutoScopeOnChannelCreate; }
    void		 setAutoScopeOnChannelCreate(bool b)
			    { mySetAutoScopeOnChannelCreate=b; }

    /// Include all the Animated Channels when building the scoped parameter
    /// list from selection
    bool		 getAutoScopeIncludeAnimated() const
			    { return myAutoScopeIncludeAnimated; }
    void		 setAutoScopeIncludeAnimated(bool b)
			    { myAutoScopeIncludeAnimated=b; }

    /// 
    bool		 getFollowScopedChannelReferences() const
			    { return myFollowScopedChannelReferences; }
    void		 setFollowScopedChannelReferences(bool b)
			    { myFollowScopedChannelReferences=b; }


    //
    // Animation functions.
    //
    fpreal		 evalBezierFunc(int thread);
    fpreal		 evalConstantFunc(int thread);
    fpreal		 evalCubicFunc(int thread);
    fpreal		 evalCycleFunc(fpreal start_frame, fpreal end_frame,
					int thread);
    fpreal		 evalCycletFunc(fpreal start_frame, fpreal end_frame,
					int thread);
    fpreal		 evalCycleOffsetFunc(
			    fpreal start_frame, fpreal end_frame, int thread);
    fpreal		 evalCycleOffsettFunc(
			    fpreal start_frame, fpreal end_frame, int thread);
    fpreal		 evalEaseFunc(int thread);
    fpreal		 evalEaseinFunc(int thread);
    fpreal		 evalEaseinpFunc(fpreal ease_speed, int thread);
    fpreal		 evalEaseoutFunc(int thread);
    fpreal		 evalEaseoutpFunc(fpreal ease_speed, int thread);
    fpreal		 evalEasepFunc(fpreal ease_speed, int thread);
    fpreal		 evalLinearFunc(int thread);
    fpreal		 evalMatchFunc(int thread);
    fpreal		 evalMatchinFunc(int thread);
    fpreal		 evalMatchoutFunc(int thread);
    fpreal		 evalQcubicFunc(int thread);
    fpreal		 evalQlinearFunc(int thread);
    fpreal		 evalQuinticFunc(int thread);
    fpreal		 evalRepeatFunc(fpreal start_frame, fpreal end_frame,
					int thread);
    fpreal		 evalRepeattFunc(fpreal start_frame, fpreal end_frame,
					int thread);
    fpreal		 evalSplineFunc(int thread);
    fpreal		 evalVmatchFunc(int thread);
    fpreal		 evalVmatchinFunc(int thread);
    fpreal		 evalVmatchoutFunc(int thread);

    /// Include all the Child nodes when building the scoped parameter list
    /// from selection
    bool		 getAutoScopeIncludeChild() const
			    { return myAutoScopeIncludeChild; }
    void		 setAutoScopeIncludeChild(bool b)
			    { myAutoScopeIncludeChild=b; }

    /// Include all the Constraints nodes when building the scoped parameter
    /// list from selection
    bool		 getAutoScopeIncludeConstraints() const
			    { return myAutoScopeIncludeConstraints; }
    void		 setAutoScopeIncludeConstraints(bool b)
			    { myAutoScopeIncludeConstraints=b; }

    // Functions for accessing and controlling the default context option
    // values. These values are used by all OP_Context objects unless a
    // value is explicitly overridden on that context object. These functions
    // also manage the associated micro nodes and send notifications as
    // appropriate.
    DEP_ContextOptionsHandle getDefaultContextOptions() const
			 { return myDefaultContextOptions; }
    DEP_MicroNode	&getDefaultContextOptionMicroNode(
				const UT_StringHolder &opt);
    void		 setDefaultContextOption(
				const UT_StringHolder &opt,
				const UT_StringHolder &value,
				const UT_StringHolder &ui_config);
    void		 setDefaultContextOption(
				const UT_StringHolder &opt,
				fpreal64 value,
				const UT_StringHolder &ui_config);
    void		 removeDefaultContextOption(
				const UT_StringHolder &opt);
    void		 clearDefaultContextOptions();
    bool		 saveDefaultContextOptions(std::ostream &os) const;
    bool		 loadDefaultContextOptions(UT_IStream &is);

    void		 setChannelColorManager( CH_ChannelColorManager *mgr );
    CH_ChannelColorManager *channelColorManager();

    void		 setChannelChanged( CH_ChannelChanged *mgr );
    CH_ChannelChanged   *getChannelChanged();
    void		 triggerChannelChanged();

    virtual bool	 getChannelAutoSelect(const CH_ChannelRef &chref) const
			 { return true; }

    // Resolve channel labels and node path without exposing OP_Node*
    virtual bool getChRefLabelForUI(const CH_ChannelRef &r, UT_String &ret) const;
    virtual bool getChRefAlias(const CH_ChannelRef &r, UT_String &ret) const;
    virtual bool getChRefNodePath(const CH_ChannelRef &r, UT_String &ret) const;
protected:
    void		 initExpressionLibrary();

    // Since this is possibly a base class, the library glue initialization has
    // to be done after the construction
    void		 initLibraryGlue();

private:
    friend CH_ScopedChannels;

    void		 internalScopeChanged(unsigned flags,
				bool propagate=true );

    void		 handleGroupCollision(CH_Group *new_group,
				CH_Group *existing,
				CollisionBehaviour cb );

    void		 destroyContents();
    int			 internalSetGlobalParam(const char *token,
				fpreal val, const char *strval,
				CH_Collection *&owner);
    int			 setSingleGlobalParam(const char *token,
				fpreal val, const char *strvel,
				CH_Collection *&owner, CH_Collection *scope);
    void		 internalDirtyOrphanGlobalParamDependents(
				const char *token, CH_Collection *scope,
				UT_Set<CH_Collection *> *dependents);
    void		 stretchCollection(CH_Collection *grp,
				fpreal os, fpreal ns, fpreal stretch);
    void		 propagateDefaultContextOptionDirtied(
				const UT_StringHolder &opt);

    static char		*chExprExpander(const char *str, int thread);
    
    class chParamKey
    {
    public:
	chParamKey(const UT_StringRef &token, CH_Collection *scope);
	bool operator==(const chParamKey &key) const;
	bool operator!=(const chParamKey &key) const
	{ return !operator==(key); }
	SYS_HashType hash() const;
	const char *getToken() const
	{ return myToken; }
	CH_Collection *getScope() const
	{ return myScope; }
	
	struct Hasher
	{
	    SYS_HashType operator()(const chParamKey &key) const
	    { return key.hash(); }
	};
    private:
	UT_StringRef 	 myToken;
	CH_Collection	*myScope;
    };
    
    UT_ThreadSpecificValue<CH_EvalContext> myEvalContext;

    // Default Context Options for cooking nodes.
    DEP_ContextOptionsHandle		 myDefaultContextOptions;
    UT_StringMap<DEP_MicroNode *>	 myDefaultContextOptionMicroNodes;
    UT_StringMap<UT_StringHolder>	 myDefaultContextOptionUIs;

    CH_Group			*myTopGroup;
    UT_StringArray		 mySelectedGroupPaths;
    UT_String			 myCurrentGroupPath;
    UT_ValArray<CH_TimeGroup *>	 myTimeGroups;
    CH_Collection		*myRootCollection;
    fpreal			 mySampleCount;
    fpreal			 myFPS;
    fpreal			 myStart, myEnd;

    fpreal			 myRangeStart;
    fpreal			 myRangeEnd;
    fpreal			 myRealTimeFactor;
    fpreal			 mySampleStep;
    fpreal			 myTimeTolerance;
    fpreal			 myTimeCodeOffset;

    chParamData			*findGlobalParam(const char *token,
						 CH_Collection *scope);
    chParamData			*findOrCreateGlobalParam(
					const char *token, CH_Collection *scope,
					fpreal fval, const char *sval);
    UT_Map<chParamKey, chParamData *, chParamKey::Hasher> myGlobalParams;
    chParamData			*myLastGlobalParam;
    int				 mySettingGlobalParam;

    int				 mySettingGuideParam;

    int				 myChopMotionSamples;

    unsigned			 myGroupsSorted:1,
				 myIntegerFrames:1,
				 myRealTimePlay:1,
				 myRealTimeDisableSkip:1,
				 myHoldFlag:1,
				 myRestrictRange:1,
				 myShowRange:1,
				 myShowTicks:1,
				 myShowAudio:1, 
				 myShowKeys:1,
				 myShowSimCache:1,
				 myAutoKeyTuples:1,
				 mySetAutoScopeOnChannelCreate:1,
				 myAutoScopeIncludeAnimated:1,
				 myFollowScopedChannelReferences:1,
				 myAutoScopeIncludeChild:1,
				 myAutoScopeIncludeConstraints:1;


    short			 myRawInterpFlag;

    UT_String		myDefaultExpression;
    UT_String		myDefaultRotationExpression;
    bool		myAutoSplit;
    UT_String		myDefaultSplitExpression;
    SlopeMode		myAutoSlope;
    fpreal		myDefaultSlope;
    bool		myHoldLastKey;
    // auto-commit value change when we are on a keyframe
    bool		myAlwaysCommitKey;
    UT_String		myChanCreateKeyExpression;
    fpreal		myAudioLevel;
    fpreal		myUnitLength;
    fpreal		myUnitMass;

    CH_ScopedChannels		myScopedChannels;
    UT_Array<CH_ScopedChannels*> myPinnedScopedChannels;

    UT_Set<CH_Channel *>	myPendingChannelsTable;

    CH_ExprLanguage		myDefaultExprLanguage;

    int				myBlockModifyScope;
    unsigned			myPendingScopeChange;
    bool			myPendingChannelChanged;

    CH_ChannelColorManager	*myChannelColorsManager;
    CH_ChannelChanged		*myChannelChanged;

    static CH_Manager		*theManager;

    friend class CH_SetGlobalParamScope;
    friend class CH_SetGuideParamScope;
};

/// Push & restore the evaluation time (and optionally a channel collection)
class CH_API CH_AutoEvaluateTime
{
public:
    CH_AutoEvaluateTime(CH_Manager &chman,
		int thread,
		fpreal time,
		CH_Collection *ch = nullptr,
		const DEP_ContextOptionsStack *context_options_stack = nullptr,
		DEP_ContextOptionsReadHandle context_options =
		    DEP_ContextOptionsReadHandle())
	: myContext(chman.evalContext(thread))
	, myCollection(nullptr)
	, myPopCollection(false)
	, myPopContextOptions(false)
    {
	myTime = myContext.myTime;
	myContext.myTime = time;
	if (ch)
	{
	    myPopCollection = true;
	    myCollection = myContext.myCollection;
	    myContext.myCollection = ch;
	}
	if (!context_options.isNull())
	{
	    myPopContextOptions = true;
	    myContextOptionsStack = myContext.myContextOptionsStack;
	    myContext.myContextOptionsStack = context_options_stack;
	    myContextOptions = myContext.myContextOptions;
	    myContext.myContextOptions = context_options;
	}
    }

    ~CH_AutoEvaluateTime()
    {
	myContext.myTime = myTime;
	if (myPopCollection)
	    myContext.myCollection = myCollection;
	if (myPopContextOptions)
	{
	    myContext.myContextOptionsStack = myContextOptionsStack;
	    myContext.myContextOptions = myContextOptions;
	}
    }

private:
    CH_EvalContext			&myContext;
    CH_Collection			*myCollection;
    const DEP_ContextOptionsStack	*myContextOptionsStack;
    DEP_ContextOptionsReadHandle	 myContextOptions;
    fpreal				 myTime;
    bool				 myPopCollection;
    bool				 myPopContextOptions;

    friend class CH_Manager;
};

/// Push & restore a channel collection only. If the given collection is NULL,
/// then the eval context is not modified.
class CH_API CH_AutoEvalCollection
{
public:
    CH_AutoEvalCollection(int thread, CH_Collection *collection = NULL)
	: myContext(CH_Manager::getContext()->evalContext(thread))
    {
	myCollection = myContext.myCollection;
	if (collection)
	    myContext.myCollection = collection;
    }
    CH_Collection * collection() const
    {
	return myContext.myCollection;
    }
    void set(CH_Collection *collection)
    {
	UT_ASSERT(collection);
	myContext.myCollection = collection;
    }
    ~CH_AutoEvalCollection()
    {
	myContext.myCollection = myCollection;
    }

private:
    CH_EvalContext	&myContext;
    CH_Collection	*myCollection;
};

// Small utility class to set the active layer contribution and reset it
// The animation layer value and weight comes from CHOP_Layer extra tracks.
// The active_layer_value and active_layer_weight tracks are used correctly
// reapply values on the active layer channels.
class CH_API CH_ActiveLayerContrib
{
public:
    CH_ActiveLayerContrib();
    ~CH_ActiveLayerContrib();

    // set the layer contribution for the lifespan of the object.
    void set(fpreal layer_value, fpreal layer_weight);

    // Apply the active layer contribution to input
    void apply(fpreal& v);

private:
    bool myReset;
};

CH_API void CHsortChannels( CH_ChannelList &channels );
CH_API void CHsortChannelRefs( CH_ChannelRefList &channels );
CH_API void CHsortChannelPaths( UT_StringArray &channel_paths );
CH_API void CHsortLayerRefs( CH_ChannelRefList &channels );

CH_API void CHchannelsToCollections( CH_ChannelList const& channels,
				     CH_CollectionList &collections );

inline
CH_Manager *CHgetManager()
{
    return CH_Manager::getContext();
}

inline
CH_ScopedChannels &CHgetScopedChannels()
{
    return CHgetManager()->getGlobalScopedChannels();
}

inline fpreal CHgetEvalTime()
{
    return CH_Manager::getContext()->getEvaluateTime(SYSgetSTID());
}

inline fpreal CHgetEvalTime(int thread)
{
    return CH_Manager::getContext()->getEvaluateTime(thread);
}

CH_API extern fpreal	  CHgetTimeFromFrame(fpreal frame);
CH_API extern int  	  CHgetFrameFromTime(fpreal time);
CH_API extern fpreal 	  CHgetSampleFromTime(fpreal time);
CH_API extern void	  CHbuildRange(int start, int end, UT_SuperIntervalR &r);

/// The CHoutputChannelList functions output a formatted string 
/// representing the given channels.  One version of the function takes a 
/// list of channels while the other takes a string array of full paths
/// to channels.  The given arrays may be modified (they might need sorting).
CH_API extern void	  CHoutputChannelList(UT_WorkBuffer &info,
					      CH_ChannelList &channels);
CH_API extern void	  CHoutputChannelList(UT_WorkBuffer &info,
					      UT_StringArray &channel_paths);

CH_API extern void	  CHdisplayFlags( unsigned flags, std::ostream &s );

#endif
