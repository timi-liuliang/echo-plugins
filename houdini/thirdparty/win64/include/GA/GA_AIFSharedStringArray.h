/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFSharedStringArray.h ( GA Library, C++)
 *
 * COMMENTS:	Attribute Interface class for string index table methods
 */

#ifndef __GA_AIFSharedStringArray__
#define __GA_AIFSharedStringArray__

#include "GA_API.h"

#include "GA_BlobContainer.h"
// Following for the Index Type
#include "GA_AIFSharedStringTuple.h"
#include "GA_Types.h"

#include <UT/UT_Array.h>
#include <UT/UT_StringHolder.h> // needed to avoid -Wattributes on gcc 4.8
#include <UT/UT_VectorTypes.h>

#include <stddef.h>

class GA_Attribute;
class GA_Range;
class UT_StringArray;

/// @brief A specialization of GA_AIFStringArray to access "shared strings"
///
/// This class provides the interface to access string table data.  Each
/// attribute type may provide this interface if it makes sense.
class GA_API GA_AIFSharedStringArray
{
public:
	     GA_AIFSharedStringArray();
    virtual ~GA_AIFSharedStringArray();

    /// Query information about the string storage.
    virtual bool	getStatistics(const GA_Attribute *attrib,
				GA_StringTableStatistics &stats) const = 0;

    /// Compact the string storage
    virtual bool	compactStorage(GA_Attribute *attrib) const = 0;

    /// Extract data from the string table.  This will extract all the unique
    /// strings which are referenced by the attribute.
    /// The string handles are guaranteed to be in ascending order, but
    /// may or may not be contiguous.
    virtual bool	extractStrings(const GA_Attribute *attrib,
				    UT_StringArray &strings,
				    UT_IntArray &handles) const = 0;

    /// Extract all of the unique string handles of the attribute.
    /// The string handles are guaranteed to be in ascending order, but
    /// may or may not be contiguous.
    virtual bool	extractHandles(const GA_Attribute *attrib,
				       UT_IntArray &handles) const = 0;

    /// @{
    /// Return the length of longest array
    virtual exint	findMaximumArrayLength(const GA_Attribute *attrib) const = 0;
    /// @}

public:
    // --------------------------------------------------------------
    // Interface to deal with string handles.
    // --------------------------------------------------------------

    /// Return the number of entries in the shared string table
    GA_Size	getTableEntries(const GA_Attribute *attrib) const
		{
		    GA_StringTableStatistics	stats;
		    if (attrib && getStatistics(attrib, stats))
			return stats.getCapacity();
		    return 0;
		}

    /// It's possible that there are invalid handle indexes mixed with the
    /// valid handles.  When iterating over all the handles, you can call @c
    /// validateTableHandle() which will ensure that there's a string
    /// associated with the given handle.
    virtual GA_StringIndexType	 validateTableHandle(const GA_Attribute *attrib,
					GA_StringIndexType index) const = 0;

    /// Get a string from the string table (without going through an attribute)
    virtual const char		*getTableString(const GA_Attribute *attrib,
					GA_StringIndexType handle) const = 0;

    /// Get the handle (index) corresponding to the given string, returning -1
    /// if none.
    virtual GA_StringIndexType	getTableHandle(const GA_Attribute *attrib,
				       const char *string) const = 0;

    /// GA_StringIndexType indices may not be contiguous, this method allows
    /// you to get a string given an ordered index.  Strings will be defined
    /// for all contiguous strings.  This may be an expensive operation, it's
    /// better to access strings by their index if possible.
    ///
    /// This method will return a NULL pointer past the end of the string table
    virtual const char		*getTableOrderedString(const GA_Attribute *a,
					exint index) const = 0;


    /// Replace a string in the shared string table with a new value.  Warning,
    /// it's possible that the table will "collapse" after the replacement.
    /// For example: @code
    ///	  table := [ "foo", "bar" ]
    ///   table.replaceString(1, "foo")	# Replace "bar" with "foo"
    ///   table := [ "foo" ]
    /// @endcode
    /// In the above example, all elements which originally referenced "bar"
    /// will now reference "foo".  This means that trying to swap two values
    /// will not work as expected.
    virtual bool		replaceTableString(GA_Attribute *attrib,
					GA_StringIndexType handle,
					const char *string) const = 0;

    /// Class to iterate over all the strings in the shared string table
    class GA_API iterator
    {
	public:
	    iterator()
		: myAIF(NULL)
		, myAttrib(NULL)
		, myString(NULL)
		, myCount(0)
		, myIndex(0)
	    {
	    }
	    ~iterator() {}
	    iterator	&operator=(const iterator &src)
			{
			    myAIF = src.myAIF;
			    myAttrib = src.myAttrib;
			    myString = src.myString;
			    myCount = src.myCount;
			    myIndex = src.myIndex;
			    return *this;
			}
	    bool	operator==(const iterator &src)
			{
			    if (!src.myAIF)
				return atEnd();
			    if (!myAIF)
				return src.atEnd();
			    return myAIF == src.myAIF &&
				    myAttrib == src.myAttrib &&
				    myCount == src.myCount &&
				    myIndex == src.myIndex;
			}
	    iterator	&operator++()		{ advance(); return *this; }
	    iterator	&operator++(int)	{ advance(); return *this; }

	    void	rewind()
			{
			    myIndex = 0;
			    setupString();
			}
	    bool	atEnd() const	{ return myIndex >= myCount; }
	    void	advance()
			{
			    myIndex++;
			    setupString();
			}
	    GA_Size		 getCount() const	{ return myCount; }
	    GA_Size		 getIndex() const	{ return myIndex; }
	    GA_StringIndexType	 getHandle() const
				    { return GA_StringIndexType(myIndex); }
	    const char		*getString() const	{ return myString; }
	private:
	    void	setupString()
			{
			    while (myIndex < myCount)
			    {
				myString = myAIF->getTableString(myAttrib,
						GA_StringIndexType(myIndex));
				if (myString)
				    break;
				myIndex++;
			    }
			}
	    iterator(const GA_AIFSharedStringArray *aif,
			const GA_Attribute *a)
		: myAIF(aif)
		, myAttrib(a)
		, myString(NULL)
		, myIndex(0)
		, myCount(aif ? aif->getTableEntries(a) : 0)
	    {
		setupString();
	    }

	    const GA_AIFSharedStringArray	*myAIF;
	    const GA_Attribute			*myAttrib;
	    const char				*myString;
	    GA_Size				 myCount;
	    GA_Size				 myIndex;
	    friend class GA_AIFSharedStringArray;
    };

    iterator	begin(const GA_Attribute *a) const
			{ return iterator(this, a); }
    iterator	end() const
			{ return iterator(); }

protected:
    /// Add (or increment) reference to a string
    virtual GA_StringIndexType	addStringReference(GA_Attribute *attribute,
					const char *string) const = 0;
    /// Decrement reference to a handle
    virtual void		delHandleReference(GA_Attribute *attribute,
					GA_StringIndexType handle) const = 0;

public:
    // --------------------------------------------------------------
    // Array Interface
    // --------------------------------------------------------------

    virtual int		getTupleSize(const GA_Attribute *attrib) const = 0;

    virtual GA_Storage	getStorage(const GA_Attribute *attrib) const = 0;

    /// Get a single string from the array for a single tuple of an element.
    virtual void		 getString(const GA_Attribute *attribute, GA_Offset offset, UT_StringArray &strings) const = 0;
    virtual void		 getString(const GA_Attribute *attribute, GA_Offset offset, UT_Array<UT_StringHolder> &strings) const = 0;

    /// Get the handle from the array for a single tuple of an element
    virtual void		 getStringIndex(const GA_Attribute *attribute, GA_Offset offset, UT_Array<GA_StringIndexType> &indices) const = 0;

    /// Set a single component for a single element.
    virtual void		 setString(GA_Attribute *attribute, GA_Offset offset, const UT_StringArray &strings) const = 0;
    virtual void		 setString(GA_Attribute *attribute, GA_Offset offset, const UT_Array<UT_StringHolder> &strings) const = 0;
    /// Set a single component for a range of elements.
    virtual void		 setStringIndex(GA_Attribute *attribute, GA_Offset offset, const UT_Array<GA_StringIndexType> &indices) const = 0;

    /// Return the size of the array for the GA_Offset
    virtual exint	arraySize(const GA_Attribute *atr, GA_Offset off) const = 0;

    /// Get a UT_StringArray at specific GA_Offset. This is an alias for
    /// getString() so that we can treat this AIF similar to
    /// GA_AIFNumericArray.
    bool get(const GA_Attribute *attrib, GA_Offset o, UT_StringArray &v) const
    {
	getString(attrib, o, v);
	return true;
    }

    /// Set a UT_StringArray at specific GA_Offset. This is an alias for
    /// setString() so that we can treat this AIF similar to
    /// GA_AIFNumericArray.
    bool set(GA_Attribute *attrib, GA_Offset o, const UT_StringArray &v) const
    {
	setString(attrib, o, v);
	return true;
    }

};

#endif
