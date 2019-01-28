/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Guide_h__
#define __SIM_Guide_h__

#include "SIM_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_VectorTypes.h>
#include <GU/GU_DetailHandle.h>

class SIM_Time;
class SIM_Data;
class SIM_RootData;

#include <DEP/DEP_MicroNode.h>

/// This class provides an interface between a SIM_Data and its guide
/// geometry. It decides when it is necessary to actually build the
/// geometry.
class SIM_API SIM_Guide
{
public:
    /// Constructor which takes a const SIM_Data as our owner.
    explicit			 SIM_Guide(const SIM_Data *owner);
    /// Destructor for this class.
    virtual			~SIM_Guide();

    /// This function retrieves some guide geometry and a transform.
    /// getGuideGeometrySubclass() does all the real work.
    GU_ConstDetailHandle	 getGuideGeometry(const SIM_RootData &root,
						  UT_DMatrix4 &xform,
						  const SIM_Time &t);

    /// Returns the amount of memory used by this guide object. This function
    /// calls getMemorySizeSubclass().
    int64			 getMemorySize() const;

    /// This function clears all guide geometry information. It calls
    /// clearSubclass() to do the actual work.
    void			 clear();

    /// Sets the version parms value of our creator node at the time the
    /// guide geometry was created.
    void			 setParmVersion(long version);
    /// Gets the version parms value that was set when this object was
    /// created.
    long			 getParmVersion() const;

    DEP_MicroNode		&guideMicroNode()
    { return myMicroNode; }
    const DEP_MicroNode		&guideMicroNode() const
    { return myMicroNode; }

protected:
    /// Returns our owner data.
    const SIM_Data		&getOwner() const;
    /// Returns the current simulation time.
    const SIM_Time		&getSimulationTime() const;
    /// Calls SIM_Data::buildGuideGeometry() on the owner data.
    bool			 buildGuideGeometry(const SIM_RootData &root,
						const GU_DetailHandle &gdh,
						UT_DMatrix4 *xform,
						const SIM_Time &t) const;

    /// Override this function to customize how guide geometry is
    /// created or retrieved. Implementing this function is the whole
    /// point of this class. This function may build geometry, fetch
    /// existing geometry, modify existing geometry, or whatever else
    /// it sees fit.
    virtual GU_ConstDetailHandle getGuideGeometrySubclass(
						const SIM_RootData &root,
						UT_DMatrix4 &xform,
						const SIM_Time &t);
    /// Override this to report the amount of memory used by this object.
    /// Call the base class implementation as well and add that to the
    /// subclass-specific memory size.
    virtual int64		 getMemorySizeSubclass() const;
    /// Override this function to clear any data create by this class.
    virtual void		 clearSubclass();

private:
    const SIM_Data		*myOwner;
    long			 myParmVersion;
    DEP_MicroNode		 myMicroNode;
};

#endif

