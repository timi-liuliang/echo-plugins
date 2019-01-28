/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_LoadMap.h
 *
 * COMMENTS:
 */

#ifndef GA_LOADMAP_H_
#define GA_LOADMAP_H_

#include "GA_API.h"
#include "GA_Types.h"
#include "GA_Range.h"
#include "GA_SharedLoadData.h"
#include "GA_LoadOptions.h"
#include "GA_GeometryIndex.h"
#include "GA_SharedDataHandle.h"

#include <UT/UT_Assert.h>
#include <UT/UT_String.h>
#include <UT/UT_StringSet.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_Set.h>
#include <UT/UT_SharedPtr.h>

#include <SYS/SYS_Types.h>

#include <stddef.h>

class GA_Detail;
class GA_Primitive;
class GA_SecondaryLookupInfo;
class GA_IndexMap;
class GA_PrimitiveFactory;
class UT_JSONParser;

/// @brief Options during loading
class GA_API GA_LoadMap
{
public:

    typedef UT_StringMap<GA_SharedDataHandlePtr> SharedDataHandleMapType;
    typedef UT_StringMap<GA_Primitive*> PrimitiveStorageMapType;

    // Class used to call the constructor that builds a map for loading into
    // an existing, possibility fragmented, detail.
    class nontrivialmap {};

     GA_LoadMap(GA_Detail &gdp, const GA_LoadOptions *options,
		GA_Offset vtxoff, GA_Offset ptoff, GA_Offset primoff);
     GA_LoadMap(GA_Detail &gdp, const GA_LoadOptions *options, nontrivialmap);
    ~GA_LoadMap();

    /// Get the geometry being loaded
    GA_Detail		&getDetail() const	{ return myDetail; }

    /// Read only version of options.  Options which callers might set that the
    /// loading code can examine:
    /// - @b bool @b geo:readonly @n
    ///   This indicates that the geometry will be loaded as read-only geometry
    ///   and will never be modified (no new points, no new attributes, no
    ///   modification of data).  This can be used to perform things like
    ///   on-demand/deferred loading of data.
    /// - @b string @b fileversion @n
    ///   This is set to the "fileversion" tag in the JSON file.
    const GA_LoadOptions	&getOptions() const	{ return myOptions; }

    /// Set/get the file version
    void	setFileVersion(const char *version)
		{ myOptions.setOptionS("fileversion", version); }
    const char	*getFileVersion(UT_String &storage) const
		{ return getDefaultedString("fileversion", storage, ""); }
    /// Test to see whether the file version is at least the given version
    bool	isFileAtLeastVersion(const char *version) const;

    /// Set a string option
    void	setOptionS(const char *key, const char *value)
		    { myOptions.setOptionS(key, value); }
    /// Set an integer option
    void	setOptionI(const char *key, int v)
		    { myOptions.setOptionI(key, v); }
    /// Set a real/float option
    void	setOptionF(const char *key, fpreal v)
		    { myOptions.setOptionF(key, v); }

    /// Return the index map used to map the load offset to the real offset,
    /// if any.  An active map is present when loading a fragmented block.
    const GA_IndexMap	*getActivePointMap() const
				{ return myPointMap; }
    /// Return the index map used to map the load offset to the real offset,
    /// if any.  An active map is present when loading a fragmented block.
    const GA_IndexMap	*getActiveVertexMap() const
				{ return myVertexMap; }
    /// Return the index map used to map the load offset to the real offset,
    /// if any.  An active map is present when loading a fragmented block.
    const GA_IndexMap	*getActivePrimitiveMap() const
				{ return myPrimitiveMap; }
    /// Return the index map used to map the load offset to the real offset,
    /// if any.  An active map is present when loading a fragmented block.
    const GA_IndexMap	*getActiveIndexMap(GA_AttributeOwner owner) const
				{
				    switch (owner)
				    {
					case GA_ATTRIB_POINT:
					    return myPointMap;
					case GA_ATTRIB_VERTEX:
					    return myVertexMap;
					case GA_ATTRIB_PRIMITIVE:
					    return myPrimitiveMap;
					default:
					    break;
				    }
				    UT_ASSERT(owner == GA_ATTRIB_DETAIL);
				    return 0;
				}

    /// Return the mapping of the load offset to the real offset
    GA_Offset	getOffset(GA_Size load_index, GA_AttributeOwner o) const
		    { return GA_Offset(getLoadOffset(o) + load_index); }
    /// Map the load_index to the actual point number in the detail
    GA_Offset	getPointOffset(GA_Size load_index) const
		    {
			if (myPointMap)
			    return offsetFromIndex(myPointMap,
						   myPointOffset + load_index);
			else
			    return GA_Offset(myPointOffset + load_index);
		    }
    /// Map the load_index to the actual vertex number in the detail
    GA_Offset	getVertexOffset(GA_Size load_index) const
		    {
			if (myVertexMap)
			    return offsetFromIndex(myVertexMap,
						   myVertexOffset + load_index);
			else
			    return GA_Offset(myVertexOffset + load_index);
		    }
    /// Map the load_index to the actual primitive number in the detail
    GA_Offset	getPrimitiveOffset(GA_Size load_index) const
		    {
			if (myPrimitiveMap)
			    return offsetFromIndex(myPrimitiveMap,
					       myPrimitiveOffset + load_index);
			else
			    return GA_Offset(myPrimitiveOffset + load_index);
		    }

    /// Elements are loaded at a fixed offset (usually 0) in the index maps.
    /// This method returns the load offset for a particular owner type.
    GA_Offset	getLoadOffset(GA_AttributeOwner owner) const
		{
		    switch (owner)
		    {
			case GA_ATTRIB_POINT:
			    return myPointOffset;
			case GA_ATTRIB_VERTEX:
			    return myVertexOffset;
			case GA_ATTRIB_PRIMITIVE:
			    return myPrimitiveOffset;
			default:
			    break;
		    }
		    UT_ASSERT(owner == GA_ATTRIB_DETAIL);
		    return GA_Offset(0);
		}
    // Return the offset of the loaded points
    GA_Offset	getPointOffset() const		{ return myPointOffset; }
    // Return the offset of the loaded vertices
    GA_Offset	getVertexOffset() const		{ return myVertexOffset; }
    // Return the offset of the loaded primitives
    GA_Offset	getPrimitiveOffset() const	{ return myPrimitiveOffset; }

    /// This method returns the number of elements being loaded of each type.
    GA_Size	getLoadCount(GA_AttributeOwner owner) const;

    /// Warn about unknown primitive types in the load process
    void	badPrimitive(UT_JSONParser &p, const char *type);

    /// Returns true if this is the time this type of primitive was added to
    /// bad primitive table.
    bool	addBadPrimitive(const char *type);

    /// Convert the given secondary primitive indices into data offsets.
    bool	getSecondaryOffsets(GA_Offset primary_offset,
				const UT_Int64Array &secondary_detail_indices,
				const UT_Int64Array &secondary_prim_indices,
				UT_Int64Array &secondary_prim_offsets) const;


    /// Check the load options.  Return @c defvalue if token isn't defined.
    bool	getDefaultedBool(const char *name, bool defvalue) const;
    /// Check the load options.  Return @c defvalue if token isn't defined.
    exint	getDefaultedInt(const char *name, exint defvalue) const;
    /// Check the load options.  Return @c defvalue if the token isn't defined
    const char	*getDefaultedString(const char *name, UT_String &storage,
				const char *defvalue) const;
    // -------------------------------------------------------------------
    // Methods to query load options.  These methods provide a common
    // interface to querying the GA_LoadOptions which are passed to the load
    // methods.
    /// "string geo:attributeloadmask" @n
    /// Specify the "mask" for attributes which should be loaded.  This
    /// mask is in the form used by UT_String::multiMatch().
    ///
    const char 	*optionAttributeLoadMask(UT_String &storage,
			const char *default_mask="*") const
		{
		    return getDefaultedString(
				"geo:attributeloadmask", storage, default_mask);
		}

    /// "string geo:grouploadmask" @n
    /// Specify the "mask" for groups which should be loaded.  This
    /// mask is in the form used by UT_String::multiMatch()
    const char	*optionGroupLoadMask(UT_String &storage,
			const char *default_mask="*") const
		{
		    return getDefaultedString(
				"geo:grouploadmask", storage, default_mask);
		}

    /// "bool geo:loadprimitives" @n
    /// Load vertex & primitive data.
    ///
    /// Turning this off will perform early termination when loading JSON files
    /// (which may affect reading from streamed data).
    bool	optionLoadPrimitives() const
		{
		    return getDefaultedBool("geo:loadprimitives", true);
		}
    /// "bool geo:loadprimitiveshared" @n
    /// Load primitive shared data.  This is data which can be shared between
    /// multiple primitives and is optionally loaded.
    ///
    /// Turning this off will perform early termination when loading JSON files
    /// (which may affect reading from streamed data).
    bool	optionLoadPrimitiveSharedData() const
		{
		    return getDefaultedBool("geo:loadprimitiveshared", true);
		}


    // End of common load options
    // -------------------------------------------------------------------

    /// When a primitive is being loaded, and it has a reference to shared
    /// data, it needs to call this method to retrieve a handle to the shared
    /// data so it can be loaded upon request.
    GA_SharedDataHandlePtr	needSharedData(const UT_StringHolder &key,
					const UT_StringRef &typekey,
					GA_Primitive* callerPrimitive) const;

    /// This method is called internally to resolve shared data when
    /// there is no index.
    void resolveSharedData(GA_SharedLoadData *data);

    /// Called by the loader to parse the shared load data.
    /// Not used when the geomentry has an index
    bool	forceLoadSharedData(UT_JSONParser &p,
			const GA_PrimitiveFactory &factory);

    /// Retrieves a random primitive that needs the shared data
    /// with the given key.
    GA_Primitive* getRandomPrimitive(const char* key);

    /// Set's the filename to load shared data from
    void 	setFilename(const char* filename);

    /// Loads the index from given stream into this load map
    /// @return true on sucsess false on failure
    bool	loadIndex(UT_IStream* jsonInternalStream, const char* indexPosKey, bool shouldSwap);

    GA_GeometryIndex* getIndex() { return &myIndex; }

private:
    static GA_Offset	 offsetFromIndex(const GA_IndexMap *map, GA_Size index);

    GA_Detail			&myDetail;
    GA_LoadOptions		 myOptions;
    GA_Offset			 myPointOffset;
    GA_Offset			 myVertexOffset;
    GA_Offset			 myPrimitiveOffset;
    const GA_IndexMap		*myPointMap;
    const GA_IndexMap		*myVertexMap;
    const GA_IndexMap		*myPrimitiveMap;
    UT_StringSet		 myBadPrimitives;
    GA_GeometryIndex		 myIndex;
    bool			 myHaveIndex;
    SharedDataHandleMapType	 mySharedDataHandles;
    PrimitiveStorageMapType	 myTemporaryPrimitiveStorage;
    const char*			 myFilename;
};

#endif // GA_LOADMAP_H_
