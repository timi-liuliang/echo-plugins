/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_DeepReader.h ( TIL Library, C++)
 *
 * COMMENTS:	A deep reader is in an interface to writing a deep raster.
 *		The user may write arbitrary tags to the image.  However, there
 *		are a couple of well defined tags which should be handled:
 *			flipbook	- It's in flipbook mode
 *			blackpoint(P)	- Set the blackpoint
 *			whitepoint(P)	- Set the whitepoint
 *			edgestreak(P)	- Turn on edge streaking (4 ints)
 *			sourcename	- The name of the image
 *			statusmessage	- A status message
 *			aspect		- Aspect ratio for the image
 *			cropwindow	- Specify the crop window (4 ints)
 *		Attributes marked with (P) are considered either global or
 *		per-plane.
 */

#ifndef __TIL_DeepReader__
#define __TIL_DeepReader__

#include "TIL_API.h"
#include <SYS/SYS_Types.h>

class UT_StringArray;

template<typename T>
class UT_Vector3T;

class TIL_API TIL_DeepReader
{
public:
	     TIL_DeepReader();
    virtual ~TIL_DeepReader();

    virtual int64	 getMemoryUsage(bool inclusive) const = 0;

    // Query methods.
    virtual int		 getXres() const = 0;
    virtual int		 getYres() const = 0;
    virtual int		 getPlaneDataFormat(int plane_index) const = 0;

    /// When no more data will be written to the reader, this method is called.
    virtual bool	 complete();

    /// This assumes that the data is in the "correct" endian-ness.  Please
    /// ensure that this is done (esp. when reading from sockets).
    virtual bool	 writeTile(int plane_index,
				  int x0, int x1, int y0, int y1,
				  void *data) = 0;

    virtual bool         writeSamples(
                            int nsamples,
                            int floats_per_sample,
                            const UT_Vector3T<int> *samples,
                            const float *data) = 0;

    /// Call to notify something that the image has updated. This allows you
    /// to uncouple tile writes from updates. This version does nothing.
    virtual void	 updateImage();

    // Per plane attributes.  For an attribute which is global (i.e.
    // cropwindow), set the plane index to -1.
    virtual bool	 setTag(int plane, int tag, const char *val);
    virtual bool	 setTag(int plane, int tag,
				const UT_StringArray &values);
    virtual bool	 setTag(int plane, int tag,
				const int *values, int nvalues);
    virtual bool	 setTag(int plane, int tag,
				const float *values, int nvalues);

protected:
    virtual int		 adjustPlaneIndex( int index ) const;
};

#endif
