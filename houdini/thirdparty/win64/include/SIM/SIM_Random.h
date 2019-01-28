/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Random_h__
#define __SIM_Random_h__

#include "SIM_API.h"
#include "SIM_DataUtils.h"

/// This class defines a bunch of useful tools for random number
/// generation.  It is designed to be attached as subdata to objects
/// or solvers that need a stream of random numbers.
class SIM_API SIM_Random : public SIM_Data
{
public:
    // Methods to manipulate the stream:

    /// Sets the seed to the system.
    void		 setSeed(uint seed);

    /// Gets a random unsigned integer
    uint		 urandom();

    /// Returns a random float from [0..1)
    fpreal		 frandom();

    /// Returns a random float from [-.5...5)
    fpreal		 frandom0();

    /// Returns a random integer from [0..choices-1]
    /// 0 choices will return 0.  Negative choices will go
    /// for [-choices+1..0]
    int			 choice(int choices);

    /// Returns a number within the range [a..b]
    /// In the floating point case, the range is [a..b)
    int			 irange(int a, int b);
    fpreal		 frange(fpreal a, fpreal b);

    /// Returns true with the givent probability.  < 0 always false,
    /// > 1 always true.
    bool		 chance(fpreal probability);

protected:
    explicit		 SIM_Random(const SIM_DataFactory *factory);
    virtual		~SIM_Random();

    /// Override this function to initialize the random number generator.
    virtual void	 setSeedSubclass(uint seed) = 0;
    /// Override this function to implement a new random number generator.
    virtual uint	 urandomSubclass() = 0;

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_Random, SIM_Data);
};

#endif
