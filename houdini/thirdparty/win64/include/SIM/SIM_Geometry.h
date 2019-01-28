/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Geometry_h__
#define __SIM_Geometry_h__

#include "SIM_API.h"
#include <GA/GA_CEAttribute.h>
#include <GU/GU_DetailHandle.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_Array.h>
#include "SIM_OptionsUser.h"
#include "SIM_DataUtils.h"

class UT_IStream;

/// This is the definitions for the SIM_Geometry interface. It also
/// provides a default implementation that returns an empty geometry and
/// an identity transform. It also has the facility to hold its own
/// geometry and transform as internal data. If set, these are saved and
/// loaded automatically. But there is no way for anyone except subclasses
/// to set these internal data. Thus they are really just a convenience
/// when writing SIM_Geometry subclasses.
class SIM_API SIM_Geometry : public SIM_Data,
			     public SIM_OptionsUser
{
public:
    /// Accesses the relative path to the position data associated with
    /// this geometry.
    GETSET_DATA_FUNCS_S(SIM_NAME_POSITIONPATH, PositionPath);

    /// Method for accessing the geometry. If we have some internal
    /// geometry data, we return that. Otherwise this function calls
    /// getGeometrySubclass().
    GU_ConstDetailHandle getGeometry() const;

    /// Get the geometry transform. If we have some internal transform
    /// data, we return that. Otherwise this function calls
    /// getTransformSubclass().
    void		 getTransform(UT_DMatrix4 &xform) const;

    /// Attempt to build a GPU-backed attribute from our geometry.
    /// Returns 0 on failure.
    /// You do not own the resulting GA_CEAttribute.
    virtual GA_CEAttribute	*getReadableCEAttribute(GA_AttributeOwner owner, const UT_StringRef &aname, GA_StorageClass storage, int &tuplesize, bool isarray, bool docopy) const;

    /// Acquires our geometry without triggering any write backs
    /// of pending CE operations, this is useful if you care only
    /// about meta data.
    GU_ConstDetailHandle getGeometryWithoutCEFlush() const;

    /// Clear any pending writes from the GPU.
    virtual void	 flushCEWriteCaches() const;
    /// Clear all GPU caches.
    virtual void	 flushCECaches() const;

    void		 markCECacheStatus(bool status)
    { myHasCECache = status; }


protected:
    explicit		 SIM_Geometry(const SIM_DataFactory *factory);
    virtual		~SIM_Geometry();

    /// Set initial values on all the geometry attributes.
    virtual void	 initializeSubclass();
    /// Makes this geometry equal to some other SIM_Geometry.
    virtual void	 makeEqualSubclass(const SIM_Data *source);
    /// Saves our attributes, and our internal data if it has been set.
    virtual void	 saveSubclass(std::ostream &os) const;
    virtual void	 saveIOSubclass(std::ostream &os, SIM_DataThreadedIO *io) const;
    /// Loads our attributes and internal data if it was set when we saved.
    virtual bool	 loadSubclass(UT_IStream &is);
    virtual bool	 loadIOSubclass(UT_IStream &is, SIM_DataThreadedIO *io);
    /// Creates a query object specifically for geometry data to access
    /// the transforms associated with geometry data.
    virtual SIM_Query	*createQueryObjectSubclass() const;
    /// Create an interpolation between two other SIM_Geometry subclasses.
    /// This is one place where the internal data gets used automatically.
    /// Since the source data may be of any subclass, the only way to
    /// safely and correctly interpolate between two SIM_Geometry subclasses
    /// is to copy one of the source geometry and transform to our internal
    /// data, and then do a blend towards the second source data.
    virtual void	 interpolateSubclass(const SIM_Data *source1,
					     const SIM_Data *source2,
					     fpreal interp);
    virtual int64	 getMemorySizeSubclass() const;

    /// This function is called by getGeometry() to return the actual
    /// geometry data. The default implementation returns an empty geometry.
    virtual GU_ConstDetailHandle getGeometrySubclass() const;
    /// This function is called by getTransform() to return the actual
    /// transform data. The default implementation returns an identity matrix.
    virtual void		 getTransformSubclass(UT_DMatrix4 &xform) const;

public:
    /// Set our internal geometry data. If some internal data already exists,
    /// it is deleted. The pointer passed in becomes controlled by the
    /// SIM_Geometry so must not be deleted by external code. A null pointer
    /// can be passed in to clear our internal geometry data.
    void		 setOwnGeometry(GU_Detail *gdp);
    /// Set our internal geometry data from an existing GU_DetailHandle.
    /// If some internal data already exists, it is deleted.
    void		 setOwnGeometry(const GU_DetailHandle &gdh);
    virtual void	 setOwnGeometrySubclass(const GU_DetailHandle &gdh);
    /// Returns a const pointer to our internal geometry data, if it is set.
    GU_ConstDetailHandle getOwnGeometry() const;
    /// Returns a pointer to our internal geometry data, if it is set.
    GU_DetailHandle	 getOwnGeometry();

    /// Set our internal transform data. If some internal data already exists,
    /// it is deleted. The pointer passed in becomes controlled by the
    /// SIM_Geometry so must not be deleted by external code. A null pointer
    /// can be passed in to clear our internal transform data.
    void		 setOwnTransform(UT_DMatrix4 *xform);
    /// Returns a const pointer to our internal transform data, if it is set.
    const UT_DMatrix4	*getOwnTransform() const;
    /// Returns a pointer to our internal transform data, if it is set.
    UT_DMatrix4		*getOwnTransform();

private:
    GU_DetailHandle	 myOwnGeometry;
    UT_DMatrix4		*myOwnTransform;

protected:
    mutable bool			myHasCECache;

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_Geometry, SIM_Data);
};

/// Utility class that handles creating a GU_DetailHandleAutoReadLock for the
/// (possibly NULL) SIM_Geometry.
class SIM_API SIM_GeometryAutoReadLock : UT_NonCopyable
{
public:
    SIM_GeometryAutoReadLock(const SIM_Geometry *simgeo)
        : myReadLock(simgeo ? simgeo->getGeometry() : GU_DetailHandle())
    {
    }

    const GU_Detail *getGdp() const { return myReadLock.getGdp(); }

private:
    GU_DetailHandleAutoReadLock myReadLock;
};

#endif
