/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_AgentDefinition.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_AgentDefinition__
#define __GU_AgentDefinition__

#include "GU_API.h"
#include "GU_AgentClip.h"
#include "GU_AgentLayer.h"
#include "GU_AgentRig.h"
#include "GU_AgentShapeLib.h"
#include "GU_AgentTransformGroup.h"

#include <UT/UT_Array.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_StringMap.h>

class GU_AgentDefinition;
typedef UT_IntrusivePtr<GU_AgentDefinition> GU_AgentDefinitionPtr;

/// An agent definition contains a rig, a shape library, a set of available
/// clips, a set of available layers, and a set of transform groups.
class GU_API GU_AgentDefinition : public UT_IntrusiveRefCounter<GU_AgentDefinition>
{
public:
    GU_AgentDefinition(const GU_AgentRigConstPtr &rig,
                       const GU_AgentShapeLibConstPtr &shapelib);

    /// Create a copy of the given agent definition, but referencing a new
    /// version of the shape library.
    GU_AgentDefinition(const GU_AgentDefinition &src_defn,
                       const GU_AgentShapeLibConstPtr &new_shapelib);

    int64 getMemoryUsage(bool inclusive) const;

    const GU_AgentRigConstPtr &rig() const { return myRig; }
    const GU_AgentShapeLibConstPtr &shapeLibrary() const { return myShapeLib; }

    /// Add a layer to the agent definition. Replaces any pre-existing layer of
    /// the same name.
    void addLayer(const GU_AgentLayerConstPtr &layer);

    /// Return the list of layers.
    const UT_Array<GU_AgentLayerConstPtr> &layers() const
        { sortLayersIfNeeded(); return myLayers; }

    /// Get a pointer to a specfic layer. This may be a NULL pointer.
    const GU_AgentLayer *layer(const UT_StringRef &name) const;

    /// Get a reference to the ith layer.
    const GU_AgentLayer &layer(exint i) const
	{ sortLayersIfNeeded(); return *myLayers(i); }

    /// Return the layer count.
    exint numLayers() const { return myLayers.size(); }

    /// Add a clip to the agent definition. Replaces any pre-existing clip of
    /// the same name.
    void addClip(const GU_AgentClipConstPtr &clip);

    /// Return the list of clips.
    const UT_Array<GU_AgentClipConstPtr> &clips() const
        { sortClipsIfNeeded(); return myClips; }

    /// Get a pointer to a specfic clip. This may be a NULL pointer.
    const GU_AgentClip *clip(const UT_StringRef &name) const;

    /// Get a reference to the ith clip.
    const GU_AgentClip &clip(exint i) const
	{ sortClipsIfNeeded(); return *myClips(i); }

    /// Return the clip count.
    exint numClips() const { return myClips.size(); }

    /// Add a transform group to the agent definition. Replaces any
    /// pre-existing group of the same name.
    void addTransformGroup(const GU_AgentTransformGroupConstPtr &group);

    /// Return the list of groups.
    const UT_Array<GU_AgentTransformGroupConstPtr> &transformGroups() const
        { sortTransformGroupsIfNeeded(); return myTransformGroups; }

    /// Get a pointer to a specfic group. This may be a NULL pointer.
    const GU_AgentTransformGroup *transformGroup(const UT_StringRef &name) const;

    /// Get the index of a transform group, or -1 if it does not exist.
    exint transformGroupIndex(const UT_StringRef &name) const;

    /// Get the default transform group, which contains all transforms in the
    /// rig.
    const GU_AgentTransformGroup &defaultTransformGroup() const
        { return *myDefaultTransformGroup; }

    /// Get a reference to the ith group.
    const GU_AgentTransformGroup &transformGroup(exint i) const
	{ sortTransformGroupsIfNeeded(); return *myTransformGroups(i); }

    /// Return the transform group count.
    exint numTransformGroups() const { return myTransformGroups.size(); }

private:
    void sortLayersIfNeeded() const;
    void sortClipsIfNeeded() const;
    void sortTransformGroupsIfNeeded() const;

private:
    GU_AgentRigConstPtr		myRig;
    GU_AgentShapeLibConstPtr    myShapeLib;
    UT_Array<GU_AgentLayerConstPtr> myLayers;
    UT_StringMap<exint>		myLayersIndex;
    UT_Array<GU_AgentClipConstPtr>  myClips;
    UT_StringMap<exint>		myClipsIndex;
    UT_Array<GU_AgentTransformGroupConstPtr> myTransformGroups;
    UT_StringMap<exint>		myTransformGroupsIndex;
    GU_AgentTransformGroupConstPtr myDefaultTransformGroup;
    bool			myAreLayersSorted;
    bool			myAreClipsSorted;
    bool			myAreTransformGroupsSorted;
};

#endif
