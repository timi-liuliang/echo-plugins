/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_RayIntersect_h__
#define __SIM_RayIntersect_h__

#include "SIM_API.h"
#include <GU/GU_RayIntersect.h>
#include "SIM_DataUtils.h"

class SIM_Geometry;

/// This class hold a GU_RayIntersect structure. This is a data structure
/// that can be built from any GU_Detail. As such, this data should be
/// used by attaching it as subdata to a SIM_Geometry, and calling
/// createRayIntersect(). Because this data can be recreated on demand,
/// it returns true from the getIsAlternateRepresentationSubclass() function
/// and is never saved to or loaded from disk. It also gets deleted
/// automatically if the parent SIM_Geomtry data changes.
class SIM_API SIM_RayIntersect : public SIM_Data
{
public:
    /// Get the ray intersection structure. Note that the ray intersect
    /// information is always stored for the geometry without applying the
    /// geometry transform. This is unlike the SIM_SDF class, but is
    /// required by the nature of the ray intersection data structure,
    /// which holdson to primitive pointers.
    GU_RayIntersect	&getRayIntersect() const;

protected:
    explicit		 SIM_RayIntersect(const SIM_DataFactory *factory);
    virtual		~SIM_RayIntersect();

    /// Clears the ray intersection data.
    virtual void	 initializeSubclass();
    /// Returns the information that this data type is built on demand.
    /// It can be rebuilt at any time from any SIM_Geometry data.
    virtual int64	 getMemorySizeSubclass() const;
    virtual bool	 getIsAlternateRepresentationSubclass() const;
    virtual void	 initAlternateRepresentationSubclass(const SIM_Data &);

private:
    /// Build the ray intersection structure from some geometry.
    void		 createRayIntersect(const SIM_Geometry *geo);

    mutable GU_RayIntersect	 myRayIntersect;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_RayIntersect,
			SIM_Data,
			"Geometry Ray Intersector",
			getEmptyDopDescription());
};

#endif

