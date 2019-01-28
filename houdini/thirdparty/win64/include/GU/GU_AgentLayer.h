/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_AgentLayer.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_AgentLayer__
#define __GU_AgentLayer__

#include "GU_API.h"
#include "GU_AgentRig.h"
#include "GU_AgentShapeDeformer.h"
#include "GU_AgentShapeLib.h"
#include "GU_PackedImpl.h"
#include <UT/UT_BoundingBox.h>
#include <UT/UT_Map.h>
#include <UT/UT_VectorTypes.h>

class GU_AgentLayer;
typedef UT_IntrusivePtr<GU_AgentLayer> GU_AgentLayerPtr;
typedef UT_IntrusivePtr<const GU_AgentLayer> GU_AgentLayerConstPtr;

/// A packed agent
///
/// The agent is composed of multiple named layers.  Each layer consists of
/// zero or more shapes.
///
/// All agents must have a default layer.  This is the layer used when a
/// specific layer isn't found.  Example layers might be:
///	- "default":  The default layer
///	- "low":  Low resolution representation
///	- "medium":  Medium resolution
///	- "high": High resolution
///	- "collision": Used for collision detection
///
/// The packed agent conforms to the GU_PackedImpl API, but also provides
/// access to the layers/shapes.
class GU_API GU_AgentLayer : public UT_IntrusiveRefCounter<GU_AgentLayer>
{
public:
    typedef UT_StringHolder		NameType;
    typedef GU_AgentShapeLib::ShapePtr	ShapePtr;

    /// Used to store the result of parsing a shape binding from a JSON file.
    struct ShapeBindingData
    {
        UT_StringHolder myShapeName;
        UT_StringHolder myTransformName;
        UT_StringHolder myDeformerName;
        fpreal myBoundsScale;
    };

    /// Used to store the result of parsing a layer from a JSON file.
    struct LayerData
    {
        UT_StringHolder myLayerName;
        UT_Array<ShapeBindingData> myStatic;
        UT_Array<ShapeBindingData> myDeforming;
    };

    class ShapeBinding
    {
    public:
	ShapeBinding()
	    : myShapeName()		    // Definition of shape name
	    , myShapeId(-1)		    // Unique id corresponding to shape
	    , myOffset(GA_INVALID_OFFSET)   // Offset in shape library's detail.
	    , myTransformId(-1)		    // Root transform
            , myDeformer(nullptr)
	    , myBoundsScale(1.0)	    // BBox scale for the shape
	    , myShapePtr(NULL)
	{
            myBounds.makeInvalid();
	}

	ShapeBinding(const GU_AgentShapeLib &lib,
		     const NameType &name,
                     const ShapePtr &shape,
                     GA_Offset offset,
                     int transform_id,
                     bool deforming,
                     const UT_BoundingBoxF &bounds,
                     fpreal bounds_scale = 1.0)
            : ShapeBinding(lib, name, shape, offset, transform_id,
                           deforming ? theLinearSkinDeformer : nullptr,
                           bounds, bounds_scale)
	{
	}

	ShapeBinding(const GU_AgentShapeLib &lib,
		    const NameType &name,
		    const ShapePtr &shape,
		    GA_Offset offset,
		    int transform_id,
                    const GU_AgentShapeDeformerConstPtr &deformer,
                    const UT_BoundingBoxF &bounds,
		    fpreal bounds_scale = 1.0)
	    : myShapeName(name)
	    , myShapeId(shape->uniqueId())
	    , myOffset(offset)
	    , myTransformId(transform_id)
            , myBounds(bounds)
	    , myBoundsScale(bounds_scale)
	    , myShapePtr(shape)
            , myDeformer(deformer)
	{
	}

        int64           getMemoryUsage(bool inclusive) const;

	GA_Offset	offset() const		{ return myOffset; }
	int		transformId() const	{ return myTransformId; }
        const UT_BoundingBoxF &bounds() const   { return myBounds; }
	fpreal		boundsScale() const	{ return myBoundsScale; }
        bool            isDeforming() const     { return myDeformer != nullptr; }
        const GU_AgentShapeDeformerConstPtr deformer() const { return myDeformer; }

	bool		save(UT_JSONWriter &w, const GU_AgentRig& rig) const;
        static bool     load(UT_JSONParser &p, ShapeBindingData &data);

	// Access to the underlying shape library
	const NameType	&shapeName() const	{ return myShapeName; }
	int		 shapeId() const	{ return myShapeId; }
        ShapePtr         shape() const          { return myShapePtr; }

	// Check if our shape is out-of-date
	bool		 isDirty(const GU_AgentShapeLib &lib) const;

    private:
	ShapePtr	    myShapePtr;
	NameType	    myShapeName;
	int		    myShapeId;
	GA_Offset	    myOffset;
	int		    myTransformId;
        UT_BoundingBoxF	    myBounds; // Cached bounds for the shape.
	fpreal		    myBoundsScale;
        GU_AgentShapeDeformerConstPtr myDeformer;
    };
    typedef UT_Array<ShapeBinding>		ShapeArray;
    typedef ShapeArray::const_iterator          const_iterator;

    static GU_AgentLayerPtr addLayer(const char *unique_name,
                                     const GU_AgentRigConstPtr &rig,
                                     const GU_AgentShapeLibConstPtr &shapelib);
    static GU_AgentLayerPtr addLayerFromFile(
            const char *filename, const GU_AgentRigConstPtr &rig,
            const GU_AgentShapeLibConstPtr &shapelib, UT_StringArray &errors);

    /// Create a clone of a layer, referencing the specified rig and shape
    /// library. 'copy_external_ref' should be disabled if the new layer will
    /// have its bindings changed.
    static GU_AgentLayerPtr
    addLayerCopy(const GU_AgentLayer &src, const GU_AgentRigConstPtr &rig,
                 const GU_AgentShapeLibConstPtr &shapelib,
                 bool copy_external_ref);

private:
    // Use the static addLayer() method to create new layers.
    GU_AgentLayer(const char *unique_name, bool is_file,
                  const GU_AgentRigConstPtr &rig,
                  const GU_AgentShapeLibConstPtr &shapelib);

public:
    ~GU_AgentLayer();

    /// Add shape bindings to a layer.
    /// - @c shape_names: The shape name for each shape binding.
    /// - @c transforms: The transform index for each shape binding.
    /// - @c deformers: A GU_AgentShapeDeformer for each shape binding, or
    ///                 nullptr for static shapes.
    /// - @c bounds_scales: Optional scale for the bounding boxes.
    bool            construct(const UT_StringArray &shape_names,
                              const UT_IntArray &transforms,
                              const UT_Array<GU_AgentShapeDeformerConstPtr> &deformers,
                              const UT_FprealArray *bounds_scales = 0);

    /// Add shape bindings to a layer.
    /// - @c shape_names: The shape name for each shape binding.
    /// - @c transforms: The transform index for each shape binding.
    /// - @c deforming: Whether each shape is static or deforming.
    /// - @c bounds_scales: Optional scale for the bounding boxes.
    bool            construct(const UT_StringArray &shape_names,
                              const UT_IntArray &transforms,
                              const UT_Array<bool> &deforming,
                              const UT_FprealArray *bounds_scales = 0);

    /// Add the shape bindings from another layer.
    bool            copyShapeBindings(const GU_AgentLayer &source);

    int64           getMemoryUsage(bool inclusive) const;

    /// Return a unique name for the layer (since there may e.g. be many
    /// different layer definitions named 'collision'). This is the filename if
    /// the layer was loaded from disk.
    const NameType &uniqueName() const          { return myUniqueName; }

    /// Name accessor
    /// @{
    const NameType &name() const		{ return myLayerName; }
    void	    setName(const NameType &name) { myLayerName = name; }
    /// @}

    /// Return whether the layer was loaded from disk.
    bool            isFile() const              { return myIsFile; }
    /// Clear the flag marking that the layer references a file on disk.
    void            clearIsFile();

    /// Return the rig associated with the layer.
    const GU_AgentRig &rig() const { return *myRig; }

    /// Return the underlying geometry.
    GU_ConstDetailHandle    detail() const { return myShapeLib->detail(); }

    /// Return the shape library for the layer.
    const GU_AgentShapeLib &shapeLib() const { return *myShapeLib; }

    /// @{
    /// Shape count
    exint	entries() const
		    { return myShapes.entries(); }
    exint	staticEntries() const
		    { return myStaticShapes.entries(); }
    exint	deformingEntries() const
		    { return myDeformingShapes.entries(); }
    /// @}

    /// Return the number of shapes bound to a specific transform.
    exint       numBoundShapes(exint xform_idx) const
    {
        return myTransformStarts(xform_idx + 1) - myTransformStarts(xform_idx);
    }

    bool	save(UT_JSONWriter &w) const;
    /// Load the layer from a JSON file.
    bool	load(UT_JSONParser &p);
    /// @{
    /// Parse the JSON file into an intermediate format, which can be used to
    /// load the layer at a later time.
    static bool load(UT_JSONParser &p, LayerData &data);
    bool	load(const LayerData &data, UT_StringArray &errors);
    /// @}

    /// Clear the layer
    void        clear();

    /// Get binding information for the given shape.
    const ShapeBinding	&shape(exint i) const { return myShapes(i); }

    /// Get binding information for the ith shape bound to the given transform.
    /// @see numBoundShapes
    const ShapeBinding  &boundShape(exint transform, exint i) const
                        { return myShapes(myTransformStarts(transform) + i); }

    /// Return the indices of the static shapes in the layer.
    /// @see shape
    const UT_IntArray   &getStatic() const { return myStaticShapes; }

    /// Return the indices of the deforming shapes in the layer.
    /// @see shape
    const UT_IntArray   &getDeforming() const { return myDeformingShapes; }

    /// Get the geometry for the given shape binding.
    GU_ConstDetailHandle shapeGeometry(const ShapeBinding &shape_binding) const;
    /// Get the geometry for the given shape index.
    GU_ConstDetailHandle shapeGeometry(exint i) const
                        { return shapeGeometry(shape(i)); }

    /// Get deformer source for given shape binding if present. Only valid for
    /// deforming shapes.
    const GU_LinearSkinDeformerSource *
    shapeDeformerSource(const ShapeBinding &shape) const;
    /// Get deformer source for given shape index if present. Only valid for
    /// deforming shapes.
    const GU_LinearSkinDeformerSource *
    shapeDeformerSource(exint i) const { return shapeDeformerSource(shape(i)); }

    /// Update the shape bindings when the shape library gets modified.
    void	updateShapes();

    /// Enlarge bounding box based on the shapes inside along with the
    /// transform array.
    void        enlargeBounds(UT_BoundingBox &box,
                              const UT_Array<UT_Matrix4F> &xforms) const;

    /// Expand the given velocity range using the v attributes in the shapes.
    bool	expandVelocityRange(UT_Vector3& vmin, UT_Vector3& vmax) const;

    /// Unpack geometry into a detail
    /// NOTE: This transforms the unpacked geometry by the agent's xform.
    bool	unpackToDetail(
                    GU_Detail &dest,
                    const GU_PrimPacked *prim,
                    const GU_Agent &agent,
                    const GU_AgentRig &rig,
                    const UT_Array<UT_Matrix4F> &xforms,
                    STY_StylerGroup *prim_styler_group,
                    const STY_Styler *parent_styler,
                    const UT_Matrix4D *transform) const;

    /// @{
    /// Unpack a specific shape into a detail.
    /// NOTE: This does not transform the geometry by the agent's xform.
    bool	unpackShapeToDetail(
                    GU_Detail &dest,
                    const ShapeBinding &binding,
                    const GU_Agent &agent,
                    const GU_AgentRig &rig,
                    const UT_Array<UT_Matrix4F> &xforms) const;
    bool	unpackShapeToDetail(
                    GU_Detail &dest,
                    exint shape_i,
                    const GU_Agent &agent,
                    const GU_AgentRig &rig,
                    const UT_Array<UT_Matrix4F> &xforms) const
    {
        return myShapes.isValidIndex(shape_i)
                   ? unpackShapeToDetail(dest, shape(shape_i), agent, rig, xforms)
                   : false;
    }
    /// @}

    /// @{
    /// Iterators
    const_iterator  begin() const { return myShapes.begin(); }
    const_iterator  end() const	{ return myShapes.end(); }
    /// @}

    /// Register a new shape deformer.
    static void registerDeformer(const GU_AgentShapeDeformerConstPtr &deformer);

    /// Return the deformer with the given name, or nullptr.
    static GU_AgentShapeDeformerConstPtr findDeformer(const UT_StringRef &name);

    /// Return a list of the registered deformers.
    static UT_Array<GU_AgentShapeDeformerConstPtr> registeredDeformers();

    /// Called by GU_Agent during startup to register default deformers and
    /// load any custom deformers.
    static void installDeformers();

    /// Return the default (linear skinning) deformer.
    static GU_AgentShapeDeformerConstPtr getLinearSkinDeformer()
    {
        return theLinearSkinDeformer;
    }

    /// Return the dual quaternion skinning deformer.
    static GU_AgentShapeDeformerConstPtr getDualQuatSkinDeformer()
    {
        return theDualQuatSkinDeformer;
    }

private:
    /// Add a shape from the shape library to the layer.
    /// @see sortShapeList
    bool addShape(const NameType &name, int transform_id,
                  const GU_AgentShapeDeformerConstPtr &deformer,
                  fpreal bounds_scale = 1.0);

    /// Sort myShapes and rebuild the static/deforming shape lists.
    void        sortShapeList();

    const GU_LinearSkinDeformerSource *
    shapeDeformerSource(const ShapeBinding &shape,
                        const GU_ConstDetailHandle &packed_geo) const;

    NameType		myUniqueName;
    NameType		myLayerName;
    bool                myIsFile;
    GU_AgentRigConstPtr myRig;
    GU_AgentShapeLibConstPtr myShapeLib;

    /// List of the shape bindings in the layer, ordered by transform.
    ShapeArray          myShapes;
    /// Indices of the static shapes in myShapes.
    UT_IntArray         myStaticShapes;
    /// Indices of the deforming shapes in myShapes.
    UT_IntArray         myDeformingShapes;
    /// For each transform, stores the index into myShapes for where the shape
    /// bindings attached to that transform begin.
    UT_IntArray         myTransformStarts;

    static GU_AgentShapeDeformerConstPtr theLinearSkinDeformer;
    static GU_AgentShapeDeformerConstPtr theDualQuatSkinDeformer;
};

extern "C" {
    /// Entry point for registering custom deformers.
    SYS_VISIBILITY_EXPORT extern void GUregisterAgentShapeDeformer(void *);
};

#endif
