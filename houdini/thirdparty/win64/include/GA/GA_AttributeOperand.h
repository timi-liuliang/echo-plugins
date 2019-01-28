/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AttributeOperand.h ( GA Library, C++)
 *
 * COMMENTS:	Wrapper class around different styles of operands for
 *		operations on attributes.
 */

#ifndef __GA_AttributeOperand__
#define __GA_AttributeOperand__

#include "GA_API.h"
#include "GA_AttributeProxy.h"
#include "GA_Iterator.h"
#include "GA_Range.h"
#include "GA_Types.h"

#include <SYS/SYS_Types.h>

#include <stddef.h>


class GA_AIFTuple;
class GA_Attribute;
class GA_Range;


/// The GA_AttributeOperand class provides a simple interface to look at data
/// in operations performed on attributes.  Accessing the data may be less
/// efficient than direct access.  But it allows you to have a trade-off
/// between code complexity and efficiency.
class GA_API GA_AttributeOperand 
{
public:
    /// Constant value for all iterations/tuples
    GA_AttributeOperand(fpreal, const GA_Attribute &attrib);

    /// Constant tuple of fpreal32 values for all iterations
    GA_AttributeOperand(const fpreal32 *value, int tuple_size,
			const GA_Attribute &attrib);

    /// Constant tuple of fpreal64 values for all iterations
    GA_AttributeOperand(const fpreal64 *value, int tuple_size,
			const GA_Attribute &attrib);

    /// Writable attribute for a single iteration
    GA_AttributeOperand(GA_Attribute &attrib, GA_Offset offset);

    /// Read-only attribute, single value for all iterations
    GA_AttributeOperand(const GA_Attribute &attrib, GA_Offset offset);

    /// Writable attribute with a selection of offsets based on the iterator
    GA_AttributeOperand(GA_Attribute &attrib, const GA_Range &it);

    /// Read-only attribute with a selection of offsets based on the iterator
    GA_AttributeOperand(const GA_Attribute &attrib, const GA_Range &it);

    /// Destructor
    ~GA_AttributeOperand();

    /// Rewind and start iterating.
    void	rewind();
    /// Test whether the iterations are complete.  This operation will return
    /// false for all operands except.
    ///	   - Operands with full iterators (dependent on iterator state)
    ///    - Writeable attributes with a single offset.  These attributes will
    ///      return true after the first iteration.
    /// @note Read-only attributes that have a single offset will keep
    /// returning the same value and iterate for ever.
    bool	atEnd() const;	// Advance to next element
    /// Advance to the next iteration.
    /// This operation is a no-op for operands which don't have an iterator.
    /// The exception is the writable attribute with a single offset.
    void	advance();

    /// Query the tuple size.  For constant scalar values, the tuplesize will
    /// be 2**31-1.
    int		 getTupleSize() const	{ return myTupleSize; }

    /// @{
    /// Import/Store a value from the given tuple index.  The function will
    /// return false if the operation fails.
    bool	 getI(GA_Offset off, int64 &v, int tuple_idx=0) const;
    bool	 getF(GA_Offset off, fpreal &v, int tple_idx=0) const;
    bool	 setF(GA_Offset off, fpreal v, int tuple_idx=0) const;
    bool	 getI(const GA_Iterator &it, int64 &v, int tuple_idx=0) const
		    { return getI(it.getOffset(), v, tuple_idx); }
    bool	 getF(const GA_Iterator &it, fpreal &v, int tuple_idx=0) const
		    { return getF(it.getOffset(), v, tuple_idx); }
    bool	 setF(const GA_Iterator &it, fpreal v, int tuple_idx=0) const
		    { return setF(it.getOffset(), v, tuple_idx); }
    /// @}

    /// @{
    /// Extract a full tuple.  This will succeed fail if the tuple size
    /// of the attribute is smaller than the requested size.  Otherwise, the
    /// tuple of the attribute may be truncated to the requested size.
    bool	 getF(GA_Offset off, fpreal32 *v, int tuple_size) const;
    bool	 getF(GA_Offset off, fpreal64 *v, int tuple_size) const;
    bool	 getF(const GA_Iterator &it, fpreal32 *v, int tuple_size) const
		    { return getF(it.getOffset(), v, tuple_size); }
    bool	 getF(const GA_Iterator &it, fpreal64 *v, int tuple_size) const
		    { return getF(it.getOffset(), v, tuple_size); }
    /// @}

    /// @{
    /// Store a tuple of values.  The tuple size passed in will be clamped to
    /// the tuple size of the underlying attribute.
    bool	 setF(GA_Offset off, const fpreal32 *v, int tuple_size) const;
    bool	 setF(GA_Offset off, const fpreal64 *v, int tuple_size) const;
    bool	 setF(const GA_Iterator &it, const fpreal32 *v, int size) const
		    { return setF(it.getOffset(), v, size); }
    bool	 setF(const GA_Iterator &it, const fpreal64 *v, int size) const
		    { return setF(it.getOffset(), v, size); }
    /// @}

    /// Raw access to the writable attribute
    GA_Attribute	*getAttribute()		
			 { 
			     return myDestProxy
				 ? myDestProxy->getAttribute() 
				 : NULL;
			 }

    /// Raw access to the read-only attribute
    const GA_Attribute	*getAttribute() const
			 { 
			     return myDestProxy
				 ? myDestProxy->getAttribute()
				 : NULL;
			 }

    /// Query whether the underlying attribute, if any, uses integer storage.
    bool		 getIsIntegral() const;

    /// Test to determine whether the operand is writable
    bool		 getIsWritable() const;

    /// Test to determine whether the operand has a finite selection.  That is,
    /// whether it has a finite iterator (i.e. a constant has an infinite number
    /// of iterations).
    bool		 getIsFinite() const;

    /// Test whether the operand is responsible for iterating.
    bool		 isIterator() const { return myRange != &myRepeater; }

    /// Get raw access to the iterator.  This is used to prevent multiple
    /// iterations of the same iterator.
    const GA_Range	&getRange() const	{ return *myRange; }

private:
#if 1
    GA_AttributeProxyHandle	myDestProxy;
#else
    union {
	const GA_AttributeProxy	*myConst;
	GA_AttributeProxy	*myNonConst;
    } myAttribProxy;
#endif
    GA_Range		 myRepeater;	// Range for non-iterator operands
    const GA_Range	*myRange;
    const GA_AIFTuple	*myTuple;	// To access attributes as floats
    const fpreal32	*myReal32;
    const fpreal64	*myReal64;
    fpreal		 myValue;
    GA_Offset		 myOffset;
    int			 myTupleSize;
    int			 myMode;	// Mode of operation
    bool		 myOffsetIteratorDone;
};

#endif
