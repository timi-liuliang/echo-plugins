/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        CL_AsciiClipWriter.h
 *
 * COMMENTS:
 *	This class writes to .clip files.
 */

#ifndef __CL_AsciiClipWriter_h__
#define __CL_AsciiClipWriter_h__

#include "CL_API.h"
#include "CL_ClipWriter.h"

#include <UT/UT_StringArray.h>

class UT_OStream;
class CL_ClipPrivateIO;

class CL_API CL_AsciiClipWriter : public CL_ClipWriter
{
public:
    CL_AsciiClipWriter() {}
    virtual ~CL_AsciiClipWriter() {}

    virtual bool writeClip(const char *file_name, const CL_Clip &clip);
    bool writeClip(UT_OStream &os, const CL_Clip &clip,
		   const CL_ClipPrivateIO *priv = 0);
    bool writeClip(UT_OStream &os, const CL_Clip &clip,
		   UT_StringArray& parms, const CL_ClipPrivateIO *priv = 0);
};

#endif
