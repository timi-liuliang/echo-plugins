/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_Poisson.h (UT Library, C++)
 *
 * COMMENTS:    Generating discrete random Poisson variates. 
 */

#ifndef __UT_Poisson__
#define __UT_Poisson__

#include <SYS/SYS_Floor.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>

class UT_Poisson
{
public:
    template<typename T>
    static int64 GetPoisson(fpreal mean, T &generator, bool limit_range=false, int limit_min=-1, int limit_max=-1)
    {
        if (mean <= 0.0f)
            return 0;

		if (limit_range)
		{
			if (limit_min > limit_max)
			{
				int tmp = limit_min;
				limit_min = limit_max;
				limit_max = tmp;
			}

			if (limit_min < 0)
				limit_min = 0;
			if (limit_max < 0)
				return 0;
		}
		
		int max_tries = 1000;
		
        // For small mean, just sum the probabilities until they're
        // greater than or equal to a uniform random number.
        const float smallmeanthreshold = 19;
        // (50-19)/sqrt(19) = 7.1 standard deviations, so we'll never hit it
        // unless things have gone wrong or maybe if fpreal becomes 32-bit.
        const int64 smallmeanmaxi = 50;
        if (mean < smallmeanthreshold)
        {
            int tries = 0;
            while (tries < max_tries)
            {
                tries++;

                const float u = generator.getFloat();
                // Exact equality check, just to avoid ln(0)
                if (u == 0)
                {
                    if (limit_range && (0 < limit_min || 0 > limit_max))
                        continue;
                    else
                        return 0;
                }

                fpreal r0 = u * SYSexp(mean);
                if (r0 <= 1.0)
                {
                    if (limit_range && (0 < limit_min || 0 > limit_max))
                        continue;
                    else
                        return 0;
                }

                fpreal f = 1.0 + mean;
                if (r0 <= f)
                {
                    if (limit_range && (1 < limit_min || 1 > limit_max))
                        continue;
                    else
                        return 1;
                }

                fpreal term = 0.5*mean*mean;
                f += term;
                int64 i = 2;
                while (r0 > f && i < smallmeanmaxi && (!limit_range || i <= limit_max))
                {
                    ++i;
                    term *= (mean / i);
                    f += term;
                }

                if (limit_range && (i < limit_min || i > limit_max))
                    continue;

                return i;
            }

			return limit_min; // or max?
        }

        // TODO: Which SYSfloor to use?
        //       fpreal or float? why?
        const int64 mode = int64(mean);
        const fpreal a = mean + 0.5;
        const int64 k = int64(SYSfloor(a - SYSsqrt(2.0*a))) + 1;
        fpreal f_k = 1;
        if (k == mode)
        {
            // do nothing. It's one.
        }
        else if (k > mode)
        {
            int64 curr = k;
            while (curr > mode)
            {
                f_k *= (mean / fpreal(curr));
                curr--;
            }
        }
        else // (k < mode)
        {
            int64 curr = mode;
            while (curr > k)
            {
                f_k *= (fpreal(curr) / mean);
                curr--;
            }
        }

        const fpreal s = (a - k)*SYSsqrt(f_k);
        // Now we have (s) and (a)
        int64 result = 0;
        fpreal x, y, f_x;
		int tries = 0;
		while (!limit_range || tries < max_tries)
        {
			tries++;
			
            // Generate u and v
            const float u = generator.getFloat();
            const float v = generator.getFloat();
            // set X
            x = a + (s * (2 * v - 1) / u);
            // reject all negative results
            if (x < 0)
                continue;
            // set K (= result)
            result = int64(SYSfloor(x));

            // if there is a range limit, and it falls outside of the range
            if (limit_range && (result < limit_min || result > limit_max))
                continue;

            // set Y
            y = u * u;
            // evaluate f_x and compare against y
            f_x = 1;
            if (result == mode)
            {
                return result;
            }
            else if (result > mode)
            {
                int64 curr = result;
                bool stopped = false;
                while (curr > mode)
                {
                    f_x *= (mean / fpreal(curr));
                    if (y > f_x)
                    {
                        stopped = true;
                        break;
                    }
                    curr--;
                }
                if (stopped)
                    continue;
            }
            else // (result < mode)
            {
                int64 curr = mode;
                bool stopped = false;
                while (curr > result)
                {
                    f_x *= (fpreal(curr) / mean);
                    if (y > f_x)
                    {
                        stopped = true;
                        break;
                    }
                    curr--;
                }
                if (stopped)
                    continue;
            }

            if (y <= f_x)
                return result;
        }
		
		return limit_min; // or max?
    }

};

#endif
