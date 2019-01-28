/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RAY_ProcGT.h ( RAY Library, C++)
 *
 * COMMENTS:	Procedural to render a GT primitive
 */

#ifndef __RAY_ProcGT__
#define __RAY_ProcGT__

#include "RAY_Procedural.h"
#include "RAY_StylerInfo.h"
#include <GT/GT_Handles.h>
#include <STY/STY_Styler.h>
#include <GA/GA_Types.h>
#include <UT/UT_Array.h>
#include <UT/UT_ArrayMap.h>
#include <UT/UT_Lock.h>

class GT_RefineParms;
class GU_Primitive;

/// @brief Procedural to render a single GT primitive
///
/// When rendering GT primitives, they will be refined until they hit geometry
/// that Mantra can handle.
///
/// When GT_PRIM_INSTANCE primitives are encountered, the procedural will
/// create multiple instance objects in mantra, allowing the geometry to be
/// shared between instances automatically.  In addition, the following
/// uniform/detail attributes are handled on the instance primitives:
///  - shop_materialpath @n
///    Assign a material for the instance.  This calls
///    RAY_Procedural::setShopMaterialPath().  If the @c shop_materialpath is
///    found, then additional attributes (@c material_override and @c
///    property_override) are evaluated to handle shader argument and property
///    overrides.
/// - lightcategories @n
///    Set the @b lightcategories property on the instance
/// - lightmask @n
///    Set the @b lightmask property on the instance.  Note that this may not
///    handle light instancing properly.
/// - categories @n
///    Set the categories for instance.
class RAY_API RAY_ProcGT : public RAY_Procedural
{
public:
    class StylingContext
    {
    public:
	using StylerInfoArray = UT_Array<RAY_StylerInfo>;
	using SharedPrimGroup = UT_SharedPtr<GSTY_SubjectPrimGroup>;
	using GroupSharingMap = UT::ArrayMap<exint, SharedPrimGroup>;

	const StylerInfoArray	&infos() const { return myInstanceStylerInfos; }
	void	appendStyle(const RAY_StylerInfo &styler)
		    { myInstanceStylerInfos.append(styler); }

	void	setInstanceStylers(StylingContext &child,
			const GT_PrimitiveHandle &prim);
	void	clearInfos()	{ myInstanceStylerInfos.clear(); }
    private:
	StylerInfoArray myInstanceStylerInfos;
	SharedPrimGroup myGroupSharingHolder;
	GroupSharingMap myInstanceGroupSharingMap;
	UT_Lock		myInstanceGroupLock;
    };
    using StylerInfoArray = StylingContext::StylerInfoArray;
    using SharedPrimGroup = StylingContext::SharedPrimGroup;
    using GroupSharingMap = StylingContext::GroupSharingMap;
    static const GT_RefineParms	*mantraRefineParms(const RAY_Procedural &proc);

    /// A mantra procedural to render a GT primitive.  If the GT primitive has
    /// multiple segments, the primitive will be rendered with deformation
    /// motion blur unless @c use_multi_segments is false.
    ///
    /// When multi-segment blur is active, the @c shutter_close can be
    /// specified.  If the @c shutter_close is less than 0, the value will be
    /// imported from the camera:shutter setting.  If @c shutter_close is 0,
    /// multi-segment blur will be disabled.
    ///
    /// If the GT primitive has the proper sub-frame sampling time, then you
    /// likely want to set the @c shutter_close to 1.0.
    RAY_ProcGT(const GT_PrimitiveHandle &prim,
		    bool use_multi_segments=true,
		    fpreal shutter_close=-1);
    /// Alternatively, motion segments can be specified using an array of
    /// primitives.
    RAY_ProcGT(const UT_Array<GT_PrimitiveHandle> &primitives,
		    fpreal shutter_close=-1);
    virtual ~RAY_ProcGT();

    /// @{
    /// Methods from RAY_Procedural
    virtual const char	*className() const;
    virtual int		 initialize(const UT_BoundingBox *box);
    virtual void	 getBoundingBox(UT_BoundingBox &box);
    virtual void	 render();
    virtual bool	 canGenerateInstancedGeometry() const;
    /// @}

    /// @{
    /// @private
    /// Provide public accessor to create new procedural children.
    RAY_ProceduralChildPtr	newChild() const { return createChild(); }
    /// @}

    void	dump(bool dump_full_geo=false);
    void	instanceVelocityBlur(const GT_DataArrayHandle &v,
				     const GT_DataArrayHandle &w,
				     const GT_DataArrayHandle &a,
				     const GT_DataArrayHandle &pivot);
    int		velocityBlur() const	{ return myVelocityBlur; }

    /// @{
    /// @private
    /// Create a new styler for each instance in a child ProcGT based on our
    /// existing instance stylers and any new instance defined in the prim.
    StylingContext	&stylingContext() { return myStylingContext; }
    void	setInstanceTransform(const RAY_ProceduralChildPtr &kid,
			const GT_TransformHandle &xform) const;
    void	processInstanceAttributes(const RAY_ProceduralChildPtr &kid,
			const GT_AttributeListHandle &attrib,
			exint idx) const;
    void	velocityBlurTransform(const RAY_ProceduralChildPtr &kid,
			const GT_TransformArrayHandle &xforms,
			const GT_DataArrayHandle &v,
			const GT_DataArrayHandle &w,
			const GT_DataArrayHandle &accel,
			const GT_DataArrayHandle &pivot,
			const GT_TransformHandle &packed,
			exint xform) const;
    void	renderPrim(const GT_PrimitiveHandle &prim,
			StylingContext &styling) const;
    /// @}

    // Top level primitives will handle pre-blur on velocity different, since
    // they assume the velocity blur has been applied to the top level
    // geometry.
    void	setTopLevel(bool v)	{ myTopLevel = v; }

private:
    void	computeBounds(UT_BoundingBox &box,
			const GT_PrimitiveHandle &prim) const;
    static void	computeInstanceBounds(UT_BoundingBox &box,
			const GT_PrimitiveHandle &prim,
			const GT_DataArrayHandle &v,
			const GT_DataArrayHandle &w,
			const GT_DataArrayHandle &a,
			const GT_DataArrayHandle &pivot,
			int velocityBlur,
			fpreal preblur,
			fpreal postblur);
    void	renderSimpleInstances(const UT_StringHolder &basename,
			const GT_PrimitiveHandle &prim,
			const GT_TransformArrayHandle &xforms,
			const GT_AttributeListHandle &uniform,
			const GT_AttributeListHandle &detail,
			const StylingContext &styling,
			bool pack_instance) const;
    void	initShutter()
		{
		    if (!myShutterInitialized)
		    {
			myShutterInitialized = true;
			importShutter();
		    }
		}
    void	importShutter();
    int		getMotionSegments() const
		{
		    return mySegments;
		}


    GT_PrimitiveHandle			myPrim;
    UT_BoundingBox			myBox;
    GT_DataArrayHandle			myParentVelocity;
    GT_DataArrayHandle			myParentAngularVelocity;
    GT_DataArrayHandle			myParentAcceleration;
    GT_DataArrayHandle			myParentPivot;
    StylingContext			myStylingContext;
    fpreal				myPreBlur, myPostBlur;	// Velocity blur
    fpreal				myShutterClose;		// Shutter close
    int					mySegments;
    int					myVelocityBlur;
    bool				myShutterInitialized;
    bool				myTopLevel;
};

#endif
