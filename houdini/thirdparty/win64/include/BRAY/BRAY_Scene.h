/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BRAY_Scene.h (RAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __RAY_BatchScene__
#define __RAY_BatchScene__

#include "BRAY_API.h"
#include "BRAY_Range.h"
#include "BRAY_SceneGraph.h"
#include "BRAY_Random.h"
#include "BRAY_RayList.h"
#include <UT/UT_UniquePtr.h>
#include <UT/UT_StringMap.h>
#include <VPRM/VPRM_Space.h>

class UT_JSONWriter;
class BRAY_Accelerator;
class BRAY_AttribList;
class BRAY_BufferList;
class BRAY_Options;
class BRAY_HitList;
class BRAY_VexMaterial;

class BRAY_API BRAY_Scene
    : public UT_NonCopyable
{
public:
    BRAY_Scene();
    ~BRAY_Scene();

    /// Set the output image planes
    void			 setOutputPlanes(const BRAY_AttribList *alist);
    const BRAY_AttribList	*outputPlanes() const { return myOutputPlanes; }

    /// Initialize for rendering
    bool	beginRender();

    /// Finish render.  This automatically unlocks the rendering options
    void	endRender();

    /// Test whether the scene is currently being rendered
    bool	isRendering() const { return myActive; }

    /// Send primary rays and get computed color
    void	shadeSamples(const BRAY_Range &range,
			const BRAY_RayList &rays,
			BRAY_BufferList &pixels) const;

    /// Trace rays
    void	trace(const BRAY_Range &range,
			const BRAY_RayList &rays,
			BRAY_HitList &hits) const;

    /// Get a VPRM_SpacePtr for the given transforms
    static VPRM_SpacePtr	getSpace(const UT_Matrix4D *xforms, int nxforms);

    /// Dump information about scene
    void	dump() const;
    void	dump(UT_JSONWriter &w) const;

    void	addObject(const BRAY_Object *obj);

    const BRAY_SceneGraph *sceneGraph() const { return mySceneGraph.get(); }

    // Return whether the accelerator is thread-safe
    bool		threadSafe() const;
    // Return whether the accelerator supports nested instancing
    bool		nestedInstancing() const;

    // Update an object
    void		updateObject(const BRAY_Object *obj, BRAY_EventType e);

    const BRAY_Accelerator	*accelerator() const { return myOctree.get(); }

    /// @{
    /// Rendering parameters
    BRAY_Options		&options() { return *myOptions; }
    const BRAY_Options		&options() const { return *myOptions; }
    /// @}

    // XXX: TEST CODE
    void setWorldToCamera(const UT_Matrix4D &world_to_camera)
	{ myWorldTransform = getSpace(&world_to_camera, 1); }

    const UT_Matrix4D &getWorldToCamera(int seg) const
	{
	    UT_ASSERT(myWorldTransform);
	    return myWorldTransform->getXform(seg);
	}

    /// Find or create a material
    BRAY_VexMaterial	*createMaterial(const UT_StringRef &name);

    /// Find an existing material
    BRAY_VexMaterial	*findMaterial(const UT_StringRef &name) const;

    /// Delete a material
    void		 destroyMaterial(const UT_StringRef &name);

private:
    const BRAY_AttribList		*myOutputPlanes;
    UT_UniquePtr<BRAY_Accelerator>	 myOctree;
    UT_UniquePtr<BRAY_SceneGraph>	 mySceneGraph;
    UT_StringMap<BRAY_VexMaterial *>	 myMaterials;
    UT_UniquePtr<BRAY_Options>		 myOptions;
    mutable UT_Lock			 myMaterialLock;
    bool				 myActive;
    bool				 myFirstRender;

    VPRM_SpacePtr			 myWorldTransform;
};

#endif
