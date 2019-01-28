/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_RandomTwister_h__
#define __SIM_RandomTwister_h__

#include "SIM_API.h"
#include <UT/UT_MTwister.h>
#include "SIM_Random.h"

/// This class defines a bunch of useful tools for random number
/// generation.  It is designed to be attached as subdata to objects
/// or solvers that need a stream of random numbers.
class SIM_API SIM_RandomTwister : public SIM_Random
{
protected:
    explicit		 SIM_RandomTwister(const SIM_DataFactory *factory);
    virtual		~SIM_RandomTwister();

    virtual void	 initializeSubclass();
    virtual void	 makeEqualSubclass(const SIM_Data *source);
    virtual void	 saveSubclass(std::ostream &os) const;
    virtual bool	 loadSubclass(UT_IStream &is);

    virtual uint	 urandomSubclass();
    virtual void	 setSeedSubclass(uint seed);

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_RandomTwister,
			SIM_Random,
			"Random Stream",
			getEmptyDopDescription());

    UT_MersenneTwister	 myTwister;
};

#endif
