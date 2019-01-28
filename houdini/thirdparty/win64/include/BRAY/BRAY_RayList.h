/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BRAY_RayList.h (RAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __RAY_RayList__
#define __RAY_RayList__

#include "BRAY_API.h"
#include <UT/UT_NonCopyable.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_SharedArray.h>

#include "BRAY_Random.h"

class BRAY_Range;

template <typename T>
class BRAY_API BRAY_RayListT
    : public UT_NonCopyable
{
public:
    using ftype = T;
    using vector3 = UT_Vector3T<T>;
    using color3 = UT_Vector3;

     BRAY_RayListT();
    ~BRAY_RayListT();

    // Transform the source rays into this ray list
    void	transform(const BRAY_RayListT<T> &src, const UT_Matrix4D &x);

    // Copy over the information from a base set of rays (but selectively for
    // the active elements in the range).
    void	copyFrom(const BRAY_Range &range, const BRAY_RayListT<T> &src);

    // Compact rays so rays are contiguous
    void	compact(const BRAY_Range &range, const BRAY_RayListT<T> &src);

    // Number of individual rays
    exint	size() const { return mySize; }
    void	setSize(exint nrays);

    // Propagation level
    int		level() const { return myLevel; }
    void	setLevel(int level) { myLevel = level; }

    vector3		*getP() { return myP; }
    const vector3	*getP() const { return myP; }

    vector3		*getD() { return myD; }
    const vector3	*getD() const { return myD; }

    /// Contribution to final pixel result (weight of the ray)
    color3		*contrib() { return myContrib; }
    const color3	*contrib() const { return myContrib; }

    // Scale contribution amounts
    void		scaleContrib(const BRAY_Range &range,
				    const color3 *scale);

    /// Pixel to write into (buffer list offset)
    int			*bufferIndex() { return myBufferIndex.get(); }
    const int		*bufferIndex() const { return myBufferIndex.get(); }

    T				*getTime() { return myTime.get(); }
    const T			*getTime() const { return myTime.get(); }

    BRAY_Random			*getRandom() const { return myRandom.get(); }

private:
    // TODO: Vectorize
    vector3			*myP;
    vector3			*myD;
    color3			*myContrib;
    UT_SharedArray<T>		 myTime;
    UT_SharedArray<int>		 myBufferIndex;
    UT_SharedArray<BRAY_Random>	 myRandom;
    int				 myLevel; // Ray propagation level
    int				 mySize;
};

using BRAY_RayList = BRAY_RayListT<fpreal32>;

#endif
