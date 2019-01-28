/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_GBPrimitiveList.h ( GA Library, C++)
 *
 * COMMENTS:	Backward compatible class to represent lists of elements
 */

#ifndef __GA_GBPrimitiveList__
#define __GA_GBPrimitiveList__

#include "GA_API.h"
#include "GA_ElementGroupOrder.h"
#include "GA_IndexMap.h"
#include "GA_Types.h"


class GA_ElementGroup;
class GA_IndexMap;
class GA_Primitive;
class GA_SecondaryLookupInfo;

SYS_DEPRECATED_PUSH_DISABLE()

class GA_API GA_GBPrimitiveList 
{
public:
    SYS_DEPRECATED_HDK(13.0)
     GA_GBPrimitiveList(const GA_IndexMap &index_map);
    SYS_DEPRECATED_HDK(13.0)
    ~GA_GBPrimitiveList();

    //----------------------------------------------------------
    //  Methods inherited from UT_ValArray
    //----------------------------------------------------------
    SYS_DEPRECATED_HDK(13.0)
    GA_Size	entries() const	{ return myIndexMap.indexSize(); }

    //----------------------------------------------------------
    //  Methods inherited from GB_PrimitiveList
    //----------------------------------------------------------
    SYS_DEPRECATED_HDK(13.0)
    void		 clearAndDestroy();

    SYS_DEPRECATED_HDK(13.0)
    const GA_Primitive	*rawEntry(GA_Size n) const;
    SYS_DEPRECATED_HDK(13.0)
    GA_Primitive	*rawEntry(GA_Size n);

    SYS_DEPRECATED_HDK(13.0)
    const GA_Primitive	*rawHead() const
			    { return myIndexMap.indexSize() == 0 ? 0:rawEntry(0); }
    SYS_DEPRECATED_HDK(13.0)
    const GA_Primitive	*rawTail() const
			    {
				GA_Size	n = myIndexMap.indexSize();
				return n ? rawEntry(n-1) : 0;
			    }
    SYS_DEPRECATED_HDK(13.0)
    GA_Primitive	*rawHead()
			    { return myIndexMap.indexSize() == 0 ? 0:rawEntry(0); }
    SYS_DEPRECATED_HDK(13.0)
    GA_Primitive	*rawTail()
			    {
				GA_Size	n = myIndexMap.indexSize();
				return n ? rawEntry(n-1) : 0;
			    }

    SYS_DEPRECATED_HDK(13.0)
    const GA_Primitive	*rawHead(const GA_ElementGroup &g) const;
    SYS_DEPRECATED_HDK(13.0)
    const GA_Primitive	*rawTail(const GA_ElementGroup &g) const;
    SYS_DEPRECATED_HDK(13.0)
    GA_Primitive	*rawHead(const GA_ElementGroup &g);
    SYS_DEPRECATED_HDK(13.0)
    GA_Primitive	*rawTail(const GA_ElementGroup &g);

    /// The group overloads of rawNext()/rawPrev() have O(n^2) behavior with
    /// ordered groups.  Use the GroupIterationCache overloads to avoid this.

    const GA_Primitive	*rawNext(const GA_Primitive *i) const;
    const GA_Primitive	*rawNext(const GA_Primitive *i,
				const GA_ElementGroup &g) const;
    GA_Primitive	*rawNext(GA_Primitive *i);
    GA_Primitive	*rawNext(GA_Primitive *i, const GA_ElementGroup &g);

    const GA_Primitive	*rawPrev(const GA_Primitive *i) const;
    const GA_Primitive	*rawPrev(const GA_Primitive *i,
				const GA_ElementGroup &g) const;
    GA_Primitive	*rawPrev(GA_Primitive *i);
    GA_Primitive	*rawPrev(GA_Primitive *i, const GA_ElementGroup &g);

    /// The group overloads of rawNextMix()/rawPrevMix() have O(n^2) behavior
    /// with ordered groups.  Use the GroupIterationCache overloads to avoid
    /// this.

    int		rawHeadMix(const GA_ElementGroup &group,
				const GA_Primitive *&elem,
				const GA_Primitive *&sec) const;
    int		rawTailMix(const GA_ElementGroup &group,
				const GA_Primitive *&elem,
				const GA_Primitive *&sec) const;
    int		rawNextMix(const GA_ElementGroup &group,
				const GA_Primitive *&elem,
				const GA_Primitive *&sec,
				const GA_Primitive *&newelem,
				const GA_Primitive *&newsec) const;
    int		rawPrevMix(const GA_ElementGroup &group,
				const GA_Primitive *&elem,
				const GA_Primitive *&sec,
				const GA_Primitive *&newelem,
				const GA_Primitive *&newsec) const;

    int		rawHeadMix(const GA_ElementGroup &group,
				GA_Primitive *&elem,
				GA_Primitive *&sec);
    int		rawTailMix(const GA_ElementGroup &group,
				GA_Primitive *&elem,
				GA_Primitive *&sec);
    int		rawNextMix(const GA_ElementGroup &group,
				GA_Primitive *&elem,
				GA_Primitive *&sec,
				GA_Primitive *&newelem,
				GA_Primitive *&newsec);
    int		rawPrevMix(const GA_ElementGroup &group,
				GA_Primitive *&elem,
				GA_Primitive *&sec,
				GA_Primitive *&newelem,
				GA_Primitive *&newsec);

    /// Class used to avoid O(n^2) traversal of ordered groups.
    class GA_API GroupIterationCache
    {
	public:
	    GroupIterationCache() : myLastGroupIndex(-1) {}

	private:
	    /// @{
	    /// Not implemented/supported.
	    GroupIterationCache(const GroupIterationCache &);
	    GroupIterationCache	&operator=(const GroupIterationCache &);
	    bool		operator==(const GroupIterationCache &);
	    /// @}
	    //
	    inline bool		isValid(const GA_ElementGroupOrder *,
					const GA_Primitive *) const;
	    inline bool		isValid(const GA_ElementGroupOrder *,
					const GA_Primitive *,
					const GA_Primitive *) const;

	    GA_ElementGroupOrderIndex	myLastGroupIndex;

	    friend class		 GA_GBPrimitiveList;
    };

    const GA_Primitive	*rawHead(const GA_ElementGroup &g,
				 GroupIterationCache &cache) const;
    const GA_Primitive	*rawTail(const GA_ElementGroup &g,
				 GroupIterationCache &cache) const;
    GA_Primitive	*rawHead(const GA_ElementGroup &g,
				 GroupIterationCache &cache);
    GA_Primitive	*rawTail(const GA_ElementGroup &g,
				 GroupIterationCache &cache);

    const GA_Primitive	*rawNext(const GA_Primitive *i,
				 const GA_ElementGroup &g,
				 GroupIterationCache &cache) const;
    GA_Primitive	*rawNext(GA_Primitive *i, const GA_ElementGroup &g,
				 GroupIterationCache &cache);
    const GA_Primitive	*rawPrev(const GA_Primitive *i,
				 const GA_ElementGroup &g,
				 GroupIterationCache &cache) const;
    GA_Primitive	*rawPrev(GA_Primitive *i, const GA_ElementGroup &g,
				 GroupIterationCache &cache);

    int		rawHeadMix(const GA_ElementGroup &group,
				const GA_Primitive *&elem,
				const GA_Primitive *&sec,
				GroupIterationCache &cache) const;
    int		rawTailMix(const GA_ElementGroup &group,
				const GA_Primitive *&elem,
				const GA_Primitive *&seci,
				GroupIterationCache &cache) const;
    int		rawNextMix(const GA_ElementGroup &group,
				const GA_Primitive *&elem,
				const GA_Primitive *&sec,
				const GA_Primitive *&newelem,
				const GA_Primitive *&newsec,
				GroupIterationCache &cache) const;
    int		rawPrevMix(const GA_ElementGroup &group,
				const GA_Primitive *&elem,
				const GA_Primitive *&sec,
				const GA_Primitive *&newelem,
				const GA_Primitive *&newsec,
				GroupIterationCache &cache) const;

    int		rawHeadMix(const GA_ElementGroup &group,
				GA_Primitive *&elem,
				GA_Primitive *&sec,
				GroupIterationCache &cache);
    int		rawTailMix(const GA_ElementGroup &group,
				GA_Primitive *&elem,
				GA_Primitive *&sec,
				GroupIterationCache &cache);
    int		rawNextMix(const GA_ElementGroup &group,
				GA_Primitive *&elem,
				GA_Primitive *&sec,
				GA_Primitive *&newelem,
				GA_Primitive *&newsec,
				GroupIterationCache &cache);
    int		rawPrevMix(const GA_ElementGroup &group,
				GA_Primitive *&elem,
				GA_Primitive *&sec,
				GA_Primitive *&newelem,
				GA_Primitive *&newsec,
				GroupIterationCache &cache);

private:
    const GA_Primitive	*getEntryByOffset(GA_Offset offset) const;
    GA_Primitive	*getEntryByOffset(GA_Offset offset);
    bool		 getMixEntryByOffset(GA_Offset offset,
					     const GA_SecondaryLookupInfo *info,
					     const GA_Primitive *&elem,
					     const GA_Primitive *&sec) const;
    bool		 getMixEntryByOffset(GA_Offset offset,
					     const GA_SecondaryLookupInfo *info,
					     GA_Primitive *&elem,
					     GA_Primitive *&sec);

    const GA_IndexMap	&myIndexMap;
};

/// T must be a sub-class of GA_Primitive
template <typename T>
class GA_GBPrimitiveListT : public GA_GBPrimitiveList
{
public:
    SYS_DEPRECATED_HDK(13.0)
     explicit GA_GBPrimitiveListT(const GA_IndexMap &index_map)
	 : GA_GBPrimitiveList(index_map) {}

    //----------------------------------------------------------
    //  Methods inherited from GEO_PrimList/GD_PrimList
    //----------------------------------------------------------

    SYS_DEPRECATED_HDK(13.0)
    const T	*entry(GA_Size n) const	{ return cast(rawEntry(n)); }
    SYS_DEPRECATED_HDK(13.0)
	  T	*entry(GA_Size n)		{ return cast(rawEntry(n)); }

    SYS_DEPRECATED_HDK(13.0)
    const T	*head() const	{ return cast(rawHead()); }
    SYS_DEPRECATED_HDK(13.0)
          T	*head()		{ return cast(rawHead()); }
    SYS_DEPRECATED_HDK(13.0)
    const T	*tail() const	{ return cast(rawTail()); }
    SYS_DEPRECATED_HDK(13.0)
          T	*tail()		{ return cast(rawTail()); }

    SYS_DEPRECATED_HDK(13.0)
    const T	*head(const GA_ElementGroup &group) const
				{ return cast(rawHead(group)); }
    SYS_DEPRECATED_HDK(13.0)
          T	*head(const GA_ElementGroup &group)
				{ return cast(rawHead(group)); }
    SYS_DEPRECATED_HDK(13.0)
    const T	*tail(const GA_ElementGroup &group) const
				{ return cast(rawTail(group)); }
    SYS_DEPRECATED_HDK(13.0)
          T	*tail(const GA_ElementGroup &group)
				{ return cast(rawTail(group)); }

    SYS_DEPRECATED_HDK(13.0)
    const T	*head(const GA_ElementGroup &group,
		      GroupIterationCache &cache) const
				{ return cast(rawHead(group, cache)); }
    SYS_DEPRECATED_HDK(13.0)
          T	*head(const GA_ElementGroup &group,
		      GroupIterationCache &cache)
				{ return cast(rawHead(group, cache)); }
    SYS_DEPRECATED_HDK(13.0)
    const T	*tail(const GA_ElementGroup &group,
		      GroupIterationCache &cache) const
				{ return cast(rawTail(group, cache)); }
    SYS_DEPRECATED_HDK(13.0)
          T	*tail(const GA_ElementGroup &group,
		      GroupIterationCache &cache)
				{ return cast(rawTail(group, cache)); }

    /// The group overloads of next()/prev() have O(n^2) behavior with ordered
    /// groups.  Use the GroupIterationCache overloads to avoid this.

    SYS_DEPRECATED_HDK(13.0)
    const T	*next(const GA_Primitive *i) const
				{ return cast(rawNext(i)); }
    SYS_DEPRECATED_HDK(13.0)
          T	*next(GA_Primitive *i)
				{ return cast(rawNext(i)); }
    SYS_DEPRECATED_HDK(13.0)
    const T	*next(const GA_Primitive *i, const GA_ElementGroup &g)
			    const { return cast(rawNext(i, g)); }
    SYS_DEPRECATED_HDK(13.0)
          T	*next(GA_Primitive *i, const GA_ElementGroup &g)
				{ return cast(rawNext(i, g)); }

    SYS_DEPRECATED_HDK(13.0)
    const T	*next(const GA_Primitive *i, const GA_ElementGroup &g,
		      GroupIterationCache &cache) const
				{ return cast(rawNext(i, g, cache)); }
    SYS_DEPRECATED_HDK(13.0)
          T	*next(GA_Primitive *i, const GA_ElementGroup &g,
		      GroupIterationCache &cache)
				{ return cast(rawNext(i, g, cache)); }

    SYS_DEPRECATED_HDK(13.0)
    const T	*prev(const GA_Primitive *i) const
				{ return cast(rawPrev(i)); }
    SYS_DEPRECATED_HDK(13.0)
          T	*prev(GA_Primitive *i)
				{ return cast(rawPrev(i)); }
    SYS_DEPRECATED_HDK(13.0)
    const T	*prev(const GA_Primitive *i, const GA_ElementGroup &g)
			    const { return cast(rawPrev(i, g)); }
    SYS_DEPRECATED_HDK(13.0)
          T	*prev(GA_Primitive *i, const GA_ElementGroup &g)
				{ return cast(rawPrev(i, g)); }

    SYS_DEPRECATED_HDK(13.0)
    const T	*prev(const GA_Primitive *i, const GA_ElementGroup &g,
		      GroupIterationCache &cache) const
				{ return cast(rawPrev(i, g, cache)); }
    SYS_DEPRECATED_HDK(13.0)
          T	*prev(GA_Primitive *i, const GA_ElementGroup &g,
		      GroupIterationCache &cache)
				{ return cast(rawPrev(i, g, cache)); }

    SYS_DEPRECATED_HDK(13.0)
    const T	*operator()(GA_Size i) const
				{ return cast(rawEntry(i)); }
    SYS_DEPRECATED_HDK(13.0)
          T	*operator()(GA_Size i)
				{ return cast(rawEntry(i)); }
    SYS_DEPRECATED_HDK(13.0)
    const T	*operator[](GA_Size i) const
				{ return cast(rawEntry(i)); }
    SYS_DEPRECATED_HDK(13.0)
          T	*operator[](GA_Size i)
				{ return cast(rawEntry(i)); }
     
private:
    const T	*cast(const GA_Primitive *e) const
				    { return reinterpret_cast<const T *>(e); }
	  T	*cast(GA_Primitive *e) const
				    { return reinterpret_cast<T *>(e); }
};

/// Both T and SEC_T must be sub-classes of GA_Primitive.  Moreover, given the
/// use of reinterpret_cast, both classes must have the GA_Primitive portion
/// laid out in memory with no offset.  That is, casting either T * or SEC_T *
/// to a GA_Primitive * should not offset the memory address.
template <typename T, typename SEC_T>
class GA_GBPrimitiveListWMixT : public GA_GBPrimitiveListT<T>
{
public:
    SYS_DEPRECATED_HDK(13.0)
     explicit GA_GBPrimitiveListWMixT(const GA_IndexMap &index_map)
	 : GA_GBPrimitiveListT<T>(index_map) {}

    //----------------------------------------------------------
    //  Methods inherited from GEO_PrimList
    //----------------------------------------------------------
    SYS_DEPRECATED_HDK(13.0)
    int		headMix(const GA_ElementGroup &group,
			const T *&elem, const SEC_T *&sec) const
		    { return this->rawHeadMix(group, rcast(elem), sec_rcast(sec)); }
    SYS_DEPRECATED_HDK(13.0)
    int		headMix(const GA_ElementGroup &group, T *&elem, SEC_T *&sec)
		    { return this->rawHeadMix(group, rcast(elem), sec_rcast(sec)); }
    SYS_DEPRECATED_HDK(13.0)
    int		tailMix(const GA_ElementGroup &group,
			const T *&elem, const SEC_T *&sec) const
		    { return this->rawTailMix(group, rcast(elem), sec_rcast(sec)); }
    SYS_DEPRECATED_HDK(13.0)
    int		tailMix(const GA_ElementGroup &group, T *&elem, SEC_T *&sec)
		    { return this->rawTailMix(group, rcast(elem), sec_rcast(sec)); }
    SYS_DEPRECATED_HDK(13.0)
    int		headMix(const GA_ElementGroup &group,
			const T *&elem, const SEC_T *&sec,
			GA_GBPrimitiveList::GroupIterationCache &c) const
		    { return this->rawHeadMix(group, rcast(elem), sec_rcast(sec),c); }
    SYS_DEPRECATED_HDK(13.0)
    int		headMix(const GA_ElementGroup &group, T *&elem, SEC_T *&sec,
			GA_GBPrimitiveList::GroupIterationCache &c)
		    { return this->rawHeadMix(group, rcast(elem), sec_rcast(sec),c); }
    SYS_DEPRECATED_HDK(13.0)
    int		tailMix(const GA_ElementGroup &group,
			const T *&elem, const SEC_T *&sec,
			GA_GBPrimitiveList::GroupIterationCache &c) const
		    { return this->rawTailMix(group, rcast(elem), sec_rcast(sec),c); }
    SYS_DEPRECATED_HDK(13.0)
    int		tailMix(const GA_ElementGroup &group, T *&elem, SEC_T *&sec,
			GA_GBPrimitiveList::GroupIterationCache &c)
		    { return this->rawTailMix(group, rcast(elem), sec_rcast(sec),c); }
    /// The group overloads of nextMix()/prevMix() have O(n^2) behavior with
    /// ordered groups.  Use the GroupIterationCache overloads to avoid this.
    SYS_DEPRECATED_HDK(13.0)
    int		nextMix(const GA_ElementGroup &group,
			const T *&elem, const SEC_T *&sec,
			const T *&newelem, const SEC_T *&newsec) const
		    { return this->rawNextMix(group, rcast(elem), sec_rcast(sec),
					      rcast(newelem), sec_rcast(newsec)); }
    SYS_DEPRECATED_HDK(13.0)
    int		nextMix(const GA_ElementGroup &group,
			T *&elem, SEC_T *&sec, T *&newelem, SEC_T *&newsec)
		    { return this->rawNextMix(group, rcast(elem), sec_rcast(sec),
					      rcast(newelem), sec_rcast(newsec)); }
    SYS_DEPRECATED_HDK(13.0)
    int		prevMix(const GA_ElementGroup &group,
			const T *&elem, const SEC_T *&sec,
			const T *&newelem, const SEC_T *&newsec) const
		    { return this->rawPrevMix(group, rcast(elem), sec_rcast(sec),
					      rcast(newelem), sec_rcast(newsec)); }
    SYS_DEPRECATED_HDK(13.0)
    int		prevMix(const GA_ElementGroup &group,
			T *&elem, SEC_T *&sec, T *&newelem, SEC_T *&newsec)
		    { return this->rawPrevMix(group, rcast(elem), sec_rcast(sec),
					      rcast(newelem), sec_rcast(newsec)); }
    SYS_DEPRECATED_HDK(13.0)
    int		nextMix(const GA_ElementGroup &group,
			const T *&elem, const SEC_T *&sec,
			const T *&newelem, const SEC_T *&newsec,
			GA_GBPrimitiveList::GroupIterationCache &c) const
		    { return this->rawNextMix(group, rcast(elem), sec_rcast(sec),
					      rcast(newelem), sec_rcast(newsec), c); }
    SYS_DEPRECATED_HDK(13.0)
    int		nextMix(const GA_ElementGroup &group,
			T *&elem, SEC_T *&sec, T *&newelem, SEC_T *&newsec,
			GA_GBPrimitiveList::GroupIterationCache &c)
		    { return this->rawNextMix(group, rcast(elem), sec_rcast(sec),
					      rcast(newelem), sec_rcast(newsec), c); }
    SYS_DEPRECATED_HDK(13.0)
    int		prevMix(const GA_ElementGroup &group,
			const T *&elem, const SEC_T *&sec,
			const T *&newelem, const SEC_T *&newsec,
			GA_GBPrimitiveList::GroupIterationCache &c) const
		    { return this->rawPrevMix(group, rcast(elem), sec_rcast(sec),
					      rcast(newelem), sec_rcast(newsec), c); }
    SYS_DEPRECATED_HDK(13.0)
    int		prevMix(const GA_ElementGroup &group,
			T *&elem, SEC_T *&sec, T *&newelem, SEC_T *&newsec,
			GA_GBPrimitiveList::GroupIterationCache &c)
		    { return this->rawPrevMix(group, rcast(elem), sec_rcast(sec),
					      rcast(newelem), sec_rcast(newsec), c); }


private:
    /// @{
    /// Any code instantiating this template class should ensure that the
    /// relative memory alignment of the GA_Primitive derived types allows
    /// the following reinterpret_cast<> usage.
    /// TODO: Rewrite using temporaries and safer static_cast<> uses.
    const GA_Primitive	*&rcast(const T *&e) const
			  { return reinterpret_cast<const GA_Primitive *&>(e); }
	  GA_Primitive	*&rcast(T *&e) const
			  { return reinterpret_cast<GA_Primitive *&>(e); }
    const GA_Primitive	*&sec_rcast(const SEC_T *&e) const
			  { return reinterpret_cast<const GA_Primitive *&>(e); }
	  GA_Primitive	*&sec_rcast(SEC_T *&e) const
			  { return reinterpret_cast<GA_Primitive *&>(e); }
    /// @}
};

SYS_DEPRECATED_POP_DISABLE()

#endif

