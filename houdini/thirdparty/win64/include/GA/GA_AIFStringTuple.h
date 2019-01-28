/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFStringTuple.h ( GA Library, C++)
 *
 * COMMENTS:	Attribute Interface class for string index table methods
 */

#ifndef __GA_AIFStringTuple__
#define __GA_AIFStringTuple__

#include "GA_API.h"
#include "GA_Types.h"

class GA_Attribute;
class GA_Range;
class UT_StringArray;

/// @brief Generic Attribute Interface class to work with string indices
/// directly, rather than string values.
///
/// This class provides the interface to access string table data.  Each
/// attribute type may provide this interface if it makes sense.
class GA_API GA_AIFStringTuple 
{
public:
	     GA_AIFStringTuple();
    virtual ~GA_AIFStringTuple();

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
				int tuple_index=0) const = 0;

    /// Get the full tuple of string values for a single element
    virtual bool	getStrings(const GA_Attribute *attrib, GA_Offset ai,
				UT_StringArray &strings,
				int count, int start=0) const;

    /// Set a single component for a single element.
    virtual bool	setString(GA_Attribute *attrib, GA_Offset ai,
				const char *string, int tuple_index) const = 0;
    /// Set a single component for a range of elements.
    virtual bool	setString(GA_Attribute *attrib, const GA_Range &ai,
				const char *string, int tuple_index) const;

    /// Set multiple components for a single element.
    virtual bool	setStrings(GA_Attribute *attrib, GA_Offset ai,
				const char **strings, int count,
				int start=0) const;
    /// Set multiple components for a range of elements.
    virtual bool	setStrings(GA_Attribute *attrib, const GA_Range &ai,
				const char **strings, int count,
				int start=0) const;

public:
    // --------------------------------------------------------------
    // Interface to deal with a detail attribute as a list of strings
    //
    // By default interface uses the simple tuple interface defined above.
    // --------------------------------------------------------------

    /// Reserve space in the array.
    virtual bool	arrayReserve(GA_Attribute *attrib, int length) const
			{
			    if (length <= getTupleSize(attrib))
				return true;
			    return setTupleSize(attrib, length);
			}
    /// Truncate the array to the given entries
    virtual bool	arrayTruncate(GA_Attribute *attrib, int length) const
			{
			    if (length >= getTupleSize(attrib))
				return true;
			    return setTupleSize(attrib, length);
			}

    /// Trim the array to the minimum required size.  This may be an expensive
    /// operation as it traverses all elements to compute the minimum size,
    /// then calls truncate.  The method returns the new size.
    virtual int		arrayTrim(GA_Attribute *attrib) const;

    /// Append an index to the array.  The resulting index will be returned.
    /// The @c element_index specifies the point, primitive, vertex etc.
    virtual int		arrayAppendString(GA_Attribute *attrib,
					const char *string,
					GA_Offset element_index=GA_Offset(0)) const;

    /// Append an string to the array, but only if the string is unique in the
    /// array.
    virtual int		arrayAppendUniqueString(GA_Attribute *attrib,
					const char *string,
					GA_Offset element_index=GA_Offset(0)) const;

    /// Return the length of the array.  That is, the number of strings which
    /// occupy slots in the tuple.
    /// The @c element_index specifies the point, primitive, vertex etc.
    virtual int		arrayGetLength(const GA_Attribute *attrib,
					GA_Offset element_index=GA_Offset(0)) const;

    /// Find the maximum length of all elements in the range
    virtual int		arrayGetMaxLength(const GA_Attribute *attrib,
					const GA_Range &range) const;

    /// Extract a string from the array (by index)
    /// The @c element_index specifies the point, primitive, vertex etc.
    virtual const char	*arrayGetString(const GA_Attribute *attrib,
					int string_index,
					GA_Offset element_index=GA_Offset(0)) const;

    /// Extract the list of strings from the array.  Returns the number of
    /// strings extracted.
    /// The @c element_index specifies the point, primitive, vertex etc.
    virtual int		arrayGetStrings(UT_StringArray &strings,
					const GA_Attribute *attrib,
					GA_Offset element_index=GA_Offset(0)) const;

    /// Find the index of a string in the array of strings (-1 if the string
    /// isn't found).
    /// The @c element_index specifies the point, primitive, vertex etc.
    virtual int		arrayFindString(const GA_Attribute *attrib,
					const char *string,
					GA_Offset element_index=GA_Offset(0)) const;

    /// Delete a string from the array (by index)
    /// The @c element_index specifies the point, primitive, vertex etc.
    virtual bool	arrayDestroyString(GA_Attribute *attrib,
					int string_index,
					GA_Offset element_index=GA_Offset(0)) const;

    /// Remove duplicate strings from the array
    /// The @c element_index specifies the point, primitive, vertex etc.
    virtual int		arrayRemoveDuplicates(GA_Attribute *attrib,
					GA_Offset element_index=GA_Offset(0)) const;
};

#endif
