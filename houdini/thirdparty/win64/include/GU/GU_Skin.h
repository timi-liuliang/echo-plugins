/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Skin.C (C++)
 *
 * COMMENTS:
 *	This is the filter used to create skinned patches.
 */

#ifndef __GU_Skin_h__
#define __GU_Skin_h__

#include "GU_API.h"
#include <GEO/GEO_Primitive.h>
#include <GEO/GEO_SurfaceType.h>

class GA_PrimitiveGroup;
class GU_Detail;

enum GU_SkinClosureType
{
    GU_SKIN_CLOSE_NONE	= 0,
    GU_SKIN_CLOSE	= 1,
    GU_SKIN_CLOSE_AUTO	= 2
};

/// This class can be used to queue up all primitives that are
/// to be deleted, with actual deletion being done once the operation
/// is complete. 
/// 
/// This is particularly useful when an operation is performed on 
/// multiple groups within one detail, since each deletion opeartion
/// itself is O(n) where n is the number of primitives in the detail.
/// Thus, accumulating primitives that are to be deleted and deleting them
/// once instead of every time for every group can lead to signficantly
/// faster code.
class GU_API GU_SkinCache
{
public:
    GU_SkinCache(GEO_Detail& parent_detail);
    virtual ~GU_SkinCache();

    /// Queues up an array of primitives to delete. If delete_points is true,
    /// will delete primitives' points as well. All queued primitives are
    /// deleted either by calling cleanup() or in the destructor.
    void addCleanupPrimitives(UT_Array<GEO_Primitive*>* prims_array, bool delete_points);

    /// Queues up a single primitive to delete. If delete_points is true,
    /// will delete primitives' points as well. All queued primitives are
    /// deleted either by calling cleanup() or in the destructor.
    void addCleanupPrimitive(GEO_Primitive* prim, bool delete_points);

    /// Deletes all queued up primitives.
    virtual void cleanup(void);

private:
    /// Disable the assignment operator. We don't need to copy the arrays
    /// entirely (slow) and at the same time we don't want pointers being
    /// shared between multiple objects.
    GU_SkinCache& operator=(const GU_SkinCache &source) { return *this; }

private:

    /// Holds pointers to primitives whose points also have to be deleted
    UT_Array<GEO_Primitive*> *myCleanupPrimsDeletePoints;

    /// Holds pointers to primitives whose points are to be left alone
    UT_Array<GEO_Primitive*> *myCleanupPrimsLeavePoints;

    /// Parent detail of the above primitives. For now we only support deleting
    /// primitives from one detail, but this can easily be extended to support
    /// multiple ones without changing external functions.
    GEO_Detail* myParentDetail;
};

class GU_API GU_SkinParms
{
public:
    GU_SkinParms();
    virtual ~GU_SkinParms();

    void ConvertGroupToPrimsArray(GEO_Detail* parent_detail, const GA_PrimitiveGroup& group, UT_Array<GEO_Primitive*>& prims_array);
    virtual void InitializePrimArrays(GEO_Detail* parent_detail);

    GEO_SurfaceType 		 surfType;
    int 	 		 vOrder;
    GU_SkinClosureType 		 vWrap;
    int			 	 keepShape;	// ruled or truly skinned
    const GA_PrimitiveGroup	*uFaces;

    UT_Array<GEO_Primitive*> *myUPrimsToProcess;
};

#endif
