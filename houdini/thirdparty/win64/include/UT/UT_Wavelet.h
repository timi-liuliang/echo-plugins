/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Wavelet.h ( UT Library, C++)
 *
 * COMMENTS:
 *	This is a verb class to apply various wavelet transforms
 *	to common Houdini objects.
 */

#ifndef __UT_Wavelet__
#define __UT_Wavelet__

#include "UT_API.h"

#include "UT_VoxelArray.h"
#include "UT_VectorTypes.h"

class UT_API UT_Wavelet 
{
public:
    enum WAVELET_NAMES
    {
	WAVELET_HAAR,
	NUM_WAVELETS
    };

    /// These functions build a packed pyramid of coefficients
    /// or, for inverse, unpack it.  The VoxelArray variants
    /// work on 3d arrays but can also be used for 2d arrrays
    /// by setting one of the dimensions to 1.
    static void transformOrdered(WAVELET_NAMES wavelettype,
			const UT_FloatArray &source,
			UT_FloatArray &dest,
			int maxlevels = -1);
    static void inverseTransformOrdered(WAVELET_NAMES wavelettype,
			const UT_FloatArray &source,
			UT_FloatArray &dest,
			int maxlevels = -1);

    static void transformOrdered(WAVELET_NAMES wavelettype,
			const UT_VoxelArrayF &source,
			UT_VoxelArrayF &dest,
			int maxlevels = -1);
    static void inverseTransformOrdered(WAVELET_NAMES wavelettype,
			const UT_VoxelArrayF &source,
			UT_VoxelArrayF &dest,
			int maxlevels = -1);

    /// Extracts one of the subfields from a packed wavelet.
    /// All subfields have the rounded-up size so are the same
    /// dimension - missing wavelet coefficients are set to 0.
    /// The zeroth component is the low pass filter and only
    /// is valid if the packed pyramid was stopped at that
    /// level.
    /// level 0 is the first set of detail coefficients.
    static void extractComponent(const UT_VoxelArrayF &wavelets,
			UT_VoxelArrayF &dest,
			int level, int component);

    /// Computes the power of the given level of a packed voxel array.
    /// level 0 is the first set of detail coefficients.
    /// This corresponds to computing the power for a single
    /// frequency - this is the sum of squares of the detail coefficients.
    static void	computePowerSpectrum(const UT_VoxelArrayF &wavelets,
			UT_VoxelArrayF &dest,
			int level);

    /// Performs a single pass of the given wavelet transform
    /// Does an ordered transform, so is not in place.
    static void	transformOrderedOnePass(WAVELET_NAMES wavelettype,
			const UT_FloatArray &source,
			UT_FloatArray &wavelets);
    static void	inverseTransformOrderedOnePass(WAVELET_NAMES wavelettype,
			const UT_FloatArray &wavelets,
			UT_FloatArray &data);

    /// Specific wavelet implementations
    static void transformOrderedOnePassHaar(const UT_FloatArray &source,
			UT_FloatArray &wavelets);
    static void inverseTransformOrderedOnePassHaar(const UT_FloatArray &wavelets,
			UT_FloatArray &data);

private:
    static fpreal computeEnergy(const UT_VoxelArrayF &wavelets,
			int x, int y, int z,
			int nx, int ny, int nz);
};

#endif

