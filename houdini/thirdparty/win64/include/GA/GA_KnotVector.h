/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA Library (C++)
 *
 */

#ifndef __GA_KnotVector_h__
#define __GA_KnotVector_h__

#include "GA_API.h"

#include "GA_Types.h"

#include <UT/UT_Array.h>
#include <UT/UT_ValArray.h>

#include <SYS/SYS_Types.h>


class UT_JSONWriter;
class UT_JSONParser;
class UT_JSONValue;


typedef fpreal64	GA_KnotFloat;

template <typename T>
class GA_KnotVectorT : public UT_ValArray<T>
{
public:
    GA_KnotVectorT(const GA_KnotVectorT &a)
	: UT_ValArray<T>(a)
    {}
    GA_KnotVectorT(unsigned int sz, unsigned int count)
	: UT_ValArray<T>(sz, count)
    {}
    explicit GA_KnotVectorT(unsigned int sz = 0)
	: UT_ValArray<T>(sz)
    {}
    ~GA_KnotVectorT(void);

    /// Save data to a JSON stream.
    /// @section JSON-GA_KnotVector JSON Schema: GA_KnotVector
    /// @code
    /// {
    ///     "name"          : "GA_KnotVector",
    ///     "description"   : "An knot vector array.",
    ///     "type"          : "array",
    ///     "items"         : "number",
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    bool		jsonSave(UT_JSONWriter &w) const;

    /// Save to a JSON value
    bool		jsonSave(UT_JSONValue &w) const;

    /// Load from a JSON stream
    bool		jsonLoad(UT_JSONParser &p);

    /// Load from a JSON value
    bool		jsonLoad(UT_JSONParser &p, const UT_JSONValue &v);


    void		copyMemory(const GA_KnotVectorT &from)
			{
			    *this = from;
			}

    // Move howMany objects starting at index srcIndex to destIndex;
    // Method returns 0 if OK, -1 if overflow.
    int			shift(GA_Size srcIdx, GA_Size destIdx, GA_Size howMany);

    const T		&getValue(GA_Size index) const
			{
			    return UT_Array<T>::operator()(index);
			}
    void		setValue(GA_Size index, fpreal v)
			{
                            if (index >= UT_Array<T>::entries())
                                UT_Array<T>::entries(index + 1);
			    UT_Array<T>::operator()(index) = v;
			}
    void		reserve(int size)
			    { UT_Array<T>::setCapacityIfNeeded(size); }

    GA_Size		insertKnot(GA_Size idx)
			    { return insert(idx); }
    GA_Size		insertKnot(fpreal t, GA_Size i)
			    { return insert(t, i); }
     
private:
    /// @{
    /// The order of arguments for insert() is different for GA_Basis and for
    /// UT_ValArray.  Thus, we make the insert methods private and ask people
    /// to use "insertKnot" instead.
    unsigned int	insert(unsigned index)
			    { return UT_ValArray<T>::insert(index); }
    unsigned int	insert(const T &t, unsigned int i)
			    { return UT_ValArray<T>::insert(t, i); }
    /// @}

};

typedef GA_KnotVectorT<GA_KnotFloat>	GA_KnotVector;

#endif
