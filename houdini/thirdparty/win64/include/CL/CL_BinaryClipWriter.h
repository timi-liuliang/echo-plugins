/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        CL_BinaryClipWriter.h
 *
 * COMMENTS:
 *	This class writes to .bclip files.
 */

#ifndef __CL_BinaryClipWriter_h__
#define __CL_BinaryClipWriter_h__

#include "CL_API.h"
#include "CL_ClipWriter.h"

#include <UT/UT_StringArray.h>

class UT_OStream;
class CL_ClipPrivateIO;

class CL_API CL_BinaryClipWriter : public CL_ClipWriter
{
public:
    CL_BinaryClipWriter(bool force_noncommercial)
        : myForceNonCommercialFlag(force_noncommercial)
    {
    }

    virtual ~CL_BinaryClipWriter() {}

    virtual bool writeClip(const char *file_name, const CL_Clip &clip);
    bool writeClip(UT_OStream &os, const CL_Clip &clip,
		   const char *file_name, const CL_ClipPrivateIO *priv = 0);
    bool writeClip(UT_OStream &os, const CL_Clip &clip,
		   UT_StringArray& parms, const char *file_name,
		   const CL_ClipPrivateIO *priv = 0);

private:
    bool	myForceNonCommercialFlag;
};

#endif
