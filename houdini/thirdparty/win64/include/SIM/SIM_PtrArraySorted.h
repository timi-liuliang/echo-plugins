/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_PtrArraySorted_h__
#define __SIM_PtrArraySorted_h__

#include <UT/UT_ValArray.h>

namespace
{
    static inline int
    comparePointers(void *const* a, void *const* b)
    {
	ptrdiff_t	pa = ptrdiff_t(*a), pb = ptrdiff_t(*b);
	if (pa < pb) return -1;
	if (pa > pb) return 1;
	return 0;
    }
}

SYS_DEPRECATED_PUSH_DISABLE()
template <class T>
class SIM_PtrArraySorted
{
public:
			 SIM_PtrArraySorted()
			     : myComparator(
				typename UT_ValArray<T>::Comparator(
					comparePointers))
			 { }

			 SIM_PtrArraySorted(
			    typename UT_ValArray<T>::Comparator comparator)
			     : myComparator(comparator)
			 { }
    virtual		~SIM_PtrArraySorted()
			 { }

    void		 setComparator(typename
				       UT_ValArray<T>::Comparator cmp)
			 { myComparator = cmp; myArray.sort(myComparator); }
    void		 sort()
			 { myArray.sort(myComparator); }

    exint                size() const
    {
        return myArray.size();
    }
    void                 setSize(exint newsize)
    {
        myArray.setSize(newsize);
    }
    void                 setCapacity(exint newcapacity)
    {
        myArray.setCapacity(newcapacity);
    }
    exint		 entries() const
			 { return size(); }
    void		 entries(int newnumentries)
			 { setSize(newnumentries); }
    void		 resize(int newsize)
			 { setCapacity(newsize); }
    
    T		 	 operator()(int index) const
			 { return myArray(index); }
    T		 	 last() const
			 { return myArray.last(); }
    int			 find(T ptr) const
			 { return myArray.find(ptr, myComparator); }

    bool		 operator==(const SIM_PtrArraySorted &cmp) const
			 {
			     T	 	 ptr, cmpptr;
			     int	 idx, num, diff;

			     if( entries() != cmp.entries() )
				 return false;

			     num = entries();
			     for( idx = 0; idx < num; idx++ )
			     {
				 ptr = myArray(idx);
				 cmpptr = cmp.myArray(idx);
				 diff = myComparator(&ptr, &cmpptr);

				 if( diff != 0 )
				     return false;
			     }

			     return true;
			 }

    bool		 containsAny(const SIM_PtrArraySorted &cmp) const
			 {
			     T	 	 ptr, cmpptr;
			     int	 idx, cmpidx, num, numcmp, diff;

			     numcmp = cmp.entries();
			     num = entries();
			     for( idx = 0, cmpidx = 0;
				  idx < num && cmpidx < numcmp; )
			     {
				 ptr = myArray(idx);
				 cmpptr = cmp.myArray(cmpidx);
				 diff = myComparator(&ptr, &cmpptr);

				 if( diff < 0 )
				 {
				     idx++;
				 }
				 else if( diff > 0 )
				 {
				     cmpidx++;
				 }
				 else
				 {
				     return true;
				 }
			     }

			     return false;
			 }
    bool		 containsFully(const SIM_PtrArraySorted &cmp) const
			 {
			     T		 ptr, cmpptr;
			     int	 idx, cmpidx, num, numcmp, diff;

			     numcmp = cmp.entries();
			     num = entries();
			     for( idx = 0, cmpidx = 0;
				  idx < num && cmpidx < numcmp; )
			     {
				 ptr = myArray(idx);
				 cmpptr = cmp.myArray(cmpidx);
				 diff = myComparator(&ptr, &cmpptr);

				 if( diff < 0 )
				 {
				     idx++;
				 }
				 else if( diff > 0 )
				 {
				     return false;
				 }
				 else
				 {
				     idx++;
				     cmpidx++;
				 }
			     }

			     return (cmpidx >= numcmp) ? true : false;
			 }

    void		 remove(int index)
			 { myArray.removeIndex(index); }
    void		 remove(T ptr)
			 { int pos = find(ptr); if( pos >= 0 ) remove(pos); }
    void		 remove(const SIM_PtrArraySorted &src)
			 {
			     T	 	 srcptr, destptr;
			     int	 idx, srcidx, diff, numsrc;
			     bool	 removed = false;

			     numsrc = src.entries();
			     for( idx = 0, srcidx = 0;
				  srcidx < numsrc && idx < entries(); )
			     {
				 destptr = myArray(idx);
				 srcptr = src.myArray(srcidx);
				 diff = myComparator(&destptr, &srcptr);

				 if( diff < 0 )
				 {
				     idx++;
				 }
				 else
				 {
				     if( diff == 0 )
				     {
					 myArray(idx) = 0;
					 removed = true;
					 idx++;
				     }
				     srcidx++;
				 }
			     }
			     if( removed )
				 myArray.removeZeros();
			 }
    void		 keepOnly(const SIM_PtrArraySorted &src)
			 {
			     T	 	 srcptr, destptr;
			     int	 idx, srcidx, diff, numsrc;
			     bool	 removed = false;

			     numsrc = src.entries();
			     for( idx = 0, srcidx = 0; idx < entries(); )
			     {
				 if( srcidx < numsrc )
				 {
				     destptr = myArray(idx);
				     srcptr = src.myArray(srcidx);
				     diff = myComparator(&destptr, &srcptr);
				 }
				 else
				     diff = -1;

				 if( diff < 0 )
				 {
				     myArray(idx) = 0;
				     removed = true;
				     idx++;
				 }
				 else
				 {
				     if( diff == 0 )
					 idx++;
				     srcidx++;
				 }
			     }
			     if( removed )
				 myArray.removeZeros();
			 }
    
    const SIM_PtrArraySorted &operator=(const SIM_PtrArraySorted &src)
			 { set(src); return *this; }
    int			 add(T ptr)
			 {
			     return myArray.
				    uniqueSortedInsert(ptr, myComparator);
			 }
    void		 set(const SIM_PtrArraySorted &src)
			 {
			     myComparator = src.myComparator;
			     myArray = src.myArray;
			 }
    void		 setUnsorted(const UT_ValArray<T> &src,
				     SIM_PtrArraySorted &duplicates)
			 {
			     T	 	 lastptr, nextptr;
			     int	 idx;

			     myArray = src;
			     sort();
			     for( idx = 1; idx < entries(); idx++ )
			     {
				 lastptr = myArray(idx-1);
				 nextptr = myArray(idx);
				 if( myComparator(&lastptr, &nextptr) == 0 )
				 {
				     duplicates.add(lastptr);
				     myArray(idx-1) = 0;
				 }
			     }
			     if( duplicates.entries() )
				 myArray.removeZeros();
			 }
    void		 merge(const SIM_PtrArraySorted &src)
			 {
			    // shift existing entries
			    int n_src = src.entries();
			    if(!n_src)
				return;
			    int i = entries();
			    int n = i + n_src;
			    resize(n);
			    entries(n);
			    int idx = n;
			    T srcptr = src.myArray(0);
			    for(--i; i >= 0 && myComparator(&myArray(i), &srcptr) > 0; --i)
			    {
				myArray(--idx) = myArray(i);
			    }
			    ++i;
			    // merge items into the hole at [i, i+n_src)
			    int merge_start = i;
			    int src_idx = 0;
			    for(;;)
			    {
				if(idx < n)
				{
				    if(src_idx == n_src)
					break;
				    int cmp = myComparator(&myArray(idx), &src.myArray(src_idx));
				    if(cmp < 0)
					myArray(i++) = myArray(idx++);
				    else
					myArray(i++) = src.myArray(src_idx++);
				    continue;
				}
				while(src_idx < n_src)
				    myArray(i++) = src.myArray(src_idx++);
				break;
			    }
			    // remove duplicates
			    if(!merge_start)
				++merge_start;
			    for(int i = merge_start; i < n; ++i)
			    {
				if(myComparator(&myArray(i), &myArray(i - 1)))
				    myArray(merge_start++) = myArray(i);
			    }
			    entries(merge_start);
			 }
    void SYS_DEPRECATED(13.0) mergeSelected(const SIM_PtrArraySorted &src,
				       bool (*selectfunc)(T))
			 {
			     T	 	 srcptr, destptr;
			     int	 idx, srcidx, diff, numsrc;

			     numsrc = src.entries();
			     for( idx = 0, srcidx = 0; srcidx < numsrc; )
			     {
				 // Just skip over any source pointers that
				 // aren't accepted by the select function.
				 if( selectfunc(src.myArray(srcidx)) )
				 {
				     if( idx < entries() )
				     {
					 destptr = myArray(idx);
					 srcptr = src.myArray(srcidx);
					 diff = myComparator(&destptr, &srcptr);
				     }
				     else
					 diff = 1;
				     if( diff < 0 )
				     {
					 idx++;
				     }
				     else
				     {
					 if( diff > 0 )
					     myArray.insert(src(srcidx), idx);
					 idx++;
					 srcidx++;
				     }
				 }
				 else
				     srcidx++;
			     }
			 }


    // Not that this function can ruin the sort order. Use with caution.
    void			 appendUnsafe(T ptr)
				 { myArray.append(ptr); }
    // This function gets our contained UT_ValArray.
    const UT_ValArray<T>	&getRawArray() const
				 { return myArray; }
     // This function gets our contained UT_ValArray.
    UT_ValArray<T>		&getRawArray()
				 { return myArray; }
    
private:
    typename UT_ValArray<T>::Comparator	 myComparator;
    UT_ValArray<T>			 myArray;
};
SYS_DEPRECATED_POP_DISABLE()


#endif
