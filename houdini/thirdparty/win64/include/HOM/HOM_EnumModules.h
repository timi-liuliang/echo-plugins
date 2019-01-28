/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	This file contains classes that, when wrapped by swig, behave like
 *	submodules.  These submodules contain enumerated values.
 */

#ifndef __HOM_EnumModules_h__
#define __HOM_EnumModules_h__

#include "HOM_EnumValue.h"

// Swig ignores #include by default so we use %include instead.
#ifdef SWIG
%include <UT/UT_Experimental.h>
%include <UT/UT_EnumMacros.h>
#else
#include <UT/UT_Experimental.h>
#include <UT/UT_EnumMacros.h>
#endif

#define HOM_NUMERIC_DATA_VALUES \
    Int8, Int16, Int32, Int64, Float16, Float32, Float64
HOM_DECLARE_ENUM_MODULE(numericData, 7, HOM_NUMERIC_DATA_VALUES)

#define HOM_ATTRIB_DATA_VALUES \
    Int, Float, String
HOM_DECLARE_ENUM_MODULE(attribData, 3, HOM_ATTRIB_DATA_VALUES)


#define HOM_ATTRIB_TYPE_VALUES \
    Point, Prim, Vertex, Global
HOM_DECLARE_ENUM_MODULE(attribType, 4, HOM_ATTRIB_TYPE_VALUES)


#define HOM_PRIM_TYPE_VALUES \
    Polygon, NURBSCurve, BezierCurve, Mesh, NURBSSurface, BezierSurface, \
    Circle, Sphere, Tube, Metaball, TriangleFan, TriangleStrip, \
    TriangleBezier, PastedSurface, Volume, ParticleSystem, Unknown, \
    Tetrahedron, PolySoup, VDB, AlembicRef, Custom, PackedPrim, Agent, \
    PackedFragment, PackedGeometry
HOM_DECLARE_ENUM_MODULE(primType, 26, HOM_PRIM_TYPE_VALUES)


#define HOM_PARM_DATA_VALUES \
    Int, Float, String, Ramp, Data
HOM_DECLARE_ENUM_MODULE(parmData, 5, HOM_PARM_DATA_VALUES)


#define HOM_PARM_TYPE_CATEGORY_VALUES \
    Int, Float, String, Toggle, Menu, Button, FolderSet, Folder, Separator, \
    Label, Ramp, Data
HOM_DECLARE_ENUM_MODULE(parmTemplateType, 12, HOM_PARM_TYPE_CATEGORY_VALUES)


#define HOM_PARM_LOOK_VALUES \
    Regular, Angle, Vector, ColorSquare, HueCircle, CRGBAPlaneChooser, \
    Logarithmic
HOM_DECLARE_ENUM_MODULE(parmLook, 7, HOM_PARM_LOOK_VALUES)


#define HOM_PARM_NAMING_SCHEME_VALUES \
    Base1, XYZW, XYWH, UVW, RGBA, MinMax, MaxMin, StartEnd, BeginEnd
HOM_DECLARE_ENUM_MODULE(parmNamingScheme, 9, HOM_PARM_NAMING_SCHEME_VALUES)


#define HOM_PARM_COND_TYPE_VALUES \
    DisableWhen, HideWhen, NoCookWhen
HOM_DECLARE_ENUM_MODULE(parmCondType, 3, HOM_PARM_COND_TYPE_VALUES)


#define HOM_PARM_EXTRAPOLATE_VALUES \
    Default, Hold, Cycle, Extend, Slope, CycleOffset, Oscillate
HOM_DECLARE_ENUM_MODULE(parmExtrapolate, 7, HOM_PARM_EXTRAPOLATE_VALUES)

#define HOM_PARM_BAKE_CHOP_VALUES \
    Off, KeepExportFlag, DisableExportFlag, CreateDeleteChop
HOM_DECLARE_ENUM_MODULE(parmBakeChop, 4, HOM_PARM_BAKE_CHOP_VALUES)

#define HOM_STRING_PARM_TYPE_VALUES \
    Regular, FileReference, NodeReference, NodeReferenceList
HOM_DECLARE_ENUM_MODULE(stringParmType, 4, HOM_STRING_PARM_TYPE_VALUES)

#define HOM_DATA_PARM_TYPE_VALUES \
    Geometry, KeyValueDictionary
HOM_DECLARE_ENUM_MODULE(dataParmType, 2, HOM_DATA_PARM_TYPE_VALUES)


#define HOM_EXPR_LANGUAGE_VALUES \
    Python, Hscript
HOM_DECLARE_ENUM_MODULE(exprLanguage, 2, HOM_EXPR_LANGUAGE_VALUES)


#define HOM_SCRIPT_LANGUAGE_VALUES \
    Python, Hscript
HOM_DECLARE_ENUM_MODULE(scriptLanguage, 2, HOM_SCRIPT_LANGUAGE_VALUES)


#define HOM_FILE_TYPE_VALUES \
    Any, Image, Geometry, Ramp, Capture, Clip, Lut, Cmd, Midi, I3d, Chan, \
    Sim, SimData, Hip, Otl, Dae, Gallery, Directory
HOM_DECLARE_ENUM_MODULE(fileType, 18, HOM_FILE_TYPE_VALUES)


#define HOM_FILE_CHOOSER_MODE_VALUES \
    Read, Write, ReadAndWrite
HOM_DECLARE_ENUM_MODULE(fileChooserMode, 3, HOM_FILE_CHOOSER_MODE_VALUES)


#define HOM_FOLDER_TYPE_VALUES \
    Simple, Collapsible, Tabs, RadioButtons, MultiparmBlock, \
    ScrollingMultiparmBlock, TabbedMultiparmBlock, ImportBlock
HOM_DECLARE_ENUM_MODULE(folderType, 8, HOM_FOLDER_TYPE_VALUES)


#define HOM_MENU_TYPE_VALUES \
    Normal, Mini, StringReplace, StringToggle, ControlNextParameter
HOM_DECLARE_ENUM_MODULE(menuType, 5, HOM_MENU_TYPE_VALUES)


#define HOM_PANE_TAB_TYPE_VALUES \
    SceneViewer, ContextViewer, ChannelViewer, CompositorViewer, \
    OutputViewer, MaterialPalette, ShaderViewer, IPRViewer, NetworkEditor, \
    Parm, DetailsView, ChannelEditor, ChannelList, Textport, PythonShell, \
    HandleList, BundleList, TakeList, TreeView, PythonPanel, HelpBrowser, \
    ParmSpreadsheet, LightLinker, AssetBrowser, PerformanceMonitor, DataTree, \
    SceneGraphTree, TaskGraphTable
HOM_DECLARE_ENUM_MODULE(paneTabType, 27, HOM_PANE_TAB_TYPE_VALUES)


#define HOM_STATE_VIEWER_TYPE_VALUES \
    Scene, Compositor, SceneGraph
HOM_DECLARE_ENUM_MODULE(stateViewerType, 3, HOM_STATE_VIEWER_TYPE_VALUES) 


#define HOM_PANE_LINK_TYPE_VALUES \
    Pinned, Group1, Group2, Group3, Group4, Group5, Group6, Group7, Group8, \
    Group9, FollowSelection
HOM_DECLARE_ENUM_MODULE(paneLinkType, 11, HOM_PANE_LINK_TYPE_VALUES)


#define HOM_NETWORK_ITEM_TYPE_VALUES \
    Connection, NetworkBox, Node, StickyNote, SubnetIndirectInput, NetworkDot
HOM_DECLARE_ENUM_MODULE(networkItemType, 6, HOM_NETWORK_ITEM_TYPE_VALUES)


#define HOM_COLOR_ITEM_TYPE_VALUES \
    NetworkBox, StickyNote, StickyNoteText
HOM_DECLARE_ENUM_MODULE(colorItemType, 3, HOM_COLOR_ITEM_TYPE_VALUES)


#define HOM_GEOMETRY_VIEWPORT_TYPE_VALUES \
    Perspective, Top, Bottom, Front, Back, Right, Left, UV
HOM_DECLARE_ENUM_MODULE(geometryViewportType, 8, \
    HOM_GEOMETRY_VIEWPORT_TYPE_VALUES)


#define HOM_GL_SHADING_TYPE_VALUES \
    WireBoundingBox, ShadedBoundingBox, Wire, WireGhost, HiddenLineInvisible, \
    HiddenLineGhost, Flat, FlatWire, Smooth, SmoothWire
HOM_DECLARE_ENUM_MODULE(glShadingType, 10, HOM_GL_SHADING_TYPE_VALUES)


#define HOM_VIEWPORT_STEREO_MODES \
 Anaglyph, HorizontalInterlace, HorizontalInterlaceReverse, QuadBufferGL
HOM_DECLARE_ENUM_MODULE(viewportStereoMode, 4, HOM_VIEWPORT_STEREO_MODES)


#define HOM_VIEWPORT_HOME_CLIP_MODES \
 Neither, FarOnly, NearOnly, NearAndFar
HOM_DECLARE_ENUM_MODULE(viewportHomeClipMode, 4, HOM_VIEWPORT_HOME_CLIP_MODES)


#define HOM_GEOMETRY_VIEWPORT_LAYOUT_VALUES \
    Single, Quad, DoubleStack, DoubleSide, TripleBottomSplit, \
    TripleLeftSplit, QuadBottomSplit, QuadLeftSplit
HOM_DECLARE_ENUM_MODULE(geometryViewportLayout, 8, \
    HOM_GEOMETRY_VIEWPORT_LAYOUT_VALUES)


#define HOM_SNAPPING_MODE_VALUES \
    Off, Grid, Prim, Point, Multi
HOM_DECLARE_ENUM_MODULE(snappingMode, 5, HOM_SNAPPING_MODE_VALUES)

#define HOM_SELECTION_MODE_VALUES \
    Object, Geometry, Dynamics
HOM_DECLARE_ENUM_MODULE(selectionMode, 3, HOM_SELECTION_MODE_VALUES)

#define HOM_PICK_STYLE_VALUES \
    Box, Lasso, Brush, Laser
HOM_DECLARE_ENUM_MODULE(pickStyle, 4, HOM_PICK_STYLE_VALUES)


#define HOM_PICK_MODIFIER_VALUES \
    Add, Toggle, Remove, Replace, Intersect
HOM_DECLARE_ENUM_MODULE(pickModifier, 5, HOM_PICK_MODIFIER_VALUES)


#define HOM_PICK_FACING_VALUES \
    Front, Back, FrontAndBack
HOM_DECLARE_ENUM_MODULE(pickFacing, 3, HOM_PICK_FACING_VALUES)

#define HOM_DISPLAY_SET_VALUES \
    SceneObject, SelectedObject, GhostObject, DisplayModel, CurrentModel, \
    TemplateModel
HOM_DECLARE_ENUM_MODULE(displaySetType, 6, HOM_DISPLAY_SET_VALUES)


#define HOM_MARKER_VISIBILITY_VALUES \
    Always, Selected, AroundPointer, UnderPointer
HOM_DECLARE_ENUM_MODULE(markerVisibility, 4, HOM_MARKER_VISIBILITY_VALUES)

#define HOM_VIEWPORT_GUIDE_VALUES \
    FloatingGnomon, OriginGnomon, ParticleGnomon, ViewPivot, GroupList, \
    SafeArea, FieldGuide, CameraMask, XZPlane, XYPlane, YZPlane, NodeGuides, \
    NodeHandles, FollowSelection, IKCriticalZone, ObjectNames, ObjectPaths, \
    DisplayNodes, CurrentGeometry, TemplateGeometry, SelectableTemplates, \
    ShowDrawTime, ObjectSelection, FillSelections
HOM_DECLARE_ENUM_MODULE(viewportGuide, 24, HOM_VIEWPORT_GUIDE_VALUES)

#define HOM_VIEWPORT_GEOMETRY_INFO \
    Off, SelectedOnly, AlwaysOn
HOM_DECLARE_ENUM_MODULE(viewportGeometryInfo, 3, HOM_VIEWPORT_GEOMETRY_INFO)

#define HOM_VIEWPORT_HANDLE_HIGHLIGHT \
    Off, Small, Normal
HOM_DECLARE_ENUM_MODULE(viewportHandleHighlight,3,HOM_VIEWPORT_HANDLE_HIGHLIGHT)

#define HOM_VIEWPORT_CLOSURE_SELECTION \
    Hide, HullPrimitives, Show
HOM_DECLARE_ENUM_MODULE(viewportClosureSelection, 3,
			HOM_VIEWPORT_CLOSURE_SELECTION)

#define HOM_VIEWPORT_GUIDE_FONT \
    Tiny, Small, Medium, Large
HOM_DECLARE_ENUM_MODULE(viewportGuideFont, 4, HOM_VIEWPORT_GUIDE_FONT)

#define HOM_VIEWPORT_VOLUME_QUALITY \
    VeryLow, Low, Normal, High
HOM_DECLARE_ENUM_MODULE(viewportVolumeQuality, 4, HOM_VIEWPORT_VOLUME_QUALITY)

#define HOM_VIEWPORT_BG_VIEW \
 Perspective, Camera, Top, Front, Right, Bottom, Back, Left, UV
HOM_DECLARE_ENUM_MODULE(viewportBGImageView, 8, HOM_VIEWPORT_BG_VIEW)

#define HOM_VIEWPORT_COLOR_SCHEME \
 Light, Dark, Grey
HOM_DECLARE_ENUM_MODULE(viewportColorScheme, 3, HOM_VIEWPORT_COLOR_SCHEME)

#define HOM_VIEWPORT_PARTICLE_DISPLAY \
    Points, Pixels, Lines, Discs
HOM_DECLARE_ENUM_MODULE(viewportParticleDisplay,
			4, HOM_VIEWPORT_PARTICLE_DISPLAY)

#define HOM_VIEWPORT_STANDIN_GEO \
 DisplayOff, LocationMarker, BoundingBox
HOM_DECLARE_ENUM_MODULE(viewportStandInGeometry, 3, HOM_VIEWPORT_STANDIN_GEO)

#define HOM_VIEWPORT_LIGHTING \
 Off, Headlight, Normal, HighQuality, HighQualityWithShadows
HOM_DECLARE_ENUM_MODULE(viewportLighting, 5, HOM_VIEWPORT_LIGHTING)

#define HOM_VIEWPORT_TRANSPARENCY \
 Cutout, Low, Medium, High
HOM_DECLARE_ENUM_MODULE(viewportTransparency, 4, HOM_VIEWPORT_TRANSPARENCY)

#define HOM_VIEWPORT_SHADOW_QUALITY \
 Point, PointAA, Area, AreaAA
HOM_DECLARE_ENUM_MODULE(viewportShadowQuality, 4, HOM_VIEWPORT_SHADOW_QUALITY)

#define HOM_VIEWPORT_MATERIAL_UPDATE \
 Always, OffForPlayback, Manual
HOM_DECLARE_ENUM_MODULE(viewportMaterialUpdate, 3, HOM_VIEWPORT_MATERIAL_UPDATE)

#define HOM_VIEWPORT_AGENT_BONE_DEFORM \
 Disabled, ReducedLOD, Always
HOM_DECLARE_ENUM_MODULE(viewportAgentBoneDeform, 3,
			HOM_VIEWPORT_AGENT_BONE_DEFORM)

#define HOM_VIEWPORT_AGENT_WIREFRAME \
 Line, Bone
HOM_DECLARE_ENUM_MODULE(viewportAgentWireframe, 2,HOM_VIEWPORT_AGENT_WIREFRAME)

#define HOM_VIEWPORT_PACKED_BOX_MODE \
 NoDisplay, Wireframe, Shaded, CurrentShadingMode
HOM_DECLARE_ENUM_MODULE(viewportPackedBoxMode, 4,HOM_VIEWPORT_PACKED_BOX_MODE)

#define HOM_VIEWPORT_GRID_RULER \
 Hide, MainAxis, GridPoints
HOM_DECLARE_ENUM_MODULE(viewportGridRuler, 3, HOM_VIEWPORT_GRID_RULER)

#define HOM_VIEWPORT_TEXTURE_DEPTH \
 FullHDR, HDR16, Fixed8, Compressed8
HOM_DECLARE_ENUM_MODULE(viewportTextureDepth, 4, HOM_VIEWPORT_TEXTURE_DEPTH)

#define HOM_BOUNDARY_DISPLAY_VALUES \
 Off, View3D, ViewUV, On
HOM_DECLARE_ENUM_MODULE(boundaryDisplay, 4, HOM_BOUNDARY_DISPLAY_VALUES)

#define HOM_FLIPBOOK_OBJECTS_VALUES \
Visible, GeoOnly, GeoExcluded, AllObjects
HOM_DECLARE_ENUM_MODULE(flipbookObjectType, 4, HOM_FLIPBOOK_OBJECTS_VALUES)

#define HOM_GROUP_LIST_TYPE_VALUES \
    Points, Vertices, Edges, Breakpoints, Primitives, MatchPickType
HOM_DECLARE_ENUM_MODULE(groupListType, 6, HOM_GROUP_LIST_TYPE_VALUES)


#define HOM_GEOMETRY_TYPE_VALUES \
    Points, Vertices, Edges, Breakpoints, Primitives
HOM_DECLARE_ENUM_MODULE(geometryType, 5, HOM_GEOMETRY_TYPE_VALUES)


#define HOM_CONNECTIVITY_TYPE_VALUES \
    NoConnectivity, Texture, Position
HOM_DECLARE_ENUM_MODULE(connectivityType, 3, HOM_CONNECTIVITY_TYPE_VALUES)


#define HOM_SEVERITY_TYPE_VALUES \
    Message, ImportantMessage, Warning, Error, Fatal
HOM_DECLARE_ENUM_MODULE(severityType, 5, HOM_SEVERITY_TYPE_VALUES)


#define HOM_CONFIRM_TYPE_VALUES \
    OverwriteFile, UnlockNode, DeleteSpareParameters, DeleteWithoutReferences, \
    NestedChannelGroups, SiblingChannelGroups, DeleteShelfElement, \
    DeleteGalleryEntry, InactiveSnapMode, BackgroundSave, LockMultiNode
HOM_DECLARE_ENUM_MODULE(confirmType, 11, HOM_CONFIRM_TYPE_VALUES)


#define HOM_POSITION_TYPE_VALUES \
    WorldSpace, ViewportXY, ViewportUV
HOM_DECLARE_ENUM_MODULE(positionType, 3, HOM_POSITION_TYPE_VALUES)


#define HOM_COMPONENT_LOOP_TYPE_VALUES \
    Partial, Extended, Closed
HOM_DECLARE_ENUM_MODULE(componentLoopType, 3, HOM_COMPONENT_LOOP_TYPE_VALUES)


#define HOM_FLIPBOOK_ANTIALIAS_VALUES	\
 UseViewportSetting, Off, Fast, Good, HighQuality
HOM_DECLARE_ENUM_MODULE(flipbookAntialias, 5, HOM_FLIPBOOK_ANTIALIAS_VALUES)


#define HOM_FLIPBOOK_MOTION_BLUR_BIAS_VALUES \
 Previous, Centered, Forward
HOM_DECLARE_ENUM_MODULE(flipbookMotionBlurBias, 3,
			HOM_FLIPBOOK_MOTION_BLUR_BIAS_VALUES)


// Boost has a limit of 25 on the size of a tuple, so we need to use a sequence
// for this enumeration.  Swig can't handle sequences, though, so we need
// to use the tuple version for it.
#ifndef SWIG
#define HOM_SHADER_TYPE_SEQ \
    (Surface)(SurfaceShadow)(Displacement)(Geometry)(Interior)(Light)\
    (LightShadow)(Atmosphere)(Lens)(Output)(Background)(Photon)\
    (Image3D)(CVex)(CoShader)(Mutable)(Properties)(Material)(VopMaterial)\
    (ShaderClass)(StructDef)(CoShaderArray)(Integrator)(Generic)(BSDF)\
    (LightFilter)(Invalid)
HOM_DECLARE_ENUM_MODULE_FROM_SEQ(shaderType, HOM_SHADER_TYPE_SEQ)
#else
#define HOM_SHADER_TYPE_VALUES \
    Surface, SurfaceShadow, Displacement, Geometry, Interior, Light, \
    LightShadow, Atmosphere, Lens, Output, Background, Photon, \
    Image3D, CVex, CoShader, Mutable, Properties, Material, VopMaterial, \
    ShaderClass, StructDef, CoShaderArray, Integrator, Generic, BSDF, \
    LightFilter, Invalid
HOM_DECLARE_ENUM_MODULE(shaderType, 27, HOM_SHADER_TYPE_VALUES)
#endif


#define HOM_RAMP_BASIS_VALUES \
    Constant, Linear, CatmullRom, MonotoneCubic, Bezier, BSpline, Hermite
HOM_DECLARE_ENUM_MODULE(rampBasis, 7, HOM_RAMP_BASIS_VALUES)


#define HOM_RAMP_PARM_TYPE_VALUES \
    Color, Float
HOM_DECLARE_ENUM_MODULE(rampParmType, 2, HOM_RAMP_PARM_TYPE_VALUES)


#define HOM_COLOR_TYPE_VALUES \
    HSL, HSV, RGB, XYZ, LAB, TMI
HOM_DECLARE_ENUM_MODULE(colorType, 6, HOM_COLOR_TYPE_VALUES)


#define HOM_NODE_TYPE_SOURCE_VALUES \
    Internal, Subnet, CompiledCode, VexCode, RslCode
HOM_DECLARE_ENUM_MODULE(nodeTypeSource, 5, HOM_NODE_TYPE_SOURCE_VALUES)


#define HOM_FIELD_TYPE_VALUES \
    NoSuchField, Integer, Boolean, Float, String, \
    Vector2, Vector3, Vector4, Quaternion, \
    Matrix3, Matrix4, UV, UVW, IntArray, FloatArray, Matrix2, StringArray
HOM_DECLARE_ENUM_MODULE(fieldType, 17, HOM_FIELD_TYPE_VALUES)


#define HOM_RENDER_METHOD_VALUES \
    RopByRop, FrameByFrame
HOM_DECLARE_ENUM_MODULE(renderMethod, 2, HOM_RENDER_METHOD_VALUES)


#define HOM_UPDATE_MODE_VALUES \
    AutoUpdate, OnMouseUp, Manual
HOM_DECLARE_ENUM_MODULE(updateMode, 3, HOM_UPDATE_MODE_VALUES)


#define HOM_LICENSE_CATEGORY_TYPE_VALUES \
    Commercial, Indie, Education, ApprenticeHD, Apprentice
HOM_DECLARE_ENUM_MODULE(licenseCategoryType, 5, \
	HOM_LICENSE_CATEGORY_TYPE_VALUES)


#define HOM_HDA_LICENSE_TYPE_VALUES \
    Execute, Read, Full
HOM_DECLARE_ENUM_MODULE(hdaLicenseType, 3, HOM_HDA_LICENSE_TYPE_VALUES)


#define HOM_ORIENTATION_UPAXIS_VALUES \
    Y, Z
HOM_DECLARE_ENUM_MODULE(orientUpAxis, 2, HOM_ORIENTATION_UPAXIS_VALUES)


#define HOM_HANDLE_ORIENT_TO_NORMAL_AXIS_VALUES \
    Y, Z
HOM_DECLARE_ENUM_MODULE(handleOrientToNormalAxis, 2,
			HOM_HANDLE_ORIENT_TO_NORMAL_AXIS_VALUES)


#define HOM_NODE_EVENT_TYPE_VALUES \
    BeingDeleted, NameChanged, InputRewired, FlagChanged, ParmTupleChanged, \
    AppearanceChanged, PositionChanged, ChildCreated, ChildDeleted, \
    ChildSwitched, ChildSelectionChanged, InputDataChanged, \
    SpareParmTemplatesChanged, SelectionChanged
HOM_DECLARE_ENUM_MODULE(nodeEventType, 14, HOM_NODE_EVENT_TYPE_VALUES)

#define HOM_HDA_EVENT_TYPE_VALUES \
    AssetCreated, AssetDeleted, AssetSaved, LibraryInstalled, LibraryUninstalled
HOM_DECLARE_ENUM_MODULE(hdaEventType, 5, HOM_HDA_EVENT_TYPE_VALUES)

#define HOM_HIPFILE_EVENT_TYPE_VALUES \
    BeforeClear, AfterClear, BeforeLoad, AfterLoad, BeforeMerge, \
    AfterMerge, BeforeSave, AfterSave
HOM_DECLARE_ENUM_MODULE(hipFileEventType, 8, HOM_HIPFILE_EVENT_TYPE_VALUES);

#define HOM_APPEARANCE_CHANGE_TYPE_VALUES \
    Any, ErrorState, Pick, Color, DeleteScript, Comment, LockFlag, \
    CompressFlag, OTLMatchState, ActiveInput, Connections, ExpressionLanguage, \
    NetworkBox, PostIt, Dot
HOM_DECLARE_ENUM_MODULE(appearanceChangeType, 15, \
	HOM_APPEARANCE_CHANGE_TYPE_VALUES)


#define HOM_IMAGE_DEPTH_VALUES \
    Int8, Int16, Int32, Float16, Float32
HOM_DECLARE_ENUM_MODULE(imageDepth, 5, HOM_IMAGE_DEPTH_VALUES)

#define HOM_PLAY_MODE_VALUES \
    Loop, Once, Zigzag
HOM_DECLARE_ENUM_MODULE(playMode, 3, HOM_PLAY_MODE_VALUES);

#define HOM_PLAYBAR_EVENT_VALUES \
    Started, Stopped, FrameChanged
HOM_DECLARE_ENUM_MODULE(playbarEvent, 3, HOM_PLAYBAR_EVENT_VALUES);

#define HOM_PERFMON_TIME_FORMAT_VALUES \
    Absolute, Percent
HOM_DECLARE_ENUM_MODULE(perfMonTimeFormat, 2, HOM_PERFMON_TIME_FORMAT_VALUES);

#define HOM_PERFMON_OBJECT_VIEW_VALUES \
    List, Tree, EventLog
HOM_DECLARE_ENUM_MODULE(perfMonObjectView, 3, HOM_PERFMON_OBJECT_VIEW_VALUES);

// Compression types
#define HOM_COMPRESSION_TYPE_VALUES \
    NoCompression, Gzip, Blosc
HOM_DECLARE_ENUM_MODULE(compressionType, 3, HOM_COMPRESSION_TYPE_VALUES);


// Boost has a limit of 25 on the size of a tuple, so we need to use a sequence
// for this enumeration.  Swig can't handle sequences, though, so we need
// to use the tuple version for it.
#ifndef SWIG
#define HOM_NODE_TYPE_FILTER_SEQ \
    (NoFilter)(Sop)(Pop)(Popnet)(Chop)(Chopnet)(Cop)(Copnet)(Vop)(Vopnet) \
    (Rop)(Obj)(ObjGeometry)(ObjFog)(ObjGeometryOrFog)(ObjLight)(ObjCamera) \
    (ObjBone)(ObjMuscle)(ObjSubnet)(Shop)(ShopMaterial)(ShopProperties) \
    (ShopSurface)(ShopDisplacement)(ShopInterior)(ShopLight)(ShopLightShadow) \
    (ShopAtmosphere)(ShopPhoton)(ShopImage3D)(ShopCVEX)(Dop)(Lop)
HOM_DECLARE_ENUM_MODULE_FROM_SEQ(nodeTypeFilter, HOM_NODE_TYPE_FILTER_SEQ)
#else
#define HOM_NODE_TYPE_FILTER_VALUES \
    NoFilter, Sop, Pop, Popnet, Chop, Chopnet, Cop, Copnet, Vop, Vopnet, \
    Rop, Obj, ObjGeometry, ObjFog, ObjGeometryOrFog, ObjLight, ObjCamera, \
    ObjBone, ObjMuscle, ObjSubnet, Shop, ShopMaterial, ShopProperties, \
    ShopSurface, ShopDisplacement, ShopInterior, ShopLight, ShopLightShadow, \
    ShopAtmosphere, ShopPhoton, ShopImage3D, ShopCVEX, Dop, Lop
HOM_DECLARE_ENUM_MODULE(nodeTypeFilter, 34, HOM_NODE_TYPE_FILTER_VALUES)
#endif


#define HOM_VDB_DATA_VALUES \
    Boolean, Float, Int, Vector3
HOM_DECLARE_ENUM_MODULE(vdbData, 4, HOM_VDB_DATA_VALUES)


#define HOM_CHANNEL_EDITOR_MODE_VALUES \
    Graph, Table, Dopesheet
HOM_DECLARE_ENUM_MODULE(channelEditorMode, 3, HOM_CHANNEL_EDITOR_MODE_VALUES)

#define HOM_VOP_PARM_GEN_VALUES \
    Constant, Parameter, SubnetInput
HOM_DECLARE_ENUM_MODULE(vopParmGenType, 3, HOM_VOP_PARM_GEN_VALUES)

#define HOM_STATE_GENERATE_MODE_VALUES \
    Insert, Branch
HOM_DECLARE_ENUM_MODULE(stateGenerateMode, 2, HOM_STATE_GENERATE_MODE_VALUES)

#define HOM_STATE_USD_PRIM_MASK_VALUES \
    All, Geometry, Light, Camera, LightAndCamera, ViewerSetting
HOM_DECLARE_ENUM_MODULE(scenePrimMask, 5, HOM_STATE_USD_PRIM_MASK_VALUES)

#define HOM_VIEWPORT_VISUALIZER_CATEGORY_VALUES \
    Node, Scene, Common 
HOM_DECLARE_ENUM_MODULE(
    viewportVisualizerCategory, 3, HOM_VIEWPORT_VISUALIZER_CATEGORY_VALUES)

#define HOM_VIEWPORT_VISUALIZER_SCOPE_VALUES \
    NodeOnly, SameNetworkTypeDescendents, AllDescendents, Global 
HOM_DECLARE_ENUM_MODULE(
    viewportVisualizerScope, 4, HOM_VIEWPORT_VISUALIZER_SCOPE_VALUES)

#define HOM_NODE_FLAG_VALUES \
    Audio, Bypass, ColorDefault, Compress, Current, Debug, Display, \
    DisplayDescriptiveName, DisplayComment, Export, Expose, Footprint, \
    Highlight, InOutDetailLow, InOutDetailMedium, InOutDetailHigh, \
    Material, Lock, Origin, Pick, Render, Selectable, SoftLock, \
    Template, Unload, Visible, XRay
HOM_DECLARE_ENUM_MODULE(
    nodeFlag, 27, HOM_NODE_FLAG_VALUES)

#define HOM_FBX_COMPATIBILITY_MODE_VALUES \
    FBXStandard, Maya
HOM_DECLARE_ENUM_MODULE(
    fbxCompatibilityMode, 2, HOM_FBX_COMPATIBILITY_MODE_VALUES)

#define HOM_FBX_MATERIAL_MODE_VALUES \
    FBXShaderNodes, VopNetworks
HOM_DECLARE_ENUM_MODULE(
    fbxMaterialMode, 2, HOM_FBX_MATERIAL_MODE_VALUES)

#define HOM_SAVE_MODE_VALUES \
    Binary, Text
    HOM_DECLARE_ENUM_MODULE(saveMode, 2, HOM_SAVE_MODE_VALUES)

#define HOM_RADIALITEM_TYPE_VALUES \
    Script, Submenu, Menu
    HOM_DECLARE_ENUM_MODULE(radialItemType, 3, HOM_RADIALITEM_TYPE_VALUES)

#define HOM_RADIALITEM_LOCATION_VALUES \
    TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight
    HOM_DECLARE_ENUM_MODULE(radialItemLocation, 8, HOM_RADIALITEM_LOCATION_VALUES)

#define HOM_GEOMETRY_VIEWPORT_EVENT_VALUES CameraSwitched
HOM_DECLARE_ENUM_MODULE(geometryViewportEvent, 1, HOM_GEOMETRY_VIEWPORT_EVENT_VALUES)

#define HOM_VALUE_LADDER_TYPE_VALUES \
    Generic, Angle
HOM_DECLARE_ENUM_MODULE(valueLadderType, 2, HOM_VALUE_LADDER_TYPE_VALUES)

#define HOM_VALUE_LADDER_DATA_TYPE_VALUES \
    Float, Int, FloatArray, IntArray
HOM_DECLARE_ENUM_MODULE(
    valueLadderDataType, 4, HOM_VALUE_LADDER_DATA_TYPE_VALUES)

#define HOM_PROMPT_MESSAGE_TYPE_VALUES \
    Error, Message, Prompt, Warning
HOM_DECLARE_ENUM_MODULE(
    promptMessageType, 4, HOM_PROMPT_MESSAGE_TYPE_VALUES)

#define HOM_DRAWABLE_DISPLAY_MODE_VALUES \
    CurrentViewportMode, WireframeMode
HOM_DECLARE_ENUM_MODULE(
    drawableDisplayMode, 2, HOM_DRAWABLE_DISPLAY_MODE_VALUES)

HOM_DECLARE_ENUM_MODULE_START(		uiEventValueType)
UT_UIEVENT_VALUE_CODE_GENERATOR(HOM_DECLARE_ENUM_VALUE2)
HOM_DECLARE_ENUM_MODULE_END

HOM_DECLARE_ENUM_MODULE_START(		uiEventReason)
UT_UIEVENT_REASON_CODE_GENERATOR(HOM_DECLARE_ENUM_VALUE2)
HOM_DECLARE_ENUM_MODULE_END

HOM_DECLARE_ENUM_MODULE_START(		drawablePrimitive)
UT_DRAWABLE_PRIMITIVE_CODE_GENERATOR(HOM_DECLARE_ENUM_VALUE2)
HOM_DECLARE_ENUM_MODULE_END

#endif // __HOM_EnumModules_h__
