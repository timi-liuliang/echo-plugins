/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Render Library (C++)
 *
 * COMMENTS:
 *	Handle to a 2D array texture stored in the texture cache. This class is
 *	primarily responsible for fetching images from various 2D sources and
 *	creating the texture for them.
 *	A 2D texture array is made up of multiple 2D texture sources. It can
 *	also support UDIM texturing. 
 *	
 */
#ifndef __RE_Texture2DArrayMap_h__
#define __RE_Texture2DArrayMap_h__

#include <UT/UT_Rect.h>
#include <IMG/IMG_FileTypes.h>

#include "RE_API.h"
#include "RE_TextureMap.h"

class RE_API RE_Texture2DArrayMap : public RE_TextureMap
{ 
public:
	     RE_Texture2DArrayMap(bool is_tiled_texture = false,
				  int u_stride = 10);
    virtual ~RE_Texture2DArrayMap();

    virtual const char *className() const { return "RE_Texture2DArrayMap"; }
   
    /// This texture array represents a UDIM or UV tile sequence for a tiled
    /// texture when this is enabled. The U stride is normally 10 for UDIM,
    /// and ignored for UV tile textures.
    void	enableTileSource(bool e, int u_stride = 10);

    /// Returns the detected udim stride (not the u_stride above). This is the
    /// number of images in the U direction for tiled textures.
    int		getTileStride() const { return myTileArea.w(); }

    /// Returns the bounding area of the populated tiles 
    UT_DimRect  getTileArea() const { return myTileArea; }

    /// Sets a series of image files for the 2D array. They will appear in
    /// the texture layers in the same order as passed. The resolution of the
    /// array will be files.entries() layers, and the width and height will
    /// be the maximum width and height of all images, up to max_width and
    /// max_height if they are non-zero. An empty string can be passed to
    /// fill that layer with (1,1,1,1).
    void	setImageList(const UT_StringArray &files,
			     int max_width = 0,
			     int max_height = 0);

    /// Returns the amount of main memory owned by this object.
    /// GL Textures are shared in the texture cache itself.
    virtual int64 getMemoryUsage(bool inclusive) const;

    virtual RE_TextureDimension getMapType() { return RE_TEXTURE_2D_ARRAY; }
    virtual RE_TextureMap *clone() const;

    /// Returns true if the string is a tiled texture (UDIM or UV Tile) based
    /// on the prescense of %(UDIM), %(U), %(V) or %(UVTILE) in the filename.
    static bool		isArrayMap(const char *map_filename);
    
    // Public interface is in RE_TextureMap
    
    typedef void (*RE_TextureHDAResolverCB)(const char *optable_optype,
					    const char *pattern,
					    UT_StringArray &files);

    static void installHDACallback(RE_TextureHDAResolverCB cb)
			    { theHDAFileResolver = cb; }
private:
    virtual bool	buildTexture(RE_Render *r, RE_Texture *tex, void *data);
    virtual bool	getNodeSource(const char *map, const char *rel,
				      void **mapptr,
				      RE_TextureDimension &textype,
				      RE_GPUType &datatype,
				      int &vectorsize,
				      int &w, int &h, int &d);
    virtual bool	getFileSource(const char *map,
				      void **mapptr,
				      RE_TextureDimension &textype,
				      RE_GPUType &datatype,
				      int &vectorsize,
				      int &w, int &h, int &d);

    virtual void	cachedTextureFound(RE_Texture *tex);

    // UDIM-style tiled textures (1001+)
    bool		determineUDIMBounds(const char *map);

    // Mari-style u#v# textures
    bool		determineTileBounds(const char *map);

    // for %(U)d %(V)d syntax, mari-style texures
    bool		determineUVBounds(const char *map);
    // for %(UVTILE)d syntax, mari-style texures
    bool		determineUVTileBounds(const char *map);
    
    bool		findUVTileFiles(bool is_opdef,
					const char *pattern,
					const char *path,
					int base_length,
					int ext_length,
					bool u_first);
    
    bool		determineFileBounds();
    void		buildTiledTexture(RE_Render *r, RE_Texture *tex);
    void		buildArrayTexture(RE_Render *r, RE_Texture *tex);

    // TODO: for File COP support.
    // fpreal		myOpTime;
    // unsigned long	myOpVersion;
    
    bool		myIsTiledTexture;

    // image file details, per file
    IMG_DataType	myDataType;
    IMG_ColorModel	myColorModel;
    RE_GPUType		myFormat;
    int			myVectorSize;
    int			myResW;
    int			myResH;
    int			myResD;

    // for non-UDIM 2D image collections
    UT_StringArray      myFiles;
    int			myMaxW, myMaxH;
    bool		myFilesDirty;

    // tiling information for UDIM
    class re_TexArrayElement
    {
    public:
	re_TexArrayElement(const char *file, int u, int v)
	    : myFile(file), myU(u), myV(v) {}

	UT_StringHolder myFile;
	int		myU;
	int		myV;
    };
    
    int			myTileStride;
    UT_DimRect		myTileArea;
    
    UT_Array<re_TexArrayElement> myTiles;
    UT_StringHolder	myLastMap;

    // For resolving files inside HDAs.
    static RE_TextureHDAResolverCB	theHDAFileResolver;
};

#endif
