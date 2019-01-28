/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_LinearSkinDeformer.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_LINEARSKINDEFORMER_H_INCLUDED__
#define __GU_LINEARSKINDEFORMER_H_INCLUDED__

#include "GU_API.h"
#include <GEO/GEO_AttributeIndexPairs.h>
#include <GA/GA_Range.h>
#include <UT/UT_Array.h>
#include <UT/UT_Assert.h>
#include <UT/UT_DualQuaternion.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_Quaternion.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_VectorTypes.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_StaticAssert.h>
#include <SYS/SYS_TypeDecorate.h>
#include <SYS/SYS_Types.h>

#include <stddef.h>


class UT_TaskList;
class GA_Attribute;
class GA_ROAttributeRef;
class GEO_AttributeCapturePath;
class GU_Detail;
class GU_LinearSkinDeformer;

struct GU_CaptureElement
{
    GU_CaptureElement() { }
    GU_CaptureElement(int i, float w) : myI(i), myW(w) { }

    bool operator>(const GU_CaptureElement& elem) const
    {
	return (myW > elem.myW);
    }

    int	    myI;
    float   myW;
};
SYS_DECLARE_IS_POD(GU_CaptureElement)
typedef UT_Array<GU_CaptureElement> GU_CaptureElementArray;


template <typename T>
struct GU_DualQuatXformT
{
    using ThisType = GU_DualQuatXformT<T>;

    SYS_FORCE_INLINE	GU_DualQuatXformT() = default;
    SYS_FORCE_INLINE	GU_DualQuatXformT(const ThisType&) = default;
    SYS_FORCE_INLINE	GU_DualQuatXformT(ThisType&&) = default;
    SYS_FORCE_INLINE
    ThisType&		operator=(const GU_DualQuatXformT<T>&) = default;
    SYS_FORCE_INLINE
    ThisType&		operator=(GU_DualQuatXformT<T>&&) = default;

    SYS_FORCE_INLINE
    UT_Vector3T<T> transform(const UT_Vector3T<T>& src) const
    {
	UT_Vector3T<T> dst;
	myDQ.transform(src * myStretch, dst);
	return dst;
    }
    SYS_FORCE_INLINE
    UT_Vector3T<T> transformTransposed(const UT_Vector3T<T>& src) const
    {
	UT_Vector3T<T> dst;
	myDQ.transform(src, dst);
	dst *= myStretch;
	return dst;
    }
    SYS_FORCE_INLINE
    UT_QuaternionT<T> transform(const UT_QuaternionT<T>& src) const
    {
	UT_ASSERT(false);
	return src;
    }
    SYS_FORCE_INLINE
    UT_QuaternionT<T> transformTransposed(const UT_QuaternionT<T>& src) const
    {
	UT_ASSERT(false);
	return src;
    }
    SYS_FORCE_INLINE
    UT_Vector3T<T> rotate(const UT_Vector3T<T>& src) const
    {
	UT_Vector3T<T> dst;
	myDQ.rotate(src, dst);
	return dst;
    }
    SYS_FORCE_INLINE
    UT_QuaternionT<T> rotate(const UT_QuaternionT<T>& src) const
    {
	return src * myDQ.getRotation();
    }

    UT_DualQuaternionT<T>	    myDQ;
    UT_Matrix3T<T>		    myStretch;
};
SYS_DECLARE_IS_POD(GU_DualQuatXformT<fpreal32>)
SYS_DECLARE_IS_POD(GU_DualQuatXformT<fpreal64>)


/// Class representing source data for GU_LinearSkinDeformer
class GU_API GU_LinearSkinDeformerSource : UT_NonCopyable
{
public:
				    GU_LinearSkinDeformerSource();

    int64                           getMemoryUsage(bool inclusive) const;

    /// Reset all state
    void			    reset();

    /// Setup the source geometry. Returns the number of transforms requireds
    /// to deform using the capture weight attribute. If in error, then 0 is
    /// returned.
    /// If max_pt_weights > 0, then it will limit the number of weight entries
    /// to use per point.
    int				    init(
					const GU_Detail& src,
					const GA_Range& pt_range,
					int max_pt_weights = 0);

    /// Clear the list of appended attributes
    void			    clearAttribs();

    /// Append attribute to be deformed. How it is deformed depends on src's
    /// GA_TypeInfo. initSrc() must have been called first.
    void			    appendAttrib(
					const GA_Attribute& src_attrib);

    const GU_Detail*		    srcGdp() const
					{ return myGdp; }
    void			    setSrcGdp(const GU_Detail &src)
					{ myGdp = &src; }

    exint			    numPoints() const
					{ return myPointStarts.entries() > 0
					    ? myPointStarts.entries()-1 : 0; }

    /// Return the number of regions found by initSrc()
    int				    numRegions() const
					{ return myRegionXforms.entries(); }

    /// Return the name given the region index. init() must have been
    /// called first.
    const char*			    regionName(int i) const
					{ return myAttribCaptPath.getPath(i); }

    /// Static utility method to get capture parameters
    static bool			    getCaptureParms(
					const GU_Detail& src,
					GA_ROAttributeRef& pcapt,
				        GEO_AttributeCapturePath& attr_cap_path,
					UT_Array<UT_Matrix4F>& xforms,
					int& max_pt_regions);

private:
    const GU_Detail*		    myGdp;
    GEO_AttributeCapturePath	    myAttribCaptPath;
    GA_OffsetArray		    myPointOffsets;
    UT_ExintArray		    myPointStarts;
    GU_CaptureElementArray	    myCaptureElements;

    UT_Array<UT_Matrix4F>	    myRegionXforms;

    UT_Array<const GA_Attribute*>   myPosAttribs;
    UT_Array<const GA_Attribute*>   myVecAttribs;
    UT_Array<const GA_Attribute*>   myNmlAttribs;
    UT_Array<const GA_Attribute*>   myQuatAttribs;

    friend class GU_LinearSkinDeformer;
};


/// Performs linear blend skinning of geometry
class GU_API GU_LinearSkinDeformer : UT_NonCopyable
{
public:
				    GU_LinearSkinDeformer();

    /// Reset all state
    void			    reset();

    enum Method
    {
	DEFORM_LINEAR,
	DEFORM_DUALQUAT,
	DEFORM_DUALQUAT_BLEND,
    };

    /// Setup the destination geometry. Fails if the destination is missing
    /// attributes that were appended to deform. A reference to the src is
    /// maintained until reset() is called.
    bool			    init(
					GU_Detail& dst,
					const GA_Range& pt_range,
					const GU_LinearSkinDeformerSource& src,
					Method method = DEFORM_LINEAR,
					bool create_missing_attribs = false,
					const char *blend_attrib = nullptr);

    int				    numRegions() const
				    { return mySrc->myRegionXforms.entries(); }

    const char*			    regionName(int i) const
				    { return mySrc->regionName(i); }

    /// For debugging purposes, call this before making your
    /// setRegionTransform() calls to avoid asserts about missing transforms.
    void			    clearNumTransformsSet()
					{ UT_IF_ASSERT(myNumTransformsSet=0;) }

    /// Set deforming transform for region_idx, which matches indices found in
    /// the capture weight attribute. init() must have been called first.
    void			    setRegionTransform(
					int region_idx,
					const UT_Matrix4F& xform);

    /// Set deforming transform for region_idx. To apply the capture region's
    /// transform use setRegionTransform(). init() must have been called first.
    void			    setFullRegionTransform(
					int region_idx,
					const UT_Matrix4F& xform);

    /// Append UT_Tasks that will perform the deformation. The tasks will have
    /// references to this object, so they must be executed before this object
    /// is destroyed.
    ///
    /// The necessary steps in this specific order:
    /// 1. N = GU_LinearSkinDeformerSource::init() for the point geometry you 
    ///    want to deform
    /// 2. GU_LinearSkinDeformerSource::appendAttrib() for each src attribute
    ///	   you want to deform
    /// 3. GU_LinearSkinDeformer::init() to set up where to put the results
    /// 4. Call setRegionTransform() N times for each of the cregions
    /// 5. Call appendDeformTasks()
    /// 6. When done with the task list, spawn them: tasks.spawnRootAndWait();
    ///
    void			    appendDeformTasks(
					UT_TaskList& tasks);
    void			    appendInverseDeformTasks(
					UT_TaskList& tasks);

    /// Do the deform.
    ///
    /// The necessary steps in this specific order:
    /// 1. N = GU_LinearSkinDeformerSource::init() for the point geometry you 
    ///    want to deform
    /// 2. GU_LinearSkinDeformerSource::appendAttrib() for each src attribute
    ///	   you want to deform
    /// 3. GU_LinearSkinDeformer::init() to set up where to put the results
    /// 4. Call setRegionTransform() N times for each of the cregions
    /// 5. Call deform()
    ///
    void			    deform();
    void			    inverse_deform();

public:
    typedef UT_Array<UT_UniquePtr<GA_Attribute>> AttribPtrArray;
    typedef GU_DualQuatXformT<float>		 DualQuatXformF;

    static const char* SKIN_LINEAR;
    static const char* SKIN_DUAL_QUATERNION;
    static const char* SKIN_BLEND_DUAL_QUATERNION_AND_LINEAR;

    static bool			    getGlobalAttribMethod(const GU_Detail *igdp, Method &type);
    static bool			    getGlobalAttribBlendAttrib(const GU_Detail *igdp,
							       UT_StringHolder &blend_attrib);

private:

    void			    clearXforms();

    void			    deformInParallel();

    template <bool ALIGNED, bool INVERT>
    void			    appendLinearTasks(
					UT_TaskList& tasks);
    template <bool ALIGNED, bool INVERT>
    void			    appendDualQuatTasks(
					UT_TaskList& tasks);
    template <bool ALIGNED, bool INVERT>
    void			    appendBlendedDualQuatTasks(
					UT_TaskList& tasks);

private:
    Method			    myMethod;

    GU_Detail*			    myDstGdp;
    GA_Range			    myDstRange;
    UT_Array<GA_Attribute*>	    myDstPosAttribs;
    UT_Array<GA_Attribute*>	    myDstVecAttribs;
    UT_Array<GA_Attribute*>	    myDstNmlAttribs;
    UT_Array<GA_Attribute*>	    myDstQuatAttribs;

    const GU_LinearSkinDeformerSource*
				    mySrc;

    const GA_Attribute*		    mySrcBlendAttrib;

    AttribPtrArray		    mySrcPromotedAttribs;

    // Linear blending
    UT_Array<UT_Matrix4F>	    myPosXforms;
    UT_Array<UT_QuaternionF>	    myQuatXforms;

    // DualQuaternion blending
    UT_Array<DualQuatXformF>	    myDualQuatXforms;

    // This is only used when assertions are enabled
    exint			    myNumTransformsSet;
};

#endif // __GU_LINEARSKINDEFORMER_H_INCLUDED__
