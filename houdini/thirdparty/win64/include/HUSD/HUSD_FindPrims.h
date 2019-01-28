/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_FindPrims_h__
#define __HUSD_FindPrims_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include "HUSD_Utils.h"
#include <UT/UT_BoundingBox.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_VectorTypes.h>
#include <pxr/pxr.h>

PXR_NAMESPACE_OPEN_SCOPE

class XUSD_PathSet;
class XUSD_PathPattern;

PXR_NAMESPACE_CLOSE_SCOPE

class HUSD_TimeCode;

class HUSD_API HUSD_FindPrims
{
public:
			 HUSD_FindPrims(HUSD_AutoAnyLock &lock,
				 HUSD_PrimTraversalDemands demands =
				    HUSD_TRAVERSAL_DEFAULT_DEMANDS,
				 bool find_point_instancer_ids = false);
			 // Simple constructors when you just want to operate
			 // on one or an array of primitives and you know the
			 // paths.
			 HUSD_FindPrims(HUSD_AutoAnyLock &lock,
				 const UT_StringRef &primpath);
			 HUSD_FindPrims(HUSD_AutoAnyLock &lock,
				 const UT_StringArray &primpaths);
			~HUSD_FindPrims();

    enum BBoxContainment {
	BBOX_FULLY_INSIDE,
	BBOX_FULLY_OUTSIDE,
	BBOX_PARTIALLY_INSIDE,
	BBOX_PARTIALLY_OUTSIDE
    };

    const PXR_NS::XUSD_PathSet	&getExpandedPathSet() const;
    const PXR_NS::XUSD_PathSet	&getCollectionAwarePathSet() const;

    bool		 addPattern(const UT_StringArray &pattern_tokens);
    bool		 addPattern(const UT_StringRef &pattern);
    bool		 addPrimitiveType(const UT_StringRef &primtype);
    bool		 addPrimitiveKind(const UT_StringRef &primkind);
    bool		 addPrimitivePurpose(const UT_StringRef &primpurpose);
    bool		 addVexpression(const UT_StringRef &vexpression,
				const HUSD_TimeCode &time_code) const;
    bool		 addBoundingBox(const UT_BoundingBox &bbox,
				const HUSD_TimeCode &t,
				const UT_StringArray &purposes,
				BBoxContainment containment);

    void		 setNodeId(int id);

    const UT_StringMap<UT_Int64Array> &getPointInstancerIds() const;
    void		 getExpandedPaths(UT_StringArray &paths) const;
    void		 getCollectionAwarePaths(UT_StringArray &paths) const;
    bool		 getFindPointInstancerIds() const;
    bool		 getIsTimeVarying() const;

    /// Returns a collection path, if only a single collection was added.
    /// Returns an empty string, if primitive target consists of more than 
    /// a single collection.
    UT_StringHolder	 getSingleCollectionPath() const;

private:
    bool		 addPattern(const PXR_NS::XUSD_PathPattern &pattern);

    class husd_FindPrimsPrivate;

    UT_UniquePtr<husd_FindPrimsPrivate>	 myPrivate;
    HUSD_AutoAnyLock			&myAnyLock;
    HUSD_PrimTraversalDemands		 myDemands;
    int					 myNodeId;
    bool				 myFindPointInstancerIds;
};

#endif

