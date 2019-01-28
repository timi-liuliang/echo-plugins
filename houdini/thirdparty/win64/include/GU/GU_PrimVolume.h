/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimVolume.h ( GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_PrimVolume__
#define __GU_PrimVolume__

#include "GU_API.h"

#include <GEO/GEO_PrimVolume.h>
#include <UT/UT_ThreadedAlgorithm.h>

#include "GU_Detail.h"

class GEO_ConvertParms;
class GU_PrimVolumeCache;
class GU_PrimVolumeCacheParms;
class UT_MemoryCounter;

class GU_API GU_PrimVolume: public GEO_PrimVolume
{
protected:
    /// NOTE: Primitives should not be deleted directly.  They are managed
    ///       by the GA_PrimitiveList and the stash.
    virtual ~GU_PrimVolume() {}

public:
    /// NOTE: This constructor should only be called via GU_PrimitiveFactory.
    GU_PrimVolume(GU_Detail *gdp, GA_Offset offset=GA_INVALID_OFFSET)
	: GEO_PrimVolume(gdp, offset)
    {}

    virtual const GA_PrimitiveDefinition &getTypeDef() const
    {
        UT_ASSERT(theDefinition);
        return *theDefinition;
    }

    /// Report approximate memory usage.
    virtual int64 getMemoryUsage() const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// NOTE: This should always include sizeof(*this).
    virtual void countMemory(UT_MemoryCounter &counter) const;

    // Conversion Methods
    virtual GEO_Primitive *convert(GEO_ConvertParms &parms,
				GA_PointGroup *usedpts = 0);
    virtual GEO_Primitive *convertNew(GEO_ConvertParms &parms);

    virtual void	normal(NormalComp &output) const {}

    // NOTE:  For static member functions please call in the following
    //        manner.  <ptrvalue> = GU_PrimSphere::<functname>
    //        i.e.      sphereptr = GU_PrimSphere::sphereBuild(params...);

    // Optional Build Method 
    static GEO_PrimVolume	*build(GU_Detail *gdp);

    // Builds a GU_PrimVolume attached to the given gdp with values
    // initialized from the callback function.  This is intentionally similar
    // to the GU_Detail::polyIsoSurface method.
    // Note that sampling is done at centers of voxels!
    static GEO_PrimVolume	*buildFromFunction(GU_Detail *gdp, 
				    float (*valAtPoint)(const UT_Vector3 &,
							void *data),
				    void *data,
				    const UT_BoundingBox &bbox,
				    int xres, int yres, int zres);

    // A threadable version.  This should be preferred as you can
    // avoid using static data for callbacks.
    // The jobinfo you receive can be queried to find how many
    // threads and which one you are being called from.
    // Setting allowthreading to false avoids threading.
    static GEO_PrimVolume	*buildFromFunction(GU_Detail *gdp, 
				    float (*valAtPoint)(const UT_Vector3 &,
							void *,
							const UT_JobInfo &),
				    void *data,
				    const UT_BoundingBox &bbox,
				    int xres, int yres, int zres,
				    bool allowthreading = true);

    typedef float (*VolumeSampler)(const UT_Vector3 &, void *, const UT_JobInfo &);
protected:

    THREADED_METHOD5(GU_PrimVolume, vox->numTiles() > 1,
		buildFromFunctionInternal,
		UT_VoxelArrayF *, vox,
		const UT_Matrix3 &, xform,
		const UT_Vector3 &, center,
		VolumeSampler, valAtPoint,
		void *, data)

    void	buildFromFunctionInternalPartial(UT_VoxelArrayF *vox, 
			    const UT_Matrix3 &xform, 
			    const UT_Vector3 &center, 
			    VolumeSampler valAtPoint,
			    void *data, 
			    const UT_JobInfo &info);

private:
    static GA_PrimitiveDefinition	*theDefinition;
    friend class			 GU_PrimitiveFactory;

// Don't warn about overriding deprecated virtual methods
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif
