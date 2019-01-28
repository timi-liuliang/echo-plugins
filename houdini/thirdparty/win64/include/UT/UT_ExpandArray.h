/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ExpandArray.h ( UT Library, C++)
 *
 * COMMENTS:
 *		This class is used to encapsulate the methods
 *		involved with expanding a string pattern.
 */

#ifndef __UT_ExpandArray__
#define __UT_ExpandArray__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_String.h"

class UT_API UT_ExpandArray
{

public:

     explicit UT_ExpandArray(int start_size = 10);
    ~UT_ExpandArray();

    int			 entries() const
			 { return myCurSize; }

    const char		*operator()(unsigned int i) const
			 {
			     UT_ASSERT_P(i < myCurSize);
			     return myToken[i];
			 }

    int		 	 setPattern(const char *pattern, int *dirty_flag=0);
    int		 	 appendPattern(const char *pattern);

    const UT_String	&getPattern() const
			 { return myPattern; }

    int64		 getMemoryUsage(bool inclusive) const;

private:
    void 	  setPattern(const char *pattern, int append);
    void	  clear();

    UT_String	  myPattern;
    int		  myMaxSize;
    int		  myCurSize;
    char	**myToken;

};

#endif

