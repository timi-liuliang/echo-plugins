/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PerfMonProfile.h (UT Library, C++)
 *
 * COMMENTS:
 *
 *		Performance monitor profile.
 */

#ifndef __UT_PerfMonProfile__
#define __UT_PerfMonProfile__

#include "UT_API.h"

#include "UT_PerfMonEvent.h"
#include "UT_PerfMonRecordOptions.h"
#include "UT_PerfMonTypes.h"
#include "UT_RWLock.h"
#include "UT_String.h"
#include "UT_StringArray.h"
#include "UT_StringMap.h"
#include "UT_PerfMonStats.h"

class UT_PerfMonStats;
class UT_PerfMonEvent;
class UT_PerfMonTimedEvent;

class UT_API UT_PerfMonProfile
{
public:
    /// `id` is the unique id generated by the performance monitor for
    /// this profile.  `title` is a user-readable profile title.
    UT_PerfMonProfile(int id, const char *title, 
	const UT_PerfMonRecordOptions *options=NULL);

    ~UT_PerfMonProfile();

    /// ********************************************************
    /// GENERAL.
    /// ********************************************************

    /// Return the profile id that was assigned by the performance monitor.
    int			     id() const;

    /// Return the user-readable profile title.
    const char		    *title() const;

    void		    uiFriendlyTitle(UT_String &title) const;

    /// Set the profile title.
    void		     setTitle(const char *title);

    /// ********************************************************
    /// EVENT RECORDING.
    /// ********************************************************

    /// Start recording events.
    void		     startRecording();

    /// Stop event recording.
    void		     stopRecording();

    /// Add the given event to the log.
    /// The performance monitor is responsible for the deletion of the event.
    void		     addEvent(UT_PerfMonEvent *event);

    /// Collect data from the event.  The data contributes to the profile's
    /// statistics.
    void		     collectDataFromEvent(UT_PerfMonEvent *event);

    /// Return true if the profile is active.
    /// An active profile is one that is still interested in events
    /// but may not be recording at the moment (i.e. if it is paused).
    bool		     isActive() const;

    /// Turn on/off event recording.
    void		     enableRecording(bool enable);

    /// Return true if the profile is recording.
    bool		     isRecording() const;

    /// Return the recording options.
    const UT_PerfMonRecordOptions &recordOptions() const;

    /// Set the recording options.
    void		     setRecordOptions(
				const UT_PerfMonRecordOptions &options);

    /// Return the minimum frame recorded
    int			     getMinFrame() const;
    
    /// Return the maximum frame recorded
    int			     getMaxFrame() const;

    /// Return whether the current profile is a diff profile, for the purposes 
    /// of rendering differently in OPUI
    bool		     getIsDiff() const;

    /// Return a list of stat definitions registered with the profile.
    /// Return NULL if the profile has no stat definitions.
    const UT_PerfMonStatDefinitions &statDefinitions() const;

    /// Return a list of stat names that belong to the given stat category.
    /// Return NULL if `category` is not a valid enum value.
    const UT_StringArray    *statsByCategory(
				UT_PerfMonCategory category) const;

    /// Return the stat definition with the given name.
    /// Return NULL if no such definition has been registered with the profile.
    const UT_PerfMonStatDefinition *getStatDefinition(
					const char *stat_name) const;

    /// Return the set of stats collected by the profile.
    /// Return NULL if the profile has not started or if the profile
    /// is still recording.
    const UT_PerfMonStats   *stats() const;
    
    /// Calculate percent changes on a per stat basis.
    fpreal		    statsPercentValue(const UT_PerfMonStats* stats,
						const char *stat_name) const;

    fpreal		    statsPercentValue(const UT_PerfMonStats* stats,
				const char *stat_name, 
				int first_frame,
				int last_frame,
				int thread_id = UT_PERFMON_STAT_WALL_TIME) const;

    fpreal		    diffStatsPercentValue(const UT_PerfMonStats* stats,
				const char *stat_name) const;

    fpreal		    diffStatsPercentValue(const UT_PerfMonStats* stats,
				const char *stat_name, 
				int first_frame,
				int last_frame,
				int thread_id = UT_PERFMON_STAT_WALL_TIME) const;

    fpreal		    getMaxStatValue(const char * stat_name) const;

    /// Override the generated stats (if any) in the profile
    /// with the specified stats.  The profile takes ownership of
    /// the stat objects and the stat definitions.
    /// `min_frame` and `max_frame` describe the frame range that the stats
    /// reside in.
    void		     setStats(
				UT_PerfMonStats *stats, 
				const UT_PerfMonStatDefinitions &stat_defs,
				int min_frame, int max_frame);
    
    void		     createDiffStats(const UT_PerfMonProfile *profile1,
					     const UT_PerfMonProfile *profile2);

    const UT_PerfMonStats   *getStatsByPath(const char *path) const;

    const char		    *getDiffProfileTitle(int index) const;

    /// Save the collected stats to disk.
    /// Return true if the save succeeded.  Return false otherwise.
    bool		     save(const char *file_path) const;

    /// Export the collected stats in CSV format.
    /// Return true if the export succeeded.  Return false otherwise.
    bool		     exportStatsAsCSV(const char *file_path) const;

    /// Clear recorded stats in the profile.
    void		     clearStats();

private:
    /// Clear out contents of the profile and delete any objects
    /// that it owns.
    void		     clear_();

    /// Create a new, empty root stats object.
    /// The caller of this method takes ownership of the new object.
    UT_PerfMonStats	    *newRootStats_() const;

    /// Helper method for updateStatsFromEvents().
    /// Update stats from the given frame event.
    void		     updateStatsFromTimedFrameEvent_(
				UT_PerfMonTimedEvent *event);

    /// Helper method for updateStatsFromEvents().
    /// Update stats from the given event and its child events.
    void		     updateStatsFromTimedEvent_(
				UT_PerfMonTimedEvent *event);

    /// Helper method for updateStatsFromEvents().
    /// Update stats from the given memory event.
    void		     updateStatsFromMemoryEvent_(
				UT_PerfMonEvent *event);

    /// Return the specified stat object.
    /// Return NULL if no such object exists.
    UT_PerfMonStats	    *getStats_(const char *name,
				UT_PerfMonObjectType object_type) const;

    /// Return the specified stat object.
    /// Create a new object if it does not exist.
    UT_PerfMonStats	    *getOrCreateStats_(const char *name,
				UT_PerfMonObjectType object_type,
				const char *object_icon);

    /// Return the specified category stat for the given object type.
    /// Create a new stat object if it does not exist.
    UT_PerfMonStats	    *getOrCreateCategoryStats_(
				UT_PerfMonObjectType type);

    /// Helper method for createDiffStats().
    /// Create diff stats for the category stats that appear
    /// in the given 2 source profiles.
    void		     createDiffStatsForCategories_(
				const UT_PerfMonProfile *profile1, 
				const UT_PerfMonProfile *profile2,
				const char *profile_title1, 
				const char *profile_title2);

    /// Helper method for createDiffStats().
    /// Create the diff stats for the given 2 source category stats.
    /// Register the diff stats with the profile.
    void		     createCategoryDiffStats_(
				const UT_PerfMonStats *src_category_stats1,
				const UT_PerfMonStats *src_category_stats2,
				const char *profile_title1,
				const char *profile_title2);

    /// Helper method for createDiffStats().
    /// Return a list of stat objects that need diff stats generated for them.
    /// Only the stat object's path and type are used from the list.
    /// Parent stat objects always appear before their children in the list.
    void		     getStatsThatNeedDiffing_(
				const UT_PerfMonProfile *profile1,
				const UT_PerfMonProfile *profile2,
				UT_ConstPerfMonStatsList &stats_need_diffing) 
				const;

    /// Helper method for createDiffStats().
    /// Create a diff stats for the 2 given sources and register
    /// the diff stats with the profile.
    void		     createDiffStats_(
				const UT_PerfMonStats *src_stats1, 
				const UT_PerfMonStats *src_stats2,
				const char *profile_title1, 
				const char *profile_title2);

    /// Register the stat definitions from the 2 given source profiles.
    /// The definitions are registered as diff stat definitions.
    void		     registerDiffStatDefs_(
				const UT_PerfMonProfile *src_profile1,
				const UT_PerfMonProfile *src_profile2);

    /// Helper method for updateStatsFromEvents().
    /// Update the collection of statistics with the given event.
    void		     updateStatsFromTimedEvent_(
				UT_PerfMonStats *stats,
				const char *event_name, 
				UT_PerfMonCategory category,
				fpreal event_time,
				int thread_id, 
				int frame);
    
    /// Keep a record of largest seconds values for each stat-type across
    /// all frames. Used for diff stats and diff stat bar rendering.
    void updateMaxStats_(fpreal val, const char * stat_name);
 
    /// Helper function for diffStatsPercentValue
    void		    getDiffIndices_(int &index1, int &index2, 
					    const UT_PerfMonStats *stats, 
					    const char *stat_name) const;

    /// Register the given stat definition.
    /// If the name of the stat definition is already registered,
    /// then don't do anything.
    /// Returns the registered stat definition.
    const UT_PerfMonStatDefinition *
    registerStat_(const char *name, UT_PerfMonCategory category,
                  UT_PerfMonStatType type, bool is_diff = false);

    /// Update the minimum and maximum frames registered in this profile
    /// after examining the given frame number.
    void		     updateMinAndMaxFrames_(int frame);

    /// Set the minimum frame to the given number.
    void		     setMinFrame_(int frame);

    /// Set the maximum frame to the given number.
    void		     setMaxFrame_(int frame);

    int			     myId;
    int			     myMinFrame;
    int			     myMaxFrame;
    UT_String		     myTitle;
    bool		     myIsActive;
    bool		     myIsRecording;
    bool		     myIsDiff;
    UT_PerfMonRecordOptions  myRecordOptions;
    mutable UT_PerfMonStats *myStats;
    UT_PerfMonStats	    *myCategoryStats[UT_PERFMON_NUM_OBJECT_TYPES];
    UT_StringMap<UT_PerfMonStats *> myStatsTable[UT_PERFMON_NUM_OBJECT_TYPES];
    // maps UT_Strings to fpreals
    UT_StringMap<fpreal>   *myMaxStatValues;
    UT_String		    myDiffProfile0;
    UT_String		    myDiffProfile1;

    // Table of statistics contained within the profile.
    // Maps stat names to the stat definitions (i.e. name, type, etc.).
    UT_StringMap<UT_PerfMonStatDefinition *> myStatDefsTable;
    UT_PerfMonStatDefinitions	*myStatDefs;

    // Table of statistics contained within the profile.
    // Groups the statistics by category.
    UT_Array<UT_StringArray>	 myStatsByCategory;
};

typedef UT_ValArray<UT_PerfMonProfile *> UT_PerfMonProfileList;

#endif
