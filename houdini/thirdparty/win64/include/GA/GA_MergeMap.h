/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_MergeMap.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_MergeMap__
#define __GA_MergeMap__

#include "GA_API.h"
#include "GA_Types.h"
#include "GA_MergeOptions.h"
#include "GA_MergeOffsetMap.h"
#include "GA_TempBuffer.h"

#include <SYS/SYS_Types.h>

#include <stddef.h>


class GA_Attribute;
class GA_Detail;
class GA_Range;


/// @brief The merge map keeps track of information when merging details
/// 
/// Merging details involves merging
/// - Attributes
/// - Elements
/// This class keeps track of information between the different stages of
/// merging.
///
/// When merging attribute data there are two different modes.  The mode is per
/// attribute type (i.e. point data might be a full merge while primitives
/// might be a compact merge).
/// - <b>full merging</b> will copy the source arrays verbatim.  Certain
/// optimizations may be possible using full merging in that it may be possible
/// to share data during the merge.
/// - <b>compact merging</b> will compact data data and produce a more compact
/// representation of the data.  However, since the data is re-ordered, it may
/// not be possible to share data.
///
/// When merging subsets of the source geometry, compact merging is
/// automatically turned on.
///
/// @see GA_AIFMerge, GA_MergeOptions
///
class GA_API GA_MergeMap 
{
public:
     GA_MergeMap(GA_Detail &dest, const GA_Detail &src, GA_MergeOptions &opts);
    ~GA_MergeMap();

    /// Clear for multiple iterations
    void		clear();

    /// @{
    /// When performing multiple merges of a single source geometry, this is
    /// set to the iteration count.
    GA_Size		iterationCount() const		{ return myIteration; }
    void		setIterationCount(GA_Size i)	{ myIteration = i; }
    /// @}

    /// Before the merge map can be used, the source & destination iterators
    /// need to be created.  This may also adjust index maps if they need to be
    /// interleaved.
    void		createOffsetMaps();

    /// Creates a trivial merge map, and doesn't change the destination detail,
    /// because the index maps are assumed to have already been updated.
    void                createTrivialMaps();

    /// Create offset maps for merging of global attributes
    void		createGlobalOffsetMap();

    /// Get the destination geometry
    GA_Detail		&getDestination() const	{ return myDest; }

    /// Get the source geometry
    const GA_Detail	&getSource() const	{ return mySource; }

    /// Get the merging options
    GA_MergeOptions	&getOptions() const	{ return myOptions; }

    /// Store a mapping between a source attribute and a destination attribute.
    /// This is built automatically for during attribute processing during
    /// merging.  The GA_AIFMerge::getDestination() is used to define the map.
    /// @see GA_AIFMerge
    void		 setAttributeMap(const GA_Attribute &src,
					 GA_Attribute &dest);

    /// Retrieve the source attribute mapped to the destination attribute.
    const GA_Attribute	*getSourceAttribute(const GA_Attribute &dest) const;

    /// @{
    /// Get the offset map for a class of elements.
    /// @see GA_MergeOffsetMap
    GA_MergeOffsetMap	&getOffsetMap(GA_AttributeOwner owner) const
			    { return *myOffsetMaps[owner]; }
    /// Convenience method to get iterator over the source elements
    const GA_Range	 &getSourceRange(GA_AttributeOwner owner) const
			    { return myOffsetMaps[owner]->getSourceRange(); }

    /// Convenience method to get iterator over the destination elements.
    ///
    /// @note It is more efficient to use mapDestFromSource() with the source
    /// range, especially when iterating over both.
    const GA_Range	 &getDestRange(GA_AttributeOwner owner) const
			    { return myOffsetMaps[owner]->getDestRange(); }

    /// Convenience method to get old destination size
    GA_Offset		 getDestInitCapacity(GA_AttributeOwner owner) const
			    { return myOffsetMaps[owner]->getDestInitCapacity(); }

    /// Convenience method to get new destination size
    GA_Offset		 getDestCapacity(GA_AttributeOwner owner) const
			    { return myOffsetMaps[owner]->getDestCapacity(); }

    /// @{
    /// Convenience methods to get the destination range where the merged
    /// data is to be placed (MERGE_COPY, MERGE_APPEND). This range is
    /// inclusive [start..end].
    GA_Offset		 getDestStart(GA_AttributeOwner owner) const
			    { return myOffsetMaps[owner]->getDestStart(); }
    GA_Offset		 getDestEnd(GA_AttributeOwner owner) const
			    { return myOffsetMaps[owner]->getDestEnd(); }
    /// @}

    /// Convenience method to get the destination offset from the source offset
    GA_Offset		mapDestFromSource(GA_AttributeOwner own,
						GA_Offset off) const
			    { return myOffsetMaps[own]->mapDestFromSource(off);}

    /// Convenience method to check for an identity offset map.
    bool		isIdentityMap(GA_AttributeOwner own) const
			    { return myOffsetMaps[own]->isIdentityMap(); }

    /// Convenience method to check for a trivial offset map.
    bool		isTrivialMap(GA_AttributeOwner own) const
			    { return myOffsetMaps[own]->isTrivial(); }

    /// Convenience method to check for the dest-source of a trivial offset map.
    GA_Size		getTrivialDiff(GA_AttributeOwner own) const
			    { return myOffsetMaps[own]->getTrivialDiff(); }

    /// Convenience method to get the merge strategy
    /// @see GA_MergeOptions
    GA_MergeOptions::MergeStrategy
			getMergeStrategy(GA_AttributeOwner own) const
			    { return myOffsetMaps[own]->getMergeStrategy(); }

    /// @}

    /// Used to fill out the GA_MergeOptions primitive maps, after the
    /// primitives have been merged.
    void		buildMergeOptionPrimitiveMaps() const;

    class AttributeMap
    {
    public:
	 AttributeMap() {}
	~AttributeMap() {}

	void	append(GA_Attribute &dest, const GA_Attribute &src)
		{
		    myDest.append(&dest);
		    mySrc.append(&src);
		}
	void			 reserve(exint entries)
				 {
				     myDest.reserve(entries);
				     mySrc.reserve(entries);
				 }
	bool			 empty() const { return myDest.entries() == 0; }
	exint			 entries() const { return myDest.entries(); }
	GA_Attribute		*getDest(exint i) const { return myDest(i); }
	const GA_Attribute	*getSrc(exint i) const { return mySrc(i); }
	void			 clear()
				 {
				     myDest.clear();
				     mySrc.clear();
				 }

	class const_iterator
	{
	public:
	    const_iterator()
		: myI(0)
		, myEnd(0)
		, myMap(NULL)
	    {
	    }
	    const_iterator(const const_iterator &src)
		: myI(src.myI)
		, myEnd(src.myEnd)
		, myMap(src.myMap)
	    {
	    }
	    const_iterator	&operator=(const const_iterator &src)
				 {
				     myI = src.myI;
				     myEnd = src.myEnd;
				     myMap = src.myMap;
				     return *this;
				 }
	    bool	operator==(const const_iterator &src) const
			{
			    return myI == src.myI &&
				    myEnd == src.myEnd &&
				    myMap == src.myMap;
			}
	    bool	operator!=(const const_iterator &src) const
			    { return !(*this == src); }
	    exint	operator*() const	{ return myI; }
	    bool	atEnd() const	{ return myI == myEnd; }
	    void	advance()	{ ++myI; }
	    void	rewind()	{ myI = 0; }
	    const_iterator	&operator++() { advance(); return *this; }

	    GA_Attribute	*getDest() const { return myMap->getDest(myI); }
	    const GA_Attribute	*getSrc() const	{ return myMap->getSrc(myI); }

	private:
	    const_iterator(const AttributeMap *map, exint where)
		: myI(where)
		, myEnd(map->entries())
		, myMap(map)
	    {
	    }
	    const AttributeMap	*myMap;
	    exint		 myI, myEnd;
	    friend class AttributeMap;
	};

	const_iterator	begin() const { return const_iterator(this, 0); }
	const_iterator	end() const { return const_iterator(this, entries()); }

    private:
	GA_TempBuffer<GA_Attribute *>		myDest;
	GA_TempBuffer<const GA_Attribute *>	mySrc;
    };

    /// Access the attribute map that maps between the destination and source
    /// attributes.
    /// The key is the destination attribute, the value is the source
    /// attribute.  All values are stored as pointers.
    const AttributeMap	&getAttributeMap() const
			{
			    return myAttributeMap;
			}
private:
    /// Used to fill out the GA_MergeOptions point maps
    void		buildMergeOptionPointMaps() const;

    GA_MergeOptions	&myOptions;
    GA_Detail		&myDest;
    const GA_Detail	&mySource;
    GA_Size		 myIteration;
    AttributeMap	 myAttributeMap;
    GA_MergeOffsetMap	*myOffsetMaps[4];
};

#endif
