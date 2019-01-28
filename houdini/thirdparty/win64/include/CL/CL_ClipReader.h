/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        CL_ClipReader.h
 *
 * COMMENTS:
 *	This abstract class defines the interface for an object that knows
 *	how to read a clip from a file.  Clip readers are registered with
 *	the CL_ClipIO singleton object.
 */

#ifndef __CL_ClipReader_h__
#define __CL_ClipReader_h__

#include "CL_API.h"
class CL_Clip;

class CL_API CL_ClipReader
{
public:
    CL_ClipReader() {}
    virtual ~CL_ClipReader() {}

    virtual bool readClip(const char *file_name, CL_Clip &clip) = 0;
};

#endif
