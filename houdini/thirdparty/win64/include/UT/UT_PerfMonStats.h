/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PerfMonStats.h (UT Library, C++)
 *
 * COMMENTS:
 *
 *		Collection of performance monitor statistics 
 *		for a particular object.
 */

#ifndef __UT_PerfMonStats__
#define __UT_PerfMonStats__

#include "UT_API.h"

#include "UT_JSONWriter.h"
#include "UT_ValArray.h"
#include "UT_String.h"
#include "UT_Pair.h"
#include "UT_StringArray.h"
#include "UT_StringMap.h"
#include "UT_PerfMonTypes.h"

class UT_PerfMonStatDefinition;
class UT_PerfMonStats;
class UT_WorkBuffer;
typedef UT_ValArray<UT_PerfMonStatDefinition *> UT_PerfMonStatDefinitions;
typedef UT_ValArray<UT_PerfMonStats *> UT_PerfMonStatsList;
typedef UT_ValArray<const UT_PerfMonStats *> UT_ConstPerfMonStatsList;

static const int UT_PERFMON_STAT_WALL_TIME = 0;

/// Definition of a collected statistic.
class UT_API UT_PerfMonStatDefinition
{
public:
    UT_PerfMonStatDefinition();
    UT_PerfMonStatDefinition(
	const char *name, 
	UT_PerfMonCategory category,
	UT_PerfMonStatType type, 
	bool is_diff=false);
    UT_PerfMonStatDefinition(const UT_PerfMonStatDefinition &src_def);
    ~UT_PerfMonStatDefinition();

    const UT_StringHolder &name() const { return myName; }
    UT_PerfMonCategory	 category() const;
    UT_PerfMonStatType	 type() const;
    bool		 isDiff() const;
    void		 setName(const char *name);
    void		 setCategory(UT_PerfMonCategory category);
    void		 setType(UT_PerfMonStatType type);
    void		 setIsDiff(bool is_diff);
    void		 save(UT_JSONWriter &writer) const;

    bool		 operator==(const UT_PerfMonStatDefinition &def) const;

    /// Return the name used for the total time stats.
    static const char	*totalTimeStatName();

    /// Return the name used for the memory growth stats.
    static const char	*memoryGrowthStatName();

    static void		 getRootStatName(
			    const UT_PerfMonStatDefinition &def, 
			    UT_WorkBuffer &root_stat_name);
    static void		 getSelfStatName(
			    const char *root_stat_name, 
			    UT_WorkBuffer &self_stat_name);
    static void		 getCountStatName(
			    const char *root_stat_name,
			    UT_WorkBuffer &count_stat_name);
    static void		 getAverageStatName(
			    const char *root_stat_name,
			    UT_WorkBuffer &avg_stat_name);
    static void		 getMinStatName(
			    const char *root_stat_name,
			    UT_WorkBuffer &min_stat_name);
    static void		 getMaxStatName(
			    const char *root_stat_name,
			    UT_WorkBuffer &max_stat_name);
    static void		 getDiffStatName(
			    const char *src_stat_name,
			    UT_WorkBuffer &diff_stat_name);

private:

    /// Strip out the given suffix from the stat name
    /// and store the result in `result`.
    static void		 stripStatSuffix_(
			    const char *stat_name,
			    const char *stat_suffix,
			    UT_WorkBuffer &result);


    UT_StringHolder	 myName;
    UT_PerfMonCategory	 myCategory;
    UT_PerfMonStatType	 myType;
    bool		 myIsDiff;
};

/// A collection of statistics for a particular object (i.e. node, viewport).
class UT_API UT_PerfMonStats
{
public:
    UT_PerfMonStats();
    UT_PerfMonStats(const UT_PerfMonStats *stats1,
		    const UT_PerfMonStats *stats2 = NULL,
		    const char* title1 = NULL,
		    const char* title2 = NULL);
    UT_PerfMonStats(
	const char *name, const char *path, const char *icon, 
	UT_PerfMonObjectType type, UT_PerfMonStats *parent=NULL);
    ~UT_PerfMonStats();

    /// Return the name of the object that the stats apply to.
    const char		    *name() const;

    /// Set the name of the object that the stats apply to.
    void		     setName(const char *name);

    /// Return the full path of the object that the stats apply to.
    const char		    *path() const;

    /// Set the path of the object that the stats apply to.
    void		     setPath(const char *path);

    /// Return extra information about this stats object.
    const char		    *extraInfo() const;

    /// Set extra information pertaining to this stats object.
    void		     setExtraInfo(const char *extra_info);

    /// Return the type of the object that the stats apply to.
    UT_PerfMonObjectType     type() const;

    /// Set the type of the object that the stats apply to.
    void		     setType(UT_PerfMonObjectType type);

    /// Return the icon name associated with the collection of stats.
    /// Return NULL if no such icon exists.
    const char		    *icon() const;

    /// Set the object icon.
    void		     setIcon(const char *icon);

    /// Return true if the icon has been set.
    /// Note that this does not mean that the icon is not NULL,
    /// just that the object's icon has been determined.
    bool		     isIconSet() const;

    /// Set the parent stat object.
    void		     setParent(UT_PerfMonStats *parent);

    /// Return the object that this object is parented to.
    /// Return NULL if this is the root object.
    UT_PerfMonStats	    *parent() const;

    /// Append a child stat object.
    void		     appendChild(UT_PerfMonStats *child);

    /// Return a list of the object's children.
    const UT_PerfMonStatsList	&children() const;

    /// Return a list of stat names that are tracked by this object.
    const UT_StringArray    &stats() const;

    /// Return true if the object contains the given stat.
    /// Return false otherwise.
    bool		     hasStat(const char *stat_name) const;

    /// Return the value of the given stat.
    /// Return -1.0 if the stat does not exist.
    /// Always call hasStat() before value().

    /// This method will always return the value over all threads and frames
    fpreal		     value(const UT_StringRef &stat_name) const;

    /// This method returns the values over the given frame range and thread id
    fpreal		     value(
				const char *stat_name, 
				int first_frame,
				int last_frame,
				int thread_id = UT_PERFMON_STAT_WALL_TIME) 
				const;

    /// Return a list of values separated by thread.
    UT_ValArray<UT_Pair<int, fpreal> > threadedValue(
			    const char* stat_name,
			    int first_frame= UT_PERFORMANCE_INVALID_FRAME, 
			    int last_frame = UT_PERFORMANCE_INVALID_FRAME) const;

    /// Sum the value for the given stat name and thread id for ALL frames.
    fpreal		valueForThread(const char *stat_name, 
					int thread_id) const;

    /// Set the specified stat to the given value.
    void		     setValue(
				const UT_PerfMonStatDefinition *stat_def, 
				int thread_id,
				int frame, 
				fpreal stat_value);

    /// Set the specified stat to the given value.
    /// This method should be called for stats that are independent
    /// of frames and threads (i.e. avg, min, max).
    void		     setValue(
				const UT_PerfMonStatDefinition *stat_def,
				fpreal stat_value);

    /// Add the given value to the current value of the specified stat.
    /// Use this method if you want to keep track of delta values that
    /// can be used later to generate statistics in updateStatsFromDeltas().
    /// Otherwise, call setValue() instead.
    void		     addToValue(
				const UT_PerfMonStatDefinition *stat_def,
				int thread_id,
				int frame,
				fpreal stat_value);

    /// Return the normalized percentage of the given time value
    /// when compared to the maximum time value reported by the
    /// stat object's children.
    /// Return 0 if the normalized percentage cannot be calculated.
    fpreal		     normalizedPercentage(
				const char *time_stat_name, 
				fpreal time_value) const;
 
    /// Save the stats using the specified JSON writer.
    void		     save(UT_JSONWriter &writer) const;

    /// Update the cumulative and total time stats using the time deltas.
    /// This method is intended to be called only from UT_PerfMonProfile 
    /// when it is updating the profile stats.
    void		     updateStatsFromDeltas();
 
    /// Return true if the given stat object is an artificial object,
    /// that is, it is not really an object (like a node or a viewport)
    /// but can be attached to a real object.  For example, '{drawlist}' 
    /// is an artificial stat object that is a part of either a node, 
    /// a viewport or on its own.
    static bool		     isArtificialStatObject(const char *stat_obj_path);

    /// Convert the given stat object name to one that uses notation
    /// that denotes that it is an artificial object.
    static void		     convertToArtificialStatObjectName(
				UT_WorkBuffer &artificial_obj_name,
                                const char *stat_obj_name);

    /// Convert the given stat object name to one that
    /// does not have notation which denotes it as an artificial object.
    static void		     convertToRealStatObjectName(
				UT_String &artificial_stat_obj_name);

    /// Build a stat object path using the given real object path
    /// and artificial object name.
    static void		     buildObjectPath(
				UT_WorkBuffer &path,
				const char *real_object_path,
				const char *artificial_object_name);

    /// Split the given object path into its real object path
    /// and its artificial name.  If the path does not have a real object path,
    /// then `real_object_path.isstring()` will be false.
    /// Likewise, if the path does not have an artificial
    /// name, then `artificial_object_name.isstring()` will be false.
    static void		     splitObjectPath(
				const char *path,
				UT_String &real_object_path,
				UT_String &artificial_object_name);

    /// Split the given object path into its parent path
    /// and object name.  If the path does not have a parent,
    /// then `parent_path.isstring()` will be false.
    static void		     splitParentPath(
				const char *path,
				UT_String &parent_path,
				UT_String &object_name);

    /// Return the title for the root statistics.
    static const char	    *rootStatsTitle();

    /// Return the title for the unaccounted statistics.
    static const char	    *unaccountedStatsTitle();

private:

    // Simple structure that stores a change for a time stat.
    struct ut_StatDelta
    {
	ut_StatDelta()
	{
	    statDef = NULL;
	    threadId = 0;
	    frameNum = 0;
	    deltaValue = 0.0;
	}

	ut_StatDelta(
	    const UT_PerfMonStatDefinition *stat_def, int thread_id, 
	    int frame_num, fpreal delta_value)
	{
	    statDef = stat_def;
	    threadId = thread_id;
	    frameNum = frame_num;
	    deltaValue = delta_value;
	}

	ut_StatDelta(const ut_StatDelta &src)
	{
	    statDef = src.statDef;
	    threadId = src.threadId;
	    frameNum = src.frameNum;
	    deltaValue = src.deltaValue;
	}

	const UT_PerfMonStatDefinition	*statDef;
	int				 threadId;
	int				 frameNum;
	fpreal				 deltaValue;
    };

    /// Set the specified global stat to the given value.
    void		     setGlobalValue_(const UT_StringHolder &stat_name,
                                             fpreal stat_value);

    /// Set the specified stat to the given value.
    /// Replace = false adds on to any existing value
    void		     setValue_(const UT_StringHolder &stat_name, 
				       int thread_id, int frame, 
                                       fpreal stat_value, bool replace = true);

    /// Save the global stat values to the given JSON writer.
    void		     saveGlobalValues_(UT_JSONWriter &writer) const;

    /// Save the stat values to the given JSON writer.
    void		     saveValues_(UT_JSONWriter &writer) const;

    /// Helper method for updateStatsFromDeltas().
    /// Update stats with the specified time delta value.
    void		     updateStatsFromTimeDelta_(
				const ut_StatDelta *delta, 
				const char *total_time_stat_name, 
				const char *total_self_time_stat_name, 
				UT_StringArray &avg_time_stats_to_update,
				UT_StringArray &max_stats_to_update);

    /// Helper method for updateStatsFromDeltas().
    /// Update stats with the specified memory delta value.
    void		     updateStatsFromMemoryDelta_(
				const ut_StatDelta *delta,
				UT_StringArray &max_stats_to_update);

    /// Helper method for updateStatsFromDeltas().
    /// Update the average times for the given list of stats.
    void		     updateAverageTimeStats_(
				const UT_StringArray &stat_names);

    /// Helper method for updateStatsFromDeltas().
    /// Update the maximum child stat values on the parent 
    /// for the given list of stats.
    void		     updateParentMaxChildStats_(
				const UT_StringArray &stat_names);

    /// Helper method for updateStatsFromDeltas().
    /// Update the object's maximum child stat value for the given stat
    /// using the suggested new maximum value.  Do nothing if the
    /// suggested value is less than or equal to the object's current
    /// maximum value.
    void		     updateMaxChildStat_(
				const UT_StringHolder &stat_name,
				fpreal suggested_max_value);

    UT_String		     myName;
    UT_String		     myPath;
    UT_String		     myIcon;
    bool		     myIsIconSet;
    UT_PerfMonStats	    *myParent;
    UT_PerfMonStatsList      myChildren;
    UT_String		     myExtraInfo;

    /// myStats is  a three level hash table.
    ///	    The first level is indexed by Thread ID
    ///	    The second level is indexed by Frame Number
    ///	    The third level is indexed by Stat Name
    typedef UT_StringMap<fpreal> StatHashTable;
    typedef UT_Map<int, StatHashTable> FrameHashTable;
    typedef UT_Map<int, FrameHashTable> ThreadHashTable;
    ThreadHashTable myStats;

    /// myGlobalStats contains the values for statistics
    /// that are independent of frames and threads (i.e. avg, min, max).
    StatHashTable           myGlobalStats;

    /// myMaxChildStats contains the largest values for statistics
    /// recorded by the children.  This is useful when calculating
    /// an object's stat value as a normalized percentage.
    StatHashTable           myMaxChildStats;

    UT_StringArray	     myStatNames;
    UT_PerfMonObjectType     myType;

    /// List of stat value changes that have to be propagated
    /// to the object's ancestors.
    UT_ValArray<ut_StatDelta>	 myStatDeltas;
};

#endif

