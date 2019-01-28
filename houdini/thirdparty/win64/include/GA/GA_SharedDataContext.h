/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_SharedDataContext.h (GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_SHAREDATACONTEXT_H_INCLUDED__
#define __GA_SHAREDATACONTEXT_H_INCLUDED__

#include "GA_API.h"
#include <UT/UT_ArrayStringMap.h>

class UT_IStream;
class UT_StringRef;
class UT_SCFReader;

///
/// The context stores information we can share between multiple
/// resolves, to allow us to keep file handles open.
///
class GA_API GA_SharedDataContext
{
public:
	GA_SharedDataContext();
    virtual ~GA_SharedDataContext();

    UT_IStream *openStream(const UT_StringRef &filename);

private:
    UT_ArrayStringMap<UT_IStream *>   myIStreams;
    UT_ArrayStringMap<UT_SCFReader *> mySCFReaders;
};

#endif // __GA_SHAREDATACONTEXT_H_INCLUDED__
