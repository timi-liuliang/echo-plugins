/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP Library (C++)
 *
 * COMMENTS:	The base class for all OP operators
 *
 */

#ifndef __OP_Node_h__
#define __OP_Node_h__

#include "OP_API.h"
#include "OP_BundleReferences.h"
#include "OP_Cache.h"
#include "OP_ConnectorId.h"
#include "OP_Context.h"
#include "OP_DataMicroNode.h"
#include "OP_DataTypes.h"
#include "OP_Dependency.h"
#include "OP_Error.h"
#include "OP_EventMicroNode.h"
#include "OP_InterestRef.h"
#include "OP_ItemId.h"
#include "OP_Lock.h"
#include "OP_NetworkBoxItem.h"
#include "OP_NodeFlags.h"
#include "OP_OpTypeId.h"
#include "OP_OTLLicenseType.h"
#include "OP_Parameters.h"
#include "OP_Value.h"
#include "OP_Version.h"

#include <CH/CH_EventManager.h>
#include <CH/CH_Types.h>
#include <EXPR/EXPR_Lock.h>
#include <DEP/DEP_MicroNode.h>
#include <PY/PY_Result.h>
#include <UT/UT_Algorithm.h>
#include <UT/UT_Array.h>
#include <UT/UT_ArrayMap.h>
#include <UT/UT_Assert.h>
#include <UT/UT_BitArray.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_Color.h>
#include <UT/UT_Error.h>
#include <UT/UT_LockedRawPtr.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_Options.h>
#include <UT/UT_PathSearch.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_SmallArray.h>
#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_StringView.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_TaskState.h>
#include <UT/UT_TokenString.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_XformOrder.h>
#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_Deprecated.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>

#include <iosfwd>
#include <limits.h>
#include <string.h>

class UT_InfoTree;
class UT_IStream;
class UT_Ramp;
class UT_StringArray;
class UT_Undo;
class UT_WorkArgs;
class UT_WorkBuffer;
class IMG_Raster;
class PY_CompiledCode;
class PY_EvaluationContext;
class PY_OpaqueObject;
class DD_Source;
class DD_ChoiceList;
class CL_Clip;
class CL_Track;
class CMD_Args;
class CH_Channel;
class CH_ChannelRef;
class CH_LocalVariable;
class PRM_BatchList;
class PRM_Name;
class PRM_Parm;
class PRM_ParmList;
class PRM_ParmMicroNode;
class PRM_RefId;
class PRM_Template;
class OP_Bundle;
class OP_DopParent;
class OP_Dot;
class OP_EditorContext;
class OP_GalleryEntry;
class OP_GlobContext;
class OP_Group;
class OP_IndirectInput;
class OP_Input;
class OP_Network;
class OP_NetworkBox;
class OP_Node;
class OP_NodeInfoParms;
class OP_NodeInfoTreeParms;
class OP_Operator;
class OP_Output;
class OP_OutputCodeParms;
class OP_PostIt;
class OP_PreDefRules;
class OP_PropagateData;
class OP_SaveFlags;
class OP_SubnetIndirectInput;
class DOP_Parent;
class VOP_CodeGenerator;
class VOP_ExportedParmsManager;
class VOP_ScriptMaterialCodeMapper;

// avoid lengthy compilation dependency
typedef UT_ValArray<const CL_Track *>   CL_TrackListC;	

typedef int  (*OP_EditCallback)(void *data, OP_Node *src,
		CL_Track *track, fpreal t, fpreal value);

typedef UT_ArrayMap<OP_ConnectorId, OP_Input*>	OP_ConnectorInputMap;
typedef UT_ArrayMap<OP_ConnectorId, OP_Output*>	OP_ConnectorOutputMap;
typedef OP_ConnectorInputMap::iterator		OP_ConnectorInputIter;

struct OP_InputPair
{
    OP_Input *myInput;
    OP_ConnectorId myName;
};

struct OP_OutputPair
{
    OP_Output *myOutput;
    OP_ConnectorId myName;
};

struct OP_NodeParmRef
{
    OP_NodeParmRef(const OP_Node *eval_node = 0,
		   const PRM_Parm *parm_ref = 0,
		   int vi = -1)
	: myEvalNode(eval_node)
	, myParmRef(parm_ref)
	, myVectorIndex(vi)
    {
    }

    bool operator==(const OP_NodeParmRef &other)
    {
	return (myEvalNode == other.myEvalNode
		&& myParmRef == other.myParmRef
		&& myVectorIndex == other.myVectorIndex);
    }

    const OP_Node *	myEvalNode;	// node evaluating the given parameter
    const PRM_Parm *	myParmRef;	// pointer to evaluated parameter
    int			myVectorIndex;	// parameter component index
};

struct OP_UnresolvedConnection
{
    UT_String myInputName;
    OP_Input* mySavedInput;
};

// Utility for detecting cycles when traversing graphs
typedef UT_CycleDetect<OP_Node *>	OP_NodeCycleDetect;
typedef UT_CycleDetect<OP_NodeParmRef>	OP_NodeParmRefCycle;

typedef UT_LockedRawPtr<OP_NodeList, OP_Lock> OP_LockedNodeList;
typedef UT_LockedRawPtr<const OP_NodeList, OP_Lock> OP_LockedConstNodeList;
typedef UT_SymbolMap<PY_OpaqueObject *> OP_PythonObjectCache;

// The prefix used to specify an operator where a path would normally be used.
#define OPREF_PREFIX		"op:"
#define OPREF_PREFIX_LEN	3

// Default value of hash for compiled code associated with this node that
// shouldn't conflict with the hash of any actual cached code.
#define DEFAULT_COMP_HASH_VALUE (17*19)

// Please, no spaces in these table names!
#define INVALID_TABLE_NAME	"Node"
#define OBJ_TABLE_NAME		"Object"
#define SOP_TABLE_NAME  	"Sop"
#define POPNET_TABLE_NAME	"Particle"
#define POP_TABLE_NAME		"Pop"
#define CHOPNET_TABLE_NAME	"ChopNet"
#define CHOP_TABLE_NAME 	"Chop"
#define ROP_TABLE_NAME		"Driver"
#define SHOP_TABLE_NAME		"Shop"
#define COP2_TABLE_NAME		"Cop2"
#define COPNET_TABLE_NAME	"CopNet"
#define VOP_TABLE_NAME		"Vop"
#define VOPNET_TABLE_NAME	"VopNet"
#define DOP_TABLE_NAME		"Dop"
#define TOP_TABLE_NAME		"Top"
#define LOP_TABLE_NAME		"Lop"

// Manager and Director tabel names:
#define MGR_TABLE_NAME		"Manager"
#define DIR_TABLE_NAME		"Director"

// These are the "script" directories where help and initialization scripts
// are found.
#define INVALID_SCRIPT_NAME	"node"
#define OBJ_SCRIPT_NAME		"obj"
#define SOP_SCRIPT_NAME  	"sop"
#define POPNET_SCRIPT_NAME	"part"
#define POP_SCRIPT_NAME		"pop"
#define CHOPNET_SCRIPT_NAME	"ch"
#define CHOP_SCRIPT_NAME 	"chop"
#define ROP_SCRIPT_NAME		"out"
#define SHOP_SCRIPT_NAME	"shop"
#define COPNET_SCRIPT_NAME	"img"
#define COP2_SCRIPT_NAME	"cop2"
#define VOP_SCRIPT_NAME		"vop"
#define VOPNET_SCRIPT_NAME	"vex"
#define DOP_SCRIPT_NAME		"dop"
#define TOP_SCRIPT_NAME		"top"
#define LOP_SCRIPT_NAME		"lop"

// Manager and Director scripts...  I doubt these will be used.
#define MGR_SCRIPT_NAME		"mgr"
#define DIR_SCRIPT_NAME		"dir"
#define MAT_SCRIPT_NAME		"mat"

// These are the op type names.  These are used by icons among others.
#define INVALID_OPTYPE_NAME	"NODE"
#define OBJ_OPTYPE_NAME		"OBJ"
#define SOP_OPTYPE_NAME		"SOP"
#define POPNET_OPTYPE_NAME	"PART"
#define POP_OPTYPE_NAME		"POP"
#define CHOPNET_OPTYPE_NAME	"CHOPNET"
#define CHOP_OPTYPE_NAME	"CHOP"
#define ROP_OPTYPE_NAME		"ROP"
#define SHOP_OPTYPE_NAME	"SHOP"
#define COP2_OPTYPE_NAME	"COP2"
#define COPNET_OPTYPE_NAME	"IMG"
#define VOP_OPTYPE_NAME		"VOP"
#define VOPNET_OPTYPE_NAME	"VOPNET"
#define DOP_OPTYPE_NAME		"DOP"
#define TOP_OPTYPE_NAME		"TOP"
#define LOP_OPTYPE_NAME		"LOP"

// These are the types of the manager nodes & director nodes.
// Manager nodes are things like /shop which don't have a specific
// type 
// The director node is the root node which has everything inside it.
#define MGR_OPTYPE_NAME		"MGR"
#define DIR_OPTYPE_NAME		"DIR"

/// Nice label names used by network view, corresponding to OP_OpTypeId
OP_API extern const char *OPtypeIdLabels[NUM_MANAGERS];

// Stores the status of the node - undefined for currently
// uncached state,
enum OP_CompileStatusType
{
    OP_COMPILE_STATUS_UNDEFINED = 0,
    OP_COMPILE_STATUS_COMPILED,
    OP_COMPILE_STATUS_UNCOMPILED
};

// These are some special defines used by VOPs. They need to be here
// because the OP_ConnectionMemory has to handle these operators in a
// special way,
#define VOP_OUTPUT_NODE_NAME		"output"
#define VOP_SUBNET_OUTPUT_NODE_NAME	"suboutput"
#define VOP_SUBNET_INPUT_NODE_NAME	"subinput"

// These defines provide a common place where the names of the management
// operator types are defined.
#define SHOP_MANAGEMENT_OPTYPE		"shopnet"
#define POP_MANAGEMENT_OPTYPE		"popnet"
#define COP2_MANAGEMENT_OPTYPE		"cop2net"
#define ROP_MANAGEMENT_OPTYPE		"ropnet"
#define CHOP_MANAGEMENT_OPTYPE		"chopnet"
#define SOP_MANAGEMENT_OPTYPE		"sopnet"
#define OBJ_MANAGEMENT_OPTYPE		"objnet"
#define VOP_MANAGEMENT_OPTYPE		"vopnet"
#define DOP_MANAGEMENT_OPTYPE		"dopnet"
#define TOP_MANAGEMENT_OPTYPE		"topnet"
#define LOP_MANAGEMENT_OPTYPE		"lopnet"

// This class has each of the above entities filled out.
// You can thus convert between different types by looking up
// with the known key and extracting what you want from this class.
// Looking up by id is fast, the rest requires a scan.
class OP_API OP_TypeInfo
{
public:
    OP_OpTypeId			 myOptypeId;
    const UT_StringHolder	 myOptypeName;
    const UT_StringHolder	 myTableName;
    const UT_StringHolder	 myScriptDir;
};

/// Options class for use with OP_Node::saveCommand()
struct OP_API OP_SaveCommandOptions
{
    // Initialize the options to match the opscript defaults
    OP_SaveCommandOptions
	    ( bool values_only_ = false
	    , bool defaultstoo_ = true
	    , bool docreate_ = true
	    , bool doparms_ = true
	    , bool doflags_ = true
	    , bool dowires_ = true
	    , bool dowiresout_ = true
	    , bool frames_ = false
	    , bool dogeneral_ = false
	    , bool dochblock_ = true
	    , bool dospareparms_ = true
	    , bool omit_version_info_ = false
	    )
	: values_only(values_only_)
	, defaultstoo(defaultstoo_)
	, docreate(docreate_)
	, doparms(doparms_)
	, doflags(doflags_)
	, dowires(dowires_)
	, dowiresout(dowiresout_)
	, frames(frames_)
	, dogeneral(dogeneral_)
	, dochblock(dochblock_)
	, dospareparms(dospareparms_)
	, omit_version_info(omit_version_info_)
    {
    }

    bool values_only;
    bool defaultstoo;
    bool docreate;
    bool doparms;
    bool doflags;
    bool dowires;
    bool dowiresout;
    bool frames;
    bool dogeneral;
    bool dochblock;
    bool dospareparms;
    bool omit_version_info;
};

// Simple class that makes it easy to write loops that iterate over all the
// outputs from a node. The parameters are passed on to getOutputNodes.
class OP_API OP_OutputIterator : public UT_SmallArray<OP_Node*>
{
public:
     OP_OutputIterator(const OP_Node &node,
	     bool into_subnets = false,
	     bool through_dots = true,
	     int output_idx = -1);
};

// Same as above, but with the iterators all swapped around so a range loop
// will go backwards (without having to mess with reverse adaptors)
class OP_API OP_OutputReversedIterator : public OP_NodeList
{
public:
     OP_OutputReversedIterator(const OP_Node &node,
	     bool into_subnets = false,
	     bool through_dots = true,
	     int output_idx = -1);

     OP_NodeList::const_reverse_iterator begin() const
     { return OP_NodeList::rbegin(); }
     OP_NodeList::const_reverse_iterator end() const
     { return OP_NodeList::rend(); }
     OP_NodeList::reverse_iterator	 begin()
     { return OP_NodeList::rbegin(); }
     OP_NodeList::reverse_iterator	 end()
     { return OP_NodeList::rend(); }

     OP_NodeList::const_iterator	 rbegin() const
     { return OP_NodeList::begin(); }
     OP_NodeList::const_iterator	 rend() const
     { return OP_NodeList::end(); }
     OP_NodeList::iterator		 rbegin()
     { return OP_NodeList::begin(); }
     OP_NodeList::iterator		 rend()
     { return OP_NodeList::end(); }
};

//___________________________________________________________________________


//
// a container class to describe the instantaneous state of a parameter
//
class OP_API opParmData
{
public:
    fpreal	 myValue;
    UT_String	 myLabel;
    OP_Node	*myNode;
    int		 myIndex;
    int		 mySubIndex;

    int		 operator==(const opParmData &v) const
		 {
		     return (myValue == v.myValue && 
			     myLabel == v.myLabel &&
			     myNode == v.myNode &&
			     myIndex == v.myIndex &&
			     mySubIndex == v.mySubIndex
			     );
		 }
};


// Blank class definitions for all our node types...
class OBJ_Node;
class SOP_Node;
class POP_Node;
class POPNET_Node;
class CHOPNET_Node;
class CHOP_Node;
class COP2_Node;
class COPNET_Node;
class ROP_Node;
class TOP_Node;
class LOP_Node;
class SHOP_Node;
class VOP_Node;
class VOPNET_Node;
class DOP_Node;

#define INSTANTIATE_FINDNODE_FUNCTIONS(PREFIX) 			\
    PREFIX##_Node *find##PREFIX##Node(const char *path) const 	\
    {								\
	OP_Node		*node;					\
								\
	node = findNode(path);					\
	if (node) return node->castTo##PREFIX##Node();		\
	return 0;						\
    }
#define INSTANTIATE_CASTNODE_FUNCTIONS(PREFIX) 			\
    PREFIX##_Node *castTo##PREFIX##Node() const			\
    {								\
	/* If this is triggered, use CAST_FOONODE instead */    \
	/* This is because foo->bar() should not be done with */ \
	/* NULL foo for aesthetic reasons. */ 			\
	{ const void *this_local = this;			\
	  UT_ASSERT(this_local); 				\
	  if (!this_local) return 0; } 				\
	if (getOpTypeID() == PREFIX##_OPTYPE_ID)		\
	{							\
	    return (PREFIX##_Node *) this;			\
	}							\
	return 0;						\
    }

/// This macro allows us to run another macro for all node types
#define INSTANTIATE_FOR_ALL_NODE_TYPES(MACRO_FUNC)		\
    MACRO_FUNC(OBJ)						\
    MACRO_FUNC(SOP)						\
    MACRO_FUNC(POPNET)						\
    MACRO_FUNC(POP)						\
    MACRO_FUNC(CHOPNET)						\
    MACRO_FUNC(CHOP)						\
    MACRO_FUNC(ROP)						\
    MACRO_FUNC(SHOP)						\
    MACRO_FUNC(COP2)						\
    MACRO_FUNC(COPNET)						\
    MACRO_FUNC(VOP)						\
    MACRO_FUNC(VOPNET)						\
    MACRO_FUNC(DOP)						\
    MACRO_FUNC(TOP)						\
    MACRO_FUNC(LOP)

OP_API extern const UT_StringHolder OP_USERDATA_NODESHAPE;
OP_API extern const UT_StringHolder OP_USERDATA_BGIMAGES;
OP_API extern const UT_StringHolder OP_USERDATA_WIRESTYLE;
OP_API extern const UT_StringHolder OP_USERDATA_DESCRIPTIVE_PARM;

class OP_API OP_Node : public OP_Parameters, public OP_NetworkBoxItem
{
public:
    // Methods to convert our enums and strings:
    static const OP_TypeInfo 	*getOpInfoFromOpTypeID(OP_OpTypeId opid);
    static const OP_TypeInfo 	*getOpInfoFromOpTypeName(const char *name);
    static const OP_TypeInfo 	*getOpInfoFromTableName(const char *name);
    static const OP_TypeInfo 	*getOpInfoFromScriptDir(const char *dir);
    static const char	 	*getOpTableNameFromManagerTypeName(const char *name);

    OP_Network		*getParent()  const
			 { return myParent; }
    OP_Network		*getRootCompiledParent() const
			 { return myRootCompiledParent; }
    
    //Return the network this item resides in (i.e. getParent()). This method
    //was introduced to provide a standardized way of accessing the parent
    //network of all different network box item types.
    virtual OP_Network	*getParentNetwork() const;

    // This function looks up our parent tree to determine if the given node
    // is our parent, grandparent, great-grandparent...
    bool		 getIsContainedBy(const OP_Node *testparent) const;

    /// Return the name of this node's parent
    const UT_String	&getNetName() const;

    /// This returns the OP_OpTypeId which corresponds to this node type.
    /// Management nodes will return the type they really are, which
    /// is likely different than the network they currently reside in.
    virtual OP_OpTypeId	 getOpTypeID() const = 0;

    // This is the proper way to determine the optype.
    // Do not assume your parents child type is your type!  This will only
    // become less true as time goes on :>
    // The strings returned are the *_OPTYPE_NAME defined in OP_Network.h
    virtual const char	*getOpType() const = 0;

    virtual OP_ItemType	 getItemType() const;
    
    /// @anchor OP_Node_findFOONode
    ///
    ///	FOO_Node *findFOONode(const char *path) const; @n
    ///	    This function searches for the node at path relative
    ///	    to this node.  If not found, or if the found node is
    ///	    not of type FOO_Node, it returns NULL.  The result
    ///	    is already cast to FOO_Node.
    ///	    findOBJNode(const char *path)
    ///	    findSOPNode(const char *path)
    ///	    findPOPNETNode(const char *path)
    ///	    findPOPNode(const char *path)
    ///	    findCHOPNETNode(const char *path)
    ///	    findCHOPNode(const char *path)
    ///	    findROPNode(const char *path)
    ///	    findSHOPNode(const char *path)
    ///	    findCOP2Node(const char *path)
    ///	    findCOPNETNode(const char *path)
    ///	    findVOPNode(const char *path)
    ///	    findVOPNETNode(const char *path)
    ///	    findDOPNode(const char *path)
    ///	    findTOPNode(const char *path)
    ///	    findLOPNode(const char *path)
    // @{
    INSTANTIATE_FOR_ALL_NODE_TYPES(INSTANTIATE_FINDNODE_FUNCTIONS)
    // @}

    /// @anchor OP_Node_castToFOONode
    ///
    ///	FOO_Node *castToFOONode() const; @n
    ///	    This type checks the current node against FOO_Node.
    ///	    If the cast is legal, this is returned.  Otherwise, NULL.
    ///	    While it likely will do the "right" thing with this == NULL,
    ///	    that case is UT_ASSERTed as it is just asking for trouble.
    ///	    castToOBJNode()
    ///	    castToSOPNode()
    ///	    castToPOPNETNode()
    ///	    castToPOPNode()
    ///	    castToCHOPNETNode()
    ///	    castToCHOPNode()
    ///	    castToROPNode()
    ///	    castToSHOPNode()
    ///	    castToCOP2Node()
    ///	    castToCOPNETNode()
    ///	    castToVOPNode()
    ///	    castToVOPNETNode()
    ///	    castToDOPNode()
    ///	    castToTOPNode()
    ///	    castToLOPNode()
    // @{
    INSTANTIATE_FOR_ALL_NODE_TYPES(INSTANTIATE_CASTNODE_FUNCTIONS)
    // @}

    /// Obtain the node's script which is called when it is being deleted.
    const UT_String	&getDelScript(void) const    { return myDelScript; }

    /// Set the node's deletion script. Returns true if the delete script was
    /// set successfully.
    bool		 setDelScript(const char *str);

    /// Accessors for this node's comment string.
    // @{
    const UT_String	&getComment(void) const	     { return myComment; }
    bool		 setComment(const char *str);
    // @}
    
    /// Overrides the NetworkBoxItem implementations of getColor and setColor
    /// to provide support Node-specific features like using the default color.
    // @{
    virtual UT_Color	 getColor() const;
    virtual bool	 setColor(const UT_Color &col);
    // @}

    /// Accessors for this node's flag indicating if it should use its
    /// operator's default color instead of its local color.
    // @{
    int			 getColorDefault() const;
    int			 setColorDefault(int usedefault);

    /// Accessors for this node's shape
    // @{
    const UT_StringHolder &getNodeShape() const;
    void		 setNodeShape(const UT_StringHolder &shape);
    // @}
    
    /// Node position/scale is used by the UI.
    // @{
    // These are in absolute coordinates, not the UI coordinates found in
    // OPUI_Worksheet, though.
    virtual void	 setXY(fpreal x, fpreal y);
    virtual fpreal	 getX() const { return myPosX; }
    virtual fpreal	 getY() const { return myPosY; }
    virtual fpreal	 getW() const;
    virtual fpreal	 getH() const;
    // @}

    void		 pickRequest(int shift);
    
    /// Set allow_rel_paths to true to accept "." or ".." as valid paths.
    static bool		 isValidOpName(const UT_StringView &s,
				       bool allow_rel_paths = false,
				       bool ignore_node_names_env_var = false);
    static bool		 isValidOpName(const char *s,
				       bool allow_rel_paths = false,
				       bool ignore_node_names_env_var = false)
			 {
			     return isValidOpName(UT_StringView(s),
						  allow_rel_paths,
						  ignore_node_names_env_var);
			 }
    static bool		 isValidOpPath(const UT_StringRef &s);
    static bool		 forceValidOpName(UT_String &name);

    /// Override these methods so that search-and-replace operations
    /// affect our delete script as well as our parameters.
    // @{
    virtual int		 findString(const char *str, bool fullword,
				    bool usewildcards) const;
    virtual int		 changeString(const char *from, const char *to,
				      bool fullword);
    // @}

    /// Mark this node, its descendants, and all data dependents containing
    /// varname as being dirty. If changed_nodes is non-NULL, then it is
    /// updated with the list of dirtied nodes which contains varname.
    /// Returns true iff at least one node was dirtied.
    bool		 notifyVarChange(
				const char *varname,
				OP_NodeList *changed_nodes = nullptr);
    
    //  These are no-ops for nodes but will be implemented for networks:
    virtual int		 getNchildren() const;
    virtual OP_Node	*getChild(const char *name, int *hint=0) const;
    virtual OP_Node	*getChild(int index) const;
    void		 getPickedChildren(OP_NodeList &picked,
					   bool include_hidden=false) const;
    void		 getAllChildren(OP_NodeList &children) const;
    virtual OP_Node	*matchChild(const char *pattern, OP_Node *prevmatch=0,
    					const char *optype=0,
					int casesensitive = 0) const;
    virtual OP_Node	*matchChildBackwards(const char *pattern,
					OP_Node *prevmatch=0,
    					const char *optype=0,
					int casesensitive = 0) const;
    virtual OP_Network	*createProxyRefNode( const char *path );

    // Returns the node with the requested unique integer identifier.
    static OP_Node	*lookupNode(int unique_id, bool include_proxy=false)
			 {
			     if( unique_id >= 0 && unique_id <= theUniqueId
				     && theUniqueNodes[unique_id] )
			     {
				 if( include_proxy || !theUniqueNodes[unique_id]
							   ->isProxyRefNode() ) 
				     return theUniqueNodes[unique_id];
			     }
			     return 0;
			 }
    static int		 getNumUniqueIds()
			 { return theUniqueId + 1; }
    // Fills the provided array with every node that currently exists.
    static void		 getAllNodes(OP_NodeList &nodes);

    int			 getUniqueId() const
			 { return myUniqueId; }
    virtual int64	 getItemUniqueId() const
			 { return myUniqueId; } 

    // The following function will take a standard pattern and expand
    // all the groups out of the pattern, leaving a simple pattern
    // in the result.
    bool		 expandGroupPattern(const char *pattern,
					    UT_String &result,
					    bool expandToFullPaths = false);

    // The following methods take a standard pattern and return whether
    // the pattern contains the specified bundle or group.
    bool		 patternContainsBundle(const char *pattern,
					       const OP_Bundle *bundle) const;
    bool		 patternContainsGroup(const char *pattern,
					      const OP_Group *group) const;

    virtual int		 isManager() const;
    // This differs from isManager.  Managers are the old school static
    // things which are always present in fixed directories.  Management
    // nodes are managers that are in the wrong directory.
    virtual int		 isManagementNode() const;
    // This is a bit broader than management nodes.  These nodes may
    // be in the right directory, but represent an unexpected change
    // of directory type, so need to be tracked so our tree filter
    // will know about them.
    bool		 isEffectivelyAManagementNode() const;

    // This function checks if the node is a subnet. Now that we can have
    // NWN subnets (object subnets, vopnet subnets), we need to explicitly
    // specify if these types of subnets count. In some cases we want them
    // to count (such as checking for VOPNET subnets to see if we can
    // generate code, or SHOP subnets to see if we can save to a palette).
    // In other cases we want to exclude these management ops (such as
    // when deciding to show the "Create Type From" menu item or checking
    // if we can do an opwire -i command).
    virtual int		 isSubNetwork(bool includemanagementops) const;
    virtual int		 isNetwork() const;
    virtual int          isInSubNetwork() const;

    bool		 isProxyRefNode() const
			    { return myIsProxyRefNode; }
    void		 setProxyRefNode(bool f)
			    { myIsProxyRefNode = f; }

    // Used by SOPs to determine which node is the output of a subnetwork.
    virtual bool	 isOutputNode() const;
    // Returns which output this node represents.
    virtual int		 whichOutputNode() const;

    // isNetworkWithKids() will return 1 if the node is a network and the
    // operator table for the node contains children which can be added to the
    // node.
    int			 isNetworkWithKids() const;

    virtual int		 isVex() const;

    // methods for managing the bundles we depend on in so that any change to
    // the bundle in which we are interested triggers bundleChange call.
    // NB: getParmBundle methods find or create a bundle ad it to the
    //     internal list of referenced bundles
    OP_Bundle *		 getParmBundle(const char* parm_name, int vector_index,
				UT_String &pattern, OP_Network *creator,
				const char *filter);
    void		 markBundleRefsAsUnused();
    void		 cleanUnusedBundleRefs();
    void		 cleanAllBundleRefs();

    static void		 getValueAsBundlePaths(fpreal t, 
				       UT_WorkBuffer &resultbuf,
				       PRM_Parm *parm, int index,
				       OP_Node *srcnode
				       );
    static void		 getValueAsBundlePaths(fpreal t, 
				       OP_NodeList &resultnodes,
				       PRM_Parm *parm, int index,
				       OP_Node *srcnode
				       );

    // The bundleChanged() method is called whenever a bundle referenced by the
    // OP is changed.  The bundle is passed to the OP to let the OP know that
    // it's out of date.  The function should return 1 if the event was handled
    // properly or 0 if it failed.  The return codes are used for debugging
    // only.
    virtual bool	 bundleChanged(const OP_Bundle *bundle);
    // If a bundle changes, you should call this method to touch the parameter
    // which the bundle affects.  The name is the token of the parm.
    void		 touchBundleParm(const char *name);
    void		 touchBundleParm(int parm_index);

    // Function to determine if Display and Render node ptrs are the same
    virtual int		 getDandROpsEqual() { return 1; }
    // Function to update info about whether Render and Display node ptrs
    // are the same. Recurses tree and updates all kids values.
    virtual int		 updateDandROpsEqual(int = 1) { return 1; }

    /// Check to see whether @c node is an ancestor of @c this node in the
    /// input hierarchy.  Optionally check extra inputs.  This method will
    /// recurse using @c getInputs() (and optional getExtraInputNodes()
    /// @note This node is considered to be an ancestor of itself
    bool		isInputAncestor(const OP_Node *parent,
				bool check_extra=true) const;
    /// Check to see whether the @c parent node is an ancestor of @c this node
    /// in the parent/child hierarchy.  That is, this method will call
    /// getParent()/getChild() to traverse the hierarchy.
    /// @note This node is considered to be an ancestor of itself
    bool		isParentAncestor(const OP_Node *parent) const;

    /// Get the info text for this op. This non-virtual function does
    /// all the stuff common to all operator types.
    /// @warning The return value is the value of the work buffer in the
    /// OP_NodeInfoParms.  Thus, you should never do @code
    ///	    str = node->getInfoText(context, OP_NodeInfoParms());
    /// @endcode
    /// Since the lifetime of the temporary may not be long enough to extract
    /// the string properly.
    const char  	*getInfoText(OP_Context &context,
				OP_NodeInfoParms &parms);
    // Get Info Text that is specific to this node type.
    virtual void	 getNodeSpecificInfoText(OP_Context &context,
				OP_NodeInfoParms &parms);

    /// Uses the path (eg. "/obj/geo1") to find a node in our heirarchy
    OP_Node		*findNode(const char *path) const;

    /// Same as findNode() except it will return NULL if the path is to a node
    /// that is either not a descendent of this node, or if it isn't the same
    /// type as our children.
    OP_Node		*findSubNode(const char *path) const;
    
    // These are all Implemented in OP_Network, but put here as a virtual to
    // make working / with hscript easier so that it can be called directly on
    // an op node
    virtual OP_NetworkBox *findNetworkBox(const char *name)
			       { return nullptr; }
    virtual OP_PostIt	*findPostItNote(const char *const_path)
				{ return nullptr; }
    virtual OP_Dot	*findDot(const char *const_path)
				{ return nullptr; }
    virtual OP_SubnetIndirectInput *findParentInput(const char *const_path)
				{ return nullptr; }
    virtual OP_NetworkBoxItem *findItem(const char *const_path, 
					OP_ItemTypeMask item_type = 
					OP_ITEMTYPE_ALL)
				{ return nullptr; }

    // These functions will adhere to certain conventions, such as a reference
    // to an object actually means a reference to the display or render
    // guy in that object.  This will also check this->isCookingRender
    // to determine if it should auto-use the Render or Display.
    // These will not fail if the returned node is the node itself.
    SOP_Node		*getSOPNode(const char *path, int addextra = 0) const;
    // This method will use the render node of the composite network if
    // it is a comp rather than a cop2 that is pointed to...
    COP2_Node		*getCOP2Node(const char *path, int addextra = 0) const;
    // And this one will also use the render node, which corresponds to the
    // cook node in POPs land.
    POP_Node		*getPOPNode(const char *path, int addextra = 0) const;
    // And this one will use the display node, which corresponds to the
    // cook node in DOPs land.
    DOP_Node		*getDOPNode(const char *path, int addextra = 0) const;
    ROP_Node		*getROPNode(const char *path, int addextra = 0) const;
    // This method will use the audio node of the chop network
    CHOP_Node		*getCHOPNode(const char *path, int addextra = 0, bool *got_by_flag=0) const;
    // This method will use the display node of the top network
    TOP_Node		*getTOPNode(const char *path, int addextra = 0) const;
    // This method will use the display node of the lop network
    LOP_Node		*getLOPNode(const char *path, int addextra = 0) const;
    OBJ_Node		*getOBJNode(const char *path, int addextra = 0) const;
    
    virtual OP_Node	    *castToOPNode()	    { return this; }
    virtual const OP_Node   *castToOPNode() const   { return this; }

    /// Returns the path of this node relative to its getCreator()
    virtual void	 getPathWithSubnet(UT_String &str) const;
    void	 	 getPathWithSubnet(UT_StringHolder &str) const
    { UT_String tmp; getPathWithSubnet(tmp); str.adoptFromString(tmp); }

    /// This will try to complete the path by finding the longest suffix
    /// that will work with the given prefix.  It will only complete up
    /// to the next child.  If it finds a valid child, it will append
    /// a slash (/).
    void		 completePath(const char *prefixpath, 
				      UT_String &completepath) const;

    virtual CH_Channel	*getTrackChannel(const char *name);

    virtual bool	 findParmFromTrack(const OP_FollowChanRefsOptions& opt,
					   const char *trackname,
					   OP_NodeParmRefCycle &cycle,
					   OP_Node *&node,
					   PRM_Parm *&parm, int &vecidx);

    // Batch evaluation of named parameters.  This method requires extra lookup
    // since parameters are specified by names, but it can handle generic
    // parameter layout.
    virtual void	 evaluateBatchParms(PRM_BatchList &list, fpreal now); 

    // Do global expansion on a pattern - get lists of channels or nodes
    // If expand is given, concatenated with full list of names
    // If list is given, appended with list of nodes.
    // If dependent given, addExtraInputs.
    void		 globNodes(const char *pat, UT_String *expand,
					UT_ValArray<OP_Node *> *list=0,
					OP_GlobContext *context = 0,
					const char *prefix = " ");

    CH_Channel		*findChannel(const char *path);
    void		 globChannels(const char *, UT_String &expand,
					OP_GlobContext *context = 0,
					const char *prefix = " ") const;

    void		 globChannels(const char *pattern,
				      CH_ChannelList &clist,
				      OP_GlobContext *context = 0,
				      OP_Node *dependent = 0);
    // If use_multi_match is false, the pattern is
    // treated as a single pattern instead of a space
    // separated list of patterns. (Only applies
    // to direct parameter names, i.e. the pattern must not be
    // a path and must not contain a slash)
    void		 globChanRefs(const char *pattern,
				      CH_ChannelRefList &list,
				      OP_GlobContext *context = 0,
				      OP_Node *dependent = 0,
				      bool use_multi_match = true);

    int			 globParms(const char *pattern, fpreal t,
				   UT_Array<opParmData> &list,
				   OP_GlobContext *context = 0,
				   OP_Node *dependent = 0,
				   int calc_value = 1);

    // this version only globs the current node
    void		 globNodeParms(const char *pattern, fpreal t,
		                       UT_Array<opParmData> &list,
				       OP_GlobContext *context,
				       OP_Node *dependent,
				       int calc_value,
				       bool animated_only);

    // this version only globs the current node
    void		 globNodeChannels(const char *pattern,
					  CH_ChannelList &list,
					  OP_GlobContext *context,
					  int *parm_start = nullptr,
					  UT_IntArray *parm_indices = nullptr,
					  UT_IntArray *parm_sub_indices = nullptr
					  ) const;

    // this version only globs the current node
    // If use_multi_match is false, the pattern is 
    // treated as a single pattern instead of a space
    // separated list of patterns
    void		 globNodeChanRefs(const char *pattern,
					  CH_ChannelRefList &list,
					  OP_GlobContext *context,
					  int *parm_start = nullptr,
					  UT_IntArray *parm_indices = nullptr,
					  UT_IntArray *parm_sub_indices=nullptr,
					  bool use_multi_match = true
                      			  ) const;

    ///Same as 'globNodes', except for network boxes. This is to be implemented
    ///by OP_Network, but has been put here so it can be called directly on 
    ///OP_Node like all the other "glob" methods without a cast
    virtual void	 globNetworkBoxes(const char *pat, UT_String *expand,
				    UT_ValArray<OP_NetworkBox *> *list=0,
				    OP_GlobContext *glob_context = 0,
				    const char *prefix = " ");

    virtual void	 globPostIts(const char *pat, UT_String *expand,
				    UT_ValArray<OP_PostIt *> *list=0,
				    OP_GlobContext *glob_context = 0,
				    const char *prefix = " ");

    // Fetch all channels from this node on down.
    int			 getAllChannels(CH_ChannelList &list, int scoped=0);

    // If you override opChanged() in a sub-class, you must call
    // the OP_Node base method from the sub-class method
    virtual void	 opChanged(OP_EventType reason, void *data=0);
    
    // If you override opChanged() in a subclass, you should first call
    // opShouldHandleChange() before taking any custom action.
    virtual bool	 opShouldHandleChange(OP_EventType reason);

    // This function is called whenever we have a parameter that references
    // another parameter, and that referenced parameter changes. The pi
    // value is the index of the parameter containing the reference.
    virtual void	 referencedParmChanged(int pi);

    /// Mark this node as needing a full recook.
    /// If 'evensmartcache' is true, a recook will occur even if all the
    /// parameters match in this node and all its inputs (only valid for
    /// COP2_Node). 
    virtual void	 forceRecook(bool evensmartcache = true);

    /// This method causes any operators that load files to recook.
    /// By default, this searches for 'file' operators by iterating through
    /// inputs. Parms of type PRM_FILE are also marked dirty. Other nodes that
    /// load files differently may wish to override this method.
    virtual void	 reloadExternalFiles();

    /// Determines if this node needs to cook.
    ///
    /// The queryonly flag will not flag this as a read attempt to the
    /// caching mechanisms, and thus not update the touch time.
    virtual unsigned	 needToCook(OP_Context &context, bool queryonly=false);

    /// Cook this node's data. Returns true upon success, false otherwise.
    ///
    /// Upon failure, use OP_Parameters::error() and
    /// OP_Parameters::getErrorMessages() to determine reason.
    virtual bool	 cook(OP_Context &context);

    /// Schedule a task for cooking this node in parallel and wait for it to
    /// finish. Returns true upon success, false otherwise.
    bool		 parallelCook(OP_Context &context);

    /// Cook the given node list in parallel
    static bool		 parallelCook(OP_Context &context,
				      const OP_NodeList &nodes);

    /// The cook lock is used to protect per node data structures during
    /// cooking. Subclasses should use this when calling cook(). Example:
    /// @code
    /// cookLockedExecute([&]() {
    ///	    // run code
    /// });
    /// @endcode
    template <typename F>
    void		 cookLockedExecute(const F &functor)
			 {
			    ev_GlobalEvalLock().lockedExecute([&]()
			    {
				OP_CookLock::Scope scope(myCookLock);
				functor();
			    });
			 }

    /// The cook lock is used to protect per node data structures during
    /// cooking. Subclasses should use lock this to protect private data.
    OP_CookLock &	 getCookLock()	    { return myCookLock; }

    UT_TaskState &	 taskState()	    { return myTaskState; }
    const UT_TaskState & taskState() const  { return myTaskState; }

    // This normally chains to cookInputGroups.
    // Some networks, such as SOPs, use it to be able to
    // properly lock their gdp pointer around the cook.
    virtual OP_ERROR	 pubCookInputGroups(OP_Context &context, int alone = 0);

    void		 bumpVersionParm()
			    {
				dataMicroNode().bumpModVersion();
				if (hasIndepParmListMicroNode())
				    parmListMicroNode().bumpModVersion();
			    }
    int			 cookParmsModified() const
			    { return parmListMicroNodeConst()
				.isModVersionOutdated(); }

    // The data class of an OP is currently used solely for networks which
    // contain heterogeneous node types (i.e. used in SHOP networks to
    // distinguish different shader types).  A data class of 0 is returned by
    // default.
    // A value of 0 is returned by default.
    virtual int		 getDataClass() const;

    // The xform order for how to interpret the given channel.  Default
    // is XYZ.
    virtual void	 getXformOrder(UT_XformOrder &xord,
				       const CH_Channel *chp) const;

    // "getCookedData" should return a void * version of the results of
    // the cook. NULL if it fails with appropriate stuff in our error manager.
    virtual OP_DataType	 getCookedDataType() const = 0;
    virtual void	*getCookedData(OP_Context &);
    virtual void	 deleteCookedData() = 0;
    
    virtual int		 saveCookedData(std::ostream &os, OP_Context &,
					int binary = 0) = 0;
    virtual bool	 loadCookedData(UT_IStream &is, const char *path=0);
    virtual int		 saveCookedData(const char *filename, OP_Context &) = 0;

    /// Get the transform from this node's space to the 'to' node's space.
    /// Returns 1 if this was possible, 0 otherwise (e.g. failure to cook).
    virtual int          getRelativeTransform(OP_Node &to, UT_Matrix4 &xform,
					      OP_Context &context);
    virtual int          getRelativeTransform(OP_Node &to, UT_DMatrix4 &xform,
					      OP_Context &context);
    // overrides for OPs that can capture (right now only OBJ_Bone)
    virtual int          getRelativeCaptureTransform(OP_Node &to, 
					      UT_Matrix4 &xform,
					      OP_Context &context);
    virtual int          getRelativeCaptureTransform(OP_Node &to, 
					      UT_DMatrix4 &xform,
					      OP_Context &context);

    enum TransformMode
    {
	TRANSFORM_WORLD,
	TRANSFORM_IWORLD,
	TRANSFORM_PRE,
	TRANSFORM_PARENT,
	TRANSFORM_PARM,
	TRANSFORM_PRECONSTRAINT,
	TRANSFORM_PARENTBONE,
    };

    // these next 3 are the minimal overrides for subclasses that have
    // transforms (see OBJ_Node)
    virtual bool	 getWorldTransform(UT_Matrix4D &xform, OP_Context &);
    virtual bool	 getIWorldTransform(UT_Matrix4D &xform, OP_Context &);
    virtual bool	 getTransform( TransformMode mode, UT_Matrix4D &xform, OP_Context &);

    // single precision versions of the above
    bool		 getWorldTransform(UT_Matrix4F &xform, OP_Context &ctx)
			 {
			     UT_Matrix4D m;
			     bool ok = getWorldTransform(m, ctx);
			     xform = m;
			     return ok;
			 }
    bool		 getIWorldTransform(UT_Matrix4F &xform, OP_Context &ctx)
			 {
			     UT_Matrix4D m;
			     bool ok = getIWorldTransform(m, ctx);
			     xform = m;
			     return ok;
			 }
    bool		 getTransform( TransformMode mode, UT_Matrix4F &xform, OP_Context &ctx)
			 {
			     UT_Matrix4D m;
			     bool ok = getTransform(mode, m, ctx);
			     xform = m;
			     return ok;
			 }

    /// Registers an error for cooking the given node's transform. If label is
    /// supplied, then it will be used as a label of the transform error.
    void		 addTransformError(const OP_Node &node,
					   const char *label=0) const;

    // Compute the bounding box for the operator (if applicable).  If the box
    // could be computed, return true, else return false.
    virtual bool	getBoundingBox(UT_BoundingBox &box, OP_Context &);

    // This is used by VOPNETs to determine which table needs to be
    // refreshed when they change.
    virtual const char  *getInternalOpTable() const 
			 { return INVALID_TABLE_NAME; }
    virtual OP_Operator	*getInternalOperator() const
			 { return 0; }

    virtual const char  *inputLabel(unsigned idx) const;
    virtual const char  *outputLabel(unsigned idx) const;

    // Here's a useful method which will return the string "Source %d", where
    // %d is the idx passed. Lets us check if the input label is using the
    // default or not.
    const char		*inputLabelNum(unsigned idx) const;
    const char		*outputLabelNum(unsigned idx) const;

    // returns a label that is applied to the drawn connector. NULL may be
    // returned as 'no label'.
    virtual const char  *inputConnectorLabel(unsigned idx);
    
    // if true, the connector to the input will be dashed rather than solid.
    virtual int		 isRefInput(unsigned idx) const;

    // returns the index of the auxilary input, if any (currently only used
    // for the mask inputs in COPs).
    virtual int		 getAuxInput() const;
    virtual void	 getAuxInputName(OP_ConnectorId& name_out);
    
    //  The lock method is used to lock the OP.
    //  Returns the previous state of the lock flag.
    //
    // Note that to mimic the behaviour of the lock button, you MUST
    // call setModelLock, not setLock, or your next unlock won't have
    // a cached data.
    int			 setLock(int state, int saveundo = 1);
    int			 getSoftLock() const {return flags().getSoftLocked();}
    int			 getHardLock() const {return flags().getHardLocked();}
    int			 getLock() const     {return flags().getLocked(); }
    // getNetworkLock() checks to see if the OP is effectively locked by
    // the network, i.e., each of the OPs output branches connect to a locked
    // OP down the network. Returns > 0 if locked, <= 0 if unlocked.
    // When relaxedlockcheck is set, subnet branches that are not displayed
    // are ignored.
    int			 getNetworkLock(bool relaxedlockcheck = false,
					const OP_Node *inputNode=nullptr) const;
    //  The Model lock is similar to the normal lock but will also cause
    //  the node data to be saved.
    virtual int		 setModelLock(int on_off, 
				      void *modeler = 0,
				      int allow_softlock_promotion=0,
				      UT_Undo *undo = 0);
    int			 getModelLock() const;

    // The unload flag triggers whether we keep the ops data around
    // after a cook or discard it as soon as its not needed.  Its currently
    // only implemented in CHOPs.
    int			 setUnload(int on_off);
    int			 getUnload() const;

    // Returns whether this node has data loaded into it.
    virtual bool	 isLoaded() const { return true; }

    // This will explicitly delete all data associated with the OP
    // false is returned if no data was deleted (ie, OP is in use, etc)
    // Only works with SOPs now.
    virtual bool	 unloadData();

    // Take control for flags
    void		 takeActivateFlag(uchar flag, bool enable=true,
					  bool send_flag_event=true);
    void		 takeDeactivateFlag(uchar flag)
			 { takeActivateFlag(flag, false); }
    bool		 takeIsActiveFlag(uchar flag) const;
    void		 setHasTakeData(int onoff);

    bool		 canAccessFlag(unsigned mask, uchar flag) const;

    // The template flag indicates that the node date should be displayed
    // The display & render flags indicate that the node is turned on
    // for display purposes or for rendering purposes, which may take
    // on different meanings per node type.
    // Setting save data only has meaning for some node types.
    virtual int		 setPicked(int on_off, 
				   bool propagate_parent_event = true);

    /// The edit flag is used whenever something is picked
    /// from the viewport. This causes the pane paths to
    /// be changed to the first writable parent of the
    /// node being picked.
    int			 setEditPicked(int on_off, 
				   bool propagate_parent_event = true);

    // The Autoscope flag on OP_Node is used to temporary turn-off the Auto-Add to Channel List
    // behavior when selecting nodes.
    // This is used when setting selected nodes from selected channels in the channel list
    void		 setAutoscope(bool on_off) { myAutoscope = on_off; }
    bool		 isAutoscope() { return myAutoscope; }

    int			 setCurrent(int on_off);
    int			 setExpose(int on_off);
    int			 setBypass(int on_off);
    int			 setTemplate(int on_off);
    int			 setFootprint(int on_off);
    int			 setXray(int on_off);
    int			 setDisplay(int on_off);
    int			 setRender(int on_off);
    int			 setHighlight(int on_off);
    int			 setModified(int on_off);
    int			 setSaveBypass(int on_off);
    int			 setSaveData(int on_off);
    // these are for subclasses to implement, they do nothing by default
    virtual int		 setPickable(int on_off)
			 { return 0; }
    virtual int		 setAudio(int on_off)
			 { return 0; }
    virtual int		 setExport(int on_off)
			 { return 0; }
    virtual int		 setDebug(int on_off)
			 { return 0; }
    virtual int		 setDisplayOrigin(int on_off_unchanged)
			 { return 0; }

    /// The user has set the template flag. If we are turning on the template
    /// flag, we do the same as setTemplate(). If we are turning it *off*,
    /// we also turn off the selectable template (footprint) flag.
    void userSetTemplate(bool onoff);

    /// The user has set the selectable template flag. If we are making a node
    /// into a selectable template, we turn on the template flag as well.
    /// If we are turning off selectable template, we leave template alone.
    void userSetSelectableTemplate(bool onoff);

    // Here are the generalised versions, they add a bit of overhead to
    // the code because of the big switch statement.
    virtual int		 setFlag(char tag, int on_off);
    virtual int		 getFlag(char tag) const;

    virtual int		 getPicked()   const  {return flags().getPicked(); }
    virtual int		 getEditPicked() const
					{ return flags().getEditPicked(); }
    int			 getCurrent()  const;
    int			 getDisplay()  const  {return flags().getDisplay(); }
    int			 getRender()   const  {return flags().getRender(); }
    int			 getHighlight()const  {return flags().getHighlight();}
    int			 getBypass()   const  {return flags().getBypass(); }
    int 		 getTemplate() const  {return flags().getTemplate(); }
    int 		 getFootprint() const {return flags().getFootprint(); }
    int			 getXray() const      {return flags().getXray(); }
    int			 getExpose()   const  {return flags().getExpose(); }
    int			 getModified() const  {return flags().getModified(); }
    int			 getSaveBypass() const{return flags().getSaveBypass(); }
    int			 getSaveData() const  {return flags().getSaveData(); }
    virtual int		 getHasTakeData() const
			 { return flags().getHasTakeData(); }
    virtual int		 getPickable()
			 { return 0; }
    virtual int		 getAudio() const
			 { return 0; }
    virtual int		 getExport() const
			 { return 0; }
    virtual int		 getDebug() const
			 { return 0; }
    virtual int		 getDisplayOrigin() const
			 { return 0; }
    virtual bool	 getItemExpose() const { return getExpose(); }
    virtual void	 setItemExpose(bool expose) { setExpose(expose); }
    virtual bool	 getUseBypassLook(const OP_EditorContext &ctx) const;

    int			 getDisplayDescriptiveName() const
			 {return flags().getDisplayDescriptiveName();}
    int 		 setDisplayDescriptiveName(int state);
    int			 getDisplayComment() const
			 {return flags().getDisplayComment();}
    int 		 setDisplayComment(int state);

    OP_Node		*getPickableSelfOrParent();

    // The compress flag indicates that the OP connector should be displayed
    // in a compressed UI. In ICE this means don't display the mini raster
    // image.
    int			 setCompress(int on_off);
    int			 getCompress() const;

    // This method sets this node as the current and picked node and correctly
    // adjusts the picked states of the other sibling nodes.
    void		 setCurrentAndPicked();		 

    // setVisible() simply calls setDisplay() at this level but subclasses
    // override this to perform smarter behaviour. Objects for example will
    // try to set the display flag on parent subnets and change visible object
    // parameters as necessary.
    // Returns true if it was successful, false if there were not sufficient
    // permissions.
    virtual bool	 setVisible(int onoff);
    virtual bool	 getVisible() const;

    const OP_NodeFlags	&flags() const
			 { return myFlags; }
    OP_NodeFlags	&flags()
			 { return myFlags; }

    virtual void	 getSaveFlagsString(UT_String &cmd,
					    const char *flags,
					    bool save_to_hip) const;
    virtual void	 getPreParmSaveFlagsString( UT_String &cmd ) const;
    virtual void	 getPostParmSaveFlagsString( UT_String &cmd ) const;

    int			 hasOpInterest(void *data, OP_EventMethod m) const
			     { return myEventValue.hasOpInterest(data, m); }
    void		 addOpInterest(void *data, OP_EventMethod m)
			     { myEventValue.addOpInterest(data, m); }
    void		 removeOpInterest(void *data, OP_EventMethod m)
			     { myEventValue.removeOpInterest(data, m); }
    int			 isBeingDeleted() const
			     { return myBeingDeleted; }
    void		 setNodeBeingDeleted(int beingdeleted);

    /// Sets the flag that prevents consolidation of variable inputs.
    /// Returns previous value of the flag;
    bool		 setInhibitInputConsolidation(bool value);

    /// Move the upper-left most node in 'ops' to x-coordinate 'x' and to have
    /// its TOP border at y-coordinate 'y', and move all other nodes in 'ops'
    /// by the same amount. All netboxes in 'netboxes' will also be moved that
    /// amount. If a netbox's top border is higher than any node's top border,
    /// everything will be moved down accordingly. Note that the repositioning
    /// calculation does not include the nodes contained in the network boxes
    /// in 'netboxes'.
    static void          repositionOpsMaxY(OP_NetworkBoxItemList &items, 
				fpreal x, fpreal y);

    /// Minimum inputs that must be connected to a node for it to cook.
    virtual unsigned	 minInputs() const;

    /// Maximum inputs that can be connected to a node.
    virtual unsigned	 maxInputs() const;

    /// Number of input connectors that should be visible on a node. This is
    /// only used by VOPs and DOPs. All other OPs return maxInputs() here.
    virtual unsigned	 getNumVisibleInputs() const;

    /// Returns the number of ordered inputs on a node with unordered inputs.
    /// Even nodes with unordered inputs can have a few ordered inputs before
    /// the unordered inputs start. If hasUnorderedInputs returns false, this
    /// function is meaningless.
    virtual unsigned	 orderedInputs() const;

    /// Maximum number of output connectors on a node. Only VOPs and DOPs ever
    /// return a value other than 1 here.
    virtual unsigned	 maxOutputs() const;

    /// Number of output connections that should be visible on a node tile.
    /// Only used by VOPs and DOPs.
    virtual unsigned	 getNumVisibleOutputs() const;

    /// Returns true if the operator type says we have unordered inputs.
    bool		 hasUnorderedInputs() const;

    /// Returns the index of the last connected input incremented by one. 
    /// If there are no inputs connected, returns 0. This means that the
    /// function behaves as if the lasted connected input was always the
    /// last entry in the inputs array. Note that other array entries up
    /// to the index of the one returned may contain null entries, so this
    /// is not necessarily the same as the number of connections. Also note
    /// that there may be more null entries at indices beyond the returned
    /// one.
    virtual unsigned	 nInputs() const;

    /// The number of nodes actually connected to this node. This function
    /// goes through the myInputs array and counts non-null entries.
    /// @param check_subnet_inside If true, the (vopnet) subnet node will check
    ///	    if a corresponding connector is connected inside, and count it in
    ///	    if so.
    unsigned		 nConnectedInputs(bool check_subnet_inside=true) const;

    unsigned		 getActiveInputIndex() const
			 { return myActiveInputIndex; }
    void		 setActiveInputIndex(unsigned idx);

    /// Returns the node connected to a particular input (may be null).
    OP_Node		*getInput(unsigned idx, bool mark_used=false) const;

    /// Returns the connected input, may be null.  If it is a SOP & subnet,
    /// and not primary output, will chase to the child output node.
    /// This may induce flag dependencies, the provided micronode
    /// can accumulate them.
    OP_Node		*getInputFollowingOutputs(int input,
				DEP_MicroNode *depnode=0) const;

    /// Returns the first non-null input beyond the specified starting index.
    /// This function can be used to easily loop through all connected inputs.
    int			 getConnectedInputIndex(int startAt = -1) const;

    /// Gets the index of the Nth non-null connection to this node.
    int			 getNthConnectedInput(int n) const;

    /// Override this to specify the inputs which are needed for cooking this
    /// node which can be executed in parallel.
    virtual void	 getParallelInputs(OP_Context &context,
					   OP_NodeList &nodes) const;

    /// Forcibly clear an input. Used when deleting the input.
    void		 forceClearInput(int idx);

    /// Sets a given input to connect to an output of a particular node.
    virtual OP_ERROR	 setInput(unsigned idx, OP_Node *op,
				  unsigned outputIdx = 0);

    /// Connects an input to an indirect input of our parent subnet.
    virtual OP_ERROR	 setIndirectInput(unsigned idx,
					  OP_IndirectInput *input);

    /// Connects an input to particular node by name in the network.
    virtual OP_ERROR	 setInputReference(unsigned idx, const char *label,
					   int keeppos, unsigned outputIdx = 0);

    /// Returns the index of the first connection from node "who" to this node,
    /// or -1 if "who" is not an input.
    int			 whichInputIs(const OP_Node *who) const;

    /// Returns the index of the first connection of an indirect input.
    int			 whichInputIs(const OP_IndirectInput *who) const;

    // Returns the index of the Nth connection of an idirect input.
    int			 whichInputIs(const OP_IndirectInput *whoi,
				      int cnt) const;

    // Returns an index of the output which goes out from this node to
    // the target_node, and connects to the target_node at the specified
    // input index. Returns -1 if the connection doesn't exist.
    // Use whichOutputIsFollowingIndirect for a more robust solution.
    int			 whichOutputIs(const OP_Node* target_node,
				int input_on_target_node) const;

    // Computes the output, but handles the case that target_node's input
    // is an indirect input.  This will then follow the indirect chain
    // up until a node's real input.
    int			 whichOutputIsFollowingIndirect(
				const OP_Node* target_node,
				int input_on_target_node) const;

    // Returns the index of an input into which source_node is connected
    // on this node. counter determines the Nth such connection from 
    // source node to this particular node.
    int			 whichInputIs(OP_Node* source_node, int counter);

    // Compacts the inputs array, removing all null entries.
    void		 consolidateInputs();
    virtual void	 clearUnreferencedInputs();
    virtual bool	 allowConsolidatingInput(int idx);

    // Returns a pointer to an input given an input index. If the input index
    // is less than nInputs() it will always return a valid input. If the grow
    // flag is set to true, then any inputs between nInputs and the index
    // given will be created, up to maxInputs().
    virtual OP_Input	*getInputReference(unsigned idx, bool grow);

    // As before, except missing inputs are not created. NULL is returned for
    // non-existent inputs.
    virtual OP_Input	*getInputReferenceConst(unsigned idx) const;

    virtual int		 doDeleteRewire() const { return 1; }
    // This moves the input at srcidx to dstidx.  forcesubnet will enable
    // modificatio of subnet inputs and outputs even when inside a
    // blockModify().
    virtual void	 moveInput(int srcidx, int dstidx,
				   bool forcesubnet = false);
    OP_ERROR		 insertInput(unsigned idx, OP_Node *op,
				     unsigned outputIdx);
    OP_ERROR		 insertIndirectInput(unsigned idx,
					     OP_IndirectInput *input);

    // For nodes with editable input names, this function is used to set the
    // default name for a new input.
    virtual UT_Options	 getInputDataDefault(int idx) const
			 { return UT_Options(); }

    /// The following methods are used for nodes that allow reordering inputs.
    /// Returns the number of inputs that editor should display. 
    virtual int		 getInputEditorInputs();
    /// Returns true if editor should display parameter option menu button.
    virtual bool	 allowInputEditorInputParm();
    /// Returns true if editor should display delete input button.
    virtual bool	 allowInputEditorInputDelete();
    /// Gets the input label to display in editor and returns true. Otherwise, 
    /// returns false if editor should use backward-compatible labeling scheme.
    virtual bool	 getInputEditorInputLabel(UT_String &label, int idx);

    virtual const OP_DataMicroNode &
			 dataMicroNodeConst() const
			    { return myDataMicroNode; }
    OP_DataMicroNode &	 dataMicroNode()
			 {
			     return const_cast<OP_DataMicroNode &>(
					dataMicroNodeConst());
			 }

    /// Subclasses should override this to customize the micro-node dependency
    /// of their parameter list. The default is just dataMicroNode().
    virtual const OP_DataMicroNode &
			 parmListMicroNodeConst() const 
			    { return dataMicroNodeConst(); }
    OP_DataMicroNode &	 parmListMicroNode()
			 {
			     return const_cast<OP_DataMicroNode &>(
					parmListMicroNodeConst());
			 }

    bool		 hasIndepParmListMicroNode() const
			 {
			     return (&parmListMicroNodeConst()
					!= &dataMicroNodeConst());
			 }

    DEP_MicroNode &	 flagMicroNode()
			    { return myFlagMicroNode; }
    DEP_MicroNode &	 parmMicroNode(int parm_idx, int vi)
			    { return getParmList()->parmMicroNode(
							    parm_idx, vi); }

    /// Accessors to event micro nodes.
    const DEP_MicroNode &eventMicroNodeConst(OP_EventType e) const
			 {
			     return SYSconst_cast(this)->eventMicroNode(e);
			 }
    DEP_MicroNode &	 eventMicroNode(OP_EventType e)
			 {
			    // Create the micro node if it does not exist.
			    if (!myEventMicroNodes[e])
			    {
			        myEventMicroNodes[e] = 
				    new OP_EventMicroNode(*this, e);
			    }

			    return *myEventMicroNodes[e];
			 }

    /// Subclasses should override this provide any additional micronodes that
    /// they own. This list is used in places such as node deletion so that
    /// dependents are dirtied.
    virtual void	 getOwnedMicroNodes(
			    DEP_MicroNodeList &micronodes);

    /// Explicitly propagate dirtiness for a particular micronode owned by
    /// some node.
    void		 propagateDirtyMicroNode(
			    DEP_MicroNode &micronode,
			    OP_EventType reason,
			    void *data,
			    bool send_root_event);

    /// Declares that this node's data depends on the @c op for the given
    /// reason (OP_INTEREST_DATA, OP_INTEREST_FLAG).
    /// @note There are no other acceptable values for type.
    virtual void	 addExtraInput(OP_Node *op, OP_InterestType type)
			    { addExtraInput(OP_InterestRef(*op, type)); }

    /// Declares that this node's data depends on @c op channel. If vec_i is
    /// -1, then this node depends on all the channels of the parameter.
    void		 addExtraInput(OP_Node &op, int parm_i, int vec_i)
			    { addExtraInput(OP_InterestRef(op,parm_i,vec_i)); }

    /// Generic version of addExtraInput() which adds a dependency from an
    /// source ref to this node's data micronode.
    void		 addExtraInput(const OP_InterestRef &source_ref);

    /// Generic version of addExtraInput() which adds a dependency from an
    /// source micronode to this node's data micronode.
    void		 addExtraInput(DEP_MicroNode &source_micronode);

    /// Marks this to be dependent on all the parameters inside of the
    /// multiparm.
    void		 addMultiparmInterests(OP_Node *srcnode, PRM_Parm &parm);


    static DEP_MicroNode *getEvalChannelMicroNode(int thread);

    /// Declare from an expression callback that the current evaluation channel
    /// is dependent on the given source.
    // @{
    static void		 addExtraInputToEvalChannel(
			    int thread,
			    OP_Node &op, OP_InterestType type)
			{
			    addExtraInputToEvalChannel(
				    thread,
				    OP_InterestRef(op, type));
			}
    static void		 addExtraInputToEvalChannel(
			    int thread,
			    OP_Node &op, int parm_i, int vec_i)
			{
			    addExtraInputToEvalChannel(
				    thread,
				    OP_InterestRef(op,parm_i,vec_i));
			}
    static void		 addExtraInputToEvalChannel(
			    int thread,
			    const OP_InterestRef &source_ref)
			{
			    DEP_MicroNode *
				target = getEvalChannelMicroNode(thread);
			    if (target)
				OP_Node::addExtraInput(*target, source_ref);
			}
    static void		 addExtraInputToEvalChannel(
			    int thread,
			    DEP_MicroNode &src_micronode)
			{
			    DEP_MicroNode *
				target = getEvalChannelMicroNode(thread);
			    if (target)
				target->addExplicitInput(src_micronode);
			}
    // @}

    /// Generic versions of addExtraInput() used by the others. In particular,
    /// the static version of addExtraInput() allows you to define channel to
    /// channel dependencies.
    // @{
    static void		 addExtraInput(
			    const OP_InterestRef &target_ref,
			    const OP_InterestRef &source_ref);
    static void		 addExtraInput(
			    const OP_InterestRef &target_ref,
			    DEP_MicroNode &source_micronode);
    static void		 addExtraInput(
			    DEP_MicroNode &target_micronode,
			    const OP_InterestRef &source_ref);
    // @}

    static void		 addMultiparmInterests(const OP_InterestRef &target_ref,
					       OP_Node *srcnode,
					       PRM_Parm &parm);
    static void		 addMultiparmInterests(DEP_MicroNode &target,
					       OP_Node *srcnode,
					       PRM_Parm &parm);

    void		 getExtraInputNodes(
			    OP_NodeList &extras,
			    bool remove_duplicates = true,
			    bool data_interest = true,
			    bool parm_interest = true,
			    bool flag_interest = true,
			    bool include_parmlist = true) const;
    void		 getExtraOutputNodes(
			    OP_NodeList &extras,
			    bool remove_duplicates = true,
			    bool data_interest = true,
			    bool parm_interest = true,
			    bool flag_interest = true) const;

    int			 getNumExtraInputs() const;

    void		 dumpExtraInputs(
			    std::ostream &os,
			    bool as_DOT,
			    int indent_level = 0) const;

    // addExprOpDependency is a helper function for building dependencies for
    // the expression library callback functions.
    static void		 addExprOpDependency(const char *arg_str,
					     const PRM_RefId &ref_id,
					     OP_InterestType interest_type);
    static void		 addExprOpDependency1From2(const char *arg_str1,
					     const char *arg_str2,
					     const PRM_RefId &ref_id,
					     OP_InterestType interest_type);
    static void		 addExprOpParmDependency(const char *arg_str,
						 const PRM_RefId &ref_id,
						 OP_InterestType interest_type);
    // NB: is reponsible for freeing new_arg!
    static void		 changeExprOpRef(const char *arg_str,
					 char *&new_arg,
					 const char *new_fullpath,
					 const char *old_fullpath,
					 const char *old_cwd);
    // NB: is reponsible for freeing new_arg!
    static void		 changeExprOpRef1From2(const char *arg_str1,
					 const char *arg_str2,
					 char *&new_arg1,
					 char *&new_arg2,
					 const char *new_fullpath,
					 const char *old_fullpath,
					 const char *old_cwd);
    // NB: is reponsible for freeing new_arg!
    static void		 changeExprOpParmRef(const char *arg_str,
					     char *&new_arg,
					     const char *new_fullpath,
					     const char *old_fullpath,
					     const char *old_cwd,
					     const char *chan_name,
					     const char *old_chan_name);

    void		 addGenericOpNameReference( const UT_String &oppath );
    void		 addGenericOpNameReference( const PRM_RefId &ref_id,
						    OP_Node *node );
    void		 addGenericOpInputReference( const PRM_RefId &ref_id,
						    OP_Node *node );

    static void		 moveAndUpdateDependencies( 
			    const OP_NodeList &src_nodes,
			    const OP_NodeList &dest_nodes );

    static void          updateChannelPtrs( CH_CollectionList &parents );

    bool		 getParmBaseNodePath(
			    UT_String &path,
			    int parm_index,
			    int thread) const;

    // Make this function public so it can be called by OPUI_EditSubnet
    // and possibly others. Note that it is defined in PRM_ParmOwner so
    // that it can be called from the PRM library.
    virtual void	 rebuildParmDependency( int parm_index );

    // Clear op name dependencies
    virtual void	 clearParmDependency( int parm_index );
    // decrementOpReference() should only be called by clearParmDependency()
    void		 decrementOpReference(int node_id);

    virtual void	 dumpDependencies(std::ostream &os);
    void		 dumpOpDependents(OP_Node *ref, int brief, 
        		                  std::ostream &os);
    const OP_DependencyList
			&getOpDependents() const { return myOpDependents; }


    // INTERNAL: Do NOT use outside of OP_Node! Will be made private.
    int			 countReferences(int op_id);

    // Get the existing node *NAME* references (eg. expr references)
    // NB: This function assumes that refs is sorted without duplicate entries!
    void		 getExistingOpReferences(OP_NodeList &refs,
						 bool recurse) const;

    // Get the as complete list of data references as possible for this node.
    // This combines getExistingOpReferences() which exist without cooking, and
    // getExtraInputNodes() which are the current references since the last
    // cook of its refernces.
    void		 getFullOpReferences(OP_NodeList &refs,
					     bool include_descendants) const;

    // Get the existing node *NAME* dependents (eg. expr references)
    // NB: This function assumes that deps is sorted without duplicate entries!
    void		 getExistingOpDependents(OP_NodeList &deps,
					     bool include_descendants) const;

    // Get the as complete list of data dependents as possible for this node.
    // This combines getExistingOpDependents() which exist without cooking, and
    // getExtraOutputNodes() which are the current dependents since the last
    // cook.
    void		 getFullOpDependents(OP_NodeList &deps,
					     bool include_descendants) const;

    // Gets a list of parms that reference one of our channels/parms.
    void		 getParmsThatReference(const char *channame,
					       UT_ValArray<PRM_Parm *> &parms,
					       UT_IntArray &parmsubidxs);

    virtual void	 inputConnectChanged(int which);

    // Returns the number of output entries in the output array. Note 
    // that this done regardless of which entries have any outputs connected.
    unsigned		 nOutputEntries() const;
    // Get access to the raw network items connected to our outputs.
    // These functions can focus on a single output connector, or treat
    // all outputs as a flat array by passign output_idx = -1.
    int			 nOutputItems(int output_idx = -1) const;
    OP_NetworkBoxItem	*getOutputItem(int idx, int output_idx = -1) const;
    // Quick check if this node has any node outputs.
    bool		 hasAnyOutputNodes(bool through_dots = true,
				int output_idx = -1) const;
    // Fills a node list with all outputs from this node. This function can
    // search through dots, and dive into subnets. It can also run on a
    // single output connector or all of them.
    void		 getOutputNodes(UT_Array<OP_Node*> &outputs,
				bool into_subnets = false,
				bool through_dots = true,
				int output_idx = -1) const;
    // Quick function to return the first output node, searching depth first
    // through dots if requested.
    OP_Node		*getFirstOutputNode(bool through_dots = true,
				int output_idx = -1) const;

    // obtains an output node that leads to a target node (e.g., during network
    // traversal). Null if target is not a descendent. 
    OP_Node		*getOutputTowardsNode(const OP_Node *target);

    /// Fill in 'tree' with details that are common to all node types. Then call
    /// fillInfoTreeNodeSpecific() to get specific details about this specific
    /// node (be it a SOP, CHOP, etc)
    void	 	 fillInfoTree(UT_InfoTree &tree,
				const OP_NodeInfoTreeParms &parms);

    /// Triggered by 'fillInfoTree()', this virtual function adds all node-
    /// specific info to 'tree'. All child classes must create their own 
    /// branches under 'tree' to place their info
    virtual void 	 fillInfoTreeNodeSpecific(UT_InfoTree &tree,
				const OP_NodeInfoTreeParms &parms);

    /// Optionally called on parent from fillInfoTreeNodeSpecific() to provide
    /// some information about node that only parent network can provide.
    /// @return True if parent added info specific to that child.
    virtual bool 	 fillInfoTreeChildSpecific(UT_InfoTree &tree,
				const OP_NodeInfoTreeParms &parms,
				OP_Node *child);

    /// This method has to do with 'fillInfoTree()' above. When the call to 
    /// fillInfoTree() completes, the UT_InfoTree 'tree' will be parsed and 
    /// displayed in a dialog box by OPUI_OpInfoDialog, at which point, that 
    /// class will want to ask us - which branch of the tree would you like to
    /// be selected by default when the dialog is displayed for the first time?
    virtual const char	*getDefaultSelectedInfoTreeBranchName() const;
    
    //  These methods get input data for the OP.  If they return an error code
    //  >= UT_ERROR abort, then the errno should be set, and there may be a
    //  further local error message.  There is no error checking to ensure that
    //  each locked source is only released once, so it's up to the user.  The
    //  standard method is to call lockInputs(), then call unlockInputs() on
    //  exit.  However, in some cases (i.e. switch SOP) only some sources need
    //  to be cooked.
    //
    //  lockInput() will return an error code >= UT_ERROR iff idx <
    //  minInputs().  Otherwise, it's considered to be an optional input, so no
    //  data will be returned.  A simple rule of thumb:  If lockInput() returns
    //  ok, the source has to be unlocked, otherwise it shouldn't be unlocked!
    //  If lockInputs() detects an error, it will automatically unlock the
    //  inputs so that unlockInputs() should NOT be called!
    virtual OP_ERROR	   lockInput(unsigned idx, OP_Context &context);
    virtual void	 unlockInput(unsigned idx);
    virtual OP_ERROR	   lockInputs(OP_Context &context);
    virtual void	 unlockInputs(void);

    //  This "serial" number refers to the number of times that the geometry
    //  had been changed.  Note, only the "modeller" should bump the cook count,
    //  the OP cook method does this also, but has privilaged access.
    int			 getCookCount(void) const { return myCookCount; }
    void		 triggerOutputChanged();
    
    void		 triggerUIChanged(
				OP_UIChangeType type = OP_UICHANGE_ANY);

    //  Returns the index of cooked input to a SOP switch
    virtual int		 cookedInputIndex() const { return -1; }

    /// For nodes which simply pass-through their data from some other node,
    /// this returns the node whose data will used transparently as this node's
    /// output. This function can give different values depending on its
    /// parameters. Used by node traversal algorithms.
    ///
    /// The default implementation returns the first input if it's bypassed, so
    /// subclasses should only do something different if this function returns
    /// NULL.
    ///
    /// @param t	    Eval time
    /// @param mark_used    Mark this node is using the returned node (if
    ///                     non-NULL).
    /// 
    virtual OP_Node	*getPassThroughNode(
			    fpreal t,
			    bool mark_used = false);

    bool		 setMinimumCacheSize(int min_size);

    OP_VERSION		 getVersionParms() const
			    { return dataMicroNodeConst().modVersion(); }

    // This will be called if the timeInterest flag is set.
    // This will return true if the node needs to be dirtied for cooking
    virtual bool	 handleTimeChange(fpreal /* t */) { return false; }

    /// Returns true if the OP depends on time at the given time.  This
    /// is sensitive to handleTimeChange's overrides.
    bool		 isTimeDependent(OP_Context &context);

    static int		 getGlobbedNodes(OP_Node *cwd, UT_String &holder,
				const char *pattern, UT_WorkArgs &argv,
				OP_GlobContext *context);
    
    static int		 getGlobbedNetworkBoxes(OP_Node *cwd, UT_String &holder,
				const char *pattern, UT_WorkArgs &argv,
				OP_GlobContext *context);

    static int		 getGlobbedPostIts(OP_Node *cwd, UT_String &holder,
				const char *pattern, UT_WorkArgs &argv,
				OP_GlobContext *context);

    
    /// Obtains additional nodes that should be copied or deleted when this
    /// node is copied or deleted. It applies to situations, such as, where
    /// an hidden input node feeds only to this node and serves only that
    /// purpose, so it should be coppied/deleted along with the master. 
    virtual void	 getExtraNodesForCopyOrDelete(OP_NodeList &extras)const;

    // Runs the creation script (if there is one). The return value is false
    // if the node was deleted while running the script. In this case
    // obviously the node pointer becomes invalid and should not be used
    // any more. It returns true if the node still exists.
    virtual bool	 runCreateScript();
    // Go through all our parms to match the values to the current hip file
    // units settings.
    void		 updateParmsToMatchCurrentUnits();

    /// Matches the node to the current HDA definition, loading the contents
    /// section if ncecessary and setting the match flag.
    virtual void	 matchOTLDefinition();

    /// Unmatches (unlocks) the node from the current HDA definition.
    virtual void	 unmatchOTLDefinition();

    /// Method that is invoked when the HDA index file has been cleared
    /// because the underlying HDA definition has changed.
    virtual void	 handleOTLIndexFileCleared();

    /// Returns the error string where we keep all errors generated during
    /// our most recent synchronization attempt.
    const UT_String	&getSyncErrors() const
			 { return mySyncErrors; }
    /// Returns the error level generated in our last synchronization attempt.
    OP_ERROR		 getSyncErrorLevel() const
			 { return mySyncErrorLevel; }

    /// Interface to flag that specifies whether or not we match our definition.
    int			 getMatchesOTLDefinition() const;

    /// Utility function for syncing/unsyncing all uncestor nodes.
    static void		 propagateMatchOTLDefinitionToAncestors(OP_Node *node,
						  bool sync_flag);

    // Some extra functions for dealing with access rights as they relate
    // to matching the OTL definition.
    int			 getCanDeviateFromOTLDefinition() const;
    void		 setAllChildAssetPermissions(int permission);
    // Recompute the asset permissions because something changed like the node
    // was locked or unlocked.
    void		 computeAllChildAssetPermissions();
    // Returns true if this node is contained inside a locked HDA, but
    // is marked as an editable node within that HDA.
    bool		 getIsEditableAssetSubNode() const;

    // Turn off our export flag and the export flags of our children.
    void		 turnOffAllChildExports(void);

    // Collection of methods for working with licensed assets.
    bool		 haveLicenseToAccessContents(
				OP_Node* opt_specific_child = nullptr) const;
    bool		 haveLicenseToAccessParentContents() const;
    bool		 haveAncestorWithoutFullLicense(bool ignore_self) const;
    OP_OTLLicenseType	 getLicense() const;
    // Get the first (oldest) ancestor without a license to access contents.
    OP_Node		*getFirstExecLicenseOnlyAncestor(
				bool ignore_self = true) const;
    // Get the last (youngest) ancestor without a license to access contents.
    OP_Node		*getLastExecLicenseOnlyAncestor(
				bool ignore_self = true) const;

    bool		 isCompiled() const;

    // Save as a series of ASCII commands.
    void		 saveWires(std::ostream &os, const char *name, 
	    			int dogeneral = 0);
    void		 saveOutputWires(std::ostream &os, const char *name, 
	    			int dogeneral = 0);
    virtual void	 saveIntrinsicCommand(std::ostream &os, 
                	        const char *name);
    
    /// Writes to 'os' a string of hscript commands to recreate this node.
    virtual OP_ERROR	 saveCommand(std::ostream &os,
				     const char *name,
				     fpreal x,
				     fpreal y,
				     const char *netboxname,
				     const OP_SaveCommandOptions &options);

    // This method saves a chlock style command for this node's parameters.
    // name        - the name of the node to print on the command;
    // command     - the name of the command;
    // flag_method - a PRM_Parm method that can be queried for the flag to
    //		     output;
    // defaultstoo - prints a default "-*" or "+*" as the first parameter
    // reverse     - normally the default is "-*" followed by the list of
    //		     parameters that result true for flag_method().  If 
    //               reverse is true, then the default is "+*" followed by
    //		     the parameters where flag_method() is false.  Only useful
    //		     when defaultstoo is true.
    // parm_array  - an optional array of parameters whose values should be
    //		     printed.  If this array is NULL, then the entire node
    //		     is searched.
    void		 saveParameterFlags(std::ostream &os, const char *name,
				     const char *command,
				     bool (PRM_Parm::*flag_method)(int) const,
				     bool defaultstoo, bool reverse,
				     UT_Array<opParmData> *parm_array);

    void		 saveUserDataCommand(
				     std::ostream &os, const char *node_name,
				     bool omit_version = false);

    enum RampError
    {
	RAMP_ERR_SUCCESS,
	RAMP_ERR_NOT_FOUND,
	RAMP_ERR_ONLY_KEY
    };
    virtual RampError	 rampCommand(bool remove, fpreal pos, float rgba[4]);
    RampError		 rampCommand(bool remove, fpreal pos, float rgba[4],
				     UT_Ramp &ramp);

    virtual bool	 loadRamp(UT_IStream &is, const char *path=0);

    bool		 loadRamp(UT_IStream &is, UT_Ramp &ramp,
				  const char *path=0);
    OP_ERROR		 saveRamp(std::ostream &os, UT_Ramp &ramp,
				  const char *name = 0, int command = 0);

    bool		 getUserData(const UT_StringRef &key,
				UT_StringHolder &result) const;
    void		 setUserData(const UT_StringRef &key,
				const UT_StringHolder &data,
				bool save_undo);
    bool		 hasUserData(const UT_StringRef &key) const;
    bool		 deleteUserData(const UT_StringRef &key,
				bool save_undo);
    const UT_Options	&userData() const
			 { return myUserData; }
    bool		 loadUserData(UT_IStream &is, const char *path=nullptr);
    OP_ERROR		 saveUserData(std::ostream &os, const char *path);
    void		 clearUserData(bool save_undo);
    virtual void	 userDataChanged(const UT_StringHolder &key);

    /// Prefix to use for internally used user data.
    static const char	*internalUserDataPrefix() { return "sidefx::"; }
    
    /// Returns a shared pointer to a previously stored blind data. If no
    /// blind data exists with the given key, an empty shared pointer is
    /// returned.
    UT_SharedPtr<void>	 getBlindData(const char *key) const;

    /// Return a pointer to a previously stored blind data, statically cast
    /// to the type given. If the cast fails, or if there's no data of that
    /// type, a NULL pointer is returned. To distinguish between the two cases
    /// call hasBlindData().
    template<typename T>
    UT_SharedPtr<T> 	 getBlindData(const char *key) const
    { return UTstatic_pointer_cast<T>(getBlindData(key)); }

    /// Stores a blind data pointer on this OP_Node object. Any entry that
    /// previously existed will be erased, and, if it is the last reference,
    /// the object itself deleted. The \c void type can convert from any other
    /// shared pointer type, while storing the deleter function, allowing
    /// OP_Node to delete the data without knowing anything about the class.
    /// Blind data is *not* stored with the class.
    void		 setBlindData(const char *key, UT_SharedPtr<void> ptr);

    /// Returns true if this object stores a blind data with the given key.
    bool		 hasBlindData(const char *key) const;

    /// Remove any reference to the blind data of the given key from this 
    /// object.
    bool		 deleteBlindData(const char *key);

    void		 getDataBlockKeys(UT_StringArray &keys,
				const UT_StringHolder &type =
				    UT_StringHolder()) const;
    OP_DataBlockPtr	 getDataBlock(const char *key) const;
    void		 setDataBlock(const char *key, OP_DataBlockPtr ptr);
    OP_ERROR		 saveDataBlocksPacket(std::ostream &os,
				const char *path_prefix,
				const UT_String &node_name);
    bool		 loadDataBlocks(UT_IStream &is, const char *path);
    virtual void	 dataBlockChanged(const UT_StringHolder &key);

    // deleteNodeData is called when the OP_Cache wants to delete some data.
    // This method should return 1 if you want change events to be propagated
    // to the extra inputs when data is deleted from the cache.
    virtual int		 deleteNodeData(void *);
    void		 propagateNodeDataDeleted();

    // getCachedPythonObject returns NULL if no object exists with the given
    // key.
    PY_OpaqueObject	*getCachedPythonObject(const char *key) const;
    // Call setCachedPythonObject with a PyObject * cast to a void *, not with
    // a PY_OpaqueObject * cast to a void *.  The pointer must not be null.
    void		 setCachedPythonObject(
			    const char *key, void *opaque_py_object);
    // deleteCachedPythonObject returns false if no value exists with that key.
    bool		 deleteCachedPythonObject(const char *key);
    // cachedPythonObjects returns a UT_SymbolMap containing pointers to 
    // PY_OpaqueObjects.  The pointers are not null.
    const OP_PythonObjectCache &cachedPythonObjects() const
			    { return myCachedPythonObjects; }

    bool		 loadExtraInputs(UT_IStream &is, const char *path=0);

    void		 saveOverrides(std::ostream &os);
    void		 saveOverrides(std::ostream &os, OP_Node *root,
				       const UT_String &rootpath,
				       bool &added);
    void		 saveChildOverrides(std::ostream &os, OP_Node *root,
					    const UT_String &rootpath,
					    bool &added);
    bool		 loadOverride(UT_IStream &is);
    virtual UT_ErrorSeverity
			 prmCookOverrides(fpreal t, int thread,
					  const char *parm_name, int vec_idx);
    virtual bool	 isParmPendingOverride(const char *parm_name,
					       int vec_idx) const;
    OP_ERROR		 cookOverrides(OP_Context &c);

    virtual void	 removeOverrideDestination(OP_Node * /*dest*/) {}
    virtual void	 removeOverrideDestination(OP_Node * /*dest*/,
						   int /*parm_idx*/) {}

    // Overriding virtual function in PRM_ParmOwner to run a command.
    virtual void	 executeCommand(const char *command,
                	                std::ostream *out,
                	                std::ostream *err) const;
    virtual void	 pushAsPwdAndRunPython(
			    PY_CompiledCode &compiled_code,
			    PY_Result::Type desired_result_type,
			    PY_Result &result,
			    PY_EvaluationContext *context=nullptr) const;

    OP_ERROR		 executeHscriptScript(UT_String &script,
				const OP_Context &context);
    OP_ERROR		 executePythonScript(UT_String &script,
				const OP_Context &context);

    void		 addPythonNodeError(const PY_Result &py_result);
    
    void		 saveInputs(std::ostream &os, bool compile_basic);
    bool		 loadInputs(UT_IStream &is, const char *path=0,
				bool named = false);
    void		 saveNamedInputs(std::ostream &os, bool compile_basic);
    void		 saveNamedOutputs(std::ostream &os);
    bool		 loadNamedOutputs(UT_IStream &is, const char *path=0);

    // These methods provide access to the options that can be stored on each
    // input of a node, if the node type enables this feature.
    void		 getEditableInputDataKeys(int idx,
				UT_StringArray &keys) const;
    const UT_OptionEntry*getEditableInputData(int idx,
				const UT_StringRef &key) const;
    void		 setEditableInputData(int idx, 
				const UT_StringHolder &key,
				const UT_OptionEntry &value);
    void		 saveEditableInputData(std::ostream &os) const;
    bool		 loadEditableInputData(UT_IStream &is,
				const char *path);
    // Special case functions for getting and setting the input "name", which
    // just call the generic InputData functions.
    UT_StringHolder	 getEditableInputString(int idx,
				const UT_StringRef &key) const;
    void		 setEditableInputString(int idx,
				const UT_StringRef &key,
				const UT_StringHolder &str);

    /// @{ If a subclass is interested in the saved input or output names, then
    /// it should  override this method. The array will be filled during load.
    virtual UT_StringArray *	getArrayForLoadedInputNames();
    virtual UT_StringArray *	getArrayForLoadedOutputNames();
    /// @}

    void		 checkInputs();	// this method will check the
    					// node references in all the inputs

    // Saves out extra information for this node to be included in a dialog
    // script that defines an operator type created from this one.
    virtual void	 saveDialogScriptExtraInfo(std::ostream &os);

    // Fills in the supplied default gallery entry with info from this node.
    virtual void	 createGalleryEntry(OP_GalleryEntry &entry);

    // When a string parameter can have the syntax: 'op:/...', use these
    // evaulate methods to do the approprate checking and reference building.
    // These methods will return a reference to the OP_Node if one is
    // referenced.  If the syntax is something like:
    //		op:/foo/bar[32]
    // Then the optime will be (32-1)/$FPS rather than the evaluation time.
    void		 evalOpPathString(UT_String &val, int pi, int vi,
				    fpreal t, int &op_id, fpreal &op_time,
				    int expanded = 1);
    void		 evalOpPathString(UT_String &val, const char *pn,
				    int &pi, int vi, fpreal t,
				    int &op_id, fpreal &op_time,
				    int expanded=1);
    static int		 findOpFramePair(const char *path, int &op_id,
				    fpreal &frame);

    /// Build a UT_XformOrder out of TRS and XYZ parameter values
    static void		 buildXformOrder(int trs, int xyz,
				UT_XformOrder &order);

    /// Translate a TRS parameter menu index into the UT_XformOrder type
    static UT_XformOrder::rstOrder  getMainOrder( int trs );
    /// Translate a UT_XformOrder type into a TRS parameter menu index
    /// (inverse of getMainOrder())
    static int		 getMainOrderMenuIndex(UT_XformOrder::rstOrder order);
    /// Translate a XYZ parameter menu index into the UT_XformOrder type
    static UT_XformOrder::xyzOrder  getRotOrder( int xyz );
    /// Translate a UT_XformOrder type into a XYZ parameter menu index
    /// (inverse of getRotOrder())
    static int		 getRotOrderMenuIndex(UT_XformOrder::xyzOrder order);

    /// NB: None of the getXformPivot() or getPivotParmValue() methods take
    ///     any pivot compensation transform into account.  It is up to the
    ///     caller to account for any such transform.
    static UT_Vector3R	 getXformPivot(int trs,
				  fpreal tx, fpreal ty, fpreal tz,
				  fpreal px, fpreal py, fpreal pz);
    static UT_Vector3R	 getXformPivot(int trs,
				  fpreal tx, fpreal ty, fpreal tz,
				  fpreal px, fpreal py, fpreal pz,
				  fpreal pivot_rx, fpreal pivot_ry,
				  fpreal pivot_rz, bool inverse = false);
    static UT_Vector3R	 getPivotParmValue(int trs,
				  fpreal tx, fpreal ty, fpreal tz,
				  fpreal px, fpreal py, fpreal pz);
    static UT_Vector3R	 getPivotParmValue(int trs,
				  fpreal tx, fpreal ty, fpreal tz,
				  fpreal px, fpreal py, fpreal pz,
				  fpreal pivot_rx, fpreal pivot_ry,
				  fpreal pivot_rz, bool inverse = false);

    static void		 buildXform(int trs, int xyz,
				float tx, float ty, float tz,
				float rx, float ry, float rz,
				float sx, float sy, float sz,
				float px, float py, float pz,
				UT_Matrix4 &mat);
    static void		 buildXform(int trs, int xyz,
				double tx, double ty, double tz,
				double rx, double ry, double rz,
				double sx, double sy, double sz,
				double px, double py, double pz,
				UT_DMatrix4 &mat);
    static void		 buildXform(int trs, int xyz,
				float tx, float ty, float tz,
				float rx, float ry, float rz,
				float sx, float sy, float sz,
				const UT_Matrix4F::PivotSpace &pivot,
				UT_Matrix4 &mat);
    static void		 buildXform(int trs, int xyz,
				double tx, double ty, double tz,
				double rx, double ry, double rz,
				double sx, double sy, double sz,
				const UT_DMatrix4::PivotSpace &pivot,
				UT_DMatrix4 &mat);
    static void		 buildXform(int trs, int xyz,
				float tx, float ty, float tz,
				float rx, float ry, float rz,
				float sx, float sy, float sz,
				float s_xy, float s_xz, float s_yz,
				float px, float py, float pz,
				UT_Matrix4 &mat);
    static void		 buildXform(int trs, int xyz,
				double tx, double ty, double tz,
				double rx, double ry, double rz,
				double sx, double sy, double sz,
				double s_xy, double s_xz, double s_yz,
				double px, double py, double pz,
				UT_DMatrix4 &mat);
    static void		 buildXform(int trs, int xyz,
				float tx, float ty, float tz,
				float rx, float ry, float rz,
				float sx, float sy, float sz,
				float s_xy, float s_xz, float s_yz,
				const UT_Matrix4F::PivotSpace &pivot,
				UT_Matrix4 &mat);
    static void		 buildXform(int trs, int xyz,
				double tx, double ty, double tz,
				double rx, double ry, double rz,
				double sx, double sy, double sz,
				double s_xy, double s_xz, double s_yz,
				const UT_DMatrix4::PivotSpace &pivot,
				UT_DMatrix4 &mat);
    static void		 buildXform(
				const UT_Matrix4F::FullTransformModel &parms,
				UT_Matrix4F &mat);
    static void		 buildXform(
				const UT_Matrix4D::FullTransformModel &parms,
				UT_Matrix4D &mat);
    static void		 buildXform(int trs, 
				float tx, float ty, float rz,
				float sx, float sy, float px, float py,
				UT_Matrix3 &mat);
    static void		 buildXform(int trs, 
				double tx, double ty, double rz,
				double sx, double sy, double px, double py,
				UT_DMatrix3 &mat);
    static void		 buildXform(int trs, 
				float tx, float ty, float rz,
				float sx, float sy, float s_xy,
				float px, float py,
				UT_Matrix3 &mat);
    static void		 buildXform(int trs, 
				double tx, double ty, double rz,
				double sx, double sy, double s_xy,
				double px, double py,
				UT_DMatrix3 &mat);

    virtual int		 getTranslateParmIndex();

    // This is done only after a load to make sure that
    // all inputs have been resolved correctly.
    void		 resolveInputReferences();

    static void		 clearAllPendingUndoFlags();

    virtual void	 clearUndoFlags();
    virtual void	 setAnyUndoFlag();
    void		 saveForUndoInput();

    void		 blockModify(int on_off, int propagate = 1);
    int			 isBlockModify(void) const;
    bool		 isModifyWaiting() const;
    virtual void	 propagateEndBlockModify();

    // Only returns anything in the case of chops
    virtual const CL_Clip *getClip(OP_Context *context = 0);

    /// Returns the a chop network path suitable for storing motion effects
    /// associated with this node.
    void		 getMotionEffectsNetworkPath(UT_String &path);

    /// Return the cook information as a formatted string.
    void		 getCookInfo(UT_String &info) const;
    /// Return the cook information in a UT_InfoTree.
    void		 getCookInfoTree(UT_InfoTree &tree) const;

    // Query color for Net Overview rendering
    virtual int		 getNetOverviewColor( UT_Color &color );
    
    // Those operators which need a ramp editor must define this method.
    virtual UT_Ramp	*getRamp();

    virtual int		 isCookingRender() const { return 0; }
    virtual void	 setCookingRender(int val);

    virtual void	 clearInterrupted()
			    { clearInterruptedImpl(/*allow_recook*/true); }
    int			 wasInterrupted() const
			    { return flags().getInterrupted(); }

    // work-around to display warning message when export src or destination
    // are renamed.  OP_Node::cook clears the error messages when a node export
    // changes so we had to save the fact that a rename conflict is pending
    void		setRenameConflict()
    			{ myRenameConflict = 1; }
    void		setRunningCreateScript(int onoff);

    // Base class methods to output code to a file or a stream:
    virtual int		outputCode(OP_OutputCodeParms &, OP_Context &);

    virtual fpreal	getTimeTransform(int input, fpreal t);
    virtual void	getInputRes(int input, fpreal t,
				    const OP_Context &context,
				    OP_Context &input_context);

    // Utility methods:
    UT_TokenString &	 getHashCode(OP_Context &context, int group_mask =1);
    virtual UT_TokenString & getParmHashCode(OP_Context &context,
					     int group_mask = 1);

    virtual int		getNumInputsToHash();
    virtual OP_Node    *getInputToHash(int i);
    
    void		clearHashCodeFlags();
    bool		isHashCodeBuilt() const { return myBuiltHashCode;}
    void		builtHashCode(bool b = true) { myBuiltHashCode = b; }
    void		buildInputHashCode(UT_TokenString &string,
					   OP_Context &context,
					   int group_mask,
					   int level);

    // If an interactive state was used to create this node, set it here.
    void		 setCreatorState(const char *s)
			 { myCreatorState.harden(s); }
    const UT_String	&getCreatorState() const   { return myCreatorState;    }
    void		 builtExplicitly(int yn)   { myBuiltExplicitly = yn;   }
    int			 wasBuiltExplicitly() const{ return myBuiltExplicitly; }

    bool		 matchesState(const char *state) const;

    /// Some nodes (VOPs and SHOPs) represent shaders, so this method
    /// returns node parameters that are used as shader parameters.
    virtual const PRM_Template	*getShaderParmTemplates();

    // Don't even think about calling changeParmTemplate if you aren't
    // OP_Operator.
    virtual void	 changeParmTemplate(PRM_Template *new_template);

    // Deletes any input or output connections in excess of maximum allowed.
    virtual void	 ensureInputsAndOutputsAreValid();

    // Connect the node to an input.  If branch_off is false and the input node
    // currently has outputs, the node will be inserted before the outputs.
    void		 connectToInputNode(OP_Node &inputnode, int input_idx,
					    int branch_off=0);

    // This method should be called when the user edits an overridden channel
    virtual int                  editCallback(CL_Track *track, fpreal t,
					      fpreal newValue);

    // This mechanism allows users to communicate editing changes
    static OP_EditCallback	 getEditCallback(void *&data);
    //static void			 setEditCallback(OP_EditCallback func,
	    					 //void *data);

    // These build an xform matrix based upon the given parameters.
    // The axis is the axis around which to rotate by 90 degrees.  If
    // it is lower case, it will be rotated by +90, if it is upper case,
    // by -90.  To not rotate at all, use '-'.
    static void		 buildQuadricXform(UT_Matrix4 &mat,
				float tx, float ty, float tz,
				float sx, float sy, float sz,
				char axis = 'y',
				float rx = 0.0f, float ry = 0.0f,
				float rz = 0.0f);

    static void		 buildQuadricXform(UT_DMatrix4 &mat,
				double tx, double ty, double tz,
				double sx, double sy, double sz,
				char axis = 'y',
				double rx = 0.0, double ry = 0.0,
				double rz = 0.0);

    static int		 buildOpMenu(OP_Network *net, PRM_Name *theMenu,
				int theMenuSize, int (*doAdd)(OP_Node *) = 0,
				int startItem = 0, const PRM_Parm *parm = 0,
				int showSubnets = 1, int expandSubnets = 1,
				bool recurse = false);

    // Build the "Predefined Rules" menu that is common to several OPs
    // including Event and Group POPs. This is built from the text file table
    // OP<name>Rules, where name is the OP type (ie Event).
    static void		 buildPreDefRulesMenu(PRM_Name *menu,
					      OP_PreDefRules &pdr);

    // Allows operators to use their own local table for variables and build
    // structures on demand...  Implement the syntax highlight version only
    // if you need to resolve variables for syntax highlighting that are not
    // normally resolved by the generic resolveVariable() method.
    virtual const CH_LocalVariable *resolveVariable(const char *name);
    virtual const CH_LocalVariable *resolveExtraVariableForSyntaxHighlight(
							    const char *name);

    // This value bumps whenever nodes are created/deleted/renamed.
    static int		 getNameSerialIndex();

    // export mapping techniques
    virtual void	setMapping(int idx, int type, const char *label = 0);
    virtual int		getMapping(int idx, const char *&label) const;

    // drag-n-drop receive methods
    virtual int         acceptDragDrop(DD_Source &src, const char *label);
    virtual int         testDragDrop(DD_Source &src);
    virtual void        getDragDropChoice(DD_Source &src, DD_ChoiceList &c);

    // These methods are used to give quick overview information about a node
    bool		hasComment() const;
    bool		hasParmsNotAtFactoryDefault() const;
    bool		hasAnimatedParms() const;
    bool		hasChopOverriddenParms() const;

    /// traverseInputs calls 'callback' on this node and all its inputs.
    /// If callback returns true for any node, it will stop and return
    /// true, otherwise it will return false;
    /// @param callback		    Callback function
    /// @param data		    User data passed to the callback function
    ///	@param follow_subnets	    If input is a subnet, we continue
    ///				    traversing from its display node.
    ///	@param follow_ref_inputs    Follow inputs marked as reference inputs
    ///	@param follow_extra_inputs  Follow nodes which this node implicitly
    ///				    depends on.
    /// @param extra_interest_mask  For extra inputs, this mask allows one to
    ///				    restrict which of those dependency types
    ///				    follow.
    //@{
    bool		traverseInputs(bool (*callback)(OP_Node &, void *),
				       void *data,
				       bool follow_subnets,
				       bool follow_ref_inputs,
				       bool follow_extra_inputs,
				       bool follow_passthrough_inputs,
				       OP_InterestType extra_interest_mask =
						OP_INTEREST_ALL);
    bool		traverseInputs(bool (*callback)(const OP_Node &, void*),
				       void *data,
				       bool follow_subnets,
				       bool follow_ref_inputs,
				       bool follow_extra_inputs,
				       bool follow_passthrough_inputs,
				       OP_InterestType extra_interest_mask =
						OP_INTEREST_ALL) const;
    //@}
    
    /// Traverses children of this node, calling 'callback' on all children.
    /// If callback returns true, the traversal stops at that node.
    bool		traverseChildren(bool (*callback)(OP_Node &, void*),
					 void *data,
					 bool recurse_into_subnets);

    // Very specialized to see if this node (and optionally, any input
    // ancestors are also cooking.
    bool		isCooking(bool include_ancestors) const;

    // This method returns true if the OP_Node makes use of its footprint
    // flag and false otherwise.
    virtual bool	 usesFootprint() const	{ return false; }

    void		 getExternalReferences(UT_StringArray &reflist,
					       UT_StringArray *nodelist = 0,
					       bool from_children_too = true,
					       bool collapse = false,
					       bool check_missing = false,
					       bool show_missing_only = false);
    virtual bool	 getSaveWithVopnets();
    virtual void	 runDelScript();
    /// Overriden in VOPs to remove any channel refrences we may have 
    /// to the node being deleted.
    virtual void	 preDelete() { }

    // Two functions for upcasting this node to a DOP_Parent. DOP_Parent
    // is a class which owns a simulation and has DOP_Nodes as children.
    virtual DOP_Parent		*castToDOPParent()		{ return 0; }
    virtual const DOP_Parent	*castToDOPParent() const	{ return 0; }

    // These provide a version which you can query without being above DOP.
    OP_DopParent	*castToOpDopParent()
			 { return (OP_DopParent *) castToDOPParent(); }
    const OP_DopParent	*castToOpDopParent() const
			 { return (const OP_DopParent *) castToDOPParent(); }

    // This function upcasts this node to a VOP_CodeGenerator.
    // VOP_CodeGenerator is a class which generates VEX code from the VOPs
    // contained inside the node.
    virtual VOP_CodeGenerator	*getVopCodeGenerator()		{ return 0; }
    virtual VOP_CodeGenerator	*getVopAutoCodeGenerator()	{ return 0; }

    // Return the manager that handles exported parameters from contained
    // Parameter VOPs.  Only certain kinds of VOP nodes can have such a manager
    // (i.e. Subnet VOPs, VOP HDAs).  Note that a node cannot have both a code
    // generator AND a parms manager because the code generator already has its
    // own parms manager.
    virtual VOP_ExportedParmsManager *getVopExportedParmsManager(
					bool create_if_needed=false)
					{ return nullptr; }

    /// @{
    /// Methods to postpone the update of the code generator until ending
    /// the last (level zero) update. This avoids recalculation of parameters
    /// and vop signatures, which can be quite costly.
    // TODO: remove these two virtuals in farour of getVopCodeGenerator()
    //       returning OP_VopCodeGenerator from which VOP_CodeGenerator
    //       derives and which has these two virtuals available at OP level.
    virtual int			 beginVopCodeGeneratorUpdate()	{ return 0; }
    virtual void		 endVopCodeGeneratorUpdate(int update_level) {}
    /// @}

    // Build a command line for a VEX script to be called with.  This does all
    // the channel evaluation and string expansion required.  If there is an
    // error, 0 is returned (otherwise 1).
    int			 buildVexCommand(UT_String &result,
					 const PRM_Template *templatelist,
					 fpreal now,
					 int start_parm = 0,
					 int end_parm = INT_MAX,
					 bool use_parmvop_tag=true
					 );
    int			 buildVexCommandWithDependencies(OP_Node *owner, 
					 DEP_MicroNode *depnode,
					 UT_String &result,
					 const PRM_Template *templatelist,
					 fpreal now,
					 int start_parm = 0,
					 int end_parm = INT_MAX,
					 bool use_parmvop_tag=true
					 );

    // Builds a VEX script using standardized parameter names.
    // "vexsrc": int for myself, shop, or script.
    // "script": raw vex script
    // "soppath": for the shop path.
    static PRM_ChoiceList	theVexSrcParmMenu;
    static PRM_Name		theVexSrcParmName;
    static PRM_Name		theVexScriptParmName;
    static PRM_Name		theVexNodePathParmName;
    static PRM_Name		theVexClearParmName;
    virtual void	 buildVexScript(UT_String &script, fpreal t,
				OP_Node *owner,
				UT_Map<int, bool> *visitlist = 0);
    virtual void	 buildVexScript(UT_String &script, fpreal t,
				DEP_MicroNode *depnode,
				UT_Map<int, bool> *visitlist = 0);


    // Returns true if vex shader has parameter by a given name.
    virtual bool	 hasVexShaderParameter(const char *parm_name);


    /// This function returns a string that encodes all the necessary
    /// information about a node-based material that helps the script-based
    /// materials to function properly. The string usually encodes info stored
    /// on child nodes, such as rendering properties or procedurals. Then
    /// the script node can use that info to provide necessary data
    /// for rendering.
    /// Returns true if the method provides usable information.
    virtual bool		getScriptMaterialInfo( UT_String & mat_info,
				    VOP_ScriptMaterialCodeMapper * mapper )
				{ return false; }

    /// Returns the sub type name, if the node's operator wants to be
    /// saved as an HDA in a special way (ie, other than the regular
    /// scripted operator handling). The sub type name is then used
    /// to distinguish how to handle the HDA creation.
    virtual const char	*getHDASubType()
				{ return 0; }

    // Set the channel alias for a channel (or parm subindex).  Returns true
    // if successful and false otherwise.
    bool		 setChannelAlias(PRM_Parm &parm, int subindex, 
					 const char *alias_name);

    void		 disconnectAllInputsOutputs(bool keepSelected, 
				bool forceDisconnectOutputs = false);

    void		 disconnectAllInputs(void);
    void		 disconnectAllOutputs(void);

    // Notify dependents that a parm name has changed.
    virtual void	 notifyParmRenameDependents( 
					const UT_String &chan_name,
					const UT_String &old_chan_name );

    void		 writeChannel(CH_Channel *channel, std::ostream &os,
				      const char *name, bool frames);
    void		 writeAllChannels(std::ostream &os, const char *name,
					  bool frames, bool dochblock,
					  bool dospareparms);

    // Run the callback function for a particular parm, and the callbacks
    // of any parms linked to this parm. Returns false if the node is
    // deleted while running the callback.
    virtual bool	 triggerParmCallback(PRM_Parm *parmptr, fpreal now,
					     int value, void *data,
					     const UT_Options *options=0);

    virtual int64	 getMemoryUsage(bool inclusive) const;
    int64		 getExtraInputMemoryUsage() const;
    static void		 printOpMemoryUsageStats(std::ostream &os);

    /// This method will unscope all the channels belonging to this op.
    /// If the recurse flag is set it will recurse through the children
    /// (at this level the recurse flag is ignored).
    virtual void	 unscopeChannels(bool recurse);

    /// This method will undisplay all the channels belonging to this op.
    /// If the recurse flag is set it will recurse through the children
    /// (at this level the recurse flag is ignored).
    virtual void	 undisplayChannels(bool recurse);

    /// This method will unpin all the channels belonging to this op.
    /// If the recurse flag is set it will recurse through the children
    /// (at this level the recurse flag is ignored).
    virtual void	 unpinChannels(bool recurse);

    /// This method takes a pattern string and will perform the ch_scope_op
    /// on each of our paramaeters that match the pattern.   This method
    /// primarily supports the chscope command.
    virtual void	 setChannelScope(const char *pattern, 
					 OP_ScopeOp scope_op,
					 const OP_ScopeOptions &scope_opts);

    /// A simple wrapper around getName() to allow us to abstract its 
    /// functionality up to OP_NetworkBoxItem.
    virtual const UT_String	&getItemName() const
				 { return getName(); };
    virtual bool		 setItemName(const UT_String &name);

    /// Caches the descriptive name to member data, and returns that. The
    /// cache is automatically dirtied in opChanged.
    const UT_StringHolder &getCachedDescriptiveName();

    // This function is called when our spare parm definition is changing.
    virtual bool	 changeSpareParms(UT_IStream &istream,
					  UT_String &errors);
    // This function is called during the changeSpareParms operation when
    // one of our parameters is being removed.
    virtual void	 spareParmRemoved(const char *parmname);
    
    // This is a secondary parm list where all obsolete parameters are
    // placed (if any)
    virtual PRM_ParmList    *createObsoleteParmList();
    virtual void	     resolveAndDeleteObsoleteParmList(
						PRM_ParmList *&obsolete_parms);

    virtual unsigned	 referenceAllParameters(OP_Parameters *from,
				bool relative_references = true);

    void		 clearVersionUserData();
    void		 updateVersionUserData();

    /// After the nodes parameter list is loaded, syncNodeVersionIfNeeded() is
    /// called to perform any necessary upgrades if the version number of
    /// the loaded node (taken from user data) is different from the node
    /// type's version number (getOperator()->getVersion()). Use the
    /// from_version parameter to override the node current version.
    /// If needed, this will call syncNodeVersion(). After synching, the node's
    /// version user data will be set to the current version if
    /// update_node_version is true.
    void		 syncNodeVersionIfNeeded(
				bool *node_deleted,
				const char *from_version = nullptr,
				bool update_node_version = false);

    /// Sync the node assuming that it is currently at old_version to the
    /// current_version. By default, this will call the corresponding
    /// OTL_SYNCNODEVERSION section if it exists.
    virtual void	 syncNodeVersion(
				const char *old_version,
				const char *current_version,
                                bool *node_deleted);

    // Sets the given stamping parameter.
    // Returns true if the param was changed. Note that we may return false
    // even if this was a valid operation as param might have already been
    // set to the given value.
    // If the warned variable is given, then we only warn if its contents
    // is false. Then in that case, we will set it to true when needed.
    bool		 setGlobalFloatParam(const char *param,
					fpreal value,
					bool *warned);
    bool		 setGlobalStringParam(const char *param,
					const char *strvalue,
					bool *warned);

    // This allows one to prevent chop cooking due to parameter modifications
    // until the outermost block is done.
    void		 beginPropagateModification();
    void		 endPropagateModification();

    // Add any OTL-defined operators used by this node to the specified table.
    virtual void	 getActiveOperatorsDefinedByOTL(
				UT_Set<OP_Operator *> &active_operators) const;

    // Returns true if we are in the middle of a call to sendBulkNotifications,
    // Or doing some other operation that involves sending out a whole bunch
    // of change notifications all at once. This allows VOP Networks to avoid
    // over-cooking.
    static bool		 getDoingBulkNotification();
    static bool		 isDirectorDoingBulkNotification();

    /// Returns whether multi-inputs on this particular op might have different
    /// names for each of the inputs, or if the name will always be the same for
    /// all the inputs.
    virtual bool	 hasDifferentMultiInputs() const;


    virtual bool	 canCreateNewOpType() const;
    static bool		 createNewOpType
			    (OP_Node *node,
			     UT_String &errors,
			     const char *typeName=nullptr,
			     const char *typeLabel=nullptr,
			     const char *otlFile=nullptr,
			     const char *metaSource=nullptr,
			     const char *comment=nullptr,
			     const char *version=nullptr,
			     const char *newName=nullptr,
			     int	 minimumInputs=-1,
			     int	 maximumInputs=-1,
			     bool ignore_external_references=false, 
			     bool compress_contents=false,
			     bool force=false,
			     int *newNodeId=nullptr,
			     bool compile_contents = false,
			     bool change_node_type = true,
			     bool create_backup = true);

    virtual UT_String	*getMaterialIconFilename() { return 0; }
    virtual void	 setMaterialIconFilename(const char * /*icon_file*/) {}
    virtual IMG_Raster	*getMaterialIconImage() { return 0; }
    virtual bool	 getMaterialIconAllowRegenerateFlag() { return true; }
    virtual void	 setMaterialIconAllowRegenerateFlag(bool) {}

    // Call these functions when making a lot of calls to opChanged on a
    // list of nodes. These functions make sure that VOP Networks only get
    // one change notification no matter how many child nodes are about
    // to send any number of change events.
    // @parm caller The node that starts the bulk notifications, if applicable.
    static void		 startBulkNotifications(
				const OP_NodeList &changed,
				OP_Node *caller = nullptr);
    static void		 endBulkNotifications(
				const OP_NodeList &changed);

    /// This is overriden in VOPs. It returns true if the input at input_idx
    /// will need *and* can perform autoconversion of its input type to the
    /// type it needs. If autoconversion is not needed, or is impossible,
    /// this will return false.
    virtual bool	 willAutoconvertInputType(int input_idx);

    bool		 isDependentOn(OP_Node* other_node, PRM_Parm* parm);

    // Gets whether the input at idx is collapsed.  Currently this is only used
    // in VOPs.  
    virtual bool	 getIsInputVisibleDefault(int idx);

    // Debugging function to confirm that all PRM_Template's are consistent
    // with their parameters.
    bool		 verifyTemplatesWithParameters() const;

    // Same as getInputReference, except using a named reference.
    OP_Input		*getNamedInputReference(
				const OP_ConnectorId& input_name, bool grow);
    OP_Input		*getNamedInputReferenceConst(
				const OP_ConnectorId& input_name) const;

    OP_Node		*getNamedInput(const OP_ConnectorId& input_name,
				bool mark_used=false) const;
    /// Note that this function only looks through currently connected
    /// inputs for the name, not through all possible input names on
    /// this node. mapInputNameToIndex does that instead.
    virtual int		 getInputFromName(const UT_String &in) const;
    virtual int		 getOutputFromName(const UT_String &out) const;
    virtual void	 getInputName(UT_String &in, int idx) const;
    virtual void	 getOutputName(UT_String &out, int idx) const;

    int			 getInputFromUniqueName(
				const OP_ConnectorId& id) const;
    int			 getOutputFromUniqueName(
				const OP_ConnectorId& id) const;

    // Returns the unique name of the input at index idx. Creates the input if
    // grow is set to true.
    void		 getUniqueInputName(
				OP_ConnectorId& id_out, int idx, bool grow);
    bool		 getUniqueInputNameConst(
				OP_ConnectorId& id_out, int idx) const;

    void		 getUniqueOutputName(OP_ConnectorId& id_out, int idx);
    bool		 getUniqueOutputNameConst(
				OP_ConnectorId& id_out, int idx) const;

    /// New input functions that use names instead of indices.
    virtual OP_ERROR	 setNamedInput(const OP_ConnectorId& input_name,
				OP_Node *op,
				const OP_ConnectorId* output_name = nullptr);

    virtual OP_ERROR	 setNamedIndirectInput(
				const OP_ConnectorId& input_name,
				OP_IndirectInput *input);
    virtual OP_ERROR	 setNamedInputReference(
				const OP_ConnectorId& input_name, 
				const char *label, int,
				const OP_ConnectorId* output_name = nullptr);

    OP_ERROR		 insertNamedInput(
				const OP_ConnectorId& input_name, OP_Node *op,
				const OP_ConnectorId*  output_name);
    OP_ERROR		 insertNamedIndirectInput(
				const OP_ConnectorId& input_name,
				OP_IndirectInput *input);

    /// Overriden in VOPs.
    virtual void	 onCreated(void) { }

    virtual bool	 isOrderedInput(const OP_ConnectorId& input_name) const;

    /// Returns the name of the first connection from node "who" to this node,
    /// or an empty string if "who" is not an input.
    OP_ConnectorId	 whichNamedInputIs(const OP_Node *who) const;

    /// Returns the index of the first connection of an indirect input.
    OP_ConnectorId	 whichNamedInputIs(const OP_IndirectInput *who) const;
    virtual bool	 willAutoconvertNamedInputType(
				const OP_ConnectorId& input_name);

    /// Note: Use these carefully, since they assume that no inputs are actually 
    /// added or removed (or renamed) during the traversal.
    OP_ConnectorInputIter traverseInputs(OP_ConnectorInputIter* prev_iter);
    OP_ConnectorInputIter traverseConnectedInputs(OP_ConnectorInputIter* prev_iter);
    OP_ConnectorInputIter getTraverseEndIterator(void);

    unsigned		 getInputsArraySize()
			 { return myInputs.entries(); }
    /// Deprecated in favour of getInputsArraySize()
    SYS_DEPRECATED(17.0) unsigned isInput(unsigned idx)
			 { return (getInputsArraySize() > idx) ? 1 : 0; }

    /// Retrieve a sorted list of local variables visible to this OP_Node.
    void		 getLocalVarNames(UT_StringArray &out_vars);

    // Forcibly enable evaluation of local variables outside of cooking code
    // paths so that the last cooked value can be used for things like
    // pivot values for handles. 
    bool		 setLocalVarActive(bool f)
			 {
			    bool old = myLocalVarActive;
			    myLocalVarActive = f;
			    return old;
			 }

    /// Get the myLocalVarActive flag
    bool		 isLocalVarActive() const
			 { return myLocalVarActive; }

    void		 recomputeCompiledHash();

    /// Clears the compiled hash (needed when switching from a compiled library
    /// definition to one that is not compiled).
    void                 clearCompiledHash()
                         { myCompHash = DEFAULT_COMP_HASH_VALUE; }

    /// You should use opChanged() instead. This is only for very special
    /// cases. If you don't know you are a special case, you aren't.
    void		 directSendEvent(OP_EventType type, void *data=0)
			 { sendEvent(type, data); }

    virtual bool	 scopedChannelsDirty();

    /// This mechanism allows users to evaluate active layer contribution
    /// without needing to link against CHOP.
    virtual bool	 getActiveLayerContribution(const UT_String &track_name,
				fpreal t, fpreal &value, fpreal &weight)
			 { return false; }

    /// Try to resolve exports on CHOP Channel Nodes from a value parameter
    /// within the array.
    virtual bool	 resolveExport(const PRM_Parm* p, int subindex,
				CH_ChannelRef& out_export )
			 { return false; }

    /// Creates or moves an existing visualizer in this network to
    /// this nodes output.
    virtual bool	 addOrMoveVisualizerToOutput(int outputidx)
			 { return false; }

    virtual const PRM_Parm *traverseRef(int *sub_idx, fpreal time,
				int parm_idx, int vec_idx) const;

    OP_Bundle *		 getParmBundle(const char* parm_name, int vector_index,
				UT_String &pattern, OP_Network *creator,
				const char *filter, bool subnet_inclusion);

    /// For node types with no other default color override, this is the
    /// final fallback default color for all nodes.
    static const UT_Color &getGlobalDefaultColor();

    /// The following methods are to be used to implement getW() and getH() in
    /// sub-classes.  They are in absolute, pan-and-scale independent UI units.

    /// Get the width of flags.
    static fpreal 	 getFlagWidth();
    /// Get the default node height.
    static fpreal 	 getNodeHeight();
    /// Get the default width of the "node button" (the part of the node 
    /// that you can drag and click on that isn't flags).
    static fpreal	 getNodeButtonWidth();
    /// Get the default connector height.
    static fpreal	 getConnectorHeight();

    /// Returns true if we are allowed to destroy the specified nodes. If
    /// nodes is non-null, we test those nodes. If nodes is null, we test
    /// the picked nodes of the src_net. The return value assumes that all
    /// specified nodes will be destroyed, which is important because it
    /// may be safe to delete node A iff node B is also going to be deleted.
    static bool		 canDestroyNodes(OP_Network *src_net,
					 const OP_NodeList *nodes);

    /// Add an error to the given error manager, collapsing chains
    /// of invalid sources.
    static void		 addInputNodeErrorToManager(UT_ErrorManager *error,
				OP_Node *src);

    virtual void	 getDefaultState(UT_String &default_state_name);

    // If this node sets a context option to a specific value, we can clear
    // this option dependency for this node (and its outputs), because we
    // know that changing this option value past this node won't affect the
    // cooking of our input nodes, even if they use this context option. One
    // exception to this is if this node sets the context option as a
    // function of the option value handed to it. This exception is dealt with
    // when transferring option dependencies from our inputs to this node.
    //
    // Another exception is when the context used to cook this node has a
    // non-default (i.e. set by a node) value for the option. Because our
    // context option change may get "popped" by a node higher up the chain,
    // we must assume that nodes above this "pop" node depend on this option
    // set by one of our outputs, so we need to recook when that node changes
    // option value (even though we overwrite it). This isn't a problem when
    // the option is set to a default value because dependencies on default
    // values are tracked by micro nodes, so changing the default will force
    // a recook from the top of the chain. Testing for this case is why we
    // need to be passed the context passed to this nodes cook method.
    void		 clearInputContextOptionDepIfSafe(
				const UT_StringHolder &opt,
				const OP_Context &context);

protected:
	     OP_Node(OP_Network *parent, const char *name, OP_Operator *entry);
    virtual ~OP_Node();

    /// Implement the method from PRM_ParmOwner
    virtual void	 doGetFullPath(UT_WorkBuffer &str) const;

    /// Retrieve a list of the local variables visible to this OP_Node.
    /// This list is unsorted and may contain duplicates.
    virtual void	 buildLocalVarNames(UT_StringArray &out_vars);

    virtual UT_StringHolder evaluateDisableExpression(const PRM_Parm &prm,
				const UT_StringRef &function) const;

    /// Called when loadNetwork finishes loading the whole parent network of
    /// this node.
    void		 finishedLoadingParentNetwork(void);

    void		 setNewParent( OP_Network *new_parent );

    static void		 bumpNameSerialIndex();

    void		 setLegacyConnectingInputIndex(int index)
			 { myLegacyConnectingInputIndex = index; }

    // All external references must be removed by the time this function
    // returns!
    virtual void	 clearAndDestroy(void);

    // invalidate any cached data
    virtual void	 clearCache();

    void		 clearInterruptedImpl(bool allow_recook);

    // "cookMe" is where the actual work is done.
    // The error is returned (OP_ERR_NONE on success).
    virtual OP_ERROR	 cookMe(OP_Context &context) = 0;

    // "bypassMe" is used to cook the op when the bypass flag is set. 
    // The copied_input parameter should be set to indicate which input was
    // copied. Use -1 if no input was copied.
    // The error is returned (OP_ERR_NONE on success).
    virtual OP_ERROR	 bypassMe(OP_Context &context, int &copied_input) = 0;

    // The following two functions are called from evaluateAllParms() so that
    // subclasses may hook into it to do some work before and after evaluating
    // all the parameters.
    virtual void	 doOverridePreCook()	{ }
    virtual void	 doOverridePostCook()	{ }

    // These "serial" numbers are used for keeping track of how many sources
    // are accessing us. Only call getSourceCount() when locked inside
    // getCookLock()!
    int			 getSourceCount(void) const;
    virtual int		 bumpSourceCount(int d);

    // This is what you should override to do your own OP specific cooking.
    // This should not be called other than from pubCookInputGroups.
    virtual OP_ERROR	 cookInputGroups(OP_Context &context, int alone = 0);

    //  I/O methods
    virtual OP_ERROR	 saveIntrinsic(std::ostream &os,
				       const OP_SaveFlags &flags);
    virtual OP_ERROR	 save(std::ostream &os, const OP_SaveFlags &flags,
				const char *path_prefix = "",
				const UT_String &name_override = UT_String());
    OP_ERROR		 saveUserDataPacket(std::ostream &os,
				const char *path_prefix,
				const UT_String &node_name);

    bool		 loadIntrinsic(UT_IStream &is, const char *path=0);
    virtual bool	 loadPacket(UT_IStream &is, short class_id,
				    short sig, const char *path=0);
    virtual bool	 loadPacket(UT_IStream &is, const char *token,
				    const char *path=0);
    virtual bool	 load(UT_IStream &is, const char *ext="",
					      const char *path=0);

    // this called when the entire network is done loading
    virtual void	 loadStart();
    virtual void	 loadFinished();

    // Loads the OTL_CONTENTS_SECTION for our OTL definition, if it exists,
    // using the syncContents method.
    void		 loadContentsSection();

    // This is a dummy virtual that just exists to be overridden by
    // OP_Network. See that class for more description.
    virtual bool	 syncContents(UT_IStream &is);

    /// Sets the flag idicating if the node is synced (matched) to the HDA.
    void		 setMatchesOTLDefinition(int matches);

    virtual const char	*getFileExtension(int binary) const = 0;
    virtual const char	*getDataFileExtension(int binary) const;

    // This will set the time dependent flag based on parms, inputs and/or
    // extra (non-graph) inputs.
    virtual void	 checkTimeDependencies(int do_parms=1,
				int do_inputs=1,
				int do_extras=1);
    // This will gather context option dependencies based on parms,
    // inputs and extra (non-graph) inputs. These dependencies are
    // applied to the data micro node and the node itself.
    virtual void	 checkContextOptionDependencies(int do_parms);

    // To allow our sub-classes to send events, we make this protected.
    void		 sendEvent(OP_EventType type, void *data=0);

    // haveSeenDataModification() can be used by sub-classes to decide if they
    // need to respond to a particular modification notice.
    int			 haveSeenDataModification(exint modification_id);

    // If you sub-class propagateModification, then be sure to call this
    // OP_Node base class method *last*.
    //
    // This routine will return TRUE if the modification is a new one for this
    // node.  FALSE will be returned if the modification has been seen before.
    //
    // Note that "by_whom" is the immediate input node that caused the
    // propagation to "this" node.
    virtual int		 propagateModification(OP_Node *by_whom,
					       OP_EventType reason,
					       int parm_index,
					       OP_PropagateData &prop_data);

    // Use this to obtain the cook cache.
    // @pre The caller has locked myCookLock if it is currently being cooked.
    OP_Cache *		 getCookCache()	{ return &myCache; }

    static void		 cmd_locate(CMD_Args &);

    virtual void	 saveFlagsForUndo();
    void		 saveForUndoLayout();

    static void		 saveForUndoDelete(OP_NodeList &nodes);

    /// Clears all cook dependencies (i.e., items that may cause it to cook)
    void		 clearDependency();

    /// This function will dirty our data if the given time is different from
    /// our last cooked time.
    ///
    /// This function is normally called from cook() but subclasses can prevent
    /// this with flags().setClearDependency(false). In that case, subclasses
    /// are responsible for calling this function at the appropriate time. This
    /// is primarily for nodes which cache their data across time. In that
    /// case, if they did _not_ reuse their cached data, then
    /// dirtyForTimeChange() needs to be called.
    ///
    /// @returns true if node was dirtied, false otherwise.
    bool		dirtyForTimeChange(fpreal t)
			{
			    if (isCookedTime(t))
				return false;

			    OP_DataMicroNode &dm = dataMicroNode();
			    OP_DataMicroNode &pm = parmListMicroNode();
			    dm.setDirty(true);
			    if (&dm != &pm)
				pm.setDirty(true);

			    return true;
			}

    // clone dependencies from the proxy. proxy is no longer valid
    // after the clone!
    virtual void	 cloneFromProxyRefNode( OP_Network *proxy );
    // clone a new proxy node. this node is no longer valid after this!
    virtual OP_Network * cloneToProxyRefNode();
    void		 stealDependents( OP_Node *other );

    bool		 hasOpDependents()
			    { return !myOpDependents.isEmpty(); }
    virtual int		 hasProxyRefNodes() const
			    { return 0; }
    virtual void	 moveProxyRefNodes( OP_Network * /*dest */ ) { }
    void		 clearOpReferences();
    virtual void	 rebuildOpDependents( bool proxy_only );

    /// removeOpDependent() returns the number of dependents removed
    virtual int		 removeOpDependent(
			    int op_id, const PRM_RefId &ref_id,
			    OP_InterestType mask = OP_INTEREST_ALL );
    virtual int		 removeOpDependent(
			    int op_id, OP_InterestType mask = OP_INTEREST_ALL );

    // new method for dependencies: use the PRM_TYPE_OP_REF_*
    //	in your parm template to add your dependency.
    // override this to do special cases, make sure you call the base class
    // first before adding your own dependencies!
    virtual void	 buildOpDependencies();
    virtual void	 checkChannelDependencies( CH_Channel *ch,
						   CH_CHANGE_TYPE reason );

    void		 notifyOpDependents( OP_InterestType interest,
					     bool recurse );
    virtual void	 moveDependencies( OP_Node *from_node );
    // Two functions for notifying dependent nodes of name changes. The
    // non-virtual function is the one that should be called to do the
    // update. It calls the virtual function to do the actual updating,
    // and generate a list of dependent nodes that were changed. Then it
    // forces a recook on the changed nodes.
    void		 notifyRenameDependents( const UT_String &full_from );
    virtual void	 notifyRenameDependents( const UT_String &full_from,
						 OP_NodeList &cook_nodes );
    // Two functions for notifying referencing nodes of name changes. The
    // non-virtual function is the one that should be called to do the
    // update. It calls the virtual function to do the actual updating,
    // and generate a list of dependent nodes that were changed. Then it
    // forces a recook on the changed nodes.
    void		 notifyRenameReferences( const UT_String &full_from );
    virtual void	 notifyRenameReferences( const UT_String &full_from,
						 OP_NodeList &cook_nodes );
    // this method is called to alert this op that its dependency has
    // changed. if it's a name interest, then the old full path is given
    virtual void	 handleOpDependency( int referenced_op_id,
					     const OP_Dependency &op_dep,
					     OP_InterestType interest_type,
					     bool &need_cook,
					     const char *old_fullpath,
					     const char *old_cwd,
					     const char *old_chan_name );
    virtual void	 buildParmDependency( int parm_index );

    void		 addOpNameReference(
			    const PRM_RefId &ref_id, const UT_String &op_path,
			    OP_InterestType type );
    // chan_name must be real channel name not an alias
    void		 addChannelNameReference(
			    const PRM_RefId &ref_id,
			    const UT_StringRef &op_path,
			    const UT_StringRef &chan_name,
			    OP_InterestType type );

    OP_Node		*getNodeOrCreateProxy(const UT_StringRef &op_path);

    void		 addOpReference( const PRM_RefId &ref_id, OP_Node *node,
					 const PRM_RefId &source_ref_id,
					 OP_InterestType type );

    /// This function takes the given parm string value containing an op path
    /// and does a search and replace given the old and new op paths.
    ///
    /// value_str	    - the old parm value to modify
    /// new_fullpath	    - the new path value to change to
    /// old_fullpath	    - the old path that needs changing
    /// old_cwd	    - the old path that the parm value was relative to
    /// new_cwd	    - the new path that the parm value is to be relative to
    ///
    /// The reason for old_cwd & new_cwd is when collapsing a node into a
    /// subnet.  We need to figure out the new path references of the collapsed
    /// subnet since it has changed location. We match using the old_cwd but
    /// take the new path relative to new_cwd. Note that in this situation,
    /// old_fullpath is the same as new_fullpath.
    bool		 changeOpPathRef( UT_String &value_str,
					  const char *new_fullpath,
					  const char *old_fullpath,
					  const char *old_cwd,
					  const char *new_cwd );

    // handy functions for dealing with string parms that can have the
    // op:/path[frame] syntax
    static bool		 getStringParmOpPath(
				PRM_Parm &parm, int vi, UT_String &oppath,
				int thread );
    void		 handleStringParmOpPathDependency(
				int parm_index, int vi,
				const char *new_fullpath,
				const char *old_fullpath, const char *old_cwd );

    //  Only OP_Network should call notifyNodeDeletion()
    void		 notifyNodeDeletion();

    //  Only notifyNodeDeletion() should call nodeDeleted().
    //	nodeDeleted()		- gets called every time a node is deleted
    virtual void	 nodeDeleted(OP_Node *op, int propagate=1);

    void		 startCookTimer(const OP_Context &context);
    void		 stopCookTimer(const OP_Context &context);

    virtual const char	*getCookTimerLabel(const OP_Context &context) const;

    // Overrides permission errors so that we don't have problems when cooking.
    virtual void	 permissionError(const char *chname = 0);

    // Add an error for the invalid input given by input_idx
    void		 addInputError(unsigned input_idx);

    // Add an error for the given node having failed to load.
    void		 addInputNodeError(OP_Node *src);

protected:
    /// This function is currently only used by VOPs. It checks if an input 
    /// on the subnet is connected inside the subnet. The default implementation
    /// here simply returns false to preserve existing behaviour for all other
    /// contexts.
    virtual bool	 isInputConnectedInsideSubnet(int input_idx) const;
    virtual bool	 isNamedInputConnectedInsideSubnet(
				const OP_ConnectorId& input_name) const;

    // notifier when this node is unlocked
    virtual void	 nodeUnlocked() { ; }

    // These functions must be used with care!
    virtual void	 setUniqueId(int id);
    void		 changeOpDependentUniqueId( int old_id, int new_id );
    void		 changeOpReferenceUniqueId( int old_id, int new_id );
    virtual void	 clearUniqueId();

    void		 setAllowBuildDependencies( int yesno )
			    { myAllowBuildDependencies = yesno; }
    int			 allowBuildDependencies() const
			    { return myAllowBuildDependencies; }

    /// Evaluate all parameters for this operator. This is used for scripted
    /// Ops to make sure that our dependencies get set up properly.
    void		 evaluateAllParms(fpreal t);

    /// Override and assign any external (file) references to the reflist,
    /// and if nodelist is non-null, the full pathname of the node with the
    /// reference.
    virtual void	 getMyExternalReferences(UT_StringArray &reflist,
						 UT_StringArray *nodelist =0,
						 bool collapse = false,
						 bool check_missing = false,
						 bool show_missing_only=false);

    void		 getExternalFiles(UT_StringArray &files,
					  const char *stringparm,
					  int framestart, int frameend,
					  bool collapse,
					  bool check_missing,
					  bool show_only_missing,
					  UT_KnownPath path);

    /// Rather than overloading the descriptive name, you can instead
    /// just provide a parameter name that will be evaluated to
    /// become the descriptive name.
    virtual void	 getDescriptiveParmName(UT_String &str) const;

    /// Additonal information that should be provided at the network
    /// level in addition to the name.
    /// By default will inspect the getDescriptiveParmName.
    virtual void	 getDescriptiveName(UT_String &str) const;

    /// Marks our cached descriptive name as dirty.
    void		 dirtyDescriptiveNameCache();

    /// Return true if it is safe at this time to destroy this node.
    virtual bool	 canDestroyNode();

    /// Override this to return true in an operator type that wants its
    /// cooked data to rely on all its parameter values but doesn't actually
    /// evaluate any parameters in its cook function (the prototypical
    /// example of this is SHOPs).
    virtual bool	 cookedDataUsesAllParameters() const
			 { return false; }
    /// If the cook method relies on the base class cook to evaluate parameters
    /// (like in SHOPs), we need to trap the errors during this evaluation.
    virtual bool	 cookedDataNeedsErrors() const
			 { return false; }
    /// If this node should be cooked for any output connected to it, return
    /// true here. Otherwise we only cook a node if something is connected to
    /// it's primary output. This does not let the node cook differently for
    /// each output, since the cook method doesn't know what output is being
    /// cooked for. So this should only be used if the cook is unaffected by
    /// the specific connected output.
    virtual bool	 cookDataForAnyOutput() const
			 { return false; }

    // Returns true if we are running our creation script, or any parent
    // node of us is running its creation script.
    bool		 getIsRunningInCreateScript() const;

    /// This method is called whenever something in the network changes
    /// that may have an impact on the representative node this network may
    /// have.  It is used at this level to notify parents, and at the OBJ level
    /// to actually refresh the representative object.
    virtual void	 refreshRepresentativeNode(OP_Node &changed_child);

    /// Sets up the inputs and outputs after all nodes have been loading 
    /// from file (in case it needs its children to determine inputs/oututs).
    virtual void	 setupConnectorsAfterLoad();

    /// Converts opdef and oplib format paths from opdef:nodepath to
    /// opdef:tablename/optype.
    void                 convertOpdefToAbsolutePath(UT_String &str) const;

    bool		 isCookedTime(fpreal t) const
			 {
			    return dataMicroNodeConst()
				.isEqualToLastUpdateTime(t);
			 }

    /// Subclasses should override this to dump extra micronodes that they own
    /// for debugging purposes.
    virtual void	 dumpMicroNodes(
			    std::ostream &os,
			    bool as_DOT,
			    int indent_level) const;


    UT_TokenString		  myHashCode;		// Generates a unique
    UT_TokenString		  myParmHashCode;	// parameters & inputs.
    int				  myLastGroupMask;
    OP_VERSION			  myHashVersion;
    fpreal			  myHashTime;
    int				  myParmLastGroupMask;
    OP_VERSION			  myParmHashVersion;
    fpreal			  myParmHashTime;
    UT_String			  mySyncErrors;
    OP_ERROR			  mySyncErrorLevel;
    int				  mySyncDepth;

    /// Deletes an input completely if it is disconnected. If the input
    /// is connected, does nothing.
    void		 deleteInput(int idx);
    void		 deleteInput(const OP_ConnectorId& input_name);

    /// Performs actual removal from the member input array.
    virtual void	 doDeleteInput(int idx);

    /// Update the error severity of this node.
    void		 updateErrorSeverity();

    /// Overriden in VOPs.
    virtual void	 onInputAllocated(OP_Input* new_input, int index) { }

    /// Debug method for checking connector consistency.
    void		 checkConnectorsConsistency(void);

    /// Use these to collect all inputs/outputs in cases where they will
    /// need to be modified.
    void		 gatherInputs(UT_Array<OP_InputPair> &input_pairs);
    void		 gatherOutputs(UT_Array<OP_OutputPair> &output_pairs);

    void		 renameInput(OP_Input* input, int new_id);
    void		 renameOutput(OP_Output* output, int new_id);

    // Compiled hash code
    uint32		 myCompHash;

private:
    bool		 doDebugConsistencyCheck();
    void		 createCollection( OP_Operator *entry );

    // these are extra private. only to be called from saveIntrinsic and save
    // respectively
    OP_ERROR		 saveIntrinsicError();
    OP_ERROR		 saveError();
    OP_ERROR		 saveGroupMemberships(std::ostream &os,
					      const OP_SaveFlags &flags,
					      const char *path_prefix);
    bool		 loadGroupMemberships(UT_IStream &is,const char*path=0);

    ///Make a note of which network box we're in
    OP_ERROR		 saveNetworkBox(std::ostream &os,
				        const OP_SaveFlags &flags,
				        const char *path_prefix);

    ///Put ourselves into a network box if we saved ourselves as being in one
    bool		 loadNetworkBox(UT_IStream &is, const char *path=0);

    bool		 loadNodeFlags(UT_IStream &is, const char *path);

    void		 saveForUndoComment();
    void		 saveForUndoDelScript();

    UT_BitArray		&getGroups()	   { return myGroups; }
    const UT_BitArray	&getGroups() const { return myGroups; }
    // Only OP_Input should call setOutput() and delOutput() functions.
    bool		 setOutput(OP_NetworkBoxItem *item, int outputIdx);
    bool		 delOutput(OP_NetworkBoxItem *item, int outputIdx);
    bool		 setNamedOutput(OP_NetworkBoxItem *item,
				const OP_ConnectorId& output_name);
    bool		 delNamedOutput(OP_NetworkBoxItem *item,
				const OP_ConnectorId& output_name);

    void		 setInterrupted();

    int			 findAncestor(const OP_Node *node) const;
    int			 findDataAncestor(const OP_Node *node) const;
    void		 clearRecursionBit() const;

    void		 initializeExpressions() const;
    void		 clearInputs();
    void		 clearOutputs();

    virtual PRM_ParmMicroNode
			*createParmMicroNodes(PRM_Parm &parm) const;

    static bool		 getMicroNodesFromRef(
			    const OP_InterestRef &ref,
			    DEP_MicroNodeList &micronodes,
			    bool data_target);

    // new method: dependencies get created from parm template and only
    //	       gets updated when necessary. only buildOpDependencies()
    //	       needs to call this
    void		 removeOpReference(
			    const PRM_RefId &ref_id,
			    OP_InterestType mask = OP_INTEREST_ALL );
    void		 removeOpReference(
			    const PRM_RefId &ref_id,
			    int op_id,
			    OP_InterestType mask = OP_INTEREST_ALL );
    // ref_chan_name must be the real channel name and not an alias
    int			 addOpDependent( int op_id, 
					 const PRM_RefId &source_ref,
					 const PRM_RefId &ref_id,
					 OP_InterestType type );

    void		 updateRenameDependent( OP_Node *dependent,
						const char *old_cwd,
						OP_NodeList &cook_nodes );

    void		 notifyInputOpDependents(int input_i);

    void		 getExistingOpReferencesImpl(
			    OP_NodeList &refs, bool include_descendants) const;
    void		 getExistingOpDependentsImpl(
			    OP_NodeList &deps, bool include_descendants) const;

    void		 buildScriptOpDependencies();
    void		 changeScriptOpRef( const char *new_fullpath,
					    const char *old_path,
					    const char *old_cwd );
    static void		 changeScriptCB( UT_String &str,
					 const char *token_start,
					 const char *&ch, void *userdata );

    // Go through and find all nodes which have time interest and requires
    // cooking.  If the update_cook flag is true, then we will call force
    // recook on the node as long as node != this. 'interests' if non-null,
    // stores the path of nodes that has time interest and will set the time
    // interest cook flag on those nodes.
    bool		 findTimeInterests(OP_Context &context, OP_Node *node,
					   bool update_cook,
					   OP_NodeCycleDetect &cycle,
					   OP_NodeList *interests = nullptr);

    /// Helper method for 'completePath()'
    bool 		 attemptPrefixMatch(const OP_NetworkBoxItem *item, 
					    char *childpref, int childpreflen, 
					    UT_String &maxprefix) const;
	
    /// Helper for propagateModification()
    void		 findUsedDataInterests(OP_NodeList &nodes, OP_Node *op);

    /// Helper function for the setGlobal*Param functions. This function
    /// interprets the return value from CH_Manager::setGlobal*Param.
    bool		 internalSetGlobalParam(const char *param,
						int setreturned,
						bool *warned);

    int			 internalSetPicked(int on_off,
					   bool propagate_parent_event,
					   bool edit);

    OP_ERROR		 internalCookInput(OP_Context &context, int input_idx,
					   OP_Node &input_node);

    virtual void	 removeInputFromMapping(int idx);
    void		 removeOutputFromMapping(int idx);

    /// Helper function to skip until the next closing brace in a stream
    /// (including the brace itself).
    void		 skipUntilClosingBrace(UT_IStream &is);

    /// Resolves any inputs for which the names were not found during
    /// actual loading.
    void		 resolveUnresolvedLoadedInputs(void);

    void		 generateConnectorUniqueId(OP_ConnectorId& id_out);
    OP_Output		*getOrCreateOutput(int array_index, bool create,
				int id_to_use);

    uint32		 computeCompiledHash() const;

    // Adds a warning to the current error manager if this node has been
    // marked as deprecated.
    void		 addDeprecationWarningsIfAny();

    // Find an unused connectorId to assign to the next input/output
    int                  getConnectorNextId() const;

    bool		 computeCompiledStatus() const;

public:
    /// SOP, OBJ, and SOP Solver DOP nodes can be a selection owner,
    /// but other nodes cannot.
    /// This replaces the previous inheritance from SOP_SelectionOwner.
    virtual bool isSelectionOwnerType() const
    { return false; }

private:
    OP_Cache			 myCache;

    OP_ERROR			 myLastErrorSeverity;
    OP_Network			*myParent;
    OP_Network			*myRootCompiledParent;
    UT_ValArray<OP_Input *>	 myInputs;
    // This is a map of names to inputs. 
    OP_ConnectorInputMap	 myInputsMap;
    mutable UT_BitArray		 myUsedInputs;
    UT_ValArray<OP_Output *>	 myOutputs;
    // This is a map of names to outputs. 
    OP_ConnectorOutputMap	 myOutputsMap;

    // Default data micro node.
    OP_DataMicroNode		 myDataMicroNode;
    DEP_MicroNode		 myFlagMicroNode;
    UT_StringArray		 myClearInputContextOptionDeps;

    // Micro nodes for OP events.
    UT_UniquePtr<DEP_MicroNode *[]> myEventMicroNodes;

    UT_String			 myComment;
    UT_String			 myDelScript;
    UT_String			 myCreatorState;
    UT_BitArray			 myGroups;
    OP_Value			 myEventValue;
    fpreal			 myPosX;
    fpreal			 myPosY;
    exint			 myCookCount;
    SYS_AtomicInt32		 mySourceCount;
    int				 myBlockModify;
    int				 myUniqueId;
    int				 myNextIdOverride;
    unsigned			 myActiveInputIndex;
    // Indicates which input is currently being connected,
    // -1 if none.
    int				 myLegacyConnectingInputIndex;

    bool			 myAutoscope;

    fpreal			 myCookDuration;	// time spent cooking
    unsigned int		 myRenameConflict  		: 1,
    				 myCookedForRender 		: 1,
				 myModifyWaiting   		: 1,
				 myBuiltExplicitly 		: 1,
				 myAllowBuildDependencies	: 1,
				 myBeingDeleted	   		: 1,
				 myRunningCreateScript		: 1,
				 myRunningDelScript		: 1,
				 myAlreadyRunDelScript		: 1,
				 myMatchesOTLDefinition		: 1,
				 myLoadCookedDataError		: 1,
				 myBuiltHashCode		: 1,
				 myIsProxyRefNode		: 1,
				 myInhibitInputConsolidation	: 1,
    // Tracks if we have engaged our local variables, true from the
    // SOP_Node::setupLocalVars() until the SOP_Node::resetLocalVarRefs().  Any
    // attempt to evaluate local vars outside of here will redflag. This
    // flag is here instead of SOP_Node so that PI_OpHandleLink has access
    // to temporarily enable it.
				 myLocalVarActive		: 1;

    mutable bool		 myRecursionFlag;

    // Tracks the nesting level of the modification propagation chains
    static int			 thePropagateLevel;

    // ops that we have a reference to
    OP_ReferenceList		 myOpReferences;
    // ops that depend on us
    OP_DependencyList		 myOpDependents;

    // bundles that we have a reference to
    OP_BundleReferences		 myBundleRefs;

    // Locking for cooking and adding extra inputs
    mutable OP_CookLock		 myCookLock;
    UT_TaskState		 myTaskState;

    // Arbitrary string data that users can attach to nodes that gets saved 
    // with the hip file.
    UT_Options			 myUserData;
    // Cached value for the node shape.
    UT_StringHolder		 myNodeShape;
    
    // Arbitrary blind data that users can attach to nodes. These do not get
    // saved with the hip file.
    UT_StringMap<UT_SharedPtr<void> > myBlindData;

    // Arbitrary data blocks that, unlike blind data, are saved with the node.
    // This obviously implies the data must be self contained (no pointers).
    UT_StringMap<OP_DataBlockPtr> myDataBlocks;

    // Arbitrary data (in the form of Python objects) the users can attach to
    // nodes that does not get saved with the hip file.
    OP_PythonObjectCache	 myCachedPythonObjects;

    // During load, not all of our input names are known. This stores
    // unresolved input names until OP_Node::loadFinished() is called,
    // where they are resolved.
    UT_Array<OP_UnresolvedConnection *> *myUnresolvedInputs;

    OP_NodeFlags		 myFlags;

    /// Cached evaluation of the node's descriptive name.
    UT_StringHolder		 myCachedDescriptiveName;
    bool			 myCachedDescriptiveNameDirty;

    /// Only used during loading. Set to false initially, it is set to true
    /// once we load inputsNamed intrinsic section. If we do, the regular
    /// inputs section will then be ignored.
    bool			 myLoadedNamedInputs;

    /// Used for caching the compiled status of the node. Initially undefined,
    /// gets set on first isCompiled() call.
    mutable OP_CompileStatusType myCachedCompileStatus;

    // Maintenance of node id's
    static int			 makeUniqueId(OP_Node *node);

    static bool			*theCheckConnectionConsistency;
    static int			 theUniqueId;
    static int			 theUniqueSize;
    static OP_Node	       **theUniqueNodes;
    static bool			 theDoingBulkNotification;
    static OP_Node		*theBulkNotificationCaller;

    friend class		 OP_Network;
    friend class		 OP_Group;
    friend class		 OP_DataMicroNode;
    friend class		 OP_EventMicroNode;
    friend class		 OP_ParmMicroNode;
    friend class		 OP_Input;
    friend class		 OP_IndirectInput;
    friend class		 OP_UndoDelete;
    friend class		 OP_UndoInput;
    friend class		 OP_UndoCreate;
    friend class		 OP_UndoSpareParm;
    friend class		 OP_UndoUserData;

    friend class		 op_MicroNodeDirtied;
};

// helper function
// should always return a valid pointer
OP_API OP_Node * OPgetNodeFromChannel( const CH_Channel *chp );

// UTformat support.
static inline size_t
format(char *buffer, size_t buffer_size, const OP_Node &v)
{
    UT_StringHolder s = v.getFullPath();
    if (!buffer)
	return s.length();
    else
    {
	size_t len = std::min(size_t(s.length()), buffer_size);
	::memcpy(buffer, s.c_str(), len);
	return len;
    }
}

class OP_API OP_DisablePropagateAssertScope
{
public:
    OP_DisablePropagateAssertScope();
    ~OP_DisablePropagateAssertScope();
};

#define CAST_INSTANTIATE(PREFIX)					\
inline static PREFIX##_Node *CAST_##PREFIX##NODE(OP_Node *op)		\
{									\
    return ((op) ? (op)->castTo##PREFIX##Node() : 0);			\
}									\
									\
inline static const PREFIX##_Node *CAST_##PREFIX##NODE(const OP_Node *op) \
{									\
    return ((op) ? (op)->castTo##PREFIX##Node() : 0);			\
}									\

/// @anchor OP_Node_CAST_FOONODE
/// These convenience functions let you do the cast without worrying if the
/// underlying pointer is null or not. They aren't macros because this way we
/// avoid double evaluating the op.
///	CAST_OBJNODE()
///	CAST_SOPNODE()
///	CAST_POPNETNODE()
///	CAST_POPNODE()
///	CAST_CHOPNETNODE()
///	CAST_CHOPNODE()
///	CAST_ROPNODE()
///	CAST_SHOPNODE()
///	CAST_COP2NODE()
///	CAST_COPNETNODE()
///	CAST_VOPNODE()
///	CAST_VOPNETNODE()
///	CAST_DOPNODE()
///	CAST_TOPNODE()
///	CAST_LOPNODE()
//@{
INSTANTIATE_FOR_ALL_NODE_TYPES(CAST_INSTANTIATE)
//@}

#undef CAST_INSTANTIATE

// We don't want anyone to conflict with this..
#undef INSTANTIATE_FINDNODE_FUNCTIONS
#undef INSTANTIATE_CASTNODE_FUNCTIONS
#undef INSTANTIATE_FOR_ALL_NODE_TYPES

#endif
