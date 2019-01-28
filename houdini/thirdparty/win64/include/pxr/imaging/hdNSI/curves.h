//
// Copyright 2017 Pixar
// Copyright 2018 Illumination Research Pte Ltd.
// Authors: J Cube Inc (Marco Pantaleoni, Bo Zhou, Paolo Berto Durante)
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
#ifndef HDNSI_CURVES_H
#define HDNSI_CURVES_H

#include "pxr/pxr.h"
#include "pxr/imaging/hd/basisCurves.h"
#include "pxr/imaging/hd/enums.h"
#include "pxr/imaging/hd/vertexAdjacency.h"
#include "pxr/base/gf/matrix4f.h"
#include "pxr/base/gf/rotation.h"

#include <nsi.hpp>

PXR_NAMESPACE_OPEN_SCOPE

/// \class HdNSICurves
///
/// An HdNSI representation of a basis curves object.
/// This class is an example of a hydra Rprim, or renderable object, and it
/// gets created on a call to HdRenderIndex::InsertRprim() with a type of
/// HdPrimTypeTokens->points.
///
/// The prim object's main function is to bridge the scene description and the
/// renderable representation. The Hydra image generation algorithm will call
/// HdRenderIndex::SyncAll() before any drawing; this, in turn, will call
/// Sync() for each curves with new data.
///
/// Sync() is passed a set of dirtyBits, indicating which scene buffers are
/// dirty. It uses these to pull all of the new scene data and constructs
/// updated NSI geometry objects.  Rebuilding the acceleration datastructures
/// is deferred to HdNSIRenderDelegate::CommitResources(), which runs after
/// all prims have been updated. After running Sync() for each prim and
/// HdNSIRenderDelegate::CommitResources(), the scene should be ready for
/// rendering via ray queries.
///
/// An rprim's state is lazily populated in Sync(); matching this, Finalize()
/// does the heavy work of releasing state (such as handles into the top-level
/// NSI scene), so that object population and existence aren't tied to
/// each other.
///
class HdNSICurves final : public HdBasisCurves {
public:
    HF_MALLOC_TAG_NEW("new HdNSICurves");

    /// HdNSICurves constructor.
    ///   \param id The scene-graph path to this curves.
    ///   \param instancerId If specified, the HdNSIInstancer at this id uses
    ///                      this curves as a prototype.
    HdNSICurves(SdfPath const& id,
                 SdfPath const& instancerId = SdfPath());

    /// HdNSICurves destructor.
    /// (Note: NSI resources are released in Finalize()).
    virtual ~HdNSICurves() = default;

    /// Release any resources this class is holding onto: in this case,
    /// destroy the geometry object in the NSI scene graph.
    ///   \param renderParam An HdNSIRenderParam object containing top-level
    ///                      NSI state.
    virtual void Finalize(HdRenderParam *renderParam) override;

    /// Pull invalidated scene data and prepare/update the renderable
    /// representation.
    ///
    /// This function is told which scene data to pull through the
    /// dirtyBits parameter. The first time it's called, dirtyBits comes
    /// from _GetInitialDirtyBits(), which provides initial dirty state,
    /// but after that it's driven by invalidation tracking in the scene
    /// delegate.
    ///
    /// The contract for this function is that the prim can only pull on scene
    /// delegate buffers that are marked dirty. Scene delegates can and do
    /// implement just-in-time data schemes that mean that pulling on clean
    /// data will be at best incorrect, and at worst a crash.
    ///
    /// This function is called in parallel from worker threads, so it needs
    /// to be threadsafe; calls into HdSceneDelegate are ok.
    ///
    /// Reprs are used by hydra for controlling per-item draw settings like
    /// flat/smooth shaded, wireframe, refined, etc.
    ///   \param sceneDelegate The data source for this geometry item.
    ///   \param renderParam An HdNSIRenderParam object containing top-level
    ///                      NSI state.
    ///   \param dirtyBits A specifier for which scene data has changed.
    ///   \param reprSelector A specifier for which representation to draw with.
    ///   \param forcedRepr A specifier for how to resolve reprSelector
    ///                     opinions.
    ///
    virtual void Sync(HdSceneDelegate      *sceneDelegate,
                      HdRenderParam        *renderParam,
                      HdDirtyBits          *dirtyBits,
                      HdReprSelector const &reprSelector,
                      bool                  forcedRepr) override;

    // Inform the scene graph which state needs to be downloaded in the
    // first Sync() call: in this case, topology and points data to build
    // the geometry object in the NSI scene graph.
    virtual HdDirtyBits GetInitialDirtyBitsMask() const override;

protected:
    // Update the named repr object for this Rprim. Repr objects are
    // created to support specific reprName tokens, and contain a list of
    // HdDrawItems to be passed to the renderpass (via the renderpass calling
    // HdRenderIndex::GetDrawItems()). Draw items contain prim data to be
    // rendered, but HdNSICurves bypasses them for now, so this function is
    // a no-op.
    virtual void _UpdateRepr(HdSceneDelegate *sceneDelegate,
                             HdReprSelector const &reprSelector,
                             HdDirtyBits *dirtyBits) override;

    // This callback from Rprim gives the prim an opportunity to set
    // additional dirty bits based on those already set.  This is done
    // before the dirty bits are passed to the scene delegate, so can be
    // used to communicate that extra information is needed by the prim to
    // process the changes.
    //
    // The return value is the new set of dirty bits, which replaces the bits
    // passed in.
    //
    // See HdRprim::PropagateRprimDirtyBits()
    virtual HdDirtyBits _PropagateDirtyBits(HdDirtyBits bits) const override;

    // Initialize the given representation of this Rprim.
    // This is called prior to syncing the prim, the first time the repr
    // is used.
    //
    // reprSelector is the name of the repr to initalize.  HdRprim has already
    // resolved the reprSelector to its final value.
    //
    // dirtyBits is an in/out value.  It is initialized to the dirty bits
    // from the change tracker.  InitRepr can then set additional dirty bits
    // if additional data is required from the scene delegate when this
    // repr is synced.  InitRepr occurs before dirty bit propagation.
    //
    // See HdRprim::InitRepr()
    virtual void _InitRepr(HdReprSelector const &reprSelector,
                           HdDirtyBits *dirtyBits) override;

private:
    // Populate the NSI geometry object based on scene data.
    void _PopulateRtCurves(HdSceneDelegate *sceneDelegate,
                           std::shared_ptr<NSI::Context> nsi,
                           HdDirtyBits *dirtyBits,
                           HdBasisCurvesReprDesc const &desc);

    // Populate _primvarSourceMap (our local cache of primvar data) based on
    // scene data. Primvars will be turned into samplers in _PopulateRtCurves,
    // through the help of the _CreatePrimvarSampler() method.
    void _UpdatePrimvarSources(HdSceneDelegate* sceneDelegate,
                               HdDirtyBits dirtyBits);

    // Utility function to create a NSI triangle curves and populate topology.
    void _CreateNSICurves(std::shared_ptr<NSI::Context> nsi);
    void _SetNSICurvesAttributes(std::shared_ptr<NSI::Context> nsi);

private:
    // Cached scene data. VtArrays are reference counted, so as long as we
    // only call const accessors keeping them around doesn't incur a buffer
    // copy.
    HdBasisCurvesTopology _topology;
    GfMatrix4d _transform;
    VtVec3fArray _points;
    VtIntArray _curveVertexCounts;
    VtIntArray _curveVertexIndices;
    VtFloatArray _widths;
    VtVec3fArray _colors;
    TfToken _basis;

    // NSI handles.
    std::string _masterShapeHandle;

    std::string _shaderHandle;
    std::string _attrsHandle;

    // From the unique color to the attribute and shader node handles.
    static std::multimap<size_t, std::string> _nsiCurvesShaderHandles;

    // From USD id to the NSI curves node handles.
    static std::map<SdfPath, std::string> _nsiCurvesShapeHandles;

    // From USD id to the NSI transforms node handles.
    static std::multimap<SdfPath, std::string> _nsiCurvesXformHandles;

    // Draw styles.
    bool _refined;

    // A local cache of primvar scene data. "data" is a copy-on-write handle to
    // the actual primvar buffer, and "interpolation" is the interpolation mode
    // to be used. This cache is used in _PopulateRtCurves to populate the
    // primvar sampler map in the prototype context, which is used for shading.
    struct PrimvarSource {
        VtValue data;
        HdInterpolation interpolation;
    };
    TfHashMap<TfToken, PrimvarSource, TfToken::HashFunctor> _primvarSourceMap;

    // This class does not support copying.
    HdNSICurves(const HdNSICurves&)             = delete;
    HdNSICurves &operator =(const HdNSICurves&) = delete;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDNSI_CURVES_H
