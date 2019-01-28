/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	MSS_SingleOpBaseState.h
 *
 * COMMENTS:
 *	Parent class for SOP custom states.
 *
 */

#ifndef __MSS_SingleOpBaseState_H__
#define __MSS_SingleOpBaseState_H__

#include "MSS_API.h"
#include <BM/BM_SingleOpState.h>

class DM_Workbench;
class OP3D_View;
class JEDI_View;

/// Parent class for SOP custom states.
class MSS_API MSS_SingleOpBaseState : public BM_SingleOpState
{
public:
    MSS_SingleOpBaseState(JEDI_View &view, PI_StateTemplate &templ,
			  BM_SceneManager *scene,
			  const char *cursor = BM_DEFAULT_CURSOR);
    virtual ~MSS_SingleOpBaseState(void);

    virtual const char	*className() const;

protected:
    virtual const char	*getPresetTypeName() const { return "MSS"; }
    
    /// Provide the node to work with. The criteria for which node to use will
    /// be specific to each network type. This method does NOT overwrite
    /// the myOpNode pointer.
    virtual OP_Node	*getChosenOp(void) const;

    /// Set the flags (display/current) of the given node:
    virtual void	 setNodeFlags(OP_Node &node);

    /// Respond to keyboard events
    virtual int		 handleKeyTypeEvent(UI_Event *event, BM_Viewport &);

    // This depends on the secure selection button and 
    // whether a selector is active.
    virtual bool         isSelecting() const;
    virtual bool         needSelectMask() const { return true; }

    /// Map viewport coordinates to worldspace location and direction
    void		 mapToWorld(float x, float y,
				    UT_Vector3 &dir, UT_Vector3 &rayorig);

    /// Transform a point from object to world space or world to object space.
    void		 xformToWorldCoord(UT_Vector3 &p);
    void		 xformToObjectCoord(UT_Vector3 &p);

    /// Transform a vector from object to world space or world to object space.
    void		 xformToWorldVector(UT_Vector3 &v);
    void		 xformToObjectVector(UT_Vector3 &v);

    /// Get worldspace to cameraspace transform
    void		 getViewportTransform(UT_Matrix4 &xform);

    /// Get cameraspace to worldspace transform
    void		 getViewportItransform(UT_Matrix4 &xform);

    /// Get the projection transform
    void		 getViewportProjectionTransform(
			    UT_Matrix4 &xform) const;
    /// Get the projection parameters. @see UT_Matrix4::perspective()
    bool		 getViewportProjection(
			    fpreal &focal_length,
			    fpreal &horizontal_aperture,
			    bool &is_ortho,
			    fpreal &ortho_width,
			    fpreal &aspect_ratio,
			    fpreal &near_plane,
			    fpreal &far_plane
			    ) const;

    /// Get level of detail for the viewport
    float		 getViewportLOD() const;

    /// Set the status bar text
    void		 showPrompt(const char *msg);

	  JEDI_View	&viewer()	{ return (JEDI_View &)baseViewer(); }
    const JEDI_View	&viewer() const	{ return (JEDI_View &)baseViewer(); }

	  DM_Workbench	&workbench()
				    { return (DM_Workbench &)sceneManager(); }
    const DM_Workbench	&workbench() const
				    { return (DM_Workbench &)sceneManager(); }
    
private:
};

#endif
