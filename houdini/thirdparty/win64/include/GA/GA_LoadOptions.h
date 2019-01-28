/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_LoadOptions.h (GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_LoadOptions__
#define __GA_LoadOptions__

#include "GA_API.h"
#include <UT/UT_Options.h>

/// Class to specify options for loading geometry
class GA_API GA_LoadOptions : public UT_Options
{
public:
    GA_LoadOptions()
	: UT_Options()
    {
    }
    virtual ~GA_LoadOptions();

    GA_LoadOptions	&operator=(const UT_Options &opts)
    {
	UT_Options::operator=(opts);
	return *this;
    }

#define BOOL_ACCESSOR(NAME, GET, IMPORT, SET, DEFAULT)	\
    static const char	*GET##Name() { return NAME; } \
    bool IMPORT(bool &v) const { return importOption(NAME, v); } \
    bool GET() const { \
	bool v; \
	if (!importOption(NAME, v)) v = DEFAULT; \
	return v; \
    } \
    void SET(bool v) { setOptionB(UTmakeUnsafeRef(NAME), v); }
#define INT_ACCESSOR(NAME, GET, IMPORT, SET, DEFAULT)	\
    static const char	*GET##Name() { return NAME; } \
    bool IMPORT(int32 &v) const { return importOption(NAME, v); } \
    bool IMPORT(int64 &v) const { return importOption(NAME, v); } \
    int64 GET() const { \
	int64 v; \
	if (!importOption(NAME, v)) v = DEFAULT; \
	return v; \
    } \
    void SET(int64 v) { setOptionI(UTmakeUnsafeRef(NAME), v); }
#define REAL_ACCESSOR(NAME, GET, IMPORT, SET, DEFAULT)	\
    static const char	*GET##Name() { return NAME; } \
    bool IMPORT(fpreal32 &v) const { return importOption(NAME, v); } \
    bool IMPORT(fpreal64 &v) const { return importOption(NAME, v); } \
    fpreal64 GET() const { \
	fpreal64 v; \
	if (!importOption(NAME, v)) v = DEFAULT; \
	return v; \
    } \
    void SET(fpreal64 v) { setOptionI(UTmakeUnsafeRef(NAME), v); }
#define STR_ACCESSOR(NAME, GET, IMPORT, SET)	\
    static const char	*GET##Name() { return NAME; } \
    bool IMPORT(UT_StringHolder &v) const { return importOption(NAME, v); } \
    UT_StringHolder GET() const { return getOptionS(NAME); } \
    void SET(const UT_StringHolder &v) { setOptionS(UTmakeUnsafeRef(NAME), v); }

    /// @{
    /// Tell the loader that the geometry loaded will be read-only (never
    /// modified - i.e. no new points, no new attributes, no modification of
    /// data, etc.).  This lets the loader perform things like
    /// on-demand/deferred loading of data.
    BOOL_ACCESSOR("geo:readonly", readOnly, importReadOnly, setReadOnly, false)
    /// @}

    /// @{
    /// The @c fileversion option is set to the value of the tag in the JSON
    /// file.
    STR_ACCESSOR("fileversion", fileversion, importFileVersion, setFileversion)
    /// @}

    /// @{
    /// The @c geo:attributeloadmask is a "mask" to specify which attributes
    /// should be loaded.  The mask is in the form used by
    /// UT_String::multiMatch()
    STR_ACCESSOR("geo:attributeloadmask",
		attributeLoadMask, importAttributeLoadMask, setAttributeLoadMask)
    /// @}

    /// @{
    /// The @c geo:grouploadmask is a "mask" to specify which groups should be
    /// loaded.  The mask is in the form used by UT_String::multiMatch()
    STR_ACCESSOR("geo:grouploadmask", groupLoadMask,
		importGroupLoadMask, setGroupLoadMask)
    /// @}

    /// @{
    /// Whether to load primitives, or just load the points for the geometry
    BOOL_ACCESSOR("geo:loadprimitives", loadPrimitives,
		importLoadPrimitives, setLoadPrimitives, true)
    /// @}

    /// @{
    /// Whether to disable load warnings
    BOOL_ACCESSOR("geo:disable_warnings", disableWarnings,
		importDisableWarnings, setDisableWarnings, false)
    /// @}

    /// @{
    /// When loading classic Houdini files, an attribute translation file can
    /// be applied.  This is normally $HH/GEOclassicXlate.xml, but can be
    /// overridden by this option.
    STR_ACCESSOR("geo:classicxlatefile", classicXlateFile,
		importClassicXlateFile, setClassicXlateFile)
    /// @}

    /// @{
    /// Internal flag to indicate whether this is a delayed load of data
    BOOL_ACCESSOR("geo:delayload", delayLoad,
		importDelayLoad, setDelayLoad, false)
    /// @}

    /// @{
    /// When loading packed geometry, you can override the "viewport level of
    /// detail" by setting the appropriate value here.  Note that not all
    /// packed primitives respect this option.
    STR_ACCESSOR("geo:overridePackedViewportLOD", overridePackedViewportLOD,
	    importOverridePackedViewportLOD, setOverridePackedViewportLOD)
    /// @}

#undef STR_ACCESSOR
#undef INT_ACCESSOR
#undef REAL_ACCESSOR
#undef BOOL_ACCESSOR
};

#endif
