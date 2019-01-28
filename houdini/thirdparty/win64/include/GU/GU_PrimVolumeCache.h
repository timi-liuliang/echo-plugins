/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimVolumeCache.h ( GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_PrimVolumeCache__
#define __GU_PrimVolumeCache__

#include "GU_API.h"

#include <string.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4.h>
#include <UT/UT_Array.h>
#include <UT/UT_VoxelArray.h>
#include <UT/UT_ThreadedAlgorithm.h>

#include <GEO/GEO_PrimVolume.h>

#include "GU_Detail.h"
#include "GU_DetailHandle.h"
#include "GU_DisplayCache.h"

class UT_Ramp;
class GEO_PrimVolume;
class GEO_PrimVDB;

// TODO: When deleting GU_DisplayCache, DON'T delete
//       GU_PrimVolumeCache.  Move it to GT_PrimVolumeCache,
//       along with any functions needed from the superclass.
//       It's still used by GT_PrimVolume!!!
SYS_DEPRECATED_PUSH_DISABLE()

class GU_API GU_PrimVolumeCacheLight
{
public:
    GU_PrimVolumeCacheLight()
	: myOrg(0,0,0), myDir(0,0,0), myCd(0,0,0), myShadow(0.0),
	  myHasAttenuation(false), myIsAmbient(false), myIsInfinite(false),
	  myIsHeadlight(false), myIsEnvironment(false)
    {
	memset(myAtten, 0, sizeof(float)*8);
    }
    
    bool		 operator==(const GU_PrimVolumeCacheLight &l) const
			{
			    // ignore headlight to prevent updates on tumble
			    if (myIsHeadlight &&
				myIsHeadlight == l.myIsHeadlight)
				return true;
			    
			    return (myOrg == l.myOrg) &&
				   (myDir == l.myDir) &&
				   (myCd == l.myCd) &&
				   (myIsAmbient == l.myIsAmbient) &&
				   (myIsInfinite == l.myIsInfinite) &&
				   (myShadow == l.myShadow) &&
				   (myIsEnvironment == l.myIsEnvironment);
			}
    
    void		 setOrg(const UT_Vector3 &org) { myOrg = org; }
    void		 setDir(const UT_Vector3 &dir) { myDir = dir; }
    void		 setCd(const UT_Vector3 &cd) { myCd = cd; }
    void		 setShadowIntensity(float shadow) { myShadow = shadow; }
    void		 setIsAmbient(bool isamb) { myIsAmbient = isamb; }
    void		 setIsInfinite(bool isinf) { myIsInfinite = isinf; }
    void		 setIsHeadlight(bool hl) { myIsHeadlight = hl; }
    void		 setIsEnvironment(bool env) { myIsEnvironment = env; }
    
    const UT_Vector3	&getOrg() const { return myOrg; }
    const UT_Vector3	&getDir() const { return myDir; }
    const UT_Vector3	&getCd() const { return myCd; }
    float		 getShadow() const { return myShadow; }

    bool		 isAmbient() const { return myIsAmbient; }
    bool		 isInfinite() const { return myIsInfinite; }
    bool		 isHeadlight() const { return myIsHeadlight; }
    bool		 isEnvironment() const { return myIsEnvironment; }

    float		 getDistance(const UT_Vector3 &pos) const;
    // Inv Brightness attenuation for the 8 corners: x, then y, then z; - then +
    // interpolate, then invert. 
    void		 setCornerAttenuation(float a[8])
			 {
			     myHasAttenuation = true;
			     for(int i=0; i<8; i++)
				 myAtten[i] = a[i];
			 }
    bool		 hasAttenuation() const { return myHasAttenuation; }
    float		 getAttenuation(const UT_Vector3F &uvw) const;
protected:
    UT_Vector3		myOrg, myDir;
    UT_Vector3		myCd;
    float		myShadow;
    bool		myHasAttenuation;
    float		myAtten[8]; 
    bool		myIsAmbient, myIsInfinite, myIsHeadlight;
    bool		myIsEnvironment;
};


class GU_PrimVolumeCacheParms;
class GU_PrimVolume;

class GU_API GU_PrimVolumeTexture
{
public: 
    SYS_DEPRECATED_HDK(13.0)
		 GU_PrimVolumeTexture() {}
    SYS_DEPRECATED_HDK(13.0)
    virtual	~GU_PrimVolumeTexture() {}

    SYS_DEPRECATED_HDK(13.0)
    virtual void	 refresh(const UT_VoxelArrayV4 *voxels) = 0;
    SYS_DEPRECATED_HDK(13.0)
    virtual int64	 getMemoryUsage(bool inclusive) const = 0;
};

class GU_API GU_PrimVolumeCacheRamp
{
public:
    GU_PrimVolumeCacheRamp(UT_Ramp *ramp, bool periodic);
    ~GU_PrimVolumeCacheRamp();

    void 		eval(float &rval, float val) const;	
    void 		eval(UT_Vector4 &rval, float val) const;	

protected:
    UT_Ramp		*myRamp;
    bool		 myPeriodic;
};

class GU_API GU_PrimVolumeCacheSampler
{
public:
    GU_PrimVolumeCacheSampler(const GEO_Detail *gdp, const GEO_Primitive *vol);
    ~GU_PrimVolumeCacheSampler();

    GU_PrimVolumeCacheRamp *buildRampFromAttribute(
				    const GEO_Detail *gdp, 
				    const char *attribname) const;
    int			 findCollationIndex(const GEO_Detail *gdp,
					    const GEO_Primitive *vol) const;
    const GEO_Primitive *selectVolumeFromAttribute(
				    const GEO_Detail *gdp, 
				    const char *attribname,
				    int collateidx) const;
    void		 selectVolumesFromAttribute(
				    const GEO_Primitive *vol[3], 
				    const GEO_Detail *gdp, 
				    const char *attribname,
				    int collateidx) const;
    void		 getRangeFromAttribute(
				    const GEO_Detail *gdp, 
				    const char *attribname, 
				    float &rmin, float &rscale) const;
    float		 getFloatFromAttribute(
				    const GEO_Detail *gdp, 
				    const char *attribname, 
				    float def) const;

    float	getDensity(int x, int y, int z) const;
    UT_Vector4	getDiffuse(int x, int y, int z) const;

    void	getDensityProbe(UT_VoxelProbeF &probe) const;
    void	getEmitProbe(UT_VoxelProbeF &probe) const;
    void	getEmitCdProbe(UT_VoxelProbeV4 &probe) const;
    const UT_VoxelArrayF *getDensityArray() const;

    const GEO_Primitive *densityField() const { return myDensityField; }
    void		 setDensityField(const GEO_Primitive *densityfield);

    void	hardenFields(const UT_VoxelArray<UT_Vector4> &refvol, 
			     const GEO_PrimVolumeXform &volxform);

    UT_VoxelArrayReadHandleF	hardenScalar(const UT_VoxelArrayV4 &refvol, 
			    const GEO_PrimVolumeXform &volxform,
			    const GEO_Primitive *field,
			    GU_PrimVolumeCacheRamp *ramp,
			    float rmin, float rinvscale);
    UT_VoxelArrayReadHandleV4	hardenVector(const UT_VoxelArrayV4 &refvol, 
			    const GEO_PrimVolumeXform &volxform,
			    const GEO_Primitive *field,
			    GU_PrimVolumeCacheRamp *ramp,
			    float rmin, float rinvscale);
    UT_VoxelArrayReadHandleV4 hardenVectorMulti(const UT_VoxelArrayV4 &refvol, 
			    const GEO_PrimVolumeXform &volxform,
			    const GEO_Primitive *field[3],
			    GU_PrimVolumeCacheRamp *ramp,
			    float rmin, float rinvscale);
    UT_VoxelArrayReadHandleV4	hardenVDBVector(const UT_VoxelArrayV4 &refvol, 
			    const GEO_PrimVolumeXform &volxform,
			    const GEO_Primitive *field,
			    GU_PrimVolumeCacheRamp *ramp,
			    float rmin, float rinvscale);

    THREADED_METHOD7_CONST(GU_PrimVolumeCacheSampler,
			    dst->numTiles() > 1,
			    doHardenAligned,
			    UT_VoxelArrayF *, dst,
			    const UT_VoxelArrayF *, src,
			    const GEO_PrimVolumeXform &, dst_xform,
			    const GEO_PrimVolumeXform &, src_xform,
			    GU_PrimVolumeCacheRamp *, ramp,
			    float, rmin,
			    float, rinvscale)
    void	doHardenAlignedPartial(UT_VoxelArrayF *dst,
				const UT_VoxelArrayF *src,
				const GEO_PrimVolumeXform &dst_xform,
				const GEO_PrimVolumeXform &src_xform,
				GU_PrimVolumeCacheRamp *ramp,
				float rmin, float rinvscale,
				const UT_JobInfo &info) const;
    THREADED_METHOD7_CONST(GU_PrimVolumeCacheSampler,
			    dst->numTiles() > 1,
			    doHardenUnaligned,
			    UT_VoxelArrayF *, dst,
			    const UT_VoxelArrayF *, src,
			    const GEO_PrimVolumeXform &, dst_xform,
			    const GEO_PrimVolumeXform &, src_xform,
			    GU_PrimVolumeCacheRamp *, ramp,
			    float, rmin,
			    float, rinvscale)
    void	doHardenUnalignedPartial(UT_VoxelArrayF *dst,
				const UT_VoxelArrayF *src,
				const GEO_PrimVolumeXform &dst_xform,
				const GEO_PrimVolumeXform &src_xform,
				GU_PrimVolumeCacheRamp *ramp,
				float rmin, float rinvscale,
				const UT_JobInfo &info) const;
    THREADED_METHOD7_CONST(GU_PrimVolumeCacheSampler,
			    dst->numTiles() > 1,
			    doHardenVectorAligned,
			    UT_VoxelArrayV4 *, dst,
			    const UT_VoxelArrayF *, src,
			    const GEO_PrimVolumeXform &, dst_xform,
			    const GEO_PrimVolumeXform &, src_xform,
			    GU_PrimVolumeCacheRamp *, ramp,
			    float, rmin,
			    float, rinvscale)
    void	doHardenVectorAlignedPartial(UT_VoxelArrayV4 *dst,
				const UT_VoxelArrayF *src,
				const GEO_PrimVolumeXform &dst_xform,
				const GEO_PrimVolumeXform &src_xform,
				GU_PrimVolumeCacheRamp *ramp,
				float rmin, float rinvscale,
				const UT_JobInfo &info) const;
    THREADED_METHOD7_CONST(GU_PrimVolumeCacheSampler,
			    dst->numTiles() > 1,
			    doHardenVectorUnaligned,
			    UT_VoxelArrayV4 *, dst,
			    const UT_VoxelArrayF *, src,
			    const GEO_PrimVolumeXform &, dst_xform,
			    const GEO_PrimVolumeXform &, src_xform,
			    GU_PrimVolumeCacheRamp *, ramp,
			    float, rmin,
			    float, rinvscale)
    void	doHardenVectorUnalignedPartial(UT_VoxelArrayV4 *dst,
				const UT_VoxelArrayF *src,
				const GEO_PrimVolumeXform &dst_xform,
				const GEO_PrimVolumeXform &src_xform,
				GU_PrimVolumeCacheRamp *ramp,
				float rmin, float rinvscale,
				const UT_JobInfo &info) const;
    THREADED_METHOD8_CONST(GU_PrimVolumeCacheSampler,
			    dst->numTiles() > 1,
			    doHardenVectorAxisAligned,
			    UT_VoxelArrayV4 *, dst,
			    const UT_VoxelArrayF *, src,
			    int, axis,
			    const GEO_PrimVolumeXform &, dst_xform,
			    const GEO_PrimVolumeXform &, src_xform,
			    GU_PrimVolumeCacheRamp *, ramp,
			    float, rmin,
			    float, rinvscale)
    void	doHardenVectorAxisAlignedPartial(UT_VoxelArrayV4 *dst,
				const UT_VoxelArrayF *src,
				int axis,
				const GEO_PrimVolumeXform &dst_xform,
				const GEO_PrimVolumeXform &src_xform,
				GU_PrimVolumeCacheRamp *ramp,
				float rmin, float rinvscale,
				const UT_JobInfo &info) const;
    THREADED_METHOD8_CONST(GU_PrimVolumeCacheSampler,
			    dst->numTiles() > 1,
			    doHardenVectorAxisUnaligned,
			    UT_VoxelArrayV4 *, dst,
			    const UT_VoxelArrayF *, src,
			    int, axis,
			    const GEO_PrimVolumeXform &, dst_xform,
			    const GEO_PrimVolumeXform &, src_xform,
			    GU_PrimVolumeCacheRamp *, ramp,
			    float, rmin,
			    float, rinvscale)
    void	doHardenVectorAxisUnalignedPartial(UT_VoxelArrayV4 *dst,
				const UT_VoxelArrayF *src,
				int axis,
				const GEO_PrimVolumeXform &dst_xform,
				const GEO_PrimVolumeXform &src_xform,
				GU_PrimVolumeCacheRamp *ramp,
				float rmin, float rinvscale,
				const UT_JobInfo &info) const;

    THREADED_METHOD6_CONST(GU_PrimVolumeCacheSampler,
			    dst->numTiles() > 1,
			    doHardenVDB,
			    UT_VoxelArrayF *, dst,
			    const GEO_PrimVDB *, src,
			    const GEO_PrimVolumeXform &, xform,
			    GU_PrimVolumeCacheRamp *, ramp,
			    float, rmin,
			    float, rinvscale)
    void	doHardenVDBPartial(UT_VoxelArrayF *dst,
				const GEO_PrimVDB *src,
				const GEO_PrimVolumeXform &xform,
				GU_PrimVolumeCacheRamp *ramp,
				float rmin, float rinvscale,
				const UT_JobInfo &info) const;
    THREADED_METHOD6_CONST(GU_PrimVolumeCacheSampler,
			    dst->numTiles() > 1,
			    doHardenVectorVDB,
			    UT_VoxelArrayV4 *, dst,
			    const GEO_PrimVDB *, src,
			    const GEO_PrimVolumeXform &, xform,
			    GU_PrimVolumeCacheRamp *, ramp,
			    float, rmin,
			    float, rinvscale)
    void	doHardenVectorVDBPartial(UT_VoxelArrayV4 *dst,
				const GEO_PrimVDB *src,
				const GEO_PrimVolumeXform &xform,
				GU_PrimVolumeCacheRamp *ramp,
				float rmin, float rinvscale,
				const UT_JobInfo &info) const;
    THREADED_METHOD7_CONST(GU_PrimVolumeCacheSampler,
			    dst->numTiles() > 1,
			    doHardenVectorAxisVDB,
			    UT_VoxelArrayV4 *, dst,
			    const GEO_PrimVDB *, src,
			    int, axis,
			    const GEO_PrimVolumeXform &, xform,
			    GU_PrimVolumeCacheRamp *, ramp,
			    float, rmin,
			    float, rinvscale)
    void	doHardenVectorAxisVDBPartial(UT_VoxelArrayV4 *dst,
				const GEO_PrimVDB *src,
				int axis,
				const GEO_PrimVolumeXform &xform,
				GU_PrimVolumeCacheRamp *ramp,
				float rmin, float rinvscale,
				const UT_JobInfo &info) const;
    THREADED_METHOD6_CONST(GU_PrimVolumeCacheSampler,
			    dst->numTiles() > 1,
			    doHardenVectorVectorVDB,
			    UT_VoxelArrayV4 *, dst,
			    const GEO_PrimVDB *, src,
			    const GEO_PrimVolumeXform &, xform,
			    GU_PrimVolumeCacheRamp *, ramp,
			    float, rmin,
			    float, rinvscale)
    void	doHardenVectorVectorVDBPartial(UT_VoxelArrayV4 *dst,
				const GEO_PrimVDB *src,
				const GEO_PrimVolumeXform &xform,
				GU_PrimVolumeCacheRamp *ramp,
				float rmin, float rinvscale,
				const UT_JobInfo &info) const;

    THREADED_METHOD2_CONST(GU_PrimVolumeCacheSampler,
			    dst->numTiles() > 1,
			    applyAmbientLight,
			    UT_VoxelArrayV4 *, dst,
			    const UT_Vector4, cd)
    void	applyAmbientLightPartial(UT_VoxelArrayV4 *dst,
					const UT_Vector4 &cd,
					const UT_JobInfo &info) const;

    bool	hasEmission() const;
    float	getEmissionScale() const { return myEmissionScale; }
    float	getShadowScale() const { return myShadowScale; }

protected:
    float			 myDensityMin, myDensityInvRange;
    const GEO_Primitive		*myDensityField;
    GU_PrimVolumeCacheRamp	*myDensityRamp;
    float			 myShadowScale;


    float			 myDiffuseMin, myDiffuseInvRange;
    const GEO_Primitive		*myDiffuseField[3];
    GU_PrimVolumeCacheRamp	*myDiffuseRamp;

    float			 myEmissionScale;
    float			 myEmissionMin, myEmissionInvRange;
    const GEO_Primitive		*myEmissionField;
    GU_PrimVolumeCacheRamp	*myEmissionRamp;


    float			 myEmissionCdMin, myEmissionCdInvRange;
    const GEO_Primitive		*myEmissionCdField[3];
    GU_PrimVolumeCacheRamp	*myEmissionCdRamp;

    UT_VoxelArrayReadHandleF	 myDensityHandle;
    UT_VoxelArrayReadHandleV4	 myDiffuseHandle;
    UT_VoxelArrayReadHandleF	 myEmissionHandle;
    UT_VoxelArrayReadHandleV4	 myEmissionCdHandle;
};

class GU_API GU_PrimVolumeCache : public GU_DisplayCache
{
public:
    		 GU_PrimVolumeCache(void);
    virtual	~GU_PrimVolumeCache(void);

    // Build the cache.
    int		 refresh(const GEO_Primitive *parent_prim,
			 const GU_PrimVolumeCacheParms &parms);

    // Get the cached data.
    UT_VoxelArrayV4		*getShadedVoxels(bool *reduced=nullptr) const
				{
				    if(reduced)
					*reduced = myVolumeReduced;
				    return myVoxels;
				}
    SYS_DEPRECATED_HDK(13.0)
    GU_PrimVolumeTexture	*getTexture() const	 { return myTexture; }
    SYS_DEPRECATED_HDK(13.0)
    void			 setTexture(GU_PrimVolumeTexture *tex)
				 { myTexture = tex; }

    GU_ConstDetailHandle	 getIsoSurface() const	{ return myIsoSurf; }

    UT_Vector3			 getIsoCd() const { return myIsoCd; }

    /// The cached resolution
    UT_Vector3I			 getRes() const { return myCacheRes; }

    GEO_PrimVolumeXform		 getSpaceTransform() const
				    { return myVolumeXform; }

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        if (myVoxels)
            mem += myVoxels->getMemoryUsage(true);
        mem += myLightList.getMemoryUsage(true);
        if (myTexture)
            mem += myTexture->getMemoryUsage(true);
	mem += myIsoSurf.getMemoryUsage(false);
        return mem;
    }

protected:
    void	 lightVoxelsFromLight(const GU_PrimVolumeCacheSampler &sampler,
				    const GU_PrimVolumeCacheLight &light,
				    const UT_Vector3 &cd,
				    float shadowdensity);

    void	 rainbowVoxels(const GU_PrimVolumeCacheSampler &sampler);

    THREADED_METHOD3(GU_PrimVolumeCache, myVoxels->numTiles() > 1,
		    computeAlphaVoxels,
		    const GU_PrimVolumeCacheSampler &, sampler,
		    float, lightdensity,
		    bool, premultiply)
    void	 computeAlphaVoxelsPartial(
				const GU_PrimVolumeCacheSampler &sampler,
				float lightdensity,
				bool premultiply,
				const UT_JobInfo &info);

    /// Maximum per-axis resolution to do the self shadowing calculations
    /// at.
    int			 getMaxRes(const GEO_Primitive *volume,
				   const GU_PrimVolumeCacheParms &parms,
				   int axis) const;

    /// Used to convert to and from our *cached* indices, not the
    /// actual primitive indices that may be a different resolution
    bool		 indexToPos(int x, int y, int z, UT_Vector3 &pos) const;
    bool		 posToIndex(UT_Vector3 pos, int &x, int &y, int &z) const;	

    UT_VoxelArrayV4		*myVoxels;
    UT_Array<GU_PrimVolumeCacheLight> myLightList;
    SYS_DEPRECATED_HDK(13.0)
    GU_PrimVolumeTexture	*myTexture;
    GU_DetailHandle		 myIsoSurf;
    float			 myOldLOD;
    UT_Vector3			 myIsoCd;
    
    UT_Vector3I			 myCacheRes;
    GEO_PrimVolumeXform		 myVolumeXform;
    bool			 myVolumeReduced;
    
    // Ignore these
    UT_Matrix4			 myCacheXform;
    UT_Matrix4			 myCacheIXform;
    
    const UT_VoxelArrayV4		*myCacheVolume;
};

class GU_API GU_PrimVolumeCacheParms : public GU_CacheParms
{
public:
    		 GU_PrimVolumeCacheParms(void)
		     : myMaxResW(0), myMaxResH(0), myMaxResD(0)
		     , myMax2DRes(0)
		     , myIsoPoints(false)
		     , myPreMultiply(true)
		     , myCachedFlag(true)
		 {}
    virtual	~GU_PrimVolumeCacheParms(void) {}

    void	setLights(UT_Array<GU_PrimVolumeCacheLight> &lightlist)
		{ myLightList = lightlist; }
    const UT_Array<GU_PrimVolumeCacheLight> &getLights() const
		{ return myLightList; }

    void	setMaxResolution(int w, int h, int d)
		{ myMaxResW = w; myMaxResH = h; myMaxResD = d; }
    void	getMaxResolution(int &w, int &h, int &d) const
		{ w = myMaxResW; h = myMaxResH; d = myMaxResD; }

    void	setMax2DRes(int s)
		{ myMax2DRes = s; }
    int		getMax2DRes() const
		{ return myMax2DRes; }

    void	setIsoPoints(bool iso_pts)  { myIsoPoints = iso_pts; }
    bool	isIsoPoints() const	    { return myIsoPoints; }

    void	setPreMultiply(bool premultiply) { myPreMultiply = premultiply; }
    bool	getPreMultiply() const { return myPreMultiply; }

    void	setCached(bool b) const { myCachedFlag = b; }
    bool	isCached() const { return myCachedFlag; }

private:
    UT_Array<GU_PrimVolumeCacheLight> myLightList;
    int				      myMaxResW;
    int				      myMaxResH;
    int				      myMaxResD;
    int				      myMax2DRes;
    bool			      myIsoPoints;
    bool			      myPreMultiply;
    mutable bool		      myCachedFlag;
};

SYS_DEPRECATED_POP_DISABLE()

#endif

