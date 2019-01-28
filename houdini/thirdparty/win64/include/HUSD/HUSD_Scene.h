/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	HUSD_Scene.h (HUSD Library, C++)
 *
 * COMMENTS:	Scene info for the native Houdini viewport renderer
 */
#ifndef HUSD_Scene_h
#define HUSD_Scene_h

#include <pxr/pxr.h>

#include "HUSD_API.h"
#include <UT/UT_Map.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_Pair.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_StringSet.h>
#include <SYS/SYS_Types.h>
#include "HUSD_PrimHandle.h"

PXR_NAMESPACE_OPEN_SCOPE
class XUSD_SceneGraphDelegate;
class HdRenderIndex;
class HdRenderParam;
PXR_NAMESPACE_CLOSE_SCOPE

class HUSD_HydraCamera;
class HUSD_HydraGeoPrim;
class HUSD_HydraLight;
class HUSD_HydraPrim;
class HUSD_HydraMaterial;

/// Scene information for the native viewport renderer
class HUSD_API HUSD_Scene : public UT_NonCopyable
{
public:
	     HUSD_Scene();
    virtual ~HUSD_Scene();

    UT_StringMap<HUSD_HydraGeoPrim *> &geometry() { return myGeometry; }
    UT_StringMap<HUSD_HydraCamera  *> &cameras()  { return myCameras; }
    UT_StringMap<HUSD_HydraLight   *> &lights()   { return myLights; }
    UT_StringMap<HUSD_HydraMaterial*> &materials(){ return myMaterials; }

    const UT_StringRef &lookupPath(int id) const;

    static PXR_NS::XUSD_SceneGraphDelegate *newDelegate();
    static void freeDelegate(PXR_NS::XUSD_SceneGraphDelegate *del);

    static void pushScene(HUSD_Scene *scene); 
    static void popScene(HUSD_Scene *scene);

    virtual void addGeometry(HUSD_HydraGeoPrim *geo);
    virtual void removeGeometry(HUSD_HydraGeoPrim *geo);

    virtual void addCamera(HUSD_HydraCamera *cam);
    virtual void removeCamera(HUSD_HydraCamera *cam);

    virtual void addLight(HUSD_HydraLight *light);
    virtual void removeLight(HUSD_HydraLight *light);

    virtual void addMaterial(HUSD_HydraMaterial *mat);
    virtual void removeMaterial(HUSD_HydraMaterial *mat);

    virtual void updateScene() {}

    void	 deferUpdates(bool defer) { myDeferUpdate = defer; }
    bool	 isDeferredUpdate() const { return myDeferUpdate; }
    
    // Volumes
    const UT_StringSet &volumesUsingField(const UT_StringRef &field) const;
    void addVolumeUsingField(const UT_StringHolder &volume,
			     const UT_StringHolder &field);
    void removeVolumeUsingFields(const UT_StringRef &volume);

    // Selections. A highlight is a temporary selection which can be turned into
    // a selection in various ways.
    void	addToHighlight(int id);
    void	addPathToHighlight(const UT_StringHolder &path);
    void	clearHighlight();

    void	setHighlightAsSelection();
    void	addHighlightToSelection();
    void	removeHighlightFromSelection();
    void	toggleHighlightInSelection();
    void	intersectHighlightWithSelection();
    void	clearSelection();
    void	setSelection(const UT_StringArray &paths);
    const UT_StringArray &getSelectionList();

    bool	hasSelection() const;
    bool	hasHighlight() const;
    bool	isSelected(int id) const;
    bool	isSelected(const HUSD_HydraPrim *prim) const;
    bool	isHighlighted(const HUSD_HydraPrim *prim) const;

    int64	highlightID() const { return myHighlightID; }
    int64	selectionID() const { return mySelectionID; }

    int64	getMaterialID(const UT_StringRef &path);

    static int  getMaxGeoIndex();
    
    // bumped when a geo prim is added or removed.
    int64	getGeoSerial() { return myGeoSerial; }

    enum PrimType
    {
	INVALID_TYPE = 0,
	
	GEOMETRY,
	LIGHT,
	CAMERA,
	MATERIAL,
	PATH,
	INSTANCE
    };
    PrimType	getPrimType(int id) const;

    int		getOrCreateID(const UT_StringRef &path);
    
    void	setStage(const HUSD_DataHandle &data,
			 const HUSD_ConstOverridesPtr &overrides);

    void	setRenderOptions(const UT_Options &render_options)
		{ myRenderOptions = render_options; }
    const UT_Options &getRenderOptions() const
		{ return myRenderOptions; }

    PXR_NS::HdRenderIndex *renderIndex() { return myRenderIndex; }
    void setRenderIndex(PXR_NS::HdRenderIndex *ri) { myRenderIndex = ri; }
    
    PXR_NS::HdRenderParam *renderParam() { return myRenderParam; }
    void setRenderParam(PXR_NS::HdRenderParam *rp) { myRenderParam = rp; }
    
    // Debugging only... Do not use in production code.
    HUSD_PrimHandle getPrim(const UT_StringHolder &path) const;

protected:
    void	selectionModified(int id) const;
    UT_Map<int, UT_Pair<UT_StringHolder, PrimType> >	myNameIDLookup;
    UT_StringMap<int>			myPathIDs;
    UT_StringMap<UT_StringSet>		myFieldsInVolumes;
    UT_StringMap<HUSD_HydraGeoPrim *>	myGeometry;
    UT_StringMap<HUSD_HydraCamera *>	myCameras;
    UT_StringMap<HUSD_HydraLight *>	myLights;
    UT_StringMap<HUSD_HydraMaterial *>	myMaterials;

    UT_Options				myRenderOptions;
    UT_Map<int,int>			myHighlight;
    UT_Map<int,int>			mySelection;
    UT_StringMap<int64>			myMatIDs;
    UT_StringArray			mySelectionArray;
    int64				mySelectionArrayID;
    int64				myHighlightID;
    int64				mySelectionID;
    int64				myGeoSerial;
    bool				myDeferUpdate;

    PXR_NS::HdRenderIndex	       *myRenderIndex; // TMP, hopefuly
    PXR_NS::HdRenderParam	       *myRenderParam; // TMP, hopefuly
    HUSD_DataHandle			myStage;	// TMP
    HUSD_ConstOverridesPtr		myStageOverrides;  // TMP
};

#endif
