/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_GBPointRedirectArray.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_GBPointRedirectArray__
#define __GA_GBPointRedirectArray__

#include "GA_API.h"
#include "GA_Types.h"

#include <UT/UT_Assert.h>
#include <SYS/SYS_Types.h>

#include <map>

SYS_DEPRECATED_PUSH_DISABLE()

/// @brief Class used to create a map of source points during merging
///
/// This class is used only by mirror and should not be used in future code
/// The class stores a map of the points from the source geometry merged to
/// their newly created destination points.  In the map's pair:
///	- first := destination point index
///	- second := source point index
/// Since the map is indexed based on the newly created points (destination
/// points), we use a map (rather than an array).
class GA_API GA_GBPointRedirectArray 
{
public:
    SYS_DEPRECATED_HDK(13.0)
     GA_GBPointRedirectArray(GA_Size size);
    SYS_DEPRECATED_HDK(13.0)
    ~GA_GBPointRedirectArray();

    /// Entries in the array
    SYS_DEPRECATED_HDK(13.0)
    GA_Size	entries() const		{ return myMap.size(); }

    typedef std::map<GA_Index,GA_Index>::const_iterator	const_iterator;

    /// Given the index of a point for the newly created merged point (after
    /// the merge), get the original point's number from the source detail.
    SYS_DEPRECATED_HDK(13.0)
    GA_Index		getSrc(GA_Index created) const
			{
			    const_iterator it;
			    it = myMap.find(GA_Index(created));
			    UT_ASSERT(it != myMap.end());
			    return it->second;
			}

    /// Clear the array and reserve memory for the map
    SYS_DEPRECATED_HDK(13.0)
    void	clear()	{ clearAndResize(); }
    SYS_DEPRECATED_HDK(13.0)
    void	clearAndResize(GA_Size newsize = 0)
		{
		    myMap = std::map<GA_Index, GA_Index>();
		}

    /// @{
    /// Iterator over all the points in the map
    ///  - const_iterator->first == Point number in the merged result (dest)
    ///  - const_iterator->second == Point number in the merge source
    SYS_DEPRECATED_HDK(13.0)
    const_iterator	begin()	{ return myMap.begin(); }
    SYS_DEPRECATED_HDK(13.0)
    const_iterator	end()	{ return myMap.end(); }
    /// @}

private:
    /// Set the map from the source to the created point
    void	setSrc(GA_Index src, GA_Index created) { myMap[created] = src; }

    std::map<GA_Index, GA_Index>	myMap;

    // It's only when we merge that we build the map
    friend class GA_MergeMap;
};

SYS_DEPRECATED_POP_DISABLE()

#endif
