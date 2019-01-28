/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __UT_RandomSequence_H__
#define __UT_RandomSequence_H__

#include "UT_API.h"

class UT_API UT_RandomSequence 
{
public:
    explicit UT_RandomSequence (int n = 0, int c = 0);

    void	setCurrent(int c = 0) { current = (unsigned)(c + 1); }
    void	setSize(int n);

    unsigned	getCurrent(void) { return(current - 1); }
    unsigned	getSize(void) { return(size); }

// This method returns the next integer in the sequence.
    unsigned	getNext(void);

protected:
private:

    unsigned	current;
    unsigned	mask;
    unsigned	size;		
};

#endif
