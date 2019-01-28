/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_VolumeRasterize.h ( GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_VolumeRasterize__
#define __GU_VolumeRasterize__

#include "GU_API.h"

#include <UT/UT_Array.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_VoxelArray.h>
#include <UT/UT_ThreadedAlgorithm.h>
#include <CVEX/CVEX_Context.h>

#include <GA/GA_Handle.h>
#include <GA/GA_OffsetList.h>
#include <GA/GA_AttributeInstanceMatrix.h>
#include "GU_PrimVolume.h"
#include "GU_PrimVDB.h"

class GU_Detail;
class GA_PointGroup;

class GU_API GU_VolumeRasterizeBinding
{
public:
    GU_VolumeRasterizeBinding(const char *name, CVEX_Type type)
	: myName(UT_String::ALWAYS_DEEP, name)
	, myType(type)
	, myData(0)
	, myIsFilled(0)
    {
    }
    virtual ~GU_VolumeRasterizeBinding() 
    {
	free(myData);
    }

    const char		*name() const { return myName; }
    CVEX_Type		 type() const { return myType; }

    void		 alloc(exint entries);

    void		*data() const { return myData; }
    float		*fdata() const { return (float *) myData; }
    int			*idata() const { return (int *) myData; }
    UT_Vector3		*v3data() const { return (UT_Vector3 *) myData; }
    UT_Vector4		*v4data() const { return (UT_Vector4 *) myData; }
    UT_Matrix3		*m3data() const { return (UT_Matrix3 *) myData; }
    UT_Matrix4		*m4data() const { return (UT_Matrix4 *) myData; }

    bool		myIsFilled;

private:
    UT_String		myName;
    CVEX_Type		myType;
    void		*myData;
};

class GU_API GU_VolumeRasterizePrimList
{
public:
    GU_VolumeRasterizePrimList() {}
    virtual ~GU_VolumeRasterizePrimList() {}


    // All of the following functions must be threadsafe.
    virtual exint		numPrimitives() const = 0;

    virtual void		getBBox(exint prim, 
					UT_BoundingBox &bbox) const = 0;

    // Do we support validating bounding boxes?
    virtual bool		canValidate(exint prim) const { return false; }

    // Query if this world space box has any voxels worth looking at.
    virtual bool		validateBBox(exint prim, const UT_BoundingBox &bbox) const { return true; }

    // threaddata is a pointer to a void * that you can initialize to
    // point to your allocated thread local data
    virtual void		initThread(const UT_JobInfo &info, void **threaddata) const {}
    // This is the value of the threaddata that you set on initThread.
    virtual void		endThread(const UT_JobInfo &info, void *threaddata) const {}

    /// Initialize a block binding information.  This is responsible
    /// for passing to the rasterizer information about the primitives.
    /// It takes a worldspace location in P which is present as one
    /// of the bindings that is already filled out.
    virtual void		fillBindings(const UT_ExintArray &primnum, 
		    UT_Array<GU_VolumeRasterizeBinding> &bindings) const = 0;

    /// Returns a list of bindings that we know how to write to.
    virtual void		proposeBindings(UT_Array<GU_VolumeRasterizeBinding> &bindings) const = 0;
};

class GU_API GU_VolumeRasterizePointAsPrimList 
		: public GU_VolumeRasterizePrimList
{
public:
    GU_VolumeRasterizePointAsPrimList(const GU_Detail *gdp, const GA_OffsetList &ptlist, bool scaletexturespace);
    virtual ~GU_VolumeRasterizePointAsPrimList();

    // Rasterizes pscale sized spheres, will expand them by *= relative
    // and += absolute.
    void			setDisplaceBounds(fpreal relative, fpreal absolute);

    virtual exint		numPrimitives() const;

    virtual void		getBBox(exint prim, 
					UT_BoundingBox &bbox) const;

    virtual bool		canValidate(exint prim) const;
    virtual bool		validateBBox(exint prim, const UT_BoundingBox &bbox) const;

    /// Initialize a block binding information.  This is responsible
    /// for passing to the rasterizer information about the primitives.
    /// It takes a worldspace location in P which is present as one
    /// of the bindings that is already filled out.
    virtual void		fillBindings(const UT_ExintArray &primnum, 
		    UT_Array<GU_VolumeRasterizeBinding> &bindings) const;
    /// Returns a list of bindings that we know how to write to.
    virtual void		proposeBindings(UT_Array<GU_VolumeRasterizeBinding> &bindings) const;

    /// Computes the transform world position to texture coordinates.
    void			getInverseXform(exint prim, UT_Matrix4 &xform) const;
    /// Computes the tranform from texture to world
    void			getXform(exint prim, UT_Matrix4 &xform) const;
private:
    const GU_Detail		*myGdp;
    GA_OffsetList		myPointList;
    GA_ROHandleF		myPscaleH;
    GA_ROHandleF		myPaddingAbsH;
    GA_ROHandleF		myPaddingRelH;
    GA_AttributeInstanceMatrix	myInstanceMatrix;
    float			myScaleAbs, myScaleRel;
};

class GU_API GU_VolumeRasterizeOutput
{
public:
    enum CompositeType {
	MIX_ADD,
	MIX_MAX,
	MIX_MIN
    };
    GU_VolumeRasterizeOutput( const char *name,
			     CompositeType comp,
			     CVEX_Type type);

    GU_PrimVDB  *vdb(int i, int j) const { return myVDB[i][j]; }
    GU_PrimVolume  *vol(int i, int j) const { return myVol[i][j]; }
    void	setField(int i, int j, GU_PrimVolume *vol, GU_PrimVDB *vdb)
    {
	myVDB[i][j] = vdb;
	myVol[i][j] = vol;
	if (!i && !j)
	{
	    if (vdb)
		myIdxXform = vdb->getIndexSpaceTransform();
	    if (vol)
		myIdxXform = vol->getIndexSpaceTransform();
	}
    }
    const char	*name() const { return (const char *) myName; }
    CompositeType comptype() const { return myComp; }
    CVEX_Type type() const { return myType; }

    inline UT_Vector3		 posToIndex(UT_Vector3 pos) const
    { return myIdxXform.toVoxelSpace(pos); }
    inline UT_Vector3		 indexToPos(UT_Vector3 index) const
    { return myIdxXform.fromVoxelSpace(index); }

    inline void posToIndex(UT_BoundingBox &box) const
    { return myIdxXform.toVoxelSpace(box); }
    inline void indexToPos(UT_BoundingBox &box) const
    { return myIdxXform.fromVoxelSpace(box); }

private:
    UT_String		myName;
    GU_PrimVDB		*myVDB[3][3];
    GU_PrimVolume	*myVol[3][3];
    CompositeType	myComp;
    CVEX_Type		myType;
public:
    GEO_PrimVolumeXform	myIdxXform;
};

class GU_API GU_VolumeRasterize
{
public:
    GU_VolumeRasterize();
    virtual ~GU_VolumeRasterize();

    class		BlockData;
    class		WorkUnit;
    class		WorkJob;

    void		bindOutput(const char *name,
				    GU_VolumeRasterizeOutput::CompositeType comptype,
				    GU_PrimVolume *vol,
				    GU_PrimVDB *vdb);

    void		setRasterizer(const char *vexscript,
				    int vexcwdopid);

    void		rasterize(const GU_VolumeRasterizePrimList &primlist);

    const char 		*getVexErrors() const;
    const char 		*getVexWarnings() const;

    void		setOpCaller(UT_OpCaller *opcaller) { myOpCaller = opcaller; }
    void		setTime(fpreal time, fpreal frame, fpreal timeinc)
			{ myTime = time; myFrame = frame; myTimeInc = timeinc; }
    void		setSampling(int samples, float seed)
			{ mySamples = samples; mySeed = seed; }
    void		setPreMult(bool premult, bool unpremult)
			{ myDoPreMult = premult; myDoUnPreMult = unpremult; }

    bool		isTimeDependent() const { return myTimeDependent; }

private:
    void		processBlock(const GU_VolumeRasterizePrimList &primlist, BlockData &data);
    void		rasterizeBlock(BlockData &data);

    THREADED_METHOD3(GU_VolumeRasterize, vox->numTiles() > 1,
		    compositeVoxels,
		    UT_VoxelArrayF *, vox,
		    const UT_VoxelArrayF *, src,
		    GU_VolumeRasterizeOutput::CompositeType , comptype);
    void	compositeVoxelsPartial(UT_VoxelArrayF *dst,
				    const UT_VoxelArrayF *src,
				    GU_VolumeRasterizeOutput::CompositeType comptype,
				    const UT_JobInfo &info);

    THREADED_METHOD2(GU_VolumeRasterize, vox->numTiles() > 1,
		    unpremultiplyVoxels,
		    UT_VoxelArrayF *, vox,
		    const UT_VoxelArrayF *, densvox)
    void	unpremultiplyVoxelsPartial(UT_VoxelArrayF *dst,
				    const UT_VoxelArrayF *densvox,
				    const UT_JobInfo &info);

    THREADED_METHOD3(GU_VolumeRasterize, true,
			doRasterize,
			const GU_VolumeRasterizePrimList &, primlist,
			const BlockData &, refblock,
			UT_Array<BlockData *> &, blocks);
    void	doRasterizePartial(
				    const GU_VolumeRasterizePrimList &primlist, 
				    const BlockData &refblock,
				    UT_Array<BlockData *> &blocks,
				    const UT_JobInfo &info);

    UT_String				myVexScript;
    int					myVexCWDNodeId;
    UT_OpCaller				*myOpCaller;
    UT_WorkArgs				*myVexArgs;
    UT_StringArray			myVexErrorList, myVexWarningList;
    mutable UT_String			myVexErrors, myVexWarnings;
    fpreal				myTime, myTimeInc;
    fpreal				myFrame;
    bool				myTimeDependent;

    int					mySamples;
    float				mySeed;
    bool				myDoPreMult;
    bool				myDoUnPreMult;;

    UT_Array<GU_VolumeRasterizeOutput>	myOutputs;
};

#endif

