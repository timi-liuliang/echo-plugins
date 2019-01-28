/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_Merge_h__
#define __HUSD_Merge_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include <UT/UT_StringArray.h>
#include <UT/UT_UniquePtr.h>

enum HUSD_MergeStyle {
    HUSD_MERGE_FLATTENED_LAYERS,
    HUSD_MERGE_SEPARATE_LAYERS,
};

class HUSD_API HUSD_Merge
{
public:
			 HUSD_Merge(HUSD_MergeStyle merge_style);
			~HUSD_Merge();

    bool		 addHandle(const HUSD_DataHandle &src,
				const UT_StringHolder &dest_path =
				    UT_StringHolder::theEmptyString);
    bool		 execute(HUSD_AutoWriteLock &lock) const;

private:
    class husd_MergePrivate;

    UT_UniquePtr<husd_MergePrivate>	 myPrivate;
    HUSD_MergeStyle			 myMergeStyle;
};

#endif

