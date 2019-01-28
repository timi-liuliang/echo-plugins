/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_ConvertParms.h (GEO Library, C++)
 *
 * COMMENTS:    A series of classes for specifying options when converting
 *              between different primitive types.  This was formerly in
 *              the GU library as GU_ConvertParms.h.
 */

#ifndef __GEO_ConvertParms__
#define __GEO_ConvertParms__

#include "GEO_PrimTypeCompat.h"
#include "GEO_SurfaceType.h"
#include <GA/GA_ElementGroup.h>
#include <GA/GA_ElementWrangler.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_Vector3.h>
#include <stddef.h>

class GA_Detail;
class GEO_Detail;

enum GEO_ConvertStyle
{
    GU_CONVERT_LOD = 0, // use float level of detail
    GU_CONVERT_DIV = 1  // use integer divisions
};

// Conversion styles for metaballs.
enum GEO_MetaConvertStyleType
{
    GU_MetaConvertStyleStep = 0,
    GU_MetaConvertStyleDiv,
    GU_MetaConvertStyleLod,
    GU_MetaConvertStyleXsect,

    GU_MetaConvertStyleUndefined
};

class GEO_API GEO_ConvertMethod
{
public:
    GEO_ConvertMethod(GEO_ConvertStyle s = GU_CONVERT_LOD) : style(s) {}

    int		isLOD() const	{ return style == GU_CONVERT_LOD;  }
    int		isDiv() const	{ return style == GU_CONVERT_DIV;  }

    void	doLOD()		{ style = GU_CONVERT_LOD;  }
    void	doDiv()		{ style = GU_CONVERT_DIV;  }

    float	getULOD() const	   { return isLOD() ? u.lod    : u.div;    }
    float	getVLOD() const	   { return isLOD() ? v.lod    : v.div;    }
    float	getTrimLOD() const { return isLOD() ? trim.lod : trim.div; }

    void	setULOD(float lod)	{ doLOD(); u.lod = lod;    }
    void	setVLOD(float lod)	{ doLOD(); v.lod = lod;    }
    void	setTrimLOD(float lod)	{ doLOD(); trim.lod = lod; }

    int		validULOD() const
		{ return !isLOD() || (isLOD() && (u.lod > 0));    }
    int		validVLOD() const
		{ return !isLOD() || (isLOD() && (v.lod > 0));    }
    int		validTrimLOD() const
		{ return !isLOD() || (isLOD() && (trim.lod > 0)); }

    int 	getUDiv() const		{ return u.div;    }
    int 	getVDiv() const		{ return v.div;    }
    int 	getTrimDiv() const	{ return trim.div; }

    void	setUDiv(int div)	{ doDiv(); u.div = div;    }
    void	setVDiv(int div)	{ doDiv(); v.div = div;    }
    void	setTrimDiv(int div)	{ doDiv(); trim.div = div; }

    int		validUDiv() const	{ return isDiv() && (u.div >= 0);    }
    int		validVDiv() const	{ return isDiv() && (v.div >= 0);    }
    int		validTrimDiv() const	{ return isDiv() && (trim.div >= 0); }

    static const int GEO_EXTRA_LOD = 12;
    static int floatLODToIntLOD(float lod) { return int(lod * float(GEO_EXTRA_LOD)); }

private:
    typedef union
    {
        float lod;
        int div;
    } ConvertUnion;

    ConvertUnion u;
    ConvertUnion v;
    ConvertUnion trim;
    GEO_ConvertStyle style;
};

class GEO_API GEO_ConvertParms : private UT_NonCopyable
{
public:
     GEO_ConvertParms();
    ~GEO_ConvertParms() {}

    int			 orderu;
    int			 orderv;
    GEO_SurfaceType	 surface;

    /// @{
    /// Accessors
    int			uOrder() const	{ return orderu; }
    int			vOrder() const	{ return orderv; }
    void		setUOrder(int o)	{ orderu = o; }
    void		setVOrder(int o)	{ orderv = o; }
    GEO_SurfaceType	surfaceType() const	{ return surface; }
    void		setSurfaceType(GEO_SurfaceType t)	{ surface = t; }
    GEO_ConvertMethod	convertMethod() const	{ return method; }
    void		setConvertMethod(GEO_ConvertMethod m)	{ method = m; }
    bool		interpolateHull() const	{ return interpHull; }
    void		setInterpolateHull(bool b)	{ interpHull = b; }
    bool		trimSurface() const		{ return trimmed; }
    void		setTrimSurface(bool b)		{ trimmed = b; }
    bool		sharedFaces() const		{ return mySharedFaces; }
    void		setSharedFaces(bool b)		{ mySharedFaces = b; }
    bool		metaClustering() const	{ return myMetaClusterFlag; }
    void		setMetaClustering(bool b) { myMetaClusterFlag = b; }
    float		metaOffset() const	{ return myOffset; }
    void		setMetaOffset(fpreal f)	{ myOffset = f; }
    bool		keepGroups() const	{ return preserveGroups; }
    void		setKeepGroups(bool f)	{ preserveGroups = f; }
    const GEO_Detail	*sourceDetail() const	{ return mySourceDetail; }
    void		setSourceDetail(const GEO_Detail *g) { mySourceDetail=g;}
    GEO_Detail		*destDetail() const	{ return myDestDetail; }
    void		setDestDetail(GEO_Detail *g) { myDestDetail = g;}
    /// @}

    /// @{
    /// If set, use the feature, if not set use the base
    bool	pastedFeatureCoord() const	{ return featureCoord; }
    void	setPastedFeatureCoord(bool b)	{ featureCoord = b; }
    bool	pastedFeatureAttrib() const	{ return featureAttrib; }
    void	setPastedFeatureAttrib(bool b)	{ featureAttrib = b; }
    /// @}

    GEO_ConvertMethod	 method;
    int			 interpHull;
    int			 trimmed;	// trimmed spline surface
    bool		 mySharedFaces;	// Output shared faces from tets.

    int			 featureCoord;	// 1 feature, 0 base (for pasting)
    int			 featureAttrib;	// 1 feature, 0 base (for pasting)
    int			 myMetaClusterFlag;

    float		 myOffset;	// Used for biasing isosurfaces.

    int			 preserveGroups;

    // Warning: This primGroup is potentially modified by the conversion
    GA_PrimitiveGroup	*primGroup;

    const GEO_Detail    *mySourceDetail;
    GEO_Detail          *myDestDetail;

    // If the gdp passed in is null, then no group will be created, the
    // existing group (if any) will be returned.
    // The user of the convert parms is required to blow these groups away
    // TODO: Change these to take GA_Detail pointers instead
    GA_PointGroup	*getDeletePoints(GEO_Detail *gdp = nullptr);
    GA_PrimitiveGroup	*getDeletePrimitives(GEO_Detail *gdp = nullptr);
    void		 clearGroups()
			 {
			     myDeletePoints = nullptr;
			     myDeletePrimitives = nullptr;
			 }

    /// Returns a GA_ElementWranglerCache using mySourceDetail and myDestDetail
    GA_ElementWranglerCache &getWranglers()
    {
        UT_ASSERT(mySourceDetail && myDestDetail);
        return getWranglers(*(GA_Detail*)myDestDetail,
			     (const GA_Detail*)mySourceDetail);
    }
    GA_ElementWranglerCache &getGroupWranglers()
    {
        UT_ASSERT(mySourceDetail && myDestDetail);
	return getGroupWranglers(*(GA_Detail*)myDestDetail,
				  (const GA_Detail *)mySourceDetail);
    }

    GA_ElementWranglerCache &getWranglers(
				GA_Detail &dest, const GA_Detail *src = nullptr)
			{
			    cacheWranglers(myWranglers, dest, src, false);
			    return *myWranglers;
			}
    GA_ElementWranglerCache &getGroupWranglers(
				GA_Detail &dest, const GA_Detail *src = nullptr)
			{
			    cacheWranglers(myGroupWranglers, dest, src, true);
			    return *myGroupWranglers;
			}

    GA_PrimCompat::TypeMask	fromType() const { return myFromType; }
    void			setFromType(const GA_PrimCompat::TypeMask &t)
				    { myFromType = t; }

    GA_PrimCompat::TypeMask	toType() const { return myToType; }
    void			setToType(const GA_PrimCompat::TypeMask &t)
				    { myToType = t; }

private: // METHODS
    void		     cacheWranglers(
				    UT_UniquePtr<GA_ElementWranglerCache>
					&wranglers,
				    GA_Detail &dest,
				    const GA_Detail *src,
				    bool for_groups);

    GA_ElementWranglerCache *allocWranglerCache(
				    GA_Detail &dest,
				    const GA_Detail &src,
				    bool for_groups) const;

private: // DATA
    GA_PrimCompat::TypeMask	myFromType;
    GA_PrimCompat::TypeMask	myToType;


    GA_PointGroup	*myDeletePoints;	// Extra points to delete
    GA_PrimitiveGroup	*myDeletePrimitives;	// Extra primitives to delete

    UT_UniquePtr<GA_ElementWranglerCache> myWranglers;
    UT_UniquePtr<GA_ElementWranglerCache> myGroupWranglers;
};


inline void
GEO_ConvertParms::cacheWranglers(
	UT_UniquePtr<GA_ElementWranglerCache> &wranglers,
	GA_Detail &dest,
	const GA_Detail *src,
	bool for_groups)
{
    if (!src)
	src = &dest;

    // TODO: Include GEO_Detail.h?
    if (!wranglers)
	wranglers.reset(allocWranglerCache(dest, *src, for_groups));
    else if (!wranglers->isValid(dest, *src))
	wranglers->reset(dest, *src);
}

// Conversion parameters for special metaball conversions. See documentation
// of Convert Meta SOP for more details.
class GEO_API GEO_MetaConvertParms
{
public:
    GEO_MetaConvertParms();
    ~GEO_MetaConvertParms() {}

    bool myDoAdjust;
    UT_Vector3 myStepSizes;
    int myDivs[3];
    fpreal  myLod;
    bool myClustering;
    int myNumSections;
    int myAxis;
    int myMaxDepth;
    bool myAccurateNormals;
    bool myMakePolySoup;

    GEO_MetaConvertStyleType myStyle;
};

#endif
