/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	ROP_RenderManager.h ( ROP Library, C++)
 *
 * COMMENTS:
 *    This analyzes a net of ROPs, and produces a render ordering for them.
 */
#ifndef ROP_RenderManager_h
#define ROP_RenderManager_h

#include "ROP_API.h"
#include <UT/UT_ValArray.h>
#include "ROP_RenderList.h"

class ROP_RenderCmdParms;
class ROP_RenderItem;
class ROP_Node;
class ROP_Verbose;

class ROP_API ROP_RenderManager
{
public:

    bool	render(ROP_Node *render_node,
		       fpreal startframe, fpreal endframe, fpreal finc,
		       bool single_rop,
		       bool frame_by_frame,
		       bool ignore_bypass,
		       bool ignore_locks,
		       bool merge_frames,
		       ROP_Verbose *verbose);
    
    // returns the order in which to render frames in 'order'. 
    void	buildFrameOrder(ROP_RenderList &order,
				ROP_Node *render_node,
				fpreal startframe, fpreal endframe, fpreal finc,
				bool single_rop,
				bool frame_by_frame,
				bool ignore_bypass,
				bool ignore_locks,
				bool merge_frames);

    // returns true if all successfully rendered.
    bool	renderOrder(const ROP_RenderList &order,
			       ROP_Verbose *verbose);

    void	getFrameOrder(ROP_Node *render_node,
			      fpreal startframe, fpreal endframe, fpreal finc,
			      ROP_RenderCmdParms &render_parms);

    // returns the range that the user specified for the render.
    void	getOutputFrameRange(fpreal &start, fpreal &end,
				    fpreal &finc) const;

    bool	isActive() const { return (myRenderOrder != NULL); }
    
    int		getNumRenderedFrames(ROP_Node *for_node) const;

    void	addPostProcessor(ROP_Node *node);

    // copies the current post processor list into nodes, and starts a new list.
    // Do not touch the returned node list; pass it back to popPostProcessors()
    void	pushPostProcessors(UT_ValArray<ROP_Node *> &nodes);
    
    // post processes 'list' with any post processes, then restores the old
    // post processor list from nodes.
    void	popPostProcessors(ROP_RenderList &list,
				  const ROP_RenderDepParms &parms,
				  UT_ValArray<ROP_Node *> &nodes);

    static ROP_RenderManager	*pushManager();	// Returns new manager
    static ROP_RenderManager	*getManager();	// Returns top of stack
    static void			 popManager();	// Must be one for every push
    
private:
    ROP_RenderManager();
    
    // called by ROP_Node only.
    void       setCurrentPercent(float p);

    UT_ValArray<ROP_Node *> myPostProcessors;
    fpreal	myFrameStart;
    fpreal	myFrameEnd;
    fpreal	myFrameInc;
    ROP_RenderList *myRenderOrder;
    static int	theRenderSerial;
    int		theCurrentCount;
    int		theTotalCount;
    int		theCurrentTotal;

    
    friend class ROP_Node;
};

class ROP_API ROP_RenderManagerAutoPush
{
public:
     ROP_RenderManagerAutoPush()	{ ROP_RenderManager::pushManager(); }
    ~ROP_RenderManagerAutoPush()	{ ROP_RenderManager::popManager(); }
};

#endif
