/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_SaveMap.h ( GA Library, C++)
 *
 * COMMENTS:	Contains options used for load/save
 */

#ifndef __GA_SaveMap__
#define __GA_SaveMap__

#include "GA_API.h"
#include "GA_Types.h"
#include "GA_Range.h"
#include "GA_SharedLoadData.h"
#include "GA_SaveOptions.h"

#include <UT/UT_Assert.h>
#include <UT/UT_Options.h>
#include <UT/UT_String.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_StringSet.h>

#include <SYS/SYS_Types.h>
#include <stddef.h>

class GA_Detail;
class GA_Primitive;
class GA_SecondaryLookupInfo;
class GA_IndexMap;
class GA_PrimitiveFactory;
class UT_JSONParser;


// This is used by GA_LoadMap
#define GA_SAVEMAP_END_TOKEN	":end"


/// @brief Used to pass options and map offset values during saving.
///
/// This class is a container which stores iterators that iterate over the
/// points/primitives/vertices to be saved.  It also stores keyword arguments
/// in the form of a UT_Options.
/// @see UT_JSONWriter
class GA_API GA_SaveMap 
{
public:
    /// Generic save map used when saving geometry
    GA_SaveMap(const GA_Detail &detail, const GA_SaveOptions *options);

    /// Destructor
    ~GA_SaveMap();

    /// The options in the save map are used to control behaviour during
    /// saving/loading of the geometry.  The user can pass any arbitrary
    /// key/value pairs.  Common options (used in the baseline GA library) are:
    ///
    /// - @b bool @b geo:primrun           -- Enable primitive run compression
    /// - @b int  @b geo:primrunqueue      -- Size of primitives run queue [4]
    /// - @b bool @b geo:savepointgroups   -- Save point groups [true]
    /// - @b bool @b geo:saveprimitivegroups -- Save primitive groups [true]
    /// - @b bool @b geo:savevertexgroups  -- Save vertex groups [true]
    /// - @b bool @b geo:saveedgegroups    -- Save edge groups [true]
    /// - @b bool @b geo:savegroups        -- Save all groups [true]
    /// - @b bool @b geo:saveinternal      -- Saving of internal groups [false]
    /// - @b bool @b geo:houdini10format   -- Save in Houdini10 format [false]
    /// - @b bool @b geo:rlebits   -- Run length encoding of bit arrays [true]
    /// - @b bool @b geo:ignoreattribscope -- Save private attribs by default
    //                                        [false]
    ///
    /// Additional options for controlling the JSON stream can be found in
    /// UT_JSONWriter::setOptions() (i.e. json:precision).
    /// @see UT_JSONWriter
    const GA_SaveOptions	&getOptions() const	{ return myOptions; }

    /// During the loading process, the file version may be set
    void		 setFileVersion(const char *v)  { myFVersion.harden(v);}
    const char		*getFileVersion() const		{ return myFVersion; }

    /// Get the geometry being loaded
    const GA_Detail	&getDetail() const	{ return myDetail; }

    /// Return the iterator for the points to be saved
    const GA_Range	&getPointRange() const
			    { return myI[GA_ATTRIB_POINT]; }
    /// Return the iterator for the vertices to be saved
    const GA_Range	&getVertexRange() const
			{
			    if (!myI[GA_ATTRIB_VERTEX].isValid())
				lazyMakeVertexRange();
			    return myI[GA_ATTRIB_VERTEX];
			}
    /// Return the iterator for the primitives to be saved
    const GA_Range	&getPrimitiveRange() const
			    { return myI[GA_ATTRIB_PRIMITIVE]; }
    /// Return the detail iterator
    const GA_Range	&getGlobalRange() const
			    { return myI[GA_ATTRIB_GLOBAL]; }
    /// Return an arbitrary iterator based on the owner type
    const GA_Range	&getRange(GA_AttributeOwner i) const
			{
			    if (i == GA_ATTRIB_VERTEX && !myI[i].isValid())
				lazyMakeVertexRange();
			    return myI[i];
			}

    /// Take a point offset and returns its offset(index) in the save file.
    GA_Index		getPointIndex(GA_Offset point_offset) const;
    /// Take a vertex offset and returns its offset(index) in the save file.
    GA_Index		getVertexIndex(GA_Offset vertex_offset) const;
    /// Take a primitive offset and returns its offset(index) in the save file.
    GA_Index		getPrimitiveIndex(GA_Offset primitive_offset) const;

    /// Return the save index associated with an element
    GA_Index		getIndex(GA_Offset offset, GA_AttributeOwner o) const
			{
			    switch (o)
			    {
				case GA_ATTRIB_VERTEX:
				    return getVertexIndex(offset);
				case GA_ATTRIB_POINT:
				    return getPointIndex(offset);
				case GA_ATTRIB_PRIMITIVE:
				    return getPrimitiveIndex(offset);
				case GA_ATTRIB_DETAIL:
				    UT_ASSERT(offset == GA_Offset(0));
				    return GA_Index(0);
				default:
				    break;
			    }
			    UT_ASSERT(0 && "Invalid attribute owner");
			    return GA_INVALID_INDEX;
			}

    // Returns true if all offsets of this owner would be mapped to themselves.
    bool isTrivialMap(GA_AttributeOwner owner) const;

    /// Check the save options.  Return @c defvalue if token isn't defined.
    bool	getDefaultedBool(const char *name, bool defvalue) const;
    /// Check the save options.  Return @c defvalue if token isn't defined.
    exint	getDefaultedInt(const char *name, exint defvalue) const;
    /// Check the save options.  Return @c defvalue if the token isn't defined
    const char	*getDefaultedString(const char *name, UT_String &storage,
				const char *defvalue) const;

    /// Convert the secondary primitives offsets in the given lookup object
    /// into indices.
    bool	getSecondaryIndices(GA_Offset primary_offset,
				    const GA_SecondaryLookupInfo &info,
				    UT_Int64Array &secondary_prim_indices) const;

    // -------------------------------------------------------------------
    // Methods to query save options.  These methods provide a common
    // interface to querying the UT_Options which are passed to the save/load
    // methods.
    /// "bool geo:saveinfo" @n Whether to save info block
    bool	optionSaveInfo() const
		    { return getDefaultedBool("geo:saveinfo", true); }

    /// "string info:artist" @n String containing the artist's name
    const char	*optionArtistName(UT_String &s) const
		{ return getDefaultedString("info:artist", s, NULL); }
    /// "string info:hostname" @n String containing the host name
    const char	*optionHostname(UT_String &s) const
		{ return getDefaultedString("info:hostname", s, NULL); }
    /// "string info:date" @n Current date (default: %Y-%m-%d %T)
    const char	*optionDate(UT_String &s) const
		{ return getDefaultedString("info:date", s, NULL); }
    /// "string info:software" @n Software (and version)
    const char	*optionSoftware(UT_String &s) const
		{ return getDefaultedString("info:software", s, NULL); }

    /// "bool info:savebounds" @n Whether to compute and save bounding
    /// box in the info block.
    bool	optionSaveBounds() const
		{ return getDefaultedBool("info:savebounds", true); }

    /// "bool info:saveprimcounts" @n Whether to compute and save the counts of
    /// each primitive type into the info block.
    bool	optionSavePrimCount() const
		{ return getDefaultedBool("info:saveprimcounts", true); }

    /// "bool info:savevolumesummary" @n Whether to compute and save the 
    /// volume info into the info block.
    bool	optionSaveVolumeSummary() const
		{ return getDefaultedBool("info:savevolumesummary", true); }

    /// "bool info:saveattributesummary" @n Whether to save a summary of
    /// attributes in the info block.
    bool	optionSaveAttributeSummary() const
		{ return getDefaultedBool("info:saveattributesummary", true); }

    /// "bool info:saverenderattributeranges" @n
    /// Will save the ranges for velocity attributes ("v" as point/primitive)
    /// and the "width" attribute (point, primitive, detail) if they exist.
    /// These can be used by rendering procedurals to adjust the bounds to
    /// include velocity and width attributes.
    ///
    /// This is included in the attribute summary, so for this option to work,
    /// you need to have @c info:saveattributesummary enabled.
    bool	optionSaveRenderAttributeRanges() const
		{
		    return getDefaultedBool("info:saverenderattributeranges",
			    true);
		}

    /// "bool info:savegroupsummary" @n Whether to save a summary of
    /// groups in the info block.
    bool	optionSaveGroupSummary() const
		{ return getDefaultedBool("info:savegroupsummary", true); }

    /// "bool geo:savepointgroups" @n Whether to save point groups
    /// @note This will returns the intersection with "bool geo:savegroups"
    bool	optionSavePointGroups() const
		{
		    return getDefaultedBool("geo:savegroups", true) &&
			getDefaultedBool("geo:savepointgroups", true);
		}
    /// "bool geo:saveprimitivegroups" @n Whether to save primitive groups
    /// @note This will returns the intersection with "bool geo:savegroups"
    bool	optionSavePrimitiveGroups() const
		{
		    return getDefaultedBool("geo:savegroups", true) &&
			getDefaultedBool("geo:saveprimitivegroups", true);
		}
    /// "bool geo:savevertexgroups" @n Whether to save vertex groups
    /// @note This will returns the intersection with "bool geo:savegroups"
    bool	optionSaveVertexGroups() const
		{
		    return getDefaultedBool("geo:savegroups", true) &&
			getDefaultedBool("geo:savevertexgroups", true);
		}
    /// "bool geo:saveedgegroups" @n Whether to save edge groups
    /// @note This will returns the intersection with "bool geo:savegroups"
    bool	optionSaveEdgeGroups() const
		{
		    return getDefaultedBool("geo:savegroups", true) &&
			getDefaultedBool("geo:saveedgegroups", true);
		}
    /// "bool geo:savebreakpointgroups" @n Whether to save breakpoint groups
    /// @note This will returns the intersection with "bool geo:savegroups"
    bool	optionSaveBreakpointGroups() const
		{
		    return getDefaultedBool("geo:savegroups", true) &&
			getDefaultedBool("geo:savebreakpointgroups", true);
		}

    /// "bool geo:ignoreattribscope" @n
    /// When saving, private attributes are not typically saved.  If
    /// this option is set, the scope of attributes should be ignored
    /// The GA_OPTION_EXPORT_ON_SAVE should override this option.
    bool	optionIgnoreAttributeScope() const
		{ return getDefaultedBool("geo:ignoreattributescope", false); }

    /// "string geo:attributesavemask" @n
    /// Specify the "mask" for attributes which should be saved.  This
    /// mask is in the form used by UT_String::multiMatch().
    /// 
    const char 	*optionAttributeSaveMask(UT_String &storage,
			const char *default_mask="*") const
		{
		    return getDefaultedString(
				"geo:attributesavemask", storage, default_mask);
		}

    /// "string geo:groupsavemask" @n
    /// Specify the "mask" for groups which should be saved.  This
    /// mask is in the form used by UT_String::multiMatch()
    const char	*optionGroupSaveMask(UT_String &storage,
			const char *default_mask="*") const
		{
		    return getDefaultedString(
				"geo:groupsavemask", storage, default_mask);
		}
    // End of common save options
    // -------------------------------------------------------------------

    /// Test if the data associated with the unique key has been saved
    /// The key specified should be something which is guaranteed to be unique
    /// for the given data.  However, it should be the same key for each shared
    /// instance.  For example, given a shared pointer to data, you might
    /// consider: @code
    ///    sprintf(key, "%s:%s:%p",
    ///			getTypeName(),	// Primitive type name
    ///			"data_type",
    ///			shared_ptr.get());
    /// @endcode
    bool	hasSavedSharedData(const char *key) const;
    /// Indicate the given data has been shared
    void	setSavedSharedData(const char *key);

private:
    void	lazyMakeVertexRange() const;
    void	reallyMakeVertexRange();
    const GA_Detail		&myDetail;
    GA_SaveOptions		 myOptions;
    GA_Range			 myI[4];
    UT_String			 myFVersion;
    GA_ListType<GA_Offset, GA_Index>	 myVertexIndexFromOffset;
    UT_StringSet		 mySharedDataKeys;
};

#endif
