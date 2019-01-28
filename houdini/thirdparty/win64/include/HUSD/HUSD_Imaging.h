/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_Imaging_h__
#define __HUSD_Imaging_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include "HUSD_Scene.h"
#include <UT/UT_NonCopyable.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_Rect.h>

class HUSD_Scene;

class HUSD_API HUSD_Imaging : public UT_NonCopyable
{
public:
			 HUSD_Imaging();
			~HUSD_Imaging();

    // The scene is not owned by this class.
    void setScene(HUSD_Scene *scene_ref);

    // only the USD modes that map to ours
    enum DrawMode
    {
	DRAW_WIRE,
	DRAW_SHADED_NO_LIGHTING,
	DRAW_SHADED_FLAT,
	DRAW_SHADED_SMOOTH,
	DRAW_WIRE_SHADED_SMOOTH
    };

    void		 showPurposeRender(bool enable);
    void		 showPurposeProxy(bool enable);
    void		 showPurposeGuide(bool enable);

    void		 setDrawMode(DrawMode mode);
    void		 setDrawComplexity(float complexity);
    void		 setStage(const HUSD_DataHandle &data_handle,
				const HUSD_ConstOverridesPtr &overrides);
    void		 setSelection(const UT_StringArray &paths);
    bool		 setFrame(fpreal frame);
    bool		 setHeadlight(bool doheadlight);
    void		 setLighting(bool enable);
    bool		 render(const UT_Matrix4F &view_matrix,
				const UT_Matrix4F &proj_matrix,
				const UT_DimRect &viewport_rect,
				const UT_StringRef &renderer,
				const UT_Options *render_options,
				bool update_deferred);

    bool		 getBoundingBox(UT_BoundingBox &bbox,
				const UT_Matrix3R *rot) const;

    HUSD_Scene		&scene()
			 { return *myScene; }
    bool		 isConverged() const
			 { return myConverged; }

    static bool		 getAvailableRenderers(UT_StringArray &renderers,
					       UT_StringArray &rentokens);

    void		terminateRender();
private:
    class husd_ImagingPrivate;

    void	updateLightsAndCameras();
    void	updateDeferredPrims();

    UT_UniquePtr<husd_ImagingPrivate>	 myPrivate;
    fpreal				 myFrame;
    HUSD_DataHandle			 myDataHandle;
    HUSD_ConstOverridesPtr		 myOverrides;
    UT_StringArray			 mySelection;
    unsigned				 myWantsHeadlight : 1,
					 myHasHeadlight : 1,
					 myDoLighting : 1,
					 myHasLightCamPrims : 1,
					 myHasGeomPrims : 1,
					 mySelectionNeedsUpdate : 1,
					 myConverged : 1;
    HUSD_Scene				*myScene;
    UT_StringHolder			 myRenderName;
};

#endif

