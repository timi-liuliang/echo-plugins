/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_ScriptMaterialInfo.h ( VOP Library, C++)
 *
 * COMMENTS:	This class represents information needed for the
 *		script-based vopnet materials (compiled multi-context vopnet
 *		material HDAs to match the behaviour of node-based vopnet 
 *		materials from which they were created (eg, vopmaterial SHOP).
 *		This is usually information about the rendering properties
 *		and procedural vops within the vopnet material.
 *	
 */

#ifndef __VOP_ScriptMaterialInfo__
#define __VOP_ScriptMaterialInfo__

#include "VOP_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_StringMap.h>
#include "VOP_Types.h"

class UT_StringArray;
class UT_StringHolder;
class UT_StringRef;
class PRM_Parm;
class PRM_ParmList;
class OP_Node;
class OP_Network; 
class OP_PropertyLookupList;
class VOP_Node;
class VOP_PhantomNode;
class VOP_NodeChoice;
class UT_String;
class UT_IStream;
class VOP_ScriptMaterialCodeMapper;
class VOP_FunctionArgInfo;
typedef UT_ValArray<VOP_Node *>		VOP_NodeList;
typedef UT_ValArray<VOP_PhantomNode*>	VOP_PhantomNodeList;

// ===========================================================================
// Holds info about shader layer exports.
class VOP_API VOP_ShaderLayerExports
{
public:
    void		    setInputsToCopy( const UT_IntArray &inputs )
			    { myInputsToCopy = inputs; }
    const UT_IntArray &	    getInputsToCopy() const
			    { return myInputsToCopy; }

    void		    setExportsToAdd( const UT_StringHolder &add )
			    { myExportsToAdd = add; }
    const UT_StringHolder & getExportsToAdd() const
			    { return myExportsToAdd; }

private:
    UT_IntArray		    myInputsToCopy;
    UT_StringHolder	    myExportsToAdd;
};


// ===========================================================================
class VOP_API VOP_ScriptMaterialInfo
{
public:
    /// Constructor. Takes an 'owner' as an argument, to make it possible
    /// for phantom nodes to report their full path and shader code section
    /// to be picked based on parameter value of the switch indices (top level
    /// parameters on owner).
	     VOP_ScriptMaterialInfo( OP_Network * owner );
    virtual ~VOP_ScriptMaterialInfo();

    /// Static function for encoding the relevant child nodes of the material 
    /// vopnet into a serialized stream, parsable by the instantiation 
    /// methods of this class.
    /// Returns true if there was some relevant info to encode and 
    /// there were no errors while encoding that info.
    /// @param	buffer	    Outgoing argument that will contain the encoded 
    ///			    info in an XML form.
    /// @param	material    Vopnet material node whose metadata needs to be 
    ///			    saved. The metadata is in the form of child nodes
    ///			    that don't participate in the vex shader generation
    ///			    (such as properties nodes and procedurals). However
    ///			    information about such nodes is necessary for
    ///			    proper functioning of the compiled material HDAs.
    /// @param	mapper	    Maps the operator paths of encapsulated shaders
    ///			    found in the material node into the HDA section
    ///			    names that contain vex code for these shader nodes.
    static bool		encodeInfo( UT_String & buffer, OP_Node & material,
				    VOP_ScriptMaterialCodeMapper * mapper );
	

    /// Obtains the names of the HDA code sections that are used by this info.
    static void		getReferencedSectionNames( UT_IStream & is,
						   UT_StringArray & names );


    /// This method parses and loads information from the stream (created
    /// with encodeInfo()). It initializes the internal data structures
    /// so that it can properly handle all the other requests and queries from 
    /// other methods. 
    ///
    /// It creates phantom nodes within an owner network.
    /// Phantom nodes represent render properties, procedural shaders,
    /// and encapsulated shaders that were part of the original material node,
    /// but which cannot be part of the HDA since it is a compiled code (ie,
    /// childless) node type. Houdini still considers myParentNode as a
    /// childless node so users cannot dive it and see them, but the
    /// phantom nodes are still present. Namely, when knowing their exact
    /// node paths and searching for them with findNode(). So,
    /// the 'parent' argument is necessary so that phantom nodes can report 
    /// a valid full path.
    /// 
    /// It also parses the information about the code sections that are present
    /// in the HDA. This info is used to pick a correct shader code from a
    /// correct section, in case there are a few versions of the shader, each
    /// corresponding to an Output VOP node, which are picked by Switch VOPs
    /// connected as inputs to the Collect VOP.
    bool		loadInfo( UT_IStream & is );

    /// Returns true if the instance of this class contains relevant information
    /// for the given shader type and can answer the queries for this type.
    bool		providesVopType( VOP_Type shader_type );

    /// Returns a name of the HDA section that has the vex code that should
    /// be used for the shader of a given type. HDA materials may have several
    /// code sections for the same shader type if the original node had several
    /// Output VOP nodes connected to the Collect VOP node via Switch VOPs.
    /// This method mimics the switch vop hierarchy to make a decision as to
    /// which shader code to use.
    bool		getCodeSectionName( UT_String & section_name,
					    VOP_Type shader_type );

    /// Returns all known export parameters of a shader. 
    void		getExportParmInfos( 
				UT_Array<VOP_FunctionArgInfo> &arg_infos, 
				VOP_Type shader_type ) const;

    /// @{ Returns info needed for constructing output variable tags for
    /// describing shader layer exports.
    UT_IntArray		getShaderLayerExportsInputsToCopy(const char *out_name,
				    VOP_Type shader_type) const;
    UT_StringHolder	getShaderLayerExportsToAdd(const char *out_name,
				    VOP_Type shader_type) const;
    /// @}

    /// Track down the parameter associated with a given property.
    virtual bool	getParameterOrProperty(
				    const UT_StringRef &name, fpreal now,
				    OP_Node *&op, PRM_Parm *&parm,
				    bool create_missing_multiparms,
				    PRM_ParmList *obsolete );

    /// Track down the parameter associated with a given a channel name.
    virtual bool	getParameterOrPropertyByChannel(
				    const UT_StringRef &chname,
				    fpreal now, OP_Node *&op, PRM_Parm *&parm,
				    int &vector_index, PRM_ParmList *obsolete );

    /// Track down the parameters or properties in the given list.
    /// Fill out the property lookup list with parameters. 
    /// Returns the total of found and resolved parameters in the list (whether
    /// they were resolved by previous calls or this particular call).
    virtual int		findParametersOrProperties(fpreal now,
				    OP_PropertyLookupList &list);


    /// Returns a procedural shader node of a given type if it has one,
    /// otherwise returns NULL.
    virtual VOP_Node *	getProcedural(VOP_Type type);

    /// Returns the phantom node of a given name or NULL if no such child found.
    virtual OP_Node *	getChild(const char *name, int *hint = NULL) const;

    /// Map: shader type -> list of exported parameters in that shader.
    using ExportsMap = UT_Map< VOP_Type, UT_Array<VOP_FunctionArgInfo>>;

    /// Map: outupt name, shader type -> shader layer exports
    using LayerExportsMap = UT_StringMap< 
	UT_Map< VOP_Type, VOP_ShaderLayerExports>>;

private:
    /// Gets the string data from the choice leaf node that is considered
    /// to be currently selected based on the decision tree.
    const char *	getChoiceLeafData(VOP_Type type, const char *key) const;

    /// Uses switch info tree to access the node of a given type. If info
    /// does not exist, looks for the first node of a given type in the 
    /// internal list.
    VOP_PhantomNode *	getNodeOfType( VOP_Type type ) const;

    /// Uses switch info tree to access the nodes of a given type. If info
    /// does not exist, gathers all the nodes of a given type in the internal
    /// list.
    void		getNodesOfType( VOP_PhantomNodeList & nodes, 
					VOP_Type type ) const;

private:
    /// The owner network of this material info.
    OP_Network *	    myOwner;

    /// Phantom nodes that provide info about rendering properties values
    /// and the procedrual shaders.
    VOP_PhantomNodeList	    myNodes;

    /// The tree containing information about output nodes (mapped to HDA
    /// code section names), properties phantom nodes, and procedural shader 
    /// phantom nodes.
    /// This decision tree chooses the correct code section or node to use.
    ///
    /// Each code section corresponds to exactly one Output VOP that was used
    /// to generate the shader code. However, in materials, there may be a
    /// series of Switch and Null VOPs chained together from the Collect VOP
    /// to the Output VOPs. Thus, there may be a few code sections for a given
    /// type of a shader (eg, surface), and this tree helps to make a decision
    /// as to which section should be used to provide the shader code. The root
    /// in this tree represents the Collect VOP, the branches are Switch VOPs,
    /// and the leaves are the Output VOPs. The branch nodes contain info
    /// needed to choose a branch (swich input). The leaves contain the code
    /// section name. Whole tree is used to choose the shader code section.
    VOP_NodeChoice *	myNodeChoiceTree;

    /// List of export parameters for each shader type.
    ExportsMap		myExportParametersMap;

    /// Shader Layer exports info for each output.
    LayerExportsMap	myLayerExportsMap;
};


/// Mapper that provides an HDA section name for a given shader node. 
//
/// The shader may be a material component shader or an encapsulated shader.
/// The material component shader can be invoked by arbitrary caller (eg,
/// a renderer or another shader).
//
/// The encapsulated shaders are referenced by procedural geometry VOPs
/// inside the material, and are not really available to the outside world
/// for direct calls. They can be indirectly manipulated thru parameters
/// promoted to the parent material. 
class VOP_API VOP_ScriptMaterialCodeMapper
{
public:
    // Standard virtul destructor.
    virtual	~VOP_ScriptMaterialCodeMapper() {}

    /// Maps the shader node to an HDA section name that contains the
    /// code generated by that shader node. The shader node should generate
    /// own code, because mapper may try to compile and store it in the section.
    /// Returns, previously added mapped section name if already added,
    /// or adds and returns a newly constructed shection name.
    /// @param	section_name	The outgoing argument that will contain
    ///				the section name that contains the shader's 
    ///				vex code.
    /// @param	shader_node	The shader node whose code is in the mapped
    ///				section name.
    /// @param	shader_type	The type of shader code to look for (one node
    ///				may be able to provide a few vex code chunks,
    ///				each for a different shader type).
    /// @param	is_encapsulated If true, the shader is mareked as encapsulated,
    ///	    which influences how the shader node is treated with regards
    //	    to generating the VEX code. Ie, it is is generated to suit the 
    ///	    purpose of calling it from the geometry procedurals (without
    ///	    import directoves), rather than as an explicit VEX shader call 
    ///	    from another VEX shader (which require import directives, etc).
    virtual void	mapToVexCodeSection( UT_String & section_name, 
				VOP_Node * shader_node,
				VOP_Type shader_type, 
				bool is_encapsulated) = 0;

    /// Checks if the shader node was added as part of the primary output
    /// compilation phase. These nodes don't have phantom nodes, but rather
    /// reference a saved code section directly.
    virtual bool	isPrimaryOutput( VOP_Node *shader_node, 
				VOP_Type shader_type ) const = 0;
};


#endif

