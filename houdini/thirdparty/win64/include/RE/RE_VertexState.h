/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_VertexState.h ( RE Library, C++)
 *
 * COMMENTS:
 *	Encapsulates all state required for vertex processing by a vertex
 *	shader. It uses a VAO to include all vertex array state, but
 *	additionally captures constant vertex attribs.
 *
 *	This only works if RE_EXT_VERTEX_ARRAY_OBJECT is supported.
 */
#ifndef RE_VertexState_h
#define RE_VertexState_h

class RE_VertexArray;
class RE_Render;
class RE_VertexMap;
class RE_OGLTexture;

#include <iosfwd>
#include <UT/UT_Pair.h>
#include <UT/UT_String.h>
#include <UT/UT_SmallArray.h>
#include <UT/UT_StringArray.h>
#include "RE_OGLVertexArrayList.h"
#include "RE_ContextSpecificTable.h"

class re_TexBufData
{
public:
    re_TexBufData() : tex_id(0), mode(0) {}
    int		   tex_id;
    int		   mode;
    UT_String	   mode_uniform;
    UT_String	   samp_uniform;
};


class RE_API RE_VertexMap : public RE_ContextSpecificObject
{
public:
		RE_VertexMap(int num_attribs = 0);
	       ~RE_VertexMap();

   void		setMapSize(int nv);

   virtual bool	init(RE_Render *r);

   // This is used by the RE_VertexState to set its attributes
   int		setAttrib(RE_VertexArray *attrib, RE_GPUType shader_type,
			  int attribute_location);

   // This is normally called by shaders to setup their vertex map
   int		setAttrib(const char *attrib_name, RE_GPUType t, int vsize,
			  int attribute_location, RE_GenericAttribID id,
			  int64 *maskptr = NULL);

   // 
   int64	setConstAttrib(const char *attrib_name, RE_GPUType t, int vsize,
			       int attribute_location, RE_GenericAttribID id);

   // if unbind_attr is true and an attribute with "attrib_name" is present, it
   // will be removed from the map. Otherwise, it will be retained.
   int		setTexBuffer(const char *attrib_name, bool unbind_attr);

   // Remove a vertex attribute or constant from a vertex shader input
   void		removeAttrib(const char *attrib_name);

   // Remove a texture buffer attribute.
   void		removeTexBuffer(const char *attrib_name);

   // Clear a vertex attribute or constant at an attribute location
   void		clearAttrib(int attribute_location);

   // Clear the entire map - buffers, attributes, constants.
   void		clear();
    
   bool		compatibleWith(const RE_VertexMap &shader_map,
			       const RE_VertexMap *geo_map) const;

   // Vertex attribute existence mask, indexed by (1<<RE_GenericAttribID) 
   int64	getAttributeMask() const { return myBoundAttributes; }
   // Constant attribute existence mask, indexed by (1<<RE_GenericAttribID) 
   int64	getConstantMask() const { return myBoundConstants; }
   // Texture attribute existence mask, indexed by (1<<RE_GenericAttribID) 
   int64	getTextureMask() const  { return myBoundTexAttribs; }

   // Attribute location filled, indexed by (1<<attribute_location)
   int64	getLocationMask() const { return myBoundLocations; }

   // Return true if the custom attribute 'attrib_name' is present
   bool		hasCustomAttrib(const char *attrib_name) const;
   
   void		print(RE_Render *r, std::ostream &os) const;

    /// Returns the amount of main memory owned by this RE_VertexMap.
    /// RE_VertexMap does not have an associated graphics memory.
    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myCustomAttribs.getMemoryUsage(false);
        mem += myAttribLocs.getMemoryUsage(false);
        mem += myTexAttribNames.getMemoryUsage(false);
        mem += myAttribIDs.getMemoryUsage(false);
        return mem;
    }

private:
   int		   myMapSize;
   UT_SmallArray<RE_GenericAttribID,sizeof(int)*32>
		   myAttribIDs;
   UT_SmallArray<int,sizeof(int)*RE_GENATTRIB_NUM_IDS>
		   myAttribLocs;
   UT_StringArray  myTexAttribNames;
   int64	   myBoundLocations;
   int64	   myBoundAttributes;
   int64	   myBoundConstants;
   int64	   myBoundTexAttribs;
   int		   myUndefAttribIndex;

   class re_CustomAttrib
   {
   public:
       re_CustomAttrib() : location(-1) {}
       re_CustomAttrib(const char *name, int loc)
	   : attrib_name(name,1), location(loc) {}
       
       UT_String attrib_name;
       int	 location;
   };

   UT_Array<re_CustomAttrib> myCustomAttribs;
};


class RE_API RE_VertexState
{
public:
		RE_VertexState();
	       ~RE_VertexState();

    // Initialization only fails if VAOs are not supported.
    bool	init(RE_Render *r);

    void	reset();

    // Bind for update only binds the VAO. 
    bool	bindForUpdate(RE_Render *r);

    // Vertex stride, usually 0 (tightly packed)
    void	setVertexStride(int stride) { myVertexStride = stride; }
    
    // Add an array during update to the vertex state.
    bool	assignArray(RE_Render	   *r,
			    RE_VertexArray *attrib,
			    int		    attrib_location,
			    RE_GPUType	    shader_type);

    // Assign a zeroed, but present, attribute to the generic vertex index.
    bool	assignZero(RE_Render *r, const char *attrib_name,
			   RE_GPUType t, int vector_size,
			   int attrib_location);

    bool	assignTexBuffer(RE_Render *r,
				const char *name,
				RE_OGLTexture *t,
				int mode);

    // Remove an array from the vertex state.
    bool	removeArray(RE_Render *r,
			    int	       attrib_location);

    // Unbinds an array from the VAO.
    void	unbindElementArray(RE_Render *r);

    // Bind for draw binds both the VAO and the constants. It is an error to
    // bind the vertex state for drawing if it is not initialized yet.
    bool	bindForDrawing(RE_Render *r, RE_Shader *sh);

    // Sets the current VAO back to 0 (none bound).
    // Does not restore the values of constant attributes.
    bool	unbind(RE_Render *r);

    // Marks the vertex state as dirty if the passed serial index doesn't match
    // the stored serial index.
    bool	checkDirty(RE_Render *r, int serial_index);

    // Resets the vertex state so that buffers will be rebound.
    void	reset(RE_Render *r);

    // Returns true if this vertex state is compatible with the given map.
    // It must be the same or a superset of the map (attribute names matching
    // and in the correct attribute locations).
    bool	compatibleWith(RE_Render *r,
			       const RE_VertexMap &shader_map,
			       const RE_VertexMap *geo_map,
			       unsigned int stride);

    void	print(RE_Render *r, std::ostream &os);

    bool	hasBoundTBO(int tex_unit) const;

    // Generic Attribute ID functions
    static void assignDefaultMap(RE_Render *r, RE_Shader *shader);
    
    static RE_GenericAttribID getGenericAttributeID(const char *name,
						    bool *in_instance_group =
						            NULL);
    static const char	     *getGenericAttributeName(RE_GenericAttribID id);
    static const char	    *getGenericAttributeModeName(RE_GenericAttribID id);
    static bool		      getGenericAttributeUniforms(RE_GenericAttribID id,
							  UT_String &mode,
							  UT_String &sampler);
    static RE_ShaderAttribLocation
			   getGenericAttributeLocation(RE_GenericAttribID id);
    static bool	      getConstInstanceAttributeSupport(RE_GenericAttribID id);

    static bool texBufferDeleted(void *, void *);
    
    static void dumpGLVertexState(RE_Render *r);

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_VertexState.
    int64 getMemoryUsage(bool inclusive) const;

private:
    RE_SharedVertexArray	 myVAO;
    fpreal64			*myConstAttribValue;
    RE_GPUType			*myConstAttribType;
    unsigned char		*myConstAttribSize;
    RE_ContextSpecificTable<RE_VertexMap> myVertexMap;
    re_TexBufData		*myTexBufferData;
    UT_ValArray<re_TexBufData*>  myUndefTexBufferData;
    int				 myNumVertexAttribs;
    int				 myVertexStride;
    int				 myBoundState;
    int				 myDirtySerial;
    UT_ValArray< UT_Pair<const RE_OGLTexture *, int> > mySavedTex;
    UT_IntArray			 myHasBoundTBO;
};

inline bool
RE_VertexState::checkDirty(RE_Render *r, int serial_index)
{
    if(myDirtySerial != serial_index)
    {
	myDirtySerial = serial_index;
	if(myBoundState >= 0)
	    myVertexMap.clear();

	if(r->getGraphicsDevice() & RE_DEVICE_ATI)
	    myVAO.free(r);
	
	return true;
    }
    return false;
}

inline void
RE_VertexState::reset(RE_Render *r)
{
    checkDirty(r, myDirtySerial-1);
}

inline bool
RE_VertexState::hasBoundTBO(int tex_unit) const
{
    return (tex_unit < myHasBoundTBO.entries() && myHasBoundTBO(tex_unit));
}
#endif


