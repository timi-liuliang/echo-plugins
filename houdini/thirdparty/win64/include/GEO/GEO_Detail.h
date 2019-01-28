/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This defines the detail class (the container for all points/primitives)
 */

#ifndef __GEO_Detail_h__
#define __GEO_Detail_h__

#include "GEO_API.h"
#include "GEO_Error.h"
#include "GEO_PrimList.h"
#include "GEO_PrimType.h"
#include "GEO_PrimTypeCompat.h"

// It would be great if these two includes could be removed, because they
// don't strictly need to be here, but almost nothing includes them
// directly, so they're here until someone makes sure that everything
// that needs them includes them.
#include "GEO_Primitive.h"
#include "GEO_Macros.h" // Most will probably just need GA/GA_GBMacros.h

#include <GA/GA_Detail.h>
#include <GA/GA_ElementGroup.h>
#include <GA/GA_Types.h>

#include <UT/UT_Color.h>
#include <UT/UT_FloatArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Array.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector3Array.h>
#include <UT/UT_VectorTypes.h>

#include <iosfwd>
#include <stddef.h>

template<typename T> class UT_BoundingBoxT;
typedef UT_BoundingBoxT<float> UT_BoundingBox;
class UT_BoundingSphere;
class UT_String;
class UT_IStream;
class UT_MemoryCounter;
class UT_Ramp;
class UT_StringHolder;
template<typename T> class UT_StringMap;
class UT_StringRef;
class GA_AttributeRefMap;
class GA_Breakpoint;
class GA_BreakpointGroup;
class GA_EdgeGroup;
class GA_VertexWrangler;
class GA_PointWrangler;
class GEO_AttributeHandle;
class GEO_Delta;
class GEO_MetaExpression;
class GEO_MirrorParms;
class GEO_PointSliderCache;
class GEO_Primitive;
class GEO_Rolloff;
class GEO_Vertex;
class GA_GBPointRedirectArray;
class TS_MetaExpression;

template<typename T, bool B> class GA_EdgeT; 
using GA_Edge = GA_EdgeT<GA_Offset, false>;

template <typename T>
struct AdjacencyFilter;

//
// these are the names of standard attributes
//

#define	GEO_STD_ATTRIB_POSITION		"P"
#define	GEO_STD_ATTRIB_NORMAL		"N"
#define	GEO_STD_ATTRIB_TEXTURE		"uv"
#define	GEO_STD_ATTRIB_VELOCITY		"v"
#define GEO_STD_ATTRIB_ANGULAR_VELOCITY	"w"
#define	GEO_STD_ATTRIB_MATERIAL		"shop_materialpath"
#define GEO_STD_ATTRIB_OGLSHOP		"ogl_shop"
#define	GEO_STD_ATTRIB_DIFFUSE		"Cd"
#define	GEO_STD_ATTRIB_ALPHA		"Alpha"
#define	GEO_STD_ATTRIB_COM		"com"
#define	GEO_STD_ATTRIB_DISTANCE		"dist"
#define	GEO_STD_ATTRIB_LOD		"lod"
#define	GEO_STD_ATTRIB_REST		"rest"
#define GEO_STD_ATTRIB_MASS		"mass"
#define GEO_STD_ATTRIB_DRAG		"drag"
#define GEO_STD_ATTRIB_TENSION		"tension"
#define GEO_STD_ATTRIB_SPRINGK		"springk"
#define GEO_STD_ATTRIB_LIFE		"life"
#define GEO_STD_ATTRIB_AGE		"age"
#define GEO_STD_ATTRIB_DIST		"dist"
#define GEO_STD_ATTRIB_UP		"up"
#define GEO_STD_ATTRIB_ORIENT		"orient"
#define GEO_STD_ATTRIB_ID		"id"
#define GEO_STD_ATTRIB_PSCALE		"pscale"
#define GEO_STD_ATTRIB_CREASE		"creaseweight"
#define GEO_STD_ATTRIB_AREA		"area"
#define GEO_STD_ATTRIB_PERIMETER	"perimeter"
#define GEO_STD_ATTRIB_WIDTH		"width"
#define GEO_STD_ATTRIB_LAYERMERGE	"layermerge"
#define GEO_STD_ATTRIB_SEGS		"segs"
#define GEO_STD_ATTRIB_DIV		"div"
#define GEO_STD_ATTRIB_LAGE		"lage"
#define GEO_STD_ATTRIB_ARC		"arc"
#define GEO_STD_ATTRIB_GEN		"gen"
#define	GEO_STD_ATTRIB_TUBECAPTURE	"tubeCapt"
#define GEO_STD_ATTRIB_PNT_CAPTURE_ALPHA "pCaptAlpha"
#define GEO_STD_ATTRIB_PNT_CAPTURE_PATH  "pCaptPath"
#define	GEO_STD_ATTRIB_PNT_CAPTURE_DATA  "pCaptData"
#define GEO_STD_ATTRIB_CAPTUREFRAME	"pCaptFrame"
#define GEO_STD_ATTRIB_CAPT_SKELROOT	"pCaptSkelRoot"
#define GEO_STD_ATTRIB_CLOTH_CAPT	"clothCapt"
#define GEO_STD_ATTRIB_WIRE_CAPT_U	"wireCaptU"
#define GEO_STD_ATTRIB_WIRE_CAPT_V	"wireCaptV"
#define GEO_STD_ATTRIB_WIRE_CAPT_PRIMU	"wireCaptPrimU"
#define GEO_STD_ATTRIB_META_CAPT_FRAME	"metaCaptFrame"
#define GEO_STD_ATTRIB_META_CAPT_GROUPS	"metaCaptGroups"
#define GEO_STD_ATTRIB_META_CAPT_ROOT	"metaCaptRoot"
#define GEO_STD_ATTRIB_META_CAPT_PATHS	"metaCaptPaths"
#define GEO_STD_ATTRIB_META_CAPT_DATA	"metaCaptData"
#define GEO_STD_ATTRIB_META_CAPT_GROUP_MAP "metaCaptGroupMap"
#define GEO_STD_ATTRIB_META_SLIDE	"slideModifier"
#define GEO_STD_ATTRIB_INFLATE_MODIFIER	"inflatemodifier"
#define GEO_STD_ATTRIB_MUSCLE_REST_ANCHOR "restAnchor"
#define GEO_STD_ATTRIB_MUSCLE_REST_XFORMS "restTransforms"
#define GEO_STD_ATTRIB_MUSCLE_INFLATE_NORM "muscleInflateNormal"
#define GEO_STD_ATTRIB_MUSCLE_POSITION_BIAS "positionbiasvalues"
#define GEO_STD_ATTRIB_MUSCLE_CAPT_NAMES    "muscleCaptNames"
#define GEO_STD_ATTRIB_RIXLATE		"rixlate"
#define GEO_STD_ATTRIB_DEFORM_SKIN_METHOD "deformSkinMethod"
#define GEO_STD_ATTRIB_DEFORM_DUAL_QUATERNION_BLEND_ATTRIB "deformDualQuaternionBlendAttrib"
#define GEO_STD_ATTRIB_DEFORM_TRANSFORMS_PATH "deformTransformsPath"
#define GEO_STD_ATTRIB_POSE_SHAPE_EXAMPLE_PATH	"poseShapeExamplePath"
#define GEO_STD_ATTRIB_SHAPE_DIFF_METHOD "shapeDiffMethod"
#define GEO_STD_ATTRIB_SHAPE_DIFF_ORIENT_ATTRIB "shapeDiffOrientAttrib"
#define GEO_STD_ATTRIB_SHAPE_DIFF_TRANSFORM_ATTRIB "shapeDiffTransformAttrib"
#define GEO_STD_ATTRIB_STASH_POSE_PATH	"stashPosePath"

/* Update the getStandardAttributes method when adding attribs */
/* These become the indices into the "context" detail attribute to
 * look up when doing a getStandardAttribute */
enum GEO_Standard_Attributes
{
    GEO_ATTRIBUTE_NORMAL,
    GEO_ATTRIBUTE_TEXTURE,
    GEO_ATTRIBUTE_VELOCITY,
    GEO_ATTRIBUTE_MATERIAL,
    GEO_ATTRIBUTE_DIFFUSE,
    GEO_ATTRIBUTE_ALPHA,
    GEO_ATTRIBUTE_COM,
    GEO_ATTRIBUTE_DISTANCE,
    GEO_ATTRIBUTE_LOD,
    GEO_ATTRIBUTE_REST,
    GEO_ATTRIBUTE_MASS,
    GEO_ATTRIBUTE_DRAG,
    GEO_ATTRIBUTE_TENSION,
    GEO_ATTRIBUTE_SPRINGK,
    GEO_ATTRIBUTE_LIFE,
    GEO_ATTRIBUTE_DIST,
    GEO_ATTRIBUTE_UP,
    GEO_ATTRIBUTE_ID,
    GEO_ATTRIBUTE_PSCALE,
    GEO_ATTRIBUTE_CREASE,
    GEO_ATTRIBUTE_AREA,
    GEO_ATTRIBUTE_PERIMETER,
    GEO_ATTRIBUTE_WIDTH,
    GEO_ATTRIBUTE_OGLSHOP,
    GEO_ATTRIBUTE_LAYERMERGE,
    GEO_ATTRIBUTE_SEGS,
    GEO_ATTRIBUTE_DIV,
    GEO_ATTRIBUTE_LAGE,
    GEO_ATTRIBUTE_ARC,
    GEO_ATTRIBUTE_GEN,
    GEO_ATTRIBUTE_AGE,
    GEO_ATTRIBUTE_ANGULAR_VELOCITY,
    GEO_ATTRIBUTE_ORIENT,
    GEO_ATTRIBUTE_TRANSFORM,
    GEO_NUM_STANDARD_ATTRIBUTE
};

enum GEO_Capture_Bone_Property
{
    GEO_CAPTBONE_PROPERTY_PATH,
    GEO_CAPTBONE_PROPERTY_DATA,
    GEO_NUM_CAPTBONE_PROPERTY
};

enum GEO_Capture_Muscle_Property
{
    GEO_CAPTMUSCLE_PROPERTY_GROUPS,
    GEO_CAPTMUSCLE_PROPERTY_XFORMS,
    GEO_CAPTMUSCLE_PROPERTY_POSITIONBIAS,
    GEO_NUM_CAPTMUSCLE_ROPERTY
};

enum GEO_Capture_Meta_Property
{
    GEO_CAPTMETA_PROPERTY_GROUPMAP,
    GEO_CAPTMETA_PROPERTY_PATH,
    GEO_CAPTMETA_PROPERTY_DATA,
    GEO_NUM_CAPTMETA_PROPERTY
};

enum GEO_Capture_Wire_Property
{
    GEO_CAPTWIRE_PROPERTY_PRIMU,
    GEO_NUM_CAPTWIRE_PROPERTY
};

/// An enum for GEO_Detail::copy's method parameter.
enum GEO_CopyMethod {
    GEO_COPY_ONCE,
    GEO_COPY_START,
    GEO_COPY_ADD,
    GEO_COPY_END
};

class GEO_API GEO_Detail : public GA_Detail
{
public:
    	     GEO_Detail(GA_PrimitiveFactory &factory,
			bool full_topology=true);
    virtual ~GEO_Detail();

    /// Compute memory usage (includes all shared memory)
    virtual int64 getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// Clear all the points/primitives out of this detail
    void clearAndDestroy()
    {
        clearCaches();
        incrementMetaCacheCount();
        GA_Detail::clear();
    }

    ///	Merge the whole detail, or only a group of prims, or only some points.
    ///
    /// If a @c primGroup is given, only the primitives in that group will be
    /// merged into this detail.  Only points referenced by the primitives will
    /// be merged.  The @c primGroup should exist on the @c src detail.
    ///
    /// If a @c dest_to_src_ptarray is given, the array will be filled out,
    /// mapping the destination points (in this detail) to the source points
    /// (in the source detail).
    ///
    /// Primitives will be appended to the primitive list unless @c
    /// insertPrimsAtHead is set.
    ///
    /// If @c mergePrimGroup is set, primitive groups will be copied from the
    /// source detail.  If @c keep_internal_groups is true, @b all groups
    /// will be copied, not just public primitive groups.
    ///
    /// If @c data_id_strategy is GA_DATA_ID_CLONE and it's a full merge into
    /// an empty detail, any attributes whose source has a valid data id will
    /// inherit that data id.  Otherwise, each merged attribute will get a new
    /// unique data id if this detail is generating such.
    ///
    void	 merge(const GEO_Detail &src, const GA_PrimitiveGroup *primGrp = 0,
			     int mergePrimGroup = 1, int insertPrimsAtHead= 0,
			     GA_GBPointRedirectArray *dest_to_src_ptarray=0,
			     bool keep_internal_groups = true,
			     GA_DataIdStrategy data_id_strategy
							    = GA_DATA_ID_BUMP);
    /// Merge points from the source detail into this detail.
    ///
    /// If the @c ptGrp group is specified, only points in the @c src detail's
    /// group will be merged.
    ///
    /// If @c merge_groups is set, point groups will be copied from the source
    /// detail.  If @c keep_internal_groups is set, @b all groups will be
    /// copied (not just public point groups).
    void	 mergePoints(const GEO_Detail &src,
			     const GA_PointGroup *ptGrp = 0,
			     bool merge_groups = true,
			     bool keep_internal_groups = true);

    ///	Merge only a group of points.
    void	 mergePoints(const GEO_Detail &src, const GA_Range &points);

    ///	Merge only a group of prims.
    void	 mergePrimitives(const GEO_Detail &src, const GA_Range &prims);

    /// Merge a single primitive into this detail.
    void	 merge(const GEO_Primitive &sprim, bool insertPrimAtHead=false);

    /// Copy the source detail into this detail.
    ///
    /// If @c data_id_strategy is GA_DATA_ID_CLONE and method is GEO_COPY_ONCE,
    /// any attributes whose source has a valid data id will inherit that data
    /// id.  Otherwise, each merged attribute will get a new unique data id if
    /// this detail is generating such.
    bool	 copy(const GEO_Detail &src, GEO_CopyMethod method = GEO_COPY_ONCE,
		      bool this_parameter_is_ignored = true,
		      bool keep_internal_groups = true,
		      GA_DataIdStrategy data_id_strategy = GA_DATA_ID_BUMP);

    void	 collapseIndexAttributes();

    /// Mirrors a portion of the gdp across some plane
    void	 mirror(GEO_MirrorParms &parms);

    /// Transform a range of primitives.  Note that this will only transform
    /// the points used by the primitives.
    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    template<typename FLOAT_T>
    void	 transform(const UT_Matrix4T<FLOAT_T> &mat,
			    const GA_Range &primitives,
			    bool just_P,
			    bool keep_vector_lengths = true,
			    bool check_pasting = true,
			    bool neg_determinant_flip_vector = true,
			    bool update_ptnormals = false,
			    GEO_Delta *geodelta = NULL,
                            bool updateaffectednormals = false,
                            const char *attribpattern = NULL);
    /// Transform a range of primitives & points.
    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    template<typename FLOAT_T>
    void	 transform(const UT_Matrix4T<FLOAT_T> &mat,
			    const GA_Range &primitives,
			    const GA_Range &points,
			    bool just_P,
			    bool keep_vector_lengths = true,
			    bool check_pasting = true,
			    bool neg_determinant_flip_vector = true,
			    bool update_ptnormals = false,
			    GEO_Delta *geodelta = NULL,
                            bool updateaffectednormals = false,
                            const char *attribpattern = NULL);


    /// Transform/translate all or some of the primitives. The methods will run
    /// more efficiently if you happen to know the point closure apriori.
    /// If 'just_P' is true, we only transform P, and not other attributes,
    /// and don't check for reflection matrix.
    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    template<typename FLOAT_T>
    void	 transform(const UT_Matrix4T<FLOAT_T> &mat,
			   const GA_PrimitiveGroup *primGrp = 0,
			   const GA_PointGroup *pointClosure = 0,
			   bool just_P = false,
			   bool keep_vector_lengths = true,
			   bool check_pasting = true,
			   bool neg_determinant_flipvector = true,
			   bool update_ptnormals = false,
			   GEO_Delta *geodelta = 0,
                           bool updateaffectednormals = false,
                           const char *attribpattern = NULL);

    // We transform from min_ptnum to max_ptnum *inclusive*.
    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    template<typename FLOAT_T>
    void	 transformRange(const UT_Matrix4T<FLOAT_T> &mat,
			   GA_Index min_ptnum, GA_Index max_ptnum,
			   GA_Index min_primnum, GA_Index max_primnum,
			   bool just_P = false,
			   bool keep_vector_lengths = true,
			   bool check_pasting = true,
			   bool neg_determinant_flipvector = true,
			   bool update_ptnormals = false,
			   GEO_Delta *geodelta = 0,
                           bool updateaffectednormals = false,
                           const char *attribpattern = NULL);

    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    template<typename FLOAT_T>
    void	 translate(const UT_Vector3T<FLOAT_T> &delta,
			   const GA_PrimitiveGroup *primGrp = 0,
			   const GA_PointGroup *pointClosure = 0,
			   bool check_pasting = true,
			   bool update_ptnormals = false,
			   GEO_Delta *geodelta = 0,
                           bool updateaffectednormals = false,
                           const char *attribpattern = NULL);

    /// Transform all range of the points.
    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    template<typename FLOAT_T>
    void	 transformPoints(const UT_Matrix4T<FLOAT_T> &mat,
			         const GA_Range &points,
			         bool just_P = false,
				 bool keep_vector_lengths = true,
				 bool check_pasting = true,
				 bool neg_determinant_flipvector = true,
				 bool update_ptnormals = false,
				 GEO_Delta *geodelta = 0,
                                 bool updateaffectednormals = false,
                                 const char *attribpattern = NULL);
    /// Transform/translate all or some of the points.
    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    template<typename FLOAT_T>
    void	 transformPoints(const UT_Matrix4T<FLOAT_T> &mat,
			         const GA_PointGroup *pointGroup = 0,
			         const GA_PrimitiveGroup *primClosure = 0,
			         bool just_P = false,
				 bool keep_vector_lengths = true,
				 bool check_pasting = true,
				 bool neg_determinant_flipvector = true,
				 bool update_ptnormals = false,
				 GEO_Delta *geodelta = 0,
                                 bool updateaffectednormals = false,
                                 const char *attribpattern = NULL);

    /// Transform a range of points by an attrib which stores a matrix4
    /// for each point.
    template <typename FLOAT_T, bool INVERT=false>
    void	transformPointsByAttribute(
			const GA_ROHandleT<UT_Matrix4T<FLOAT_T> > &xformhandle,
			const GA_Range &points,
			bool keep_vector_lengths = true,
			bool updateaffectednormals = true,
			const char *attribpattern = NULL);

    /// Rotates/shears all the attributes by a matrix3 for each point.
    /// Note: Does not scale/rotate GA_TYPE_POINT/GA_TYPE_HPOINT.
    template <typename FLOAT_T, bool INVERT=false>
    void	transformPointsByAttribute(
			const GA_ROHandleT<UT_Matrix3T<FLOAT_T> > &xformhandle,
			const GA_Range &points,
			bool keep_vector_lengths = true,
			bool updateaffectednormals = true,
			const char *attribpattern = NULL);

    /// Rotates all the attributes by a quaternion for each point.
    /// Note: Does not rotate GA_TYPE_POINT/GA_TYPE_HPOINT.
    template <typename FLOAT_T, bool INVERT=false>
    void	transformPointsByAttribute(
			const GA_ROHandleT<UT_QuaternionT<FLOAT_T> > &xformhandle,
			const GA_Range &points,
			bool keep_vector_lengths = true,
			bool updateaffectednormals = true,
			const char *attribpattern = NULL);

    /// Translates all the attributes by a displacement for each point.
    /// Note: Only affects GA_TYPE_POINT/GA_TYPE_HPOINT.
    template <typename FLOAT_T, bool INVERT=false>
    void	translatePointsByAttribute(
			const GA_ROHandleT<UT_Vector3T<FLOAT_T> > &xformhandle,
			const GA_Range &points,
			bool keep_vector_lengths = true,
			bool updateaffectednormals = true,
			const char *attribpattern = NULL);

    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    template<typename FLOAT_T>
    void	 translatePoints(const UT_Vector3T<FLOAT_T> &delta,
			         const GA_PointGroup *pointGroup = 0,
				 bool check_pasting = true,
				 bool update_ptnormals = false,
				 GEO_Delta *geodelta = 0,
                                 bool updateaffectednormals = false,
                                 const char *attribpattern = NULL);

    /// Transforms edges, calls transformPoints on the point closure of the
    /// edges.
    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    template<typename FLOAT_T>
    void	 transformEdges(const UT_Matrix4T<FLOAT_T> &mat,
				const GA_EdgeGroup *edgegrp = 0,
				const GA_PrimitiveGroup *primclosure = 0,
				bool just_P = false,
				bool keep_vector_lengths = true,
				bool check_pasting = true,
				bool neg_determinant_flipvector = true,
				bool update_ptnormals = false,
				GEO_Delta *geodelta = 0,
                                bool updateaffectednormals = false,
                                const char *attribpattern = NULL);

    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    template<typename FLOAT_T>
    void 	 translateEdges(const UT_Vector3T<FLOAT_T> &delta,
				const GA_EdgeGroup *edgegrp = 0,
				bool check_pasting    = true,
				bool update_ptnormals = false,
				GEO_Delta *geodelta = 0,
                                bool updateaffectednormals = false,
                                const char *attribpattern = NULL);

    /// Transforms breakpoints
    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    template<typename FLOAT_T>
    void 	 transformBreakpoints(const UT_Matrix4T<FLOAT_T> &mat,
				      const GA_BreakpointGroup *grp = 0,
				      bool just_P = false,
				      bool update_ptnormals = false,
				      GEO_Delta *geodelta = 0,
                                      bool updateaffectednormals = false,
                                      const char *attribpattern = NULL);

    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    template<typename FLOAT_T>
    void	 translateBreakpoints(const UT_Vector3T<FLOAT_T> &delta,
				      const GA_BreakpointGroup *grp = 0,
				      bool update_ptnormals = false,
				      GEO_Delta *geodelta = 0,
                                      bool updateaffectednormals = false,
                                      const char *attribpattern = NULL);

    /// Convenience function, will call the appropriate transform/translate
    /// depending on the type of group
    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    template<typename FLOAT_T>
    void 	 transformGroup(const UT_Matrix4T<FLOAT_T> &mat,
                                const GA_Group &grp,
 		                bool just_P = false,
				bool keep_vector_lengths = true,
				bool check_pasting = true,
				bool neg_determinant_flipvector = true,
				const GA_PointGroup *ptclosure = 0,
				const GA_PrimitiveGroup *primclosure = 0,
				bool update_ptnormals = false,
				GEO_Delta *geodelta = 0,
                                bool updateaffectednormals = false,
                                const char *attribpattern = NULL);

    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    template<typename FLOAT_T>
    void	 translateGroup(const UT_Vector3T<FLOAT_T> &delta,
			        const GA_Group &grp,
				bool check_pasting = true,
				const GA_PointGroup *ptclosure = 0,
				bool update_ptnormals = false,
				GEO_Delta *geodelta = 0,
                                bool updateaffectednormals = false,
                                const char *attribpattern = NULL);

    void	 visualizeSoftPoints(const GEO_Rolloff &rolloff,
				const GA_PointGroup *ptgroup,
				UT_ColorRamp ramp);

    /// Class for passing symmetry arguments to certain soft transform related
    /// methods.
    class GEO_API SoftSymmetryParms
    {
    public:
	bool operator==(const SoftSymmetryParms &other) const
	{
	    return myNormal == other.myNormal
		&& myOrigin == other.myOrigin
		&& myTol == other.myTol;
	}

	bool operator!=(const SoftSymmetryParms &other) const
	{
	    return !(*this == other);
	}

	UT_Vector3	 myNormal;	// assumed to be normalized!
	UT_Vector3	 myOrigin;
	float		 myTol;
    };

    // this function is outdated, GUsurfaceDistance should be used
    ///   symmetry - the soft falloff from any points not directly on the plane
    ///              of symmetry will be clamped to that plane of symmetry when
    ///              provided
    ///   affected_group - if provided, all points within the soft falloff
    ///                    will be added to this group
    void	 computeSoftPointFalloff(const bool connected,
                                         const fpreal radius,
                                         const GEO_Rolloff *rolloff,
					 const GA_PointGroup *ptgroup,
					 const SoftSymmetryParms *symmetry,
					 const GA_RWHandleF &falloff,
                                         const GA_ROHandleV3 &p_attrib,
                                         GA_PointGroup* affected_group = NULL,
                                         GA_RWHandleID* source_points = NULL)
                 const;

    /// The computeNormal methods compute the normals of primitives, points,
    /// edges and breakpoints.  The primitive one is there for completeness.
    /// An edge normal is the average of its point normals.

    /// For internal functions:
    /// If nml_offset > -1, the attribute at that offset will be returned,
    /// else if the geometry has a normal attribute ("N"), nml_offset
    ///         will be set to the offset of that attribute and that attributes
    ///         value will be returned,
    /// else if the geometry has an internal normal attribute ("internalN")
    ///         the value of the internal attribute will be returned and
    ///         nml_offset will be set to its offset,
    /// else the "internalN" attribute is created by a call to normal(1),
    ///         nml_offset is set to it, and the calculated value returned.
    /// If nml_offset is null, the case of nml_offset == -1 is followed
    /// except nml_offset is not written to.
    /// Otherwise:
    /// Recompute the normal. No side-effects happen, but this is much
    /// slower.

    /// A faster version of computeNormal which has the side effect of
    /// computing the internal normal if it's not there
    void	 computeNormalInternal(GA_Offset ptoff, UT_Vector3 &nml);
   
    /// This is the same as prim.computeNormal()
    void	 computeNormal(const GEO_Primitive &prim, UT_Vector3 &nml) const;
    /// WARNING: This could be very slow! Are you sure you need no side-effects?
    /// See computeNormalInternal!
    void	 computeNormal(const GA_Edge &edge, UT_Vector3 &nml) const;

    /// A faster version of computeNormal which has the side effect of
    /// computing the internal normal if it's not there.
    void	 computeNormalInternal(const GA_Edge &edge, UT_Vector3 &nml);

    /// WARNING: This could be very slow! Are you sure you need no side-effects?
    /// See computeNormalInternal!
    void	 computeNormal(const GA_Breakpoint &bkpt, UT_Vector3 &nml) const;

    /// Compute the normal, and (optionally) tangents, at the specified
    /// breakpoint.  An extension of computeNormal() that also computes
    /// partial tangents.
    void	 computeNormalAndTangents(const GA_Breakpoint &bkpt,
					  UT_Vector3 &nml,
					  UT_Vector3 *u_tangent,
					  UT_Vector3 *v_tangent,
					  bool normalize) const;

    /// The normalTranslate* methods translate points, prims, edges or
    /// breakpoints along each of their normals.
    void	 normalTranslatePoints(float length = 1.0F, 
				       int check_pasting = 1,
				       const GA_PointGroup *grp = 0,
				       int update_ptnormals = 0,
				       GEO_Delta *geodelta = 0,
                                       bool updateaffectednormals = false);
    void	 normalTranslatePrimitives(float length = 1.0F,
				       int check_pasting = 1,
				       const GA_PrimitiveGroup *grp = 0,
				       int update_ptnormals = 0,
				       GEO_Delta *geodelta = 0,
                                       bool updateaffectednormals = false);
    void	 normalTranslate(const GA_PointGroup &grp, 
			         float length = 1.0F, int check_pasting = 1,
				 int update_ptnormals = 0,
				 GEO_Delta *geodelta = 0,
                                 bool updateaffectednormals = false);
    void	 normalTranslate(const GA_PrimitiveGroup &grp, 
			         float length = 1.0F, int check_pasting = 1,
				 int update_ptnormals = 0,
				 GEO_Delta *geodelta = 0,
                                 bool updateaffectednormals = false);
    void	 normalTranslate(const GA_EdgeGroup &grp,
			         float length = 1.0F, int  check_pasting = 1,
				 int update_ptnormals = 0,
				 GEO_Delta *geodelta = 0,
                                 bool updateaffectednormals = false);
    void	 normalTranslate(const GA_BreakpointGroup &grp,
			         float length = 1.0F, int check_pasting = 1,
				 int update_ptnormals = 0,
				 GEO_Delta *geodelta = 0,
                                 bool updateaffectednormals = false);
    void	 normalTranslateGroup(const GA_Group &grp,
			  	 float length = 1.0F, int check_pasting = 1,
				 int update_ptnormals = 0,
				 GEO_Delta *geodelta = 0,
                                 bool updateaffectednormals = false);

    /// Update the point normals that are affected by a transform
    /// applied to a given point group.  If the transform is the same
    /// everywhere, only points on the boundary will be recomputed.
    /// Also recomputes vertex normals.
    /// See GEOcomputeNormals() as well.
    void 	updateAffectedNormals(const GA_Range &ptrange, 
				GEO_Delta *geodelta = 0, 
				bool allsametransform = false);

    /// Slide edges along adjacent primitive edges.  The transform matrix is
    /// used to compute translation deltas for the edge endpoints.
    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    template<typename FLOAT_T>
    void	 slideTransformEdges(const UT_Matrix4T<FLOAT_T> &mat,
				const GA_EdgeGroup *edgegrp = 0,
				const GA_PrimitiveGroup *primclosure = 0,
				bool just_P = false,
				bool keep_vector_lengths = true,
				bool check_pasting = true,
				bool neg_determinant_flipvector = true,
				bool update_ptnormals = false,
				GEO_Delta *geodelta = 0,
                                bool updateaffectednormals = false);

    /// Slide points across the face of adjacent primitives.
    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    template<typename FLOAT_T>
    void	 slideTransformPoints(const UT_Matrix4T<FLOAT_T> &mat,
				const GA_PointGroup *ptgrp = 0,
				const GA_PrimitiveGroup *primclosure = 0,
				bool just_P = false,
				bool keep_vector_lengths = true,
				bool check_pasting = true,
				bool neg_determinant_flipvector = true,
				bool update_ptnormals = false,
				GEO_Delta *geodelta = 0,
                                bool updateaffectednormals = false,
				GEO_PointSliderCache *slider_cache = 0);

    /// Slide primitives (by their points) across the face of adjacent
    /// primitives.
    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    template<typename FLOAT_T>
    void	 slideTransformPrimitives(const UT_Matrix4T<FLOAT_T> &mat,
				const GA_PrimitiveGroup *primgrp = 0,
				const GA_PointGroup *ptclosure = 0,
				bool just_P = false,
				bool keep_vector_lengths = true,
				bool check_pasting = true,
				bool neg_determinant_flipvector = true,
				bool update_ptnormals = false,
				GEO_Delta *geodelta = 0,
                                bool updateaffectednormals = false,
				GEO_PointSliderCache *slider_cache = 0);

    /// Convenience function, will call the appropriate slide transform
    /// depending on the type of group
    /// Don't use update_ptnormals, except for compatibility; use updateaffectednormals.
    /// The GEO_PointSliderCache argument is unused for edge groups.
    template<typename FLOAT_T>
    void 	 slideTransformGroup(const UT_Matrix4T<FLOAT_T> &mat,
                                const GA_Group &grp,
 		                bool just_P = false,
				bool keep_vector_lengths = true,
				bool check_pasting = true,
				bool neg_determinant_flipvector = true,
				const GA_PointGroup *ptclosure = 0,
				const GA_PrimitiveGroup *primclosure = 0,
				bool update_ptnormals = false,
				GEO_Delta *geodelta = 0,
                                bool updateaffectednormals = false,
				GEO_PointSliderCache *slider_cache = 0);


    /// Perform the transformation given by t*, s*, p*, s_*, in the
    /// coordinate frame defined by lookat(zero, forward, up).
    static UT_Matrix4 	localSpaceXformMatrix(const UT_XformOrder &order,
					      const UT_Vector3 &forward,
					      const UT_Vector3 &up,
					      float tx, float ty, float tz,
					      float rx, float ry, float rz,
					      float sx, float sy, float sz,
					      float s_xy, float s_xz, float s_yz,
					      float px, float py, float pz,
					      float prx, float pry, float prz);

    /// Class for passing local context arguments to certain softTransform*()
    /// methods.
    class GEO_API LocalXformContext
    {
    public:
	LocalXformContext(const UT_Vector3 &avg_normal, const UT_Vector3 &up)
	    : myAvgNormal(avg_normal), myUpVector(up) {}

	UT_Vector3	 myAvgNormal;
	UT_Vector3	 myUpVector;
    };

    /// Transform points using soft-selections (proportional xform):
    ///   symmetry - the soft falloff from any points not directly on the plane
    ///              of symmetry will be clamped to that plane of symmetry when
    ///              provided
    ///   falloff_output - any non-zero falloffs used during this call will be
    ///                    written to this attribute when provided
    ///   falloff_written - will be set to true when provided if falloff_output
    ///                     is provided, and this call did not skip processing
    ///			    any non-zero falloffs.  If not set, you must call
    ///			    computeSoftPointFalloff() to obtain the falloffs.
    void	 softTransformPoints(const UT_XformOrder &order,
				     float tx, float ty, float tz,
				     float rx, float ry, float rz,
				     float sx, float sy, float sz,
				     float s_xy, float s_xz, float s_yz,
				     float px, float py, float pz,
				     float prx, float pry, float prz,
				     const GEO_Rolloff &rolloff,
				     const GA_PointGroup *ptgroup = 0,
				     const SoftSymmetryParms *symmetry = 0,
				     int just_P = 0,
				     int keep_vector_lengths = 1,
				     int check_pasting = 1,
				     int neg_determinant_flipvector = 1,
				     int update_ptnormals = 0,
				     GEO_Delta *geodelta = 0,
                                     bool updateaffectednormals = false,
                                     const char *attribpattern = NULL,
				     const GA_RWHandleF *falloff_output = NULL,
				     bool *falloff_written = NULL);
    void	 softTransformEdges(const UT_XformOrder &order,
				     float tx, float ty, float tz,
				     float rx, float ry, float rz,
				     float sx, float sy, float sz,
				     float s_xy, float s_xz, float s_yz,
				     float px, float py, float pz,
				     float prx, float pry, float prz,
				     const GEO_Rolloff &rolloff,
				     const GA_EdgeGroup *edgegrp = 0,
				     const SoftSymmetryParms *symmetry = 0,
				     int just_P = 0,
				     int keep_vector_lengths = 1,
				     int check_pasting = 1,
				     int neg_determinant_flipvector = 1,
				     int update_ptnormals = 0,
				     GEO_Delta *geodelta = 0,
                                     bool updateaffectednormals = false,
                                     const char *attribpattern = NULL,
				     const GA_RWHandleF *falloff_output = NULL,
				     bool *falloff_written = NULL);

    void	 softTransformPoints(const LocalXformContext *local,
				     const UT_XformOrder &order,
				     float tx, float ty, float tz,
				     float rx, float ry, float rz,
				     float sx, float sy, float sz,
				     float s_xy, float s_xz, float s_yz,
				     float px, float py, float pz,
				     float prx, float pry, float prz,
				     const GEO_Rolloff &rolloff,
				     const GA_PointGroup *ptgroup = 0,
				     const SoftSymmetryParms *symmetry = 0,
				     int just_P = 0,
				     int keep_vector_lengths = 1,
				     int check_pasting = 1,
				     int neg_determinant_flipvector = 1,
				     int update_ptnormals = 0,
				     GEO_Delta *geodelta = 0,
                                     bool updateaffectednormals = false,
                                     const char *attribpattern = NULL,
				     const GA_RWHandleF *falloff_output = NULL,
				     bool *falloff_written = NULL);
    void	 softTransformEdges(const LocalXformContext *local,
				     const UT_XformOrder &order,
				     float tx, float ty, float tz,
				     float rx, float ry, float rz,
				     float sx, float sy, float sz,
				     float s_xy, float s_xz, float s_yz,
				     float px, float py, float pz,
				     float prx, float pry, float prz,
				     const GEO_Rolloff &rolloff,
				     const GA_EdgeGroup *edgegrp = 0,
				     const SoftSymmetryParms *symmetry = 0,
				     int just_P = 0,
				     int keep_vector_lengths = 1,
				     int check_pasting = 1,
				     int neg_determinant_flipvector = 1,
				     int update_ptnormals = 0,
				     GEO_Delta *geodelta = 0,
                                     bool updateaffectednormals = false,
                                     const char *attribpattern = NULL,
				     const GA_RWHandleF *falloff_output = NULL,
				     bool *falloff_written = NULL);

    /// Translate points using soft-selections (proportional xlate):
    ///   symmetry - the soft falloff from any points not directly on the plane
    ///              of symmetry will be clamped to that plane of symmetry when
    ///              provided
    ///   falloff_output - any non-zero falloffs used during this call will be
    ///                    written to this attribute when provided
    ///   falloff_written - will be set to true when provided if falloff_output
    ///                     is provided, and this call did not skip processing
    ///			    any non-zero falloffs.  If not set, you must call
    ///			    computeSoftPointFalloff() to obtain the falloffs.
    void	 softTranslatePoints(const UT_Vector3 &delta,
				     const GEO_Rolloff &rolloff,
				     const GA_PointGroup *ptgroup = 0,
				     const SoftSymmetryParms *symmetry = 0,
				     int check_pasting = 1,
				     int update_ptnormals = 0,
				     GEO_Delta *geodelta = 0,
                                     bool updateaffectednormals = false,
                                     const char *attribpattern = NULL,
				     const GA_RWHandleF *falloff_output = NULL,
				     bool *falloff_written = NULL);
    void	 softTranslateEdges(const UT_Vector3 &delta,
				     const GEO_Rolloff &rolloff,
				     const GA_EdgeGroup *edgegrp = 0,
				     const SoftSymmetryParms *symmetry = 0,
				     int check_pasting = 1,
				     int update_ptnormals = 0,
				     GEO_Delta *geodelta = 0,
                                     bool updateaffectednormals = false,
                                     const char *attribpattern = NULL,
				     const GA_RWHandleF *falloff_output = NULL,
				     bool *falloff_written = NULL);

    /// Translate points along their normals using soft-selections
    ///   symmetry - the soft falloff from any points not directly on the plane
    ///              of symmetry will be clamped to the plane of symmetry when
    ///              provided
    ///   falloff_output - any non-zero falloffs used during this call will be
    ///                    written to this attribute when provided
    ///   falloff_written - will be set to true when provided if falloff_output
    ///                     is provided, and this call did not skip processing
    ///			    any non-zero falloffs.  If not set, you must call
    ///			    computeSoftPointFalloff() to obtain the falloffs.
    void	 softNormalTranslatePoints(float distance,
					   const GEO_Rolloff &rolloff,
					   const GA_PointGroup *ptgroup = 0,
					   const SoftSymmetryParms *symmetry
									= 0,
					   int along_lead_normal = 1,
					   int check_pasting = 1,
					   int update_ptnormals = 0,
					   GEO_Delta *geodelta = 0,
                                           bool updateaffectednormals = false,
					   const GA_RWHandleF *falloff_output
									= NULL,
					   bool *falloff_written = NULL);

    /// Add and compute normal attributes.  Returns the normal attribute.
    /// If given a primgroup, only points within the selection are used
    /// in the calculation, all others are zeroed.
    GA_Attribute *normal(bool internal = false,
                             const GA_PrimitiveGroup *primgroup = NULL);

    /// Return normals computed from the geometry, ignoring the "N" attribute.
    void normal(UT_Vector3Array &output, bool use_internaln = true) const;

    /// Compute normals into the existing attribute given.  The attribute
    /// can be detached, and the detail won't ever be modified.
    void normal(const GA_RWHandleV3 &attrib,
                const GA_PrimitiveGroup *primgroup = NULL) const;

    /// Add and compute vertex normal attributes. Returns the normals.
    void getVertexNormals(UT_Vector3Array &output) const;

    /// Methods for manipulating capture weights (GEO_CaptureWeight.C)
    enum CaptureType
    {
	CAPTURE_BONE = 0,
	CAPTURE_META,
	CAPTURE_WIRE,
	CAPTURE_CLOTH,
	CAPTURE_MUSCLE
    };

    /// Get the name of the point attribute which contains (region, weight)
    /// pairs of capture data.
    static const UT_StringHolder
			&getPointCaptureAttribName(CaptureType type);

    /// Get the name of the property which contains an index of
    /// region names (like capture regions and metaball groups). Returns NULL
    /// if no such region mapping exists for a type.
    static const char 	*getPointCaptureIndexPropertyName(CaptureType type);

    /// Get the name of the detail attribute for capture frame. Returns NULL
    /// if a capture type doesn't use capture frame.
    static const UT_StringHolder
			&getPointCaptureFrameAttribName(CaptureType type);

    /// Get the name of the detail attribute for capture root. Returns NULL
    /// if a capture type doesn't use capture frame.
    static const UT_StringHolder
			&getPointCaptureRootAttribName(CaptureType type);

    /// Get the name of the property for capture data (usually,
    /// a linear array of transformation matrices + possibly other data).
    /// Returns NULL if a capture type doesn't use a capture data attribute.
    static const char	*getPointCaptureDataPropertyName(CaptureType type);

    // Get the cregion root path. Returns true if successful with the value
    // in root_path.
    bool	 getCaptureRegionRootPath(UT_String &root_path,
					  CaptureType t = CAPTURE_BONE) const;
    bool	 setCaptureRegionRootPath(const UT_String &root_path,
					  CaptureType t = CAPTURE_BONE);

    /// Given a string representing a capture region for type t, return that
    /// region's index. -1 is returned if no index exists.
    int			 getCaptureRegionIndex(const UT_String &cregion_name,
				       CaptureType t = CAPTURE_BONE);

    /// Add a new bone capture region. Sets the detail's capture transforms
    /// for the new region. Returns the index for this new region or -1 for
    /// failure.
    int			addCaptureRegion(const UT_String &cregion_name,
				  const UT_Matrix4 &parent_xform,
				  const GEO_Detail *tube_gdp);

    /// Add a non-bone capture capture region of type type to this detail.
    /// Returns the index for this new region, or -1 for failure.
    int			addCaptureRegion(const UT_String &cregion_name,
				  CaptureType type);
    
    /// Delete a capture region (detail and point attributes) data.
    /// Return index of deleted attribute, -1 if cannot delete capture region
    int			removeCaptureRegion(const UT_String &cregion_name,
				     CaptureType t = CAPTURE_BONE);
    
    /// Set the capture weight on a group of points
    void	 setCaptureWeight(int cregion_index, float weight,
				  const GA_PointGroup *ptgroup = NULL,
				  GEO_Delta *geodelta = NULL,
				  bool add_weight = false,
				  bool clamp_negative = false,
				  bool normalizeweight = false,
				  CaptureType t = CAPTURE_BONE);

    /// Set the capture weight on a point
    void	 setCaptureWeight(int cregion_index, float weight,
				  GA_Offset ptoff,
				  GEO_Delta *geodelta = NULL,
				  bool add_weight = false,
				  bool clamp_negative = false,
				  bool normalizeweight = false,
				  CaptureType t = CAPTURE_BONE);

    /// Get the capture weight of a point
    float 	 getCaptureWeight(int cregion_index, GA_Offset ptoff,
				  CaptureType t = CAPTURE_BONE) const;

    /// Get the average capture weight of a set of points
    bool	 addAverageCaptureWeight(const GA_PointGroup *ptgroup,
					 const UT_String &root_path,
					 UT_StringMap<float *> &averages ) const;

    void 	 getCaptureWeights(GA_Offset ptoff, UT_IntArray &regions,
				   UT_FloatArray &weights,
				   CaptureType t = CAPTURE_BONE) const;
    void 	 clearCaptureWeights(GA_Offset ptoff,
				     CaptureType t = CAPTURE_BONE);

    enum VisualizationType
    {
	VISTYPE_SINGLE,
	VISTYPE_MULTIPLE
    };

    /// Color points according to their capture (normalized) weight
    void	 colorPointsByCaptureWeight(
			    const UT_Array<const UT_Vector3 *> &colors,
			    const UT_Vector3 &zero_weight_color,
			    CaptureType t = CAPTURE_BONE,
			    VisualizationType vistype = VISTYPE_MULTIPLE,
			    UT_ColorRamp singlevismode = UT_COLORRAMP_FALSE);

    /// Color points according to their capture weight, with option to
    /// normalize the weights for visualization.
    void	 colorPointsByCaptureWeight(
			    const UT_Array<const UT_Vector3 *> &colors,
			    const UT_Vector3 &zero_weight_color,
			    CaptureType t,
			    VisualizationType vistype,
			    UT_ColorRamp singlevismode,
			    bool normalize);

    void	 normalizeCaptureWeights(const GA_PointGroup *ptgroup = NULL,
					 CaptureType t = CAPTURE_BONE);
    
    /// clamp the capture weights to the given low/hi values, if the
    /// corresponding flag is true and the capture weight is below/above
    /// the corresponding threshold. If low/hi flag is false, the capture
    /// weight will be unaffected for respecive clamping.
    void	 clampCaptureWeights(const GA_PointGroup *ptgroup = NULL,
			    bool clamp_low_flag = true,
			    float low_threshold = 0.0f, float low_val = 0.0f, 
			    bool clamp_hi_flag  = false,
			    float hi_threshold  = 1.0f, float hi_val  = 1.0f,
			    CaptureType t = GEO_Detail::CAPTURE_BONE);

    /// This specifies a capture index that is exempt from normalization.
    void	 normalizeRelativeCaptureWeights(int idx, GA_Offset ptoff,
						 GEO_Delta *delta=NULL);

    ///
    /// Methods for adding to the point list.
    ///
    GA_Offset	 insertPointOffset(GA_Index beforeWhich = GA_INVALID_INDEX);
    SYS_FORCE_INLINE
    GA_Offset	 appendPointOffset() { return GA_Detail::appendPoint(); }
    /// Insert a point, copying data from the source point.  If the @c
    /// beforeWhich index isn't specified, the point will be added at the
    /// end of the point list.
    GA_Offset	 insertPointCopy(GA_Offset src,
			GA_PointWrangler &wrangler,
			GA_Index beforeWhich = GA_INVALID_INDEX);
    /// Insert a point, copying data from the source point.  If the @c
    /// beforeWhich index isn't specified, the point will be added at the
    /// end of the point list.
    /// @note It is more efficient to use the GA_PointWrangler version.
    GA_Offset	 insertPointCopy(GA_Offset src,
			GA_Index beforeWhich = GA_INVALID_INDEX);

    /// Append a point, copying data from the source point.
    GA_Offset	 appendPointCopy(GA_Offset src, GA_PointWrangler &wrangler)
				    { return insertPointCopy(src, wrangler); }
    /// Append a point, copying data from the source point.
    /// @note It is more efficient to use the GA_PointWrangler version.
    GA_Offset	 appendPointCopy(GA_Offset src)
				    { return insertPointCopy(src); }

    /// Provide a few additional overloads of deletePoints() in addition
    /// to those available in GA_Detail.
    SYS_FORCE_INLINE
    GA_Size	 	deletePoints(const GA_PointGroup &point_group,
				GA_DestroyPointMode mode=GA_LEAVE_PRIMITIVES)
			{
			    return destroyPoints(GA_Range(point_group), mode);
			}

    /// Get a GEO_Primitive pointer by primitive offset
    /// @{
    SYS_FORCE_INLINE
    GEO_Primitive *	    getGEOPrimitive(GA_Offset primoff)
			    {
				if (!GAisValid(primoff))
				    return NULL;
                                // reinterpret_cast to avoid including header
				return reinterpret_cast<GEO_Primitive*>(
					getPrimitive(primoff));
			    }
    SYS_FORCE_INLINE
    const GEO_Primitive *   getGEOPrimitive(GA_Offset off) const
			    {
				if (!GAisValid(off))
				    return NULL;
				return reinterpret_cast<const GEO_Primitive*>(
					getPrimitive(off));
			    }
    /// @}

    /// Get a GEO_Primitive pointer by primitive index
    /// @{
    GEO_Primitive *	    getGEOPrimitiveByIndex(GA_Index idx)
			    {
				if (!GAisValid(idx))
				    return NULL;
				return reinterpret_cast<GEO_Primitive*>(
					getPrimitiveByIndex(idx));
			    }
    const GEO_Primitive *   getGEOPrimitiveByIndex(GA_Index idx) const
			    {
				if (!GAisValid(idx))
				    return NULL;
				return reinterpret_cast<const GEO_Primitive*>(
					getPrimitiveByIndex(idx));
			    }
    /// @} 

    /// @{
    /// GA uses "destroy" instead of "delete" as the verb.  However, the old
    /// GEO/GB code made extensive use of "delete".  These methods are to save
    /// the sanity of people trying to port code.
    GA_Size	deletePoints(const GA_Range &range,
				GA_DestroyPointMode mode=GA_LEAVE_PRIMITIVES)
		    { return destroyPoints(range, mode); }
    GA_Size	deletePrimitives(const GA_Range &range, bool and_points=false)
		    { return destroyPrimitives(range, and_points); }
    bool	deletePrimitive(GA_Primitive &prim, bool and_points=false)
		    { return destroyPrimitive(prim, and_points); }
    /// @}

    ///	Inserting and appending a new primitive
    ///	Insertion works the same way as insertion of points.  However, the
    /// primitive is not allocated beforehand, leaving this to the allocator.
    /// If the source primitive is from another detail, our primitive attribute
    /// dictionary must be an extension of that detail's (see
    /// sortAllAttributes()).  Note that this last condition is the reverse of
    /// that for inserting or appending a copy of a point.
    GEO_Primitive	*insertPrimitive(const GA_PrimitiveTypeId &type,
				const GEO_Primitive *beforeWhich = NULL);
    SYS_FORCE_INLINE
    GEO_Primitive *appendPrimitive(const GA_PrimitiveTypeId &type)
    {
        return static_cast<GEO_Primitive *>(GA_Detail::appendPrimitive(type));
    }

    /// Delete the primitive at the given offset.  Optionally, delete the points
    /// referenced by the primitive.
    virtual bool	 destroyPrimitiveOffset(GA_Offset offset,
					       bool and_points = false);

    /// Delete a range of primitives.  Optionally, delete the points referenced
    /// by those primitives.
    virtual GA_Size	 destroyPrimitiveOffsets(const GA_Range &it,
						bool and_points = false);

    /// Provide a few additional overloads of deletePrimitives() in addition
    /// to those available in GA_Detail.
    GA_Size	 	 deletePrimitives(const GA_PrimitiveGroup &primGrp,
					  bool and_points = false);
    GA_Size		 deletePrimitives(
				    const UT_Array<GEO_Primitive*> &prims_in,
				    bool and_points = false);

    /// Delete a whole group of breakpoints.
    /// @note The group is cleared when done (non-const operation)
    virtual void	 deleteBreakpoints(const GA_BreakpointGroup &bkptgrp);

    //	Find the bounding box (optionally of a group of primitives)
    // instantiation. Return 0 if the box is empty and 1 otherwise.
    int			 getBBox     (UT_BoundingBox *bbox,
				      const GA_PrimitiveGroup *g=0) const;

    ///	Find the bounding box (optionally of a group of primitives) of transformed
    /// instantiation. Return 0 if the box is empty and 1 otherwise.
    int			 getBBox     (UT_BoundingBox& bbox,
                                      const UT_Matrix4R & transform,
				      const GA_PrimitiveGroup *g=0) const;

    virtual int		 getBSphere  (UT_BoundingSphere *bsphere,
				      const GA_PrimitiveGroup *g=0,
				      int updateRequired = 0) const;

    ///	Find the bounding box (optionally of a group of points)
    /// instantiation. Return 0 if the box is empty and 1 otherwise.
    int		 getPointBBox(UT_BoundingBox *bbox,
			      const GA_PointGroup *g = 0) const;
    int		 getPointBBox(UT_BoundingBox& bbox,
			      const UT_Matrix4R &transform,
			      const GA_PointGroup *g=0) const;
    virtual int		 getPointBSphere(UT_BoundingSphere *bsphere,
				      const GA_PointGroup *g = 0,
				      int accuratebounds = 0) const;
    int		 getVertexBBox(UT_BoundingBox *bbox,
			       const GA_VertexGroup *g = 0) const;
    int		 getVertexBBox(UT_BoundingBox& bbox,
			       const UT_Matrix4R &transform,
			       const GA_VertexGroup *g = 0) const;
    int		 getEdgeBBox(UT_BoundingBox &bbox,
			     const GA_EdgeGroup &grp) const;
    int		 getEdgeBBox(UT_BoundingBox &bbox, const UT_Matrix4R &transform,
			     const GA_EdgeGroup &grp) const;
    virtual int		 getEdgeBSphere(UT_BoundingSphere &bsphere,
				      const GA_EdgeGroup &grp,
				      int accuratebounds = 0) const;
    virtual int		 getBreakpointBBox(UT_BoundingBox &bbox,
					   const GA_BreakpointGroup &grp) const;
    virtual int		 getBreakpointBBox(UT_BoundingBox &bbox,
					   const UT_Matrix4R &transform,
					   const GA_BreakpointGroup &grp) const;
    virtual int		 getBreakpointBSphere(UT_BoundingSphere &bsphere,
				      const GA_BreakpointGroup &grp,
				      int accuratebounds = 0) const;
    virtual int		 getGroupBBox(UT_BoundingBox *bbox,
				      const GA_Group *grp) const;
    virtual int		 getGroupBBox(UT_BoundingBox& bbox,
				      const UT_Matrix4R &transform,
				      const GA_Group *grp) const;
    virtual int		 getGroupBSphere(UT_BoundingSphere *bsphere,
				      const GA_Group *grp,
				      int accuratebounds) const;

    /// The following method computes either a 2D or 3D bounding box
    /// for the specified float attribute.
    int		 	 getPointAttribBBox(const UT_StringRef &name,
					    UT_BoundingBox *bbox,
					    const GA_PointGroup *g = 0) const;
    int		 	 getPointAttribBBox(const UT_StringRef &name,
					    UT_BoundingBox *bbox,
					    const GA_PrimitiveGroup &g) const;
    int		 	 getPointAttribBBox(const UT_StringRef &name,
					    UT_BoundingBox *bbox,
					    const GA_EdgeGroup &g) const;
    int		 	 getPointAttribBBox(const UT_StringRef &name,
					    UT_BoundingBox *bbox,
					    const GA_VertexGroup &g) const;

    /// Compute the 3D centroid with the given attribute and group.
    /// typename T can either be single or double precision.
    /// Returns false if there are no points. Behaviour is undefined if the
    /// attribute has less than 3 components.
    template <typename T>
    bool	 	 getPointAttribCentroid(
					    UT_Vector3T<T> &centroid,
					    const GA_Attribute &attrib,
					    const GA_PointGroup *grp = 0) const;

    int		 	 getPointAttribGroupBBox(const UT_StringRef &name,
				      UT_BoundingBox *bbox,
				      const GA_Group *grp) const;

    int		 	 getVertexAttribBBox(const UT_StringRef &name,
					     UT_BoundingBox *bbox,
					     const GA_PointGroup *g = 0) const;
    int		 	 getVertexAttribBBox(const UT_StringRef &name,
					     UT_BoundingBox *bbox,
					     const GA_PrimitiveGroup &g) const;
    int		 	 getVertexAttribBBox(const UT_StringRef &name,
					     UT_BoundingBox *bbox,
					     const GA_EdgeGroup &g) const;
    int		 	 getVertexAttribBBox(const UT_StringRef &name,
					     UT_BoundingBox *bbox,
					     const GA_VertexGroup &g) const;

    int		 	 getVertexAttribGroupBBox(const UT_StringRef &name,
				      UT_BoundingBox *bbox,
				      const GA_Group *grp) const;

    /// Copy vertex attributes from source to destination
    SYS_DEPRECATED_HDK(13.0)
    void		 copyVertexAttributes(GEO_Vertex dest,
					const GEO_Vertex &src) const;
    void		 copyVertexAttributes(GA_Offset dest, GA_Offset src,
					GA_VertexWrangler &wrangler) const;
    /// Copy vertex AND point attributes
    SYS_DEPRECATED_HDK(13.0)
    void		 copyPointVertexAttributes(GEO_Vertex dest,
					const GEO_Vertex &src,
					bool copy_point_P = true) const;
    void		 copyPointVertexAttributes(
				    GA_Offset dest_vtx, GA_Offset src_vtx,
				    const GA_AttributeRefMap &vertex_attribs,
				    const GA_AttributeRefMap &point_attribs);
    void		 copyPointVertexAttributes(
				    GA_Offset dest_vtx, GA_Offset src_vtx,
				    GA_VertexWrangler &vertex_wrangler,
				    GA_PointWrangler &point_wrangler,
				    bool copy_point_P)
			 {
			     copyVertex(dest_vtx, src_vtx, vertex_wrangler,
					point_wrangler, copy_point_P);
			 }
    /// Copy vertex attributes AND change the point reference of the vertex
    SYS_DEPRECATED_HDK(13.0)
    void		 copyFullVertex(GEO_Vertex dest,
					const GEO_Vertex &src) const;
    /// Copy vertex attributes AND change the point reference of the vertex
    void		 copyFullVertex(GA_Offset dest_vtx,
					GA_Offset src_vtx,
					GA_VertexWrangler &vertex_wrangler)
			 {
			     copyVertex(dest_vtx, src_vtx, vertex_wrangler,
					NULL);
			 }
    /// Copy primitive attributes
    void		 copyPrimitiveAttributes(GEO_Primitive &dest,
					const GEO_Primitive &src);

    void		 copyPrimitiveGroups(GEO_Primitive &dest,
					const GEO_Primitive &src) const;

    /// Create a copy of the source vertex. Regardless of the "shallow" parm,
    /// this method will copy the vertex attribute data. If not shallow, it
    /// will also copy the point attribute data and will assign the source
    /// point position to the destination point. If shallow, it will assign the
    /// source point offset to the destination.
    SYS_DEPRECATED_HDK(13.0)
    void		 copyVertex(GEO_Vertex dest, const GEO_Vertex &src,
				    int shallow = 0) const;

    /// Inherit the simple version of copyVertex from GA_Detail.
    using GA_Detail::copyVertex;

    /// Create a copy of the source vertex, copying only the vertex attributes
    /// specified by "vertex_attribs" and only the point attributes specified
    /// by the optional "point_attribs".  If "point_attribs" is NULL, then a
    /// shallow point copy is performed wiring the source's point (if any) to
    /// dest.
    void		 copyVertex(GA_Offset dest, GA_Offset src,
				    GA_AttributeRefMap &vertex_attribs,
				    GA_AttributeRefMap *point_attribs);

    /// Create a copy of the source vertex using the supplied wranglers to
    /// copy attributes.  If "point_wrangler" is NULL, then a shallow point
    /// copy is performed, wiring the source's point (if any) to dest.
    void		 copyVertex(GA_Offset dest, GA_Offset src,
				    GA_VertexWrangler &vertex_wrangler,
				    GA_PointWrangler *point_wrangler);

    /// Create a copy of the source vertex using the supplied wranglers to
    /// copy attributes.  Unlike the previous method which takes an optional
    /// "point_wrangler", this overload always performs a non-shallow point
    /// copy and specifies whether or not "P" should be copied, regardless of
    /// the setting of the point wrangler.
    void		 copyVertex(GA_Offset dest, GA_Offset src,
				    GA_VertexWrangler &vertex_wrangler,
				    GA_PointWrangler &point_wrangler,
				    bool copy_point_p);

    ///  The following method will fix all rational curve/patch weights by
    ///	flagging the curve/patch appropriately. It is quite expensive since
    ///  it traverses the entire primitive list. Only call it as a last resort.
    void		 fixSplineWeights();

    ///  Some useful alias functions, they simplify your life a lot
    ///  Attribute handling
    ///
    /// These alias methods return the offset of the (new) attribute, or -1 if
    /// they failed for some reason.

    GA_Attribute *addNormalAttribute(GA_AttributeOwner who,
					    GA_Storage s = GA_STORE_INVALID);
    GA_Attribute *addTextureAttribute(GA_AttributeOwner who,
					    GA_Storage s = GA_STORE_INVALID);
    GA_Attribute *addVelocityAttribute(GA_AttributeOwner who,
					    GA_Storage s = GA_STORE_INVALID);
    GA_Attribute *addAngularVelocityAttribute(GA_AttributeOwner who,
                                            GA_Storage s = GA_STORE_INVALID);
    GA_Attribute *addOrientAttribute(GA_AttributeOwner who,
                                            GA_Storage s = GA_STORE_INVALID);
    GA_Attribute *addAlphaAttribute(GA_AttributeOwner who,
					    GA_Storage s = GA_STORE_INVALID);
    GA_Attribute *addComAttribute(GA_AttributeOwner who,
					    GA_Storage s = GA_STORE_INVALID);
    GA_Attribute *addDiffuseAttribute(GA_AttributeOwner who,
					    GA_Storage s = GA_STORE_INVALID);
    GA_Attribute *addDistanceAttribute(GA_AttributeOwner who,
					    GA_Storage s = GA_STORE_INVALID);
    GA_Attribute *addLODAttribute(GA_AttributeOwner who,
					    GA_Storage s = GA_STORE_INVALID);
    GA_Attribute *addRestAttribute(GA_AttributeOwner who,
					    GA_Storage s = GA_STORE_INVALID);
    GA_Attribute *addTubeCaptureAttribute(
					    GA_Storage s = GA_STORE_INVALID);

    /// Add the detail's capture frame attribute for capture type t.
    GA_Attribute *addCaptureFrameAttribute(CaptureType t = CAPTURE_BONE,
					    GA_Storage s = GA_STORE_INVALID);

    /// Add the bone capture skeleton root detail attribute.
    GA_Attribute *addCaptureSkelRootAttribute(CaptureType t
								= CAPTURE_BONE);

    /// Add a new custom (index, weight) point attribute.
    /// TODO: geo_NPairs is temporary to force examination of old signature
    ///       uses, which took size in bytes instead of the # of pairs.
    class GEO_API geo_NPairs
    {
	public:
	    explicit geo_NPairs(int v) : myValue(v) {} 

	    int		 getValue() const { return myValue; }
	private:
	    int		 myValue;
    };
    GA_Attribute *addPtIndexPairAttribute(const UT_StringHolder &name,
						 geo_NPairs n_pairs,
						 GA_Storage s=GA_STORE_INVALID);

    /// Add the (index, weight) point attribute for capture type t.
    GA_Attribute *addPointCaptureAttribute(geo_NPairs n_pairs,
						  CaptureType t = CAPTURE_BONE,
						  GA_Storage s =
							    GA_STORE_INVALID);

    /// Add the bone capture alpha point attribute.
    GA_Attribute *addPointCaptureAlphaAttribute(float dflt,
					    GA_Storage s = GA_STORE_INVALID);

    const GA_Attribute  *findNormalAttribute(GA_AttributeOwner who) const;
    GA_Attribute        *findNormalAttribute(GA_AttributeOwner who);
    const GA_Attribute  *findTextureAttribute(GA_AttributeOwner who) const;
    GA_Attribute        *findTextureAttribute(GA_AttributeOwner who);
    const GA_Attribute  *findVelocityAttribute(GA_AttributeOwner who) const;
    GA_Attribute        *findVelocityAttribute(GA_AttributeOwner who);
    const GA_Attribute  *findAlphaAttribute(GA_AttributeOwner who) const;
    GA_Attribute        *findAlphaAttribute(GA_AttributeOwner who);
    const GA_Attribute  *findComAttribute(GA_AttributeOwner who) const;
    GA_Attribute        *findComAttribute(GA_AttributeOwner who);
    const GA_Attribute  *findDiffuseAttribute(GA_AttributeOwner who) const;
    GA_Attribute        *findDiffuseAttribute(GA_AttributeOwner who);
    const GA_Attribute  *findDistanceAttribute(GA_AttributeOwner who) const;
    GA_Attribute        *findDistanceAttribute(GA_AttributeOwner who);
    const GA_Attribute  *findLODAttribute(GA_AttributeOwner who) const;
    GA_Attribute        *findLODAttribute(GA_AttributeOwner who);
    const GA_Attribute  *findRestAttribute(GA_AttributeOwner who) const;
    GA_Attribute        *findRestAttribute(GA_AttributeOwner who);
    const GA_Attribute  *findTubeCaptureAttribute() const;
    GA_Attribute        *findTubeCaptureAttribute();

    /// Find the material attribute if it exists.  This checks for the
    /// GEO_STD_ATTRIB_MATERIAL name.
    GA_ROHandleS	 findMaterialAttribute(GA_AttributeOwner who) const;

    /// The internal normal attribute is always a point attribute
    /// @todo Remove internalN code path with the H11 viewport?
    const GA_Attribute  *findInternalNormalAttribute() const;
    GA_Attribute        *findInternalNormalAttribute();

    /// Find the offset for the detail's capture frame attribute for capture
    /// type t.
    const GA_Attribute  *findCaptureFrameAttribute(
					    CaptureType t = CAPTURE_BONE) const;
    GA_Attribute        *findCaptureFrameAttribute(
					    CaptureType t = CAPTURE_BONE);

    /// Find the offset for the bone capture skeleton root detail attribute.
    const GA_Attribute  *findCaptureSkelRootAttribute(CaptureType t 
							  = CAPTURE_BONE) const;
    GA_Attribute        *findCaptureSkelRootAttribute(CaptureType t 
							  = CAPTURE_BONE);

    /// Find the offset for the (region, weight) point capture *point*
    /// attribute for capture type t.
    const GA_Attribute  *findPointCaptureAttribute(
					    CaptureType t = CAPTURE_BONE) const;
    GA_Attribute        *findPointCaptureAttribute(
					    CaptureType t = CAPTURE_BONE);
    /// Find the offset for the bone capture alpha point attribute.
    const GA_Attribute  *findPointCaptureAlphaAttribute() const;
    GA_Attribute        *findPointCaptureAlphaAttribute();

    /// Find the offset for a custom (index, weight) point attribute.
    const GA_Attribute  *findPtIndexPairAttribute(const UT_StringRef &name) const;
    GA_Attribute        *findPtIndexPairAttribute(const UT_StringRef &name);

    void		 destroyNormalAttribute(GA_AttributeOwner who);
    void		 destroyInternalNormalAttribute();
    void		 destroyTextureAttribute(GA_AttributeOwner who);
    void		 destroyVelocityAttribute(GA_AttributeOwner who);
    void		 destroyAlphaAttribute(GA_AttributeOwner who);
    void		 destroyComAttribute(GA_AttributeOwner who);
    void		 destroyDiffuseAttribute(GA_AttributeOwner who);
    void		 destroyDistanceAttribute(GA_AttributeOwner who);
    void		 destroyLODAttribute(GA_AttributeOwner who);
    void		 destroyRestAttribute(GA_AttributeOwner who);
    void		 destroyTubeCaptureAttribute();

    /// Destroy the detail's capture frame attribute for capture type t.
    void		 destroyCaptureFrameAttribute(CaptureType t 
								= CAPTURE_BONE);

    /// Destroy the bone capture skeleton root detail attribute.
    void		 destroyCaptureSkelRootAttribute(CaptureType t
								= CAPTURE_BONE);

    /// Destroy the (region, weight) point capture *point* attribute for 
    /// capture type t.
    void		 destroyPointCaptureAttribute(CaptureType t = 
								  CAPTURE_BONE);

    /// Destroy the bone capture alpha point attribute.
    void		 destroyPointCaptureAlphaAttribute();

    /// Destroy a custom (index, weight) point attribute.
    void		 destroyPtIndexPairAttribute(const UT_StringRef &name);

    /// Retrieve the restAnchor string attribute from this gdp, if
    /// it exists - return true if successful
    bool		 getMetaRestAnchorPath(UT_String &anchorPath) const;
    
    /// You should never assume that "uv" is textures!
    /// Note that Point capture is still special cased.
    /// If the supplied layer is <= 0, then the current layer is used.
    /// To extract all the layers names, one should do:
    /// for (layer = 1; layer <= getNumLayers(); layer++)
    ///    layername[layer] = getStdAttributeName(..., layer);
    ///
    /// WARNING: The returned UT_StringHolder may own the string it references,
    /// so you *must* either assign it to a UT_StringHolder or UT_String,
    /// or pass it to a function that takes a UT_StringHolder, UT_StringRef,
    /// const UT_StringHolder&, or const UT_StringRef&.
    /// *DO* *NOT* assign it to a const char* variable!  The UT_StringHolder
    /// will be destructed immediately after the assignment, deleting the
    /// string that the const char* points to!
    UT_StringHolder      getStdAttributeName(GEO_Standard_Attributes name,
					     int layer = -1) const;
    int			 getAttributeLayer(const char* attr_name) const;

    static GA_TypeInfo   getStdAttributeTypeInfo(const char *name, int floattuplesize);

    /// These handle layers:
    int			 getCurrentLayer() const;
    void		 setCurrentLayer(int layer);
    void		 setNumLayers(int numlayer);
    int			 getNumLayers() const;
    /// These get and set the CURRENT layer's merge mode:
    void		 setLayerMerge(int preclear, int mask, int srcblend,
					int dstblend);
    void		 getLayerMerge(int &preclear, int &mask, int &srcblend,
					int &dstblend) const;

    void		 addVariableName(const char *attr, const char *varname);
    void		 removeVariableName(const char *varname)
			    { addVariableName(0, varname); }
    /// This gets a raw list of the variable maps suitable for caching
    /// to see if they changed.
    void		 getVariableNameMap(UT_String &result) const;
    
    /// Traverses the variable name mappings currently present:
    void		 traverseVariableNames(int (*function)(const char *attr,
				    const char *varname, void *data), 
				    void *data) const;

    /// increase the memory size of the attribute
    GA_Attribute *growPointCaptureAttribute(geo_NPairs num_added_pairs,
						  CaptureType t = CAPTURE_BONE);

    SYS_FORCE_INLINE
    GA_Attribute *addPointAttrib(const GA_Attribute *src)
			 {
			     return getAttributes().cloneAttribute(
						GA_ATTRIB_POINT,
						src->getName(), *src, true);
			 }
    SYS_FORCE_INLINE
    GA_Attribute *addPointAttrib(GA_AttributeScope scope,
                                 const UT_StringHolder &n,
				 const UT_Options *creation_args,
				 const GA_AttributeOptions *attribute_options,
				 const UT_StringRef &attribtype)
			 {
			     return createAttribute(GA_ATTRIB_POINT, scope, n,
					creation_args, attribute_options,attribtype);
			 }
    SYS_FORCE_INLINE
    GA_Attribute *addPointAttrib(const UT_StringHolder &n,
					const UT_Options *creation_args,
					const GA_AttributeOptions *attribute_options,
					const UT_StringRef &attribtype)
			 {
			     return createAttribute(GA_ATTRIB_POINT, n,
					creation_args, attribute_options,attribtype);
			 }

    /// Replace old findPointAttribute(const char *name...) with
    /// findIntTuple(GA_ATTRIB_POINT, ...), findFloatTuple(),
    /// findStringTuple(), etc.
    SYS_FORCE_INLINE
    const GA_Attribute  *findPointAttrib(const GA_Attribute &src) const
			 { return findPointAttribute(src.getScope(),
						     src.getName()); }
    SYS_FORCE_INLINE
    GA_Attribute        *findPointAttrib(const GA_Attribute &src)
			 { return findPointAttribute(src.getScope(),
						     src.getName()); }

    /// Looks for a 3-float point attribute
    SYS_FORCE_INLINE
    const GA_Attribute  *findPointVectorAttrib(GA_AttributeScope s,
				    const UT_StringRef &n) const
			 { return findFloatTuple(GA_ATTRIB_POINT, s, n, 3, 3); }
    SYS_FORCE_INLINE
    const GA_Attribute  *findPointVectorAttrib(const UT_StringRef &n) const
			 { return findFloatTuple(GA_ATTRIB_POINT, n, 3, 3); }
    SYS_FORCE_INLINE
    GA_Attribute        *findPointVectorAttrib(GA_AttributeScope s,
				    const UT_StringRef &n)
			 { return findFloatTuple(GA_ATTRIB_POINT, s, n, 3, 3); }
    SYS_FORCE_INLINE
    GA_Attribute        *findPointVectorAttrib(const UT_StringRef &n)
			 { return findFloatTuple(GA_ATTRIB_POINT, n, 3, 3); }

    SYS_FORCE_INLINE
    void		 destroyPointAttrib(GA_AttributeScope scope,
				    const UT_StringRef &n,
				    const GA_AttributeFilter *filter=NULL)
			 { destroyAttribute(GA_ATTRIB_POINT,scope, n, filter); }
    SYS_FORCE_INLINE
    void		 destroyPointAttrib(const UT_StringRef &n,
				    const GA_AttributeFilter *filter=NULL)
			 { destroyAttribute(GA_ATTRIB_POINT, n, filter); }

    bool		 promotePointAttrib(GA_AttributeScope scope,
					    const UT_StringRef &name);
    SYS_FORCE_INLINE
    bool		 promotePointAttrib(const UT_StringRef &name)
			 { return promotePointAttrib(GA_SCOPE_PUBLIC, name); }

    SYS_FORCE_INLINE
    GA_Attribute        *addVertexAttrib(const GA_Attribute *src)
			 {
			     return getAttributes().cloneAttribute(
						GA_ATTRIB_VERTEX,
						src->getName(), *src, true);
			 }
    SYS_FORCE_INLINE
    GA_Attribute        *addVertexAttrib(GA_AttributeScope scope, const UT_StringHolder &n,
					const UT_Options *creation_args,
					const GA_AttributeOptions *attribute_options,
					const UT_StringRef &attribtype)
			 {
			     return createAttribute(GA_ATTRIB_VERTEX, scope, n,
					creation_args, attribute_options,attribtype);
			 }
    SYS_FORCE_INLINE
    GA_Attribute        *addVertexAttrib(const UT_StringHolder &n,
					const UT_Options *creation_args,
					const GA_AttributeOptions *attribute_options,
					const UT_StringRef &attribtype)
			 {
			     return createAttribute(GA_ATTRIB_VERTEX, n,
					creation_args, attribute_options,attribtype);
			 }

    /// Replace old findVertexAttribute(const char *name...) with
    /// findIntTuple(GA_ATTRIB_VERTEX, ...), findFloatTuple(),
    /// findStringTuple(), etc.
    SYS_FORCE_INLINE
    const GA_Attribute  *findVertexAttrib(const GA_Attribute &src) const
			 { return findVertexAttribute(src.getScope(),
						      src.getName()); }
    SYS_FORCE_INLINE
    GA_Attribute        *findVertexAttrib(const GA_Attribute &src)
			 { return findVertexAttribute(src.getScope(),
						      src.getName()); }

    /// Looks for a 3-float vertex attribute
    SYS_FORCE_INLINE
    const GA_Attribute  *findVertexVectorAttrib(GA_AttributeScope s,
					const UT_StringRef &n) const
			 { return findFloatTuple(GA_ATTRIB_VERTEX,s, n, 3, 3); }
    SYS_FORCE_INLINE
    const GA_Attribute  *findVertexVectorAttrib(const UT_StringRef &n) const
			 { return findFloatTuple(GA_ATTRIB_VERTEX, n, 3, 3); }
    SYS_FORCE_INLINE
    GA_Attribute        *findVertexVectorAttrib(GA_AttributeScope s,
					const UT_StringRef &n)
			 { return findFloatTuple(GA_ATTRIB_VERTEX,s, n, 3, 3); }
    SYS_FORCE_INLINE
    GA_Attribute        *findVertexVectorAttrib(const UT_StringRef &n)
			 { return findFloatTuple(GA_ATTRIB_VERTEX, n, 3, 3); }

    SYS_FORCE_INLINE
    void		 destroyVertexAttrib(GA_AttributeScope scope,
				const UT_StringRef &n,
				const GA_AttributeFilter *filter=NULL)
			 { destroyAttribute(GA_ATTRIB_VERTEX,scope, n,filter); }
    SYS_FORCE_INLINE
    void		 destroyVertexAttrib(const UT_StringRef &n,
				const GA_AttributeFilter *filter=NULL)
			 { destroyAttribute(GA_ATTRIB_VERTEX, n, filter); }

    bool		 demoteVertexAttrib(GA_AttributeScope scope,
					    const UT_StringRef &name);
    SYS_FORCE_INLINE
    bool		 demoteVertexAttrib(const UT_StringRef &name)
			 { return demoteVertexAttrib(GA_SCOPE_PUBLIC, name); }

    SYS_FORCE_INLINE
    GA_Attribute        *addPrimAttrib(const GA_Attribute *src)
			 {
			     return getAttributes().cloneAttribute(
						GA_ATTRIB_PRIMITIVE,
						src->getName(), *src,true);
			 }
    SYS_FORCE_INLINE
    GA_Attribute        *addPrimAttrib(GA_AttributeScope scope, const UT_StringHolder &n,
					const UT_Options *creation_args,
					const GA_AttributeOptions *attribute_options,
					const UT_StringRef &attribtype)
			 {
			     return createAttribute(GA_ATTRIB_PRIMITIVE, scope,
				n, creation_args, attribute_options,attribtype);
			 }
    SYS_FORCE_INLINE
    GA_Attribute        *addPrimAttrib(const UT_StringHolder &n,
					const UT_Options *creation_args,
					const GA_AttributeOptions *attribute_options,
					const UT_StringRef &attribtype)
			 {
			     return createAttribute(GA_ATTRIB_PRIMITIVE, n,
					creation_args, attribute_options,attribtype);
			 }

    /// Replace old findPrimAttribute(const char *name...) with
    /// findIntTuple(GA_ATTRIB_PRIMITIVE, ...), findFloatTuple(),
    /// findStringTuple(), etc.
    SYS_FORCE_INLINE
    const GA_Attribute  *findPrimAttrib(const GA_Attribute &src) const
			 { return findPrimitiveAttribute(src.getScope(),
							 src.getName()); }
    SYS_FORCE_INLINE
    GA_Attribute        *findPrimAttrib(const GA_Attribute &src)
			 { return findPrimitiveAttribute(src.getScope(),
							 src.getName()); }

    SYS_FORCE_INLINE
    void		 destroyPrimAttrib(GA_AttributeScope scope,
				const UT_StringRef &n,
				const GA_AttributeFilter *filter=NULL)
			 {
			     destroyAttribute(GA_ATTRIB_PRIMITIVE, scope, n,
					      filter);
			 }
    SYS_FORCE_INLINE
    void		 destroyPrimAttrib(const UT_StringRef &n,
				const GA_AttributeFilter *filter=NULL)
			 {
			     destroyAttribute(GA_ATTRIB_PRIMITIVE, n, filter);
			 }

    /// Note: if attrib already exists, will return pointer to existing
    /// attrib.
    SYS_FORCE_INLINE
    GA_Attribute        *addGlobalAttrib(const GA_Attribute *src)
			 {
			     return getAttributes().cloneAttribute(
						GA_ATTRIB_DETAIL,
						src->getName(), *src, true);
			 }
    SYS_FORCE_INLINE
    GA_Attribute        *addGlobalAttrib(GA_AttributeScope scope,
                                        const UT_StringHolder &n,
					const UT_Options *creation_args,
					const GA_AttributeOptions *attribute_options,
					const UT_StringRef &attribtype)
			 {
			     return createAttribute(GA_ATTRIB_DETAIL, scope, n,
					creation_args, attribute_options,attribtype);
			 }
    SYS_FORCE_INLINE
    GA_Attribute        *addGlobalAttrib(const UT_StringHolder &n,
					const UT_Options *creation_args,
					const GA_AttributeOptions *attribute_options,
					const UT_StringRef &attribtype)
			 {
			     return createAttribute(GA_ATTRIB_DETAIL, n,
					creation_args, attribute_options,attribtype);
			 }

    SYS_FORCE_INLINE
    const GA_Attribute  *findGlobalAttrib(const GA_Attribute &src) const
			 { return findGlobalAttribute(src.getScope(),
						      src.getName()); }
    SYS_FORCE_INLINE
    GA_Attribute        *findGlobalAttrib(const GA_Attribute &src)
			 { return findGlobalAttribute(src.getScope(),
						      src.getName()); }
    SYS_FORCE_INLINE
    void		 destroyGlobalAttrib(GA_AttributeScope scope,
				    const UT_StringRef &n,
				    const GA_AttributeFilter *filter=NULL)
			 { destroyAttribute(GA_ATTRIB_DETAIL,scope, n,filter); }
    SYS_FORCE_INLINE
    void		 destroyGlobalAttrib(const UT_StringRef &n,
				    const GA_AttributeFilter *filter=NULL)
			 { destroyAttribute(GA_ATTRIB_DETAIL, n, filter); }

    SYS_DEPRECATED_PUSH_DISABLE()
    /// Query the internal structures:
    SYS_DEPRECATED_HDK(13.0)
    GEO_PrimList		 primitives()
				 {
				     return GEO_PrimList(getPrimitiveMap()); 
				 }
    SYS_DEPRECATED_HDK(13.0)
    const GEO_PrimList		 primitives() const
				 {
				     return GEO_PrimList(getPrimitiveMap());
				 }
    SYS_DEPRECATED_POP_DISABLE()

    SYS_FORCE_INLINE
    const GA_AttributeDict	&pointAttribs() const
				{ return getAttributes().getDict(
						    GA_ATTRIB_POINT); }
    SYS_FORCE_INLINE
    const GA_AttributeDict	&primitiveAttribs() const
				{ return getAttributes().getDict(
						    GA_ATTRIB_PRIMITIVE); }
    SYS_FORCE_INLINE
    const GA_AttributeDict	&vertexAttribs() const
				{ return getAttributes().getDict(
						    GA_ATTRIB_VERTEX); }
    SYS_FORCE_INLINE
    const GA_AttributeDict	&attribs() const
				{ return getAttributes().getDict(
						    GA_ATTRIB_DETAIL); }

    // Typedef for the GA_Detail::GB_MACRO_CAST static methods.
    typedef GEO_Primitive GB_MACRO_PRIM_TYPE;

    bool	hasMetaPrimitives() const;
    bool	hasPastedPrimitives() const;
    bool	hasQuadricPrimitives() const;
    bool	hasParticlePrimitives() const;
    bool	hasVolumePrimitives() const;
    bool	hasTransformingPrimitives() const;

    /// Check to see whether the detail has any custom primitive types (i.e.
    /// non-factory primitives)
    bool	hasCustomPrimitives() const;

    GA_Size	getMetaCount() const;
    GA_Size	getPastedCount() const;
    GA_Size	getQuadricCount() const;
    GA_Size	getParticleCount() const;
    GA_Size	getVolumeCount() const;

    // This function will find the GEO_Standard_Attributes enum that
    // matches the provided string s.
    // true is returned on if a match is found; false if no match.
    bool getStandardAttributes(const char *s, GEO_Standard_Attributes &a);

    bool			 mergeCaptureProperties(
					GA_Attribute *my_atr,
					const GA_Attribute *src_atr,
					UT_IntArray &new_indices);

    /// Methods for dealing with attribute handles.  It is important to note
    /// that the const methods return READ-ONLY handles which will fail when
    /// any write methods are called.
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle	 getPointAttribute (const char *attrib_name) const;
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle	 getPointAttribute (const char *attrib_name);
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle	 getPrimAttribute  (const char *attrib_name) const;
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle	 getPrimAttribute  (const char *attrib_name);
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle	 getVertexAttribute(const char *attrib_name) const;
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle	 getVertexAttribute(const char *attrib_name);
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle	 getDetailAttribute(const char *attrib_name) const;
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle	 getDetailAttribute(const char *attrib_name);
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle	 getAttribute(GA_AttributeOwner dict,
				      const char *attrib_name) const;
    SYS_DEPRECATED_HDK(13.0)
    GEO_AttributeHandle	 getAttribute(GA_AttributeOwner dict,
				      const char *attrib_name);

    /// Change the storage type from one type to another.  For example, cast an
    /// integer attribute into a floating point attribute, or vice versa.
    /// Only some casting methods have been implemented, so it's quite likely
    /// that this method may fail.
    bool	changeAttributeStorage(GA_AttributeOwner dict,
					const UT_StringRef &name,
					GA_Storage new_storage);
    bool	changePointAttributeStorage(const UT_StringRef &nm, GA_Storage s);
    bool	changeVertexAttributeStorage(const UT_StringRef &nm, GA_Storage s);
    bool	changePrimAttributeStorage(const UT_StringRef &nm, GA_Storage s);
    bool	changeDetailAttributeStorage(const UT_StringRef &nm, GA_Storage s);

    /// @{
    /// Convenience functions to set a detail attribute to a specific
    /// value, and also create the attribute if it doesn't exist.
    /// Values will only be assigned to the first N tuple components.  For
    /// example, for strings, only the first string value will be set, while
    /// for a vector2, only the first 2 components will be set.
    /// @note Storing a ramp will call the save() method on the UT_Ramp and
    /// assign that string to the attribute.
    /// @note If the attribute already exists but is the wrong type, or the
    /// attribute has too small a tuple size, this method will fail.
    bool	setDetailAttributeS(const UT_StringHolder &aname, const char *value);
    bool	setDetailAttributeS(const UT_StringHolder &aname, const UT_StringArray &s);
    bool	setDetailAttributeR(const UT_StringHolder &aname, const UT_Ramp &ramp);
    bool	setDetailAttributeF(const UT_StringHolder &aname, float v);
    bool	setDetailAttributeF(const UT_StringHolder &aname, float v1, float v2);
    bool	setDetailAttributeF(const UT_StringHolder &aname, const UT_Vector2 &v);
    bool	setDetailAttributeI(const UT_StringHolder &aname, int v);
    /// @}

    /// @{
    /// Convenience routines to retrieve a group of points referenced
    /// by a range/group of primitives. These routines assume that the point
    /// group pointer is not NULL and return the size of the resulting point 
    /// group.
    GA_Size getPointGroupAdjacentToPrimitiveGroup(
	    const GA_PrimitiveGroup *in_prim_grp,
	    GA_PointGroup *adj_pt_grp);
    GA_Size getPointGroupAdjacentToPrimitiveRange(
	    const GA_Range &in_prim_rng,
	    GA_PointGroup *adj_pt_rng);
    GA_Size getVertexGroupAdjacentToPrimitiveGroup(
	    const GA_PrimitiveGroup *in_prim_grp,
	    GA_VertexGroup *adj_vtx_grp);
    GA_Size getVertexGroupAdjacentToPrimitiveRange(
	    const GA_Range &in_prim_rng,
	    GA_VertexGroup *adj_vtx_rng);
    /// @}

private:
    struct AdjacentPolygonData
    {
	GA_Offset myAdjacentPolygon;

        bool operator==(const AdjacentPolygonData &pad) const
	{ return myAdjacentPolygon == pad.myAdjacentPolygon; }
        bool operator<(const AdjacentPolygonData &pad) const
	{ return myAdjacentPolygon < pad.myAdjacentPolygon; }
    };

public:

    struct PointAdjacencyData : public AdjacentPolygonData
    {
	GA_Offset mySharedPoint;
    };

    struct EdgeAdjacencyData : public AdjacentPolygonData
    {
	GA_Offset myStartEdgePoint;
	GA_Offset myEndEdgePoint;
	bool myWindingReversed;
    };

    typedef UT_ValArray<PointAdjacencyData> GEO_PointAdjArray;
    typedef UT_ValArray<EdgeAdjacencyData> GEO_EdgeAdjArray;

    // Get an array of PointAdjacencyData structs representing the polygons
    // adjacent to a polygon (poly_off) through a point. Notice that
    // PointAdjacencyData contains a point shared by the two polygons
    GA_Size	getPointAdjacentPolygons(GEO_PointAdjArray &adjacencies,
					 GA_Offset poly_off) const;
    
    // Get an array of polygon Offsets adjacent to a polygon (poly_off)
    // through a point
    GA_Size	getPointAdjacentPolygons(GA_OffsetArray &polygons,
					 GA_Offset poly_off) const;


    // Get an array of EdgeAdjacencyData structs representing the polygons
    // adjacent to a polygon (poly_off) through an edge. Notice that
    // EdgeAdjacencyData contains additional information about the type of
    // adjacency
    GA_Size	getEdgeAdjacentPolygons(GEO_EdgeAdjArray &adjacencies,
    					GA_Offset poly_off) const;

    // Get an array of polygon Offsets adjacent to a polygon (poly_off)
    // through an edge
    GA_Size	getEdgeAdjacentPolygons(GA_OffsetArray &polygons,
    					GA_Offset poly_off) const;

    /// Searches primitive list for a primitive whose name attribute
    /// matches the given name.
    /// UT_String::multiMatch is used for comparison so searching
    /// for Cd.* will return as expected.
    /// Returns the matchnumberth matching primitive.
    const GEO_Primitive	*findPrimitiveByName(const char *nametomatch,
					    const GA_PrimCompat::TypeMask &mask
					       = GEO_PrimTypeCompat::GEOPRIMALL,
					    const char *nameattrib="name",
					    int matchnumber = 0) const;
    GEO_Primitive	*findPrimitiveByName(const char *nametomatch,
					    const GA_PrimCompat::TypeMask &mask
					       = GEO_PrimTypeCompat::GEOPRIMALL,
					    const char *nameattrib="name",
					    int matchnumber = 0);

    /// Like findPrimitiveByName, but adds all matching primitives to the list
    void		 findAllPrimitivesByName(
				UT_Array<const GEO_Primitive *> &primlist,
				const char *nametomatch,
				const GA_PrimCompat::TypeMask &mask
					       = GEO_PrimTypeCompat::GEOPRIMALL,
				const char *nameattrib="name") const;
    void		 findAllPrimitivesByName(
				UT_Array<GEO_Primitive*> &primlist,
				const char *nametomatch,
				const GA_PrimCompat::TypeMask &mask
					       = GEO_PrimTypeCompat::GEOPRIMALL,
				const char *nameattrib="name");

    /// Merges attributes from 'src' into this detail.  Capture attributes for
    /// newly merged points are also updated.  Newly merged points are
    /// identified by point numbers beginning from 'num_pts_prev'.
    void	 mergeDetailAttributes( const GEO_Detail &src,
					int   num_pts_prev );

    void		 destroyPointGroup(GA_PointGroup *g)
			    { destroyElementGroup(g); }
    void		 destroyPrimitiveGroup(GA_PrimitiveGroup *g)
			    { destroyElementGroup(g); }
    void		 destroyVertexGroup(GA_VertexGroup *g)
			    { destroyElementGroup(g); }
    bool		 destroyPointGroup(const char *name)
			    { return
				destroyElementGroup(GA_ATTRIB_POINT, name); }
    bool		 destroyPrimitiveGroup(const char *name)
			    { return
				destroyElementGroup(GA_ATTRIB_PRIMITIVE, name); }
    bool		 destroyVertexGroup(const char *name)
			    { return
				destroyElementGroup(GA_ATTRIB_VERTEX, name); }

    /// Attempt to copy groups from the source detail.  If override is true,
    /// existing groups with the same names as src groups are cleared, else
    /// existing groups are not affected.  The status (ordered/unordered,
    /// internal/persistent) of the source group is also copied.
    ///
    /// Membership is matched by element index for point and primitive groups,
    /// by primitive and local vertex index for vertex groups, and by element
    /// index validated by adjacency for edge groups.
    void		 mergeGroups(const GEO_Detail &src, bool override,
				     unsigned mask = GA_GMASK_FULL_MASK);
    GA_PointGroup	*mergeGroup(const GEO_Detail &src,
				    const GA_PointGroup &group,
				    bool override = true);
    GA_PrimitiveGroup	*mergeGroup(const GEO_Detail &src,
				    const GA_PrimitiveGroup &group,
				    bool override = true);
    GA_VertexGroup	*mergeGroup(const GEO_Detail &src,
				    const GA_VertexGroup &group,
				    GA_OffsetArray &map_cache,
				    bool override = true);
    GA_EdgeGroup	*mergeGroup(const GEO_Detail &src,
				    const GA_EdgeGroup &group,
				    bool override = true);

protected:
    static GA_IntrinsicManager::Registrar
		registerIntrinsics(GA_PrimitiveFactory &factory);

    virtual GA_Size	localIntrinsicTupleSize(GA_IntrinsicEval &eval) const;
    virtual GA_Size	localGetIntrinsicS(GA_IntrinsicEval &eval,
				UT_String &value) const;
    virtual GA_Size	localGetIntrinsicSA(GA_IntrinsicEval &eval,
				UT_StringArray &value) const;
    virtual GA_Size	localGetIntrinsicI(GA_IntrinsicEval &eval,
				int64 *value, GA_Size size) const;
    virtual GA_Size	localGetIntrinsicF(GA_IntrinsicEval &eval,
				fpreal64 *value, GA_Size size) const;
    virtual GA_Size	localSetIntrinsicS(GA_IntrinsicEval &eval,
				const char *value);
    virtual GA_Size	localSetIntrinsicSA(GA_IntrinsicEval &eval,
				const UT_StringArray &value);
    virtual GA_Size	localSetIntrinsicSS(GA_IntrinsicEval &eval,
				const char **value, GA_Size size);
    virtual GA_Size	localSetIntrinsicI(GA_IntrinsicEval &eval,
				const int64 *value, GA_Size size);
    virtual GA_Size	localSetIntrinsicF(GA_IntrinsicEval &eval,
				const fpreal64 *value, GA_Size size);

    GA_Detail::IOStatus	saveH9Stream(std::ostream& os, bool binary,
					const GA_SaveOptions *options) const;
    GA_Detail::IOStatus	loadH9(UT_IStream &is, const GA_LoadOptions *options);

    /// Simple stat of an H9 file.  This only fills out the point/prim counts
    /// This assumes the magic number (32-bits) has already been read from the
    /// file.
    static GA_Detail::IOStatus	statH9Basic(UT_IStream &is, GA_Stat &sbuf,
					bool binstream);

    bool	 convertIndexPairDataFromH9();
    bool	 convertIndexPairDataToH9();

    template<typename FLOAT_T>
    void	 transformInternal(const UT_Matrix4T<FLOAT_T> &mat,
			   const GA_Range &prim_range,
			   const GA_Range &pt_range,
			   bool just_P,
			   bool keep_vector_lengths,
			   bool check_pasting,
			   bool neg_determinant_flipvector,
			   bool update_ptnormals,
			   GEO_Delta *geodelta,
                           bool updateaffectednormals,
                           const char *attribpattern);

private:
    GA_ElementGroup	*mergeElementGroup(const GEO_Detail &src,
					   const GA_ElementGroup &group,
					   bool override = true);

public:
    /// Some common methods for the standard guys
    GA_Attribute        *addAttribute(const UT_StringHolder &name,
				const UT_Options *creation_args,
				const GA_AttributeOptions *attribute_options,
				const UT_StringRef &attribtype,
				GA_AttributeOwner who,
				GA_AttributeScope scope= GA_SCOPE_PUBLIC);
    GA_Attribute        *growTuple(GA_AttributeOwner owner,
				   GA_AttributeScope scope,
                                   const UT_StringRef &name,
				   int added_size);
    SYS_FORCE_INLINE
    GA_Attribute        *growTuple(GA_AttributeOwner owner,
                                   const UT_StringRef &name,
				   int added_size)
		{
		    return growTuple(owner, GA_SCOPE_PUBLIC, name, added_size);
		}
    void		 destroyAttribute(GA_AttributeOwner owner,
					  GA_AttributeScope scope,
					  const UT_StringRef &name,
					  const GA_AttributeFilter *filter = 0);
    SYS_FORCE_INLINE
    void		 destroyAttribute(GA_AttributeOwner owner,
					  const UT_StringRef &name,
					  const GA_AttributeFilter *filter = 0)
			{
			    destroyAttribute(owner, GA_SCOPE_PUBLIC, name,
					     filter);
			}

    /// Convenience method to find floating point, int, or string tuples of a
    /// specified size range. If max_size or min_size are <= 0, then we won't
    /// perform the corresponding size check.
    /// @{
    const GA_Attribute  *findFloatTuple(GA_AttributeOwner owner,
					GA_AttributeScope scope,
					const UT_StringRef &name,
					int min_size=1, int max_size=-1) const;
    SYS_FORCE_INLINE
    const GA_Attribute  *findFloatTuple(GA_AttributeOwner owner,
					const UT_StringRef &name,
					int min_size=1, int max_size=-1) const
			{
			    return findFloatTuple(owner, GA_SCOPE_PUBLIC, name,
						  min_size, max_size);
			}
    GA_Attribute        *findFloatTuple(GA_AttributeOwner owner,
					GA_AttributeScope scope,
					const UT_StringRef &name,
					int min_size=1, int max_size=-1);
    SYS_FORCE_INLINE
    GA_Attribute        *findFloatTuple(GA_AttributeOwner owner,
					const UT_StringRef &name,
					int min_size=1, int max_size=-1)
			{
			    return findFloatTuple(owner, GA_SCOPE_PUBLIC, name,
						  min_size, max_size);
			}
    const GA_Attribute  *findIntTuple(GA_AttributeOwner owner,
                                      GA_AttributeScope scope,
                                      const UT_StringRef &name,
                                      int min_size=1, int max_size=-1) const;
    SYS_FORCE_INLINE
    const GA_Attribute  *findIntTuple(GA_AttributeOwner owner,
                                      const UT_StringRef &name,
                                      int min_size=1, int max_size=-1) const
			{
			    return findIntTuple(owner, GA_SCOPE_PUBLIC, name,
						min_size, max_size);
			}
    GA_Attribute        *findIntTuple(GA_AttributeOwner owner,
                                      GA_AttributeScope scope,
                                      const UT_StringRef &name,
                                      int min_size=1, int max_size=-1);
    SYS_FORCE_INLINE
    GA_Attribute        *findIntTuple(GA_AttributeOwner owner,
                                      const UT_StringRef &name,
                                      int min_size=1, int max_size=-1)
			{
			    return findIntTuple(owner, GA_SCOPE_PUBLIC, name,
						min_size, max_size);
			}
    /// findNumericTuple will find either float or int.
    const GA_Attribute  *findNumericTuple(GA_AttributeOwner owner,
					GA_AttributeScope scope,
					const UT_StringRef &name,
					int min_size=1, int max_size=-1) const;
    SYS_FORCE_INLINE
    const GA_Attribute  *findNumericTuple(GA_AttributeOwner owner,
					const UT_StringRef &name,
					int min_size=1, int max_size=-1) const
			{
			    return findNumericTuple(owner, GA_SCOPE_PUBLIC,
						    name, min_size, max_size);
			}
    /// findNumericTuple will find either float or int.
    GA_Attribute        *findNumericTuple(GA_AttributeOwner owner,
					GA_AttributeScope scope,
					const UT_StringRef &name,
					int min_size=1, int max_size=-1);
    SYS_FORCE_INLINE
    GA_Attribute        *findNumericTuple(GA_AttributeOwner owner,
					const UT_StringRef &name,
					int min_size=1, int max_size=-1)
			{
			    return findNumericTuple(owner, GA_SCOPE_PUBLIC,
						    name, min_size, max_size);
			}
    const GA_Attribute  *findStringTuple(GA_AttributeOwner owner,
					 GA_AttributeScope scope,
					 const UT_StringRef &name,
					 int min_size=1, int max_size=-1) const;
    SYS_FORCE_INLINE
    const GA_Attribute  *findStringTuple(GA_AttributeOwner owner,
					 const UT_StringRef &name,
					 int min_size=1, int max_size=-1) const
			{
			    return findStringTuple(owner, GA_SCOPE_PUBLIC,
						   name, min_size, max_size);
			}
    GA_Attribute        *findStringTuple(GA_AttributeOwner owner,
					 GA_AttributeScope scope,
					 const UT_StringRef &name,
					 int min_size=1, int max_size=-1);
    SYS_FORCE_INLINE
    GA_Attribute        *findStringTuple(GA_AttributeOwner owner,
					 const UT_StringRef &name,
					 int min_size=1, int max_size=-1)
			{
			    return findStringTuple(owner, GA_SCOPE_PUBLIC,
						   name, min_size, max_size);
			}
    const GA_Attribute  *findIndexPair(GA_AttributeOwner owner,
				       GA_AttributeScope scope,
				       const UT_StringRef &name) const;
    SYS_FORCE_INLINE
    const GA_Attribute  *findIndexPair(GA_AttributeOwner owner,
				       const UT_StringRef &name) const
			{
			    return findIndexPair(owner, GA_SCOPE_PUBLIC, name);
			}
    GA_Attribute        *findIndexPair(GA_AttributeOwner owner,
				       GA_AttributeScope scope,
				       const UT_StringRef &name);
    SYS_FORCE_INLINE
    GA_Attribute        *findIndexPair(GA_AttributeOwner owner,
				       const UT_StringRef &name)
			{
			    return findIndexPair(owner, GA_SCOPE_PUBLIC, name);
			}
    const GA_Attribute  *findFloatArray(GA_AttributeOwner owner,
					GA_AttributeScope scope,
					const UT_StringRef &name,
					int min_size=1, int max_size=-1) const;
    SYS_FORCE_INLINE
    const GA_Attribute  *findFloatArray(GA_AttributeOwner owner,
					const UT_StringRef &name,
					int min_size=1, int max_size=-1) const
			{
			    return findFloatArray(owner, GA_SCOPE_PUBLIC, name,
						  min_size, max_size);
			}
    GA_Attribute        *findFloatArray(GA_AttributeOwner owner,
					GA_AttributeScope scope,
					const UT_StringRef &name,
					int min_size=1, int max_size=-1);
    SYS_FORCE_INLINE
    GA_Attribute        *findFloatArray(GA_AttributeOwner owner,
					const UT_StringRef &name,
					int min_size=1, int max_size=-1)
			{
			    return findFloatArray(owner, GA_SCOPE_PUBLIC, name,
						  min_size, max_size);
			}
    const GA_Attribute  *findIntArray(GA_AttributeOwner owner,
                                      GA_AttributeScope scope,
                                      const UT_StringRef &name,
                                      int min_size=1, int max_size=-1) const;
    SYS_FORCE_INLINE
    const GA_Attribute  *findIntArray(GA_AttributeOwner owner,
                                      const UT_StringRef &name,
                                      int min_size=1, int max_size=-1) const
			{
			    return findIntArray(owner, GA_SCOPE_PUBLIC, name,
						min_size, max_size);
			}
    GA_Attribute        *findIntArray(GA_AttributeOwner owner,
                                      GA_AttributeScope scope,
                                      const UT_StringRef &name,
                                      int min_size=1, int max_size=-1);
    SYS_FORCE_INLINE
    GA_Attribute        *findIntArray(GA_AttributeOwner owner,
                                      const UT_StringRef &name,
                                      int min_size=1, int max_size=-1)
			{
			    return findIntArray(owner, GA_SCOPE_PUBLIC, name,
						min_size, max_size);
			}
    const GA_Attribute  *findNumericArray(GA_AttributeOwner owner,
                                      GA_AttributeScope scope,
                                      const UT_StringRef &name,
                                      int min_size=1, int max_size=-1) const;
    SYS_FORCE_INLINE
    const GA_Attribute  *findNumericArray(GA_AttributeOwner owner,
                                      const UT_StringRef &name,
                                      int min_size=1, int max_size=-1) const
			{
			    return findNumericArray(owner, GA_SCOPE_PUBLIC, name,
						min_size, max_size);
			}
    GA_Attribute        *findNumericArray(GA_AttributeOwner owner,
                                      GA_AttributeScope scope,
                                      const UT_StringRef &name,
                                      int min_size=1, int max_size=-1);
    SYS_FORCE_INLINE
    GA_Attribute        *findNumericArray(GA_AttributeOwner owner,
                                      const UT_StringRef &name,
                                      int min_size=1, int max_size=-1)
			{
			    return findNumericArray(owner, GA_SCOPE_PUBLIC, name,
						min_size, max_size);
			}
    const GA_Attribute  *findStringArray(GA_AttributeOwner owner,
					GA_AttributeScope scope,
					const UT_StringRef &name,
					int min_size=1, int max_size=-1) const;
    SYS_FORCE_INLINE
    const GA_Attribute  *findStringArray(GA_AttributeOwner owner,
					const UT_StringRef &name,
					int min_size=1, int max_size=-1) const
			{
			    return findStringArray(owner, GA_SCOPE_PUBLIC, name,
						   min_size, max_size);
			}
    GA_Attribute        *findStringArray(GA_AttributeOwner owner,
					GA_AttributeScope scope,
					const UT_StringRef &name,
					int min_size=1, int max_size=-1);
    SYS_FORCE_INLINE
    GA_Attribute        *findStringArray(GA_AttributeOwner owner,
					const UT_StringRef &name,
					int min_size=1, int max_size=-1)
			{
			    return findStringArray(owner, GA_SCOPE_PUBLIC, name,
						   min_size, max_size);
			}
    /// @}

    /// @{
    /// Convenience method for adding a float, integer or string tuple
    /// attribute.
    ///
    /// If there's an existing attribute of the same name, the 
    /// GA_ReuseStrategy will be used.  By default, this will return
    /// the existing attribute if it is the same type.  If not, it will
    /// try to replace it with a new one and copy the old data over.
    ///
    /// The attribute storage will be determined by the @c storage parameter.
    /// For example, if it's GA_STORE_REAL32 or GA_STORE_REAL16, a floating
    /// point tuple will be added.  An integer tuple would be created for
    /// GA_STORE_INT64 and a string tuple for GA_STORE_STRING.
    ///
    /// The defaults will be ignored for string attributes.
    /// @note If you have a GA_StorageClass, you can call
    ///    @c GAgetDefaultStorage(storageclass);
    GA_Attribute        *addTuple(GA_Storage storage,
				GA_AttributeOwner owner,
				GA_AttributeScope scope,
				const UT_StringHolder &name,
				int tuple_size,
				const GA_Defaults &defaults = GA_Defaults(0.0),
				const UT_Options *creation_args = 0,
				const GA_AttributeOptions *attribute_options = 0,
				const GA_ReuseStrategy &reuse =
						GA_ReuseStrategy());
    SYS_FORCE_INLINE
    GA_Attribute        *addTuple(GA_Storage storage,
				GA_AttributeOwner owner,
				const UT_StringHolder &name,
				int tuple_size,
				const GA_Defaults &defaults = GA_Defaults(0.0),
				const UT_Options *creation_args = 0,
				const GA_AttributeOptions *attribute_options = 0,
				const GA_ReuseStrategy &reuse =
						GA_ReuseStrategy())
			{
			    return addTuple(storage,
				    owner,
				    GA_SCOPE_PUBLIC,
				    name,
				    tuple_size,
				    defaults,
				    creation_args,
				    attribute_options,
				    reuse);
			}
    /// @}

    /// @{
    /// Convenience method to add a new floating point, int or string tuple
    /// attribute.
    ///
    /// If there's an existing attribute of the same name, the 
    /// GA_ReuseStrategy will be used.  By default, this will return
    /// the existing attribute if it is the same type.  If not, it will
    /// try to replace it with a new one and copy the old data over.
    GA_Attribute        *addFloatTuple(GA_AttributeOwner owner,
				    GA_AttributeScope scope,
				    const UT_StringHolder &name,
				    int tuple_size,
				    const GA_Defaults &defaults = GA_Defaults(0.0),
				    const UT_Options *creation_args=0,
				    const GA_AttributeOptions *attribute_options=0,
				    GA_Storage storage = GA_STORE_REAL32,
				    const GA_ReuseStrategy &reuse
						= GA_ReuseStrategy());
    SYS_FORCE_INLINE
    GA_Attribute        *addFloatTuple(GA_AttributeOwner owner,
				    const UT_StringHolder &name,
				    int tuple_size,
				    const GA_Defaults &defaults = GA_Defaults(0.0),
				    const UT_Options *creation_args=0,
				    const GA_AttributeOptions *attribute_options=0,
				    GA_Storage storage = GA_STORE_REAL32,
				    const GA_ReuseStrategy &reuse
						= GA_ReuseStrategy())
			 {
			     return addFloatTuple(owner, GA_SCOPE_PUBLIC, name,
						tuple_size, defaults,
						creation_args,
						attribute_options,
						storage, reuse);
			 }
    GA_Attribute        *addIntTuple(GA_AttributeOwner owner,
				    GA_AttributeScope scope,
				    const UT_StringHolder &name,
				    int tuple_size,
				    const GA_Defaults &defaults = GA_Defaults(0),
				    const UT_Options *creation_args=0,
				    const GA_AttributeOptions *attribute_options=0,
				    GA_Storage storage = GA_STORE_INT32,
				    const GA_ReuseStrategy &reuse
						= GA_ReuseStrategy());
    SYS_FORCE_INLINE
    GA_Attribute        *addIntTuple(GA_AttributeOwner owner,
				    const UT_StringHolder &name,
				    int tuple_size,
				    const GA_Defaults &defaults = GA_Defaults(0),
				    const UT_Options *creation_args=0,
				    const GA_AttributeOptions *attribute_options=0,
				    GA_Storage storage = GA_STORE_INT32,
				    const GA_ReuseStrategy &reuse
						= GA_ReuseStrategy())
			 {
			     return addIntTuple(owner, GA_SCOPE_PUBLIC, name,
						tuple_size, defaults,
						creation_args,
						attribute_options,
						storage, reuse);
			 }

    GA_Attribute        *addStringTuple(GA_AttributeOwner owner,
				    GA_AttributeScope scope,
				    const UT_StringHolder &name,
				    int tuple_size,
				    const UT_Options *creation_args=0,
				    const GA_AttributeOptions *attribute_options=0,
				    const GA_ReuseStrategy &reuse
						= GA_ReuseStrategy());
    SYS_FORCE_INLINE
    GA_Attribute        *addStringTuple(GA_AttributeOwner owner,
				    const UT_StringHolder &name,
				    int tuple_size,
				    const UT_Options *creation_args=0,
				    const GA_AttributeOptions *attribute_options=0,
				    const GA_ReuseStrategy &reuse
						= GA_ReuseStrategy())
			 {
			     return addStringTuple(owner, GA_SCOPE_PUBLIC,
					name, tuple_size, creation_args,
					attribute_options, reuse);
			 }
    /// @}

    /// @{
    /// Convenience method to add a new floating point, or int array
    /// attribute.
    ///
    /// If there's an existing attribute of the same name, the 
    /// GA_ReuseStrategy will be used.  By default, this will return
    /// the existing attribute if it is the same type.  If not, it will
    /// try to replace it with a new one and copy the old data over.
    GA_Attribute        *addFloatArray(GA_AttributeOwner owner,
				    GA_AttributeScope scope,
				    const UT_StringHolder &name,
				    int tuple_size = 1,
				    const UT_Options *creation_args=0,
				    const GA_AttributeOptions *attribute_options=0,
				    GA_Storage storage = GA_STORE_REAL32,
				    const GA_ReuseStrategy &reuse
						= GA_ReuseStrategy());
    SYS_FORCE_INLINE
    GA_Attribute        *addFloatArray(GA_AttributeOwner owner,
				    const UT_StringHolder &name,
				    int tuple_size = 1,
				    const UT_Options *creation_args=0,
				    const GA_AttributeOptions *attribute_options=0,
				    GA_Storage storage = GA_STORE_REAL32,
				    const GA_ReuseStrategy &reuse
						= GA_ReuseStrategy())
			 {
			     return addFloatArray(owner, GA_SCOPE_PUBLIC, name,
						tuple_size, 
						creation_args,
						attribute_options,
						storage, reuse);
			 }
    GA_Attribute        *addIntArray(GA_AttributeOwner owner,
				    GA_AttributeScope scope,
				    const UT_StringHolder &name,
				    int tuple_size = 1,
				    const UT_Options *creation_args=0,
				    const GA_AttributeOptions *attribute_options=0,
				    GA_Storage storage = GA_STORE_INT32,
				    const GA_ReuseStrategy &reuse
						= GA_ReuseStrategy());
    SYS_FORCE_INLINE
    GA_Attribute        *addIntArray(GA_AttributeOwner owner,
				    const UT_StringHolder &name,
				    int tuple_size = 1,
				    const UT_Options *creation_args=0,
				    const GA_AttributeOptions *attribute_options=0,
				    GA_Storage storage = GA_STORE_INT32,
				    const GA_ReuseStrategy &reuse
						= GA_ReuseStrategy())
			 {
			     return addIntArray(owner, GA_SCOPE_PUBLIC, name,
						tuple_size,
						creation_args,
						attribute_options,
						storage, reuse);
			 }
    GA_Attribute        *addStringArray(GA_AttributeOwner owner,
				    GA_AttributeScope scope,
				    const UT_StringHolder &name, int tuple_size=1,
				    const UT_Options *creation_args=0,
				    const GA_AttributeOptions *attribute_options=0,
				    GA_Storage storage = GA_STORE_STRING,
				    const GA_ReuseStrategy &reuse
						= GA_ReuseStrategy());
    SYS_FORCE_INLINE
    GA_Attribute        *addStringArray(GA_AttributeOwner owner,
				    const UT_StringHolder &name, int tuple_size=1,
				    const UT_Options *creation_args=0,
				    const GA_AttributeOptions *attribute_options=0,
				    GA_Storage storage = GA_STORE_STRING,
				    const GA_ReuseStrategy &reuse
						= GA_ReuseStrategy())
			 {
			     return addStringArray(owner, GA_SCOPE_PUBLIC, name,
						tuple_size,
						creation_args,
						attribute_options,
						storage, reuse);
			 }
    /// @}

    /// Magic number for ASCII geometry files
    static inline int	 asciiMagicH9()
			 {
			     return (('P'<<24) + ('G'<<16) + ('E'<<8) + 'O');
			 }
    /// Magic number for binary geometry files
    static inline int	 binaryMagicH9()
			 {
                             return (('B'<<24) + ('g'<<16) + ('e'<<8) + 'o');
			 }

private:
    bool		 loadPointsH9(UT_IStream &is, int count,
				      const UT_Array<GA_AttribLoadDataH9> &);
    bool		 savePointsH9(std::ostream &os, bool binary,
				const UT_Array<GA_AttribSaveDataH9> &) const;
    bool		 savePointH9(std::ostream &os, bool binary, GA_Offset offset,
				const UT_Array<GA_AttribSaveDataH9> &) const;
    bool		 saveExtraH9(std::ostream &os, bool binary,
				const UT_Array<const GA_Primitive*> &) const;
    bool		 loadExtraH9(UT_IStream &is);
    bool		 loadVersionH9(UT_IStream &is, int binary);

//
//  Friends
    friend std::ostream	&operator<<(std::ostream &os, const GEO_Detail &d)
			{
			    d.saveClassic(os, 0, 0);
			    return os;
			}

    friend class	GEO_IOTable;	// For loadH9/saveH9
    friend class	GU_IOClassic;	// For loadH9/saveH9

protected:

    /// This is here just so that GA_AttributeSet::jsonSave() can call
    /// GEOcomputeNormals()
    virtual void privateComputeNormal(
        const GA_RWHandleV3 &normalattrib,
        const GA_Group *group,
        const float cuspangledegrees,
        const int method) const;

// The compiler reports some deprecated overrides here
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif
