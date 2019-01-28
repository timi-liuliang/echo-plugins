/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CHOP Library (C++)
 *
 * COMMENTS:	The base class for all Channel Operators
 *
 */

#ifndef __CHOP_Node_h__
#define __CHOP_Node_h__

#include "CHOP_API.h"
#include "CHOP_Error.h"
#include "CHOP_Notes.h"
#include "CHOP_Types.h"
#include "PRM_ChopShared.h"

#include <OP/OP_InputChangeHelper.h>
#include <OP/OP_Network.h>
#include <OP/OP_Node.h>
#include <PRM/PRM_Parm.h>

#include <CL/CL_Clip.h>
#include <CL/CL_Defines.h>

#include <UT/UT_Array.h>
#include <UT/UT_ExpandArray.h>
#include <UT/UT_NTStreamUtil.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_ValArray.h>


#define CHOP_AUDIO_FLAG		'a'
#define CHOP_EXPORT_FLAG	'o'

class OP_OperatorTable;
class OP_TemplatePair;
class OP_VariablePair;
class OP_OutputCodeParms;
class CHOP_Handle;
class CHOP_HandleData;
class CHOP_Fitter;
class CL_FitParms;
class OR_Mapping;
class orPath;
class UT_OStream;

typedef void (*CHOP_ReplaceCallback)(void *data, int replace);

typedef void (*CHOP_ConflictCallback)(void *data,
		void *ldata, CHOP_ReplaceCallback func,
		const char *source, const char *dest);

extern "C" {
    SYS_VISIBILITY_EXPORT extern void	newChopOperator(OP_OperatorTable *table);
};

#define CHOP_PARM_COUNT		(8)
#define CHOP_TAB	"Common"
#define NONE_TOKEN	"none"

#define CHOP_OUT_CFUNC "_updateCParam"
#define CHOP_OUT_IFUNC "_updateIParam"

#define CHOP_OUT_CPARAM "params.cUpdate"
#define CHOP_OUT_IPARAM "params.iUpdate"


#define CHOP_SWITCHER(cnt, nm) \
    static PRM_Default switcher[2] = \
    { \
	PRM_Default(cnt, nm), \
	PRM_Default(CHOP_PARM_COUNT, CHOP_TAB), \
    }

#define CHOP_SWITCHER2(cnt1,nm1,cnt2, nm2) \
    static PRM_Default switcher[3] = \
    { \
	PRM_Default(cnt1, nm1), \
	PRM_Default(cnt2, nm2), \
	PRM_Default(CHOP_PARM_COUNT, CHOP_TAB), \
    }

#define CHOP_SWITCHER3(cnt1,nm1,cnt2, nm2,cnt3,nm3) \
    static PRM_Default switcher[4] = \
    { \
	PRM_Default(cnt1, nm1), \
	PRM_Default(cnt2, nm2), \
	PRM_Default(cnt3, nm3), \
	PRM_Default(CHOP_PARM_COUNT, CHOP_TAB), \
    }

#define CHOP_SWITCHER4(cnt1,nm1,cnt2, nm2,cnt3,nm3,cnt4,nm4) \
    static PRM_Default switcher[5] = \
    { \
	PRM_Default(cnt1, nm1), \
	PRM_Default(cnt2, nm2), \
	PRM_Default(cnt3, nm3), \
	PRM_Default(cnt4, nm4), \
	PRM_Default(CHOP_PARM_COUNT, CHOP_TAB), \
    }

#define CHOP_SWITCHER5(cnt1,nm1,cnt2, nm2,cnt3,nm3,cnt4,nm4,cnt5,nm5) \
    static PRM_Default switcher[6] = \
    { \
	PRM_Default(cnt1, nm1), \
	PRM_Default(cnt2, nm2), \
	PRM_Default(cnt3, nm3), \
	PRM_Default(cnt4, nm4), \
	PRM_Default(cnt5, nm5), \
	PRM_Default(CHOP_PARM_COUNT, CHOP_TAB), \
    }

#define CHOP_SWITCHER6(cnt1,nm1,cnt2, nm2,cnt3,nm3,cnt4,nm4,cnt5,nm5,cnt6,nm6) \
    static PRM_Default switcher[7] = \
    { \
	PRM_Default(cnt1, nm1), \
	PRM_Default(cnt2, nm2), \
	PRM_Default(cnt3, nm3), \
	PRM_Default(cnt4, nm4), \
	PRM_Default(cnt5, nm5), \
	PRM_Default(cnt6, nm6), \
	PRM_Default(CHOP_PARM_COUNT, CHOP_TAB), \
    }

enum CHOP_SampleMatch 	{ FIRST_RATE	= 0,
			  SAMPLE_MAX_RATE,
			  SAMPLE_MIN_RATE,
			  ERROR_IF_DIFF	};

#define	ARG_CHOP_SCOPE		(myParmCommonBase + 0)
#define ARG_CHOP_MATCH		(myParmCommonBase + 1)
#define ARG_CHOP_UNITS		(myParmCommonBase + 2)
#define ARG_CHOP_REALTIME	(myParmCommonBase + 3)
#define ARG_CHOP_UNLOAD		(myParmCommonBase + 4)
#define ARG_CHOP_EXPORT_PATH	(myParmCommonBase + 5)
#define ARG_CHOP_DCOLOR		(myParmCommonBase + 6)
#define ARG_CHOP_COLSTEP	(myParmCommonBase + 7)

//___________________________________________________________________________

class CHOP_API CHOP_Node : public OP_Network
{
public:

    virtual const char  	*getChildType() const;
    virtual const char  	*getOpType() const;

    virtual OP_OpTypeId  getChildTypeID() const;
    virtual OP_OpTypeId  getOpTypeID() const;
    static const char	*theChildTableName;

    virtual OP_DataType 	 getCookedDataType() const;
    static void		 	 buildOperatorTable(OP_OperatorTable &table);

    static OP_TemplatePair	 myTemplatePair;
    static OP_VariablePair	 myVariablePair;
    static PRM_Template		 myTemplateList[];
    static CH_LocalVariable	 myVariableList[];

    virtual int			 getAudio() const;
    virtual int			 getExport() const;
    virtual int			 setAudio(int on_off);
    virtual int			 setExport(int on_off);

    // CHOP networks need some logic to switch between using
    // output nodes, output flag nodes nodes. This function can
    // also be used to just return the Output CHOPs by passing false to
    // "fallback_to_flag".
    CHOP_Node			*getOutputChop(int outputidx,
					bool fallback_to_flag = true);

    //
    // cooking methods
    //

    virtual OP_ERROR		 cookMyChop(OP_Context &context) = 0;
    virtual void		 cookMyHandles(OP_Context &context);
    CHOP_HandleList		*getHandles(OP_Context &context);

    // cooked data access
    virtual const CL_Clip	*getClip(OP_Context *context = 0);
    virtual const CL_Clip	*getClipOutput(int outputidx, OP_Context *context = 0, OP_Node *add_interest=0);

    // Sets the CL_Clip instance. Should only be done on locked nodes.
    void			 setClip(CL_Clip *clip);

    virtual int64		 getMemoryUsage(bool inclusive) const;
    const CL_Clip		*inputClip(int idx, OP_Context &context);
    CL_Track			*getTrack(const char *name); // doesn't cook
    CL_Track			*getTrack(int idx); // doesn't cook
    fpreal		 	 getSampleRate();
    
    UT_Color			 getChopColor(unsigned int index = 0, const CL_Clip *clip=nullptr ) const;

    virtual CH_Channel		*getTrackChannel(const char *name);

    // Do global expansion on a pattern, and get a track list, and
    // corresponding node list of track owners.
    // If dependent given, addExtraInputs.                    
    void                 globTracks(const char     		*pattern,    
				    CL_TrackListC  		*tlist,  
				    UT_ValArray<OP_Node *>	*olist = 0,  
				    OP_GlobContext		*context = 0,
				    int				 do_cook = 0,
				    fpreal 			 t = 0,
				    OP_Node 			*dependent = 0);

    //
    // return the application frame rate
    //

    static fpreal		 getGlobalRate(void);

    static fpreal		 getGlobalStart();
    static fpreal		 getGlobalEnd();

    static fpreal		 getGlobalSampleStep();


    //
    // This method is called whenever a guide handle has been dragged
    virtual fpreal		 handleChanged(CHOP_Handle *handle, 
	    				       CHOP_HandleData *data);

    //
    // Notes interface
    int				 putNote(fpreal t, const char *);
    int				 putNote(fpreal t, int index);
    int				 putNote(int index, const char *);

    const char			*getNote(fpreal t0) const;
    const char			*getNote(int index) const;
    int				 getNotes(fpreal t0, fpreal t1,
					  CHOP_NoteList &clist,
					  CHOP_TimeList &tlist) const;

    // Edit interface
    OP_Channels			*getFitData();
    void			 resampleFromFit();
    void			 setFitParms(const CL_FitParms &parms);
    void			 getFitParms(CL_FitParms &parms) const;

    // This function will copy the fit channels to the
    // destination node channels
    int				 copyFitChannels();


    // Error & Warning reporting
    static void		 addError(int code, const char *msg = 0)
			 { UTaddError("CHOP", code, msg);}

    static void		 addWarning(int code, const char *msg=0)
                         { UTaddWarning("CHOP", code, msg);}

    // Returns true if matching condition is not met for source1
    // and false otherwise
    static bool		 hasChannelMatchFailure(PRM_Match match_by,
	PRM_MatchFailure match_failure, const CL_Clip * source1,
	const char *name1, const CL_Clip * source2, const char *name2);

    // An external hook to notify the user of export conflicts
    static void		 setConflictCallback(CHOP_ConflictCallback func,
					     void *data)
			 {
			    theConflictCallback = func;
			    theConflictData = data;
			 }

    // Set a string pattern for conflict resolution
    static void		 getConflictResolvePattern(UT_StringHolder &pattern)
			 {
			    pattern = theConflictResolvePattern;
			 }

    // Set a string pattern for conflict resolution
    static void		 setConflictResolvePattern(const char *pattern)
			 {
			    theConflictResolvePattern = pattern;
			 }

    // Extra flags
    virtual int		 setFlag(char flag, int onoff);

    // method to direct-manip the start of an ancestor from
    // a descendants handle
    virtual fpreal	 shiftStart(fpreal new_offset, fpreal t);

    //
    // must maintain overriding whether or not its being displayed
    // or referenced 
    virtual void         opChanged(OP_EventType reason, void *data=0);

    // Overriden by the Realtime CHOPs.
    virtual	 int usesRealtime()	{ return 0; }
    virtual	 int isRealtimeCook()	{ return 0; }
    virtual OP_ERROR doCookRealtime(OP_Context &context);

    virtual	void realtimeInit(fpreal t);
    virtual	void realtimeCleanup();
    
    // returns true if a range has been modified, plus that range.
    virtual int		getModifiedRange(int &first,int &last,int clear=1);

    static void		installCommands();
    
    // For realtime: only called by the maxslice command.
    static void		setMaxSliceSize(fpreal size) { myMaxRealtimeSlice = size; }
    static fpreal	getMaxSliceSize() { return myMaxRealtimeSlice; }
    static void		setTimeSliceDefault(int state) { myTimeSliceDefault = state; }
    static int		getTimeSliceDefault() { return myTimeSliceDefault; }

    static void		setSmartCook(int f) { mySmartCookFlag = f; }
    static int		getSmartCook() { return mySmartCookFlag; }
    
    //
    // Menu building routines
    // 

    static void			 buildEditorMenu(
				    void *, PRM_Name *, int,
				    const PRM_SpareData *, const PRM_Parm *);
    static void			 buildNetMenu(
				    void *, PRM_Name *, int,
				    const PRM_SpareData *, const PRM_Parm *);
    static void			 buildNodeMenu(
				    void *, PRM_Name *, int,
				    const PRM_SpareData *, const PRM_Parm *);

    //
    // Limit the number of lines generated in our info methods.
    //

    bool			 limitInfo() const
				 { return myLimitInfo; }
    void			 setLimitInfo(bool limit_info)
				 { myLimitInfo = limit_info; }

    // export mapping techniques
    //
    virtual void	 setMapping(int idx, int type, const char *label = 0);
    virtual int		 getMapping(int idx, const char *&label) const;

    virtual void	 getNodeSpecificInfoText(OP_Context &context,
				OP_NodeInfoParms &iparms);
    virtual void	 fillInfoTreeNodeSpecific(UT_InfoTree &tree, 
				const OP_NodeInfoTreeParms &parms);
    void		 saveMapping( UT_OStream &os ) const;
    bool		 loadMapping( UT_IStream &is );

    ///Get absolute width
    virtual fpreal	 getW() const;
    ///Get absolute height
    virtual fpreal	 getH() const;

    // Force matching of our exports to nodes and do any necessary
    // exports.  This method is called whenever a new node is added
    // or a node is renamed and there might be a new export.
    virtual void		 updateExports();

    // Generic implementation to return a parameter representing a track.
    // It simply calls the first input to do it.
    virtual bool	 findParmFromTrack(const OP_FollowChanRefsOptions& opt,
					   const char *trackname,
					   OP_NodeParmRefCycle &cycle,
					   OP_Node *&node, PRM_Parm *&parm,
					   int &vecidx);

    virtual int		 bumpSourceCount(int d);

    // evaluates transform tracks
    bool evaluateTransformTracks( OP_Context &context, UT_Matrix4D &m, const UT_StringHolder &trackname="", int *timedep=0 );

protected:
    CHOP_Node(OP_Network *parent, const char *name, OP_Operator *entry);
    virtual ~CHOP_Node();

    virtual void	 inputConnectChanged(int which_input);

    // notifier when this node is unlocked
    virtual void	 nodeUnlocked();

    // All external references must be removed by the time this function
    // returns!
    virtual void	 clearAndDestroy(void);

    virtual OP_ERROR	 preCook(OP_Context &context);
    virtual void	 postCook();
    
    virtual OP_ERROR	 cookMe(OP_Context &context);
    virtual OP_ERROR	 bypassMe (OP_Context &context, int &);
    virtual bool	 updateParmsFlags();

    void		 initCommonBaseParm();

    virtual void	 buildOpDependencies();

	// this method is called to alert this op that its dependency has
	// changed. if it's a name interest, then the old full path is given
    virtual void	 handleOpDependency( int referenced_op_id,
					     const OP_Dependency &op_dep,
					     OP_InterestType interest_type,
					     bool &need_cook,
					     const char *old_fullpath,
					     const char *old_cwd,
					     const char *old_chan_name);

    // This method is called when a channel that we reference has changed 
    // the way it is referenced.
    virtual void	 handleChanNameDependency(const UT_String &chan_token,
						  const UT_String &chan_alias,
						  const char *old_chan_name,
						  bool &need_cook);

    // This method takes an expand array with a pattern of channels and 
    // generates a new pattern based on the renaming of a channel that was
    // guaranteed to be in the pattern (old_chan_name should be matched by
    // chan_array).  chan_token and chan_alias are the two new ways of
    // of accessing the old channel name.  If either of the new names can
    // be matched by the existing pattern in chan_array, then new_pattern
    // will not contain a valid string (that is !new_pattern.isstring()).
    static void		 renameChanReference(UT_ExpandArray &chan_array, 
					     const UT_String &chan_token,
					     const UT_String &chan_alias,
					     const char *old_chan_name,
					     bool &need_cook,
					     UT_String &new_pattern);

    // Adds dependencies to the parms/channels found in the supplied lists.
    void			 addParmReferences(
					const UT_Array<opParmData> *plist,
					const CH_ChannelList *clist);

    //
    // Override these methods to enable/disable the common parameters
    //

    virtual int          usesScope() const;
    virtual int          usesSampleMatch() const;
    virtual int          usesUnits();

    //
    // Input scoping methods
    //

    void         	 getScopedTracks(const CL_Clip *, CL_TrackListC &);
    int          	 isScoped(const UT_StringRef &name);
    int          	 allScoped();

    //
    // reset methods

    void		 destroyClip(void);
    void		 destroyHandles(void);

    //
    // reset the data and set the correct sample rate

    const CL_Clip	*copyInputAttributes(OP_Context &context, 
	    				     int destroy = 1);

    //
    // copy us exactly from the input, tracks and all
    // options to copy data samples and slerp info as well
    //

    const CL_Clip	*copyInput(OP_Context &context, int idx,
	    			   int data, int slerps);


    //
    // Conversion methods
    //
    
    fpreal		 toSampleLength(fpreal len,
				    CL_Unit unit = UNIT_NONE,
				    int end_width = 0) const;
    fpreal		 toSample(fpreal val,
	    			  CL_Unit unit = UNIT_NONE,
				  int end_width = 0) const;
    fpreal		 toUnitLength(fpreal len,
				CL_Unit unit = UNIT_NONE,
				int end_width = 0) const;
    fpreal		 toUnit(fpreal index,
	    			CL_Unit unit = UNIT_NONE,
				int end_width = 0) const;


    //
    // toStandard take (s,e) and converts it to absolute indices
    // fromStandard does the opposite

    fpreal		 toStandardStart(fpreal is, OP_Context context,
					 int relative, int ref_input = 1);

    fpreal		 toStandardEnd(fpreal is, OP_Context context,
				       int relative, int ref_input = 1);

    fpreal		 fromStandardStart(fpreal os, OP_Context context,
					   int relative, int ref_input,
					   CL_Unit unit = UNIT_NONE);

    fpreal		 fromStandardEnd(fpreal oe, OP_Context context,
				         int relative, int ref_input,
					 CL_Unit unit = UNIT_NONE);


    //
    //	I/O methods
    //

    virtual int		 saveCookedData(const char *, OP_Context &);
    virtual int		 saveCookedData(std::ostream &os, OP_Context &, int binary);
    virtual bool	 loadCookedData(UT_IStream &is, const char*);
    virtual const char	*getFileExtension(int binary) const;
    virtual void	 deleteCookedData();

    virtual OP_ERROR	 save(std::ostream &os, const OP_SaveFlags &flags,
			      const char *pathPrefix,
			      const UT_String &name_override = UT_String());
    virtual bool	 load(UT_IStream &is, const char *extension,
			      const char *path=0);

    /// @brief Mark all inputs as dirty so that useInputSource() will return
    /// true upon its next call. This is the default.
    void		 resetChangedSourceFlags();

    /// @brief Mark the given input as used for cooking by this node.
    ///
    /// This updates source flags to mark the input as now clean, returning
    /// whether the input has changed since the last time this function was
    /// called. If @c force is true, then we treat the input as always dirty.
    /// Returns @c false if the input has errors.
    bool		 useInputSource(unsigned idx, bool &changed, bool force)
			 {
			     return myInputChangeHelper.useInput(
						*this, idx, changed, force);
			 }

    //
    // Common page parameter access
    //

    const UT_String	&SCOPE_STRING()
                         {  if(myScopeStringDirty)
			    {
				evalString(myScopeString,ARG_CHOP_SCOPE, 0, 0);
				myScopeStringDirty = false;
			    }
			    return myScopeString;}

    CHOP_SampleMatch	 SAMPLE_MATCH() const
                         { return (CHOP_SampleMatch)
			     evalInt(ARG_CHOP_MATCH, 0, 0); }

    CL_Unit	 	 UNITS() const
                         { return (CL_Unit)
			     evalInt(ARG_CHOP_UNITS, 0, 0); }

    int			 REALTIME() const
			 { return evalInt(ARG_CHOP_REALTIME,0,0); }

    int			 UNLOAD() const
			 { return evalInt(ARG_CHOP_UNLOAD,0,0); }
    int			 UNLOAD(int thread) const
			 { return evalIntT(ARG_CHOP_UNLOAD,0,0,thread); }

    void		 EXPORT_PATH(UT_String &str) const
                         { evalString(str, ARG_CHOP_EXPORT_PATH, 0, 0); }

    short		 myParmBase;		// parameter offsets
    short		 myParmCommonBase;

    CL_Clip		*myClip;		// chop data..
    CHOP_HandleList	 myHandles;
    fpreal		 myHandleCookTime;


    /// Retrieve the animation range (in samples) from a (range, start, end)
    /// parm triplet. If they are NULL, then we use the standard
    /// CHOP_RangeName, CHOP_StartName, CHOP_EndName parms. It assumes that
    /// these parameters exist on the current node in the correct types.
    void		 getParmIntervalInSamples(
			    fpreal t, fpreal &start, fpreal &end,
			    const char *range_parm = 0,
			    const char *start_parm = 0,
			    const char *end_parm = 0);

    void		 upgradeRangeParm(
			    const char *old_version,
			    const char *cur_version,
			    const char *range_parm = 0);

    //
    // method to retrieve a valid index interval, or the
    // current unprocessed index interval

    static void		 getStaticInterval(fpreal t, int &istart, int &iend,
					   int &last_iend, fpreal start, 
					   fpreal end, int current, 
					   const CL_Clip *clip);

    virtual int		 getNetOverviewColor( UT_Color &color );

    static void		 buildScopeMenu(
			    void *data, PRM_Name *menuEntries, int thelistsize,
			    const PRM_SpareData *spare, const PRM_Parm *);

    //
    // Override this method if this chop exports in a specific manner
    //

    virtual const orUnitList	*getOverrideUnits() const
				 { return 0; }
    // stealOverrideUnits() will steal the allocated data from units
    virtual void		 stealOverrideUnits(orUnitList & /*units*/)
				 { }

    //
    // Menu handling routines
    // 

    void			 getMenuPath(UT_String &base,
					     const UT_String &edit,
					     const UT_String &net,
					     const UT_String &node,
					     int add_inputs = 1);

    virtual void		 EDITOR_PATH(UT_String &str);
    virtual void		 NET_PATH(UT_String &str);

public:
    int getFirstScopedTrack(const CL_Clip *, const CL_Track **track);

    /// Retrieve the animation range (in samples) from a (range, start, end)
    /// parm triplet. If they are NULL, then we use the standard
    /// CHOP_RangeName, CHOP_StartName, CHOP_EndName parms. It assumes that
    /// these parameters exist on the current node in the correct types.
    /// This methods also doesn't set flags().setTimeDep() if the range mode is CURRENT_FRAME.
    /// It returns the timedep as an argument.
    void		 getParmIntervalInSamples(
			    fpreal t, fpreal &start, fpreal &end, bool &timedep,
			    const char *range_parm = 0,
			    const char *start_parm = 0,
			    const char *end_parm = 0
			    );

    const CL_Clip	*inputClip(int idx, OP_Context &context, bool add_extrainput );
    const CL_Clip	*inputClip(int idx, OP_Context &context, bool add_extrainput, bool force_recook_timedep);
    const CL_Clip	*inputClip(int idx, OP_Context &context, bool add_extrainput, bool force_recook_timedep, CHOP_Node **out_chop);

    virtual bool isUsingCurrentFrameForVexTransformContext() const {return true;}

private:    // methods

    // This method is called whenever a cook is called causing a
    // cookMyChop or the context.myCookOverride is set to 1
    // on a getClip() which may also call cook.

    void	 	 updateOverrides();
    static void	 	 updateConflicts(void *data, int replace);

    void		 saveOverrideUnits(UT_OStream &os);
    bool		 loadOverrideUnits(UT_IStream &is);

    void		 destroyScopePatterns();

    fpreal		 getInputRate(int input_idx);
    static void		 initializeExpressions();
    void		 notifyOverrides(const OP_NodeList &list,
	    				 const UT_IntArray &indices);
    void		 getFitInterval(int &istart, int &iend);

    OP_ERROR		 doCook(OP_Context &context, int bypass);

    void		 buildChannelMenu(PRM_Name *menuEntries,
					  int themenusize);

    bool		 loadFitData(UT_IStream &is, const char *path);
    void		 destroyAllFitData();
    void		 createFitCollection();
    void		 destroyFitCollection();

    bool		 haveSeenModification( long counter );


private:    // data

    bool			 myRequireUnload;

    // These parameters reduce the number of times a scope
    // string must be parsed to traverse arrays

    UT_ExpandArray		 myScopePatterns;
    char			 myScopeAll;

    bool			 myScopeStringDirty;
    UT_String			 myScopeString;

    static PRM_ChoiceList	 scopeMenu;
    CHOP_Notes			 myNotes;

    OP_Channels			*myFitCollection;
    CL_Clip			*myFitClip;
    CL_FitParms			*myFitParms;
    fpreal			 myFitError2;
    int				 myFitDirty;
    CHOP_Fitter			*myFitter;
    orPath			*myOrPathParms;
    int				 myRealtimeCookState;

    static fpreal		 myMaxRealtimeSlice;
    static int			 myTimeSliceDefault;
    static int			 mySmartCookFlag;

    static bool			 myLimitInfo;

    OP_VERSION			 myHandleVersionParms;

    static CHOP_ConflictCallback	 theConflictCallback;
    static void				*theConflictData;
    static UT_StringHolder		 theConflictResolvePattern;

    char			 myConflictPending;

    OR_Mapping			*myMapping;
    
    long			 myLastModification;

    OP_InputChangeHelper	 myInputChangeHelper;

    // Undo classes should have access to everything so that they can undo
    friend class CHOP_UndoFitDestroy;
};

#undef FLT_PARM
#undef INT_PARM
#undef STR_PARM
#undef FLT_SET

#endif
