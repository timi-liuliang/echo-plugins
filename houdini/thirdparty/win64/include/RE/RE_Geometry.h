/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_Geometry.h ( RE Library, C++)
 *
 * COMMENTS:
 *
 *	This class acts as a wrapper around multiple RE_VertexArray objects,
 *	similar to the way a framebuffer object contains multiple textures.
 *	Arrays can be attached to various attachments points, representing
 *	different vertex attribute data
 *
 */
#ifndef RE_GEOMETRY_H
#define RE_GEOMETRY_H

#include "RE_Types.h"

class RE_ElementArray;
class RE_Render;
class RE_Shader;
class RE_VertexState;
class RE_VertexArray;
class RE_VertexMap;
class re_Attrib;
class re_Connectivity;
class re_InstanceGroup;

#include <UT/UT_ValArray.h>
#include <UT/UT_String.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_SymbolTable.h>
#include <iosfwd>

#include "RE_API.h"
#include "RE_Types.h"
#include "RE_Texture.h"
#include "RE_CachedObject.h"

#define RE_GEO_POINTS_IDX 0
#define RE_GEO_WIRE_IDX   2
#define RE_GEO_SHADED_IDX 4

#define RE_GEO_ALL_SHADED 0x40000000

/// @brief A collection of vertex arrays defining a geometry object.
/// This class acts as a wrapper around multiple RE_VertexArray objects,
/// similar to the way a framebuffer object contains multiple textures.
/// Arrays can be attached to various attachments points, representing
/// different vertex attribute data
class RE_API RE_Geometry
{
public:
		RE_Geometry(int num_points = 0,
			    bool use_buffer_object = true);
	       ~RE_Geometry();

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_Geometry.
    int64	getMemoryUsage(bool inclusive) const;

    /// @brief Choose between buffer objects and client arrays
    /// By default, buffer objects are used if supported. This allows you to
    /// turn them off and use vertex arrays. This is useful if the geometry
    /// information is throw-away. If called when vertex arrays are attached,
    /// they will be cleared (and possibly deleted)
    void	useBufferObjects(bool use_buf = true);

    /// @brief Optimize this geometry with vertex array objects.
    /// Use an RE_VertexState (and GL Vertex Array Objects) to contain vertex
    /// arrays. Requires RE_EXT_VERTEX_ARRAY_OBJECT.
    void	useVertexState(bool use_state = true);

    /// Clears all the attached buffers, and removes them from the cache.
    void	purgeBuffers();

    /// @brief Number of points in the geometry.
    /// Number of points for the arrays declared as RE_ARRAY_POINT. This will
    /// clear the data in all point arrays and reset the connectivty.
    //@{
    bool	setNumPoints(int num);
    int		getNumPoints() const { return myNumPoints; }
    //@}

    /// Sets the number of elements in arrays declared as RE_ARRAY_VERTEX.
    //@{
    bool	setNumVertices(int num);
    int		getNumVertices() const { return myNumVertices; }
    //@}

    /// Sets the number of elements in arrays declared as RE_ARRAY_PRIMITIVE.
    //@{
    bool	setNumPrimitives(int num);
    int		getNumPrimitives() const { return myNumPrimitives; }
    //@}

    /// @brief Calulcate the number of points required for some primitives
    /// This will ensure that the buffer is big enough to hold enough vertices
    /// for the given number of primitives of the specified type. The buffer
    /// size can be reduced if 'shrink_if_smaller' is true, otherwise this will
    /// only increase the size of the buffer if needed. If 'shader' is given,
    /// it will check if the shader has a geometry shader and modify the
    /// number of vertices by the output primitive type and maximum #vertices.
    /// Shaders without a geometry shader will have no effect (as if NULL).
    /// For transform feedback, only POINTS, LINES or TRIANGLES are ever
    /// produced.
    void	resizePointsToFit(RE_PrimType primtype,
				  int	      num_primitives,
				  bool	shrink_if_smaller,
				  RE_Shader  *shader = NULL);

    /// @brief Create buffers for all attached arrays
    /// When the above methods are called with attached arrays, these arrays
    /// might have their storage freed. This call will ensure that all arrays
    /// are initialized. 
    void	initialize(RE_Render *r);

    // ---------------------------------------------------------------------
    /// @name Generic Vertex Attributes
    /// GL3 shaders use generic vertex attributes, which are identified by name.
    /// GL2 shaders can use a few generic vertex attributes along with the fixed
    /// builtin attributes. Generic attributes must be used for instanced,
    /// primitive or vertex atttributes.
    //@{

    /// place all attributes in a stashed list, which clears them externally
    /// but keeps them available internally. If createAttribute() or
    /// findCached...() are called, attributes will be fetched from the stashed
    /// list. This should not be called without a clearStashedAttributes()
    /// pairing it, nor called twice in a row.
    void		stashAttributes();

    /// Remove all remaining stashed attributes. If purge is true, remove them
    /// from the GL cache as well.
    int			clearStashedAttributes(bool purge_from_cache);
    
    /// fetch stashed attribute 'name', placing it back into the geometry's list
    RE_VertexArray     *recallStashedAttribute(const char *name);
    
    /// fetch stashed attribute 'name', but only if it is a const buffer.
    RE_VertexArray     *recallStashedVaryingAttribute(const char *attrib_name);
    
    /// fetch stashed attribute 'name', but only if it is a const buffer.
    RE_VertexArray     *recallStashedConstAttribute(const char *attrib_name);

    /// fetch stashed attribute 'name' from the stash list, removing it from
    /// the list and returning it if found. This does not place it back in the
    /// geometry's list of attributes.
    RE_VertexArray     *fetchStashedAttribute(const char *name);
   
    bool		hasStashedAttribute(const char *name);
   

    /// @brief Create a generic vertex attribute attached to 'attrib_name'
    /// Attributes are referenced by name, rather than buffer type. Array size
    /// is required if array type is RANDOM or INSTANCED.
    RE_VertexArray     *createAttribute(
				RE_Render *r,
				const char *attrib_name,
				RE_GPUType  data_format,
				int	    vectorsize,
				const void *data,
				RE_ArrayType atype = RE_ARRAY_POINT,
				int array_size = 0,
				RE_BufferUsageHint h = RE_BUFFER_WRITE_FREQUENT,
				const char *cache_prefix = NULL,
				int capacity = -1);

    /// @brief Create an instanced vertex attribute attached to 'attrib_name'
    /// Create an attribute which advances once every 'instance_stride' 
    /// instances rather than once per vertex. RE_EXT_INSTANCED_ARRAYS is
    /// required, and an instanced drawing method must be used (or it acts as a
    /// uniform).
    RE_VertexArray     *createInstancedAttribute(RE_Render *r,
						 const char *attrib_name,
						 RE_GPUType  data_format,
						 int	     vectorsize,
						 int	     instance_stride,
						 int         num_instances,
						 const void *data,
						 const char *cache_prefix=NULL,
						 int capacity = -1);
    /// @brief Create a constant attribute value
    /// Only RE_GPU_FLOAT32 and _FLOAT64 are supported for constant data.
    /// The 'data' point must hold at least 1 element (sizeof(type)*vectorsize).
    RE_VertexArray     *createConstAttribute(RE_Render *r,
					     const char *attrib_name,
					     RE_GPUType data_format,
					     int vectorsize,
					     const void *data);

    /// @brief Create a constant attribute if varying attribute is not found
    /// Ensures that there is at least a constant attribute value available if
    /// the attribute doesn't exist already. Only returns an array if one was
    /// created.
    RE_VertexArray     *assignAttributeDefault(RE_Render *r, 
					       const char *attrib_name,
					       RE_GPUType data_format,
					       int vectorsize,
					       const void *default_value);
    
    /// Attach an existing attribute using its name as the binding
    bool		attachAttribute(RE_VertexArray *attrib);
    /// Detatch an attribute from this object by name
    RE_VertexArray     *detachAttribute(const char *name);

    /// @brief Delete an attached vertex attribute by name
    /// delete the RE_VertexArray, and if purge_cache is true, remove the
    /// underlying data buffer from the  GL cache.
    bool		clearAttribute(const char *name,
				bool purge_cache=false);

    /// @brief Delete an attached vertex attribute by index
    /// delete the RE_VertexArray, and if purge_cache is true, remove the
    /// underlying data buffer from the  GL cache.
    bool		clearAttributeByIndex(int i,
				bool purge_cache=false);

    /// Fetch an attribute by name
    RE_VertexArray     *getAttribute(const char *name) const;

    /// Return an attribute's index byattribute name. Can change if attributes
    /// are removed or added.
    int			getAttributeIndex(const char *name) const;

    /// Fetch an attribute by known type
    RE_VertexArray     *getAttribute(RE_GenericAttribID attrib_id) const;

    /// Return the number of generic attributes currently attached.
    int			getNumAttributes() const;

    /// Return currently attached vertex attribute 'i' 
    RE_VertexArray     *getAttributeByIndex(int i) const;

    /// Return a vertex map representing the layout locations of the attributes
    const RE_VertexMap *getVertexMap() const { return myVertexMap; }

    //@}

    // ---------------------------------------------------------------------
    /// @name Addressable attributes
    /// Addressable attributes allow you to use buffers, but sample them like
    /// textures using texelFetch on samplerBuffer uniforms. Only buffer objects
    /// are supported.
    //@{

    /// @brief Create a randomly addressable attribute (texture buffer object)
    /// Attributes can be stored in texture buffers, and accessed out of
    /// the normal vertex flow, such as for vertex or primitive attributes.
    /// Unlike normal attributes, addressable attributes can have a different
    /// array size than the RE_Geometry's number of vertices.
    RE_VertexArray     *createAddressableAttribute(RE_Render *r,
						   const char *attrib_name,
						   int	       length,
						   RE_GPUType  data_format,
						   int	   vectorsize,
						   const void *data,
						   RE_ArrayType	atype
							= RE_ARRAY_RANDOM,
						  const char *cache_prefix=NULL);
    /// Create a randomly addressable attribute from an existing vertex array
    RE_Texture	       *createAddressableAttribute(RE_Render *r,
						   const char *attrib_name,
						   RE_VertexArray *data);

    /// Fetch an addressable attribute by name
    RE_VertexArray     *getAddressableAttribute(const char *attrib_name) const;

    /// Return the index of the named addressable attribute, or -1 if not found
    int			getAddressableAttributeIndex(const char *attrib_name)
									  const;

    /// Delete the addressable attribute named 'attrib_name'
    void		clearAddressableAttribute(const char *attrib_name);

    /// Return the number of addressable attributes
    int			getNumAddressableAttributes() const
			    { return myTextureAttributes.entries(); }
    
    /// Return the texture buffer object representing addressable attribute 'i'
    RE_Texture	       *getAddressableAttributeTexture(int index) const
			    { return myBufferTextures(index); }
    /// Return the vertex arrayrepresenting addressable attribute 'i'
    RE_VertexArray     *getAddressableAttributeByIndex(int index) const
			    { return myTextureAttributes(index); }

    //@}

    // ---------------------------------------------------------------------
    /// Caching @n
    
    /// Use caching for all vertex and element arrays stored by this object.
    /// If any vertex arrays are attached, they are cleared unless the name
    /// matches the previous cache name.
    //@{

    /// @brief Set the base cache name.
    /// Attribute names will be concatenated to the base name to form the full
    /// cache name. Passing NULL will disable caching. Returns true if the
    /// cachename changed, in which case all existing buffers will be purged
    /// from the cache.
    bool		cacheBuffers(const char *name);
    
    /// Return the base cache name.
    const char	       *getCacheName() const { return myCacheName; }

    /// Check if this geometry object is caching. 
    bool		isCaching() const { return myCacheName.isstring(); }

    /// Remove all buffers from the cache when this geometry object is deleted.
    void		purgeOnDelete(bool purge = true)
					{ myPurgeOnDelete = purge; }

    /// set the cache version for all arrays attached to this geometry object.
    void		setCacheVersion(RE_CacheVersion v);

    /// @brief Assign a cache tag to monitor cache changes on this object
    /// The tag will be bumped if this or any of its vertex buffers are removed
    /// from the cache.
    void		setCacheTag(RE_CacheTagHandle h);

    /// @brief Find an attribute or array in the GL cache, possibly creating it
    /// Returns the cached array, if it exists, for the array named
    /// 'attrib_name'. The name may be one of the gl-builtin attributes, like
    /// gl_Position or gl_Color. 'array_size' is the size of an INSTANCE or 
    /// RANDOM array_type.
    RE_VertexArray     *findCachedAttrib(
				RE_Render	*r,
				const char	*attrib_name,
				RE_GPUType	 data_format,
				int		 vectorsize,
				RE_ArrayType	 array_type,
				bool create_if_missing = false,
				int	random_array_size = -1,
				const RE_CacheVersion *cv=NULL,
				RE_BufferUsageHint h = RE_BUFFER_WRITE_FREQUENT,
				const char *cache_prefix = NULL,
				int capacity = -1);

    /// @brief Find an instanced attribute in the GL cache, possibly creating
    /// it. The instance_step parameter defines how often the attribute
    /// is advanced when the instance ID changes - 1 is once per instance.
    RE_VertexArray     *findCachedInstancedAttrib(
				RE_Render	*r,
				const char	*attrib_name,
				RE_GPUType	 data_format,
				int		 vectorsize,
				int		 instance_step,
				int		 array_size,
				bool create_if_missing=false,
				const RE_CacheVersion *v=NULL,
				RE_BufferUsageHint h = RE_BUFFER_WRITE_FREQUENT,
				const char *cache_prefix=NULL,
				int capacity = -1);

    
    //@}
    
    // ---------------------------------------------------------------------
    /// @name Instance groups
    /// Instance groups allow for rendering of the geometry with different
    /// attributes, generally InstanceTransform, but others can be overriden
    /// as well. The base (default) group is instance group 0. Higher groups
    /// will substitute their attributes for the ones in instance group 0. Note
    /// that the attribute need not exist in the base group to be overriden.
    /// If an attribute is not overriden, the base group attribute is used.

    /// Create a new instance group. instance_group must be a positive int.
    void	    createInstanceGroup(int instance_group);

    /// Remove an existing instance group. Will not remove any attached
    /// attribute to that group, but the instance group can no longer be
    /// rendered.
    void	    removeInstanceGroup(int instance_group);

    /// Returns true if the specified instance group exists.
    bool	    hasInstanceGroup(int instance_group) const;

    int		    getNumInstanceGroups() const
			{ return myInstanceGroups.entries(); }

    ///	For instanced drawing, create indirection list to render only some of
    /// the instances. The base instance group can also be indexed.
    void	    setInstanceGroupIndexList(RE_Render *r,
					      int instance_group,
					      bool trivial,
					      const UT_IntArray *indices=NULL,
					      int max_capacity = -1);

    /// Set to draw a single instance in the group.
    void	    setInstanceGroupConstIndex(RE_Render *r,
					       int instance_group,
					       int instance_to_draw);

    /// Set to draw all instances (based on the # transforms).
    void	    setInstanceGroupDrawEverything(RE_Render *r,
						   int instance_group);
    /// Draw none of the instances.
    void	    setInstanceGroupDrawNothing(RE_Render *r,
						int instance_group);
					       

    /// Create a constant for an attribute in 'instance_group'. 
    RE_VertexArray *createConstInstanceGroupAttrib(RE_Render *r,
						   int instance_group,
						   const char *name,
						   RE_GPUType data_format,
						   int vectorsize,
						   const void *data);

    /// Create a constant transform for an instance group.
    void	   setConstInstanceGroupTransform(int instance_group,
						  const UT_Matrix4D &xform,
						  bool remove_instanced_xform);
    void	   setConstInstanceGroupTransform(int instance_group,
						  const UT_Matrix4F &xform,
						  bool remove_instanced_xform);


    /// @brief Find an instance-group attribute in the cache
    /// Creates an instanced attribute for an instance-group, which overrides
    /// the base attribute on the RE_Geometry. instance_group zero is the base
    /// level ("Cd"), all other groups have the group appended ("Cd1" for
    /// group 1). When drawn with drawInstanceGroup(), attributes in the
    /// specified instance group will be used (if present) instead of the base
    /// attributes.
    RE_VertexArray *findCachedInstanceGroupAttrib(RE_Render	*r,
						  int instance_group,
						  const char *name,
						  RE_GPUType data_type,
						  int vector_size,
						  int instance_step,
						  int num_instances,
						  bool create=false,
						  const RE_CacheVersion *v=NULL,
						  RE_BufferUsageHint h
						    = RE_BUFFER_WRITE_FREQUENT,
						  const char *view_name = NULL,
						  int capacity = -1);

    /// Return the attribute in the instance group with type 'attrib_id', or
    /// if it's not a known type, by name.
    RE_VertexArray *getInstanceGroupAttrib(int instance_group,
					   RE_GenericAttribID attrib_id,
					   const char *name);
    /// Remove and delete the array.
    bool	    clearInstanceGroupAttrib(int instance_group,
					     RE_GenericAttribID attrib_id,
					     const char *name);

    /// This is the same as getInstanceGroupAttrib(), but it used mainly by
    /// RE_Shader to do additional prep on the attrib.
    RE_VertexArray *getInstanceGroupTextureBufferForShader(RE_Render *r,
						   RE_Shader *sh,
						   int instance_group,
						   RE_GenericAttribID attr_id,
						   const char *name);

    /// Returns the number of instances to be drawn in an instance group.
    int		    getInstanceGroupCount(int instance_group) const;

    /// Return a full instance group name based on an original base attrib name
    static void	    getInstanceGroupName(UT_WorkBuffer &inst_name,
					 const char *base_name,
					 int instance_group);
    
    /// Return a full instance group name based on the known attribute type
    static void	    getInstanceGroupName(UT_WorkBuffer &inst_name,
					 RE_GenericAttribID attrib_type,
					 int instance_group);

    // ---------------------------------------------------------------------
    /// Primitive Connectivity @n

    /// These methods allow you to set up and cache primitive connectivity.
    /// Each bit of primitive connectivity is added to a named group, like
    /// 'wireframe' or 'shaded'. A connectivity group can have multiple
    /// types of primitives or materials.  Groups are indexed, using int ids.
    /// You can pass an optional material to each set of connected prims. If
    /// the material is null, it will use the last used material (which may be
    /// the material of earlier connected primitives). Materials are not owned
    /// by this class; the element arrays are. If 'replace' is true, any
    /// previous connectivity established with the name 'connect_group'
    /// will be removed and replaced with the new connectivity.
    //@{
    
    /// Connect all primtives and  place them in indexed group 'connect_index'
    int		connectAllPrims(RE_Render *r,
				int connect_index,
				RE_PrimType prim,
				const RE_MaterialPtr &mat = NULL,
				bool replace = false,
				int vertices_per_patch = 0);

    /// Connect a subrange of vertices and place in index group 'connect_index'
    int		connectSomePrims(RE_Render *r,
				 int connect_group,
				 RE_PrimType prim,
				 int start,
				 int length,
				 unsigned int stride = 0,
				 const RE_MaterialPtr &mat = NULL,
				 bool replace = false,
				 int vertices_per_patch = 0);

    /// @brief Connect vertices using an indexed vertex list, add to 'connect_group'.
    /// 'num' is the size of the list, not the number of primitives. If
    /// any index is greater than the number of points in the geometry,
    /// the results will be undefined. This is added to connect_group.
    int		connectIndexedPrims(RE_Render *r,
				     int connect_group,
				     RE_PrimType prim,
				     int num,
				     const unsigned int *prims,
				     const RE_MaterialPtr &mat = NULL,
				     bool replace = false,
				     int vertices_per_patch = 0);

    /// Connect vertices using the indices in the buffer object 'elements'
    int		connectIndexedPrims(RE_Render *r,
				     int connect_group,
				     RE_PrimType prim,
				     RE_VertexArray *elements,
				     const RE_MaterialPtr &mat = NULL,
				     bool replace = false,
				     int vertices_per_patch = 0);
    
    ///  Connect vertices using the indices in the element array 'elements'
    int		connectIndexedPrims(RE_Render *r,
				     int connect_group,
				     RE_ElementArray *elements,
				     const RE_MaterialPtr &mat = NULL,
				     bool replace = false);
    

    /// Returns the number of discrete connectivities in 'connect_group'
    int		    getConnectNumElementArrays(int connect_group); 
    /// Returns the element array associated with connect_group, if any.
    RE_ElementArray *getConnectElementArray(int connect_group,
					    int index = 0);

    /// Returns true if the connectivity index 'connect_group' exists.
    bool	hasConnectGroup(int connect_group) const;

    bool	hasNonEmptyConnectGroup(int connect_group) const;

    /// Returns the largest index of all the indexed connect groups. Some may
    /// be NULL.
    int		getMaxConnectGroup() const;

    /// Remove and delete the connection group specified, return false only if
    /// the group doesn't exist.
    bool	removeConnectedPrims(int connect_group);
    
    /// Removes and deletes all connectivity groups.
    void	resetConnectedPrims();

    //@}

    /// Assign a new material to connectivity group 'connect_index'. The
    /// connectivity group must exist or it will do nothing but return false.
    bool	    assignMaterialToConnectivty(RE_Render *r,
						int connect_index,
						const RE_MaterialPtr &mat);

    /// Returns the material attached to the given connectivity group, if any.
    /// If multiple connectivities were assigned to a group with different
    /// materials, the subindex is used to select which connectivity is queried.
    RE_MaterialPtr  getConnectivityMaterial(int connect_index,
					    int subindex = 0);

    /// Methods to control which attributes are used when drawing a
    /// connect_group. By default, all are enabled. When disabling an array or
    /// attribute, it will return zero to the vertex shader. 
    //@{

    /// @brief Toggles the use of a generic vertex attribute for a connect
    /// group. Index group version of useAttribute(); toggles the attribute
    /// named 'name' on or off.
    void	useAttribute(int connect_group,
			     const char *name, bool enable);
    /// @brief Toggles the use of a generic vertex attribute for a connect
    /// group. Index group version of useAttribute(); toggles the attribute
    /// specified by 'attrib' on or off.
    void	useAttribute(int connect_group,
			     RE_VertexArray *attrib, bool enable);
    
    // ---------------------------------------------------------------------

    /// Enables or disables textures on materials when drawn
    void	useMaterialTextures(bool enable = true);

    /// Clamp the number of layers to draw with a multi-layer material
    void	setNumMaterialLayers(int num);

    //@}
    
    // ---------------------------------------------------------------------

    /// @name Drawing
    /// These methods draw connectivity groups that were previously set up,
    /// with some modifications available.
    /// If 'primtype' is not INVALID, it overrides the connectivity group's
    /// primitive type. 'attrib_overrides' is a list of string pairs which
    /// defines a mapping from shader attribute names (indices 0,2,4,6...)
    /// to geometry attribute names (indices 1,3,5,7...).
    //@{
    
    /// Draw an indexed connectivity group.
    /// Draws the primitives specified by the cached connectibity info.
    void	draw(RE_Render  *r,
		     int connect_idx,
		     RE_PrimType prim_type = RE_PRIM_AS_IS,
		     RE_OverrideList *attrib_overrides = NULL)
		    {
			drawPrivate(r, getConnect(connect_idx, false),0,
				    prim_type, attrib_overrides, 0);
		    }

    /// @brief Draw all connectivity groups.
    /// Draws all connect_groups. Same as looping through all groups and calling
    /// draw(). The draw order is undefined; if you need a specific order, use
    /// multiple draw calls.
    void	drawAll(RE_Render *r,
			RE_PrimType prim_type = RE_PRIM_AS_IS,
			RE_OverrideList *attrib_overrides = NULL);

    /// Draws all indexed connect groups in the range
    /// if 'material_offset' is non-null, it acts as a offset when setting the
    /// MATERIAL_GROUP uniform, which is added to (index-connect_group_start)
    /// when drawing indexed connect groups. If num_connected_groups is
    /// negative, all valid groups >= 'connect_group_start' are drawn,
    /// otherwise it only draws up to `start+num-1`.
    void	drawRange(RE_Render *r,
			  int connect_group_start, int num_connect_groups,
			  RE_PrimType ptype = RE_PRIM_AS_IS,
			  RE_OverrideList *attrib_overrides = NULL,
			  const int *material_offset = NULL);

    /// @brief Draw with Instancing
    /// Draws the same connect group 'num_instances' times. It is up to the
    /// shader to differentiate the instances, using an instanced attribute
    /// (createInstancedAttribute()) or GLSL's gl_InstanceID. 
    void	drawInstanced(RE_Render *r,
			      int connect_idx,
			      int num_instances, 
			      RE_PrimType prim_type = RE_PRIM_AS_IS,
			      RE_OverrideList *attrib_overrides=NULL);

    /// Draw all connectivity groups using instancing, num_instance times.
    void	drawAllInstanced(RE_Render *r,
				 int num_instances, 
				 RE_PrimType prim_type = RE_PRIM_AS_IS,
				 RE_OverrideList *attrib_overrides=0);
    
    /// Draw a range of connectivity index groups with instancing
    void	drawRangeInstanced(RE_Render *r,
				   int connect_start, int num_connect,
				   int num_instances, 
				   RE_PrimType prim_type = RE_PRIM_AS_IS,
				   RE_OverrideList *attrib_overrides=0,
				   const int *material_offset = NULL);

    /// Draw an instance group using a given connectivity
    void	drawInstanceGroup(RE_Render *r,
				  int connect_idx,
				  int instance_group,
				  RE_PrimType prim_type = RE_PRIM_AS_IS,
				  RE_OverrideList *attrib_over=NULL);
				  
    /// Draw an instance group using a given connectivity
    void	drawInstanceGroupRange(RE_Render *r,
				  int connect_start,
				  int num_connect,
				  int instance_group,
				  RE_PrimType prim_type = RE_PRIM_AS_IS,
				  RE_OverrideList *attrib_over=NULL,
				  const int *material_offset = NULL);
				  
    //@}

    /// Manually enable and disable arrays. This is useful for determining
    /// which ones are actually enabled via r->dumpNewState().
    //@{
    void	 enableArrays(RE_Render *r, int connect_group,
			      unsigned int stride = 0);
    void	 disableArrays(RE_Render *r, int connect_group);
    //@}

    /// @private Used by RE_Shader::prepShader.
    void	 bindToVertexState(RE_Render *r,
				   RE_Shader *sh,
				   unsigned int stride,
				   RE_VertexState *vstate,
				   int instance_group);

    /// resets all VAOs so that on the next draw all VBOs are rebound
    void	resetVertexState(RE_Render *r);

    /// Remove all arrays and attributes from their GL bindings.
    void        unbindAllArrays(RE_Render *r);

    // --------------------------------------------------------------------
    // For debug:

    // dumps the current configuration of the geometry object (out == NULL will
    // use std::cerr).
    void	print(std::ostream *out = NULL) const;

    // For the next draw only, dump uniforms, builtin uniforms, and/or GL state.
    void	setDebugDraw(bool dump_uniforms,
			     bool dump_builtins,
			     bool dump_gl_state);
    
private:

    RE_VertexArray  *getBaseArray(RE_OverrideList *attrib_ovrrides = 0) const;

    re_Connectivity *getConnect(int group_idx,
				bool create_if_none);

    // Clears all the attached buffers. If this object is cached, all cached
    // vertex arrays/buffers will remain in the cache.
    void	clearBuffers(bool connectivity_too = true);

    void	 bindBuffer(RE_Render *r,
			    RE_VertexArray *array,
			    unsigned int stride,
			    RE_Shader *sh,
			    const char *attrib_name);

    void	 unbindBuffer(RE_Render *r,
			      RE_VertexArray *array,
			      RE_Shader *sh,
			      const char *attrib_name);

    RE_VertexArray *getCachedVertexBuffer(RE_Render    *r,
					  RE_BufferType type,
					  int		tex_level,
					  const char   *attrib_name,
					  RE_ArrayType	array_type,
					  RE_GPUType	data_format,
					  int		vectorsize,
					  int		length,
					  RE_VertexArray *target_array,
					  const char *cache_prefix);

    void	freeArray(RE_VertexArray *&a, bool mark_unused = true);
    void	purgeArray(RE_VertexArray *&a, bool mark_unused = true);

    void	assignAttribute(RE_VertexArray *a, int index);
    void	updateKnownAttrib(RE_VertexArray *a, bool set_known);
    bool	privAttachAttribute(RE_VertexArray *attrib, bool show_errors);
    
    static bool  arrayDeleted(RE_VertexArray *a, void *data);
    
    RE_VertexArray *	createNewAttribute(RE_Render  *r,
					   const char *attrib_name,
					   RE_GPUType  data_format,
					   int	       vectorsize,
					   int	       instance_stride,
					   RE_ArrayType	atype = RE_ARRAY_POINT,
					   int array_size = 0,
					   RE_BufferUsageHint usage =
					              RE_BUFFER_WRITE_FREQUENT,
					   const char *cache_prefix = NULL,
					   bool create_const_attrib = false,
					   int array_capacity = -1);

    RE_VertexArray     *fetchCachedAttrib(RE_Render	*r,
					  const char	*attrib_name,
					  RE_GPUType	 data_format,
					  int		 vectorsize,
					  RE_ArrayType	 array_type,
					  int		 inst_step,
					  bool create_if_missing,
					  int		 array_size,
					  const RE_CacheVersion *cv,
					  RE_BufferUsageHint usage,
					  const char *cache_prefix=NULL,
					  int capacity = -1);
    void		drawPrivate(RE_Render  *r,
				    re_Connectivity *connect,
				    int num_instances,
				    RE_PrimType prim_type,
				    RE_OverrideList *override_attrib,
				    int instance_group);

    RE_OGLTexture       *prepTexBufferArray(RE_Render *r,
					    RE_VertexArray *array,
					    RE_GPUType buftype,
					    int &pmode);
    void		 privEnableArrays(
				RE_Render *r,
				re_Connectivity *connect,
				unsigned int stride = 0,
				RE_OverrideList *attrib_overrides = NULL);
    void		 privDisableArrays(
				RE_Render *r,
				re_Connectivity *connect,
				RE_OverrideList *attrib_overrides = NULL);
    void		 privUseAttrib(
				re_Connectivity *connect,
				const char *attrib_name,
				bool enable);

    void		 addToInstanceGroup(RE_VertexArray *attrib,
					    int group = -1);
    void		 removeFromInstanceGroup(RE_VertexArray *attrib);
    
// DATA:
    int			 myNumPoints;
    int			 myNumVertices;
    int			 myNumPrimitives;
    RE_VertexMap	*myVertexMap;
    mutable int		 myAttribPIndex;
    bool		 myUseVertexState;
    bool		 myPurgeOnDelete;
    int			 myVertexStateSerial;
    UT_String		 myCacheName;
    bool		 myUseTextures;
    int			 myNumMaterialLayers;

    UT_ValArray<RE_VertexArray *>    myAttributes;
    
    UT_ValArray<RE_VertexArray *>    myTextureAttributes;
    UT_ValArray<RE_Texture *>	     myBufferTextures;

    UT_ValArray<RE_VertexArray *>    myKnownAttributes;
    UT_ValArray<RE_VertexArray *>    myStashedAttributes;

    UT_ValArray<re_Connectivity *>   myConnectGroups;
    UT_Array<re_InstanceGroup *>     myInstanceGroups;

    RE_CacheTagHandle		     myCacheTagHandle;

    int			 myDebugDrawFlags;

    friend class re_InstanceGroup;
};

inline void
RE_Geometry::useMaterialTextures(bool enable)
{
    myUseTextures = enable;
}

inline void
RE_Geometry::setNumMaterialLayers(int num)
{
    myNumMaterialLayers = SYSclamp(num, 1, 32);
}

inline RE_VertexArray *
RE_Geometry::findCachedAttrib(RE_Render	   *r,
			      const char   *attrib_name,
			      RE_GPUType    data_format,
			      int	    vectorsize,
			      RE_ArrayType   array_type,
			      bool	    create_if_missing,
			      int	    array_size,
			      const RE_CacheVersion *cache_version,
			      RE_BufferUsageHint usage,
			      const char *cache_prefix,
			      int capacity)
{
    return fetchCachedAttrib(r, attrib_name, data_format, vectorsize,
			     array_type, 0, create_if_missing,
			     array_size, cache_version, usage,
			     cache_prefix, capacity);
}

inline RE_VertexArray *
RE_Geometry::findCachedInstancedAttrib(RE_Render    *r,
				       const char   *attrib_name,
				       RE_GPUType    data_format,
				       int	     vectorsize,
				       int	     inst_step,
				       int	     array_size,
				       bool	     create_if_missing,
				       const RE_CacheVersion *cache_version,
				       RE_BufferUsageHint usage,
				       const char *cache_prefix,
				       int	     capacity)
{
    return fetchCachedAttrib(r, attrib_name, data_format, vectorsize,
			     RE_ARRAY_INSTANCE, inst_step,
			     create_if_missing, array_size,
			     cache_version, usage, cache_prefix, capacity);
}

inline RE_VertexArray *
RE_Geometry::getAttribute(RE_GenericAttribID id) const
{
    return (id > RE_GENATTRIB_NONE) ? myKnownAttributes(id) : NULL;
}

#endif
 
