/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_CaptureOverrideData.h (GEO Library, C++)
 *
 * COMMENTS:	Class to store overrides for point capture weight attributes.
 */

#ifndef __GEO_CaptureOverrideData_h__
#define __GEO_CaptureOverrideData_h__

#include "GEO_API.h"

#include <GA/GA_AIFIndexPair.h>
#include <GA/GA_AIFStringTuple.h>
#include <GA/GA_AttributeRef.h>
#include <GA/GA_Types.h>

#include <UT/UT_Array.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_VectorTypes.h>


class UT_IStream;
class GA_PointGroup;
class GEO_Detail;
class GEO_CaptureOverrideDelta;
struct GEO_CaptureBoneStorage;

class geo_CaptureOverridePointData;

typedef int (*GEO_CaptureRegionDataCB)( void *user_data,
		    char *full_cregion_path,
		    char *relative_cregion_path,
		    GEO_CaptureBoneStorage &tube_data );

class GEO_API GEO_CaptureOverrideData
{
public:
    GEO_CaptureOverrideData();
    virtual ~GEO_CaptureOverrideData();
    
    void	destroy();

    void	getOpPaths( UT_Array<const char *> &paths );
    void	getOpPathsCopy( UT_Array<char *> &paths );
    void	setOpPaths( const UT_Array<char *> &paths );
    int		changeOpPaths( const char *old_name,
			       const char *new_name );
    void	changeCaptureRegionRoot( const char *old_root,
					 const char *new_root );
    void	refreshCaptureFrameInfo( const GEO_Detail &gdp,
					 GEO_CaptureRegionDataCB get_region,
					 void *user_data );

    // Apply the stored overrides to the given detail.
    void	applyOverrides(GEO_Detail &gdp);

    // The following pair of methods is for either merging in the
    // specified changes, or undoing the previous merge of those
    // changes.
    void	mergeCaptureWeightOverridesDelta(
		    GEO_CaptureOverrideDelta &delta );
    void	undoCaptureWeightOverridesDelta(
		    const GEO_CaptureOverrideDelta &delta );

    bool	overrideExists( int point_num ) const;

    // Compute the memory usage of this object (in bytes).
    int64	getMemoryUsage() const;

    int		save( std::ostream &os, int binary ) const;
    bool	load( UT_IStream &is );

private:    // methods

    // The following method converts the specified override to indices
    // in our local table and applies the override, possibly overriding
    // an existing one.  It only adds paths to our region paths if the
    // added_delta_paths array is specified to record those added.  If
    // not allowed to add paths and it needs to, it is an error, and so
    // an assert is triggered.  In that case it does not add the override,
    // still potentially removing an old one.
    void	setOverride(int point_num, const float *weights, int size,
		     UT_IntArray &map_from_delta,
		     const UT_Array<char *> &delta_paths,
		     const UT_Array<GEO_CaptureBoneStorage *> &delta_tubes,
		     UT_IntArray *added_delta_paths,
		     bool &max_size_invalid);

    void	updateIndexBounds( int point_num, bool exists,
				   bool &min_invalid, bool &max_invalid );

    int		findPathIndex( const char *path ) const;

private:    // data
    UT_Array<char *>			myRegionPaths;
    UT_Array<GEO_CaptureBoneStorage *>	myCaptureTubes;

    // We store our weight overrides as <index, weight> pairs, with
    // <-1, -1> signalling the end.  The indices are indices into
    // the our array of capture regions (paths).
    UT_Array<geo_CaptureOverridePointData *>	myWeights;

    int				myMinPointIndex;
    int				myMaxPointIndex;

    int				myNumOverrides;
    int				myMaxWeightDataSize;
};

enum
{
    GEO_CAPTUREOVERRIDE_NO_CHANGE = 0,
    GEO_CAPTUREOVERRIDE_SET = 1,
    GEO_CAPTUREOVERRIDE_CHANGE = 2,
    GEO_CAPTUREOVERRIDE_UNSET = 3,
};

class geo_CaptureOverrideDeltaData;

// This class is used for the undo mechanism and generally doesn't
// contain too many changes (as opposed to the GEO_CaptureOverrideData
// class), and so is stored in a very sparse form.  Observe that it
// has no method to apply the changes to the gdp as this is unnecessary
// since it is always built directly from the changes to the gdp.  Note
// the it is only intended to undo changes directly from the gdp if the
// input has not changed at all (ie. within the cook mechanism).  When it
// is part of an actual undo, the undo should only be applied to the
// permanent overrides object, and a full cook redone.

class GEO_API GEO_CaptureOverrideDelta
{
public:
    GEO_CaptureOverrideDelta();
    virtual ~GEO_CaptureOverrideDelta();

    void	copyForUndo( const GEO_CaptureOverrideDelta &other );

    // Destroy all internal structures completely.
    void	destroy();

    // Methods to query the state of the changes for particular points.
    bool	removesOverride( int point_num ) const;
    bool	addsOverride( int point_num ) const;

    // Capture regions MUST be added before any points are changed, and
    // initCaptureWeightChange is called.
    void	addedCaptureRegion( const GEO_Detail &gdp, const char *path,
				    int cindex );

    // We must be re-initialized if the attributes have grown, been
    // added, or removed as we cache offsets and sizes.  Make sure
    // that this is not called between a beginCaptureWeightChange
    // and the corresponding endCaptureWeightChange.
    void	initCaptureWeightChange( const GEO_Detail &gdp );

    // It is important that only one change per point is stored in this
    // class.  We cannot, in all cases, combine multiple changes to the
    // same point and so disallow this.
    void	beginCaptureWeightChange( GA_Offset point );
    void	endCaptureWeightChange( GA_Offset point,
					unsigned int point_index,
					int operation );

    const UT_Array<char *>	&getPaths() const
				      { return myRegionPaths; }
    const UT_Array<GEO_CaptureBoneStorage *>
				&getCaptureTubes() const
				      { return myCaptureTubes; }
    UT_IntArray			&getRegionsAddedToOverrides()
				      { return myAddedRegionsToOverrides; }
    const UT_IntArray		&getRegionsAddedToOverrides() const
				      { return myAddedRegionsToOverrides; }

    int		getNumChanges() const { return myChanges.entries(); }
    const geo_CaptureOverrideDeltaData *getChange(int i) const 
				      { return myChanges(i); }

    // Undo the stored overrides to the given detail.
    void	undoOverrideDeltas( GEO_Detail &gdp );

    // Compute the memory usage of this object (in bytes).
    int64	getMemoryUsage() const;

private:    // methods
    void	convertAndStorePtData( const float *gdp_data,
				       int num_point_regions,
				       float *&store_data, int &store_size );

    void	prepareCaptureRegionChange( int path_index );
    void	convertAndRestorePtData( GA_Attribute *attrib,
					 const GA_AIFIndexPair *indexpair,
					 GA_Offset point,
					 int num_point_regions,
					 const float *store_data,
					 int store_size,
					 const UT_IntArray &map_to_gdp,
					 float *tmp_data );
					

private:    // data
    UT_Array<char *>		 myRegionPaths;
    UT_Array<GEO_CaptureBoneStorage *>	 myCaptureTubes;

    // We need to keep track of regions we added to the gdp or region
    // paths we added to the permanent overrides class to allow us to
    // properly undo the changes.
    UT_IntArray			 myAddedRegionsToGdp;
    UT_IntArray			 myAddedRegionsToOverrides;

    UT_Array<geo_CaptureOverrideDeltaData *>	myChanges;

    // the following data is used for init/begin/end changes
    GA_ROAttributeRef		 mySavedTubeRef;
    float			*mySavedData;
    const GA_Attribute		*mySavedDataAttrib;
    const GA_AIFIndexPair	*mySavedDataAIF;
    unsigned int		 mySavedDataSize;
    UT_IntArray			 myMapFromGdp;
};

#endif // __GEO_CaptureOverrideData_h__

