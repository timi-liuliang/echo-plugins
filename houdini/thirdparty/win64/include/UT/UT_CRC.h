/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __UT_CRC_H__
#define __UT_CRC_H__

#include "UT_API.h"

class UT_API UT_CRC 
{
public:
     UT_CRC (void);
    ~UT_CRC (void);

    unsigned long	updateCRC (unsigned long accum, char *ptr, int size);
};

#endif
