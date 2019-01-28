/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry library (C++)
 *
 * COMMENTS:	Display cache for spline surfaces.
 *
 */

#ifndef __GU_TPSurfCache_H__
#define __GU_TPSurfCache_H__

#include "GU_API.h"
#include <UT/UT_Vector4Array.h>
#include <UT/UT_Vector3Array.h>
#include <GA/GA_AttributeRef.h>
#include <GA/GA_Handle.h>
#include "GU_DisplayCache.h"

class GEO_TPSurf;
class GEO_AttributeHandleList;
class GEO_Detail;
class GU_TPSurfCacheParms;
class GU_TrimTesselator;
class GU_TrimPatch;

enum GU_TPSurfTrimState
{
    GU_TPSURF_TRIM_PARTIAL = -1,	// trim curve matters, partially trimmed
    GU_TPSURF_TRIM_ALL_OUT = 0,		// trimmed out
    GU_TPSURF_TRIM_ALL_IN  = 1		// all present (trimmed in)
};

// TODO: When deleting GU_DisplayCache, DON'T delete
//       GU_TPSurfCache!  Move it to GT_TPSurfCache, since
//       it's still used by GT_GEOPrimTPSurf!!!
SYS_DEPRECATED_PUSH_DISABLE()

class GU_API GU_TPTrimSurfCache
{
public:
     GU_TPTrimSurfCache();
    ~GU_TPTrimSurfCache();

    int64 getMemoryUsage(bool inclusive) const;

    void	resizeAttributes(const GEO_Detail *gdp);
    bool	hasAttributes() const
		    { return myHasCd || myHasUv || myHasAf; }

private:
    // Uncut loops
    UT_Vector4Array	 myPoints;
    UT_Vector3Array	 myNormals;
    UT_Vector3Array	 myColors;
    UT_Vector3Array	 myUVs;
    UT_FloatArray	 myAlphas;
    GU_TrimTesselator	*myTesselator;
    GU_TrimPatch	*myPatch;
    GU_TPSurfTrimState	 myTrimmedState; 
    bool		 myHasCd;
    bool		 myHasUv;
    bool		 myHasAf;

    friend class GU_TPSurfCache;
};

class GU_API GU_TPSurfCache : public GU_DisplayCache
{
public:
    		 GU_TPSurfCache(void);
    virtual	~GU_TPSurfCache(void);

    int64 getMemoryUsage(bool inclusive) const;

    // Record the domain interval in which the surface needs to be recomputed:
    void	 touchDomain(int ustart_idx, int ustop_idx,
			     int vstart_idx, int vstop_idx);

    // Get tesselation of patch.
    // Will tesselate if not already done...
    GU_TrimTesselator	*getTesselation();

    // Gets the tesselated trimmed patch:
    const GU_TrimPatch	*getPatch() const
			    { return (myPatches)?myPatches->myPatch : 0;}

    // Build the cache.
    int		 refresh(GU_TPSurfCacheParms &parms);

    // Find out how the cache was built: wire or shaded.
    GU_CacheMode builtAs(void) const { return myMode; }

    // Get the curve and point counts given the internal lod, the build type,
    // and the mesh's "wrap" characteristics.
    void	 getCounts(int uwrapped, int vwrapped, 
			   int &nucurves, int &nupoints,
			   int &nvcurves, int &nvpoints) const;
    void	 getCounts(int uwrapped, int vwrapped, 
			   int &nupoints, int &nvpoints) const;

    // we only worry about alpha if there is color as well
    int		 hasAttributes(GU_CacheMode mode) const
		 {
		     return (!myCdOffset.isValid() && mode != GU_CACHESHADED)
			 ? 0
			 : myCdOffset.isValid() ||
			    myTxtOffset.isValid() ||
			    myAlphaOffset.isValid();
		 }

    bool	hasColor() const	{ return myCdOffset.isValid(); }
    bool	hasTexture() const	{ return myTxtOffset.isValid(); }
    bool	hasAlpha() const	{ return myAlphaOffset.isValid(); }

    // 0 if trimmed out, 1 if all present, -1 if trim curve matters.
    GU_TPSurfTrimState	getTrimmedState(void) const	
		 { 
		     return !myPatches ? GU_TPSURF_TRIM_ALL_IN 
				       : myPatches->myTrimmedState; 
		 }

    // Allocates new trim index.
    int		 getNewTrimIndex();
    // Evaluates trim point described by index:
    void	 evaluateTrimIndex(GEO_AttributeHandleList *hlist,
			const GEO_TPSurf *surf, int index, float u, float v);

    const UT_Vector4Array		 &points () const { return myPoints; }
    const UT_Vector3Array		 &normals() const { return myNormals;}
    const UT_Vector3Array		 &colors() const { return myColors;}
    const UT_Vector3Array		 &uvs() const { return myUVs;}
    const UT_FloatArray			 &alphas() const { return myAlphas;}
    const UT_Vector4Array		 &trimPoints () const 
						{ return myPatches->myPoints; }
    const UT_Vector3Array		 &trimNormals () const 
						{ return myPatches->myNormals;}
    const UT_Vector3Array		 &trimColors() const 
						{ return myPatches->myColors;}
    const UT_Vector3Array		 &trimUVs() const 
						{ return myPatches->myUVs;}
    const UT_FloatArray			 &trimAlphas() const 
						{ return myPatches->myAlphas;}
protected:
    // Clear the cache data and other related info:
    virtual void initializeData(void);

    // If pushXform is set, the cache data has not been updated with the
    // latest xform matrix. This method makes sure the data _is_ updated,
    // and turns off the flag.
    virtual void assimilateXform(void);

private:
    const GA_ROHandleV3 &colorOffset (void) const { return myCdOffset; }
    const GA_ROHandleF	&alphaOffset (void) const { return myAlphaOffset; }
    const GA_ROHandleF	&textureOffset(void) const { return myTxtOffset; }

    UT_Vector4Array		 	 myPoints;
    UT_Vector3Array		 	 myNormals;
    UT_Vector3Array		 	 myColors;
    UT_Vector3Array		 	 myUVs;
    UT_FloatArray		 	 myAlphas;
    GU_CacheMode			 myMode;
    GU_TPTrimSurfCache			*myPatches;

    int			myUSpans, myVSpans;
    int			myUSize, myVSize;
    GA_ROHandleV3	myCdOffset;	// Color attribute
    GA_ROHandleF	myTxtOffset;	// Texture attribute
    GA_ROHandleF	myAlphaOffset;	// Alpha offset

    // Resize the attribute data and the attribute list (if necessary):
    void	 resizeAttributes(GEO_Detail *gdp, int ptattrib = 1);

    // Fill the array of float offsets based on what attributes are set.
    void	 buildFloatOffsets(GEO_Detail *gdp, GEO_AttributeHandleList *gu_farray);
    // Evaluate the surface because we know it's dirty. Return 0 if OK.
    int		 evaluateSurface(const GEO_TPSurf *surf);

    // Evaluate the normals of the whole evaluated surface.
    void	 evaluateNormals(const GEO_TPSurf *surf);
};

class GU_API GU_TPSurfCacheParms : public GU_CacheParms
{
public:
    		 GU_TPSurfCacheParms(void) {}
    virtual	~GU_TPSurfCacheParms(void);

    void		 surface(const GEO_TPSurf *surf) { myTPSurf = surf; }
    const GEO_TPSurf	*surface(void) const { return myTPSurf; }

private:
    const GEO_TPSurf	*myTPSurf;
};

SYS_DEPRECATED_POP_DISABLE()

#endif
