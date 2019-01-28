/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_GeometryCopy_h__
#define __SIM_GeometryCopy_h__

#include "SIM_API.h"
#include <UT/UT_DMatrix4.h>
#include "SIM_Geometry.h"

enum SIM_DataIdStrategy
{
    /// The default strategy is to bump all data ids after releasing the lock
    /// on the SIM_GeometryCopy's internal geometry.
    SIM_DATA_ID_BUMP = 0,

    /// The data ids will not be automatically bumped, and the caller is
    /// responsible for bumping data ids on any modified attributes.
    SIM_DATA_ID_PRESERVE = 1
};

/// This implements a SIM_Geometry that copies the source geometry.
class SIM_API SIM_GeometryCopy : public SIM_Geometry
{
public:
    /// This method provides access to the internal geometry so that it
    /// can be modified. For each call to lockGeometry(), there must be a
    /// corresponding call to releaseGeometry(). Calls to lockGeomtry()
    /// can be nested.
    GU_DetailHandle	 lockGeometry();
    /// This function releases the lock on our internal geometry. This
    /// function must be called once for each call to lockGeometry(). Also,
    /// once this function has been called, no more modifications should be
    /// made to the geometry returned by lockGeometry() or the result will
    /// be undefined.
    void		 releaseGeometry(SIM_DataIdStrategy strategy = SIM_DATA_ID_BUMP);

    /// Swaps the internal geometry with the provided one.
    GU_DetailHandle	 swapGeometry(GU_DetailHandle replacement);
    GU_DetailHandle	 swapGeometryWithoutCEFlush(GU_DetailHandle replacement, bool &hascedata);

    /// This method provides access to the internal transform so that it
    /// can be modified. For each call to lockTransform(), there must be a
    /// corresponding call to releaseTransform(). Calls to lockTransform()
    /// can be nested.
    UT_DMatrix4		&lockTransform();
    /// This function releases the lock on our internal transform. This
    /// function must be called once for each call to lockTransform(). Also,
    /// once this function has been called, no more modifications should be
    /// made to the transform returned by lockTransform() or the result will
    /// be undefined.
    void		 releaseTransform();

    /// Build a GPU backed version of the attribute that is marked
    /// to be written back to the GU_Detail after computation completes.
    virtual GA_CEAttribute	*getWriteableCEAttribute(GA_AttributeOwner owner, const UT_StringRef &name, GA_StorageClass storage, int &tuplesize, bool isarray, bool docopy);

    virtual void		 flushCEWriteCaches() const;

protected:
    explicit		 SIM_GeometryCopy(const SIM_DataFactory *factory);
    virtual		~SIM_GeometryCopy();

    /// Initializes this data by clearing our internal geometry and transform.
    virtual void	 initializeSubclass();
    /// This function creates an internal copy of any SIM_Geometry data
    /// passed to it.
    virtual void	 makeEqualSubclass(const SIM_Data *source);
    /// Respond to setting our parameters by adjusting our transform.
    /// The parameters will be in the standard t, r, s style which we
    /// convert in to a matrix and multiply by our current transform.
    virtual void	 setParametersSubclass(const SIM_Options &options);

private:
    static const SIM_DopDescription	*getGeometryCopyDopDescription();

    int					 myDetailLockCount;
    int					 myTransformLockCount;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_GeometryCopy,
			SIM_Geometry,
			"Geometry Copy",
			getGeometryCopyDopDescription());
};

/// Utility class that handles creating a GU_DetailHandleAutoWriteLock and
/// calling releaseGeometry() when it goes out of scope.
class SIM_API SIM_GeometryAutoWriteLock : UT_NonCopyable
{
public:
    SIM_GeometryAutoWriteLock(SIM_GeometryCopy *simgeo,
                              SIM_DataIdStrategy data_id_strategy = SIM_DATA_ID_BUMP)
        : mySimGeo(simgeo),
          myDataIdStrategy(data_id_strategy),
          myHandle(mySimGeo->lockGeometry()),
          myGdp(myHandle.writeLock())
    {
    }

    ~SIM_GeometryAutoWriteLock()
    {
        myHandle.unlock(myGdp);
        mySimGeo->releaseGeometry(myDataIdStrategy);
    }

    const GU_DetailHandle &handle() { return myHandle; }
    GU_Detail &getGdp() { return *myGdp; }

private:
    SIM_GeometryCopy *mySimGeo;
    const SIM_DataIdStrategy myDataIdStrategy;
    GU_DetailHandle myHandle;
    GU_Detail *myGdp;
};

#endif

