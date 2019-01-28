/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BRAY_BufferList.h (BRAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __BRAY_BufferList__
#define __BRAY_BufferList__

#include "BRAY_API.h"
#include "BRAY_Buffer.h"
#include <UT/UT_Map.h>

class BRAY_AttribList;
class BRAY_Range;

class BRAY_API BRAY_BufferList
{
    using MapType = UT_Map<UT_StringHolder, BRAY_BufferPtr>;
public:
    BRAY_BufferList();
    ~BRAY_BufferList();

    using iterator = MapType::iterator;
    using const_iterator = MapType::const_iterator;

    /// Allocate buffers for a given list of planes.  Only planes with floating
    /// point storage are created.
    void	create(const BRAY_AttribList &planes,
			int xres, int yres=1);

    /// Resize if needed
    void	resize(int xres, int yres=1);

    /// Clear buffers to their default values
    void	clear();

    /// Return the total tuple size for the buffer list (number of floats for
    /// all channels)
    int		tupleSize() const { return myTupleSize; }

    /// @{
    /// Return resolution
    int		xres() const { return myXres; }
    int		yres() const { return myYres; }
    /// @}

    /// Return the number of buffers
    exint	size() const { return myBufferList.size(); }

    /// May return nullptr
    const BRAY_BufferPtr	&getBuffer(const UT_StringRef &name) const;

    /// May return a nullptr
    const BRAY_BufferPtr	&operator[](const UT_StringRef &name) const
				    { return getBuffer(name); }
    const BRAY_BufferPtr	&operator[](int i) const
				    { return myBufferList[i]; }

    /// Copy from a source buffer list into this buffer list.
    void	copyFrom(const UT_DimRect &dest_rect,
			const BRAY_BufferList &src,
			const UT_DimRect &src_rect);

    /// Copy from a source buffer list into this buffer list.
    void	copyFrom(const BRAY_BufferList &src,
			const BRAY_Range &range);

    /// Add in the source image planes, scaled by value passed int
    void	sumScale(const BRAY_Range &range,
			const BRAY_BufferList &src,
			const UT_Vector3 *scale);

    iterator		begin() { return myBuffers.begin(); }
    iterator		end() { return myBuffers.end(); }
    const_iterator	begin() const { return myBuffers.begin(); }
    const_iterator	end() const { return myBuffers.end(); }

private:
    MapType			myBuffers;
    UT_Array<BRAY_BufferPtr>	myBufferList;
    int				myTupleSize;
    int				myXres;
    int				myYres;
};

#endif

