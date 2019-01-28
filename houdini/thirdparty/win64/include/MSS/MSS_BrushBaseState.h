/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	MSS_BrushBaseState.h
 *
 * COMMENTS:
 *	This is the state for interactively editing geometry
 *	and attributes, used with the Brush SOPs.
 */

#ifndef __MSS_BrushBaseState_H__
#define __MSS_BrushBaseState_H__

#include "MSS_API.h"
#include <DM/DM_Detail.h>
#include <DM/DM_ModifierKeys.h>
#include <GA/GA_Types.h>
#include <GU/GU_Detail.h>
#include <GU/GU_RayIntersect.h>
#include <SOP/SOP_BrushBase.h>
#include <UT/UT_Vector2.h>
#include "MSS_SingleOpState.h"

class UT_String;
class PRM_Template;
class DM_Workbench;
class JEDI_View;

class MSS_API MSS_BrushBaseState : public MSS_SingleOpState
{
public:
    // Class constructor and destructor.
	     MSS_BrushBaseState(JEDI_View &view, PI_StateTemplate &templ,
			        BM_SceneManager *scene,
			        const char *cursor = BM_DEFAULT_CURSOR);
    virtual ~MSS_BrushBaseState();

    // This constructor and parameter template list go into the DM_StateTemplate
    // for this state.
    static BM_State	*ourConstructor(BM_View &view, PI_StateTemplate &templ,
					BM_SceneManager *scene);
    static PRM_Template	*ourTemplateList;

    // The name and type of this class:
    virtual const char	*className() const;

    // Restart the selection...
    virtual void         restart();

    virtual void	 handleOpNodeChange(OP_Node &node);

    // Check if MMB can be used for indirect handle drags.  Some states use
    // the MMB themselves.
    virtual bool	 getAllowIndirectHandleDrag() const { return false; }

protected:
    virtual int		 enter(BM_SimpleState::BM_EntryType how);
    virtual void	 exit (void);
    virtual void	 interrupt(BM_SimpleState * = 0);
    virtual void	 resume   (BM_SimpleState * = 0);

    // Set the state's node
    virtual void	 setNode(OP_Node *node);

    // Generate the necessary nodes and connect them
    virtual bool         generateAllNodes();

    // We're done selecting and we've been successful. In this class we simply 
    // display the state prompt.
    virtual void	 doneSelecting(void);
    
    // Respond to mouse or keyboard events.
    virtual int		 handleMouseEvent(UI_Event *event);
    virtual int		 handleMouseWheelEvent(UI_Event *event);
    virtual int		 handleKeyTypeEvent(UI_Event *event, BM_Viewport &);
    virtual int		 handleParameterEvent(UI_Event *event);
    
    // Render the brush "cursor" geometry:
    virtual void	 doRender(RE_Render *r, int x, int y, int ghost);

    // Convert an op menu entry to a brush operation:
    virtual SOP_BrushOp	 menuToBrushOp(const UI_Value &menuvalue) const;
    virtual int		 brushopToMenu(SOP_BrushOp op) const;
    
    // If this state is reselecting and it tries to reselect within the same
    // node then that node's unique id is returned.  Otherwise, 0 is returned.
    virtual int		 nodeIdToReselectWithin() const;

    UI_Value		&getPrimaryVal  () { return myPrimaryButtonVal; }
    UI_Value		&getSecondaryVal() { return mySecondaryButtonVal; }

    SOP_BrushBase	*getBrushSOP() const
			 { return (SOP_BrushBase*)BM_SingleOpState::getNode(); }

    virtual void	 updatePrompt();

    virtual bool	 forceVisualization() const { return true; }

    virtual void	 enableVisualization();
    virtual void	 disableVisualization();

    const DM_ModifierKeys	&getModifierKeys() const
						{ return myModifierKeys; }

    virtual SOP_Node *updateBrushIntersectSop(
        SOP_BrushBase *brush_sop,
        SOP_Node *intersect_sop,
        fpreal t)
    { return nullptr; }

private:
    void		 handleBrushShapeChange (UI_Event *event);
    void		 handleBrushOrientChange(UI_Event *event);
    void		 handleAccumStencilChange(UI_Event *event);
    void		 handlePrimaryTieChange(UI_Event *event);
    void		 handleSecondaryTieChange(UI_Event *event);

    // Set the appropriate operation whether we use the primary or secondary
    // mouse button.
    void		 usePrimaryButton  (void);
    void		 useSecondaryButton(void);

    int			 sendRay( const UT_Vector2& screen_coord, bool isuv,
				 fpreal t, UT_Vector3 &rayorig,
				 UT_Vector3 &hitpos,
				 UT_Vector3 &dir, GU_RayInfo &hinfo,
				 GA_Index &hitptind,
				 float &scalehistory,
				 int checkgeo);

    // This orients the cursor.
    void		 orientCursor(fpreal t, const UT_Vector3 &hitpos,
				      const UT_Vector3 &nml,
				      float scalehistory,
				      bool isuv, GA_Index prim,
				      float u, float v, float w);
	    
    // This rebuilds the cursor orientation from the last stashed
    // value.
    void		 reorientCursor();
    

    DM_ModifierKeys	 myModifierKeys;
    DM_ModifierKeys	 myFinishModifierKeys;
    DM_ModifierKeys	 myWheelModifierKeys;
    SOP_BrushBase	*mySavedBrushNode;
    GU_RayIntersect 	 myRayIntersect;
    DM_Detail		 myBrushHandle;
    GU_Detail		 myBrushCursor;
    UT_Matrix4		 myBrushCursorXform;
    bool		 myRenderBrushCursor;
    bool		 myBrushCursorIsUV;
    bool		 myOneHit;
    bool		 myLocatedFlag;
    UT_Vector2		 myOldCoord;

    // These are used track the resizing of the cursor in the viewport
    bool		 myResizingCursor;
    int			 myLastCursorX, myLastCursorY;

    // This is used to stash the last valid cursor orientation.
    // This allows us to rebuild the orientation to resize on
    // the users request without reintersecting.
    bool		 myOldOrientValid;
    fpreal		 myOldOrientT;
    UT_Vector3		 myOldOrientHitPos;
    UT_Vector3		 myOldOrientHitNml;
    float		 myOldOrientScaleHistory;
    bool		 myOldOrientIsUV;
    GA_Index		 myOldOrientHitPrim;
    float		 myOldOrientHitU;
    float		 myOldOrientHitV;
    float		 myOldOrientHitW;

    UI_Value		 myPrimaryButtonVal;
    UI_Value		 mySecondaryButtonVal;
    UI_Value		 myBrushShapeVal;
    UI_Value		 myBrushOrientVal;
    UI_Value		 myAccumStencilVal;
};

#endif
