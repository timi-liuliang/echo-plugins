/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BRAY_Accelerator.h (RAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __RAY_Accelerator__
#define __RAY_Accelerator__

#include "BRAY_API.h"
#include "BRAY_SceneGraph.h"
#include "BRAY_Range.h"
#include "BRAY_HitList.h"
#include "BRAY_RayList.h"
#include <UT/UT_NonCopyable.h>

class UT_Options;
class BRAY_BufferList;

extern "C" {
    SYS_VISIBILITY_EXPORT extern void newRAYAccelerator(void *);
}

/// This class provides an interface to different types of accelerators.  For
/// example, there could be the SideFX BVH, Embree etc.
class BRAY_API BRAY_Accelerator
    : public UT_NonCopyable
{
public:
    class Factory
    {
    public:
	virtual ~Factory();
	virtual BRAY_Accelerator	*newAccelerator(const UT_Options *o) const = 0;
    };

    BRAY_Accelerator();
    virtual ~BRAY_Accelerator();

    /// Register an accelerator
    static void		registerAccelerator(const UT_StringHolder &name,
				    Factory *factory);

    // User is responsible for deleting the accelerator
    static BRAY_Accelerator	*newAccelerator(const UT_StringRef &style,
					const UT_Options *options=nullptr);

    // Find a named factory (may return @c nullptr)
    static const Factory	*findFactory(const UT_StringRef &name);

    /// Indicate a nice name for this class
    virtual const char	*className() const = 0;

    // Hold a reference to the BRAY_Scene that holds this accelerator.
    void		 setScene(const BRAY_Scene *scene) { myScene = scene; }
    const BRAY_Scene	*getScene() const { return myScene; }

    // Invoked in the beginning of each render pass, right after BRAY_Scene's
    // beginRender() (will be after commitScene() if first render).
    virtual void	 beginRender() {}

    void	commitScene(const BRAY_SceneGraph *obj);

    /// Shade primary rays using a headlight shader (with no transparency)
    virtual void	headlight(const BRAY_Range &range,
				BRAY_HitList &hits,
				const BRAY_RayList &rays,
				BRAY_BufferList &pixels);

    /// Indicate whether accelerator performs direct rendering
    virtual bool	directRendering() const { return false; }
    /// Clear the direct rendering buffer
    virtual bool	directClear(int width, int height);
    /// Render some rays to the direct rendering buffer
    virtual void	directRender(const UT_Matrix4D &view,
				const UT_Matrix4D &projection);

    /// Perform ray-intersection on a batch of rays.  The intersector needs to
    /// properly be able to identify the source BRAY_Object and the element id
    /// for the hit.
    virtual void	sendRays(const BRAY_Range &range,
				BRAY_HitList &hits,
				const BRAY_RayList &rays,
				float max = FLT_MAX,
				float min = 0) const = 0;

    /// Return false if the accelerator can't be run on multiple threads
    virtual bool	threadSafe() const { return true; }

    /// Return false if the accelerator can't handle nested scene graphs.  If
    /// nested instancing is not supported, instances will be flattened.
    virtual bool	nestedInstancing() const { return true; }

    /// Handle updates to an object
    void		updateObject(const BRAY_Object *obj,
				BRAY_EventType event);

    virtual void	doUpdateObject(const BRAY_Object *obj,
				BRAY_EventType event) = 0;

    /// Evaluation of attributes
    const int32		*evalAttribute(int attrib,
					const BRAY_RayList &rays,
					const BRAY_Hit &hit,
					int ray_index,
					int32 *buffer,
					int tuple_size) const
			{
			    return doEvalAttrib<int32>(attrib,
				    rays, hit, ray_index,
				    buffer, tuple_size);
			}
    const int64		*evalAttribute(int attrib,
					const BRAY_RayList &rays,
					const BRAY_Hit &hit,
					int ray_index,
					int64 *buffer,
					int tuple_size) const
			{
			    return doEvalAttrib<int64>(attrib,
				    rays, hit, ray_index,
				    buffer, tuple_size);
			}
    const fpreal32	*evalAttribute(int attrib,
					const BRAY_RayList &rays,
					const BRAY_Hit &hit,
					int ray_index,
					fpreal32 *buffer,
					int tuple_size) const
			{
			    return doEvalAttrib<fpreal32>(attrib,
				    rays, hit, ray_index,
				    buffer, tuple_size);
			}
    const fpreal64	*evalAttribute(int attrib,
					const BRAY_RayList &rays,
					const BRAY_Hit &hit,
					int ray_index,
					fpreal64 *buffer,
					int tuple_size) const
			{
			    return doEvalAttrib<fpreal64>(attrib,
				    rays, hit, ray_index,
				    buffer, tuple_size);
			}
    const UT_StringHolder *evalAttribute(int attrib,
					const BRAY_RayList &rays,
					const BRAY_Hit &hit,
					int ray_index,
					UT_StringHolder *buffer,
					int tuple_size) const
			{
			    return doEvalAttrib<UT_StringHolder>(attrib,
				    rays, hit, ray_index,
				    buffer, tuple_size);
			}
protected:
    /// Refine an object generating objects in the acceleration structure.
    /// This method should handle each type of object defined in BRAY_Object.
    /// You might consider asserting if you hit an unknown object type.
    virtual void	doCommitScene(const BRAY_SceneGraph *obj) = 0;

    const BRAY_SceneGraph	*sceneGraph() const { return mySceneGraph; }

private:
    template <typename T>
    const T	*doEvalAttrib(int attrib,
			const BRAY_RayList &rays,
			const BRAY_Hit &hit,
			int ray_index,
			T *buffer,
			int tuple_size) const;

    const BRAY_SceneGraph	*mySceneGraph;
    const BRAY_Scene		*myScene;
};

#endif
