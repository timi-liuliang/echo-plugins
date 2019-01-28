/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * 	NAME:			 DD_Receiver.h
 *
 * COMMENTS:
 *
 *
 */

#ifndef __DD_Receiver_
#define __DD_Receiver_

#include "DD_API.h"
class DD_Source;

class DD_API DD_Receiver
{

public:

    DD_Receiver();
    virtual ~DD_Receiver();

    // x,y are global screen coordinates
    // final determines whether or not the source is actually
    // accepted or simply tested for acceptance

    virtual bool	receive(DD_Source &src, int x, int y, bool final) = 0;
    virtual void	finished() = 0;

private:


};

#endif 

