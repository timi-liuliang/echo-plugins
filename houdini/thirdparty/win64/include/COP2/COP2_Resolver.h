/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP_CopResolver.h ( COP Library, C++)
 *
 * COMMENTS:	Handle grabbing texture maps from COPs.
 */

#ifndef __COP2_Resolver__
#define __COP2_Resolver__

#include "COP2_API.h"
#include <TIL/TIL_CopResolver.h>

class COP2_API COP2_Resolver : public TIL_CopResolver
{
public:
	     COP2_Resolver();
    virtual ~COP2_Resolver();

    virtual int		 resolveId(const char *fullpath);
    virtual TIL_Raster	*resolveRaster(int id);
    virtual TIL_Raster	*resolveRasterFrame(int id, fpreal frame,
					    int xres =0, int yres =0);
    virtual TIL_Raster	*resolveRasterPlane(int id, fpreal frame,
					    const char *color, int cindex,
					    const char *alpha, int aindex,
					    int xres = 0, int yres = 0,
					    TIL_DataFormat f =
						TILE_MAX_DATA_FORMAT);
    virtual bool	 resolveIsPossible(const char *fullpath);
    virtual int		 isOpBased();
    
    virtual void	 buildPlaneMenu(const char *net, const char *node,
					UT_ValArray<char *> &items);
    
    virtual void	getRes(int id, int &xres, int &yres);
    
    virtual TIL_Raster  *getFileRaster(const char *file,
				       IMG_DataType type = IMG_DT_ANY,
				       int xres = 0, int yres = 0);
    virtual TIL_Raster	*getNodeRaster(const char *fullpath,
				       const char *cmenu, const char *amenu,
				       bool override_frame = false,
				       fpreal frame = 1.0,
				       TIL_DataFormat f=TILE_MAX_DATA_FORMAT);
    
    virtual bool	returnRaster(const TIL_Raster *raster);
    virtual fpreal	getTimeForRaster(int opid);
    virtual size_t	getVersionForRaster(int opid);
    virtual TIL_Sequence *getSequence(int id);
    virtual bool	resolveRasters(int id,
				       const UT_StringArray &plane_names,
				       fpreal t,
				       UT_ValArray<TIL_Raster *> &new_rasters,
				       TIL_DataFormat format);
    virtual TIL_ImageSource *resolveImageSource(int id);

};

#endif
