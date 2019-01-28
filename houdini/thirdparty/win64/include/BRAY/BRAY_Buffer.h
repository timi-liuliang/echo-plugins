/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BRAY_Buffer.h (BRAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __BRAY_Buffer__
#define __BRAY_Buffer__

#include "BRAY_API.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_Rect.h>
#include <GA/GA_Types.h>

class BRAY_Range;

/// A buffer to store shading results.
class BRAY_API BRAY_Buffer
    : public UT_IntrusiveRefCounter<BRAY_Buffer>
{
public:
     BRAY_Buffer();
     BRAY_Buffer(const UT_StringHolder &name, int xres, int yres, int tsize);
    ~BRAY_Buffer();

    /// Create a buffer
    void	create(const UT_StringHolder &name,
			int xres, int yres, int tuple_sz);

    /// Resize the buffer
    void	resize(int xres, int yres, int tsize);

    /// @{
    /// Access properties
    const UT_StringHolder	&name() const { return myName; }
    int		xres() const { return myXres; }
    int		yres() const { return myYres; }
    int		tupleSize() const { return myTupleSize; }
    /// #}

    /// Clear to a specific value.  If the value passed in is @c nullptr, then
    /// the default value is use.
    void	clear(const fpreal32 *value=nullptr);

    /// Copy from a source buffer into my buffer
    void	copyFrom(const UT_DimRect &dest_rect,
			const BRAY_Buffer &src,
			const UT_DimRect &src_rect);

    /// Copy from a source buffer into my buffer using an indirection array
    /// The array length should match the size of the source buffer and each
    /// entry should map to a pixel in this buffer.
    void	copyFrom(const BRAY_Buffer &src,
			const BRAY_Range &range);

    /// Add in the source buffer, scaled appropriately
    void	sumScale(const BRAY_Range &range,
			const BRAY_Buffer &src,
			const UT_Vector3 *scale);

    /// Add in the source buffer, scaled appropriately
    void	sumScale(const BRAY_Range &range,
			const BRAY_Buffer &src,
			const int *indirect,
			const UT_Vector3 *scale);

    /// @{
    /// Add in a source buffer to the indexed locations scaled by the amount
    /// The @c color is a linear array with at least @c range.size() entries
    /// The @c indirect and @c contrib arrays are indexed by @c range.index(i)
    /// Basically: @code
    ///   for (i : range) {
    ///      int idx = range.index(i);
    ///      vec3()[indirect[idx]] += src[i] * contrib[idx];
    ///   }
    /// @endcode
    void	sumScale(const BRAY_Range &range,
			const UT_Vector3 *src,
			const int *indirect,
			const UT_Vector3 *contrib);
    void	sumScale(const BRAY_Range &range,
			const float *src,
			const int *indirect,
			const UT_Vector3 *contrib);
    /// @}

    /// @{
    /// Access buffer
    fpreal32		*data() { return myData; }
    const fpreal32	*data() const { return myData; }
    /// @}

    /// @{
    /// Cast float data to a particular type
    UT_Vector3		*vec3()
			{
			    UT_ASSERT(tupleSize() == 3);
			    return reinterpret_cast<UT_Vector3 *>(myData);
			}
    const UT_Vector3	*vec3() const
			{
			    UT_ASSERT(tupleSize() == 3);
			    return reinterpret_cast<const UT_Vector3 *>(myData);
			}
    UT_Vector4		*vec4()
			{
			    UT_ASSERT(tupleSize() == 4);
			    return reinterpret_cast<UT_Vector4 *>(myData);
			}
    const UT_Vector4	*vec4() const
			{
			    UT_ASSERT(tupleSize() == 4);
			    return reinterpret_cast<const UT_Vector4 *>(myData);
			}
    /// @}


    /// @{
    /// Access buffer at a given pixel offset
    fpreal32		*pixel(exint x, exint y)
			    { return myData + (y*myXres+x)*myTupleSize; }
    const fpreal32	*pixel(exint x, exint y) const
			    { return myData + (y*myXres+x)*myTupleSize; }
    fpreal32		*pixel(exint off)
			    { return myData + off*myTupleSize; }
    const fpreal32	*pixel(exint off) const
			    { return myData + off*myTupleSize; }
    /// @}

    // Currently, this is hardwired to fpreal32
    GA_Storage	storage() const { return GA_STORE_REAL32; }
private:
    fpreal32		*myData;
    fpreal32		*myDefault;
    UT_StringHolder	 myName;
    int64		 myCapacity;
    int			 myXres;
    int			 myYres;
    int			 myTupleSize;
};

using BRAY_BufferPtr = UT_IntrusivePtr<BRAY_Buffer>;

#endif
