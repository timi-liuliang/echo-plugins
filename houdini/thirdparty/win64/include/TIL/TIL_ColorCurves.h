/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_ColorCurves.h ( TIL Library, C++)
 *
 * COMMENTS:
 *	A class for sampling color curves.
 */
#ifndef TIL_COLOR_CURVES_H
#define TIL_COLOR_CURVES_H

#include "TIL_API.h"

class TIL_API TIL_ColorCurves
{
public:
	 TIL_ColorCurves(int curve_size = 256);
	~TIL_ColorCurves();

    void	setValid(bool valid);
    void	setValid(int comp, bool valid);

    void	setCurveSize(int size);
    int		getCurveSize() const { return myCurveSize; }

    const float	*getCurve(int comp) const;
    float	*getCurveForWrite(int comp);
    
    void	 setDomain(float start, float end) { myStart=start; myEnd=end;}
    void	 getDomain(float &start, float &end) const
		 { start = myStart; end = myEnd; }

    void	 clampRange(float minr, float maxr);
    
private:
    bool	 myValid[4];
    float	*myCurves[4];
    int		 myCurveSize;
    float	 myStart;
    float	 myEnd;
};

#endif
