/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        TIL_UVEnlarger.h (TIL Library, C++)
 *
 * COMMENTS:	This class is responsible for post-processing of texture maps
 *		in order to help reduce the effects of seams forming at the
 *		border of UV islands. It's currently being used to post-process
 *		UV rendered images (see VGEO_UVUnwrapper).
 */
#ifndef TIL_UVEnlarger_H
#define TIL_UVEnlarger_H

#include "TIL_API.h"
#include <PXL/PXL_Raster.h>

#include "TIL_Defines.h"
#include <set>

class TIL_API TIL_UVEnlarger
{
public:
    enum Scheme
    {
        /// Computes the average value of all the pixels that are a part of
        /// UV islands, then applies that average value to all empty parts
        /// of the image.
        SCHEME_FILL_AVERAGE,

        /// Does a flood fill on the image and expands the boundary of each
        /// UV island found by the amount specified by myFillPadding.
        SCHEME_FLOOD_FILL,

        /// Does a diffusion fill on empty areas after expanding boundaries
	/// of UV island specified by myFillPadding.
	SCHEME_DIFFUSE_FILL,

        SCHEME_INVALID
    };

    /// If craster is NULL, then pixels in raster that has zero value will be
    /// assumed to have zero alpha.
    TIL_UVEnlarger(PXL_Raster &raster,
                   PXL_Raster *craster,
                   Scheme scheme = SCHEME_FLOOD_FILL,
                   int fillPadding = 4);

    ~TIL_UVEnlarger();

    /// How many pixels to pad each UV island with, when using the
    /// flood filling scheme for enlarging.
    void	 setFillPadding(int fp)
		 { myFillPadding = fp; }

    /// Sets the scheme to use when enlarging.
    void	 setScheme(Scheme scheme)
		 { myScheme = scheme; }

    bool	 enlarge();
private:
    PXL_Raster		           &myRaster;
    PXL_Raster                     *myCRaster;
    int                             myAttribNum;

    Scheme			    myScheme;
    int				    myFillPadding;

    /// The average pixel color of every UV island is computed, then the 
    /// background of the image is filled with that average color.
    bool         fillBackgroundColor();

    /// Return true if a pixel is on an UV island (filled area).
    bool         onIsland(const int &x,
                          const int &y);
};

#endif
