/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_AgentTransformGroup.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_AgentTransformGroup__
#define __GU_AgentTransformGroup__

#include "GU_API.h"

#include "GU_AgentRig.h"

#include <UT/UT_BitArray.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_StringHolder.h>

class UT_JSONParser;
class UT_JSONWriter;

class GU_AgentTransformGroup;
using GU_AgentTransformGroupPtr = UT_IntrusivePtr<GU_AgentTransformGroup>;
using GU_AgentTransformGroupConstPtr =
    UT_IntrusivePtr<const GU_AgentTransformGroup>;
using GU_AgentTransformGroupArray = UT_Array<GU_AgentTransformGroupConstPtr>;

/// Specifies a subset of the transforms in the agent's rig, which is useful
/// for things such as clip blending.
class GU_API GU_AgentTransformGroup
    : public UT_IntrusiveRefCounter<GU_AgentTransformGroup>
{
public:
    /// Used to store the result of parsing the JSON file.
    struct GroupData
    {
        UT_StringHolder myName;
        UT_StringArray myTransformNames;
        UT_FprealArray myWeights;
    };

    static GU_AgentTransformGroupPtr addGroup(const UT_StringHolder &name,
                                              const GU_AgentRigConstPtr &rig);
    static GU_AgentTransformGroupPtr addGroupFromFile(
                                              const UT_StringHolder &filename,
                                              const GU_AgentRigConstPtr &rig,
                                              UT_StringArray &errors);

private:
    GU_AgentTransformGroup(
        const GU_AgentRigConstPtr &rig, const UT_StringHolder &name,
        const UT_StringHolder &filename = UT_StringHolder::theEmptyString);

public:
    ~GU_AgentTransformGroup();

    /// Build from a list of transform indices and weights.
    bool construct(const UT_ExintArray &transforms,
                   const UT_FprealArray &weights, UT_StringArray &errors);

    int64 getMemoryUsage(bool inclusive) const;

    /// Name of the group.
    const UT_StringHolder &name() const { return myName; }

    /// Returns whether the group was loaded from disk.
    bool isFile() const { return myFileName.isstring(); }
    /// Clear the flag marking that the group references a file on disk.
    void clearIsFile();
    /// Returns the file that the group was loaded from.
    const UT_StringHolder &fileName() const
    {
        UT_ASSERT(isFile());
        return myFileName;
    }

    /// The referenced rig.
    const GU_AgentRigConstPtr &rig() const { return myRig; }

    /// Ordered list of transform indices in the group.
    const UT_BitArray &transforms() const { return myTransforms; }

    /// Returns the weight of the specified joint (0 is returned if the joint
    /// is not in the transform group).
    fpreal weight(exint i) const { return myWeights[i]; }

    /// Returns true if all joints in the group have a weight of 1.
    bool hasTrivialWeights() const { return myHasTrivialWeights; }

    /// Save to JSON.
    bool save(UT_JSONWriter &w) const;
    /// Load from JSON.
    bool load(UT_JSONParser &p);
    /// @{
    /// Parse the JSON file into an intermediate format, which can be used to
    /// finish loading the group at a later time (e.g. after the rig has also
    /// been loaded).
    static bool load(UT_JSONParser &p, GroupData &data);
    bool        load(const GroupData &data, UT_StringArray &errors);
    /// @}

private:
    UT_BitArray myTransforms;
    UT_FprealArray myWeights;
    bool myHasTrivialWeights;
    GU_AgentRigConstPtr myRig;
    UT_StringHolder myName;
    UT_StringHolder myFileName;
};

#endif
