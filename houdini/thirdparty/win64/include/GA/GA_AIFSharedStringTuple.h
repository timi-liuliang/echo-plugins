/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFSharedStringTuple.h ( GA Library, C++)
 *
 * COMMENTS:	Attribute Interface class for string index table methods
 */

#ifndef __GA_AIFSharedStringTuple__
#define __GA_AIFSharedStringTuple__

#include "GA_API.h"

#include "GA_AIFStringTuple.h"
#include "GA_BlobContainer.h"
#include "GA_Types.h"

#include <UT/UT_Array.h>
#include <UT/UT_VectorTypes.h>

#include <stddef.h>


class GA_Attribute;
class GA_Range;
class UT_StringArray;


/// Index type
/// WARNING: We assume that GA_StringIndexType and GA_BlobIndexType
/// are binare compatible, so do not change this!
typedef GA_BlobIndex	GA_StringIndexType;
#define GA_INVALID_STRING_INDEX		GA_StringIndexType(-1)

class GA_API GA_StringTableStatistics
{
public:
    GA_StringTableStatistics()
	: myCapacity(0)
	, myEntries(0)
    {
    }
    ~GA_StringTableStatistics() {}

    GA_Size	getEntries() const { return myEntries; }
    GA_Size	getCapacity() const { return myCapacity; }

    void	setEntries(GA_Size n) { myEntries = n; }
    void	setCapacity(GA_Size n) { myCapacity = n; }
private:
    GA_Size		myEntries, myCapacity;
};

/// @brief A specialization of GA_AIFStringTuple to access "shared strings"
///
/// This class provides the interface to access string table data.  Each
/// attribute type may provide this interface if it makes sense.
class GA_API GA_AIFSharedStringTuple : public GA_AIFStringTuple 
{
public:
	     GA_AIFSharedStringTuple();
    virtual ~GA_AIFSharedStringTuple();

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
    /// Stop after reaching a maximum
    virtual bool	extractStrings(const GA_Attribute *attrib,
				    UT_StringArray &strings,
				    UT_IntArray &handles,
				    exint maxstrings) const = 0;

    /// Extract all of the unique string handles of the attribute.
    /// The string handles are guaranteed to be in ascending order, but
    /// may or may not be contiguous.
    virtual bool	extractHandles(const GA_Attribute *attrib,
				       UT_IntArray &handles) const = 0;

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
	    iterator(const GA_AIFSharedStringTuple *aif,
			const GA_Attribute *a)
		: myAIF(aif)
		, myAttrib(a)
		, myString(NULL)
		, myIndex(0)
		, myCount(aif ? aif->getTableEntries(a) : 0)
	    {
		setupString();
	    }

	    const GA_AIFSharedStringTuple	*myAIF;
	    const GA_Attribute			*myAttrib;
	    const char				*myString;
	    GA_Size				 myCount;
	    GA_Size				 myIndex;
	    friend class GA_AIFSharedStringTuple;
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
    // Tuple Interface
    // --------------------------------------------------------------

    /// Query the tuple size
    virtual int		getTupleSize(const GA_Attribute *attrib) const = 0;

    /// Set the tuple size
    virtual bool	setTupleSize(GA_Attribute *attrib, int size) const = 0;

    /// Get a single string from the array for a single tuple of an element.
    virtual const char	*getString(const GA_Attribute *attrib, GA_Offset ai,
				int tuple_index=0) const;

    /// Get the handle from the array for a single tuple of an element
    virtual GA_StringIndexType	getHandle(const GA_Attribute *attrib,
					GA_Offset ai,
					int tuple_index=0) const = 0;

    /// Get the full tuple of indices for a single element
    virtual bool	getHandles(const GA_Attribute *attrib, GA_Offset ai,
				GA_StringIndexType *handles,
				int count, int start=0) const;

    /// Set a single component for a single element.
    virtual bool	setString(GA_Attribute *attrib, GA_Offset ai,
				const char *string, int tuple_index) const;
    /// Set a single component for a range of elements.
    virtual bool	setString(GA_Attribute *attrib, const GA_Range &ai,
				const char *string, int tuple_index) const;
    /// Set a single component for a single element.
    virtual bool	setHandle(GA_Attribute *attrib, GA_Offset ai,
				GA_StringIndexType handle,
				int tuple_index) const = 0;
    /// Set a single component for a range of elements.
    virtual bool	setHandle(GA_Attribute *attrib, const GA_Range &ai,
				GA_StringIndexType handle,
				int tuple_index) const;

    /// Set multiple components for a single element
    virtual bool	setHandles(GA_Attribute *attrib, GA_Offset ai,
				const GA_StringIndexType *handles,
				int count, int start=0) const;
    /// Set multiple components for a range of elements
    virtual bool	setHandles(GA_Attribute *attrib, const GA_Range &ai,
				const GA_StringIndexType *handles,
				int count, int start=0) const;

public:
    // --------------------------------------------------------------
    // Interface to deal with a detail attribute as a list of strings
    //
    // By default interface uses the simple tuple interface defined above.
    // --------------------------------------------------------------

    /// @{
    /// Array API, select functions can be more efficiently implemented using
    /// shared strings.
    /// Please see GA_AIFStringTuple for remainder of interface
    virtual int		arrayGetLength(const GA_Attribute *attrib,
					GA_Offset element_index=GA_Offset(0)) const;
    virtual int		arrayFindString(const GA_Attribute *attrib,
					const char *string,
					GA_Offset element_index=GA_Offset(0)) const;
    virtual bool	arrayDestroyString(GA_Attribute *attrib,
					int string_index,
					GA_Offset element_index=GA_Offset(0)) const;
    /// @}

public:
    /// @brief Temporary container to hold references to multiple strings
    ///
    /// In some cases, it's more expedient to add multiple strings to the
    /// attribute, and then assign the string values after the fact.  This
    /// class is similar to the GA_AIFBlob::BlobBuffer class, but is
    /// specialized for shared string attributes.
    class GA_API StringBuffer
    {
    public:
	StringBuffer(GA_Attribute *attribute = NULL,
		    const GA_AIFSharedStringTuple *aif=NULL);
	StringBuffer(const StringBuffer &src)
	    : myAttribute(NULL)
	    , myAIFSharedStringTuple(NULL)
	{
	    *this = src;
	}
	~StringBuffer()
	{
	    clear();
	}

	StringBuffer	&operator=(const StringBuffer &src);

	/// Return number of strings referenced in the string buffer
	GA_Size		entries() const	{ return myHandleList.entries(); }

	/// Add a string to the attribute.  Returns the handle of the string in
	/// the attribute (not the string's index in the buffer).
	GA_StringIndexType	append(const char *string);

	/// Return the string index from the buffer.  The index refers to the
	/// buffer index (not the string handle).
	GA_StringIndexType	 getStringIndex(GA_Size i) const;
	/// Return a string handle from the buffer.  The index refers to the
	/// buffer index (not the string handle).
	const char		*getString(GA_Size i) const;

	/// Clear references to all strings contained in the buffer.
	void			clear();
    private:
	UT_Array<GA_StringIndexType>	 myHandleList;
	const GA_AIFSharedStringTuple	*myAIFSharedStringTuple;
	GA_Attribute			*myAttribute;
    };

    /// @{
    /// Method to add multiple strings in a batch process.  The strings will be
    /// temporarily added to the attribute for as long as the StringBuffer
    /// exists.  When the StringBuffer is deleted, the strings will be deleted
    /// from the attribute unless referenced by elements in the attribute
    /// array.
    ///
    /// Information about the strings (i.e. the handles) can be retriefed from
    /// the StringBuffer.  For example: @code
    ///    UT_StringArray   s(strings);
    ///    GA_AIFSharedStringTuple::StringBuffer handles;
    ///    handles = a->getAIFSharedStringTuple()->addStrings(s);
    ///    for (GA_Size i = 0; i < handles.entries(); ++i)
    ///        cerr << s(i) << " handle=" << handles.getStringIndex(i) << endl;
    /// @endcode
    StringBuffer	addStrings(GA_Attribute *attribute,
				const UT_StringArray &strings) const;
    StringBuffer	addStrings(GA_Attribute *attribute,
				const UT_Array<const char *> &strings) const;
    /// @}
    friend class	StringBuffer;	// Allow the string buffer to add refs
};

#endif
