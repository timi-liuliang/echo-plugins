/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_ElementGroupOrder.h ( GA Library, C++)
 *
 * COMMENTS:	Element group order information
 */

#ifndef __GA_ElementGroupOrder__
#define __GA_ElementGroupOrder__

#include "GA_API.h"
#include "GA_Types.h"
#include "GA_OffsetList.h"

#include <UT/UT_Array.h>
#include <SYS/SYS_TypeDecorate.h>

class GA_Attribute;
class GA_DataBitArray;
class GA_Defragment;
class GA_LoadMap;
class GA_MergeOffsetMap;
class GA_RangeMemberQuery;
class GA_SaveMap;
class GA_SecondaryLookupInfo;
class UT_JSONParser;
class UT_JSONWriter;

#if defined(GA_STRICT_TYPES)
#include <SYS/SYS_TypeTraits.h>

class GA_API_TMPL GA_ElementGroupOrderIndexTag {};
typedef GA_OrdinalType<GA_ElementGroupOrderIndexTag, GA_Size>
						    GA_ElementGroupOrderIndex;
SYS_DECLARE_IS_POD(GA_ElementGroupOrderIndex)
#else
typedef GA_Size	GA_ElementGroupOrderIndex;
#endif

class GA_API GA_ElementGroupOrder 
{
public:
     GA_ElementGroupOrder(GA_Attribute &attrib);
     GA_ElementGroupOrder(GA_Attribute &attrib,
			  const GA_ElementGroupOrder &src);
    ~GA_ElementGroupOrder();

    /// Return the total number of entries, mixed or not.
    GA_Size	entries() const;

    /// Return the number of un-mixed entries.
    /// ie. those indices available to getElement()
    GA_Size	elementEntries() const
		    { return (entries() - mixedEntries()); }

    /// Return the number of mixed entries.
    /// ie. those indices available to getMixed()
    GA_Size	mixedEntries() const;

    /// Will return -1 if the i'th entry is a mixed entry.
    GA_Offset	operator()(GA_ElementGroupOrderIndex i) const	{ return getElement(i); }
    /// Will return -1 if the i'th entry is a mixed entry.
    GA_Offset	getElement(GA_ElementGroupOrderIndex i) const;

    /// Query the i'th entry as a mixed entry.  Secondary is set to NULL when
    /// not mixed.
    void	getMixed(GA_ElementGroupOrderIndex i,
			 GA_Offset &element,
			 const GA_SecondaryLookupInfo *&secondary) const;

    void	clear();

    void	swapRange(GA_Offset a, GA_Offset b, GA_Size n);
    void	moveRange(GA_Offset a, GA_Offset b, GA_Size n);
    void	defragment(const GA_Defragment &defrag);

    void	append(GA_Offset element);
    bool	remove(GA_Offset element);

    void	appendMix(GA_Offset element,
			  const GA_SecondaryLookupInfo *secondary);
    bool	removeMix(GA_Offset element,
			  const GA_SecondaryLookupInfo *secondary);
    int		removeAll(GA_Offset element);
    int		removeAll(const GA_RangeMemberQuery &range_query);
    int		removeAllMix(GA_Offset element,
			     const GA_SecondaryLookupInfo *secondary_prefix,
			     const GA_RangeMemberQuery &secondary_range_query);

    bool	removeIndex(GA_ElementGroupOrderIndex i);

    /// Return the index of the item passed in (or -1).  This is O(N) which
    /// easily leads to O(N*N) algorithms and should be avoided at all costs.
    GA_ElementGroupOrderIndex	findOffset(GA_Offset item) const;

    /// Return the index of the mixed entry passed in (or -1).  This is O(N)
    /// which easily leads to O(N*N) algorithms and should be avoided at all
    /// costs.
    GA_ElementGroupOrderIndex	findMix(GA_Offset element,
					const GA_SecondaryLookupInfo *s) const;

    /// Append any elements in src that are being merged, in order.
    void	merge(const GA_ElementGroupOrder &src,
		      const GA_MergeOffsetMap &map,
                      const GA_DataBitArray &orig);

    /// Save data to a JSON file for an UNMIXED order.
    /// @section JSON-GA_ElementGroupOrder JSON Schema: GA_ElementGroupOrder
    /// @code
    /// {
    ///     "name"          : "GA_ElementGroupOrder",
    ///     "description"   : "An array of ordered elements",
    ///     "type"          : { "$ref" : "GA_OffsetList" },
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    bool	jsonSave(UT_JSONWriter &w, const GA_SaveMap &save) const;

    /// Load the element list from the JSON stream.
    bool	jsonLoad(UT_JSONParser &p, const GA_LoadMap &load);

    /// Save data to a JSON file for a MIXED order.
    /// @section JSON-GA_ElementGroupOrder JSON Schema: GA_ElementGroupOrder
    /// @code
    /// {
    ///     "name"          : "GA_ElementGroupOrder",
    ///     "description"   : "An array of ordered elements",
    ///     "type"          : { "$ref" : "GA_OffsetList" },
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    bool	jsonSaveMix(UT_JSONWriter &w, const GA_SaveMap &save) const;

    /// Load the element list from the JSON stream.
    bool	jsonLoadMix(UT_JSONParser &p, const GA_LoadMap &load);

private:
    GA_Attribute	&myAttribute;
    GA_OffsetListType<GA_ElementGroupOrderIndex>	myElements;
    UT_Array<GA_SecondaryLookupInfo *> mySecondaryElements;

    /// This tracks the number of mixed entries in the group.  A mixed entry
    /// is something like a nurbs surface and its profile curve.
    ///
    /// mySecondaryElements is only relevant if myMixCount > 0.
    GA_Size		 myMixCount;
};

#endif
