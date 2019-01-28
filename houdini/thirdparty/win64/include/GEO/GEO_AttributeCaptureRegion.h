/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_AttributeCaptureRegion.h ( GB Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GEO_AttributeCaptureRegion__
#define __GEO_AttributeCaptureRegion__

#include "GEO_API.h"
#include "GEO_Detail.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_Matrix4.h>
#include <GA/GA_Handle.h>
#include <GA/GA_Types.h>

class GA_ROAttributeRef;
class GA_RWAttributeRef;
class GA_AIFIndexPairObjects;

/// @brief Convenience class to store a bone capture region
struct GEO_CaptureBoneStorage
{
    inline
    GEO_CaptureBoneStorage()
	: myXform(1.0)
	, myTCap(1)
	, myBCap(1)
	, myTaperX(1)
	, myTaperZ(1)
    {
    }

    static const int	    tuple_size = 20;

    inline float *	    floatPtr()
				{ return reinterpret_cast<float*>(this); }
    inline const float *    floatPtr() const
				{ return reinterpret_cast<const float*>(this); }

    UT_Matrix4	myXform;    // The order matters (see enum inside)
    float	myBCap;	    // The order matters (see enum inside)
    float	myTCap;	    // The order matters (see enum inside)
    float	myTaperX;   // The order matters (see enum inside)
    float	myTaperZ;   // The order matters (see enum inside)
};

class GEO_API GEO_AttributeCaptureData
{
public:
    GEO_AttributeCaptureData(const GEO_Detail *gdp,
		GEO_Detail::CaptureType type = GEO_Detail::CAPTURE_BONE);

    bool isValid() const;
    int getNumEntries() const;
    int getTupleSize() const;
    fpreal getEntry(int i, int component) const;
    const GA_Attribute *getAttribute() const { return myAttribute; }

protected:
    const GA_Attribute *myAttribute;
    const GA_AIFIndexPair *myIndexPair;
    int	myProperty;
    int myTupleSize;
};

/// @brief Interface class for tube capture attributes
///
/// There are three types of capture regions used in Houdini
/// - Bone: @n
///	A transform, bottom/top caps and taperx/taperz
/// - Metaball: @n
///	A single transform
/// - Tube: @n
///	Bottom/Top caps, taperx/taperz and inside/outside weights
///	(the transform is picked up from the tube primitive)
///
/// @warning This is being used internally by Side Effects to simplify forward
/// compatibility issues.  The lifetime of this class is not well known and
/// code should not rely on any methods herein.
class GEO_API GEO_AttributeCaptureRegion
{
public:
    /// @{
    /// Get/Set bone capture region attribute data values
    /// The object should usually be gdp.attribs().getElement()
    static void	getBoneCaptureRegion(const GA_ROAttributeRef &h,
				int index,
				UT_Matrix4 &xform,
				float &tcap,
				float &bcap,
				float &taperx,
				float &taperz);
    static void	getBoneCaptureRegion(const GA_ROAttributeRef &h,
				int index,
				GEO_CaptureBoneStorage &s)
			{
			    getBoneCaptureRegion(h, index, s.floatPtr(),
				    GEO_CaptureBoneStorage::tuple_size);
			}
    static void	getBoneCaptureRegion(const GA_ROAttributeRef &h,
				int index,
				float *array,
				int array_size);

    static void	setBoneCaptureRegion(const GA_RWAttributeRef &h,
				int index,
				const UT_Matrix4 &xform,
				fpreal tcap,
				fpreal bcap,
				fpreal taperx,
				fpreal taperz);
    static void	setBoneCaptureRegion(const GA_RWAttributeRef &h,
				int index,
				const GEO_CaptureBoneStorage &s)
			{
			    setBoneCaptureRegion(h, index, s.floatPtr(),
				    GEO_CaptureBoneStorage::tuple_size);
			}
    static void	setBoneCaptureRegion(const GA_RWAttributeRef &h,
				int index,
				const float *array,
				int array_size);
    static void	setBoneCaptureRegion(GA_AIFIndexPairObjects *objects,
				int index,
				const UT_Matrix4 &xform,
				fpreal tcap,
				fpreal bcap,
				fpreal taperx,
				fpreal taperz);
    static void	setBoneCaptureRegion(GA_AIFIndexPairObjects *objects,
				int index,
				GEO_CaptureBoneStorage &s)
			{
			    setBoneCaptureRegion(objects, index, s.floatPtr(),
				    GEO_CaptureBoneStorage::tuple_size);
			}
    static void	setBoneCaptureRegion(GA_AIFIndexPairObjects *objects,
				int index,
				const float *array,
				int array_size);
    /// @}

    /// Given an bone point capture attribute, retrieve the index pair object
    /// and index for the capture region property. Returns NULL if the
    /// attribute isn't of the right type.
    /// @{
    static GA_AIFIndexPairObjects *
		getBoneCaptureRegionObjects(
				const GA_RWAttributeRef &h, int &property_i);
    static const GA_AIFIndexPairObjects *
		getBoneCaptureRegionObjects(
				const GA_ROAttributeRef &h, int &property_i);
    /// @}

    /// Copy a bone capture region from one to another
    static void	copyBoneCaptureRegion(const GA_RWAttributeRef &dh,
				int dindex,
				const GA_ROAttributeRef &sh,
				int sindex);

    /// Initialize bone capture region
    static void	initBoneCapture(float *array, int array_size);

    /// Initialize tube capture region
    static void	initTubeCapture(float *array, int array_size);

    /// All in one method that creates a tube capture attribute with
    /// appropriate defaults.
    static bool addTubeCaptureRegionAttrib(
				GEO_Detail *gdp, GA_RWAttributeRef &h);

    /// @{
    /// Get/Set tube capture region attribute data values
    /// The attribute element should be the primitive
    static void	getTubeCaptureRegion(GA_Offset prim_offset,
				const GA_ROAttributeRef &h,
				float &bcap,
				float &tcap,
				float &taperx,
				float &taperz,
				float &in_weight,
				float &out_weight);
    static void	setTubeCaptureRegion(GA_Offset prim_offset,
				const GA_RWAttributeRef &h,
				fpreal bcap,
				fpreal tcap,
				fpreal taperx = 1.0,
				fpreal taperz = 1.0,
				fpreal in_weight = 1.0,
				fpreal out_weight = 0.0);
    /// @}

    /// @{
    /// Access specific tube attributes through an attribute handle
    static fpreal	getTubeBCap(const GA_ROHandleR &h, GA_Offset primoff);
    static fpreal	getTubeTCap(const GA_ROHandleR &h, GA_Offset primoff);
    static void		setTubeBCap(const GA_RWHandleR &h, GA_Offset primoff, fpreal v);
    static void		setTubeTCap(const GA_RWHandleR &h, GA_Offset primoff, fpreal v);
    /// @}

    /// @{
    /// Get/Set metaball capture region attribute values
    /// The object should usually be gdp.attribs().getElement()
    static void	getMetaCaptureRegion(const GA_ROAttributeRef &h,
				int index,
				int propid,
				int objset,
				UT_Matrix4 &xform);
    static void	setMetaCaptureRegion(const GA_RWAttributeRef &h,
				int index,
				int prop,
				int objset,
				const UT_Matrix4 &xform);
    /// @}

    /// Get the inverted metaball capture region transform
    static void getMetaCaptureRegionInverted(const GA_ROAttributeRef &h,
				int index,
				int propid,
				int objset,
				UT_Matrix4 &xform);

    /// @{
    /// Get/Set the anchor/rest metaball attribute
    static void	getMetaRestCaptureRegion(const GA_ROAttributeRef &h,
				int index,
				int prop,
				int objset,
				UT_Matrix4 &xform)
		    { getMetaCaptureRegion(h, index, prop, objset, xform); }
    static void	setMetaRestCaptureRegion(const GA_RWAttributeRef &h,
				int index,
				int prop,
				int objset,
				const UT_Matrix4 &xform)
		    { setMetaCaptureRegion(h, index, prop, objset, xform); }
    /// @}

    /// @{
    /// Return the number of floats in each type of capture region
    static int	getBoneSize()		{ return theBoneSize; }
    static int	getTubeSize()		{ return theTubeSize; }
    static int	getMetaSize()		{ return theMetaSize; }
    static int	getMetaRestSize()	{ return theMetaSize; }
    /// @}

private:
    static const int	theBoneSize;	// Size of a bone capture region
    static const int	theTubeSize;	// Size of a tube capture region
    static const int	theMetaSize;	// Size of a meta capture region
};

#endif

