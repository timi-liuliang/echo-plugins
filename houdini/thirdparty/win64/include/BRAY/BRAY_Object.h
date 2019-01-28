/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BRAY_Object.h (RAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __RAY_Object__
#define __RAY_Object__

#include "BRAY_API.h"
#include <UT/UT_BoundingBox.h>
#include <UT/UT_StringHolder.h>

enum
{
    // Note - when adding event types, make sure to update ::eventType()
    BRAY_EVENT_NEW_FLAG,	// Added a new object
    BRAY_EVENT_DEL_FLAG,	// Object is being deleted
    BRAY_EVENT_ATTRIB_P_FLAG,	// P attribute changed
    BRAY_EVENT_ATTRIB_FLAG,	// Generic attribute changed
    BRAY_EVENT_TOPOLOGY_FLAG,	// Topology changed
    BRAY_EVENT_XFORM_FLAG,	// Transforms changed
    BRAY_EVENT_PROTOTYPE_FLAG,	// Prototype changed
    BRAY_EVENT_CONTENTS_FLAG,	// Contents changed (add/delete objects)
    BRAY_EVENT_PROPERTIES_FLAG,	// Instance properties changed

    BRAY_EVENT_MAXFLAGS
};
UT_ASSERT_COMPILETIME(BRAY_EVENT_MAXFLAGS <= 32);
#define ENUM_FLAG(TOKEN)  BRAY_EVENT_##TOKEN = (1 << BRAY_EVENT_##TOKEN##_FLAG)
/// Types of events that can be sent as updates to the renderer
enum BRAY_EventType
{
    BRAY_NO_EVENT = 0,
    ENUM_FLAG(NEW),
    ENUM_FLAG(DEL),
    ENUM_FLAG(ATTRIB_P),
    ENUM_FLAG(ATTRIB),
    ENUM_FLAG(TOPOLOGY),
    ENUM_FLAG(XFORM),
    ENUM_FLAG(PROTOTYPE),
    ENUM_FLAG(CONTENTS),
    ENUM_FLAG(PROPERTIES),
};
#undef ENUM_FLAG

static inline BRAY_EventType
operator|(BRAY_EventType a, BRAY_EventType b)
{
    return BRAY_EventType(uint(a)|uint(b));
}

enum class BRAY_Deriv
{
    NONE,	// No derivatives
    DU,		// Evaluate derivatives w.r.t parametric u
    DV,		// Evaluate derivatives w.r.t parametric v
    DZ,		// Evaluate derivatives w.r.t z
};

class BRAY_API BRAY_Object
{
public:
    BRAY_Object();
    virtual ~BRAY_Object();

    enum ObjectType
    {
	SceneGraph,	// A scene graph (containing a mix of objects)
	GTCurveMesh,	// A curve mesh
	GTPolygonMesh,	// A polygonal mesh
	Instance,	// One or more instances of a single object
	Light,		// One or more instances of a light

	Generic
    };

    static const char		*typeName(ObjectType type);
    static UT_StringHolder	 eventType(BRAY_EventType event);

    virtual const char	*className() const { return "BRAY_Object"; }
    virtual ObjectType	 objectType() const { return Generic; }

    const BRAY_Object	*parent() const { return myParent; }
    void		 setParent(const BRAY_Object *p);

    /// Data stored by the accelerator that owns this object
    void		*acceleratorData() const { return myAcceleratorData; }
    void		 setAcceleratorData(void *d) const;

    void	dump() const;
    void	dump(UT_JSONWriter &w) const;
protected:
    /// The dump method saves out the "name" : className() pair, this method
    /// is used to save out further keys.  The writer will be in the process of
    /// saving out a map, so the method should save a set of key/value pairs.
    virtual void	 dumpKeys(UT_JSONWriter &w) const = 0;

private:
    const BRAY_Object	*myParent;
    void		*myAcceleratorData;
};

#endif
