/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RAY_Procedural.h ( RAY Library, C++)
 *
 * COMMENTS:	RAY Procedural Primitive
 *
 *	This primitive is used to generate procedural geometry during the
 *	rendering process.
 *
 *	The procedural primitive can generate further procedural primitives, or
 *	as a leaf node, generate a GU_Detail.
 *
 *	Shaders for the geometry are inherited from the instance of the detail
 *	unless additional shaders are specified using the vm_surface or
 *	vm_displace attributes.
 */

#ifndef __RAY_Procedural__
#define __RAY_Procedural__

#include "RAY_API.h"
#include <SYS/SYS_Types.h>
#include <GA/GA_Handle.h>
#include <GU/GU_DetailHandle.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_IntrusivePtr.h>

class UT_Options;
class GU_Detail;
class GEO_Detail;
class GEO_Primitive;
class GA_Attribute;
class ray_ProcInstance;
class RAY_Procedural;
class RAY_ProceduralArg;
class VGEO_Volume;
class STY_Styler;
class RAY_StylerInfo;
class RAY_ProceduralData;
class RAY_PackedQuery;
class RAY_ProceduralFactory;

/// @brief Mantra procedural primitives
///
/// When a procedural is defined as a dynamic object, the allocProcedural()
/// function is called to build a new instance of the procedural.  The name
/// passed in will be the name defined by the table entry.  This can be used
/// for:
///	a)  Error checking -- you can verify that you're name is what you
///		expect.  However, this isn't mandatory.
///	b)  To have more than one procedural defined per C++ class.  Using
///		the name, you can create procedurals of different types.
///
/// The argument list for the procedural is specified by a list of arguments.
/// The getProceduralArgs() method should return a pointer to an array of
/// RAY_ProceduralArg's.  The list should be terminated by an entry with
/// default arguments (i.e. null pointers).  For example:
/// Arguments can then be queried using the argValue() methods in the procedural
///
extern "C" {
    /// Dynamic load entry point to create an instance
    SYS_VISIBILITY_EXPORT extern RAY_Procedural *allocProcedural(const char *name);
    /// Dynamic load entry point to query arguments for a procedural
    SYS_VISIBILITY_EXPORT extern const RAY_ProceduralArg *getProceduralArgs(const char *n);

    /// Modern interface to register procedurals
    SYS_VISIBILITY_EXPORT extern void registerProcedural(
					    RAY_ProceduralFactory *factory);
}

/// @brief Aliases for different handle types

/// A generic handle that can be used by RAY_Procedural::import and
/// RAY_Procedural::importAll.
typedef void *RAY_Handle;

/// A handle for objects. Can also be used anywhere a RAY_Handle is expected.
typedef void *RAY_ObjectHandle;

/// A handle for materials. Can also be used anywhere a RAY_Handle is expected.
typedef void *RAY_MaterialHandle;

/// A handle for lights. Can also be used anywhere a RAY_Handle is expected.
typedef void *RAY_LightHandle;

/// @brief Parameter definition for arguments to RAY_Procedural
/// 
/// Each RAY_Procedural has its arguments defined by a table of
/// RAY_ProceduralArg objects.  The procedural can query the value of these
/// parameters using RAY_Procedural::import() at run time (without having to
/// parse argument lists).  The list of arguments should be terminated by an
/// entry constructed with default arguments. For example:
/// @code
///	static RAY_ProceduralArg	theArgs[] = {
///	   RAY_ProceduralArg("intarg",		"int",		"0" ),
///	   RAY_ProceduralArg("realarg",	"real",		"3.1415" ),
///	   RAY_ProceduralArg("stringarg",	"string",	"foo bar" ),
///	   RAY_ProceduralArg("vectorarg",	"real",		"1 2 3" ),
///	   RAY_ProceduralArg()
///	}
///	const RAY_ProceduralArg *getProceduralArgs() { return theArgs; }
/// @endcode
class RAY_API RAY_ProceduralArg
{
public:
    /// @param name @n The name of the parameter (must be unique)
    /// @param type @n
    ///	The storage type of the parameter.  This should be one of
    ///    - @c int = Integer
    ///    - @c real = Floating point (fpreal) value
    ///    - @c string = String value
    /// @param value @n A @b string representing the default values for the
    ///    argument.  For @c int and @c real types, the string is tokenized and
    ///    the number of tokens in the trin determines the vector/tuple size of
    ///    the argument.  For example @code
    ///      RAY_ProceduralArg("a", "int", "1 2 3 4")
    ///    @endcode
    ///    would specify a parameter named "a" which consists of 4 integers,
    ///    and has a default value of {1,2,3,4}. @n
    ///    For string types, the vector size is always 1, and the string is
    ///    used as the default value.
    ///
    /// @warning The procedural keeps shallow references to the
    ///   <tt>const char *</tt> passed in.
    RAY_ProceduralArg(const char *name=0, const char *type=0,
			const char *value=0)
    {
	 myName = name;
	 myType = type;
	 myValue = value;
    }
    ~RAY_ProceduralArg()
    {
    }

    /// Return the name of the argument
    const char	*getName() const	{ return myName; }
    /// Return the storage type of the argument
    const char	*getType() const	{ return myType; }
    /// Return the default value
    const char	*getValue() const	{ return myValue; }

private:
    const char	*myName;
    const char	*myType;
    const char	*myValue;
};

class RAY_ProceduralGeo;

/// @brief Reference counted geometry handle for procedurals
///
/// Mantra has internal reference counted "smart" pointers for geometry.
/// The interface for managing these reference counts manually
/// (referenceGeometry()/freeGeometry() was obfuscated and is now deprecated
/// in favour of using the VGEO_ProceduralGeo class).
///
/// The @c RAY_ROProceduralGeo provides a read-only handle to geometry.  This
/// is typically used to access geometry that exists in mantra already (i.e.
/// geometry returned by <tt>queryGeometry()</tt>).
///
/// @c RAY_ProceduralGeo is a read-write handle on mantra geometry.
///
/// Each geometry object can have multiple deformation segments.
class RAY_API RAY_ROProceduralGeo
{
public:
    RAY_ROProceduralGeo() { }

    /// Copy c-tor
    RAY_ROProceduralGeo(const RAY_ROProceduralGeo &src);
    /// Destructor
    virtual ~RAY_ROProceduralGeo();

    RAY_ROProceduralGeo& operator=(const RAY_ROProceduralGeo &src);

    /// Clear the geometry
    void	clear();

    /// @{
    /// Equivalence
    bool	operator==(const RAY_ROProceduralGeo &g) const
			{ return get() == g.get(); }
    bool	operator!=(const RAY_ROProceduralGeo &g) const
			{ return get() != g.get(); }
    /// @}

    /// @{
    /// Standard "pointer" operations
    const GU_Detail	*operator->() const	{ return get(); }
    const GU_Detail	&operator*() const	{ return *get(); }
    const GU_Detail	*get(int segment=0) const;
    /// @}

    /// Access the GU_ConstDetailHandle for a given segment
    GU_ConstDetailHandle	handle(int segment=0) const;

    /// Test validity
    bool		isValid() const;

    /// Bool operator on handle
    SYS_SAFE_BOOL	operator bool() const { return isValid(); }

    /// Return the maximum velocity displacement (so bounding boxes can be
    /// computed).
    fpreal	 velocityBounds() const;

    /// Return the number of motion segments
    int		 motionSegments() const;

    /// Return reference count on the underlying geometry
    int		 getRef() const;

protected:
    friend class RAY_Procedural;
    friend class RAY_ProceduralChild;
    class Data;

    /// @{
    /// Create a read handle with a geometry.
    /// @note This method will fail if the handle isn't currently empty
    bool	setGeometry(const GU_ConstDetailHandle &gdh);
    bool	setGeometry(const GU_ConstDetailHandle *gdh, int nsegments);
    /// @}

    // Internal method to create read-only geometry from a procedural
    RAY_ROProceduralGeo(RAY_Procedural *proc,
	    bool writeable);
    RAY_ROProceduralGeo(RAY_Procedural *proc,
	    const GU_ConstDetailHandle *gdh,
	    int nsegments,
	    const fpreal *shutter_times,
	    bool writeable);

    UT_SharedPtr<Data>	myData;
};

/// RAY_ProceduralGeo is a read-write handle on mantra's geometry.
///
/// You can create these handles by calling RAY_Procedural::createGeometry()
/// and then access the underlying GU_Detail, creating or manipulating
/// geometry.
///
/// It's also possible to add motion segments to the geometry in one of three
/// ways:
///	- Pass in a reference to another VGEO_ROProceduralGeo object.  This
///	  will add a motion segment, capturing transform changes on quadric and
///	  packed primitives as well as motion of points.
///	- You can add a "motion segment" attribute.  This method returns an
///	  attribute which you can fill out for the given segment.  This method
///	  is fine in most cases, but doesn't caputre transforms on primitives.
///	- Compute velocity blur.
class RAY_API RAY_ProceduralGeo : public RAY_ROProceduralGeo
{
public:
    /// Default constructor
    RAY_ProceduralGeo() { }

    /// Copy c-tor
    RAY_ProceduralGeo(const RAY_ProceduralGeo &src)
	: RAY_ROProceduralGeo(src)
    {
    }
    /// Destructor
    ~RAY_ProceduralGeo();

    RAY_ProceduralGeo	&operator=(const RAY_ProceduralGeo &src)
    {
	RAY_ROProceduralGeo::operator=(src);
	return *this;
    }

    /// @{
    /// Standard "pointer" operations
    GU_Detail	*operator->() const	{ return get(); }
    GU_Detail	&operator*() const	{ return *get(); }
    GU_Detail	*get(int segment=0) const;
    /// @}

    /// Access the geometry using a GU_DetailHandle.  If the geometry is
    /// locked, this method will return an invalid handle.  The geometry will
    /// be locked if it's already been passed to mantra.
    GU_DetailHandle	handle(int segment=0) const;

    /// Set the geometry for the first motion segment.  This method will fail if
    /// there are any motion segments already.  This will delete any geometry
    /// already assigned to the object.
    bool		setHandle(GU_ConstDetailHandle &gdh);

    /// Set the shutter start value.  This should be set @b before any motion
    /// segments are added to the geometry.  The default shutter open is 0.
    void		setShutterOpen(fpreal shutter);

    /// Append a motion segment.
    ///
    /// The shutter times for all segments should be sequential and added in
    /// increasing order.  A warning will be printed if the segments are added
    /// out of order.
    ///
    /// The method will return an invalid GU_DetailHandle if the geometry has
    /// been locked (passed to mantra).
    GU_DetailHandle	appendSegmentGeometry(fpreal shutter);

    /// Remove a motion segment that was added with appendSegmentGeometry()
    ///
    /// The method will return an invalid GU_DetailHandle if the geometry has
    /// been locked (passed to mantra).
    bool		removeSegmentGeometry(const GU_ConstDetailHandle &geo);

    /// Add a motion segment for the given attribute.  This returns the motion
    /// segment.  Note that adding motion segments this way doesn't capture
    /// per-primitive transforms (i.e. quadrics, packed primitives, etc.)
    ///
    /// The shutter times for all segments should be sequential and added in
    /// increasing order.  A warning will be printed if the segments are added
    /// out of order.
    ///
    /// The method will return a @c nullptr if the geometry has been locked
    /// (passed to mantra).
    ///
    /// This method may return a "detached" attribute.  The method should be
    /// called @b after the original geometry attributes have been created so
    /// the length of the segment attribute is the correct length.
    GA_Attribute	*appendSegmentAttribute(fpreal shutter,
				const char *name="P");

    /// Add motion blur based on the velocity attribute.  This returns the
    /// maximum velocity displacement (for bounding box computation).  The
    /// displacement is cached and can be retrieved by calling @c
    /// velocityBounds().
    ///
    /// The method will returns -1 if the geometry has been locked (passed to
    /// mantra).  This should be considered an error.
    fpreal		 addVelocityBlur(fpreal pre_blur, fpreal post_blur,
			     const UT_StringHolder &velocity_attribute = UTmakeUnsafeRef("v"),
			     int acceleration_segments = 1,
			     const UT_StringHolder &acceleration_attribute = UT_StringHolder());

    /// @{
    /// Change a geometry setting:
    ///	- <tt>changeSetting(const char *name, int argc, char **argv)</tt>
    ///	- <tt>changeSetting(const char *name, int argc, int *argv)</tt>
    ///	- <tt>changeSetting(const char *name, int argc, fpreal *argv)</tt>
    /// If the setting specified by "name" isn't found, the function will fail
    /// (return false).
    ///
    /// Some settings which can be changed on geometry include:
    /// - "timescale": The time scale for velocity motion blur
    /// - "segmentattrs": The attributes to keep on motion segments
    /// - "volumevelocitynames": Names for velocity attributes on volume prims
    bool	 changeSetting(const UT_StringRef &name,
				int argc, const char *const*argv);
    bool	 changeSetting(const UT_StringRef &name,
				int argc, const UT_StringHolder *argv);
    bool	 changeSetting(const UT_StringRef &name,
				int argc, const int *argv);
    bool	 changeSetting(const UT_StringRef &name,
				int argc, const fpreal *argv);
    /// @}

private:
    /// Create new geometry for a procedural.  Internal method.
    RAY_ProceduralGeo(RAY_Procedural *proc);
    RAY_ProceduralGeo(RAY_Procedural *proc,
		    const GU_ConstDetailHandle *geo,
		    int nsegments,
		    const fpreal *shutter_times,
		    bool writeable);

    friend class RAY_Procedural;
    friend class RAY_ProceduralChild;
};

/// Simple struct to capture the standard attributes for material
/// specifications on procedural geometry.
struct RAY_API RAY_PrimitiveMaterialAttributes
{
    GA_ROHandleS    myShopMaterialPath;
    GA_ROHandleS    myMaterialOverride;
    GA_ROHandleS    myPropertyOverride;
    GA_ROHandleS    myLightMask;
    GA_ROHandleS    myLightCategories;
    GA_ROHandleS    myCategories;
};

/// @brief Class representing a child object of the current procedural
///
/// When a RAY_Procedural is rendered, the procedural can create child
/// geometry objects, further procedurals, or even a mix of the two.
///
/// When creating geometry to be rendered, the code would look something along
/// the lines of: @code
///   // Create geometry by loading geometry from disk.
///   RAY_ProceduralGeo	geo = createGeometry();
///   geo->load("defgeo.bgeo");
///
///   // Add the geometry to the render
///   RAY_ProceduralChildPtr	obj = createChild();
///   obj->addGeometry(geo);
/// @endcode
///
/// When adding further procedural children, the code might look something
/// like: @code
///   RAY_ProceduralChildPtr	obj = createChild();
///   child->addProcedural(new procedural_subclass(parameters));
/// @endcode
///
/// @note:  RAY_ProceduralChildPtr's are thread safe.  So, unlike the
/// deprecated addGeometry()/addProcedural() methods, multiple child objects
/// can be created in a threaded fashion within a render() method. @code
///   // Create geometry by loading geometry from disk.
///   RAY_ProceduralGeo	geo = createGeometry();
///   geo->load("defgeo.bgeo");
///
///   UTparallelFor(UT_BlockedRange<int>(0, 100),
///	[=](const UT_BlockedRange<int> &r)) {
///	    for (auto it = r.begin(); it != r.end(); ++it) {
///		RAY_ProceduralChildPtr obj = createChild();
///		obj->addGeometry(geo);
///	    }
///	}
/// @endcode
///
/// See the HDK examples RAY_DemoMountain or RAY_DemoBox for concrete
/// examples.
class RAY_API RAY_ProceduralChild
    : public UT_IntrusiveRefCounter<RAY_ProceduralChild>
{
public:
     RAY_ProceduralChild(RAY_Procedural *parent);
    ~RAY_ProceduralChild();

    /// Test whether the child is valid.  This checks to see whether there's a
    /// valid geometry, child procedural or a volume attached.
    bool	isValid() const;

    /// Bool operator on handle
    SYS_SAFE_BOOL	operator bool() const { return isValid(); }

    /// Return parent procedural
    const RAY_Procedural	*parent() const	{ return myParent; }

    /// Set the geometry for the child procedural.  This is the preferred
    /// interface for setting a child's geometry.
    void	addGeometry(const RAY_ROProceduralGeo &geo);

    /// When rendering geometry with packed prims or heightfields, mantra will
    /// normally create special procedurals to handle each type of primitive
    /// automatically.  However, care must be taken when there are a mix of
    /// special primitives and other primitives.  For example, if there are
    /// agent primitives, they are also considered as packed primitives, so the
    /// packed procedural shouldn't double render them.  The AutoProcType is
    /// used internally to indicate what types of geometry should be allowed to
    /// show up when creating procedurals.  Note that this list may change in
    /// future versions.
    enum AutoProceduralType
    {
	AUTOPROCEDURAL_ANY,		// Create any special procedural types
	AUTOPROCEDURAL_AGENT,		// Create procedural for agents
	AUTOPROCEDURAL_PACK,		// Create procedural for packed
	AUTOPROCEDURAL_HEIGHTFIELD,	// Create procedural for heightfields
	AUTOPROCEDURAL_NONE,		// Just render geometry
    };
    /// Limit the types of procedurals which can be automatically created to
    /// handle special primitive types.  For example, if setting to
    /// AUTOPROCEDURAL_PACK, no special agent primitives will be created (but
    /// heightfields will).
    void	limitAutoProcedural(AutoProceduralType aproc);

    /// Set a child procedural.  There are two ways to add a procedural.
    ///
    /// The first process is to call with an argc/argv and an optional bounding
    /// box.  This invokes the standard mantra process of creating a procedural
    /// object.  Using this interface will cause the initialize() method to be
    /// called on the created procedural.  The creation status is returned.
    bool	 addProcedural(int argc, char *argv[],
				const UT_BoundingBox *box=0);
    /// The second process for adding a procedural is to add the procedural
    /// directly.  Using this process bypasses the standard mantra
    /// initialization proces (i.e. the initialize() virtual will not be
    /// called).  This can be used to add procedural objects which aren't
    /// registered with mantra (i.e. private classes).
    void	 addProcedural(RAY_Procedural *proc);

    /// Add a volume primitive to the currently open volume object.  Volume
    /// objects are reference counted, so if you need to keep a reference
    /// to the volume after calling addVolume(), use the incref()/decref()
    /// interface to retain a reference.  An easy way to do this is to own
    /// a UT_IntrusivePtr<VGEO_Volume> object.  If you do not own a reference,
    /// it's possible the VGEO_Volume pointer will be invalid after this call.
    void	 addVolume(VGEO_Volume *volume, fpreal shutter);

    /// Transform the currently open object (geometry, procedural, volume) at
    /// the shutter time specified.  The transform is a pre-transform that
    /// acts on the parent procedural's existing transform.  This interface
    /// should be used to achieve the expected parenting behavior for
    /// procedurals, in which the transform is an object space transform
    /// inside the parent space.
    void	 setPreTransform(const UT_Matrix4D &transform, fpreal shutter);

    /// Assign new style sheet node to the currently open object.
    /// Procedurals often prune the style sheet of entries that no longer
    /// apply to the geometry hierarchy.
    void	 setStylerInfo(const RAY_StylerInfo &styler_info);

    /// Transform the currently open object (geometry, procedural, volume) at
    /// the shutter time specified.  The transform is a post-transform that
    /// acts on the parent procedural's existing transform.  This method is
    /// preferred when you need to change the world transform from within
    /// the procedural.  In most cases, setPreTransform() is preferred
    /// over setTransform().
    void	 setTransform(const UT_Matrix4D &transform, fpreal shutter);

    /// @{
    /// Change any setting on the object (geometry, procedural, volume)
    /// - <tt>changeSetting(const char *name, const char *value)</tt> @n
    ///	  Parses the string into arguments and changes the setting If you want
    ///	  to change a setting to a single string value, it is better to call:
    ///	  changeSettings(name, 1, &value);
    ///	- <tt>changeSetting(const char *name, int argc, char **argv)</tt>
    ///	- <tt>changeSetting(const char *name, int argc, int *argv)</tt>
    ///	- <tt>changeSetting(const char *name, int argc, fpreal *argv)</tt>
    /// If the setting specified by "name" isn't found, the function will fail
    /// (return false).
    bool	 changeSetting(const char *name, const char *value,
				const char *style = "object");
    bool	 changeSetting(const char *name, int argc,
				const char *const*argv,
				const char *style = "object");
    bool	 changeSetting(const char *name, int argc,
				const UT_StringHolder *argv,
				const char *style = "object");
    bool	 changeSetting(const char *name, int argc, const int *argv,
				const char *style = "object");
    bool	 changeSetting(const char *name, int argc, const fpreal *argv,
				const char *style = "object");
    /// @}

    /// @{
    /// It's possible to add new settings to a child object.  These settings
    /// can be accessed in shaders using the VEX renderstate() function.
    ///
    /// It is illegal to:
    ///	- redefine a built-in property
    /// - change the type of an existing property
    /// - change the tuple size of an existing property
    ///
    /// This is equivalent to the ray_declare command in IFDs.
    bool	declareSetting(const char *name, const char *value);
    bool	declareSetting(const char *name, int argc,
				const char *const*argv);
    bool	declareSetting(const char *name, int argc, const int *argv);
    bool	declareSetting(const char *name, int argc, const fpreal *argv);
    /// @}

    /// Print out the object's settings to stderr
    void	debugSettings(const char *style="object") const;

    /// Convenience method that declares settings for all attributes associated
    /// with a given primitive, including the corresponding vertices, points,
    /// and parent detail. This is primarily used for packed primitives.
    /// The name of the setting will be the same as the attribute name prefixed
    /// with the given prefix.
    SYS_DEPRECATED_REPLACE(16.0, setPackedQuery)
    void	 declarePrimitiveAttributeSettings(
				const GEO_Primitive &prim,
				const char *prefix);

    /// Calling @c setShopMaterialPath() will simulate adding the
    /// "shop_materialpath" and optionally the "material_override" and
    /// attributes on the geometry.
    /// This must be called after the @c openGeometryObject() and
    /// before the corresponding @c closeObject() calls.
    /// The @c property_map string is used to map the Houdini parameter names
    /// in the @c material_override string to mantra property names.  Without
    /// this map, properties will not be overridden properly.
    bool	 setShopMaterialPath(const UT_StringHolder &shop_materialpath,
		    const UT_StringHolder &material_override=UT_StringHolder(),
		    const UT_StringHolder &property_map=UT_StringHolder());

    /// Convenience method to check for the following attributes on a primitive
    /// This method will check for the following attributes:
    ///   - shop_materialpath
    ///   - material_override
    ///   - property_override
    ///   - lightcategories
    ///   - lightmask
    ///   - categories
    /// @{
    void	 processPrimitiveMaterial(const GEO_Primitive *prim);

    /// This version takes a set of cached attributes for faster processing.
    void	 processPrimitiveMaterial(
		    const GEO_Primitive *prim,
		    const RAY_PrimitiveMaterialAttributes &attribs);
    /// @}

    /// This function has no implementation in H16.0 and beyond.
    /// Please use @c setPackedQuery();
    SYS_DEPRECATED_REPLACE(16.0, setPackedQuery)
    void	 declarePackedPrimRenderStateSetting(const GEO_Primitive &)
		    { return; }

    /// Set the packed query lookup for this procedural.  This method takes
    /// ownership of the query passed in.
    void	setPackedQuery(RAY_PackedQuery *q);

    /// @{
    /// @private
    /// Only called by RAY_Procedural
    ray_ProcInstance	*handle()	{ return myHandle; }
    /// @}

private:
    RAY_Procedural	*myParent;
    ray_ProcInstance	*myHandle;
};

using RAY_ProceduralChildPtr = UT_IntrusivePtr<RAY_ProceduralChild>;

/// @brief Procedural primitive for mantra (RAY)
///
/// The RAY_Procedural class provides a means to create procedural geometry
/// during the rendering process.
class RAY_API RAY_Procedural
{
public:
	     RAY_Procedural();
    virtual ~RAY_Procedural();

    /// The class name is used in diagnostic messages.  It can simply be the
    /// name of the class, or alternatively, you can choose a unique name for
    /// each procedural instance.
    virtual const char	*className() const = 0;

    /// The initialize method is called when the procedural is created.
    /// Returning zero (failure) will abort the rendering of this procedural.
    ///
    /// The bounding box passed in is the user defined bounding box.  If the
    /// user didn't specify a bounding box, then the box will be NULL.
    virtual int		 initialize(const UT_BoundingBox *box) = 0;

    /// The bounding box is the "object space" bounds of the procedural.
    virtual void	 getBoundingBox(UT_BoundingBox &box) = 0;

    /// The render method is called when the procedural is required to either
    /// generate geometry or split into more procedurals. Inside the
    /// render() method you should make calls to open/add/close geometry or
    /// sub-procedurals.
    ///
    /// The way that a procedural splits is critical to the performance of
    /// mantra's ray tracer. For optimal performance, make sure that
    /// sub-procedural bounding boxes overlap as little as possible - which
    /// allows mantra to build more optimal trees for ray tracing.
    ///
    /// In multi-threaded renders, the render() method needs to be reentrant
    /// - that is, the render() method on different RAY_Procedural objects
    /// may be called simultaneously by different threads.  This means that
    /// you should design the code in your procedural such that global data
    /// structures are protected by a thread lock (for example, a UT_Lock).
    /// @note The render method may be called more than once during a render to
    /// regenerate the geometry.
    virtual void	 render() = 0;

    /// Called by IPR so that the procedural can determine whether it is
    /// equal to the updated procedural. This is useful if the procedural
    /// is dependent on object properties other than the its arguments.
    virtual bool	 isEqual( const RAY_Procedural *) const { return true; }


    /// This is a stop-gap measure to help improve LOD computation when the
    /// procedural generates multiple instances of the same underlying
    /// geometry.  If this method returns false, the procedural will not be
    /// processed when flattenprocedural is set.
    virtual bool	canGenerateInstancedGeometry() const { return false; }

    /// The getParm() methods evaluate parameters attached to the procedural.
    /// The lookup may be performed by name or by token.  Each parameter has a
    /// unique token associated with it.  The token lookup methods are more
    /// efficient than the name lookups and should be almost as efficient as
    /// accessing member data.

    /// Parameters to the procedural are stored as a list of RAY_ProceduralArg
    /// objects.  Each parameter is given a unique integer.  This allows for
    /// more efficient evaluation (avoiding comparing strings).
    int			  lookupParmToken(const char *name) const;

    /// @{
    /// Get the raw data pointers for a parameter by either name or value.
    /// These functions return the size of the array in @c size, and will fail
    /// if the type requested doesn't match the storage type.
    /// @see import()
    ///
    const int64			*getIParm(int token, size_t &size) const;
    const fpreal		*getFParm(int token, size_t &size) const;
    const UT_StringHolder	*getSParm(int token, size_t &size) const;

    const int64		*getIParm(const char *name, size_t &size) const
    {
	auto &&id = lookupParmToken(name);
	return id >= 0 ? getIParm(id, size) : nullptr;
    }
    const fpreal	*getFParm(const char *name, size_t &size) const
    {
	auto &&id = lookupParmToken(name);
	return id >= 0 ? getFParm(id, size) : nullptr;
    }
    const UT_StringHolder *getSParm(const char *name, size_t &size) const
    {
	auto &&id = lookupParmToken(name);
	return id >= 0 ? getSParm(id, size) : nullptr;
    }
    /// @}

    /// @{
    /// The import functions can be used as a general purpose query mechanism.
    /// This will import values from:
    ///	- The argument list for the procedural
    ///	- The argument list for the object defining the procedural
    ///	- A global rendering setting.
    /// Aside from a parameter name, the name could be "object:name",
    /// "object:shadingquality", "camera:pixelaspect", or any of the other
    /// setting defined in mantra.
    bool	 import(const char *name, int32 *value, int vectorsize) const;
    bool	 import(const char *name, int64 *value, int vectorsize) const;
    bool	 import(const char *name, fpreal32 *value, int vectorsize) const;
    bool	 import(const char *name, fpreal64 *value, int vectorsize) const;
    SYS_DEPRECATED(16.0) bool	 import(const char *name, UT_String &result, int idx=0) const;
    bool	 import(const char *name, UT_StringHolder &result, int idx=0) const;
    /// @}

    /// @{
    /// If you've opened a handle using queryObject(), queryLight() or
    /// queryMaterial(), it's possible to query the settings of that object.
    /// This makes it possible to query the parameters/values/settings of other
    /// objects in the scene.
    bool	  import(RAY_Handle handle, const char *name,
        	         int32 *value, int vectorsize) const;
    bool	  import(RAY_Handle handle, const char *name,
        	         int64 *value, int vectorsize) const;
    bool	  import(RAY_Handle handle, const char *name,
        	         fpreal32 *value, int vectorsize) const;
    bool	  import(RAY_Handle handle, const char *name,
        	         fpreal64 *value, int vectorsize) const;
    SYS_DEPRECATED(16.0) bool	  import(RAY_Handle handle, const char *name,
        	         UT_String &result, int idx = 0) const;
    bool	  import(RAY_Handle handle, const char *name,
        	         UT_StringHolder &result, int idx = 0) const;

    /// Using a handle, import all settings on the object. By default this
    /// includes settings at default values. However, by setting local_only
    /// to true, it's possible to query only values from the handle that were
    /// explicitly set on that object/light/material.
    bool	  importAll(RAY_Handle handle, UT_Options &settings,
        	            bool local_only = false) const;
    /// @}

    /// Allocate geometry for this procedural
    RAY_ProceduralGeo	createGeometry() const
    {
	return RAY_ProceduralGeo(SYSconst_cast(this));
    }

    /// When passed a GU_DetailHandle, mantra will run some internal
    /// optimizations on the geometry.  These include, but are not limited to:
    /// - Optimizing layout of polygon soups
    /// - Conversion of closed Bezier/NURBS curves
    /// - Clearing DAGS on pasted surfaces
    /// - Creation of the N attribute for smooth shading of polygons
    /// However, when passed a GU_ConstDetailHandle, mantra assumes that it's
    /// not allowed to alter the geometry and thus does not run these
    /// optimizations.  Some of these "optimizations" will change behaviour
    /// (i.e. without optimization, closed Bezier/NURBS will not render).  This
    /// hook allows you to run the internal optimizations on the geometry
    /// before passing the GU_ConstDetailHandle to mantra.
    /// This is typically called: @code
    ///     int                         computeN;
    ///     if (!import("geometry:computeN", &computeN, 1))
    ///         computeN = 1;
    ///     optimizeGeometry(gdh, computeN != 0);
    /// @endcode
    static void	optimizeGeometry(GU_Detail &gdp,
				bool create_normals_if_needed);
    static void	optimizeGeometry(GU_DetailHandle &gdp,
				bool create_normals_if_needed);

    /// @{
    /// Allocate geometry given a GU_DetailHandle/GU_ConstDetailHandle
    RAY_ROProceduralGeo	createGeometry(GU_DetailHandle &gdh) const;
    RAY_ROProceduralGeo	createGeometry(GU_ConstDetailHandle &gdh) const;
    /// @}

    /// @{
    /// Allocate geometry given an array of GU_DetailHandle's, one for each
    /// motion segment.
    RAY_ROProceduralGeo	createGeometry(const GU_DetailHandle *gdh,
					int nsegments,
					const fpreal *shutter_times=nullptr
				) const;
    RAY_ROProceduralGeo	createGeometry(const GU_ConstDetailHandle *gdh,
					int nsegments,
					const fpreal *shutter_times=nullptr
				) const;
    /// @}

    /// @private: Forward declaration of handle map
    class HandleMap;

    /// @{
    /// @private: Access data internals
    const RAY_ProceduralData	*data() const { return myData; }
    RAY_ProceduralData		*data() { return myData; }
    /// @}

protected:
    /// Query a geometry attribute segment for the given segment index.
    /// For seg == 0, this method simply returns attr.  For example: @code
    ///   const GA_Attribute *p1 = queryGeometrySegment(gdp->getP(), 1);
    /// @endcode
    const GA_Attribute	*queryGeometrySegment(
				const GA_Attribute *attr, int seg) const;

    /// Delete existing geometry segments and prepare the detail for
    /// addition of new motion blur segments.  This method can be useful if
    /// you want to reuse geometry between renders but change the motion
    /// blur.  Do not call this method on geometry that has already been
    /// added to the current render.
    void		 clearGeometrySegments(GA_Attribute *attr);
    
    // The following methods are now obsolete.  The settings/flags should be
    // set by calling changeSetting()
    //	- setGeometryFlag()
    //	- copyGeometryFlags()

    /// This method allows you to compute the level of detail (LOD) of an
    /// arbitrary bounding box.  The LOD indicates the rough screen space
    /// size of the bounding box, as an average projected length in
    /// micropolygons.
    ///
    /// For example, if the box occupies 10 horizontal pixels and 20
    /// vertical pixels, at a shading quality of 1.0 the LOD will be around
    /// 15. At a shading quality of 2.0, the LOD would be around 30. If the
    /// box surrounds the camera, the LOD will be clamped at a large
    /// number.
    fpreal		 getLevelOfDetail(const UT_BoundingBox &box) const;

    /// Create a child object.  Each child can be operated upon in separate
    /// threads.  This allows a single procedural to create multiple child
    /// objects in a threaded fashion.
    RAY_ProceduralChildPtr	createChild() const
    {
	return RAY_ProceduralChildPtr(
			new RAY_ProceduralChild(SYSconst_cast(this)));
    }

    /// When the render method is called, the procedural can add objects to
    /// the scene.  It is possible for the render() method to add multiple
    /// objects.
    ///
    /// The process for adding a geometry object: 
    ///	 - open a geometry object (openGeometryObject())
    ///	 - add geometry to the object (addGeometry())
    ///  - modify any settings (changeSetting())
    ///	 - close the object (closeObject())
    ///
    /// The process for adding a new procedural is fairly similar:
    ///	 - open a procedural object (openProcecuralObject())
    ///	 - add procedural(s) to the object (addProcedural())
    ///	 - modify any settings (changeSetting())
    ///	 - close the object (closeObject())
    ///
    /// For a geometry object, deformation motion blur is done by adding
    /// multiple geometry objects or by using appendGeometrySegment() to
    /// append motion blur segments without allocating new GU_Detail
    /// objects.
    ///
    /// If settings are not overridden by the procedural, they will be
    /// inherited from the object defining the procedural.
    ///
    /// The shutter time should be between 0 and 1.  Shutter values are
    /// used to order the geometry objects added to the procedural, so that
    /// the order of calls to addGeometry() will not affect the resulting
    /// motion blur.  Additionally, the distribution and offset of shutter
    /// values are currently ignored.  For example, shutters of (0, 0.9, 1)
    /// and (0, 0.5, 1) will produce identical motion blur, as will
    /// shutters of (0, 0.5) and (0.5, 1).  The difference between the
    /// largest and smallest shutter value will be used to scale the
    /// positions of additional motion segments toward the initial
    /// position.  For example, if a shutter of (0, 0.4) is specified, with
    /// P0 and P1 being points on the first and second segment, the
    /// rendered motion end points will be P0 and P0+0.4*(P1-P0).
    ///
    /// The renderer assumes ownership of any geometry or procedurals written
    /// to objects.  Thus, the geometry and procedurals should not be free'd
    /// by the user.
    SYS_DEPRECATED(16.0) void	 openGeometryObject();

    /// Before adding the geometry, you can change @b geometry settings by
    /// calling changeSettings().  For example, if you want to force
    /// computation of N, you can call:
    ///		changeSetting("computeN", "true", "geometry");
    /// This needs to be done @b BEFORE the addGeometry() call is made.
    /// Object settings can be changed after the geometry has been added.
    ///
    /// @warning The @c addGeometry(GU_Detail *) method was originally
    /// deprecated in H14.0, and you should seriously consider modernizing your
    /// code.  The current implementation looks like this and doesn't properly
    /// handle motion blur (since the modern implementation stores motion
    /// segments on the RAY_ProceduralGeo object): @code
    ///  void RAY_Procedural::addGeometry(GU_Detail *gdp, fpreal)
    ///  {
    ///     GU_DetailHandle   gdh;
    ///     gdh.allocateAndSet(gdp, false);  // User must delete gdp
    ///     myChild->addGeometry(createGeometry(gdh));
    ///  }
    /// @endcode
    SYS_DEPRECATED(16.0) void	 addGeometry(GU_Detail *gdp, fpreal shutter);

    /// Open a procedural object
    SYS_DEPRECATED(16.0) int		 openProceduralObject();
    /// Add a procedural to the opened procedural object.  Use this method
    /// when splitting a procedural into multiple sub-procedurals. This can
    /// be useful when it would be prohibitive (memory-wise) to generate
    /// geometry immediately, and allows mantra to delay generation of
    /// geometry for the sub-procedural until its bounding box is hit.
    ///
    /// If you've allocated the procedural yourself, mantra assumes that the
    /// procedural is fully initialized.  The initialize() method will not be
    /// called.
    /// @see openProceduralObject()
    SYS_DEPRECATED(16.0) void	 addProcedural(RAY_Procedural *proc);
    /// Add a procedural the the opened procedural object.  This method allows
    /// you to create any registered procedural by passing @c argc and @c argv
    /// arguments to initialize the procedural.
    ///
    /// When allocating a procedural with arguments, mantra will call the
    /// initialize() method.
    ///
    /// @return 0 or 1 for failure/success
    /// @see openProceduralObject()
    SYS_DEPRECATED(16.0) int		 addProcedural(int argc, char *argv[],
				const UT_BoundingBox *box=0);

    /// Open a volume object
    SYS_DEPRECATED(16.0) void	 openVolumeObject();
    /// Add a volume primitive to the currently open volume object.  Volume
    /// objects are reference counted, so if you need to keep a reference
    /// to the volume after calling addVolume(), use the incref()/decref()
    /// interface to retain a reference.  An easy way to do this is to own
    /// a UT_IntrusivePtr<VGEO_Volume> object .
    SYS_DEPRECATED(16.0) void	 addVolume(VGEO_Volume *volume, fpreal shutter);

    /// Transform the currently open object (geometry, procedural, volume) at
    /// the shutter time specified.  The transform is a pre-transform that
    /// acts on the parent procedural's existing transform.  This interface
    /// should be used to achieve the expected parenting behavior for
    /// procedurals, in which the transform is an object space transform
    /// inside the parent space.
    SYS_DEPRECATED(16.0) void	 setPreTransform(const UT_Matrix4D &transform, fpreal shutter);

    /// Transform the currently open object (geometry, procedural, volume) at
    /// the shutter time specified.  The transform is a post-transform that
    /// acts on the parent procedural's existing transform.  This method is
    /// preferred when you need to change the world transform from within
    /// the procedural.  In most cases, setPreTransform() is preferred
    /// over setTransform().
    SYS_DEPRECATED(16.0) void	 setTransform(const UT_Matrix4D &transform, fpreal shutter);

    /// Assign new style information to the currently open object.
    /// Procedurals often prune the style sheet of entries that no longer
    /// apply to the geometry hierarchy.
    SYS_DEPRECATED(16.0) void	 setStylerInfo(const RAY_StylerInfo &styler_info);

    /// @{
    /// Change any setting on the object (geometry, procedural, volume)
    /// - <tt>changeSetting(const char *name, const char *value)</tt> @n
    ///	  Parses the string into arguments and changes the setting If you want
    ///	  to change a setting to a single string value, it is better to call:
    ///	  changeSettings(name, 1, &value);
    ///	- <tt>changeSetting(const char *name, int argc, char **argv)</tt>
    ///	- <tt>changeSetting(const char *name, int argc, int *argv)</tt>
    ///	- <tt>changeSetting(const char *name, int argc, fpreal *argv)</tt>
    /// If the setting specified by "name" isn't found, the function will fail
    /// (return false).
    SYS_DEPRECATED(16.0) bool	 changeSetting(const char *name, const char *value,
				const char *style = "object")
    {
	if (!checkChild("changeSetting"))
	    return false;
	return myChild->changeSetting(name, value, style);
    }
    SYS_DEPRECATED(16.0) bool	 changeSetting(const char *name, int argc,
				const char *const*argv,
				const char *style = "object")
    {
	if (!checkChild("changeSetting"))
	    return false;
	return myChild->changeSetting(name, argc, argv, style);
    }
    SYS_DEPRECATED(16.0) bool	 changeSetting(const char *name, int argc, const int *argv,
				const char *style = "object")
    {
	if (!checkChild("changeSetting"))
	    return false;
	return myChild->changeSetting(name, argc, argv, style);
    }
    SYS_DEPRECATED(16.0) bool	 changeSetting(const char *name, int argc, const fpreal *argv,
				const char *style = "object")
    {
	if (!checkChild("changeSetting"))
	    return false;
	return myChild->changeSetting(name, argc, argv, style);
    }
    /// @}

    /// @{
    /// Once an object is open for processing, it's possible to add settings
    /// specifically for that individual object.  These properties can be
    /// accessed using the VEX renderstate() function.
    ///
    /// It is illegal to:
    ///	- redefine a built-in property
    /// - change the type of an existing property
    /// - change the tuple size of an existing property
    ///
    /// This is equivalent to the ray_declare command in IFDs.
    SYS_DEPRECATED(16.0) bool	declareSetting(const char *name, const char *value)
    {
	if (!checkChild("declareSetting"))
	    return false;
	return myChild->declareSetting(name, value);
    }
    SYS_DEPRECATED(16.0) bool	declareSetting(const char *name, int argc,
				const char *const*argv)
    {
	if (!checkChild("declareSetting"))
	    return false;
	return myChild->declareSetting(name, argc, argv);
    }
    SYS_DEPRECATED(16.0) bool	declareSetting(const char *name, int argc, const int *argv)
    {
	if (!checkChild("declareSetting"))
	    return false;
	return myChild->declareSetting(name, argc, argv);
    }
    SYS_DEPRECATED(16.0) bool	declareSetting(const char *name, int argc, const fpreal *argv)
    {
	if (!checkChild("declareSetting"))
	    return false;
	return myChild->declareSetting(name, argc, argv);
    }
    /// @}

    /// Calling @c setShopMaterialPath() will simulate adding the
    /// "shop_materialpath" and optionally the "material_override" and
    /// attributes on the geometry.
    /// This must be called after the @c openGeometryObject() and
    /// before the corresponding @c closeObject() calls.
    /// The @c property_map string is used to map the Houdini parameter names
    /// in the @c material_override string to mantra property names.  Without
    /// this map, properties will not be overridden properly.
    SYS_DEPRECATED(16.0) bool	 setShopMaterialPath(
				    const char *shop_materialpath,
				    const char *material_override=nullptr,
				    const char *property_map=nullptr);

    /// Print out the object's settings to stderr
    void	debugSettings(const char *style="object") const;

    /// Cache the set of attributes to be used by
    /// RAY_ProceduralChild::processPrimitiveMaterial()
    static void	 lookupPrimitiveMaterialAttributes(
		    const GEO_Detail &geo,
		    RAY_PrimitiveMaterialAttributes &attribs);

    /// Convenience method to check for the following attributes on a primitive
    /// This method will check for the following attributes:
    ///   - shop_materialpath
    ///   - material_override
    ///   - property_override
    ///   - lightcategories
    ///   - lightmask
    ///   - categories
    SYS_DEPRECATED(16.0) void	 processPrimitiveMaterial(const GEO_Primitive *prim);

    /// The following methods are for backwards compatibility and are
    /// simply wrappers around changeSettings().
    /// @private
    SYS_DEPRECATED(16.0) void	setSurface(const char *shader, fpreal=0);
    /// @private
    SYS_DEPRECATED(16.0) void	setDisplacement(const char *shader, fpreal=0);
    /// @private
    SYS_DEPRECATED(16.0) void	setDisplacementBounds(fpreal bounds);
    /// @private
    SYS_DEPRECATED(16.0) void	setComputeN(int value);

    /// After all geometry, procedurals, volumes have been added and all the
    /// attributes have been set, the object can be closed.
    SYS_DEPRECATED(16.0) void	 closeObject();

    /// Load materials from an IFD stream.  This is the code used by the file
    /// procedural to load material definitions.  Using this to load data other
    /// than material statements will likely cause bad things to happen.
    bool	parseMaterialIFD(const char *filename);

protected:
    /// Query the number of transform samples for the world.
    int			 queryWorldTransformSamples() const;

    /// Get the world transform.
    UT_Matrix4D		 queryWorldTransform(int sample) const;

    /// It is possible to query information about other objects in the scene to
    /// some extent.  This is done using the following query methods.

    /// Find a handle to a given object in the scene.  Note that for objects
    /// which are rendered using point instances, the object handle will be a
    /// single instance rather than all of the instances.
    ///
    /// If a null pointer is passed in for the name, then the object refers to
    /// the object containing this procedural.  The object handle does not need
    /// to be closed.
    ///
    /// Note that the initialize() method on procedurals is called as the IFD
    /// is parsed, so not all objects may be available at this time (depending
    /// on the order of objects in the IFD).  The render() method is called
    /// after the entire scene has been parsed, so other objects should be
    /// available at that point.  You should always get a valid handle when
    /// querying your own object (passing an empty string for the name) in the
    /// initialize method.
    RAY_ObjectHandle	 queryObject(const char *name) const;

    /// Find a handle to a given material in the scene. The material handle does
    /// not need to be closed.
    RAY_MaterialHandle	 queryMaterial(const char *name) const;

    /// Find a handle to a given light in the scene. The light handle does
    /// not need to be closed.
    RAY_LightHandle	 queryLight(const char *name) const;

    //
    // Given an object handle, it is now possible to query information about
    // the object.
    //

    /// Query the number of transform samples for a given query object
    /// @see queryObject()
    int			 queryTransformSamples(RAY_Handle handle) const;
    /// Get the transform associated with an object in the scene
    /// @see queryObject()
    const UT_Matrix4D	&queryTransform(RAY_Handle handle, int sample) const;

    /// Get the shading transform associated with an object in the scene
    /// @see queryObject()
    const UT_Matrix4D	&queryShaderTransform(RAY_ObjectHandle handle,
                    	                      int sample) const;

    /// Get the style sheet associated with an object in the scene.
    /// @see queryObject()
    const STY_Styler	&queryStyler(RAY_ObjectHandle handle) const;

    /// Find out the name of the object queried.  This is useful when trying to
    /// find out which object the procedural belongs to (i.e. queryObject(0))
    /// @see queryObject()
    const char		*queryName(RAY_Handle handle) const;

    /// @private Use queryName instead.
    const char		*queryObjectName(RAY_Handle handle) const;

    /// Get the name of the object which owns this procedural.
    const char		*queryRootName() const;

    /// Find out how many geometry samples associated with the object.
    /// @see queryObject()
    int			 queryGeometrySamples(RAY_ObjectHandle handle) const;

    /// Access the geometry from another object (or the geometry passed to this
    /// object in the IFD).  This method will return all segments of geometry
    /// for the handle's object.
    RAY_ROProceduralGeo	queryGeometry(RAY_ObjectHandle handle) const;

    /// Find out shader information about the object.  This information is only
    /// roughly accurate since the object shaders may be overridden by geometry
    /// attributes.  The shader information returned is the object level
    /// shaders.  The shaders returned are simply the setting bound at the
    /// instance level (including arguments).
    /// @private: Use import()
    SYS_DEPRECATED(16.0) void	querySurfaceShader(RAY_ObjectHandle handle,
        	                   UT_String &shader) const
    {
	UT_StringHolder	tmp;
	if (!import(handle, "object:surface", tmp, 0))
	    tmp.clear();
	shader = tmp;
    }
    /// @private: Use import()
    SYS_DEPRECATED(16.0) void	queryDisplacementShader(RAY_ObjectHandle h,
        	                        UT_String &shader) const
    {
	UT_StringHolder	tmp;
	if (!import(h, "object:displace", tmp, 0))
	    tmp.clear();
	shader = tmp;
    }
    /// @private: Use import()
    SYS_DEPRECATED(16.0) fpreal	queryDisplacementBounds(RAY_ObjectHandle handle)
    {
	fpreal	dbound;
	if (!import(handle, "object:displacebound",
			&dbound, 1))
	    dbound = 0;
	return dbound;
    }
    /// @private: Use import()
    SYS_DEPRECATED(16.0) int	queryPhantom(RAY_ObjectHandle handle)
    {
	int		phantom;
	if (!import(handle, "object:phantom", &phantom, 1))
	    phantom = 0;
	return phantom;
    }
    /// @private: Use import()
    SYS_DEPRECATED(16.0) int	queryReflectBounce(RAY_ObjectHandle handle)
    {
	int		bounce;
	if (!import(handle, "object:reflectbounce", &bounce, 1))
	    bounce = 0;
	return bounce;
    }
    /// @private: Use import()
    SYS_DEPRECATED(16.0) int	queryRefractBounce(RAY_ObjectHandle handle)
    {
	int		bounce;
	if (!import(handle, "object:refractbounce", &bounce, 1))
	    bounce = 0;
	return bounce;
    }
    /// @private: Use import()
    SYS_DEPRECATED(16.0)
    void	queryReflectScope(RAY_ObjectHandle handle, UT_String &mask)
    {
	UT_StringHolder	tmp;
	if (!import(handle, "object:reflectmask", tmp, 0))
	    tmp.clear();
	mask = tmp;
    }
    /// @private: Use import()
    SYS_DEPRECATED(16.0)
    void	queryRefractScope(RAY_ObjectHandle handle, UT_String &mask)
    {
	UT_StringHolder	tmp;
	if (!import(handle, "object:refractmask", tmp, 0))
	    tmp.clear();
	mask = tmp;
    }

private:
    RAY_ROProceduralGeo	makeGeometry(const GU_ConstDetailHandle *gdh,
					int nsegments,
					const fpreal *shutter_times,
					bool writeable) const;

    template <typename T>
    bool	importType(RAY_Handle handle, const char *name,
			   T &value, int vectorsize) const;

    bool	checkChild(const char *method) const;
    void	processChild(RAY_ProceduralChild *kid);

private:
    RAY_ProceduralData		*myData;
    RAY_ProceduralChildPtr	 myChild;
    friend class RAY_ProceduralChild;
};

#endif
