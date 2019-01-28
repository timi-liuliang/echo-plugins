/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_VoxelFFT.h ( UT Library, C++)
 *
 * COMMENTS:  Functions for performing 2D and 3D FFTs on voxel arrays.
 */

#ifndef __UT_VOXELFFT_H
#define __UT_VOXELFFT_H

#include "UT_API.h"

#include "UT_ValArray.h"
#include "UT_VoxelArray.h"
#include "UT_Complex.h"


class UT_API UT_VoxelFFT
{
public:

    UT_VoxelFFT() {}

    virtual ~UT_VoxelFFT();

    /// Perform a forward or inverse FFT on the pair of 2D or 3D voxel arrays
    /// representing real and imaginary parts of the complex input and output.
    /// If invert is true, this function computes an inverse FFT.
    /// Shift assumes the frequency-space data is shifted on input or output
    /// so that the DC (zero-th) frequency is in the center, similar to
    /// Matlab's fftshift and ifftshift functions.
    /// Normalize applies a 1/N normalization factor to the inverse FFT so that
    /// a forward FFT followed by inverse FFT yields the original input.
    /// If sliceaxis is 0, 1, or 2, a 3D input array will be treated as a series
    /// of 2D FFTs sliced along the specified axis.
    /// NOTE: as with most FFTs, performance is best with input arrays whose
    /// dimensions are powers of 2.
    void fft(UT_VoxelArrayF &re, UT_VoxelArrayF &im,
             bool invert, bool shift, bool normalize,
             int sliceaxis = -1, bool realdata = false);

    /// Performs forward or inverse FFT's on several pairs of 2D or 3D voxel
    /// arrays representing real and imaginary parts of the complex input and
    /// output.  All sequential complex pairs with the same resolution are
    /// transformed at once, which can yield signicant speedups.
    /// If invert is true, this function computes an inverse FFT.
    /// Shift assumes the frequency-space data is shifted on input or output
    /// so that the DC (zero-th) frequency is in the center, similar to
    /// Matlab's fftshift and ifftshift functions.
    /// Normalize applies a 1/N normalization factor to the inverse FFT so that
    /// a forward FFT followed by inverse FFT yields the original input.
    /// If sliceaxis is 0, 1, or 2, a 3D input array will be treated as a series
    /// of 2D FFTs sliced along the specified axis.
    /// NOTE: as with most FFTs, performance is best with input arrays whose
    /// dimensions are powers of 2.
    void fft(const UT_ValArray<UT_VoxelArrayF *> &re,
             const UT_ValArray<UT_VoxelArrayF *> &im,
             bool invert, bool shift, bool normalize,
             int sliceaxis = -1, bool realdata = false);


    // FFT interface
    class ut_fft
    {
    protected:
	int			myRank;
	int			myDims[3];
	bool			myInvert;
	exint			myEntries;
	int			myNTransforms;
	bool			myRealData;
    public:
	ut_fft(int rank, int *dims, bool invert, int ntransforms,
		bool realdata)
	{
	    myEntries = 1;
	    myRank = rank;
	    for (int i = 0; i < myRank; i++)
	    {
		myEntries *= dims[i];
		myDims[i] = dims[i];
	    }
	    myNTransforms = ntransforms;
	    myInvert = invert;
	    myRealData = realdata;
	}

	virtual ~ut_fft() {}

	// Common fft functions.

	virtual bool supportsHermitianComplex() const { return false; }

	virtual void allocBuffers() = 0;

	virtual void *mapInBuffer() = 0;

	virtual void *mapOutBuffer() = 0;

	virtual void unmapInBuffer(void *) = 0;

	virtual void unmapOutBuffer(void *) = 0;

	virtual void releaseBuffers() = 0;

	virtual exint entries() const { return myEntries; }

	virtual bool isMatching(int rank, int *dims, bool invert, int ntransforms,
			bool realdata)
	{
	    if (rank != myRank || invert != myInvert || 
		ntransforms != myNTransforms || realdata != myRealData)
		return false;
	    for (int i = 0; i < myRank; i++)
		if (dims[i] != myDims[i])
		    return false;
	    return true;
	}

	// Returns distance in input buffer between transforms.
	virtual size_t getInputDistance() const { return entries(); }

	// Returns distance in output buffer between transforms.
	virtual size_t getOutputDistance() const { return entries(); }
	
	virtual bool fft() = 0;
    };

protected:
    void computeFFTDims(const UT_VoxelArrayF &re,
			int &sliceaxis, int &ndims, int *dims,
			int &slicetransforms, UT_Vector3I &axes);

    // Override point where we can control which descendant of ut_fft we'll
    // create to actually compute the fft. (see CE_VoxelArrayFFT)
    virtual void fftSequence(const UT_ValArray<UT_VoxelArrayF *> &realvox,
			    const UT_ValArray<UT_VoxelArrayF *> &imagvox,
			    int begin, int end,
			    bool invert, bool shift,
			    bool normalize, int sliceaxis, bool realdata);

    void fftSequence(ut_fft &fft,
		     const UT_ValArray<UT_VoxelArrayF *> &realvox,
		     const UT_ValArray<UT_VoxelArrayF *> &imagvox,
		     int begin, int end,
		     bool invert, bool shift,
		     bool normalize, int sliceaxis,
		     bool realdata);

    THREADED_METHOD8(UT_VoxelFFT, resrc.numTiles() > 16,
		     copyToComplex,
		     const UT_VoxelArrayF &, resrc,
		     const UT_VoxelArrayF &, imsrc,
		     UT_ComplexF *, dst,
		     const UT_Vector3I &, axes,
		     bool, shift,
		     bool, sliced,
		     bool, realonly,
		     bool, hermitian);
    void copyToComplexPartial(const UT_VoxelArrayF &resrc,
			      const UT_VoxelArrayF &imsrc,
			      UT_ComplexF *dst,
			      const UT_Vector3I &axes,
			      bool shift,
			      bool sliced,
			      bool realonly,
			      bool hermitian,
			      const UT_JobInfo &info);

    THREADED_METHOD9(UT_VoxelFFT, redst.numTiles() > 16,
		     copyFromComplex,
		     const UT_ComplexF *, src,
		     UT_VoxelArrayF &, redst,
		     UT_VoxelArrayF &, imdst,
		     fpreal32, scale,
		     const UT_Vector3I &, axes,
		     bool, shift,
		     bool, sliced,
		     bool, realonly,
		     bool, hermitian);
    void copyFromComplexPartial(const UT_ComplexF *src,
				UT_VoxelArrayF &redst,
				UT_VoxelArrayF &imdst,
				fpreal32 scale,
				const UT_Vector3I &axes,
				bool shift,
				bool sliced,
				bool realonly,
				bool hermitian,
				const UT_JobInfo &info);

    THREADED_METHOD4(UT_VoxelFFT, redst.numTiles() > 16,
		     copyFromReal,
		     const fpreal32 *, src,
		     UT_VoxelArrayF &, redst,
		     fpreal32, scale,
		     const UT_Vector3I &, axes);
    void copyFromRealPartial(const fpreal32 *src,
			     UT_VoxelArrayF &redst,
			     fpreal32 scale,
			     const UT_Vector3I &axes,
			     const UT_JobInfo &info);

    THREADED_METHOD4(UT_VoxelFFT, resrc.numTiles() > 16,
		     copyToReal,
		     const UT_VoxelArrayF &, resrc,
		     fpreal32 *, dst,
		     fpreal32, scale,
		     const UT_Vector3I &, axes);
    void copyToRealPartial(const UT_VoxelArrayF& resrc,
			   fpreal32* dst,
			   fpreal32 scale,
			   const UT_Vector3I &axes,
			   const UT_JobInfo& info);
};

#endif
