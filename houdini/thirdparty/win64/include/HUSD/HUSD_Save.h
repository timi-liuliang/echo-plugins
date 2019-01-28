/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_Save_h__
#define __HUSD_Save_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_UniquePtr.h>
#include <SYS/SYS_Types.h>

enum HUSD_SaveStyle {
    HUSD_SAVE_FLATTENED_LAYERS,
    HUSD_SAVE_FLATTENED_EXPLICIT_LAYERS,
    HUSD_SAVE_FLATTENED_STAGE,
    HUSD_SAVE_SEPARATE_LAYERS,
};

class HUSD_API HUSD_Save
{
public:
			 HUSD_Save();
			~HUSD_Save();

    bool		 addCombinedTimeSample(const HUSD_AutoReadLock &lock);
    bool		 saveCombined(const UT_StringRef &filepath,
				const UT_StringRef &sublayerpath) const;
    bool		 save(const HUSD_AutoReadLock &lock,
				const UT_StringRef &filepath,
				const UT_StringRef &sublayerpath) const;

    HUSD_SaveStyle	 saveStyle() const
			 { return mySaveStyle; }
    void		 setSaveStyle(HUSD_SaveStyle save_style)
			 { mySaveStyle = save_style; }

    bool		 clearHoudiniCustomData() const
			 { return myClearHoudiniCustomData; }
    void		 setClearHoudiniCustomData(bool clear_data)
			 { myClearHoudiniCustomData = clear_data; }

    bool		 flattenFileLayers() const
			 { return myFlattenFileLayers; }
    void		 setFlattenFileLayers(bool flatten_file_layers)
			 { myFlattenFileLayers = flatten_file_layers; }

    bool		 flattenSopLayers() const
			 { return myFlattenSopLayers; }
    void		 setFlattenSopLayers(bool flatten_sop_layers)
			 { myFlattenSopLayers = flatten_sop_layers; }

    bool		 saveEmptyLayers() const
			 { return mySaveEmptyLayers; }
    void		 setSaveEmptyLayers(bool save_empty_layers)
			 { mySaveEmptyLayers = save_empty_layers; }

    fpreal64		 startFrame() const
			 { return myStartFrame; }
    void		 setStartFrame(fpreal64 start_time = -SYS_FP64_MAX)
			 { myStartFrame = start_time; }

    fpreal64		 endFrame() const
			 { return myEndFrame; }
    void		 setEndFrame(fpreal64 end_time = SYS_FP64_MAX)
			 { myEndFrame = end_time; }

    fpreal64		 timeCodesPerSecond() const
			 { return myTimeCodesPerSecond; }
    void		 setTimeCodesPerSecond(fpreal64 tps = SYS_FP64_MAX)
			 { myTimeCodesPerSecond = tps; }

    fpreal64		 framesPerSecond() const
			 { return myFramesPerSecond; }
    void		 setFramesPerSecond(fpreal64 fps = SYS_FP64_MAX)
			 { myFramesPerSecond = fps; }

private:
    class		 husd_SavePrivate;

    UT_UniquePtr<husd_SavePrivate>	 myPrivate;
    HUSD_SaveStyle			 mySaveStyle;
    fpreal64				 myStartFrame;
    fpreal64				 myEndFrame;
    fpreal64				 myTimeCodesPerSecond;
    fpreal64				 myFramesPerSecond;
    bool				 myClearHoudiniCustomData;
    bool				 myFlattenFileLayers;
    bool				 myFlattenSopLayers;
    bool				 mySaveEmptyLayers;
};

#endif

