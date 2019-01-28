/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT library (C++)
 *
 */

#ifndef __UT_StringArray_h__
#define __UT_StringArray_h__

#include "UT_API.h"
#include "UT_Array.h"
#include "UT_String.h"
#include "UT_StringHolder.h"
#include "UT_Swap.h"

#include <iosfwd>
#include <string>


class UT_API UT_StringArray : public UT_Array<UT_StringHolder>
{
public:
    using		Parent = UT_Array<UT_StringHolder>;

    // Inherit constructors
    using		Parent::Parent;

			UT_StringArray() {}
	
    explicit		UT_StringArray(const char **src);

    // Allow move/construction from parent type since we're just a wrapper
			UT_StringArray(const Parent &src)
			    : Parent::UT_Array(src) {}

			UT_StringArray(Parent &&src) SYS_NOEXCEPT
			    : Parent::UT_Array(std::move(src)) {}

    // Copy/move constructors are not inherited and need to be specified
			 UT_StringArray(UT_StringArray &&src) SYS_NOEXCEPT
			    = default;

			 UT_StringArray(const UT_StringArray &src) = default;

    UT_StringArray	&operator=(const UT_StringArray &src) = default;
    UT_StringArray	&operator=(UT_StringArray &&src) = default;

    UT_StringArray	&operator+=(const UT_StringArray &src);

    exint		 remove(exint idx);

    exint		 find(const UT_StringRef &str, exint s=0) const
			     { return UT_Array<UT_StringHolder>::find(
				 UTmakeUnsafeRef(str), s); }

    exint		 find(const UT_StringRef &str, Comparator compare) const
			    { return UT_Array<UT_StringHolder>::find(
				 UTmakeUnsafeRef(str), compare); }

    void		 sort(bool forward, bool numbered);

    // We specialize the default case explicitly instead of using default
    // parameters or else someone calling the sort(Comparator) method 
    // can unexpectedly call the wrong sort function if given the wrong
    // comparison function pointer. This way, we can get a compiler error.
    void		 sort() { sort(true, true); }

    // This method allows arbitrary sorting of the array.
    void		 sort(Comparator compare)
			     { UT_Array<UT_StringHolder>::sort(compare); }

    template <typename Compare>
    void	    	 sort(Compare is_less) { stdsort(is_less); }

    exint		 sortedInsert(const char *str,
			     bool forward = true,
			     bool numbered = true);

    template <typename Compare>
    void		 sortedInsert(const char *str, Compare is_less)
			 { 
			    UT_StringHolder entry(str);
			    UT_Array<UT_StringHolder>::sortedInsert(
				entry, is_less);
			 }

    exint		 sortedFind(const char *str,
			     bool forward = true,
			     bool numbered = true) const;

    exint		 getLongestPrefixLength() const;

    // Join the string array elements together, using the passed string
    // as a separator between elements, and return the result.
    void		 join(const char *sep, UT_String &result) const;

    // Join the string array elements together, using the first passed string
    // as a separator between all elements except the last pair, in which case
    // the second separator string is used, and return the result.
    // (This can be used to create English join strings 'foo, bar and baz')
    void		 join(const char *sep, const char *sep_last,
			     UT_String &result) const;

    // Compute a hash value from the hash for each string in the array.
    // The hash value is order-independent - that is {"Cf", "Of"} will hash
    // to the same value as {"Of", "Cf"}.
    unsigned		 hash() const;

    template <typename I>
    class IndexedCompare
    {
    public:
        IndexedCompare(const UT_StringArray &values)
            : myValues(values)
        {}
        bool operator()(I a, I b) const
        { return ::strcmp(myValues(a), myValues(b)) < 0; }
    private:
        const UT_StringArray &myValues;
    };

    template <typename I>
    void        stableSortIndices(UT_Array<I> &indices) const
    {
        IndexedCompare<I> compare(*this);
        std::stable_sort(indices.getArray(),
                         indices.getArray() + indices.size(), compare);
    }
};

#endif // __UT_StringArray_h__
