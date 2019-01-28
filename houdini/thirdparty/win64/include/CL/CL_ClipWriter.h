/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        CL_ClipWriter.h
 *
 * COMMENTS:
 *	This abstract class defines the interface for an object that knows
 *	how to write a clip to a file.  Clip writers are registered with
 *	the CL_ClipIO singleton object.
 */

#ifndef __CL_ClipWriter_h__
#define __CL_ClipWriter_h__

#include "CL_API.h"
class CL_Clip;

class CL_API CL_ClipWriter
{
public:
    CL_ClipWriter() {}
    virtual ~CL_ClipWriter() {}

    virtual bool writeClip(const char *file_name, const CL_Clip &clip) = 0;
};

#endif
