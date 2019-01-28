/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFCopyData.h ( GA Library, C++)
 *
 * COMMENTS:	Attribute Interface to copy data
 */

#ifndef __GA_AIFCopyData__
#define __GA_AIFCopyData__

#include "GA_API.h"
#include "GA_Types.h"

class GA_Range;
class GA_Attribute;
class GA_AIFTuple;
class GA_AIFStringTuple;

/// @brief Attribute Interface class to copy attribute data
///
/// Copy
class GA_API GA_AIFCopyData 
{
public:
	     GA_AIFCopyData() {}
    virtual ~GA_AIFCopyData();

    /// Copy attribute values for a single element.
    virtual bool	copy(GA_Attribute &d, GA_Offset di,
			    const GA_Attribute &s, GA_Offset si) const = 0;

    /// Copy attribute values for a range of elements.
    /// See the @c parallelCopy() method.
    virtual bool	copy(GA_Attribute &d, const GA_Range &di,
			    const GA_Attribute &s, const GA_Range &si) const=0;

    /// Assign all elements of a range from a single attribute value.
    /// @note The default implementation just calls copy() for each element
    /// in the destination range.
    virtual bool	fill(GA_Attribute &d, const GA_Range &di,
			     const GA_Attribute &s, GA_Offset si) const;

protected:
    /// Convenience method to perform a parallel copy (by calling the
    /// individual copy multiple times).
    bool	parallelCopy(GA_Attribute &d, const GA_Range &di,
			    const GA_Attribute &s, const GA_Range &si) const;

    /// @{
    /// Convenience method to copy data using the AIFTuple interface.
    /// Data is copied using the highest precision floating point format
    bool	tupleCopy(const GA_AIFTuple *tuple,
			    GA_Attribute &d, GA_Offset di,
			    const GA_Attribute &s, GA_Offset si) const;
    bool	tupleCopy(const GA_AIFTuple *tuple,
			    GA_Attribute &d, const GA_Range &di,
			    const GA_Attribute &s, const GA_Range &si) const;
    /// @}

    /// @{
    /// Convenience method to copy data using the AIFStringTuple interface.
    /// Data is copied using the highest precision floating point format
    bool	tupleCopy(const GA_AIFStringTuple *tuple,
			    GA_Attribute &d, GA_Offset di,
			    const GA_Attribute &s, GA_Offset si) const;
    bool	tupleCopy(const GA_AIFStringTuple *tuple,
			    GA_Attribute &d, const GA_Range &di,
			    const GA_Attribute &s, const GA_Range &si) const;
    /// @}
};

#endif
