/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_DeepRasterReader.h ( TIL Library, C++)
 *
 * COMMENTS:	A deep reader is in an interface to writing a deep raster.
 */

#ifndef __TIL_DeepRasterReader__
#define __TIL_DeepRasterReader__

#include "TIL_DeepReader.h"
#include "TIL_Defines.h"
#include "TIL_Sequence.h"
#include <UT/UT_StringHolder.h>

class TIL_DeepRaster;
class UT_NetSocket;

template<typename T>
class UT_Vector3T;

class TIL_API TIL_DeepRasterReader : public TIL_DeepReader
{
public:
	     TIL_DeepRasterReader();
    virtual ~TIL_DeepRasterReader();

    virtual int64 getMemoryUsage(bool inclusive) const;

    /// Initialize sequence information from the net socket.
    bool	 loadSequenceInfo(UT_NetSocket *sock,
				  int &pid, bool &new_sequence);

    /// addPlane() returns the plane index (which can be used to set additional
    /// attributes at a later time).  It's possible to create planes which are
    /// NOT renderable (i.e. vector size 5 etc.)  These planes are still
    /// created, but may not have data written to them.
    void	 setResolution(int xres, int yres);
    int		 addPlane(const char *name,		// Name of the plane
			 TIL_DataFormat storage,	// TIL_DataFormat
			 int vector_size);
    void	 createDeepRaster(bool change_vector2_to_vector3=true);

    void		 setDeepRaster(TIL_DeepRaster *raster);
    TIL_DeepRaster	*stealDeepRaster();

    TIL_DeepRaster	*getRaster()	{ return myRaster; }
    TIL_Sequence	&getSequence()	{ return mySequence; }

    // Returns the percentage of tiles received relative to the total
    // number of tiles expected for a given render.
    fpreal		 getPercentComplete() const;

    // Return the progress message
    const UT_StringHolder &getProgressMessage() const
			 { return myProgressMessage; }
    const UT_StringHolder &getProgressAction() const
			 { return myProgressAction; }
    fpreal               getElapsedTime() const
			 { return myElapsedTime; }
    fpreal               getRenderTime() const
                         { return myRenderTime; }
    int64                getPeakMemUsage() const
			 { return myPeakMemUsage; }

    // The raster version will uniquely identify a raster so that when the
    // raster changes, the version number will also change.
    int			 getVersion() const	{ return myVersion; }

protected:
    void		 setSequence( const TIL_Sequence & src )
			 { mySequence = src; }

    virtual TIL_DeepRaster	*allocRaster() const;

    virtual int		 getXres() const;
    virtual int		 getYres() const;
	    int		 getNumPlanes() const
			 { return mySequence.getNumPlanes(); }
    virtual int		 getPlaneDataFormat(int plane_index) const;

    virtual bool	 complete();
    virtual bool	 writeTile(int plane_index,
				  int x0, int x1, int y0, int y1,
				  void *data);
    virtual bool         writeSamples(
                            int nsamples,
                            int floats_per_sample,
                            const UT_Vector3T<int> *samples,
                            const float *data);
    virtual bool	 setTag(int plane, int tag, const char *val)=0;
    virtual bool	 setTag(int plane, int tag,
				const UT_StringArray &values) = 0;
    virtual bool	 setTag(int plane, int tag,
				const int *values, int nvalues=1);
    virtual bool	 setTag(int plane, int tag,
				const float *values, int nvalues=1);

private:
    TIL_Sequence	 mySequence;
    TIL_DeepRaster	*myRaster;
    UT_StringHolder      myProgressMessage;
    UT_StringHolder      myProgressAction;
    fpreal               myElapsedTime;
    fpreal               myRenderTime;
    int64                myPeakMemUsage;
    fpreal		 myProgress;
    int			 myPixelsRead;
    int			 myVersion;
    bool		 myOwnRaster;
};

#endif
