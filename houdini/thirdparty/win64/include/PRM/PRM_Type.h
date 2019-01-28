/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_Type.h (Parameter Library)
 *
 * COMMENTS:
 *		These are the flags which are used to determine the type
 *		of parameter.
 */

#ifndef __PRM_Type__
#define __PRM_Type__

#include "PRM_API.h"
#include "PRM_Callback.h"
#include <UT/UT_IStream.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>

class PRM_SpareData;
class UT_WorkBuffer;

class PRM_API PRM_Type
{
public:
    // These are the basic types of parameters supported.
    // NB: These options are mutually exclusive!
    enum PRM_BasicType {
	PRM_BASIC_NONE		= 0x00000000,
	PRM_BASIC_ORDINAL	= 0x00000001,
	PRM_BASIC_FLOAT		= 0x00000002,
	PRM_BASIC_STRING	= 0x00000004,
	PRM_BASIC_DATA		= 0x00000008,
	PRM_BASIC_MASK		= 0x0000000F
    };

    // These are extra bits of information associated with float parms.
    enum PRM_FloatType {
	PRM_FLOAT_NONE		= 0x00000000,
	// This says that even though it is stored as a floating point number,
	// it is really an integer (eg Number of divisions)
	PRM_FLOAT_INTEGER	= 0x00000001,
    
	PRM_FLOAT_POLAR		= 0x00000004,	// PJ with no radius
	PRM_FLOAT_POLARJACK	= 0x00000008,	// PJ with radius
	PRM_FLOAT_ANGLEJACK	= 0x00000010,	// PJ with radius
	PRM_FLOAT_SPINNER	= 0x00000020,
	PRM_FLOAT_RANGE		= 0x00000080,
	PRM_FLOAT_HUECIRCLE	= 0x00000100,
	PRM_FLOAT_GREYRANGE	= 0x00000200,

	PRM_FLOAT_SLIDERFRAC	= 0x00000800,	// Slider with control over
						// thumb control width. Needs a
						// v[0]=value, v[1]=fraction
						// v[2,3]=range

	PRM_FLOAT_MINMAX_R	= 0x00002000,	// Red
	PRM_FLOAT_MINMAX_G	= 0x00004000,	// Green
	PRM_FLOAT_MINMAX_B	= 0x00008000,	// Blue
	PRM_FLOAT_MINMAX_A	= 0x00010000,	// Alpha
	PRM_FLOAT_MINMAX_RGBA	= 0x00020000,	// RGBA
	PRM_FLOAT_MINMAX_MASK	= 0x0003E000,

	PRM_FLOAT_PALETTE	= 0x00040000,
	// PRM_FLOAT_RAMPEDIT is here for legacy purposes, use
	// PRM_MULTITYPE_FLOAT_RGB or PRM_MULTITYPE_FLT
	PRM_FLOAT_RAMPEDIT	= 0x00080000,
	PRM_FLOAT_RGBA		= 0x00100000,	// Includes alpha slider
						// (should be in NAMERGB, but
						// would break existing code).
	PRM_FLOAT_LOGARITHMIC	= 0x00200000,	// Logaritmic slider
    };

    // These are extra bits of information associated with ordinal parms.
    enum PRM_OrdinalType {
	PRM_ORD_NONE		= 0x00000000,
	PRM_ORD_TOGGLE		= 0x00000001,
	PRM_ORD_CALLBACK	= 0x00000002,
	PRM_ORD_SWITCHERLIST	= 0x00000004,
	PRM_ORD_RGBAMASK	= 0x00000008,
	PRM_ORD_BUTTONSTRIP	= 0x00000010,
	PRM_ORD_ICONSTRIP	= 0x00000020,
    };

    // These are extra bits of information associated with string parms.
    enum PRM_StringType {
	PRM_STR_NONE		= 0x00000000,
	PRM_STR_CMD		= 0x00000001,
	PRM_STR_PATH		= 0x00000002,
	PRM_STR_ALPHA		= 0x00000004,	

	// special flags on type of interest for strings which are ops
	PRM_STR_OP_REF_LIST	= 0x00000008,
	PRM_STR_OP_REF_NAME	= 0x00000010,
	PRM_STR_OP_REF_DATA	= 0x00000020,
	PRM_STR_OP_REF_PARM	= 0x00000040,
	PRM_STR_OP_REF_CHILD	= 0x00000080,
	PRM_STR_OP_REF_MASK	= 0x000000F8,

	// This is used by PROP string parameters that don't want to be quoted
	// when they are fetched as default parameter values.
	PRM_STR_UNQUOTED	= 0x00000100,
	// This is used to specify a label type.
        PRM_STR_LABEL		= 0x00000200,
	// This is a separator type.
        PRM_STR_SEPARATOR	= 0x00000400,
    };

    // These are extra bits of information associated with data parms.
    enum PRM_DataType {
	PRM_DATA_NONE		= 0x00000000,
	PRM_DATA_GEOMETRY	= 0x00000001,
	PRM_DATA_KEY_VALUE_DICT	= 0x00000002,
	PRM_DATA_GEODELTA	= 0x00000004,
    };

    // These are filters for path types.
    // NB: These options are currently treated as mutually exclusive although
    //     in theory, we should add support to allow at least some of them to
    //	   co-exist. eg. PRM_PATH_CMD | PRM_PATH_TXT.
    enum PRM_PathType {
	PRM_PATH_NONE		= 0x00000000,
	PRM_PATH_PIC		= 0x00000001,
	PRM_PATH_GEO		= 0x00000002,
	PRM_PATH_RAMP		= 0x00000004,	// LEGACY
	PRM_PATH_CAPT		= 0x00000008,
	PRM_PATH_CLIP		= 0x00000010,
	/* was PRM_PATH_HMV at    0x00000020 */
	PRM_PATH_PAINT		= 0x00000040,
	PRM_PATH_LUT		= 0x00000080,
	PRM_PATH_CMDF		= 0x00000100,
	PRM_PATH_MIDI		= 0x00000200,
	PRM_PATH_TXT		= 0x00000400,
	PRM_PATH_I3D		= 0x00000800,
	PRM_PATH_CHAN		= 0x00001000,
	PRM_PATH_SIM		= 0x00002000,
	PRM_PATH_SIMDATA	= 0x00004000,
	PRM_PATH_ICON		= 0X00008000,
	PRM_PATH_DIRECTORY	= 0X00010000,

	PRM_PATH_MASK		= 0x0001FFFF
    };

    // These specify the use to which the values are put.
    // This helps with creating the UI for the parameters, and more
    // importantly, provides a means for naming the english-labels and
    // channel-labels. If none of these are set, then they are named with
    // numbers.
    // NB: These options are mutually exclusive!
    enum PRM_ChannelType {
	PRM_CHAN_NONE		= 0x00000000,
	PRM_CHAN_NAMEXYZW	= 0x00000001,
	PRM_CHAN_NAMEUVW	= 0x00000002,
	PRM_CHAN_NAMERGB	= 0x00000004,
	PRM_CHAN_NAMEBEGINEND	= 0x00000008,
	PRM_CHAN_NAMEMAXMIN	= 0x00000010,
	PRM_CHAN_NAMEMINMAX	= 0x00000020,
	PRM_CHAN_NAMESTARTEND	= 0x00000040,
	PRM_CHAN_NAMEXYWH	= 0x00000080,
	PRM_CHAN_MASK		= 0x000000FF
    };

    // These specify UI options that can be combined with any other type.
    enum PRM_InterfaceType {
	PRM_INTERFACE_NONE	= 0x00000000,
	// The following tells PSI to produce no user interface
	PRM_INTERFACE_INVISIBLE	= 0x00000001,
	// This is a flag to indicate whether parameters in a group are
	// exclusive. Used by a switcher, for example, to show radio buttons.
	// instead of regular tabs.
	PRM_INTERFACE_EXCLUSIVE	= 0x00000002,
	// Join to the next parameter, so that both are on the same line
	PRM_INTERFACE_JOIN_NEXT	= 0x00000004,
	// Don't display a label on the parm.
	PRM_INTERFACE_LABEL_NONE= 0x00000008,
	// Display only the numerical text field without the slider.
	PRM_INTERFACE_PLAIN	= 0x00000010,
	// The parameter is animatable.
	PRM_INTERFACE_CHANNEL	= 0x00000020,
	// Overrides PRM_TypeExtended to PRM_TYPE_TOGGLE_JOIN
	PRM_INTERFACE_JOIN_EXT	= 0x00000040,
    };

    // These specify behavioral options that can be combined with any other
    // type.
    enum PRM_BehaviorType {
	PRM_BEHAVIOR_NONE	= 0x00000000,
	// This is a common flag for all parameters. It says that we don't
	// want a change in the parameter to cause a UI refresh. This is the
	// same as NOCOOK except that changing the value for this parameter
	// will also NOT save undos.
	PRM_BEHAVIOR_NOREFRESH	= 0x00000001,
	// This flag is similar to NOREFRESH, but is used specifically by
	// DOPs. This flag is added to all guide geometry parameters and
	// script operator parameters to prevent such parameters from
	// flagging the simulation as dirty when they change. NOREFRESH
	// is not appropriate here because that flag has other effects
	// (like preventing parm change undos from working). NORECOOK is also
	// not appropriate since those parameters still need the DOP to "cook"
	// in order for the viewer to upate.
	PRM_BEHAVIOR_NORESIM	= 0x00000002,
	// This flag is used to indicate that the node owning this parm is not
	// dependent on this parm's value.
	PRM_BEHAVIOR_NOCOOK	= 0x00000004,
	// This flag indicates that this parameter isn't meant to be used as
	// a real parameter at all. It is just a place holder used by the
	// parm layout code.
	PRM_BEHAVIOR_BASEPARM	= 0x00000008
    };


			 PRM_Type();
			 PRM_Type(const PRM_Type &t);
			 PRM_Type(const PRM_BasicType &bt);
			 PRM_Type(const PRM_FloatType &at);
			 PRM_Type(const PRM_OrdinalType &ot);
			 PRM_Type(const PRM_StringType &st);
			 PRM_Type(const PRM_DataType &st);
			 PRM_Type(const PRM_PathType &pt);
			 PRM_Type(const PRM_ChannelType &ct);
			 PRM_Type(const PRM_InterfaceType &it);
			 PRM_Type(const PRM_BehaviorType &vt);
			 PRM_Type(const PRM_BasicType &bt,
				  const PRM_FloatType &at,
				  const PRM_OrdinalType &ot,
				  const PRM_StringType &st,
				  const PRM_DataType &dt,
				  const PRM_PathType &pt,
				  const PRM_ChannelType &ct,
				  const PRM_InterfaceType &it,
				  const PRM_BehaviorType &vt);
			~PRM_Type()
			 {
			 }

    void		 init();

    void		 saveBinary(std::ostream &os) const;
    void		 saveASCII(std::ostream &os) const;
    bool		 loadBinary(UT_IStream &is);

    void		 invert();
    void		 stripUiType();

    const PRM_Type	&operator=(const PRM_Type &t);
    bool		 operator==(const PRM_Type &t) const;
    bool		 operator!=(const PRM_Type &t) const;
    void		 operator|=(const PRM_Type &t);
    void		 operator&=(const PRM_Type &t);

    // this is often used in situations like: (getType() & has_this_type)
    operator		 bool() const;

    // use these for more efficient checking of types
    bool		 isBasicType(const PRM_BasicType &t) const;
    bool		 isChannelType(const PRM_ChannelType &t) const;
    bool		 hasFloatType(const PRM_FloatType &mask) const;
    bool		 hasOrdinalType(const PRM_OrdinalType &mask) const;
    bool		 hasStringType(const PRM_StringType &mask) const;
    bool		 hasDataType(const PRM_DataType &mask) const;
    bool		 hasPathType(const PRM_PathType &mask) const;
    bool		 hasInterfaceType(const PRM_InterfaceType &mask) const;
    bool		 hasBehaviorType(const PRM_BehaviorType &mask) const;

    // commonly accessed flags
    bool		 isOrdinalType() const
			 { return isBasicType(PRM_BASIC_ORDINAL); }
    bool		 isFloatType() const
			 { return isBasicType(PRM_BASIC_FLOAT); }
    bool		 isStringType() const
			 { return isBasicType(PRM_BASIC_STRING); }
    bool		 isDataType() const
			 { return isBasicType(PRM_BASIC_DATA); }
    bool		 isVisible() const
			 { return !hasInterfaceType(PRM_INTERFACE_INVISIBLE); }
    bool		 isAnimatible() const
			 { return hasInterfaceType(PRM_INTERFACE_CHANNEL); }
    bool		 isSwitcher() const
			 {
			     return isOrdinalType()
				    && hasOrdinalType(PRM_OrdinalType(
						      PRM_ORD_SWITCHERLIST));
			 }
    bool		 isCookable() const
			 {
			     return !hasBehaviorType(
					PRM_BehaviorType(
					    PRM_BEHAVIOR_NOREFRESH
					    |PRM_BEHAVIOR_NOCOOK));

			 }

    // accessors
    const PRM_BasicType		&getBasicType() const;
    const PRM_FloatType		&getFloatType() const;
    const PRM_OrdinalType	&getOrdinalType() const;
    const PRM_StringType	&getStringType() const;
    const PRM_DataType		&getDataType() const;
    const PRM_PathType		&getPathType() const;
    const PRM_ChannelType	&getChannelType() const;
    const PRM_InterfaceType	&getInterfaceType() const;
    const PRM_BehaviorType	&getBehaviorType() const;

private:
    friend const PRM_Type operator|(const PRM_Type &t1, const PRM_Type &t2);
    friend const PRM_Type operator&(const PRM_Type &t1, const PRM_Type &t2);
    friend const PRM_Type operator~(const PRM_Type &t1);
    friend const PRM_Type PRM_STRIP_UI_TYPE(const PRM_Type &t1);

private:
    PRM_BasicType	 myBasicType;
    PRM_FloatType	 myFloatType;
    PRM_OrdinalType	 myOrdinalType;
    PRM_StringType	 myStringType;
    PRM_DataType	 myDataType;
    PRM_PathType	 myPathType;
    PRM_ChannelType	 myChannelType;
    PRM_InterfaceType	 myInterfaceType;
    PRM_BehaviorType	 myBehaviorType;
};

// Binary Operations on PRM_Type
inline const PRM_Type	 operator|(const PRM_Type &t1, const PRM_Type &t2);
inline const PRM_Type	 operator&(const PRM_Type &t1, const PRM_Type &t2);
inline const PRM_Type	 operator~(const PRM_Type &t1);
inline const PRM_Type	 PRM_STRIP_UI_TYPE(const PRM_Type &t1);


// These values all correspond to a single bit from the various
// enumerated types above. These single-bit values can be combined
// to build more complex parm types.
PRM_API extern const PRM_Type	 PRM_TYPE_ZERO;
PRM_API extern const PRM_Type	 PRM_TYPE_ORDINAL;
PRM_API extern const PRM_Type	 PRM_TYPE_FLOAT;
PRM_API extern const PRM_Type	 PRM_TYPE_STRING;
PRM_API extern const PRM_Type	 PRM_TYPE_DATA;
PRM_API extern const PRM_Type	 PRM_TYPE_GEOMETRY;
PRM_API extern const PRM_Type	 PRM_TYPE_KEY_VALUE_DICT;
PRM_API extern const PRM_Type	 PRM_TYPE_GEODELTA;
PRM_API extern const PRM_Type	 PRM_TYPE_BASIC_TYPE;
PRM_API extern const PRM_Type	 PRM_TYPE_PALETTE;
PRM_API extern const PRM_Type	 PRM_TYPE_INTEGER;
PRM_API extern const PRM_Type	 PRM_TYPE_CHANNEL;
PRM_API extern const PRM_Type	 PRM_TYPE_POLAR;
PRM_API extern const PRM_Type	 PRM_TYPE_POLARJACK;
PRM_API extern const PRM_Type	 PRM_TYPE_ANGLEJACK;
PRM_API extern const PRM_Type	 PRM_TYPE_SPINNER;
PRM_API extern const PRM_Type	 PRM_TYPE_RANGE;
PRM_API extern const PRM_Type	 PRM_TYPE_HUECIRCLE;
PRM_API extern const PRM_Type	 PRM_TYPE_GREYRANGE;
PRM_API extern const PRM_Type	 PRM_TYPE_SLIDERFRACTION;
PRM_API extern const PRM_Type	 PRM_TYPE_MINMAX_RGBAX;
PRM_API extern const PRM_Type	 PRM_TYPE_MINMAX_R;
PRM_API extern const PRM_Type	 PRM_TYPE_MINMAX_G;
PRM_API extern const PRM_Type	 PRM_TYPE_MINMAX_B;
PRM_API extern const PRM_Type	 PRM_TYPE_MINMAX_A;
PRM_API extern const PRM_Type	 PRM_TYPE_MINMAX_RGBA;
PRM_API extern const PRM_Type	 PRM_TYPE_RAMPEDIT;	    // LEGACY
PRM_API extern const PRM_Type	 PRM_TYPE_RGBA;
PRM_API extern const PRM_Type	 PRM_TYPE_LOGARITHMIC;
PRM_API extern const PRM_Type	 PRM_TYPE_TOGGLE;
PRM_API extern const PRM_Type	 PRM_TYPE_CALLBACK;
PRM_API extern const PRM_Type	 PRM_TYPE_SWITCHERLIST;
PRM_API extern const PRM_Type	 PRM_TYPE_RGBAMASK;
PRM_API extern const PRM_Type	 PRM_TYPE_BUTTONSTRIP;
PRM_API extern const PRM_Type	 PRM_TYPE_ICONSTRIP;
PRM_API extern const PRM_Type	 PRM_TYPE_CMD;
PRM_API extern const PRM_Type	 PRM_TYPE_PATH;
PRM_API extern const PRM_Type	 PRM_TYPE_ALPHA;
PRM_API extern const PRM_Type	 PRM_TYPE_OP_REF_LIST;
PRM_API extern const PRM_Type	 PRM_TYPE_OP_REF_NAME;
PRM_API extern const PRM_Type	 PRM_TYPE_OP_REF_DATA;
PRM_API extern const PRM_Type	 PRM_TYPE_OP_REF_PARM;
PRM_API extern const PRM_Type	 PRM_TYPE_OP_REF_CHILD;
PRM_API extern const PRM_Type	 PRM_TYPE_OP_REF_MASK;
PRM_API extern const PRM_Type	 PRM_TYPE_UNQUOTED;
PRM_API extern const PRM_Type	 PRM_TYPE_LABEL;
PRM_API extern const PRM_Type	 PRM_TYPE_SEPARATOR;
PRM_API extern const PRM_Type	 PRM_TYPE_PIC;
PRM_API extern const PRM_Type	 PRM_TYPE_GEO;
PRM_API extern const PRM_Type	 PRM_TYPE_RAMP;		    // LEGACY
PRM_API extern const PRM_Type	 PRM_TYPE_CAPT;
PRM_API extern const PRM_Type	 PRM_TYPE_CLIP;
PRM_API extern const PRM_Type	 PRM_TYPE_PAINT;
PRM_API extern const PRM_Type	 PRM_TYPE_LUT;
PRM_API extern const PRM_Type	 PRM_TYPE_CMDF;
PRM_API extern const PRM_Type	 PRM_TYPE_MIDI;
PRM_API extern const PRM_Type	 PRM_TYPE_TXT;
PRM_API extern const PRM_Type	 PRM_TYPE_I3D;
PRM_API extern const PRM_Type	 PRM_TYPE_CHAN;
PRM_API extern const PRM_Type	 PRM_TYPE_SIM;
PRM_API extern const PRM_Type	 PRM_TYPE_SIMDATA;
PRM_API extern const PRM_Type	 PRM_TYPE_DIRECTORY;
PRM_API extern const PRM_Type	 PRM_TYPE_FILTERS;
PRM_API extern const PRM_Type	 PRM_TYPE_NAMEXYZW;
PRM_API extern const PRM_Type	 PRM_TYPE_NAMEUVW;
PRM_API extern const PRM_Type	 PRM_TYPE_NAMERGB;
PRM_API extern const PRM_Type	 PRM_TYPE_NAMEBEGINEND;
PRM_API extern const PRM_Type	 PRM_TYPE_NAMEMAXMIN;
PRM_API extern const PRM_Type	 PRM_TYPE_NAMEMINMAX;
PRM_API extern const PRM_Type	 PRM_TYPE_NAMESTARTEND;
PRM_API extern const PRM_Type	 PRM_TYPE_NAMEXYWH;
PRM_API extern const PRM_Type	 PRM_TYPE_NAME;
PRM_API extern const PRM_Type	 PRM_TYPE_NOREFRESH;
PRM_API extern const PRM_Type	 PRM_TYPE_NORESIM;
PRM_API extern const PRM_Type	 PRM_TYPE_NOCOOK;
PRM_API extern const PRM_Type	 PRM_TYPE_INVISIBLE;
PRM_API extern const PRM_Type	 PRM_TYPE_EXCLUSIVE;
PRM_API extern const PRM_Type	 PRM_TYPE_JOIN_NEXT;
PRM_API extern const PRM_Type	 PRM_TYPE_LABEL_NONE;
PRM_API extern const PRM_Type	 PRM_TYPE_PLAIN;
PRM_API extern const PRM_Type	 PRM_TYPE_BASEPARM;

/// This type enum defines the different types of multi (dynamic) parameters.
enum PRM_MultiType
{
    PRM_MULTITYPE_LIST		= 0x00000000,	/// Normal list (WARNING: 0!)
    PRM_MULTITYPE_SCROLL	= 0x00000001,	/// Put inside a scrollable area
    PRM_MULTITYPE_SWITCHER	= 0x00000002,	/// Each instance is a tab
    PRM_MULTITYPE_NONE		= 0x00000004,	/// Since "list" is already 0

    PRM_MULTITYPE_RAMP_FLT	= 0x00000008,	/// Float ramp type
    PRM_MULTITYPE_RAMP_RGB	= 0x00000010,	/// RGB ramp type
    PRM_MULTITYPE_RAMP_MASK	= 0x00000018,	/// Mask for testing ramp

    PRM_MULTITYPE_TYPEMASK      = 0x0FFFFFFF,	/// Mask for excluding modifiers

    // modifier flags
    PRM_MULTITYPE_NO_CONTROL_UI = 0x10000000	/// Prohibit UI for add/remove
};

enum PRM_ColorType{
    PRM_COLORTYPE_FLOAT_FIELDS,
    PRM_COLORTYPE_RGB_SLIDERS,
    PRM_COLORTYPE_HSV_SLIDERS,
    PRM_COLORTYPE_TMI_SLIDERS,
};

enum PRM_RampInterpType {
    PRM_RAMP_INTERP_INVALID	= -1,
    PRM_RAMP_INTERP_CONSTANT	= 0,
    PRM_RAMP_INTERP_LINEAR,
    PRM_RAMP_INTERP_CATMULLROM,
    PRM_RAMP_INTERP_MONOTONECUBIC,
    PRM_RAMP_INTERP_BEZIER,
    PRM_RAMP_INTERP_BSPLINE,
    PRM_RAMP_INTERP_HERMITE,
    PRM_RAMP_INTERP_ITEMS
};

PRM_API const char *	    PRMgetRampInterpToken(PRM_RampInterpType interp);
PRM_API PRM_RampInterpType  PRMgetRampInterpType(const char *token);

class PRM_ChoiceList;
PRM_API PRM_ChoiceList *    PRMgetRampInterpMenu();

// Templates correspond to PRM_MULTITYPE_RAMP_*. It must be here due to
// static initialization issues.
class PRM_Template;
PRM_API PRM_Template *	    PRMcreateRampTemplate(
				const char *parent_token,
				PRM_MultiType multi_type,
				const PRM_SpareData *parent_spare,
				PRM_Callback callback,
				PRM_Template *copy_templates);
PRM_API void		    PRMfreeRampTemplate(
				PRM_Template *templates);
// NOTE: Always includes the space of the PRM_Template objects
PRM_API int64		    PRMgetRampTemplateMemoryUsage(
				PRM_Template *templates);

// Parm indices for the template retrieved from PRMcreateRampTemplate()
// NOTE: The order of this enum must correspond to PRMcreateRampTemplate()!
enum PRM_RampParmIndex
{
    PRM_RAMP_PARM_POS,
    PRM_RAMP_PARM_VAL,
    PRM_RAMP_PARM_INTERP,
    PRM_RAMP_NUM_PARMS	// sentinel
};

// Get the token names corresponding to the given parent token
PRM_API void PRMgetRampChannelToken(const char *parent_token,
				    PRM_MultiType multi_type,
				    PRM_RampParmIndex i,
				    int sub_idx,
				    UT_WorkBuffer &token);
PRM_API int PRMgetRampChannelCount(PRM_MultiType multi_type,
				   PRM_RampParmIndex i);

// If the parent token for a ramp parm ends in a '#' or digit, then we add
// an underscore to ensure that we don't end up with ambiguous situations.
// Use the following function if you need to get the proper prefix with the
// underscore added.
PRM_API void PRMgetRampParmPrefix(const char *parent_token,
				  UT_WorkBuffer &prefix);

//
//  Try not to use bit's and bit masks in this new type. It's caused
//  lot's of hassles with the old type. If you need to add options to
//  a parm or a template then add more member data for storing the options
//  or add more types to this new enum.
//
enum PRM_TypeExtended
{
    PRM_TYPE_NONE,
    PRM_TYPE_TOGGLE_JOIN,	// Enable toggle joined with next parameter.
    PRM_TYPE_JOIN_PAIR,		// Parm joined with next parm.
    PRM_TYPE_VEC_SWITCHER,	// Component/Vector switcher (POPs)
    PRM_TYPE_NO_LABEL,		// Usually for menus: leave the label off.
    PRM_TYPE_MINI_MENU,		// For ones like the resolution menu.
    PRM_TYPE_JUMP_REFERENCE,	// Network and node reference
    PRM_TYPE_NET_REFERENCE,	// Network only reference (e.g. material)
    PRM_TYPE_WIDE_STRING,	// For path fields (e.g. file OPs)
    PRM_TYPE_COP_CHROMAKEY,	// Special hook for color picker
    PRM_TYPE_COP_VIDEOKEY,	// Special hook for color picker
    PRM_TYPE_COP_TEXMAP,	// Special hook for corner mapper
    PRM_TYPE_COP_CORNERPIN,	// Special hook for corner mapper
    PRM_TYPE_COP_CROP,		// Special hook for corner mapper
    PRM_TYPE_COP_SCALE,		// Special hook for pixel values
    PRM_TYPE_COP_SEQUENCE,	// Special hook for sequence cop
    PRM_TYPE_SHOP_PATH,		// Special hook for SHOP paths
    PRM_TYPE_DYNAMIC_PATH,	// Special hook for dynamic path
    PRM_TYPE_DYNAMIC_PATH_LIST,	// Special hook for dynamic path lists
    PRM_TYPE_UNUSED_1,		// (was: log sliders, use PRM_FLT_LOG instead)
    PRM_TYPE_FULL_JUMP_REFERENCE,   // Full operator path reference
    PRM_TYPE_MATRIX_ROW,	// Ensure that the fields are all on one row
    PRM_TYPE_IMAGE_FORMAT_1,	// First part of an format/options pair
    PRM_TYPE_IMAGE_FORMAT_2,	// Second part of an format/options pair
    PRM_TYPE_MENU_JOIN,		// A menu joined with the next parameter
    PRM_TYPE_IMAGE_OPTIONS,	// Used w/PRM_TYPE_PIC: read format options
    PRM_TYPE_IMAGE_RGBA_OPTIONS	// Used w/PRM_TYPE_PIC: read options for RGBA

    // Instead of adding a new entry to this enum, please consider using 
    // PRM_SpareData instead (especially for non-ui options). There are a few
    // advantages: several options can be used at the same time, the dialog
    // scripts don't support this enum but they do support spare data,
    // PI_EditScriptedParms inherits spare data when promoting the parameter,
    // and an easier integration into OPUI_EditParms.
};

//
// The PRM_TYPE_IMAGE_FORMAT template should look like this:
//	PRM_Template(PRM_TYPE_PICFILE, ... 1, ..., callback1),
//	PRM_Template(PRM_TYPE_STRING,  PRM_TYPE_IMAGE_FORMAT_2, 1, 
//			PRM_ChoiceList(PRMbuildImageDeviceMenu), ... callback2),
//	PRM_Template(PRM_TYPE_STRING,  PRM_TYPE_IMAGE_FORMAT_3, 2, ...)
//
//  Please see PRM_SharedFunc.h for more information.
//	

// Some common parm types:

// The terminator for a parm list. Just a zero value.
PRM_API extern const PRM_Type PRM_LIST_TERMINATOR;
// A switcher to create multiple parm pages. Uses tabs to switch pages.
PRM_API extern const PRM_Type PRM_SWITCHER;
// A switcher to create multiple parm pages. Uses radio buttons to switch pages.
PRM_API extern const PRM_Type PRM_SWITCHER_EXCLUSIVE;
// A switcher to create multiple parm pages. Recooks the node when changed.
PRM_API extern const PRM_Type PRM_SWITCHER_REFRESH;
// A non-animatable float value.
PRM_API extern const PRM_Type PRM_FLT_E;
// A float value which can be animated.
PRM_API extern const PRM_Type PRM_FLT;
PRM_API extern const PRM_Type PRM_FLT_J;

// A float value which is represented logarithmically.
PRM_API extern const PRM_Type PRM_FLT_LOG_E;
// A float value which is represented logarithmically which can be animated
PRM_API extern const PRM_Type PRM_FLT_LOG;
// A non-animatable float triple with xyz channels.
PRM_API extern const PRM_Type PRM_XYZ_E;
// A float triple with xyz channel names which can be animated.
PRM_API extern const PRM_Type PRM_XYZ;
PRM_API extern const PRM_Type PRM_XYZ_J;
// A non-animatable float triple with uvw channels.
PRM_API extern const PRM_Type PRM_UVW_E;
// A float triple with uvw channel names which can be animated.
PRM_API extern const PRM_Type PRM_UVW;
PRM_API extern const PRM_Type PRM_UVW_J;
// A non-animatable float triple with rgb channels.
PRM_API extern const PRM_Type PRM_RGB_E;
// A float triple with rgb channel names which can be animated.
PRM_API extern const PRM_Type PRM_RGB;
PRM_API extern const PRM_Type PRM_RGB_J;
// A non-animatable float quad with rgba channels.
PRM_API extern const PRM_Type PRM_RGBA_E;
// A float quad with rgba channel names which can be animated.
PRM_API extern const PRM_Type PRM_RGBA;
PRM_API extern const PRM_Type PRM_RGBA_J;
// A non-animatable float quad with begin/end channels.
PRM_API extern const PRM_Type PRM_BEGINEND_E;
// A float quad with begin/end channel names which can be animated.
PRM_API extern const PRM_Type PRM_BEGINEND;
PRM_API extern const PRM_Type PRM_BEGINEND_J;
// A non-animatable float pair with start/end channel names.
PRM_API extern const PRM_Type PRM_STARTEND_E;
// A float pair with start/end channel names which can be animated.
PRM_API extern const PRM_Type PRM_STARTEND;
PRM_API extern const PRM_Type PRM_STARTEND_J;
// A non-animatable int value.
PRM_API extern const PRM_Type PRM_INT_E;
// An int value which can be animated.
PRM_API extern const PRM_Type PRM_INT;

// A non-animatable int value  which is represented logarithmically.
PRM_API extern const PRM_Type PRM_INT_LOG_E;
// An int value which is represented logarithmically and can be animated.
PRM_API extern const PRM_Type PRM_INT_LOG;


PRM_API extern const PRM_Type PRM_INT_J;
// A non-animatable int triple with xyz channel names.
PRM_API extern const PRM_Type PRM_INT_XYZ_E;
// An int triple with xyz channel names which can be animated.
PRM_API extern const PRM_Type PRM_INT_XYZ;
PRM_API extern const PRM_Type PRM_INT_XYZ_J;
// A non-animatable float pair with min/max channels.
PRM_API extern const PRM_Type PRM_FLT_MINMAX_E;
// A float pair with min/max channel names which can be animated.
PRM_API extern const PRM_Type PRM_FLT_MINMAX;
PRM_API extern const PRM_Type PRM_FLT_MINMAX_J;
// A non-animatable int pair with min/max channels.
PRM_API extern const PRM_Type PRM_INT_MINMAX_E;
// An int pair with min/max channel names which can be animated.
PRM_API extern const PRM_Type PRM_INT_MINMAX;
PRM_API extern const PRM_Type PRM_INT_MINMAX_J;
// A non-animatable int pair with start/end channels.
PRM_API extern const PRM_Type PRM_INT_STARTEND_E;
// An int pair with start/end channel names which can be animated.
PRM_API extern const PRM_Type PRM_INT_STARTEND;
PRM_API extern const PRM_Type PRM_INT_STARTEND_J;
// A ramp editor, which uses a non-animatable float parameter. (LEGACY)
PRM_API extern const PRM_Type PRM_FLT_RAMPEDIT;
// A check box.
PRM_API extern const PRM_Type PRM_TOGGLE_E;
// A check box which can be animated.
PRM_API extern const PRM_Type PRM_TOGGLE;
PRM_API extern const PRM_Type PRM_TOGGLE_J;
// A string that contains an hscript or unix command.
PRM_API extern const PRM_Type PRM_COMMAND;
// A string that points to any file.
PRM_API extern const PRM_Type PRM_FILE_E;
// A string that points to any file which can be animated.
PRM_API extern const PRM_Type PRM_FILE;
// A string that points to an image file.
PRM_API extern const PRM_Type PRM_PICFILE_E;
// A string that points to an image file which can be animated.
PRM_API extern const PRM_Type PRM_PICFILE;
// A string that points to a geometry file.
PRM_API extern const PRM_Type PRM_GEOFILE_E;
// A string that points to a geometry file which can be animated.
PRM_API extern const PRM_Type PRM_GEOFILE;
// A string that points to a capture override file.
PRM_API extern const PRM_Type PRM_CAPTFILE_E;
// A string that points to a capture override file which can be animated.
PRM_API extern const PRM_Type PRM_CAPTFILE;
// A string that points to a ramp file. (LEGACY)
PRM_API extern const PRM_Type PRM_RAMPFILE_E;
// A string that points to a ramp file which can be animated. (LEGACY)
PRM_API extern const PRM_Type PRM_RAMPFILE;
// A string that points to a clip (channel) file.
PRM_API extern const PRM_Type PRM_CLIPFILE_E;
// A string that points to a clip (channel) file which can be animated.
PRM_API extern const PRM_Type PRM_CLIPFILE;
// A string that points to an image or ramp file.
PRM_API extern const PRM_Type PRM_PAINTFILE_E;
// A string that points to an image or ramp file which can be animated.
PRM_API extern const PRM_Type PRM_PAINTFILE;
// A string that points to a LUT file.
PRM_API extern const PRM_Type PRM_LUTFILE_E;
// A string that points to a LUT file which can be animated.
PRM_API extern const PRM_Type PRM_LUTFILE;
// A string that points to a script file.
PRM_API extern const PRM_Type PRM_CMDFILE_E;
// A string that points to a script file which can be animated.
PRM_API extern const PRM_Type PRM_CMDFILE;
// A string that points to a MIDI file.
PRM_API extern const PRM_Type PRM_MIDIFILE_E;
// A string that points to a MIDI file which can be animated.
PRM_API extern const PRM_Type PRM_MIDIFILE;
// A string that points to a text file.
PRM_API extern const PRM_Type PRM_TXTFILE_E;
// A string that points to a text file which can be animated.
PRM_API extern const PRM_Type PRM_TXTFILE;
// A string that points to an i3d file.
PRM_API extern const PRM_Type PRM_I3DFILE_E;
// A string that points to an i3d file which can be animated.
PRM_API extern const PRM_Type PRM_I3DFILE;
// A string that points to a channel file file.
PRM_API extern const PRM_Type PRM_CHANFILE_E;
// A string that points to a channel file file which can be animated.
PRM_API extern const PRM_Type PRM_CHANFILE;
// A string that points to a simulation file.
PRM_API extern const PRM_Type PRM_SIMFILE_E;
// A string that points to a simulation file which can be animated.
PRM_API extern const PRM_Type PRM_SIMFILE;
// A string that points to an icon file.
PRM_API extern const PRM_Type PRM_ICONFILE_E;
// A string that points to an icon file which can be animated.
PRM_API extern const PRM_Type PRM_ICONFILE;
// A string that points to a siulation data file.
PRM_API extern const PRM_Type PRM_SIMDATAFILE_E;
// A string that points to a siulation data file which can be animated.
PRM_API extern const PRM_Type PRM_SIMDATAFILE;
// A string that points to a directory.
PRM_API extern const PRM_Type PRM_DIRECTORY_E;
// A string that points to a directory which can be animated.
PRM_API extern const PRM_Type PRM_DIRECTORY;
// A non-animatable string parameter.
PRM_API extern const PRM_Type PRM_STRING_E;
// An animatable string parameter.
PRM_API extern const PRM_Type PRM_STRING;
// An alphanumeric string (for node, variable, or group names)
PRM_API extern const PRM_Type PRM_ALPHASTRING;
// A string containing a list of operator paths.
PRM_API extern const PRM_Type PRM_STRING_OPLIST;
// A string containing the name of a single operator.
PRM_API extern const PRM_Type PRM_STRING_OPREF;
// A string containing the name of a single operator that is our child.
PRM_API extern const PRM_Type PRM_STRING_OPREF_CHILD;
// A data parameter.
PRM_API extern const PRM_Type PRM_DATA;
// A non-animatable ordinal type.
PRM_API extern const PRM_Type PRM_ORD_E;
// An ordinal type which can be animated.
PRM_API extern const PRM_Type PRM_ORD;
PRM_API extern const PRM_Type PRM_ORD_J;
// An animatable float with a polar UI.
PRM_API extern const PRM_Type PRM_POLAR;
// A non-animatable float with an angle UI.
PRM_API extern const PRM_Type PRM_ANGLE_E;
// An animatable float with an angle UI.
PRM_API extern const PRM_Type PRM_ANGLE;
PRM_API extern const PRM_Type PRM_ANGLE_J;
// A non-animatable float triple with angle UI and xyz channel names.
PRM_API extern const PRM_Type PRM_ANGLEXYZ_E;
// An animatable float triple with angle UI and xyz channel names.
PRM_API extern const PRM_Type PRM_ANGLEXYZ;
PRM_API extern const PRM_Type PRM_ANGLEXYZ_J;
// A non-animatable float pair with angle UI and min/max channel names.
PRM_API extern const PRM_Type PRM_ANGLE_MINMAX_E;
// An animatable float pair with angle UI and min/max channel names.
PRM_API extern const PRM_Type PRM_ANGLE_MINMAX;
PRM_API extern const PRM_Type PRM_ANGLE_MINMAX_J;
// An animatable float with a polar jack UI.
PRM_API extern const PRM_Type PRM_POLARJACK;
// An animatable int with up/down arrow buttons instead of a slider 
PRM_API extern const PRM_Type PRM_INT_SPINNER;
// An animatable float with up/down arrow buttons instead of a slider
PRM_API extern const PRM_Type PRM_SPINNER;

// An animatable float with a range UI.
PRM_API extern const PRM_Type PRM_RANGE;
// An animatable float with a hue circle UI.
PRM_API extern const PRM_Type PRM_HUECIRCLE;
// An animatable float with a palette UI.
PRM_API extern const PRM_Type PRM_PALETTE;
// An animatable float with a grey range UI.
PRM_API extern const PRM_Type PRM_GREYRANGE;
// An animatable float with polar UI and xyz channel names.
PRM_API extern const PRM_Type PRM_DIRECTION;
// Like PRM_DIRECTION, but not animatable.
PRM_API extern const PRM_Type PRM_DIRECTION_E;
PRM_API extern const PRM_Type PRM_DIRECTION_NOJ;
// A button which triggers a callback.
PRM_API extern const PRM_Type PRM_CALLBACK;
// A button which triggers a callback but does not recook the node,
//   create undos, nor cause the hip file to be modified.
PRM_API extern const PRM_Type PRM_CALLBACK_NOREFRESH;
// An ordinal parameter for masking R/G/B/A image channels.
PRM_API extern const PRM_Type PRM_RGBAMASK;
// A text buttonstrip for doing general masking up to 32 bits (using a menu)
PRM_API extern const PRM_Type PRM_BUTTONSTRIP;
// An icon buttonstrip for doing general masking up to 32 bits (using a menu)
PRM_API extern const PRM_Type PRM_ICONSTRIP;
// An animatable float pair with min/max channel names for the R component.
PRM_API extern const PRM_Type PRM_MINMAX_R;
// An animatable float pair with min/max channel names for the G component.
PRM_API extern const PRM_Type PRM_MINMAX_G;
// An animatable float pair with min/max channel names for the B component.
PRM_API extern const PRM_Type PRM_MINMAX_B;
// An animatable float pair with min/max channel names for the A component.
PRM_API extern const PRM_Type PRM_MINMAX_A;
// An animatable float pair with min/max channel names for all RGBA components.
PRM_API extern const PRM_Type PRM_MINMAX_RGBA;
// A label (for dialog appearance only).
PRM_API extern const PRM_Type PRM_LABEL;
// A separator (for dialog appearance only).
PRM_API extern const PRM_Type PRM_SEPARATOR;
// A geometry data object.
PRM_API extern const PRM_Type PRM_GEOMETRY;
// A key-value dictionary (keys are strings, values are strings).
PRM_API extern const PRM_Type PRM_KEY_VALUE_DICT;
// A Geometry Delta.
PRM_API extern const PRM_Type PRM_GEODELTA;
// A combination label and separator.
PRM_API extern const PRM_Type PRM_HEADING;
// A checkbox parameter joined to the next parameter.
PRM_API extern const PRM_Type PRM_JOINED_TOGGLE;

// explaining PRM_SWITCHER:
//     This is for switcher groups in PSI. The size and name of each group 
//     is in the defaults list. the parameters which follow will be used
//     to make the switcher groups.
//
// eg:
//	PRM_Default d[] =
//	{
//	    PRM_Default(3, "label 1"),	
//	    PRM_Default(1, "label 2")
//	};
//
//	PRM_Template t[] =
//	{
//					// 2 means there will be two groups
//	    PRM_Template(PRM_SWITCHER, 2, &somename, d),    
//	    PRM_Template(PRM_XYZ, 1, &somename),    // These three will
//	    PRM_Template(PRM_XYZ, 1, &somename),    // be in the first 
//	    PRM_Template(PRM_XYZ, 1, &somename),    // group named "label 1"
//
//	    PRM_Template(PRM_XYZ, 1, &somename),    // This will be
//					// in the second group named "label 2"
//
//	    PRM_Template(PRM_XYZ, 1, &somename),    
//				// and this is outside the switcher
//	};




// PRM_SWITCHER		int, named 123,	( see example above )
// PRM_FLT_E		float, named 123
// PRM_FLT		float, named 123, jive-menu
// PRM_XYZ_E		float, named xyz
// PRM_XYZ		float, named xyz, jive-menu
// PRM_UVW_E		float, named uvw
// PRM_UVW		float, named uvw, jive-menu
// PRM_DIR		float, namex xyz, jive-menu, polarjack
// PRM_BEGINEND_E	float, named begin/end
// PRM_BEGINEND		float, named begin/end, jive-menu
// PRM_ANGLE		float, named begin/end, jive-menu, anglejacks
// PRM_INT_E		int, named 123
// PRM_INT		int, named 123, jive-menu
// PRM_INT_XYZ_E	int, named xyz
// PRM_INT_XYZ		int, named xyz, jive-menu
// PRM_FLT_MINMAX	float, named min/max, jive-menu
// PRM_INT_MINMAX	int, named min/max, jive-menu
// PRM_TOGGLE		ord, toggle
// PRM_ORD		ord
// PRM_CALLBACK		ord, callback (either a button or a menu)
// PRM_STRING		string
// PRM_FILE		string, file-prompter
// Feel free to | these three:
// PRM_PICFILE		string, file-prompter with a pic filter	
// PRM_GEOFILE		string, file-prompter with a geo filter
// PRM_RAMPFILE		string, file-prompter with a ramp filter (LEGACY)
// PRM_DATA		data

//////////////////////////////////////////////////////////////////////////////

inline void
PRM_Type::init()
{
    myBasicType = PRM_BASIC_NONE;
    myFloatType = PRM_FLOAT_NONE;
    myOrdinalType = PRM_ORD_NONE;
    myStringType = PRM_STR_NONE;
    myDataType = PRM_DATA_NONE;
    myPathType = PRM_PATH_NONE;
    myChannelType = PRM_CHAN_NONE;
    myInterfaceType = PRM_INTERFACE_NONE;
    myBehaviorType = PRM_BEHAVIOR_NONE;
}
inline
PRM_Type::PRM_Type()
{
    init();
}

inline
PRM_Type::PRM_Type(const PRM_Type &t)
{
    *this = t;
}

inline
PRM_Type::PRM_Type(const PRM_BasicType &bt)
{
    init();
    myBasicType = bt;
}

inline
PRM_Type::PRM_Type(const PRM_FloatType &at)
{
    init();
    myFloatType = at;
}

inline
PRM_Type::PRM_Type(const PRM_OrdinalType &ot)
{
    init();
    myOrdinalType = ot;
}

inline
PRM_Type::PRM_Type(const PRM_StringType &st)
{
    init();
    myStringType = st;
}

inline
PRM_Type::PRM_Type(const PRM_DataType &dt)
{
    init();
    myDataType = dt;
}

inline
PRM_Type::PRM_Type(const PRM_PathType &pt)
{
    init();
    myPathType = pt;
}

inline
PRM_Type::PRM_Type(const PRM_ChannelType &ct)
{
    init();
    myChannelType = ct;
}

inline
PRM_Type::PRM_Type(const PRM_InterfaceType &it)
{
    init();
    myInterfaceType = it;
}

inline
PRM_Type::PRM_Type(const PRM_BehaviorType &vt)
{
    init();
    myBehaviorType = vt;
}

inline
PRM_Type::PRM_Type(const PRM_BasicType &bt,
		   const PRM_FloatType &at,
		   const PRM_OrdinalType &ot,
		   const PRM_StringType &st,
		   const PRM_DataType &dt,
		   const PRM_PathType &pt,
		   const PRM_ChannelType &ct,
		   const PRM_InterfaceType &it,
		   const PRM_BehaviorType &vt)
    :	myBasicType(bt),
	myFloatType(at),
	myOrdinalType(ot),
	myStringType(st),
	myDataType(dt),
	myPathType(pt),
	myChannelType(ct),
	myInterfaceType(it),
	myBehaviorType(vt)
{
}

inline const PRM_Type &
PRM_Type::operator=(const PRM_Type &t)
{
    myBasicType = t.myBasicType;
    myFloatType = t.myFloatType;
    myOrdinalType = t.myOrdinalType;
    myStringType = t.myStringType;
    myDataType = t.myDataType;
    myPathType = t.myPathType;
    myChannelType = t.myChannelType;
    myInterfaceType = t.myInterfaceType;
    myBehaviorType = t.myBehaviorType;

    return *this;
}

inline bool
PRM_Type::operator==(const PRM_Type &t) const
{
    return (myBasicType == t.myBasicType &&
	    myFloatType == t.myFloatType &&
	    myOrdinalType == t.myOrdinalType &&
	    myStringType == t.myStringType &&
	    myDataType == t.myDataType &&
	    myPathType == t.myPathType &&
	    myChannelType == t.myChannelType &&
	    myInterfaceType == t.myInterfaceType &&
	    myBehaviorType == t.myBehaviorType);
}

inline bool
PRM_Type::operator!=(const PRM_Type &t) const
{
    return !(*this == t);
}

inline
PRM_Type::operator bool() const
{
    return (myBasicType != PRM_BASIC_NONE ||
	    myFloatType != PRM_FLOAT_NONE ||
	    myOrdinalType != PRM_ORD_NONE ||
	    myStringType != PRM_STR_NONE ||
	    myDataType != PRM_DATA_NONE ||
	    myPathType != PRM_PATH_NONE ||
	    myChannelType != PRM_CHAN_NONE ||
	    myInterfaceType != PRM_INTERFACE_NONE ||
	    myBehaviorType != PRM_BEHAVIOR_NONE);
}

inline void
PRM_Type::operator|=(const PRM_Type &t)
{
    myBasicType = PRM_BasicType(myBasicType | t.myBasicType);
    myFloatType = PRM_FloatType(myFloatType | t.myFloatType);
    myOrdinalType = PRM_OrdinalType(myOrdinalType | t.myOrdinalType);
    myStringType = PRM_StringType(myStringType | t.myStringType);
    myDataType = PRM_DataType(myDataType | t.myDataType);
    myPathType = PRM_PathType(myPathType | t.myPathType);
    myChannelType = PRM_ChannelType(myChannelType | t.myChannelType);
    myInterfaceType = PRM_InterfaceType(myInterfaceType | t.myInterfaceType);
    myBehaviorType = PRM_BehaviorType(myBehaviorType | t.myBehaviorType);
}

inline void
PRM_Type::operator&=(const PRM_Type &t)
{
    myBasicType = PRM_BasicType(myBasicType & t.myBasicType);
    myFloatType = PRM_FloatType(myFloatType & t.myFloatType);
    myOrdinalType = PRM_OrdinalType(myOrdinalType & t.myOrdinalType);
    myStringType = PRM_StringType(myStringType & t.myStringType);
    myDataType = PRM_DataType(myDataType & t.myDataType);
    myPathType = PRM_PathType(myPathType & t.myPathType);
    myChannelType = PRM_ChannelType(myChannelType & t.myChannelType);
    myInterfaceType = PRM_InterfaceType(myInterfaceType & t.myInterfaceType);
    myBehaviorType = PRM_BehaviorType(myBehaviorType & t.myBehaviorType);
}

inline void
PRM_Type::invert()
{
    myBasicType = PRM_BasicType(~myBasicType);
    myFloatType = PRM_FloatType(~myFloatType);
    myOrdinalType = PRM_OrdinalType(~myOrdinalType);
    myStringType = PRM_StringType(~myStringType);
    myDataType = PRM_DataType(~myDataType);
    myPathType = PRM_PathType(~myPathType);
    myChannelType = PRM_ChannelType(~myChannelType);
    myInterfaceType = PRM_InterfaceType(~myInterfaceType);
    myBehaviorType = PRM_BehaviorType(~myBehaviorType);
}

inline void
PRM_Type::stripUiType()
{
    myInterfaceType = PRM_INTERFACE_NONE;
    myBehaviorType = PRM_BEHAVIOR_NONE;
}

inline bool
PRM_Type::isBasicType(const PRM_BasicType &t) const
{
    return (myBasicType == t);
}

inline bool
PRM_Type::isChannelType(const PRM_ChannelType &t) const
{
    return (myChannelType == t);
}

inline bool
PRM_Type::hasFloatType(const PRM_FloatType &mask) const
{
    return ((myFloatType & mask) != 0);
}

inline bool
PRM_Type::hasOrdinalType(const PRM_OrdinalType &mask) const
{
    return ((myOrdinalType & mask) != 0);
}

inline bool
PRM_Type::hasStringType(const PRM_StringType &mask) const
{
    return ((myStringType & mask) != 0);
}

inline bool
PRM_Type::hasDataType(const PRM_DataType &mask) const
{
    return ((myDataType & mask) != 0);
}

inline bool
PRM_Type::hasPathType(const PRM_PathType &mask) const
{
    return ((myPathType & mask) != 0);
}

inline bool
PRM_Type::hasInterfaceType(const PRM_InterfaceType &mask) const
{
    return ((myInterfaceType & mask) != 0);
}

inline bool
PRM_Type::hasBehaviorType(const PRM_BehaviorType &mask) const
{
    return ((myBehaviorType & mask) != 0);
}

inline const PRM_Type::PRM_BasicType &
PRM_Type::getBasicType() const
{
    return myBasicType;
}

inline const PRM_Type::PRM_FloatType &
PRM_Type::getFloatType() const
{
    return myFloatType;
}

inline const PRM_Type::PRM_OrdinalType &
PRM_Type::getOrdinalType() const
{
    return myOrdinalType;
}

inline const PRM_Type::PRM_StringType &
PRM_Type::getStringType() const
{
    return myStringType;
}

inline const PRM_Type::PRM_DataType &
PRM_Type::getDataType() const
{
    return myDataType;
}

inline const PRM_Type::PRM_PathType &
PRM_Type::getPathType() const
{
    return myPathType;
}

inline const PRM_Type::PRM_ChannelType &
PRM_Type::getChannelType() const
{
    return myChannelType;
}

inline const PRM_Type::PRM_InterfaceType &
PRM_Type::getInterfaceType() const
{
    return myInterfaceType;
}

inline const PRM_Type::PRM_BehaviorType &
PRM_Type::getBehaviorType() const
{
    return myBehaviorType;
}

inline const PRM_Type
operator|(const PRM_Type &t1, const PRM_Type &t2)
{
    return  PRM_Type(
	PRM_Type::PRM_BasicType(t1.myBasicType | t2.myBasicType),
	PRM_Type::PRM_FloatType(t1.myFloatType | t2.myFloatType),
	PRM_Type::PRM_OrdinalType(t1.myOrdinalType | t2.myOrdinalType),
	PRM_Type::PRM_StringType(t1.myStringType | t2.myStringType),
	PRM_Type::PRM_DataType(t1.myDataType | t2.myDataType),
	PRM_Type::PRM_PathType(t1.myPathType | t2.myPathType),
	PRM_Type::PRM_ChannelType(t1.myChannelType | t2.myChannelType),
	PRM_Type::PRM_InterfaceType(t1.myInterfaceType | t2.myInterfaceType),
	PRM_Type::PRM_BehaviorType(t1.myBehaviorType | t2.myBehaviorType));
}

inline const PRM_Type
operator&(const PRM_Type &t1, const PRM_Type &t2)
{
    return  PRM_Type(
	PRM_Type::PRM_BasicType(t1.myBasicType & t2.myBasicType),
	PRM_Type::PRM_FloatType(t1.myFloatType & t2.myFloatType),
	PRM_Type::PRM_OrdinalType(t1.myOrdinalType & t2.myOrdinalType),
	PRM_Type::PRM_StringType(t1.myStringType & t2.myStringType),
	PRM_Type::PRM_DataType(t1.myDataType & t2.myDataType),
	PRM_Type::PRM_PathType(t1.myPathType & t2.myPathType),
	PRM_Type::PRM_ChannelType(t1.myChannelType & t2.myChannelType),
	PRM_Type::PRM_InterfaceType(t1.myInterfaceType & t2.myInterfaceType),
	PRM_Type::PRM_BehaviorType(t1.myBehaviorType & t2.myBehaviorType));
}

inline const PRM_Type
operator~(const PRM_Type &t1)
{
    return PRM_Type(
	PRM_Type::PRM_BasicType(~t1.myBasicType),
	PRM_Type::PRM_FloatType(~t1.myFloatType),
	PRM_Type::PRM_OrdinalType(~t1.myOrdinalType),
	PRM_Type::PRM_StringType(~t1.myStringType),
	PRM_Type::PRM_DataType(~t1.myDataType),
	PRM_Type::PRM_PathType(~t1.myPathType),
	PRM_Type::PRM_ChannelType(~t1.myChannelType),
	PRM_Type::PRM_InterfaceType(~t1.myInterfaceType),
	PRM_Type::PRM_BehaviorType(~t1.myBehaviorType));
}

inline const PRM_Type
PRM_STRIP_UI_TYPE(const PRM_Type &t1)
{
    return PRM_Type(t1.myBasicType,
		    t1.myFloatType,
		    t1.myOrdinalType,
		    t1.myStringType,
		    t1.myDataType,
		    t1.myPathType,
		    t1.myChannelType,
		    PRM_Type::PRM_INTERFACE_NONE,
		    PRM_Type::PRM_BEHAVIOR_NONE);
}

// Overload for custom formatting of PRM_Type with UTformat.
PRM_API size_t format(char *buffer, size_t buffer_size, const PRM_Type &v);

#endif
