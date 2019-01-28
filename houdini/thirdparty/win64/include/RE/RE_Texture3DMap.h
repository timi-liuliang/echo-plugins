/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Render Library (C++)
 *
 * COMMENTS:	3D Texture definitions	
 */

#ifndef __RE_Texture3DMap_h__
#define __RE_Texture3DMap_h__


#include "RE_API.h"
#include "RE_TextureTypes.h"
#include "RE_TextureMap.h"
#include <UT/UT_VectorTypes.h>

template <class T> class UT_VoxelArray;
class re_Tex3D;

class RE_API RE_Texture3DMap : public RE_TextureMap
{ 
public:

	     RE_Texture3DMap();
    virtual ~RE_Texture3DMap();

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_Texture3DMap.
    virtual int64 getMemoryUsage(bool inclusive) const;

    virtual RE_TextureDimension getMapType() { return RE_TEXTURE_3D; }
    virtual RE_TextureMap *clone() const;
    
    
    // Create the texture from a voxel array
    void	 setVoxels(const char *name,
			   const UT_VoxelArray<UT_Vector4> *voxels);

protected:
    virtual bool	buildTexture(RE_Render *r, RE_Texture *t, void *data);
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
private:
    bool		getSource(const char *map,
				  void **mapptr,
				  RE_TextureDimension &textype,
				  RE_GPUType &datatype,
				  int &vectorsize,
				  int &w, int &h, int &d);
    
    const UT_VoxelArray<UT_Vector4> *myVoxelSource;
    int				     myVoxelW;
    int				     myVoxelH;
    int				     myVoxelD;
};

#endif
