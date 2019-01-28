/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AttributeSet.h ( GA Library, C++)
 *
 * COMMENTS:	A set of attributes.
 */

#pragma once

#ifndef __GA_AttributeSet__
#define __GA_AttributeSet__

#include "GA_API.h"
#include "GA_AttributeProxy.h"
#include "GA_AttributeDict.h"
#include "GA_Defaults.h"
#include "GA_ReuseStrategy.h"
#include "GA_Types.h"

#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>

#include <iosfwd>
#include <stddef.h>


class GA_AIFFileH9;
class GA_ATINumeric;
class GA_ATITopology;
class GA_AttribLoadDataH9;
class GA_AttribSaveDataH9;
class GA_AttribXlateH9;
class GA_Attribute;
class GA_AttributeFilter;
class GA_AttributeType;
class GA_Defragment;
class GA_Detail;
class GA_ElementGroup;
class GA_IndexMap;
class GA_LoadMap;
class GA_MergeMap;
class GA_SaveMap;
class GA_Stat;
template <typename AIF_TYPE> class GA_AIFDefinition;

class UT_IStream;
class UT_JSONParser;
class UT_JSONWriter;
class UT_MemoryCounter;
class UT_Options;
class UT_StringHolder;
class UT_StringRef;
class UT_WorkBuffer;
template <typename T> class UT_Array;


class GA_API GA_AttributeSet
{
private:
    /// Only GA_Detail can create attribute sets because the attributes
    /// rely on the existense of the detail for index maps and otherwise it
    /// may be too easy to create a attributes without a detail.
     friend class	GA_Detail;
     /// Group table requires friendship to be able to reach the
     /// GA_AttributeDict that it is tightly coupled to.
     friend class	GA_GroupTable;
     GA_AttributeSet(GA_Detail &detail);
     GA_AttributeSet(const GA_AttributeSet &set);

public:
    ~GA_AttributeSet();
    
    class namevalidcertificate
    {
	// Used to certify that a name has already been
	// screened for validity 
    public:
	namevalidcertificate() {}
    };

    class namevalidandfreecertificate
    {
	// Used to certify that a name has already been
	// screened for validity and collision.
    public:
	namevalidandfreecertificate() {}
    };

    // --- Attribute Management ----
    /// Create a new named attribute for the @c owner with the given @scope
    ///
    /// The @c creation_options are used to pass intrinsic properties to the
    /// attribute.  The @c creation_options are not generally stored with the
    /// attribute itself, but are used to determine the intrinsic behaviour of
    /// the attribute (i.e. specifying storage types for numeric attributes).
    ///
    /// The @c attribute_options are stored on the attribute.
    ///
    /// The @c attribtype argument is used to look up the attribute type.  Common
    /// values include @c numeric, @c "string" and @c "indexpair"
    GA_Attribute	*createAttribute(GA_AttributeOwner owner,
				GA_AttributeScope scope,
				const UT_StringHolder &name,
				const UT_Options *creation_args,
				const GA_AttributeOptions *attribute_options,
				const UT_StringRef &attribtype,
				const GA_ReuseStrategy &r = GA_ReuseStrategy());
    /// Create a detached attribute.
    GA_Attribute	*createDetachedAttribute(
                                GA_AttributeOwner owner,
				const UT_StringRef &attribtype,
				const UT_Options *creation_args,
				const GA_AttributeOptions *attribute_options) const;
    GA_Attribute	*createDetachedAttribute(
                                GA_AttributeOwner owner,
				const GA_AttributeType &attribtype,
				const UT_Options *creation_args,
				const GA_AttributeOptions *attribute_options) const;

    /// Convenience method to add a public attribute
    SYS_FORCE_INLINE
    GA_Attribute	*createAttribute(GA_AttributeOwner owner,
				const UT_StringHolder &name,
				const UT_Options *creation_args,
				const GA_AttributeOptions *attribute_options,
				const UT_StringRef &attribtype,
				const GA_ReuseStrategy &r = GA_ReuseStrategy())
			    {
				return createAttribute(owner, GA_SCOPE_PUBLIC,
					name, creation_args,
					attribute_options, attribtype);
			    }

    /// Create an attribute that has the same definition as the source.
    /// @note This will fail for group attributes!
    /// @{
    GA_Attribute	*cloneAttribute(GA_AttributeOwner owner,
				const UT_StringHolder &name, namevalidcertificate,
				const GA_Attribute &src,
				bool clone_options,
				GA_DataIdStrategy data_id_strategy
							    = GA_DATA_ID_BUMP,
                                const GA_ReuseStrategy &reuse = GA_ReuseStrategy());
    GA_Attribute	*cloneAttribute(GA_AttributeOwner owner,
				const UT_StringHolder &name,
				const GA_Attribute &src,
				bool clone_options,
				GA_DataIdStrategy data_id_strategy
							    = GA_DATA_ID_BUMP,
                                const GA_ReuseStrategy &reuse = GA_ReuseStrategy());
    /// @}

    /// Create a temporary attribute.
    /// See @c createAttribute() for details on arguments
    GA_Attribute	*createTempAttribute(GA_AttributeOwner owner,
				const UT_Options *creation_args,
				const GA_AttributeOptions *attribute_options,
				const UT_StringRef &attribtype);
    GA_Attribute	*cloneTempAttribute(GA_AttributeOwner owner,
				const GA_Attribute &src,
				bool clone_options);
    const GA_Attribute	*findAttribute(GA_AttributeScope scope,
				const UT_StringRef &name,
				const GA_AttributeOwner search_order[],
				int search_order_size) const;
    SYS_FORCE_INLINE
    GA_Attribute	*findAttribute(GA_AttributeScope scope,
				const UT_StringRef &name,
				const GA_AttributeOwner search_order[],
				int search_order_size)
			     {
				 return const_cast<GA_Attribute *>(
					 static_cast<const GA_AttributeSet *>
					 (this)->findAttribute(scope, name,
						    search_order,
						    search_order_size)); 
			     }
    SYS_FORCE_INLINE
    const GA_Attribute	*findAttribute(const UT_StringRef &name,
				const GA_AttributeOwner search_order[],
				int search_order_size) const
			    {
				return findAttribute(GA_SCOPE_PUBLIC, name,
						     search_order,
						     search_order_size);
			    }
    SYS_FORCE_INLINE
    GA_Attribute	*findAttribute(const UT_StringRef &name,
				const GA_AttributeOwner search_order[],
				int search_order_size)
			     {
				return findAttribute(GA_SCOPE_PUBLIC, name,
						     search_order,
						     search_order_size);
			     }
    SYS_FORCE_INLINE
    GA_Attribute	*findAttribute(GA_AttributeOwner owner,
				GA_AttributeScope scope,
				const UT_StringRef &name)
			     { return findAttribute(scope, name, &owner, 1); }
    SYS_FORCE_INLINE
    GA_Attribute	*findAttribute(GA_AttributeOwner owner,
				const UT_StringRef &name)
			     {
				 return findAttribute(GA_SCOPE_PUBLIC, name,
						      &owner, 1);
			     }
    SYS_FORCE_INLINE
    const GA_Attribute	*findAttribute(GA_AttributeOwner owner,
				GA_AttributeScope scope,
				const UT_StringRef &name) const
			     { return findAttribute(scope, name, &owner, 1); }
    SYS_FORCE_INLINE
    const GA_Attribute	*findAttribute(GA_AttributeOwner owner,
				const UT_StringRef &name) const
			     {
				 return findAttribute(GA_SCOPE_PUBLIC, name,
						      &owner, 1);
			     }
    void		 matchAttributes(const GA_AttributeFilter &filter,
				const GA_AttributeOwner search_order[],
				int search_order_size,
				UT_Array<GA_Attribute *> &list);
    void		 matchAttributes(const GA_AttributeFilter &filter,
				const GA_AttributeOwner search_order[],
				int search_order_size,
				UT_Array<const GA_Attribute *> &list) const;
    void		 matchAttributes(const GA_AttributeFilter &filter,
				GA_AttributeOwner owner,
				UT_Array<GA_Attribute *> &list)
			     { return matchAttributes(filter, &owner,1, list); }
    void		 matchAttributes(const GA_AttributeFilter &filter,
				GA_AttributeOwner owner,
				UT_Array<const GA_Attribute *> &list) const
			     { return matchAttributes(filter, &owner,1, list); }
    bool		 destroyAttribute(GA_Attribute *attribute);
    bool		 destroyAttribute(GA_AttributeOwner owner,
				GA_AttributeScope scope,
				const UT_StringRef &name);
    SYS_FORCE_INLINE
    bool		 destroyAttribute(GA_AttributeOwner owner,
				const UT_StringRef &name)
			    {
				return destroyAttribute(owner, GA_SCOPE_PUBLIC,
							name);
			    }

    bool		 destroyAttributeIncludingP(GA_Attribute *attribute);

    int			 destroyAttributes(GA_AttributeOwner owner,
					  const GA_AttributeFilter &filter);
    bool		 renameAttribute(GA_AttributeOwner owner,
					    GA_AttributeScope scope,
					    const UT_StringRef &fromName,
					    const UT_StringHolder &toName);
    /// Rename an attribute to an unused name.  Typically used to make fromName
    /// available for another attribute.
    ///
    /// Returns new name on success, NULL on failure.
    const char 		*renameAttributeUnspecified(GA_AttributeOwner owner,
					    GA_AttributeScope scope,
					    const UT_StringRef &fromName);

    // Convenience attribute methods
    GA_Attribute	*createTupleAttribute(GA_AttributeOwner owner,
				GA_AttributeScope scope,
				const UT_StringHolder &name,
				GA_Storage storage,
				int tuple_size,
				const GA_Defaults &defaults,
				const UT_Options *creation_args = NULL,
				const GA_AttributeOptions *attribute_options = NULL,
				const GA_ReuseStrategy &r = GA_ReuseStrategy());
    SYS_FORCE_INLINE
    GA_Attribute	*createTupleAttribute(GA_AttributeOwner owner,
				const UT_StringHolder &name,
				GA_Storage storage,
				int tuple_size,
				const GA_Defaults &defaults,
				const UT_Options *creation_args = NULL,
				const GA_AttributeOptions *attribute_options = NULL,
				const GA_ReuseStrategy &r = GA_ReuseStrategy())
    {
	return createTupleAttribute(owner, GA_SCOPE_PUBLIC, name, storage,
				    tuple_size, defaults, creation_args,
				    attribute_options, r);
    }
    GA_ATINumeric       *createDetachedTupleAttribute(
                                GA_AttributeOwner owner,
                                GA_Storage storage,
                                int tuple_size,
                                const GA_Defaults &defaults=GA_Defaults(0.0f),
                                const GA_AttributeOptions *attribute_options=NULL) const;
    GA_Attribute	*createArrayAttribute(GA_AttributeOwner owner,
				GA_AttributeScope scope,
				const UT_StringHolder &name,
				GA_Storage storage,
				int tuple_size,
				const UT_Options *creation_args = NULL,
				const GA_AttributeOptions *attribute_options = NULL,
				const UT_StringRef &attribtype="arraydata",
				const GA_ReuseStrategy &r = GA_ReuseStrategy());
    GA_Attribute	*createTempTupleAttribute(GA_AttributeOwner owner,
				GA_Storage storage,
				int tuple_size,
				const GA_Defaults &defaults,
				const UT_Options *creation_args = NULL,
				const GA_AttributeOptions *attribute_options = NULL);
    GA_Attribute	*createStringAttribute(GA_AttributeOwner owner,
				GA_AttributeScope scope,
				const UT_StringHolder &name,
				int tuple_size=1,
				const UT_Options *creation_args = NULL,
				const GA_AttributeOptions *attribute_options = NULL,
				const UT_StringRef &attribtype="string",
				const GA_ReuseStrategy &r = GA_ReuseStrategy());
    SYS_FORCE_INLINE
    GA_Attribute	*createStringAttribute(GA_AttributeOwner owner,
				const UT_StringHolder &name,
				int tuple_size=1,
				const UT_Options *creation_args = NULL,
				const GA_AttributeOptions *attribute_options = NULL,
				const UT_StringRef &attribtype="string",
				const GA_ReuseStrategy &r = GA_ReuseStrategy())
    {
	return createStringAttribute(owner, GA_SCOPE_PUBLIC, name, tuple_size,
				     creation_args, attribute_options, attribtype,
				     r);
    }
    GA_Attribute	*createTempStringAttribute(GA_AttributeOwner owner,
				int tuple_size=1);
    SYS_FORCE_INLINE
    GA_Attribute	*findPointAttribute(GA_AttributeScope scope,
				const UT_StringRef &name)
			     {
				 return findAttribute(GA_ATTRIB_POINT, scope,
						      name);
			     }
    SYS_FORCE_INLINE
    GA_Attribute	*findPointAttribute(const UT_StringRef &name)
			     {
				 return findPointAttribute(GA_SCOPE_PUBLIC,
							   name);
			     }
    SYS_FORCE_INLINE
    GA_Attribute	*findVertexAttribute(GA_AttributeScope scope,
				const UT_StringRef &name)
			     {
				 return findAttribute(GA_ATTRIB_VERTEX, scope,
						      name);
			     }
    SYS_FORCE_INLINE
    GA_Attribute	*findVertexAttribute(const UT_StringRef &name)
			     {
				 return findVertexAttribute(GA_SCOPE_PUBLIC,
							    name);
			     }
    SYS_FORCE_INLINE
    GA_Attribute	*findPrimitiveAttribute(GA_AttributeScope scope,
				const UT_StringRef &name)
			     {
				 return findAttribute(GA_ATTRIB_PRIMITIVE,
						      scope, name);
			     }
    SYS_FORCE_INLINE
    GA_Attribute	*findPrimitiveAttribute(const UT_StringRef &name)
			     {
				 return findPrimitiveAttribute(GA_SCOPE_PUBLIC,
							       name);
			     }
    SYS_FORCE_INLINE
    GA_Attribute	*findGlobalAttribute(GA_AttributeScope scope,
				const UT_StringRef &name)
			     {
				 return findAttribute(GA_ATTRIB_GLOBAL, scope,
						      name);
			     }
    SYS_FORCE_INLINE
    GA_Attribute	*findGlobalAttribute(const UT_StringRef &name)
			     {
				 return findGlobalAttribute(GA_SCOPE_PUBLIC,
							    name);
			     }

    /// NOTE: These should ONLY be called by the GA_ElementGroup constructor & destructor.
    /// @{
    void registerGroup(GA_ElementGroup &group);
    void unregisterGroup(GA_ElementGroup &group);
    /// @}

    GA_ElementGroup     *findGroup(GA_AttributeOwner owner,
				const UT_StringRef &name);
    // Convenience group methods
    GA_ElementGroup     *findPointGroup(const UT_StringRef &name)
			    { return findGroup(GA_ATTRIB_POINT, name); }
    GA_ElementGroup     *findVertexGroup(const UT_StringRef &name)
			    { return findGroup(GA_ATTRIB_VERTEX, name); }
    GA_ElementGroup     *findPrimitiveGroup(const UT_StringRef &name)
			    { return findGroup(GA_ATTRIB_PRIMITIVE, name); }

    /// Try to compress data pages
    /// Will try to compress *all* pages overlapping the specified offset 
    /// range.
    void tryCompressAllPages(GA_AttributeOwner owner,
			GA_Offset start_offset = GA_Offset(0),
			GA_Offset end_offset = GA_INVALID_OFFSET);

    /// Harden data pages
    /// Will harden *all* pages overlapping the specified offset range.
    /// Once this is done, multiple threads can write to an attribute in
    /// parallel at a finer granularity than pages.
    void hardenAllPages(GA_AttributeOwner owner,
			GA_Offset start_offset = GA_Offset(0),
			GA_Offset end_offset = GA_INVALID_OFFSET);

    /// Bumps all data IDs of attributes of the specified owner.
    void bumpAllDataIds(GA_AttributeOwner owner);

    /// Save attribute sets to a JSON stream.
    /// @section JSON-GA_AttributeSet JSON Schema: GA_AttributeSet
    ///
    /// Storage for a set of attributes.
    ///
    /// @code
    /// {
    ///     "name"          : "GA_AttributeSet",
    ///     "description"   : "Attribute storage",
    ///     "type"          : "orderedmap",
    ///     "properties": {
    ///         "vertexattributes": {
    ///             "type"      : "array",
    ///             "items"     : { "$ref" : "GA_Attribute" } ,
    ///             "description" : "Vertex attributes",
    ///             "optional"  : true,
    ///         }
    ///         "pointattributes": {
    ///             "type"      : "array",
    ///             "items"     : { "$ref" : "GA_Attribute" } ,
    ///             "description" : "Point attributes",
    ///             "optional"  : true,
    ///         }
    ///         "primitiveattributes": {
    ///             "type"      : "array",
    ///             "items"     : { "$ref" : "GA_Attribute" },
    ///             "description" : "Primitive attributes",
    ///             "optional"  : true,
    ///         }
    ///         "globalattributes": {
    ///             "type"      : "array",
    ///             "items"     : { "$ref" : "GA_Attribute" } ,
    ///             "description" : "Global attributes",
    ///             "optional"  : true,
    ///         }
    ///     }
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat, GA_AIFJSON, GA_Attribute
    bool		jsonSave(UT_JSONWriter &w, const GA_SaveMap &s) const;

    /// Load attribute sets from a JSON stream
    bool		jsonLoad(UT_JSONParser &p, GA_LoadMap &m);

    /// Stat attribute stats from a JSON stream
    static bool		jsonStat(UT_JSONParser &p, GA_Stat &sbuf);

    // --- Query methods ----
    int64		getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    void		countMemory(UT_MemoryCounter &counter, bool inclusive) const;


    bool		loadDictH9(UT_IStream &is,
					    GA_AttributeOwner owner,
					    int tbl_size,
					    const GA_AIFDefinition<GA_AIFFileH9>
					    &definition,
					    UT_Array<GA_AttribLoadDataH9>
					    &list,
					    const GA_AttribXlateH9 *xlate);
    static bool		saveDictH9(std::ostream &os, const char *label,
					const UT_Array<GA_AttribSaveDataH9>
					&list);

    /// Get information about attributes
    bool		stat(GA_Stat &stat, uint level) const;

    // Private to GA_AttributeRefMap methods --------------------
    GA_Detail		*getDetail() const	{ return myDetail; }

    // Private to GA_Detail methods ----------------------------------
    void		 clearAllAttributes(bool for_deletion);	// Called on detail deletion
    void		 clearAllAttributes(GA_AttributeOwner owner, bool for_deletion); // Called in replaceWithPoints
    // Reserve some additional space in a symbol table for batch addition.
    void		 reserveNewSymbolTableSpace(GA_AttributeOwner owner,
                                                    GA_AttributeScope scope,
                                                    int size);
    GA_ATITopology	*createTopologyAttribute(GA_AttributeOwner owner,
				const UT_StringHolder &name,
                                GA_AttributeOwner linkowner);

    /// Raw access to the GA_AttributeDict for a particular owner
    SYS_FORCE_INLINE
    const GA_AttributeDict	&getDict(GA_AttributeOwner owner) const
    {
        UT_ASSERT_P(int(owner) >= 0 && int(owner) < int(GA_ATTRIB_OWNER_N));
        return myAttributes[owner];
    }

    /// Query whether a given string is a valid variable name.
    static bool		 isValidName(const char *name,
                                     GA_AttributeScope scope=GA_SCOPE_PUBLIC);

    /// @private Function used in GA_Detail::replace to replace all attributes
    /// If 'skip' is given, we will skip processing any attribute that
    /// matches it. 'skip' will be called with attributes from the
    /// destination as well as src geometries.
    void replace(const GA_AttributeSet &src, GA_AttributeOwner owner,
		 bool replaceindexmap, GA_AttributeFilter *skip);

    /// @private Method used by merging to destroy attributes before merging
    void		 mergeDestroyDestination(GA_MergeMap &map);
    /// @private Method used by merging to add attributes during merging
    void		 mergeAddDestination(GA_MergeMap &map);

    /// @private Method used by merging detail attributes
    void		 mergeDestroyGlobalDestination(GA_MergeMap &map)
			    { mergeDestroyDestination(GA_ATTRIB_DETAIL, map); }
    /// @private Method used by merging detail attributes
    void		 mergeAddGlobalDestination(GA_MergeMap &map)
			    { mergeAddDestination(GA_ATTRIB_DETAIL, map); }

    GA_AttributeDict::iterator	 begin(GA_AttributeOwner owner) const
				 {
				    return myAttributes[owner].begin();
				 }
    GA_AttributeDict::iterator	 begin(GA_AttributeOwner owner,
				       GA_AttributeScope scope) const
				 {
				    return myAttributes[owner].begin(scope);
				 }
    GA_AttributeDict::iterator	 end(GA_AttributeOwner owner) const
				 { return myAttributes[owner].end(); }
    GA_AttributeDict::iterator	 end(GA_AttributeOwner owner,
				     GA_AttributeScope scope) const
				 { return myAttributes[owner].end(scope); }

    /// Called by GA_Detail::defragment when defragmenting a particular
    /// index map.  Topology attributes won't be modified; they're
    /// handled separately.
    /// NOTE: This bumps all data IDs of attributes whose owner is that of the
    ///       index map being defragmented, including topology attributes.
    void		 defragment(const GA_Defragment &defrag);

    /// By default, data IDs are not generated for new attributes to force
    /// callers to explicitly opt-in.  It is very easy for data IDs to
    /// become stale in code that is not explicitly notifying modified
    /// attributes.
    void		 enableDataIdGeneration();

    /// Query whether generation of data IDs has been enabled.
    bool		 isGeneratingDataIds() const
			    { return myGenerateDataIds; }

private:
    /// @private
    /// Used only by GA_Detail
    void setNumBuckets(GA_AttributeOwner owner, GA_AttributeScope scope, size_t nbuckets)
    {
        myAttributes[owner].setNumBuckets(scope, nbuckets);
    }

    void	adjustArraySize(GA_AttributeOwner owner, GA_Offset size);
    /// When a previously deleted element is reallocated, we need to reset the
    /// attribute data to its default values.  The constructElement() assumes
    /// that there are no current values.
    void	constructElementBlock(GA_AttributeOwner owner, GA_Offset offset, GA_Offset nelements);
    /// When an element is destroyed, the destructElement() method is called
    /// so that the attribute data can be cleaned up properly (i.e. string
    /// references or group data).
    void	destructElement(GA_AttributeOwner owner, GA_Offset offset);
    void	batchDeletion(GA_AttributeOwner owner);

    /// @private Method to grow arrays prior to merging
    void	mergeGrowData(GA_AttributeOwner owner, GA_MergeMap &map);

    /// @private Method to copy data from the source to the destination for all
    /// attribute types.
    void	mergeCopyData(const GA_MergeMap &map);

    /// @private Method to copy data from source to dest for detail attributes
    void	mergeCopyGlobalData(const GA_MergeMap &map)
		    { mergeCopyData(GA_ATTRIB_DETAIL, map); }

    /// @private Method to copy data from the source to the destination
    void	mergeCopyData(GA_AttributeOwner owner, const GA_MergeMap &map);

    void		mergeDestroyDestination(GA_AttributeOwner owner,
					    GA_MergeMap &map);
    void		mergeAddDestination(GA_AttributeOwner owner,
					    GA_MergeMap &map);

    const GA_IndexMap	&getIndexMap(GA_AttributeOwner type) const;
    UT_StringRef	 makeUniqueName(GA_AttributeOwner owner,
					GA_AttributeScope scope,
					UT_WorkBuffer &name_buffer) const;
    UT_StringRef	 makeUniqueName(GA_AttributeOwner owner,
					GA_AttributeScope scope,
					const char *prefix,
					UT_WorkBuffer &name_buffer) const;

    /// This method, unlike createAttribute(), merely allocates the requested
    /// attribute.  The allocated attribute is not registered in our tables
    /// nor with our detail.  Consequently name collisions are ignored, and it
    /// is up to the caller to resolve any such collisions before registering
    /// this attribute.
    GA_Attribute	*allocAttribute(GA_AttributeOwner owner,
				const UT_StringHolder &name,
				const UT_Options *creation_args,
				const GA_AttributeOptions *attribute_options,
				const UT_StringRef &attribtype) const;

    inline GA_AttributeProxy *findProxy(GA_AttributeOwner owner,
                                        GA_AttributeScope scope,
					const UT_StringRef &name) const
    {
        UT_ASSERT_P(owner >= GA_AttributeOwner(0) && owner < GA_ATTRIB_OWNER_N);
        return myAttributes[owner].findProxy(scope, name);
    }
    inline GA_Attribute *find(GA_AttributeOwner owner,
                              GA_AttributeScope scope,
			      const UT_StringRef &name) const
    {
        GA_AttributeProxy *p = findProxy(owner, scope, name);
        return p ? p->getAttribute() : NULL;
    }
    void add(GA_AttributeOwner owner, GA_Attribute *atr);
    void invalidateProxyAndDestroyAttribute( GA_AttributeProxy *proxy);
    bool destroy(GA_AttributeOwner owner, GA_Attribute *atr);
    bool destroy(GA_AttributeOwner owner, GA_AttributeScope scope, const UT_StringRef &name)
	    { return destroy(owner, find(owner, scope, name)); }
    int destroy(GA_AttributeOwner owner, const GA_AttributeFilter &filter);
    /// Avoids the safety check on P.
    bool		 destroyIncludingP(GA_AttributeOwner owner, GA_Attribute *atr);
    void		 clear(GA_AttributeOwner owner, bool for_deletion);

    template<typename OP,typename FILL_OPT_OP>
    GA_Attribute *createAttributeImpl(
        OP &creation_op,
        FILL_OPT_OP &fill_opt_op,
        GA_AttributeOwner owner,
        GA_AttributeScope scope,
        const UT_StringHolder &name,
        const UT_Options *create_args,
        const GA_AttributeOptions *attribute_options,
        const GA_AttributeType &attribtype,
        const GA_ReuseStrategy &reuse);

    GA_Detail			*myDetail;
    GA_AttributeDict		 myAttributes[GA_ATTRIB_OWNER_N];
    UT_Array<GA_Attribute *>    *myDestructorCache[GA_ATTRIB_OWNER_N];
    bool			 myGenerateDataIds;
};

#endif
