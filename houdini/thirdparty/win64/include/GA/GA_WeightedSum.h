/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_WeightedSum.h (GA  Library, C++)
 *
 * COMMENTS:
 * 	This class maintains context information while performing a weighted
 * 	sum of attribute data.
 *
 *	An example of implementing linear interpolation using a weighted sum:
 *		GA_WeightedSum	sum;
 *		attribute_set.startSum(sum);
 *		attribute_set.addPartial(sum, a, 1-bias);
 *		attribute_set.addPartial(sum, b, bias);
 *		attribute_set.finishSum();
 */

#ifndef __GA_WeightedSum__
#define __GA_WeightedSum__

#include "GA_API.h"

#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>


/// @brief Context to keep track of weighted sums
///
/// Weighted sums are used in metaball blending, TP surfaces and various other
/// places.  The weighted sum class is used to keep track of progress through
/// the weighted sum.  The class keeps track of the current, maximum and total
/// of the weights.  This allows discrete attributes to perform conditional
/// copying (on the maximum value), or for normalization at the end of the sum.
class GA_API GA_WeightedSum 
{
public:
    /// The weighted sum is used to keep track of partial sums.
    ///
    /// The @c homogeneous flag will change weighted sums of homogeneous
    /// attributes (GA_TYPE_HPOINT) so that they will be homogenized before
    /// summing and de-homogenized at the conclusion of the sum.
    /// This only affects homogeneous attributes.
     GA_WeightedSum() { rewind(); }
    ~GA_WeightedSum() {}

    /// Rewind will reset the values of the sum (i.e. the count and weights)
    void	rewind();	// Reset the value

    /// Advance is called when starting a new iteration.  The weight passed in
    /// is the weight associated with this iteration.
    void	advance(fpreal weight)
		{
		    myMaxWeight = SYSmax(myMaxWeight, myCurrentWeight);
		    myCurrentWeight = weight;
		    myTotalWeight += weight;
		    myCount++;
		}

    /// Finish is called after the last partial sum has been called.  This lets
    /// the max-weight get set correctly.
    void	finish();

    /// The partial index runs from 0...N
    /// On the call to initSum(), the value will be -1, on the call to
    /// finishSum(), the value will be N-1.
    int		getPartialIndex() const		{ return myCount; }

    /// Like the partial index, but returns the "count".  That is, on
    /// finishSum(), this method will return the count of the iterations.
    int		getIterationCount() const	{ return myCount+1; }

    /// Get the current weight to sum
    fpreal	getCurrentWeight() const	{ return myCurrentWeight; }

    /// Get the sum of the weights for all iterations (for normalization)
    fpreal	getTotalWeight() const		{ return myTotalWeight; }

    /// A convenince method for conditional copying of the maximum weight
    bool	getIsMaxWeight() const	{ return myCurrentWeight>myMaxWeight; }

private:
    fpreal	myMaxWeight;		// My maximum weight
    fpreal	myCurrentWeight;	// My current weight
    fpreal	myTotalWeight;		// Total weight over all iterations
    int		myCount;		// My iteration count
};

#endif

