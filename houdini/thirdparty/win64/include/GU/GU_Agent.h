/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Agent.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_Agent__
#define __GU_Agent__

#include "GU_API.h"
#include "GU_AgentClip.h"
#include "GU_AgentDefinition.h"
#include "GU_PackedImpl.h"

#include <GA/GA_PrimitiveTypeId.h>

#include <UT/UT_Array.h>
#include <UT/UT_BitArray.h>
#include <UT/UT_Map.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_TBBSpinLock.h>
#include <UT/UT_VectorTypes.h>
#include <SYS/SYS_Types.h>


#define GU_AGENT_LAYER_DEFAULT	    "default"
#define GU_AGENT_LAYER_COLLISION    "collision"


class UT_Options;


/// A packed agent
///
/// The agent is composed of multiple named layers.  Each layer consists of
/// zero or more shapes.
///
/// All agents must have a default layer.  This is the layer used when a
/// specific layer isn't found.  Example layers might be: "default", "low-res",
/// "medium-res", "high-quality", "collision".
///
/// The packed agent conforms to the GU_PackedImpl API, but also provides
/// access to the layers/shapes.
class GU_API GU_Agent : public GU_PackedImpl
{
public:
    typedef UT_StringHolder			StringType;
    typedef fpreal32				FloatType;
    typedef UT_Matrix4T<FloatType>		Matrix4Type;
    typedef UT_Array<Matrix4Type>		Matrix4Array;
    typedef UT_SharedPtr<Matrix4Array>		Matrix4ArrayPtr;
    typedef UT_SharedPtr<const Matrix4Array>	Matrix4ArrayConstPtr;

    enum BlendMode
    {
        BLEND_INTERPOLATE,
        BLEND_ADDITIVE,
        NUM_BLEND_MODES
    };

    struct ClipLayer
    {
        ClipLayer(BlendMode blend_mode, fpreal64 weight = 0.0,
                  int64 parent = -1)
            : myBlendMode(blend_mode), myWeight(weight), myParent(parent)
        {
        }

        BlendMode myBlendMode;
        fpreal64 myWeight;
        int64 myParent;
    };

    /// State of the primitive
    struct State
    {
	GU_AgentClipPtrArray	    myClips;
	UT_Fpreal64Array	    myClipTimes;
	UT_Fpreal64Array	    myClipWeights;
        GU_AgentTransformGroupArray myClipXformGroups;
        UT_Int64Array               myClipLayerIds;

        UT_Array<ClipLayer>         myClipLayers;
    };

    GU_Agent();
    GU_Agent(const GU_Agent &src);
    virtual ~GU_Agent();

    /// Register the packed primitive type
    static void			 install(GA_PrimitiveFactory *pfact);

    /// Get the type ID for the GU_Agent primitive type.
    static GA_PrimitiveTypeId typeId()
    {
        return theTypeId;
    }

    /// Convenience method to create a packed agent primitive
    static GU_PrimPacked	*agent(GU_Detail &dest,
					const StringType &default_layer
						    = UTmakeUnsafeRef(GU_AGENT_LAYER_DEFAULT),
					const StringType &collision_layer
						    = UTmakeUnsafeRef(GU_AGENT_LAYER_COLLISION),
					GA_Offset ptoff = GA_INVALID_OFFSET);


    static bool			 hasAgentPackedPrimitives(
					const GU_Detail &gdp);

    /// @{
    /// API defined on GU_PackedImpl
    virtual GU_PackedFactory	*getFactory() const;
    virtual GU_Agent	*copy() const;
    virtual bool	 isValid() const;
    virtual void	 clearData();

    virtual bool	supportsJSONLoad() const
			    { return true; }
    virtual bool	load(GU_PrimPacked *prim, const UT_Options &options, const GA_LoadMap &map)
			    { return loadFrom(prim, options, map); }
    virtual bool	loadFromJSON(GU_PrimPacked *prim, const UT_JSONValueMap &options,
				const GA_LoadMap &map)
			    { return loadFrom(prim, options, map); }

    virtual void	 update(GU_PrimPacked *prim, const UT_Options &options)
			    { updateFrom(prim, options); }
    virtual bool	 saveSharedData(UT_JSONWriter &w, GA_SaveMap &map,
                                        GA_GeometryIndex *geometryIndex) const;
    virtual bool	 loadSharedData(int load_data_type,
				const GA_SharedLoadData *item);
    virtual bool	 save(UT_Options &options,
				const GA_SaveMap &map) const;
    virtual bool	 getBounds(UT_BoundingBox &box) const;
    virtual bool	 getRenderingBounds(UT_BoundingBox &box) const;
    virtual void	 getVelocityRange(
				UT_Vector3 &vmin,
				UT_Vector3 &vmax) const;
    virtual void	 getWidthRange(fpreal &wmin, fpreal &wmax) const;
    virtual bool         unpack(GU_Detail &destgdp, const UT_Matrix4D *transform) const;
    virtual bool         unpackWithStyler(
        GU_Detail &destgdp,
        STY_StylerGroup &prim_styler_group,
        const STY_Styler &parent_styler,
        const GU_PrimPacked *prim) const;
    virtual GU_ConstDetailHandle	getPackedDetail(GU_PackedContext *context = 0) const;

    /// Report memory usage (includes all shared memory)
    virtual int64 getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;
    /// @}

    /// Return the agent's definition (which contains e.g. available clips and
    /// layers).
    SYS_FORCE_INLINE GU_AgentDefinition &definition()
    {
        loadDefinitionIfNeeded();
        return *myDefinition;
    }

    SYS_FORCE_INLINE const GU_AgentDefinition &definition() const
    {
        loadDefinitionIfNeeded();
        return *myDefinition;
    }

    void setDefinition(GU_PrimPacked *prim, const GU_AgentDefinitionPtr &definition);

    /// Rig Accessor
    /// @{
    const GU_AgentRigConstPtr &getRig() const { return definition().rig(); }
    /// @}

    /// @{
    /// Packed implementation for specific layers
    bool	layerBounds(const GU_AgentLayer &layer,
			UT_BoundingBox &box) const;
    void	layerVelocityRange(const GU_AgentLayer &layer,
			UT_Vector3 &vmin, UT_Vector3 &vmax) const;
    void	layerWidthRange(const GU_AgentLayer &layer,
			fpreal &wmin, fpreal &wmax) const;
    bool        layerUnpack(
        const GU_AgentLayer &layer,
        GU_Detail &destgdp,
        const UT_Matrix4D *transform,
        STY_StylerGroup *prim_styler_group=nullptr,
        const STY_Styler *parent_styler=nullptr,
        const GU_PrimPacked *prim=nullptr) const;
    GU_ConstDetailHandle
		layerPackedDetail(const GU_AgentLayer &layer) const;
    /// @}

    /// Return the current layer. This may be a NULL pointer.
    const GU_AgentLayer	*getCurrentLayer() const;
    /// Return the current collision layer. This may be a NULL pointer.
    const GU_AgentLayer	*getCollisionLayer() const;

    /// @{
    /// Set the default layer (for packing, bounds, etc)
    StringType	currentLayerName() const { return myCurrentLayer; }
    StringType  intrinsicCurrentLayerName(const GU_PrimPacked *prim) const { return currentLayerName(); }
    void	setCurrentLayerName(GU_PrimPacked *prim, const StringType &name);
    /// @}

    /// @{
    /// Set the collision layer.
    StringType	collisionLayerName() const { return myCollisionLayer; }
    StringType  intrinsicCollisionLayerName(const GU_PrimPacked *prim) const { return collisionLayerName(); }
    void	setCollisionLayerName(GU_PrimPacked *prim, const StringType &name);
    /// @}

    /// Array of clips used for transform evaluation
    /// @{
    const GU_AgentClipPtrArray& clips() const;
    void                setClips(GU_PrimPacked *prim, const GU_AgentClipPtrArray& clips);
    void                setClipsByNames(GU_PrimPacked *prim, const UT_StringArray& clip_names);
    /// @}

    /// Return the number of clips used for transform evaluation
    exint		numClips() const
			    { return clips().entries(); }
    exint               intrinsicNumClips(const GU_PrimPacked *prim) const
    { return numClips(); }

    /// Names of the clips used for transform evaluation
    const char*		clipName(exint i) const;
    const char*         intrinsicClipName(const GU_PrimPacked *prim, exint i) const
    { return clipName(i); }

    /// Set a clip by name
    void		setClipByName(GU_PrimPacked *prim, exint i, const char *clip_name);

    /// Array of times in seconds to evaluate the clips.
    /// Indices in [0,numClips()-1] inclusively.
    /// @{
    fpreal		clipTime(exint i) const
			    { return myState.myClipTimes(i); }
    fpreal              intrinsicClipTime(const GU_PrimPacked *prim, exint i) const
    { return clipTime(i); }
    void		setClipTime(GU_PrimPacked *prim, exint i, fpreal seconds);
    /// @}

    /// Array of weights to evaluate the corresponding clips. Assumes they are
    /// normalized.
    /// @{
    fpreal		clipWeight(exint i) const
			    { return myState.myClipWeights(i); }
    fpreal              intrinsicClipWeight(const GU_PrimPacked *prim, exint i) const
    { return clipWeight(i); }
    void		setClipWeight(GU_PrimPacked *prim, exint i, fpreal weight);
    /// @}

    /// The subset of transforms in the rig that each clip should be applied
    /// to.
    /// @{
    const char*         clipTransformGroupName(exint i) const;
    const char*         intrinsicClipTransformGroupName(const GU_PrimPacked *prim, exint i) const
    { return clipTransformGroupName(i); }
    void                setClipTransformGroupByName(GU_PrimPacked *prim, exint i, const char *name);
    /// @}

    /// The layer that each clip belongs to.
    /// @{
    exint               clipLayerId(exint i) const
                            { return myState.myClipLayerIds(i); }
    exint               intrinsicClipLayerId(const GU_PrimPacked *prim, exint i) const
    { return clipLayerId(i); }
    void                setClipLayerId(GU_PrimPacked *prim, exint i, exint layer_id);
    /// @}

    /// List of clip layers and their blend modes.
    /// @{
    void                setClipLayers(GU_PrimPacked *prim, const UT_Array<ClipLayer> &clip_layers);
    exint               numClipLayers() const
                            { return myState.myClipLayers.entries(); }
    /// @}

    /// Get the world transforms for this agent. Disabling interpolation can
    /// improve performance if the clip time falls between two samples.
    bool		computeWorldTransforms(Matrix4ArrayConstPtr &xforms,
                                               bool interpolate_samples = true) const
                            { return computeTransforms(xforms,
                                                       /* world_space */ true,
                                                       interpolate_samples); }
    /// Get the local transforms for this agent.
    bool		computeLocalTransforms(Matrix4ArrayConstPtr &xforms,
                                               bool interpolate_samples = true) const
                            { return computeTransforms(xforms,
                                                       /* world_space */ false,
                                                       interpolate_samples); }
    /// Get a specific world transform for the agent.
    bool                computeWorldTransform(Matrix4Type &xform, exint i,
                                              bool interpolate_samples = true) const
                            { return computeWorldTransformInternal(xform, i, interpolate_samples); }
    /// Get a specific local transform for the agent.
    bool                computeLocalTransform(Matrix4Type &xform, exint i,
                                              bool interpolate_samples = true) const
                            { return computeLocalTransformInternal(xform, i, interpolate_samples); }

    /// Override the world transforms for this agent.
    void                setWorldTransforms(const Matrix4Array &xforms)
			    { setTransforms(xforms, /* world_space */ true); }
    /// Override the local transforms for this agent.
    void                setLocalTransforms(const Matrix4Array &xforms)
			    { setTransforms(xforms, /* world_space */ false); }
    /// Override a specific world transform for this agent.
    void                setWorldTransform(const Matrix4Type &xform, exint i);
    /// Override a specific local transform for this agent.
    void                setLocalTransform(const Matrix4Type &xform, exint i);
    /// Returns whether the agent's transforms have been overridden from the
    /// blended clips.
    bool                hasCustomTransforms() const { return myHasCustomXforms; }
    bool                intrinsicHasCustomTransforms(const GU_PrimPacked *) const
                            { return hasCustomTransforms(); }
    /// Number of transforms in the rig.
    exint               transformCount() const
                            { return getRig() ? getRig()->transformCount() : 0; }
    exint               intrinsicTransformCount(const GU_PrimPacked *prim) const
    { return transformCount(); }
    /// Return the name of the given transform.
    const char*         transformName(exint i) const;
    const char*         intrinsicTransformName(const GU_PrimPacked *prim, exint i) const
    { return transformName(i); }

    /// Functions for the 'agentworldtransforms' and 'agentlocaltransforms'
    /// intrinsics to manipulate the transforms as an array of floats.
    exint               transformsArraySize() const;
    exint               intrinsicTransformsArraySize(const GU_PrimPacked *prim) const
    { return transformsArraySize(); }
    void                worldTransformsArray(const GU_PrimPacked *prim, FloatType *data, exint n) const
			    { transformsArray(data, n, /* world_space */ true); }
    void                setWorldTransformsFromArray(GU_PrimPacked *prim, const FloatType *data,
                                                    exint n)
			    { setTransformsFromArray(data, n,
                                                     /* world_space */ true); }
    void                localTransformsArray(const GU_PrimPacked *prim, FloatType *data, exint n) const
			    { transformsArray(data, n,
                                                    /* world_space */ false); }
    void                setLocalTransformsFromArray(GU_PrimPacked *prim, const FloatType *data,
                                                    exint n)
			    { setTransformsFromArray(data, n,
                                                     /* world_space */ false); }

    /// Functions for the 'agentrigparents' intrinsic.
    exint               intrinsicParentIndex(const GU_PrimPacked *prim, exint i) const;
    /// Functions for the 'agentlayers' intrinsic.
    exint               intrinsicNumLayers(const GU_PrimPacked *prim) const;
    const char*         intrinsicLayerName(const GU_PrimPacked *prim, exint i) const;

    /// Used by the 'agentcliptimes', 'agentclipnames', and 'agentclipweights'
    /// intrinsics to grow or shrink the number of clips.
    void                intrinsicSetNumClips(GU_PrimPacked *prim, GA_Size n);

    /// Functions for the 'agentclipcatalog' intrinsic.
    exint               intrinsicCatalogNumClips(const GU_PrimPacked *prim) const;
    const char*         intrinsicCatalogClipName(const GU_PrimPacked *prim, exint i) const;
    const char*         intrinsicClipCatalogFilenames(const GU_PrimPacked *prim, exint i) const;

    /// Functions for the 'agentshapelibrary' intrinsic.
    exint               intrinsicNumShapes(const GU_PrimPacked *prim) const;
    void                intrinsicShapeNames(const GU_PrimPacked *prim, UT_StringArray &values) const;

    /// Functions for the 'agenttransformgroups' intrinsic.
    /// @{
    exint               intrinsicNumTransformGroups(const GU_PrimPacked *prim) const;
    const char*         intrinsicTransformGroupName(const GU_PrimPacked *prim, exint i) const;
    /// @}

    /// Functions for the clip layer intrinsics.
    /// @{
    const char*         intrinsicClipLayerMode(const GU_PrimPacked *prim, exint i) const;
    exint               intrinsicClipLayerParent(const GU_PrimPacked *prim, exint i) const;
    fpreal              intrinsicClipLayerWeight(const GU_PrimPacked *prim, exint i) const;
    exint               intrinsicNumClipLayers(const GU_PrimPacked *prim) const
    { return numClipLayers(); }
    /// @}

private:
    GU_Agent&		operator=(const GU_Agent& copy); // DISALLOWED

private:
    void                dirtyBlendedTransforms(GU_PrimPacked *prim);
    void                loadDefinition() const;
    SYS_FORCE_INLINE void loadDefinitionIfNeeded() const
    {
        if (!myDefinition)
            loadDefinition();
    }

    template <typename T>
    bool		loadFrom(GU_PrimPacked *prim, const T &options, const GA_LoadMap &map);
    template <typename T>
    void		updateFrom(GU_PrimPacked *prim, const T &options);

    /// Get the agent's current transforms, in local or world space.
    /// Disabling interpolation can improve performance if the clip time falls
    /// between two samples.
    bool                computeTransforms(Matrix4ArrayConstPtr &matrices,
                                          bool world_space,
                                          bool interpolate_samples = true,
                                          bool acquire_xform_lock = true) const;

    /// Internal version of computeWorldTransform(), which allows controlling
    /// whether myXformLock needs to be acquired.
    bool                computeWorldTransformInternal(Matrix4Type &xform,
                                                      exint i,
                                                      bool interpolate_samples,
                                                      bool acquire_xform_lock = true) const;

    /// Internal version of computeLocalTransform(), which allows controlling
    /// whether myXformLock needs to be acquired.
    bool                computeLocalTransformInternal(Matrix4Type &xform,
                                                      exint i,
                                                      bool interpolate_samples,
                                                      bool acquire_xform_lock = true) const;

    /// Override the current transforms of the agent.
    void                setTransforms(const Matrix4Array &xforms,
                                      bool world_space);
    void                transformsArray(FloatType *buf, exint n,
                                        bool world_space) const;
    void                setTransformsFromArray(const FloatType *data, exint n,
                                               bool world_space);

    /// Resize the clip arrays to n elements.
    void                changeNumClips(GU_PrimPacked *prim, exint n, bool fill_valid_clips = false);

    /// Convert the cached transforms from local to world space (or vice versa).
    void                convertCachedTransforms(bool world_space) const;

    struct DelayLoadData
    {
        DelayLoadData(const GA_SharedDataHandlePtr &definition,
                      const GA_SharedDataHandlePtr &transforms)
            : myDefinition(definition), myTransforms(transforms)
        {
        }

        DelayLoadData(const DelayLoadData &src)
            : myDefinition(src.myDefinition),
              myTransforms(src.myTransforms),
              myClipNames(src.myClipNames),
              myClipXformGroupNames(src.myClipXformGroupNames)
        {
        }

        /// Return whether the shared data has been loaded.
        bool isLoaded() const { return !myDefinition && !myTransforms; }

        int64 getMemoryUsage(bool inclusive) const;

        UT_Lock myLock;
        /// Shared agent definition.
        GA_SharedDataHandlePtr myDefinition;
        /// Shared transforms.
        GA_SharedDataHandlePtr myTransforms;
        /// Names of the clips that this agent will use after the shared
        /// definition has been loaded.
        UT_StringArray myClipNames;
        /// Names of the transform groups that this agent will use after the
        /// shared definition has been loaded.
        UT_StringArray myClipXformGroupNames;
    };

    GU_AgentDefinitionPtr myDefinition;
    StringType		myCurrentLayer;
    StringType		myCollisionLayer;
    State		myState;

    mutable Matrix4ArrayPtr myCachedTransforms;
    /// Tracks which entries of myCachedTransforms are in local or world space.
    mutable UT_BitArray     myWorldSpace;

    /// Whether or not myCachedTransforms matches the clip intrinsics.
    mutable bool myHasCustomXforms;

    using XformLock = UT_TBBSpinLock;
    using XformLockScope = UT_TBBSpinLock::Scope;
    mutable XformLock       myXformLock;

    mutable UT_UniquePtr<DelayLoadData> myDelayLoadData;

    static GA_PrimitiveTypeId theTypeId;
};

#endif
