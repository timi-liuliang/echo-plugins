/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_PrimVolume.h ( GEO Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GEO_PrimVolume__
#define __GEO_PrimVolume__

#include <UT/UT_BoundingRect.h>
#include <UT/UT_VoxelArray.h>

#include "GEO_API.h"
#include "GEO_Primitive.h"
#include "GEO_VolumeOptions.h"

class UT_JSONWriter;
class UT_JSONParser;
class GA_SaveMap;
class GA_LoadMap;
class CE_Grid;

///
/// Stores the transform associated with a volume, allowing the
/// to/from code to be inlined outside of this library.
///
class GEO_API GEO_PrimVolumeXform
{
public:
    // Converts from world space to [0,1] space.
    inline UT_Vector3 toVoxelSpace(UT_Vector3 pos) const
    {
	pos -= myCenter;
	pos *= myInverseXform;

	// Undo any taper effect.
	if (myHasTaper)
	{
	    fpreal		zscale = (1 - pos.z()) * 0.5f;
	    fpreal		taperx = 1 + (-1 + myTaperX) * zscale;
	    fpreal		tapery = 1 + (-1 + myTaperY) * zscale;

	    if (taperx == 0.0f)
		pos.x() = 0.0;
	    else
		pos.x() /= taperx;
	    if (tapery == 0.0f)
		pos.y() = 0.0;
	    else
		pos.y() /= tapery;
	}

	// This gets us a value in the -1 to 1 box.  We need to evaluate
	// in the 0 to 1 box, however.
	pos.x() += 1;
	pos.y() += 1;
	pos.z() += 1;
	pos *= 0.5;

	return pos;
    }

    // Converts from [0,1] space over the voxels to world space
    inline UT_Vector3 fromVoxelSpace(UT_Vector3 pos) const
    {
	// convert to -1 to 1 box.
	pos.x() -= 0.5;
	pos.y() -= 0.5;
	pos.z() -= 0.5;
	pos *= 2;

	// Apply the taper effect.
	if (myHasTaper)
	{
	    fpreal		zscale = (1 - pos.z()) * 0.5f;
	    fpreal		taperx = 1 + (-1 + myTaperX) * zscale;
	    fpreal		tapery = 1 + (-1 + myTaperY) * zscale;

	    pos.x() *= taperx;
	    pos.y() *= tapery;
	}
	// Convert to world space.
	pos *= myXform;
	pos += myCenter;
	
	return pos;
    }

    inline void toVoxelSpace(UT_BoundingBox &box) const
    {
	int		i;
	UT_Vector3	v[8] = {
	    UT_Vector3(box.xmin(), box.ymin(), box.zmin()),
	    UT_Vector3(box.xmin(), box.ymin(), box.zmax()),
	    UT_Vector3(box.xmin(), box.ymax(), box.zmin()),
	    UT_Vector3(box.xmin(), box.ymax(), box.zmax()),
	    UT_Vector3(box.xmax(), box.ymin(), box.zmin()),
	    UT_Vector3(box.xmax(), box.ymin(), box.zmax()),
	    UT_Vector3(box.xmax(), box.ymax(), box.zmin()),
	    UT_Vector3(box.xmax(), box.ymax(), box.zmax()),
	};

	box.initBounds();
	for (i = 0; i < 8; i++)
	{
	    box.enlargeBounds(toVoxelSpace(v[i]));
	}
    }

    inline void fromVoxelSpace(UT_BoundingBox &box) const
    {
	int		i;
	UT_Vector3	v[8] = {
	    UT_Vector3(box.xmin(), box.ymin(), box.zmin()),
	    UT_Vector3(box.xmin(), box.ymin(), box.zmax()),
	    UT_Vector3(box.xmin(), box.ymax(), box.zmin()),
	    UT_Vector3(box.xmin(), box.ymax(), box.zmax()),
	    UT_Vector3(box.xmax(), box.ymin(), box.zmin()),
	    UT_Vector3(box.xmax(), box.ymin(), box.zmax()),
	    UT_Vector3(box.xmax(), box.ymax(), box.zmin()),
	    UT_Vector3(box.xmax(), box.ymax(), box.zmax()),
	};

	box.initBounds();
	for (i = 0; i < 8; i++)
	{
	    box.enlargeBounds(fromVoxelSpace(v[i]));
	}
    }

    /// Returns the *un-tapered* matrix4 corresponding to fromVoxelSpace().
    /// @{
    void getTransform4(UT_Matrix4F &matx) const;
    void getTransform4(UT_Matrix4D &matx) const;
    /// @}

    /// Compute the size for this transform (relative to myCenter)
    UT_Vector3R computeSize() const;

    // NOTE: This ordering is saved in .hip files!
    enum SamplingType
    {
	NON_SQUARE = 0,
	X_AXIS,
	Y_AXIS,
	Z_AXIS,
	MAX_AXIS,
	BY_SIZE
    };

    /// Convert the "uniformsamples" parm menu item index to a sampling type
    static SamplingType	    samplingType(int i)
				{ return SamplingType(i); }

    /// Compute the resolution (and size) from a common set of parameters.
    /// If twod_axis is one of [0,1,2], then that axis clamped to a divisions
    /// of 1 and a size of 0.  The number of divisions is returned with size
    /// set to div_size multiplied by divisions before the z_div_scale.
    static UT_Vector3R	    computeResolution(
				SamplingType sampling_type,
				const UT_Vector3R &non_uniform_divs,
				fpreal uniform_divs,
				fpreal div_size,
				fpreal z_div_scale,
				UT_Vector3R &size_inout,
				int twod_axis = -1);

    /// Compute a space transform from center, size, and taper
    static GEO_PrimVolumeXform frustumSpaceTransform(
				    const UT_Vector3R &size,
				    const UT_Vector3R &center,
				    fpreal taper_x, fpreal taper_y);

    /// Compute a space transform from camera settings.
    /// The ortho_zoom parameter is only used when is_ortho = true.
    static GEO_PrimVolumeXform cameraFrustum(
				    fpreal focal,
				    fpreal aperture,
				    const UT_Vector2R &image_res,
				    fpreal pixel_aspect,
				    bool is_ortho,
				    fpreal ortho_zoom,
				    fpreal clip_near,
				    fpreal clip_far,
				    bool use_cam_window,
				    const UT_BoundingRectR &cam_window,
				    const UT_BoundingRectR &cam_crop,
				    const UT_BoundingRectR &window,
				    const UT_Matrix4R &post_xform,
				    const GEO_Detail *match_src);

public:
    // @{
    void	 save(std::ostream &os, bool binary = false) const;
    bool	 load(UT_IStream &is);
    // @}

    void		init()
			{
			    myCenter.assign(0, 0, 0);
			    myXform.identity();
			    myInverseXform.identity();
			    myHasTaper = false;
			    myTaperX = 1.0;
			    myTaperY = 1.0;
			}

    bool		operator==(const GEO_PrimVolumeXform &x) const
			{
			    return
				myXform == x.myXform &&
				myCenter == x.myCenter &&
				myHasTaper == x.myHasTaper &&
				myTaperX == x.myTaperX &&
				myTaperY == x.myTaperY;
			}

    UT_Matrix3		myXform, myInverseXform;
    UT_Vector3		myCenter;
    bool		myHasTaper;
    float		myTaperX, myTaperY;
};

class GEO_API GEO_PrimVolume : public GEO_Primitive
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    GEO_PrimVolume(GEO_Detail *d, GA_Offset offset = GA_INVALID_OFFSET);

    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    virtual ~GEO_PrimVolume();

public:
//
//  Methods common to all primitives.
    virtual int		 evaluateNormalVector(UT_Vector3 &nml, float u,
					float v = 0, float w = 0) const;
    virtual int		 getBBox(UT_BoundingBox *bbox) const;
    virtual void	 addToBSphere(UT_BoundingSphere *bsphere) const;
    virtual void	 enlargePointBounds(UT_BoundingBox &box) const;
    /// @{
    /// Enlarge a bounding box by the bounding box of the primitive.  A
    /// return value of false indicates an error in the operation, most
    /// likely an invalid P.  For any attribute other than the position
    /// these methods simply enlarge the bounding box based on the vertex.
    virtual bool	 enlargeBoundingBox(UT_BoundingRect &b,
					const GA_Attribute *P) const;
    virtual bool	 enlargeBoundingBox(UT_BoundingBox &b,
					const GA_Attribute *P) const;
    /// @}
    /// Enlarge a bounding sphere to encompass the primitive.  A return value
    /// of false indicates an error in the operation, most likely an invalid
    /// P.  For any attribute other than the position this method simply
    /// enlarges the sphere based on the vertex.
    virtual bool	 enlargeBoundingSphere(UT_BoundingSphere &b,
					const GA_Attribute *P) const;
    /// For a volume the barycenter is the same as the point.
    virtual UT_Vector3	 baryCenter() const;
    virtual UT_Vector3	 computeNormal() const;
    virtual bool	 saveH9(std::ostream &os, bool binary,
			   const UT_Array<GA_AttribSaveDataH9> &prim_attribs,
			   const UT_Array<GA_AttribSaveDataH9> &vtx_attribs)
			    const;
    virtual bool	 loadH9(UT_IStream &is,
			   const UT_Array<GA_AttribLoadDataH9> &prim_attribs,
			   const UT_Array<GA_AttribLoadDataH9> &vtx_attribs);

    bool		saveVoxelArray(UT_JSONWriter &w,
				const GA_SaveMap &map) const;
    bool		loadVoxelArray(UT_JSONParser &p,
				const GA_LoadMap &map);

    bool		loadRes(const UT_JSONValue &jval);
    bool		saveBorder(UT_JSONWriter &w,
				const GA_SaveMap &map) const;
    bool		loadBorder(UT_JSONParser &p,
				const GA_LoadMap &map);
    bool		saveCompression(UT_JSONWriter &w,
				const GA_SaveMap &map) const;
    bool		loadCompression(UT_JSONParser &p,
				const GA_LoadMap &map);
    bool		saveVisualization(UT_JSONWriter &w,
				const GA_SaveMap &map) const;
    bool		loadVisualization(UT_JSONParser &p,
				const GA_LoadMap &map);

    /// @{
    /// Methods to save/load shared voxel data
    static const int	theSharedVoxelMagic=('V'<<24)|('o'<<16)|('x'<<8)|('l');
    virtual bool	registerSharedLoadData(int dtype, GA_SharedDataHandlePtr data);
    virtual bool	saveSharedLoadData(UT_JSONWriter &w,
				GA_SaveMap &save, GA_GeometryIndex* geometryIndex) const;
    bool		getSharedVoxelKey(UT_WorkBuffer &key) const;
    static GA_PrimitiveDefinition::SharedDataLoader	*
			allocateSharedDataLoader();
    /// @}

    // Transforms the matrix associated with this primitive.  The
    // translate component is ignored: Translate the vertices of
    // the primitive to translate the primitive.
    // This only works with quadrics (sphere, tube, metaballs) and volumes.
    virtual void		 transform(const UT_Matrix4 &mat);

    virtual void	reverse() {}
    virtual GEO_Primitive	*copy(int preserve_shared_pts = 0) const;
    virtual void	copyPrimitive(const GEO_Primitive *src);
    virtual void	copyUnwiredForMerge(const GA_Primitive *src,
					    const GA_MergeMap &map);
#if !GA_PRIMITIVE_VERTEXLIST
    virtual void	addPointRefToGroup(GA_PointGroup &grp) const;

    virtual bool	vertexApply(bool (*apply)(GA_Offset vtx, void *),
				     void  *data = 0) const;

    virtual GA_Size	getVertexCount() const;
    virtual GA_Offset	getVertexOffset(GA_Size index) const;
#endif
#if GA_PRIMITIVE_VERTEXLIST
    using GEO_Primitive::getVertexOffset;
#endif
    using GEO_Primitive::getPointOffset;
    using GEO_Primitive::getPos3;
    using GEO_Primitive::setPos3;

    SYS_FORCE_INLINE
    GA_Offset getVertexOffset() const
    {
        return getVertexOffset(0);
    }
    SYS_FORCE_INLINE
    GA_Offset getPointOffset() const
    {
        return getPointOffset(0);
    }
    SYS_FORCE_INLINE
    UT_Vector3 getPos3() const
    {
        return getPos3(0);
    }
    SYS_FORCE_INLINE
    void setPos3(const UT_Vector3 &pos)
    {
        return setPos3(0, pos);
    }

    // Take the whole set of points into consideration when applying the
    // point removal operation to this primitive. The method returns 0 if
    // successful, -1 if it failed because it would have become degenerate,
    // and -2 if it failed because it would have had to remove the primitive
    // altogether.
    virtual int		 detachPoints (GA_PointGroup &grp);

    /// Before a point is deleted, all primitives using the point will be
    /// notified.  The method should return "false" if it's impossible to
    /// delete the point.  Otherwise, the vertices should be removed.
    virtual GA_DereferenceStatus        dereferencePoint(GA_Offset point,
						bool dry_run=false);
    virtual GA_DereferenceStatus        dereferencePoints(
						const GA_RangeMemberQuery &pt_q,
						bool dry_run=false);

    virtual bool	 isDegenerate() const;

    // Map the normalized length (distance value [0,1]) parameter to the unit 
    // parameterization of the primitve
    virtual void	 unitLengthToUnitPair(float  ulength, float  vlength,
					      float &uparm,  float &vparm)const;
    virtual void	 unitLengthToUnitPair(float  ulength, float  vlength,
					      float &uparm,  float &vparm, float tolerance)const;

    virtual void	 unitToUnitLengthPair(float  uparm,   float  vparm,
					      float &ulength, float &vlength)
									const;

    virtual fpreal	 calcVolume(const UT_Vector3 &refpt) const;
    virtual fpreal	 calcArea() const;

//
//  Methods unique to PrimVolume.

#if GA_PRIMITIVE_VERTEXLIST
    SYS_FORCE_INLINE
    void setVertexPoint(GA_Offset pt)
    {
        wireVertex(getVertexOffset(), pt);
    }
#else
    void		 setVertexPoint(GA_Offset pt)
			 {
			     wireVertex(myVertex, pt);
			 }
#endif

    /// This method assigns a preallocated vertex to the quadric, optionally
    /// creating the topological link between the primitive and new vertex.
    void		 assignVertex(GA_Offset new_vtx, bool update_topology);

    // Have we been deactivated and stashed?
    virtual void	 stashed(bool beingstashed,
					GA_Offset offset = GA_INVALID_OFFSET);

    const UT_Matrix3	&getTransform(void) const { return myXform; }
    void		 setTransform(const UT_Matrix3 &m) 
			 { myXform = m; 
			   myInverseXform = m;
			   myInverseXform.invert();
			 }

    void		 getTransform4(      UT_Matrix4 &matx) const;
    void		 getTransform4(      UT_DMatrix4 &matx) const;
    void		 setTransform4(const UT_Matrix4 &matx);
    void		 setTransform4(const UT_DMatrix4 &matx);

    virtual void	 getLocalTransform(UT_Matrix3D &x) const;
    virtual void	 setLocalTransform(const UT_Matrix3D &x);

    /// Converts from world space to local space.
    const UT_Matrix3	&getInverseTransform() const { return myInverseXform; }
    void		 getInverseTransform4(UT_Matrix4 &matx) const;

    float		 getTaperX(void) const { return myTaperX; }
    void		 setTaperX(float t) { myTaperX = t; }
    float		 getTaperY(void) const { return myTaperY; }
    void		 setTaperY(float t) { myTaperY = t; }

    /// True if the two volumes have same resolution and map the
    /// same indices to the same positions.
    bool		 isAligned(const GEO_PrimVolume *vol) const;

    /// True if we are aligned with the world axes.  Ie, all our
    /// off diagonals are zero and our diagonal is positive.
    bool		 isWorldAxisAligned() const;

    /// Returns the POD class which can convert to and from
    /// 0..1 voxel space coordinates.
    GEO_PrimVolumeXform	 getSpaceTransform() const;
    void 		 setSpaceTransform(GEO_PrimVolumeXform xform);

    /// Returns the POD class which can convert to and from
    /// voxel index space coordinates.
    GEO_PrimVolumeXform  getIndexSpaceTransform() const;
    GEO_PrimVolumeXform  getIndexSpaceTransform(const UT_VoxelArrayF &vox) const;

    /// Converts from world space to 0..1 voxel space.
    UT_Vector3		 toVoxelSpace(const UT_Vector3 &pos) const;
    /// Converts from 0..1 voxel space to world space.
    UT_Vector3		 fromVoxelSpace(const UT_Vector3 &pos) const;

    /// Converts from world space to 0..1 voxel space.
    void		 toVoxelSpace(UT_BoundingBox &box) const;
    /// Converts from 0..1 voxel space to world space.
    void		 fromVoxelSpace(UT_BoundingBox &box) const;

    /// Copies the given voxel array and makes it our own voxel array.
    void		 setVoxels(const UT_VoxelArrayF *vox);
    void		 setVoxels(UT_VoxelArrayHandleF handle);

    /// Takes ownership of the voxel array, caller should not refer
    /// to vox any more.
    void		 stealVoxels(UT_VoxelArrayF *vox);

    /// Returns a handle to a voxel array containing our data.
    /// This is should be thought of a copy of the data - changing
    /// it will not change the underlying data, casting this to
    /// a write handle will write to the newly created handle, not
    /// the one stored in this volume.
    UT_VoxelArrayHandleF	getVoxelHandle() const;

    /// Returns a voxel handle without trying to load the shared data
    /// This should only be used for the loader
    /// DO NOT USE! IF YOU THINK YOU SHOULD YOUR PROBABLY WRONG
    UT_VoxelArrayHandleF	getHandleToVoxelsWithoutLoading() const
    {
	return myVoxelHandle;
    }

    /// This is a handle that you can write to and affect the volume.
    UT_VoxelArrayWriteHandleF	getVoxelWriteHandle();

    /// Convert an index in the voxel array into the corresponding worldspace 
    /// location
    bool		 indexToPos(int x, int y, int z, UT_Vector3 &pos) const;
    void		 findexToPos(UT_Vector3 index, UT_Vector3 &pos) const;

    /// Returns true if the given point is entirely inside the volume's
    /// definition, ie, if posToIndex would return true.
    bool		 isInside(UT_Vector3 pos) const;

    /// Returns true only if strictly inside.  This means only actual
    /// voxel samples will be used for interpolation, so the boundary
    /// conditions will be unused
    bool		 isInsideStrict(UT_Vector3 pos) const;
    /// By passing in a specific read handle, we can accelerate
    /// isInsideStrict()
    bool         isInsideStrict(const UT_Vector3 &opos, const UT_VoxelArrayReadHandleF &vox) const;

    /// Convert a 3d position into the closest index value.  Returns
    /// false if the resulting index was out of range (but still sets it)
    bool		 posToIndex(UT_Vector3 pos, int &x, int &y, int &z) const;	
    bool		 posToIndex(UT_Vector3 pos, UT_Vector3 &index) const;	

    /// Evaluate the voxel value at the given world space position.
    fpreal		 getValue(const UT_Vector3 &pos) const;
    void		 getValues(float *f, int stride, const UT_Vector3 *p, int num) const;
    // Only works with scalar grids.
    UT_Vector3		 getGradient(const UT_Vector3 &pos) const;
    UT_Vector3		 getGradient(const UT_Vector3 &pos, const UT_VoxelArrayReadHandleF &handle) const;

    /// By passing in a specific read handle and inverse transform
    /// we can accelerate the getValue()
    fpreal		 getValue(const UT_Vector3 &pos, const UT_VoxelArrayReadHandleF &handle) const;

    /// Evaluate the specific voxel indexed from 0,0,0.
    fpreal		 getValueAtIndex(int ix, int iy, int iz) const;
    void		 getValuesAtIndices(float *f, int stride, const int *ix, const int *iy, const int *iz, int num) const;
    void		 getValuesAtIndices(int *f, int stride, const int *ix, const int *iy, const int *iz, int num) const;
    /// Returns the resolution of the voxel array.
    void		 getRes(int &rx, int &ry, int &rz) const;
    /// Computes the voxel diameter by taking a step in x, y, and z
    /// converting to world space and taking the length of that vector.
    fpreal		 getVoxelDiameter() const;

    /// Returns the length of the voxel when you take an x, y, and z step
    UT_Vector3		 getVoxelSize() const;

    /// Computes the total density of the volume, scaled by
    /// the volume's size.  Negative values will be ignored.
    fpreal		 calcPositiveDensity() const;

    /// Compute useful aggregate properties of the volume.
    fpreal		 calcMinimum() const;
    fpreal		 calcMaximum() const;
    fpreal		 calcAverage() const;

    /// Determines if we should be treated as an SDF.  This means
    /// our function will continue to increase outside of the bounding
    /// box according to the distance to the bounding box.
    bool		 isSDF() const { return myIsSDF; }

    /// Determine our orientation if we are to be considered a heightfield.
    /// Returns false if we shouldn't be treated as a heightfield.
    bool		 computeHeightFieldProperties(int &a1, int &a2, int &axis, fpreal &scale) const;
    bool		 computeHeightFieldProperties(int &a1, int &a2, int &axis, fpreal &scale, const UT_VoxelArrayF &vox, const GEO_PrimVolumeXform &indexxform) const;

    /// Get the border options in terms of GEO's values.
    static const char		*getBorderToken(GEO_VolumeBorder border);
    static GEO_VolumeBorder	 getBorderEnum(const char *token,
				    GEO_VolumeBorder def=GEO_VOLUMEBORDER_STREAK);
    void		 setBorder(GEO_VolumeBorder border, fpreal val);
    GEO_VolumeBorder	 getBorder() const;
    fpreal		 getBorderValue() const;

    /// Control the compression of these objects.
    fpreal		 getCompressionTolerance() const;
    void		 setCompressionTolerance(fpreal tol);
    void		 recompress();

    /// Control how we display this in the viewport
    static const char		*getVisualizationToken(GEO_VolumeVis vis);
    static GEO_VolumeVis	 getVisualizationEnum(const char *vis,
					GEO_VolumeVis def=GEO_VOLUMEVIS_SMOKE);
    const GEO_VolumeOptions	&getVisOptions() const  { return myVis; }
    void		 setVisualization(GEO_VolumeVis vis, fpreal iso, fpreal density);
    fpreal		 getVisIso() const { return myVis.myIso; }
    fpreal		 getVisDensity() const { return myVis.myDensity; }
    GEO_VolumeVis	 getVisualization() const { return myVis.myMode; }

    virtual const GA_PrimitiveJSON	*getJSON() const;

    /// Voxel traverser.  This serializes the voxels into a linear array of
    /// scalar data.
    class serialize
    {
    public:
	serialize()
	    : myVoxels()
	    , myBorder(0)
	    , myXres(0)
	    , myYres(0)
	    , myZres(0)
	    , myEntries(0)
	    , myCurr(0)
	{}
	serialize(const serialize &src)
	    : myVoxels(src.myVoxels)
	    , myBorder(src.myBorder)
	    , myXres(src.myXres)
	    , myYres(src.myYres)
	    , myZres(src.myZres)
	    , myEntries(src.myEntries)
	    , myCurr(src.myCurr)
	{}
	serialize	&operator=(const serialize &src)
			{
			     myVoxels = src.myVoxels;
			     myBorder = src.myBorder;
			     myXres = src.myXres;
			     myYres = src.myYres;
			     myZres = src.myZres;
			     myEntries = src.myEntries;
			     myCurr = src.myCurr;
			     return *this;
			}
	/// Random access of a voxel value
	fpreal		getVoxel(exint index) const
			{
			    if (index >= 0 && index < myEntries)
			    {
				exint	x, y, z;
				x = index % myXres;
				index = (index - x) / myXres;
				y = index % myYres;
				index = (index - y) / myYres;
				z = index % myZres;
				return (*myVoxels)(x, y, z);
			    }
			    return myBorder;
			}

	/// @{
	/// Iterator interface
	bool		atEnd() const	{ return myCurr >= myEntries; }
	void		rewind()	{ myCurr = 0; }
	void		advance()	{ myCurr++; }
	serialize	&operator++()	{ advance(); return *this; }
	/// No post increment as it is harmful.
	/// @}
	/// Iterator access methods
	exint		entries() const	{ return myEntries; }
	exint		index() const	{ return myCurr; }
	fpreal		voxel() const	{ return getVoxel(myCurr); }
    private:
	serialize(const GEO_PrimVolume &prim)
	    : myVoxels(prim.getVoxelHandle())
	    , myCurr(0)
	{
	    prim.getRes(myXres, myYres, myZres);
	    myBorder = prim.getBorderValue();
	    myEntries = myXres*myYres*myZres;
	}
	UT_VoxelArrayReadHandleF	myVoxels;
	fpreal				myBorder;
	exint				myCurr, myEntries;
	int				myXres, myYres, myZres;
	friend class GEO_PrimVolume;
    };
    serialize	getSerialize() const	{ return serialize(*this); }


    /// Acquire a CE grid and cache it on the GPU.  If marked for
    /// writing, the CPU version will be overwritten.
    /// Note that the getVoxelHandle does *NOT* auto-flush these!
    /// NOTE: If someone else fetches a non-read grid, and you fetch it
    /// as a read grid, you will not get any copied data.
    CE_Grid		*getCEGrid(bool read, bool write);

    /// Any modified CE cache on the GPU will be copied back to the
    /// CPU.  Will leave result on GPU.
    virtual void		flushCEWriteCaches();

    /// Remove all CE caches from the GPU, possibly writing back
    /// if necessary.
    virtual void		flushCECaches();

protected:
    static GA_PrimitiveFamilyMask 	buildFamilyMask()
					    { return GA_FAMILY_NONE; }

#if !GA_PRIMITIVE_VERTEXLIST
    virtual void	clearForDeletion();
#endif

#if !GA_PRIMITIVE_VERTEXLIST
    /// Defragmentation
    virtual void	swapVertexOffsets(const GA_Defragment &defrag);
#endif

    GA_DECLARE_INTRINSICS(GA_NO_OVERRIDE)

    virtual bool 	savePrivateH9(std::ostream &os, bool binary) const;
    virtual bool 	loadPrivateH9(UT_IStream &is);

    // A versioned loading method.
    bool		loadVoxelDataH9(UT_IStream &is,
					UT_VoxelArrayWriteHandleF voxels,
					int version);

    GA_Offset vertexPoint() const
    { return getPointOffset(); }

    /// Gets the handle to our voxels
    UT_VoxelArrayHandleF& getMyVoxelHandle() const;

    // All accesses of myVoxelHandle should go though getMyVoxelHandle
    // so that the voxels are loaded from shared data before access
    mutable UT_VoxelArrayHandleF	 myVoxelHandle;
    mutable GA_SharedDataHandlePtr	 mySharedVoxelData;
    mutable UT_Lock			 mySharedDataLock;
    bool				 mySharedDataHandleLoaded;

    mutable CE_Grid			*myCEGrid;
    mutable bool			 myCEGridAuthorative;

    virtual bool	evaluatePointRefMap(GA_Offset result_vtx,
				    GA_AttributeRefMap &hlist,
				    fpreal u, fpreal v, uint du, uint dv) const;
    virtual int		 evaluatePointV4( UT_Vector4 &pos, float u, float v = 0,
					unsigned du=0, unsigned dv=0) const
			 {
			    return GEO_Primitive::evaluatePointV4(pos, u, v,
					du, dv);
			 }
    virtual bool	 evaluateBaryCenterRefMap(GA_Offset result_vertex,
					GA_AttributeRefMap &hlist) const;
    
    virtual bool	 evaluateInteriorPointRefMap(GA_Offset result_vtx,
					GA_AttributeRefMap &map,
					fpreal u, fpreal v, fpreal w = 0) const;
    virtual int		 evaluateInteriorPointV4(UT_Vector4 &pos,
				       fpreal u, fpreal v, fpreal w = 0) const;

private:
#if !GA_PRIMITIVE_VERTEXLIST
    GA_Offset		myVertex;	// My vertex
#endif
    UT_Matrix3		myXform;	// My Transform
    UT_Matrix3		myInverseXform;	// My inverse transform
    bool		myIsSDF : 1;	// Are we a signed distance field?

    GEO_VolumeOptions	myVis;

    // The taper is the radius of the bottom half, z-minus, of the default box
    // The top half's radius is one.  These radii are then modified by
    // myXform.
    fpreal	 	myTaperX, myTaperY;

    friend std::ostream	&operator<<(std::ostream &os, const GEO_PrimVolume &d)
			{
			    d.saveH9(os, 0,
                                     GEO_Primitive::theEmptySaveAttribs,
                                     GEO_Primitive::theEmptySaveAttribs);
			    return os;
			}
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif
