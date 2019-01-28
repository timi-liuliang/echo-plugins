/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BRAY_SceneGraph.h (RAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __BRAY_SceneGraph__
#define __BRAY_SceneGraph__

#include "BRAY_Object.h"
#include "BRAY_Hit.h"
#include "BRAY_Instance.h"
#include <UT/UT_Lock.h>
#include <UT/UT_Array.h>
#include <UT/UT_Set.h>
#include <VPRM/VPRM_Space.h>

class UT_JSONWriter;
class BRAY_Light;

class BRAY_API BRAY_SceneGraph : public BRAY_Object
{
public:
    using ObjectType = BRAY_Object::ObjectType;
    using ObjectList = UT_Array<const BRAY_Object *>;
    using LightSet = UT_Set<const BRAY_Light *>;
    using iterator = ObjectList::iterator;
    using const_iterator = ObjectList::const_iterator;

    using light_iterator = LightSet::iterator;
    using light_const_iterator = LightSet::const_iterator;

    BRAY_SceneGraph();
    virtual ~BRAY_SceneGraph();

    /// @{
    /// BRAY_Object interface
    virtual const char	*className() const override final
		    { return "BRAY_SceneGraph"; }
    ObjectType	objectType() const override final
		    { return BRAY_Object::SceneGraph; }
    /// @}

    /// Add an object to the scene graph.
    void	addObject(const BRAY_Object *obj);

    /// @{
    /// Manage lights
    void	addLight(const BRAY_Light *lp);
    void	eraseLight(const BRAY_Light *lp);
    /// @}

    /// Return the number of objets in the scene graph
    exint	size() const { return myObjects.size(); }

    /// Return the number of ligths in the scene
    exint	lightCount() const { return myLights.size(); }

    /// @{
    /// Light iteration
    light_iterator		beginLights() { return myLights.begin(); }
    light_iterator		endLights() { return myLights.end(); }
    light_const_iterator	beginLights() const { return myLights.begin(); }
    light_const_iterator	endLights() const { return myLights.end(); }
    /// @}

    const VPRM_SpacePtr	&space() const { return mySpace; }
    void	setSpace(const VPRM_SpacePtr &s) { mySpace = s; }

    /// @{
    /// Object iteration
    iterator		begin() { return myObjects.begin(); }
    iterator		end() { return myObjects.end(); }
    const_iterator	begin() const { return myObjects.begin(); }
    const_iterator	end() const { return myObjects.end(); }
    /// @}

    const BRAY_Instance	*findInstance(const BRAY_Hit::HitStack &stack,
				int tail_offset=0) const
    {
	return UTverify_cast<const BRAY_Instance *>(
		findObject(stack, tail_offset+1));
    }
    const BRAY_Object	*findGeometry(const BRAY_Hit::HitStack &stack) const
			    { return findObject(stack, 0); }

private:
    const BRAY_Object	*findObject(const BRAY_Hit::HitStack &stack,
				int tail_offset) const;
    virtual void	dumpKeys(UT_JSONWriter &w) const override;

    ObjectList		myObjects;
    LightSet		myLights;
    VPRM_SpacePtr	mySpace;
    UT_Lock		myLock;
};

#endif
