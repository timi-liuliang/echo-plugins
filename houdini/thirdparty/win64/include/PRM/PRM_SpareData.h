/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_SpareData.h ( PRM Library, C++)
 *
 * COMMENTS:	This class is used to add additional token/value pairs to a
 *		parameter.
 */

#ifndef __PRM_SpareData__
#define __PRM_SpareData__

#include "PRM_API.h"
#include "PRM_Type.h"

#include <CH/CH_ExprLanguage.h>
#include <SYS/SYS_Types.h>
#include <UT/UT_Color.h>
#include <UT/UT_Pair.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_Variadic.h>

// Namespace to use for built-in parameter tags.
// Note that we cannot add the namespace to old built-in tags
// for compatibility reasons.
#define PRM_SPARE_BUILTIN_NAMESPACE "sidefx::"

//
// Declarations of built-in parm spare data tokens (tag names).
// The entries in this list must be included in `theBuiltInTokenMap` 
// (see PRM_SpareData.C).
//
// NOTE: You *must* include the SideFX namespace in the token
//	 when adding a new spare data token.
//
#define PRM_SPARE_KEY_VALUE_DICT_KEY_LABEL_TOKEN \
    PRM_SPARE_BUILTIN_NAMESPACE "key_value_dict_key_label"
#define PRM_SPARE_KEY_VALUE_DICT_VALUE_LABEL_TOKEN \
    PRM_SPARE_BUILTIN_NAMESPACE "key_value_dict_value_label"
#define PRM_SPARE_KEY_VALUE_DICT_USE_CHOOSER_TOKEN \
    PRM_SPARE_BUILTIN_NAMESPACE "key_value_dict_use_chooser"
#define PRM_SPARE_KEY_VALUE_DICT_CHOOSER_LABEL_TOKEN \
    PRM_SPARE_BUILTIN_NAMESPACE "key_value_dict_chooser_label"
#define PRM_SPARE_KEY_VALUE_DICT_CHOOSER_CALLBACK_TOKEN \
    PRM_SPARE_BUILTIN_NAMESPACE "key_value_dict_chooser_callback"
#define PRM_SPARE_ATTRIB_ACCESS_TOKEN \
    PRM_SPARE_BUILTIN_NAMESPACE "attrib_access"
#define PRM_SPARE_CONNECTOR_KIND \
    PRM_SPARE_BUILTIN_NAMESPACE "connector_kind"
#define PRM_SPARE_USDPATHTYPE_TOKEN \
    PRM_SPARE_BUILTIN_NAMESPACE "usdpathtype"

//
// Declarations of *old*, built-in parm spare data tokens (tag names).
// WARNING: Do not add new spare data tokens to this list!!!
//	    Add the tokens to the new list above and include the SideFX
//	    namespace in the tokens.
// 
#define PRM_SPARE_AUTOSCOPE_TOKEN		    "autoscope"
#define PRM_SPARE_BUTTON_ICON_TOKEN		    "button_icon"
#define PRM_SPARE_CODE_GENERATION_TOKEN		    "code_generation"
#define PRM_SPARE_COLOR_DYN_TOKEN		    "colordyn"
#define PRM_SPARE_COLOR_TYPE_TOKEN		    "colortype"
#define PRM_SPARE_COLOR_WHEEL_TOKEN		    "colorwheel"
#define PRM_SPARE_COOK_DEPENDENT_TOKEN		    "cook_dependent"
#define PRM_SPARE_EDITOR_LANG_TOKEN		    "editorlang"
#define PRM_SPARE_EDITOR_LINES_TOKEN		    "editorlines"
#define PRM_SPARE_EDITOR_TOKEN			    "editor"
#define PRM_SPARE_FILE_CHOOSER_MODE_TOKEN	    "filechooser_mode"
#define PRM_SPARE_FILE_CHOOSER_PATTERN_TOKEN	    "filechooser_pattern"
#define PRM_SPARE_GROUP_TYPE_TOKEN		    "group_type"
#define PRM_SPARE_GROUP_DEFAULT_TOKEN		    "group_default"
#define PRM_SPARE_MULTI_START_OFFSET_TOKEN	    "multistartoffset"
#define PRM_SPARE_NETREF_TOKEN			    "netref"
#define PRM_SPARE_OPFILTER_TOKEN		    "opfilter"
#define PRM_SPARE_OPPATH_KEYWORDS_TOKEN		    "oppathkeywords"
#define PRM_SPARE_OPRELATIVE_TOKEN		    "oprelative"
#define PRM_SPARE_PARM_VOP_TOKEN		    "parmvop"
#define PRM_SPARE_RAMP_BASIS_DEFAULT_TOKEN	    "rampbasisdefault"
#define PRM_SPARE_RAMP_BASIS_TOKEN		    "rampbasis"
#define PRM_SPARE_RAMP_BASIS_VAR_TOKEN		    "rampbasis_var"
#define PRM_SPARE_RAMP_COLOR_DEFAULT_TOKEN	    "rampcolordefault"
#define PRM_SPARE_RAMP_COLOR_TYPE_TOKEN		    "rampcolortype"
#define PRM_SPARE_RAMP_FLOAT_DEFAULT_TOKEN	    "rampfloatdefault"
#define PRM_SPARE_RAMP_KEYS_TOKEN		    "rampkeys"
#define PRM_SPARE_RAMP_KEYS_VAR_TOKEN		    "rampkeys_var"
#define PRM_SPARE_RAMP_SHOW_CONTROLS_DEFAULT_TOKEN  "rampshowcontrolsdefault"
#define PRM_SPARE_RAMP_VALUES_TOKEN		    "rampvalues"
#define PRM_SPARE_RAMP_VALUES_VAR_TOKEN		    "rampvalues_var"
#define PRM_SPARE_SCRIPT_ACTION_HELP_TOKEN	    "script_action_help"
#define PRM_SPARE_SCRIPT_ACTION_ICON_TOKEN	    "script_action_icon"
#define PRM_SPARE_SCRIPT_ACTION_TOKEN		    "script_action"
#define PRM_SPARE_SCRIPT_CALLBACK_TOKEN		    "script_callback"
#define PRM_SPARE_SCRIPT_RITYPE_TOKEN		    "script_ritype"
#define PRM_SPARE_SCRIPT_UNQUOTED_TOKEN		    "script_unquoted"
#define PRM_SPARE_SHADER_PARM_CONTEXTS_TOKEN	    "shaderparmcontexts"
#define PRM_SPARE_SHADER_PARM_TYPE_TOKEN	    "shaderparmtype"
#define PRM_SPARE_UNITS_TOKEN			    "units"
#define PRM_SPARE_XCF_MODE_TOKEN		    "xcfmode"
#define PRM_SPARE_VARYING_DEFAULT_TOKEN		    "varying_default"

class UT_BitArray;
class UT_StringHolder;
class UT_StringRef;
class PRM_Parm;
class prm_SharedString;

typedef int	(*PRM_SpareCallback)(const char *token, const char *value);
typedef int	(*PRM_SpareCallbackWithData)(const char *token, 
					     const char *value, void *data);

typedef UT_Pair<const char *, const char *>	PRM_SpareToken;
typedef UT_VariadicT<PRM_SpareToken>		PRM_SpareArgs;

class PRM_API PRM_SpareData
{
public:
    /// The constructor for the PRM_SpareData should takes a variadic number of
    /// token/value pairs.  For example:
    ///   static PRM_SpareData(PRM_SpareArgs()
    ///		<< PRM_SpareToken("token1", "value1")
    ///		<< PRM_SpareToken("token2", "value2")
    ///		...);
    explicit PRM_SpareData(const PRM_SpareArgs &args);

    /// A single token value pair
    explicit PRM_SpareData(const PRM_SpareToken &pair);
     PRM_SpareData();
    ~PRM_SpareData();

    PRM_SpareData(const PRM_SpareData &copy);
    PRM_SpareData &operator=(const PRM_SpareData &copy);

    uint	 entries() const;

    void	 clear();
    void	 mergeFrom(const PRM_SpareData &ref);
    void	 copyFrom(const PRM_SpareData &ref);
    
    bool	 containsAny(const PRM_SpareData &ref) const;
    bool	 containsAll(const PRM_SpareData &ref) const;

    // The the value associated with a particular token.
    const char	*getValue(const UT_StringRef &token) const;

    // Because these are traverseConst functions, we can't remove any items
    // from the table in the traversal functions.
    void	 traverseConst(PRM_SpareCallback f) const;
    void	 traverseConst(PRM_SpareCallbackWithData f, void *data) const;


    // NOTE:  When addTokenValue is called with a null pointer for the value,
    // the token is deleted from the spare data.
    void	 addTokenValue(const UT_StringHolder &token, const char *value);
    void	 deleteTokenValue(const UT_StringRef &token);

    // For backward compatibility with the ScriptParmSpareData class, we have
    // these functions.
    void	 setCallbackData(const char *str)
		     { addTokenValue(PRM_SPARE_SCRIPT_CALLBACK_TOKEN, str); }
    const char	*getCallbackData() const
		     { return getValue(PRM_SPARE_SCRIPT_CALLBACK_TOKEN); }

    void	 setCallbackLanguage(CH_ScriptLanguage language);
    CH_ScriptLanguage getCallbackLanguage() const;

    void	 setRenderManType(const char *str)
		     { addTokenValue(PRM_SPARE_SCRIPT_RITYPE_TOKEN, str); }
    const char	*getRenderManType() const
		     { return getValue(PRM_SPARE_SCRIPT_RITYPE_TOKEN); }

    void	 setOpFilter(const char *str)
		     { addTokenValue(PRM_SPARE_OPFILTER_TOKEN, str); }
    const char	*getOpFilter() const
		     { return getValue(PRM_SPARE_OPFILTER_TOKEN); }

    void	 setOpRelative(const char *str)
		     { addTokenValue(PRM_SPARE_OPRELATIVE_TOKEN, str); }
    const char	*getOpRelative() const
		     { return getValue(PRM_SPARE_OPRELATIVE_TOKEN); }

    static const char * getUnquotedStateToken()
			    { return PRM_SPARE_SCRIPT_UNQUOTED_TOKEN; }
    void	 setUnquotedState()
		     { addTokenValue(getUnquotedStateToken(), "1"); }
    bool	 getUnquotedState() const
		     { return getValue(getUnquotedStateToken()) != 0; }

    void	 setMultiStartOffset(int value);
    int		 getMultiStartOffset() const;

    void	 setVisibleTabs(const UT_BitArray &values);
    void	 getVisibleTabs(UT_BitArray &values) const;
    bool	 getVisibleTab(int tabidx) const;

    // Ramp
    static const char * getRampBasisDefaultToken()
			    { return PRM_SPARE_RAMP_BASIS_DEFAULT_TOKEN; }
    void		setRampBasisDefault(const char *value)
			    { addTokenValue(getRampBasisDefaultToken(),value); }
    const char *	getRampBasisDefault() const
			    { return getValue(getRampBasisDefaultToken()); }

    static const char * getRampShowControlsDefaultToken()
			    { return PRM_SPARE_RAMP_SHOW_CONTROLS_DEFAULT_TOKEN; }
    void		setRampShowControlsDefault(bool value)
			    {
				addTokenValue(getRampShowControlsDefaultToken(),
					      value ? "1" : "0");
			    }
    bool		getRampShowControlsDefault() const
			    {
				const char *n, *v;
				n = getRampShowControlsDefaultToken();
				v = getValue(n);
				if (!v)
				    return true;
				else
				    return *v != '0';
			    }

    static const char *	getRampColorTypeToken()
			    { return PRM_SPARE_RAMP_COLOR_TYPE_TOKEN; }
    static const char *	getRampColorTypeValueToken(UT_ColorType color_type);
    void		setRampColorType(UT_ColorType color_type);
    UT_ColorType	getRampColorType() const;

    static const char * getRampBasisVarToken()	
		    { return PRM_SPARE_RAMP_BASIS_VAR_TOKEN; }
    static const char * getRampKeysVarToken()	
		    { return PRM_SPARE_RAMP_KEYS_VAR_TOKEN; }
    static const char * getRampValuesVarToken()	
		    { return PRM_SPARE_RAMP_VALUES_VAR_TOKEN; }

    void	 setRampBasisVar(const char *value)
		    { addTokenValue(getRampBasisVarToken(), value); }
    const char * getRampBasisVar() const
		    { return getValue(getRampBasisVarToken()); }
    void	 setRampKeysVar(const char *value)
		    { addTokenValue(getRampKeysVarToken(), value); }
    const char * getRampKeysVar() const
		    { return getValue(getRampKeysVarToken()); }
    void	 setRampValuesVar(const char *value)
		    { addTokenValue(getRampValuesVarToken(), value); }
    const char * getRampValuesVar() const
		    { return getValue(getRampValuesVarToken()); }

    static const char *	getColorRampDefaultToken() 
		    {return PRM_SPARE_RAMP_COLOR_DEFAULT_TOKEN;}
    const char *	getColorRampDefaultValue() const
			    { return getValue(getColorRampDefaultToken()); }
    static const char *	getFloatRampDefaultToken() 
		    {return PRM_SPARE_RAMP_FLOAT_DEFAULT_TOKEN;}
    const char *	getFloatRampDefaultValue() const
			    { return getValue(getFloatRampDefaultToken()); }
    void		setRampDefaultFromParm(const PRM_Parm &ramp_parm);
    void		getRampDefaultToParm(PRM_Parm &ramp_parm) const;

    // Color UI Mode, Wheel flag and Dynamic flag
    static const char *	getColorTypeToken()
			    { return PRM_SPARE_COLOR_TYPE_TOKEN; }
    static const char *	getColorTypeValueToken(PRM_ColorType color_type);
    void		setColorType(PRM_ColorType color_type);
    PRM_ColorType getColorType() const;
    
    static const char *	getColorWheelToken()
			    { return PRM_SPARE_COLOR_WHEEL_TOKEN; }
    void		setColorWheel(bool value);
    bool getColorWheel() const;

    static const char *	getColorDynamicToken()
			    { return PRM_SPARE_COLOR_DYN_TOKEN; }
    void		setColorDynamic(bool value);
    bool getColorDynamic() const;
    
    static const char * getKeyValueDictKeyLabelToken()
			{ return PRM_SPARE_KEY_VALUE_DICT_KEY_LABEL_TOKEN; }
    void	 setKeyValueDictKeyLabel(const char *label);
    const char	*getKeyValueDictKeyLabel() const;
    static const char * getKeyValueDictValueLabelToken()
			{ return PRM_SPARE_KEY_VALUE_DICT_VALUE_LABEL_TOKEN; }
    void	 setKeyValueDictValueLabel(const char *label);
    const char	*getKeyValueDictValueLabel() const;
    static const char * getKeyValueDictUseChooserToken()
			{ return PRM_SPARE_KEY_VALUE_DICT_USE_CHOOSER_TOKEN; }
    void	 setKeyValueDictUseChooser(bool use_chooser);
    bool	 getKeyValueDictUseChooser() const;
    static const char * getKeyValueDictChooserLabelToken()
			{ return PRM_SPARE_KEY_VALUE_DICT_CHOOSER_LABEL_TOKEN; }
    void	 setKeyValueDictChooserLabel(const char *label);
    const char	*getKeyValueDictChooserLabel() const;
    static const char * getKeyValueDictChooserCallbackToken()
		    { return PRM_SPARE_KEY_VALUE_DICT_CHOOSER_CALLBACK_TOKEN; }
    void	 setKeyValueDictChooserCallback(const char *label);
    const char	*getKeyValueDictChooserCallback() const;

    void	 setXCFMode(const char *value)
		    { return addTokenValue(PRM_SPARE_XCF_MODE_TOKEN, value); }
    const char	*getXCFMode() const
		    { return getValue(PRM_SPARE_XCF_MODE_TOKEN); }

    const char	*getScriptAction() const;
    const char	*getScriptActionHelp() const;
    const char	*getScriptActionIcon() const;
    const char	*getButtonIcon() const;

    bool	 getUseEditor() const;
    const char	*getEditorLanguage() const 
		    { return getValue(PRM_SPARE_EDITOR_LANG_TOKEN); }
    void 	 getEditorLinesRange(int &lines_min, int &lines_max) const;
    static inline int	 getEditorLinesRangeDefaultMin() { return 8; }
    static inline int	 getEditorLinesRangeDefaultMax() { return 40; }

    static const char	*getXCFModeFloatValToken() { return "float"; }

    // Token for whether the template will be used for cooking
    static const char *	getCookDependentToken()	
		    { return PRM_SPARE_COOK_DEPENDENT_TOKEN; }
    // Get state of cook_dependent flag.
    // This is only used for SPARE parameters, which if this tag doesn't exist,
    // the spare parm defaults to false for cook dependent.
    bool		getCookDependent() const;
    // Set state of cook_dependent flag.
    // This is only used for SPARE parameters, which if this tag doesn't exist,
    // the spare parm defaults to false for cook dependent.
    void		setCookDependent(bool flag);

    // Token for whether the parm on the VOP should regenerate code
    static const char *getCodeGenDependentToken() 
		    { return PRM_SPARE_CODE_GENERATION_TOKEN; }
    // Get the state of the code_generation flag
    // This is only used for SPARE parameters, which if this tag doesn't exist,
    // the spare parm defaults to true for code generation
    bool	       getCodeGenDependent() const;
    // Set state of code_generation flag.
    // This is only used for SPARE parameters, which if this tag doesn't exist,
    // the spare parm defaults to true for code generation
    void	       setCodeGenDependent(bool flag);

    int64	 getMemoryUsage(bool inclusive) const;

    static const char	*getOpFilterKeyFromName(const char *name);
    static const char	*getOpFilterKeyFromLabel(const char *name);
    static const char	*getOpFilterNameFromKey(const char *key);
    static const char	*getOpFilterLabelFromKey(const char *key);
    // End of backward compatibility

    // Convenience spare data you might want to use
    static PRM_SpareData objNetRef;		// /obj
    static PRM_SpareData chNetRef;		// /ch
    static PRM_SpareData ropNetRef;		// /out

    static PRM_SpareData objPath;		// !!OBJ!!
    static PRM_SpareData objGeometryPath;	// !!OBJ/GEOMETRY!!
    static PRM_SpareData objLightPath;		// !!OBJ/LIGHT!!
    static PRM_SpareData objBonePath;		// !!OBJ/BONE!!
    static PRM_SpareData objMusclePath;		// !!OBJ/MUSCLE!!
    static PRM_SpareData objCameraPath;		// !!OBJ/CAMERA!!
    static PRM_SpareData dopPath;		// !!DOP!!
    static PRM_SpareData sopPath;		// !!SOP!!
    static PRM_SpareData popPath;		// !!POP!!
    static PRM_SpareData ropPath;		// !!ROP!!
    static PRM_SpareData cop2Path;		// !!COP2!!
    static PRM_SpareData chopPath;		// !!CHOP!!
    static PRM_SpareData constraintsPath;	// !!CHOP!!
    static PRM_SpareData shopPath;		// !!SHOP!!
    static PRM_SpareData shopProperties;	// !!SHOP/PROPERTIES!!
    static PRM_SpareData shopMaterial;		// !!SHOP/MATERIAL!!
    static PRM_SpareData shopSurface;		// !!SHOP/SURFACE!!
    static PRM_SpareData shopDisplacement;	// !!SHOP/DISPLACEMENT!!
    static PRM_SpareData shopSurfaceShadow;	// !!SHOP/SURFACE_SHADOW!!
    static PRM_SpareData shopBackground;	// !!SHOP/BACKGROUND!!
    static PRM_SpareData shopGeometry;		// !!SHOP/GEOMETRY!!
    static PRM_SpareData shopAtmosphere;	// !!SHOP/ATMOSPHERE!!
    static PRM_SpareData shopPhoton;		// !!SHOP/PHOTON!!
    static PRM_SpareData shopPhotonVolume;	// !!SHOP/PHOTONVOLUME!!
    static PRM_SpareData shopContour;		// !!SHOP/CONTOUR!!
    static PRM_SpareData shopLight;		// !!SHOP/LIGHT!!
    static PRM_SpareData shopEmitter;		// !!SHOP/EMITTER!!
    static PRM_SpareData shopLightShadow;	// !!SHOP/LIGHT_SHADOW!!
    static PRM_SpareData shopLens;		// !!SHOP/LENS!!
    static PRM_SpareData shopOutput;		// !!SHOP/OUTPUT!!
    static PRM_SpareData shopContourStore;	// !!SHOP/CONTOUR_STORE!!  
    static PRM_SpareData shopContourContrast;	// !!SHOP/CONTOUR_CONTRAST!!  
    static PRM_SpareData shopImage3d;		// !!SHOP/IMAGE3D!!  
    static PRM_SpareData shopCVEX;		// !!SHOP/CVEX!!
    static PRM_SpareData shopCoShader;		// !!SHOP/COSHADER!!
    static PRM_SpareData vopPath;		// !!VOP!!
    static PRM_SpareData lopPath;		// !!LOP!!
    static PRM_SpareData topPath;		// !!TOP!!
    static PRM_SpareData filterMaterial;	// !!CUSTOM/MATERIAL!!

    static PRM_SpareData autoScope;		// autoscope

    static PRM_SpareData multiStartOffsetZero;	// multistart offset of zero
    static PRM_SpareData multiStartOffsetOne;	// multistart offset of one

    static PRM_SpareData unitsLength;		          // m1
    static PRM_SpareData unitsPerLength;	          // m-1
    static PRM_SpareData unitsPerArea;		          // m-2
    static PRM_SpareData unitsMass;		          // kg1
    static PRM_SpareData unitsPerMass;		          // kg-1
    static PRM_SpareData unitsTime;		          // s1
    static PRM_SpareData unitsPerTime;		          // s-1
    static PRM_SpareData unitsPerSquareTime;	          // s-2
    static PRM_SpareData unitsVelocity;		          // m1s-1
    static PRM_SpareData unitsAngVelocity;	          // s-1
    static PRM_SpareData unitsAcceleration;	          // m1s-2
    static PRM_SpareData unitsAngAcceleration;	          // s-2
    static PRM_SpareData unitsForce;		          // kg1m1s-2
    static PRM_SpareData unitsForceDensity;	          // kg1m-2s-2
    static PRM_SpareData unitsImpulse;		          // kg1m1s-1
    static PRM_SpareData unitsTorque;		          // kg1m2s-2
    static PRM_SpareData unitsDrag;		          // kg1s-1
    static PRM_SpareData unitsAngDrag;		          // kg1m2s-1
    static PRM_SpareData unitsPressure;		          // kg1m-1s-2
    static PRM_SpareData unitsSpringConstant;	          // kg1s-2
    static PRM_SpareData unitsLinearDensity;	          // kg1m-1
    static PRM_SpareData unitsLinearDensityPerTime;	  // kg1m-1s-1
    static PRM_SpareData unitsLinearDensityPerSquareTime; // kg1m-1s-2
    static PRM_SpareData unitsAreaDensity;	          // kg1m-2
    static PRM_SpareData unitsAreaDensityPerTime;         // kg1m-2s-1
    static PRM_SpareData unitsAreaDensityPerSquareTime;   // kg1m-2s-2
    static PRM_SpareData unitsDensity;		          // kg1m-3
    static PRM_SpareData unitsDensityPerTime;	          // kg1m-3s-1
    static PRM_SpareData unitsDensityPerSquareTime;       // kg1m-3s-2
    static PRM_SpareData unitsNone;		          // ""

    static PRM_SpareData rampBasisDefault;	// rampbasis "ramp_basis"
    static PRM_SpareData rampKeysDefault;	// rampkeys "ramp_keys"
    static PRM_SpareData rampValuesDefault;	// rampvalues "ramp_values"

    static PRM_SpareData attribAccessR;	 // "sidefx::attrib_access" "read"
    static PRM_SpareData attribAccessW;  // "sidefx::attrib_access" "write"
    static PRM_SpareData attribAccessRW; // "sidefx::attrib_access" "readwrite"

    static PRM_SpareData connectorKindNone; // "sidefx::connector_kind" "none"
    static PRM_SpareData connectorKindIn;   // "sidefx::connector_kind" "in"
    static PRM_SpareData connectorKindOut;  // "sidefx::connector_kind" "out"
    static PRM_SpareData connectorKindInOut;// "sidefx::connector_kind" "inout"

    /// Used to set a space- or comma-separated list of globs to use as 
    /// file filter for parameters of type @c PRM_FILE (e.g. "*.txt *.log").
    static const char *getFileChooserPatternToken()
			    { return PRM_SPARE_FILE_CHOOSER_PATTERN_TOKEN; }

    static const char *getFileChooserModeToken()
			    {return PRM_SPARE_FILE_CHOOSER_MODE_TOKEN;}
    static const char *getFileChooserModeValRead()	
			    { return "read"; }
    static const char *getFileChooserModeValWrite()	
			    { return "write"; }
    static const char *getFileChooserModeValReadAndWrite() 
			    { return "read_and_write"; }

    static PRM_SpareData fileChooserModeRead;	// filechooser_mode "read"
    static PRM_SpareData fileChooserModeWrite;	// filechooser_mode "write"
    static PRM_SpareData fileChooserModeReadAndWrite;	// ... "read_and_write"

    static PRM_SpareData stringFloatXCF;	// float XCF in string field

    static const char *getScriptActionToken()	
			    { return PRM_SPARE_SCRIPT_ACTION_TOKEN; }
    static const char *getScriptActionHelpToken()
			    { return PRM_SPARE_SCRIPT_ACTION_HELP_TOKEN; }
    static const char *getScriptActionIconToken()
			    { return PRM_SPARE_SCRIPT_ACTION_ICON_TOKEN; }
    static const char *getButtonIconToken()	
			    { return PRM_SPARE_BUTTON_ICON_TOKEN; }

    static const char *getEditorToken()	
			    { return PRM_SPARE_EDITOR_TOKEN; }
    static PRM_SpareData stringEditor;		// Use a text editor for strings

    static const char *getEditorLanguageToken()
			    { return PRM_SPARE_EDITOR_LANG_TOKEN; }
    static PRM_SpareData stringEditorLangVex;
    static PRM_SpareData stringEditorLangPython;
    static PRM_SpareData stringEditorLangHScript;
    static PRM_SpareData stringEditorLangExpr;

    /// Number of lines to show by default. This value can be set as either
    /// a single number or a range, e.g. '10-50'. The minimum number is what
    /// the editor will originally show up as, but will attempt to expand up to
    /// the maximum.
    static const char *getEditorLinesRangeToken()	
			    { return PRM_SPARE_EDITOR_LINES_TOKEN; }

    // token indicating the parameter comes from a Parameter VOP (ie it is a
    // shader parameter)
    static const char * getParmVopVarToken()	    
			    { return PRM_SPARE_PARM_VOP_TOKEN; }

    // space-separated list of shading contexts, which a material parameter 
    // belongs to, ie, shaders for each of these contexts actually have
    // this parameter as an argument, since material may contain other shaders
    // for other contexts to which a parameter does not apply
    static const char * getShaderParmContextsToken()   
			    { return PRM_SPARE_SHADER_PARM_CONTEXTS_TOKEN; }

    // token specifying special keywords that are legal in an oppath parameter
    // (in addition to the node path itself)
    static const char * getOpPathKeywordsToken()    
			    { return PRM_SPARE_OPPATH_KEYWORDS_TOKEN; }

    // token for unit specification
    static const char * getUnitsToken()			
			    { return PRM_SPARE_UNITS_TOKEN; }
    
    // token for group specialization (simple/collapsible/toggle groups).
    static const char	*getGroupTypeToken()
			    { return PRM_SPARE_GROUP_TYPE_TOKEN; }
    static PRM_SpareData groupTypeSimple;
    static PRM_SpareData groupTypeCollapsible;
    static const char	*getGroupDefaultToken()
			    { return PRM_SPARE_GROUP_DEFAULT_TOKEN; }

    // Marks parameter has having varying default values
    // Defaults to "0"
    static const char	*getVaryingDefaultToken()
			    { return PRM_SPARE_VARYING_DEFAULT_TOKEN; }
    static PRM_SpareData varyingDefault;
    bool		 getHasVaryingDefault()	const;
    void		 setHasVaryingDefault(bool f);

    // Gets information about whether this parm accepts one or more USD
    // primitive paths, and/or USD collections.
    static const char	*getUsdPathTypeToken()
			    { return PRM_SPARE_USDPATHTYPE_TOKEN; }
    static PRM_SpareToken usdPathTypePrim;
    static PRM_SpareToken usdPathTypePrimList;
    const char		*getUsdPathType() const;

    /// Return true if the given token is a built-in (internally used) spare
    /// data token.
    static bool	 isBuiltInToken(const char *token);

    /// Return a range of the built-in tags names.
    static void	builtInTokens(UT_StringArray &tokens);
private:
    UT_StringMap<prm_SharedString> myTokens;
};

#endif
