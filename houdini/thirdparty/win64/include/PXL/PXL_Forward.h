/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PXL_Forward.h ( PXL Library, C++)
 *
 * COMMENTS:
 */

#ifndef __PXL_Forward__
#define __PXL_Forward__

/// Forward declarations for PXL library
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_SharedPtr.h>

class PXL_DeepChannelList;
class PXL_DeepCompressor;
class PXL_DeepSampleFilter;
class PXL_DeepSampleList;

typedef UT_IntrusivePtr<PXL_DeepChannelList>	PXL_DeepChannelListPtr;
typedef UT_IntrusivePtr<PXL_DeepSampleList>	PXL_DeepSampleListPtr;
typedef UT_IntrusivePtr<PXL_DeepSampleFilter>	PXL_DeepSampleFilterPtr;
typedef UT_SharedPtr<PXL_DeepCompressor>	PXL_DeepCompressorPtr;

#endif

