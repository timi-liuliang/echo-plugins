/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:
 *
 * COMMENTS:
 *
 */

#ifndef DM_Drawable_H
#define DM_Drawable_H

#include "DM_API.h"
#include <GU/GU_Detail.h>
#include <GU/GU_DetailHandle.h>
#include <UT/UT_UniquePtr.h>

class UT_Color;
class DM_SceneManager;
class GUI_DetailLook;

class DM_API DM_Drawable
{
public:
    DM_Drawable(DM_SceneManager &scene_manager, const char *name, GU_Detail* detail=nullptr);
    ~DM_Drawable();

    void enable(bool flag);
    void show(bool flag);

    UT_Matrix4D transform() const;
    void setTransform(UT_DMatrix4 const & xform);

    void setWireframeMode(bool value);
    bool wireframeMode() const;
    void setWireframeColor(UT_Color const& color);

    const char* name() const;   
    GU_Detail* detail();
    GU_Detail const* detail() const;
    void acquireDetail( GU_DetailHandle const & gdh );

private:
    DM_SceneManager&		    mySceneManager;
    GU_DetailHandle		    myDetailHandle;
    UT_UniquePtr<GUI_DetailLook>    myDetailLook;
};

#endif // DM_Drawable_H

