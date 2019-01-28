/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AttributeFilter.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_AttributeFilter__
#define __GA_AttributeFilter__

#include "GA_API.h"
#include "GA_Types.h"

#include <UT/UT_IntrusivePtr.h>

#include <stddef.h>


class GA_Attribute;


// This enum is used to identify boolean operations on filters
enum GA_AttributeFilterBooleanOp
{
    GA_ATTRIB_FILTER_AND,
    GA_ATTRIB_FILTER_OR,
    GA_ATTRIB_FILTER_XOR,
};

// GA_AttributeFilterImpl is the base class for the implementation of an
// attribute filter.  These objects are reference counted and managed by
// the GA_AttributeFilter (handle) object.
class GA_API GA_AttributeFilterImpl
    : public UT_IntrusiveRefCounter<GA_AttributeFilterImpl>
{
public:
	     GA_AttributeFilterImpl();
    virtual ~GA_AttributeFilterImpl();

    virtual bool	match(const GA_Attribute *atr) const = 0;
    virtual bool	terminateAfterFirstMatch() const { return false; }

protected:

    template <typename T> static bool isType(const GA_Attribute *atr)
			    { return T::isType(atr); }
};

// GA_AttributeFilter is a handle object to a GA_AttributeFilterImpl.  This
// class properly supports return-by-value and pass-by-value semantics.
class GA_API GA_AttributeFilter 
{
public:
		 GA_AttributeFilter()
		 {
		 }
		 GA_AttributeFilter(GA_AttributeFilterBooleanOp op,
				    const GA_AttributeFilter &f0,
				    const GA_AttributeFilter &f1)
		 {
		     switch (op)
		     {
			 case GA_ATTRIB_FILTER_AND:
			     *this = selectAnd(f0, f1);
			     break;
			 case GA_ATTRIB_FILTER_XOR:
			     *this = GA_AttributeFilter::selectXor(f0, f1);
			     break;
			 default:
			     *this = GA_AttributeFilter::selectOr(f0, f1);
			     break;
		     }
		 }

		 // This constructor takes over the current reference to the
		 // supplied implementation to properly support calls of the
		 // form GA_AttributeFilter(new GA_AttributeFilterImpl).
    explicit	 GA_AttributeFilter(GA_AttributeFilterImpl *impl)
		     : myImpl(impl) {}

    bool	match(const GA_Attribute *atr) const
		{ return myImpl ? myImpl->match(atr) : false; }
    bool	terminateAfterFirstMatch() const
		{ return myImpl ? myImpl->terminateAfterFirstMatch() : false; }

    bool    	isValid() const
		{ return myImpl != NULL; }
    //----------------------------------------------
    /// Class factories

    /// selectAll was removed in H16 because it is almost certainly
    /// not what you want or what you think it does.  It also selects
    /// internal and private attributes that can result in looping
    /// over selections crashing.
    /// You should replace your calls with selectPublic().
    // static GA_AttributeFilter	 selectAll();

    // Select "standard" will select standard factory attribute types,
    // excluding groups and topology.  Optionally, a single attribute can be
    // excluded from the filter.
    static GA_AttributeFilter	 selectStandard(const GA_Attribute *exclude=0);

    /// Select "factory" will select all factory (non-HDK-registered) types.
    static GA_AttributeFilter	 selectFactory(const GA_Attribute *exclude=0);

    // These filters will also detect any sub-classes of the given types
    static GA_AttributeFilter	 selectNumeric();	// Subclass of numeric
    static GA_AttributeFilter	 selectTransforming(bool include_P);
    static GA_AttributeFilter	 selectString();	// String attributes
    static GA_AttributeFilter	 selectAlphaNum();	// String || Numeric
    static GA_AttributeFilter	 selectIndexPair();	// Index pair attributes
    static GA_AttributeFilter	 selectBlindData();	// Subclass of blinddata
    static GA_AttributeFilter	 selectGroup();		// Only groups
    static GA_AttributeFilter	 selectTopology();	// Only topological atrs

    /// Select public scope attributes and non-internal groups.
    static GA_AttributeFilter    selectPublic(
					bool include_noninternal_groups = true);

    // Select based on typename (more generic than above specialized filters)
    // This filter will only matches the exact type (not any sub-classes).
    static GA_AttributeFilter	 selectType(const char *type);

    /// Select based on the type info
    static GA_AttributeFilter	 selectTypeInfo(GA_TypeInfo info);

    // Select based on storage type
    static GA_AttributeFilter	 selectTupleByStorage(GA_Storage store);
    static GA_AttributeFilter	 selectFloatTuple(bool include_P=true);
    static GA_AttributeFilter	 selectFloatNumeric(bool include_P=true);

    // Select by name (or pattern)
    static GA_AttributeFilter	 selectByName(const char *name);
    static GA_AttributeFilter	 selectByPattern(const char *pattern);

    // Select based on tuple size, either an exact match or in a range.  The
    // range is inclusive (i.e. selectByTupleRange(3,3) will match only
    // 3-tuples)
    static GA_AttributeFilter	 selectByTupleSize(int size)
					{ return selectByTupleRange(size,size);}
    static GA_AttributeFilter	 selectByTupleRange(int min_range,
					int max_range=1024*1024*1024);

    // Select all except for the specified attribute.  This is equivalent to
    // selectAll() if passed 0.
    static GA_AttributeFilter	 selectAllExcept(const GA_Attribute *except);

    // Group specific filters
    static GA_AttributeFilter	 selectEmptyGroup();
    static GA_AttributeFilter	 selectOrderedGroup();

    // Boolean operations on selections
    static GA_AttributeFilter	 selectNot(const GA_AttributeFilter &f0,
					bool single_match=false);
    static GA_AttributeFilter	 selectAnd(const GA_AttributeFilter &f0,
					const GA_AttributeFilter &f1,
					bool single_match=false);
    static GA_AttributeFilter	 selectOr(const GA_AttributeFilter &f0,
					const GA_AttributeFilter &f1,
					bool single_match=false);
    static GA_AttributeFilter	 selectXor(const GA_AttributeFilter &f0,
					const GA_AttributeFilter &f1,
					bool single_match=false);

private:
    UT_IntrusivePtr<GA_AttributeFilterImpl> myImpl;
};

#endif
