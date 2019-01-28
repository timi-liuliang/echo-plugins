/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __XUSD_Utils_h__
#define __XUSD_Utils_h__

#include "HUSD_API.h"
#include "HUSD_TimeCode.h"
#include "HUSD_LayerOffset.h"
#include "HUSD_Utils.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_Map.h>
#include <pxr/usd/sdf/path.h>
#include <pxr/usd/sdf/layer.h>
#include <pxr/usd/sdf/layerOffset.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/timeCode.h>

PXR_NAMESPACE_OPEN_SCOPE

typedef UT_Map<std::string, SdfLayerRefPtr> XUSD_LayerMap;
typedef UT_Map<std::string, std::string> XUSD_LayerPathMap;

HUSD_API const TfToken &HUSDgetDataIdToken();
HUSD_API const TfToken &HUSDgetSavePathToken();
HUSD_API const TfToken &HUSDgetSaveControlToken();
HUSD_API const TfToken &HUSDgetCreatorNodeToken();
HUSD_API const TfToken &HUSDgetEditorNodesToken();
HUSD_API const TfToken &HUSDgetSourceNodeToken();

HUSD_API const SdfPath
HUSDgetSdfPath(const UT_StringRef &path);
HUSD_API const UsdTimeCode
HUSDgetUsdTimeCode(const HUSD_TimeCode &timecode);
HUSD_API const UsdTimeCode
HUSDgetCurrentUsdTimeCode();
HUSD_API const UsdTimeCode
HUSDgetNonDefaultUsdTimeCode(const HUSD_TimeCode &timecode);

HUSD_API const SdfLayerOffset
HUSDgetSdfLayerOffset(const HUSD_LayerOffset &layeroffset);
HUSD_API const HUSD_LayerOffset
HUSDgetLayerOffset(const SdfLayerOffset &layeroffset);

HUSD_API const Usd_PrimFlagsPredicate
HUSDgetUsdPrimPredicate(HUSD_PrimTraversalDemands demands);

HUSD_API UsdListPosition
HUSDgetUsdListPosition(const UT_StringRef &editopstr);

// Determine if a layer comes from a SOP or not.
HUSD_API bool
HUSDisSopLayer(const std::string &identifier);
HUSD_API bool
HUSDisSopLayer(const SdfLayerHandle &layer);
HUSD_API bool
// Determine if the specified layer should be saved to disk when saving a
// LOP network which sublayers or references this layer. We only want to save
// layers that we edited.
HUSDshouldSaveLayerToDisk(const std::string &identifier);
HUSD_API bool
HUSDshouldSaveLayerToDisk(const SdfLayerHandle &layer);
// Figures out from the layer metadata where the layer should be saved. This
// method only works on layers that return true from HUSDshouldSaveLayerToDisk.
HUSD_API std::string
HUSDgetLayerSaveLocation(const SdfLayerHandle &layer);

// Get or set the save path custom data on a layer. Used by the above
// HUSDgetLayerSaveLocation as one of the methods to determine where a layer
// should be saved.
HUSD_API void
HUSDsetSavePath(const SdfLayerHandle &layer,
	const UT_StringRef &savepath);
HUSD_API bool
HUSDgetSavePath(const SdfLayerHandle &layer,
	std::string &savepath);

HUSD_API void
HUSDsetSaveControl(const SdfLayerHandle &layer,
	const UT_StringRef &savecontrol);
HUSD_API bool
HUSDgetSaveControl(const SdfLayerHandle &layer,
	std::string &savecontrol);

HUSD_API void
HUSDsetCreatorNode(const SdfLayerHandle &layer,
	const UT_StringRef &nodepath);
HUSD_API bool
HUSDgetCreatorNode(const SdfLayerHandle &layer,
	std::string &nodepath);

HUSD_API void
HUSDsetSourceNode(const UsdPrim &prim,
	const UT_StringRef &nodepath);
HUSD_API bool
HUSDgetSourceNode(const UsdPrim &prim,
	std::string &nodepath);

HUSD_API void
HUSDaddEditorNode(const SdfLayerHandle &layer,
	const UT_StringRef &nodepath);
HUSD_API bool
HUSDgetEditorNodes(const SdfLayerHandle &layer,
	std::vector<std::string> &nodepaths);

HUSD_API void
HUSDclearDataId(const UsdAttribute &attr);

HUSD_API std::string
HUSDprimTypeToAlias(const UT_StringRef &primtype);

HUSD_API TfToken
HUSDgetParentKind(const TfToken &kind);

// Create a new primitive in the specified layer. The stage parameter may or
// may not include the layer. It is used only to look up any existing prims
// so we know which ancestors of the new prim should be defined and which
// should simply be over prims. If the requested prim already exists on the
// stage, this function does nothing.
HUSD_API SdfPrimSpecHandle
HUSDcreatePrimInLayer(const UsdStageWeakPtr &stage,
	const SdfLayerHandle &layer,
	const SdfPath &path,
	const TfToken &kind,
	bool parent_prims_define,
	const std::string &parent_prims_type);

HUSD_API bool
HUSDcopySpec(const SdfLayerHandle &srclayer,
	const SdfPath &srcpath,
	const SdfLayerHandle &destlayer,
	const SdfPath &destath,
	const SdfPath &srcroot = SdfPath(),
	const SdfPath &destroot = SdfPath());

HUSD_API bool
HUSDcopyLayerMetadata(const SdfLayerHandle &srclayer,
	const SdfLayerHandle &destlayer);

// Calls the USD stitch function but with a callback that looks for SOP data
// ids on the attributes to avoid creating duplicate time samples.
HUSD_API void
HUSDstitchLayers(const SdfLayerHandle &strongLayer,
	const SdfLayerHandle &weakLayer);

// Utility function used for stitching stages together and saving them.
HUSD_API void
HUSDaddExternalReferencesToLayerMap(const SdfLayerRefPtr &layer,
	XUSD_LayerMap &layermap,
	bool recursive);
// Stitch two stages together by stitching together their "corresponding"
// layers, as determined by the requested save paths for each layer.
HUSD_API bool
HUSDaddStageTimeSample(const UsdStageWeakPtr &src,
	const UsdStageRefPtr &dest,
	SdfLayerRefPtrVector &hold_layers);

enum HUSD_FlattenLayerFlags {
    HUSD_FLATTEN_NON_RECURSIVE = 0x0000,
    HUSD_FLATTEN_RECURSIVE = 0x0001,
    HUSD_FLATTEN_FILE_LAYERS = 0x0002,
    HUSD_FLATTEN_SOP_LAYERS = 0x0004,
    HUSD_FLATTEN_IMPLICIT_LAYERS = 0x0008
};

// Combine layers together based on some options and custom data set on each
// layer in the stack (or referenced by a layer in the stack).
HUSD_API SdfLayerRefPtr
HUSDflattenLayerPartitions(const UsdStageWeakPtr &stage,
	int flatten_flags,
	SdfLayerRefPtrVector &explicit_layers);

PXR_NAMESPACE_CLOSE_SCOPE

#endif

