/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_SaveOptions.h (GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_SaveOptions__
#define __GA_SaveOptions__

#include "GA_API.h"
#include <UT/UT_Options.h>

/// Class to specify options for loading geometry.
///
/// In addition, when saving to JSON format, see UT_JSONWriter for options.
/// Any options prefixed with 'json:' will be interpreted by the JSON writer
/// (i.e. json:indentstep).
///
/// For each option, there are five methods created: @code
///   static const char *optionName() { return "name"; }
///   TYPE	 option(TYPE defval=DEFAULT) const
///			{ return importOption(name,v) ? v : defval; }
///   bool	 importOption(TYPE &v) const { return importOption(name,v); }
///   void	 setOption() { setOption<>(name, v); }
/// @endcode
class GA_API GA_SaveOptions : public UT_Options
{
public:
    GA_SaveOptions()
	: UT_Options()
    {
    }
    virtual ~GA_SaveOptions();

    GA_SaveOptions	&operator=(const UT_Options &opts)
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
    /// Explicitly specify the format for the geometry (see the @c gconvert
    /// command for available options).
    STR_ACCESSOR("geo:format", format, importFormat, setFormat)
    /// @}
    /// @{
    /// Explicitly specify the extension for the save file (this is used when
    /// saving to a stream).
    STR_ACCESSOR("geo:extension", extension, importExtension, setExtension)
    /// @}

    /// @{
    /// Save the info block in the geometry file.  The info block contains the
    /// bounding box of the geometry, a summary of attributes, and other useful
    /// information.  However, saving this information requires additional
    /// computation, which adds to the cost of saving a file.  Having this
    /// information available makes querying the geometry much more efficient.
    ///
    /// Any options which begin with the prefix "info:" will be written to the
    /// info block.  Common info options are:  the software used to save the
    /// geometry, the artist name, the hostname and a comment.  Most of these
    /// are filled in automatically, but can be overridden with the methods
    /// provided here.
    BOOL_ACCESSOR("geo:saveinfo", saveInfo, importSaveInfo, setSaveInfo, true)
    STR_ACCESSOR("info:software", software, importSoftware, setSoftware)
    STR_ACCESSOR("info:artist", artist, importArtist, setArtist)
    STR_ACCESSOR("info:hostname", hostname, importHostname, setHostname)
    STR_ACCESSOR("info:comment", comment, importComment, setComment)
    /// @}

    /// @{
    /// When saving to a stream (without a filename), indicate whether the
    /// stream should be binary or ASCII.
    BOOL_ACCESSOR("geo:binary", binary, importBinary, setBinary, false)
    /// @}

    /// @{
    /// When saving to a stream (without a filename), indicate whether the
    /// stream should have gzip compression.
    BOOL_ACCESSOR("geo:gzip", gzip, importGZip, setGZip, false)
    /// @}

    /// @{
    /// When saving to a stream (without a filename), indicate whether the
    /// stream should have blosc compression.
    BOOL_ACCESSOR("geo:blosc", blosc, importBlosc, setBlosc, false)
    /// @}

    /// @{
    /// Option used to save all groups
    BOOL_ACCESSOR("geo:savegroups", saveGroups, importSaveGroups,
		setSaveGroups, true)
    /// @}

    /// @{
    /// Option used to save point groups
    BOOL_ACCESSOR("geo:savepointgroups", savePointGroups,
		importSavePointGroups, setSavePointGroups, true)
    /// @}
    /// @{
    /// Option used to save vertex groups
    BOOL_ACCESSOR("geo:savevertexgroups", saveVertexGroups,
		importSaveVertexGroups, setSaveVertexGroups, true)
    /// @}
    /// @{
    /// Option used to save edge groups
    BOOL_ACCESSOR("geo:saveedgegroups", saveEdgeGroups,
		importSaveEdgeGroups, setSaveEdgeGroups, true)
    /// @}

    /// @{
    /// Save groups using RLE encoding of bit stream
    BOOL_ACCESSOR("geo:rlebits", rleBits, importRleBits, setRleBits, true)
    /// @}

    /// @{
    /// Whether to save data arrays using paged structures of flat
    BOOL_ACCESSOR("geo:savepaged", savePaged,
		importSavePaged, setSavePaged, true)
    /// @}

    /// @{
    /// How to check for constant pages when saving
    /// - 0: No constant page optimization
    /// - 1: Use pre-computed page state
    /// - 2: Full test for constant pages
    INT_ACCESSOR("geo:constpagecheck", constPageCheck,
		importConstPageCheck, setConstPageCheck, 2)
    /// @}

    /// @{
    /// Option used to save primitive groups (default true)
    BOOL_ACCESSOR("geo:saveprimitivegroups", savePrimitiveGroups,
		importSavePrimitiveGroups, setSavePrimitiveGroups, true)
    /// @}

    /// @{
    /// Option used to enable saving runs of the same primitive types
    BOOL_ACCESSOR("geo:primrun", primRun, importPrimRun, setPrimRun, true)
    /// @}

    /// @{
    /// Option to specify how many common primitives need to exist in a run
    /// before they are merged into a single run.
    INT_ACCESSOR("geo:primrunqueue", primRunQueue,
		importPrimRunQueue, setPrimRunQueue, 4)
    /// @}

    /// @{
    /// When saving binary geometry, any delayed load sections (i.e. packed
    /// geometry, etc.) are "indexed" which allows for partial loading of
    /// geometry.  This option can turn the feature off.
    BOOL_ACCESSOR("geo:skipsaveindex", skipSaveIndex,
		importSkipSaveIndex, setSkipSaveIndex, false)
    /// @}

    /// @{
    /// When saving geometry, check to see whether we should automatically
    /// compute normals before saving the geometry.  This is used when saving
    /// geometry to mantra to efficiently compute vertex normals.  The value
    /// should be 
    ///  -1 (GA_ATTRIB_INVALID): No automatic export
    ///   0 (GA_ATTRIB_VERTEX):  Compute vertex normals
    ///   1 (GA_ATTRIB_POINT):  Compute point normals
    INT_ACCESSOR("geo:add_normals_to", addNormalsTo,
		importAddNormalsTo, setAddNormalsTo, -1)
    /// @}

    /// @{
    /// Specify the LOD for conversion when saving.  The value should be
    /// greater than 0.  This may be used when saving to formats which don't
    /// natively support certain geometry primitives.
    REAL_ACCESSOR("geo:convert_lod", convertLOD,
		importConvertLOD, setConvertLOD, 1)
    /// @}


    // ----------------------------------------------------------------
    /// @{
    /// A debug option used to save internal groups.
    BOOL_ACCESSOR("group:saveinternal", saveInternal,
		importSaveInternal, setSaveInternal, false)
    /// @}

    /// @{
    /// A debug option used to save private attributes
    BOOL_ACCESSOR("geo:ignoreattribscope", ignoreAttribScope,
		importIgnoreAttribScope, setIgnoreAttribScope, false)
    /// @}

#undef STR_ACCESSOR
#undef INT_ACCESSOR
#undef REAL_ACCESSOR
#undef BOOL_ACCESSOR

private:
};

#endif
