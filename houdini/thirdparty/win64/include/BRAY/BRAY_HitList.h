/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BRAY_HitList.h (RAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __RAY_HitList__
#define __RAY_HitList__

#include "BRAY_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_NonCopyable.h>
#include "BRAY_Hit.h"

class BRAY_Range;

class BRAY_API BRAY_HitList
    : public UT_NonCopyable
{
public:
    BRAY_HitList();
    ~BRAY_HitList();

    void	init(exint nihts);
    void	init(exint nihts, const float *dist);
    void	init(exint nihts, const float *dist, float max_dist);
    void	clear(const BRAY_Range &range, const float *dist = nullptr);

    void	copyFrom(const BRAY_Range &range, const BRAY_HitList &hits);

    void	compact(const BRAY_Range &range, const BRAY_HitList &hits);

    exint		size() const { return myHits.size(); }
    const BRAY_Hit	*data() const { return myHits.data(); }

    BRAY_Hit		&hit(exint i) { return myHits[i]; }
    const BRAY_Hit	&hit(exint i) const { return myHits[i]; }
    BRAY_Hit		&operator[](exint i) { return hit(i); }
    const BRAY_Hit	&operator[](exint i) const { return hit(i); }

    void		validate() const;

private:
    UT_Array<BRAY_Hit>	myHits;
};

#endif
