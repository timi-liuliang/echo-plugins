/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFTuple.h ( GA Library, C++)
 *
 * COMMENTS:	Tuple Attribute Interface
 */

#ifndef __GA_AIFTuple__
#define __GA_AIFTuple__

#include "GA_API.h"
#include "GA_Types.h"

#include <SYS/SYS_Types.h>
#include <SYS/SYS_TypeTraits.h>

#include <vector>


class GA_Attribute;
class GA_Range;
class GA_Defaults;

/// @brief Generic Attribute Interface class to access an attribute as a
/// tuple.
///
/// This class provides the interface to access attribute data.  Each
/// attribute type may provide this interface if it makes sense.
class GA_API GA_AIFTuple 
{
public:
	     GA_AIFTuple();
    virtual ~GA_AIFTuple();

    /// @{
    /// Query and set some tuple intrisics.
    virtual int		getTupleSize(const GA_Attribute *attrib) const = 0;
    virtual bool	setTupleSize(GA_Attribute *attrib, int size) const = 0;

    virtual GA_Storage	getStorage(const GA_Attribute *attrib) const = 0;
    virtual bool	setStorage(GA_Attribute *attrib,
				   GA_Storage storage) const = 0;
    /// @}

    /// @{
    /// Make the attribute constant
    virtual bool	makeConstant(GA_Attribute *attrib,
				int32 data, int index=0) const = 0;
    virtual bool	makeConstant(GA_Attribute *attrib,
				int64 data, int index=0) const = 0;
    virtual bool	makeConstant(GA_Attribute *attrib,
				fpreal32 data, int index=0) const = 0;
    virtual bool	makeConstant(GA_Attribute *attrib,
				fpreal64 data, int index=0) const = 0;

    virtual bool	makeConstant(GA_Attribute *attrib,
				const int32 *data, int len, int index=0) const = 0;
    virtual bool	makeConstant(GA_Attribute *attrib,
				const int64 *data, int len, int index=0) const = 0;
    virtual bool	makeConstant(GA_Attribute *attrib,
				const fpreal32 *data, int len, int index=0) const = 0;
    virtual bool	makeConstant(GA_Attribute *attrib,
				const fpreal64 *data, int len, int index=0) const = 0;
    /// @}

    /// @{
    /// Get a single value from a single offset in an attribute array.
    virtual bool	get(const GA_Attribute *attrib,
			    GA_Offset ai, int32 &data, int index=0) const = 0;
    /// Get a single value from a single offset in an attribute array.
    virtual bool	get(const GA_Attribute *attrib,
			    GA_Offset ai, int64 &data, int index=0) const = 0;
    /// Get a single value from a single offset in an attribute array.
    virtual bool	get(const GA_Attribute *attrib,
			    GA_Offset ai, fpreal32 &data, int index=0) const=0;
    /// Get a single value from a single offset in an attribute array.
    virtual bool	get(const GA_Attribute *attrib,
			    GA_Offset ai, fpreal64 &data, int index=0) const=0;
    /// @}

    /// @{
    /// Get a tuple of values from a single array element, starting at the
    /// (optionally) specified component.
    virtual bool	get(const GA_Attribute *attrib, GA_Offset ai,
			    int32 *data, int count, int start=0) const = 0;
    /// Get a tuple of values from a single array element
    virtual bool	get(const GA_Attribute *attrib, GA_Offset ai,
			    int64 *data, int count, int start=0) const = 0;
    /// Get a tuple of values from a single array element
    virtual bool	get(const GA_Attribute *attrib, GA_Offset ai,
			    fpreal32 *data, int count, int start=0) const = 0;
    /// Get a tuple of values from a single array element
    virtual bool	get(const GA_Attribute *attrib, GA_Offset ai,
			    fpreal64 *data, int count, int start=0) const = 0;
    /// @}

    // The default methods are not pure virtual and are implemented using the
    // scalar methods above, but subclasses may override them to provide more
    // efficient implementations.
    /// @{
    /// Get an array of values from a selection of elements from the attribute.
    /// The result array must have been preallocated.  If tuple_size is
    /// not positive, the tuple size is assumed to be the attribute's tuple
    /// size.  If explicitly specified, 'tuple_size' values will be copied to
    /// the result array for each element
    virtual bool	getRange(const GA_Attribute *attrib,
			    const GA_Range &range, int32 *result,
			    int start=0, int tuple_size=-1) const
	{ return templatedGetRange(attrib, range, result, start, tuple_size); }
    /// Get an array of values from a selection of elements from the attribute.
    /// The result array must have been preallocated.
    virtual bool	getRange(const GA_Attribute *attrib,
			    const GA_Range &range, int64 *result,
			    int start=0, int tuple_size=-1) const
	{ return templatedGetRange(attrib, range, result, start, tuple_size); }
    /// Get an array of values from a selection of elements from the attribute.
    /// The result array must have been preallocated.
    virtual bool	getRange(const GA_Attribute *attrib,
			    const GA_Range &range, fpreal32 *result,
			    int start=0, int tuple_size=-1) const
	{ return templatedGetRange(attrib, range, result, start, tuple_size); }
    /// Get an array of values from a selection of elements from the attribute.
    /// The result array must have been preallocated.
    virtual bool	getRange(const GA_Attribute *attrib,
			    const GA_Range &range, fpreal64 *result,
			    int start=0, int tuple_size=-1) const
	{ return templatedGetRange(attrib, range, result, start, tuple_size); }

    /// Templated version of getRange() which allows for both arithmetic types
    /// as well as UT_Vector[234] UT_Matrix[234], UT_Quaternion[FD] types.
    /// @{
    template <typename T>
    bool		getRangeTuple(
			    const GA_Attribute *attrib,
			    const GA_Range &range, T *result,
			    int start=0,
			    typename SYS_EnableIf< SYS_IsArithmetic<T>::value >::type *dummy = 0) const
			{
			    // scalar fallback
			    return getRange(attrib, range, result, start, 1);
			}
    template <typename T>
    bool		getRangeTuple(
			    const GA_Attribute *attrib,
			    const GA_Range &range, T *result,
			    int start=0,
			    typename SYS_DisableIf< SYS_IsArithmetic<T>::value >::type *dummy = 0) const
			{
			    typedef typename T::value_type S;
			    return getRange(attrib, range, (S *)result, start, T::tuple_size);
			}
    /// @}

    /// Get an array of values from a selection of elements from the attribute
    /// into a flattened std::vector of the appropriate type.
    virtual bool	getRangeInContainer(const GA_Attribute *attrib,
			    const GA_Range &range,
			    std::vector<int32> &data,
			    int start=0, int tuple_size=-1) const
		{ return templatedContainerGetRange(attrib, range, data, start, tuple_size); }
    /// Get an array of values from a selection of elements from the attribute
    /// into a flattened std::vector of the appropriate type.
    virtual bool	getRangeInContainer(const GA_Attribute *attrib,
			    const GA_Range &range,
			    std::vector<int64> &data,
			    int start=0, int tuple_size=-1) const
		{ return templatedContainerGetRange(attrib, range, data, start, tuple_size); }
    /// Get an array of values from a selection of elements from the attribute
    /// into a flattened std::vector of the appropriate type.
    virtual bool	getRangeInContainer(const GA_Attribute *attrib,
			    const GA_Range &range,
			    std::vector<fpreal32> &data,
			    int start=0, int tuple_size=-1) const
		{ return templatedContainerGetRange(attrib, range, data, start, tuple_size); }
    /// Get an array of values from a selection of elements from the attribute
    /// into a flattened std::vector of the appropriate type.
    virtual bool	getRangeInContainer(const GA_Attribute *attrib,
			    const GA_Range &range,
			    std::vector<fpreal64> &data,
			    int start=0, int tuple_size=-1) const
		{ return templatedContainerGetRange(attrib, range, data, start, tuple_size); }

private:
    template <typename T>
    bool		templatedGetRange(const GA_Attribute *attrib,
			    const GA_Range &range, T *result,
			    int start, int tuple_size) const;
    template <typename T>
    bool		templatedContainerGetRange(const GA_Attribute *attrib,
			    const GA_Range &range, std::vector<T> &data,
			    int start, int tuple_size) const;
public:
    /// @}

    /// @{
    /// Test if any elements are NAN's
    virtual bool	isNan(const GA_Attribute *attrib, GA_Offset ai) const=0;
    virtual bool	isNan(const GA_Attribute *attrib,
				const GA_Range &it) const = 0;
    /// @}

    /// Return the default values for the attribute.
    virtual const GA_Defaults getDefaults(const GA_Attribute *attrib) const = 0;

    // ----------------------------------------------------------------------

    /// @{
    /// Set a component of a single element of the array.
    /// If the component is -1, all elements of the tuple will be set to
    /// the value.
    virtual bool	set(GA_Attribute *attrib, GA_Offset ai,
				int32 data, int comp=-1) const = 0;
    virtual bool	set(GA_Attribute *attrib, GA_Offset ai,
				int64 data, int comp=-1) const = 0;
    virtual bool	set(GA_Attribute *attrib, GA_Offset ai,
				fpreal32 data, int comp=-1) const = 0;
    virtual bool	set(GA_Attribute *attrib, GA_Offset ai,
				fpreal64 data, int comp=-1) const = 0;

    virtual bool	set(GA_Attribute *attrib, const GA_Range &ai,
				int32 data, int comp=-1) const = 0;
    virtual bool	set(GA_Attribute *attrib, const GA_Range &ai,
				int64 data, int comp=-1) const = 0;
    virtual bool	set(GA_Attribute *attrib, const GA_Range &ai,
				fpreal32 data, int comp=-1) const = 0;
    virtual bool	set(GA_Attribute *attrib, const GA_Range &ai,
				fpreal64 data, int comp=-1) const = 0;
    /// @}

    /// @{
    /// Set a single element of the attribute to a tuple of values, starting
    /// at the (optionally) specified component.
    virtual bool	set(GA_Attribute *attrib, GA_Offset ai,
			    const int32 *data, int count,
			    int start = 0) const = 0;
    virtual bool	set(GA_Attribute *attrib, GA_Offset ai,
			    const int64 *data, int count,
			    int start = 0) const = 0;
    virtual bool	set(GA_Attribute *attrib, GA_Offset ai,
			    const fpreal32 *data, int count,
			    int start = 0) const = 0;
    virtual bool	set(GA_Attribute *attrib, GA_Offset ai,
			    const fpreal64 *data, int count,
			    int start = 0) const = 0;

    virtual bool	set(GA_Attribute *attrib, const GA_Range &ai,
			    const int32 *data, int count,
			    int start = 0) const = 0;
    virtual bool	set(GA_Attribute *attrib, const GA_Range &ai,
			    const int64 *data, int count,
			    int start = 0) const = 0;
    virtual bool	set(GA_Attribute *attrib, const GA_Range &ai,
			    const fpreal32 *data, int count,
			    int start = 0) const = 0;
    virtual bool	set(GA_Attribute *attrib, const GA_Range &ai,
			    const fpreal64 *data, int count,
			    int start = 0) const = 0;
    /// @}

    // The default methods are not pure virtual and are implemented using the
    // scalar methods above, but subclasses may override them to provide more
    // efficient implementations.
    /// @{
    /// Set a selection of attributes to elements from an array of values.
    /// If the tuple size is not positive, it is assumed to be the attribute's
    /// tuple size.  If explicitly specified, 'tuple_size' values will be
    /// copied from the array for each element.
    virtual bool	setRange(GA_Attribute *attrib,
			    const GA_Range &it,
			    const int32 *data,
			    int start=0,
			    int tuple_size=-1) const
	    { return templatedSetRange(attrib, it, data, start, tuple_size); }
    virtual bool	setRange(GA_Attribute *attrib,
			    const GA_Range &it,
			    const int64 *data,
			    int start=0,
			    int tuple_size=-1) const
	    { return templatedSetRange(attrib, it, data, start, tuple_size); }
    virtual bool	setRange(GA_Attribute *attrib,
			    const GA_Range &it,
			    const fpreal32 *data,
			    int start=0,
			    int tuple_size=-1) const
	    { return templatedSetRange(attrib, it, data, start, tuple_size); }
    virtual bool	setRange(GA_Attribute *attrib,
			    const GA_Range &it,
			    const fpreal64 *data,
			    int start=0,
			    int tuple_size=-1) const
	    { return templatedSetRange(attrib, it, data, start, tuple_size); }

    /// Templated version of setRange() which allows for both arithmetic types
    /// as well as UT_Vector[234] UT_Matrix[234], UT_Quaternion[FD] types.
    /// @{
    template <typename T>
    bool		setRangeTuple(
			    GA_Attribute *attrib,
			    const GA_Range &range,
			    const T *data,
			    int start=0,
			    typename SYS_EnableIf< SYS_IsArithmetic<T>::value >::type *dummy = 0) const
			{
			    // scalar fallback
			    return setRange(attrib, range, data, start, 1);
			}
    template <typename T>
    bool		setRangeTuple(
			    GA_Attribute *attrib,
			    const GA_Range &range,
			    const T *data,
			    int start=0,
			    typename SYS_DisableIf< SYS_IsArithmetic<T>::value >::type *dummy = 0) const
			{
			    typedef typename T::value_type S;
			    return setRange(attrib, range, (S *)data, start, T::tuple_size);
			}
    /// @}

private:
    template <typename T>
    bool		templatedSetRange(GA_Attribute *a,
			    const GA_Range &range, const T *data,
			    int start, int tuple_size) const;
public:
    /// @}

    // ----------------------------------------------------------------------
    /// Copy values from the source attribute to this attribute
    virtual bool	set(GA_Attribute &dest, GA_Offset di,
			    const GA_Attribute &src, GA_Offset si,
			    int component=-1) const = 0;
    /// Copy values from the source attribute to this attribute
    virtual bool	set(GA_Attribute &dest, const GA_Range &di,
			    const GA_Attribute &src, const GA_Range &si,
			    int component=-1) const = 0;

    /// @{
    /// Homogenize the tuple (i.e. multiply through by the last element of the
    /// tuple).
    virtual bool	homogenize(GA_Attribute &dest, GA_Offset di) const;
    /// @}

    /// @{
    /// Dehomogenize (divide through by the last element of the tuple)
    virtual bool	dehomogenize(GA_Attribute &dest, GA_Offset di) const;
    /// @}
};

#endif
