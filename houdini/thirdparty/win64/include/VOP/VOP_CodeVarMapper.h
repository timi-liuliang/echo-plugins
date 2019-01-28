/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __VOP_CodeVarMapper_h__
#define __VOP_CodeVarMapper_h__

#include "VOP_API.h"
#include "VOP_Language.h"

#include <UT/UT_Array.h>
#include <UT/UT_Map.h>
#include <UT/UT_ArrayMap.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringMap.h>
#include <SYS/SYS_Hash.h>

class VOP_Fetch;
class VOP_Node;
class VOP_CodeGenContext;

template<typename T> class UT_SymbolMap;
typedef UT_SymbolMap<UT_StringHolder> VOP_VariableMap;
typedef UT_Map<const VOP_Node *, VOP_VariableMap *> VOP_NodeVarMapsTable;
typedef UT_SymbolMap<VOP_VariableMap *> VOP_OpVarMapsTable;

/// Simple class used to divide variable mappings into different contexts.
/// With the introduction of the Fetch VOP, a node can produce code
/// and variables multiple times.  So we need contexts to separate
/// mappings from the same node from trampling over each other.
class VOP_CodeVarMapperContext
{
public:
    /// Constructor.
    VOP_CodeVarMapperContext(VOP_CodeVarMapperContext *parent=NULL, 
			     VOP_Fetch *fetch_vop=NULL);

    /// Destructor.
    ~VOP_CodeVarMapperContext();

    /// Return the child context which is associated to the given key.
    /// If no such child context exists, then create one.
    VOP_CodeVarMapperContext	*findOrCreateChildContext(VOP_Fetch *fetch_vop);

    /// Return the child context which is associated to the given key.
    /// Return NULL if no such context exists.
    VOP_CodeVarMapperContext	*findChildContext(VOP_Fetch *fetch_vop) const;

    /// Return a pointer to the parent context.
    VOP_CodeVarMapperContext	*getParent() const { return myParent; }

    /// Return a pointer to the Fetch VOP which was responsible
    /// for the creation of this context.
    VOP_Fetch			*getFetchVOP() const { return myFetchVOP; }

private:
    // A mapping of keys to child contexts.
    UT_Map<int, VOP_CodeVarMapperContext *> myChildContexts;

    // A pointer to the parent context.
    VOP_CodeVarMapperContext	*myParent;

    // A pointer to the Fetch VOP which was responsible for the creation
    // of this context.
    VOP_Fetch			*myFetchVOP;
};

/// An entry corresponding to a vop node in a given mapper context.
/// It contains information about output variable names and types for that vop.
class VOP_API VOP_CodeVarMapperEntry
{
public:
    VOP_CodeVarMapperEntry(VOP_Node *vop, VOP_CodeVarMapperContext *context);
    ~VOP_CodeVarMapperEntry();

    void			 addOutputVariable(const char *codevar,
						   const char *var,
						   int index,
						   const VOP_TypeInfo &typeinfo,
						   bool outonly);
    const char			*getOutputVariableCodeName(int index) const;
    const char			*getOutputVariableName(int index) const;
    const VOP_TypeInfo		&getOutputVariableTypeInfo(int index) const;
    bool			 getDeclareOutputVariable(int index) const;
    int				 getNumEntries() const;
    VOP_Node			*getVop() const;
    
private:
    void			 clear();

    UT_StringArray		 myOutputVariableCodeNames;
    UT_StringArray		 myOutputVariableNames;
    UT_Array<VOP_TypeInfo*>	 myOutputVariableTypeInfos;
    UT_Array<bool>		 myDeclareOutputVariables;
    VOP_Node			*myVop;

    friend class VOP_CodeVarMapper;
};

/// Container class that keeps track of all var mapper entries, that is
/// infos about output varialbe names for each vop node in a given mapper
/// context.
class VOP_API VOP_CodeVarMapper
{
public:
    using KeyType = std::pair<VOP_Node *, VOP_CodeVarMapperContext *>;
    struct VOP_API KeyHasher
    {
	//using KeyType = Keystd::pair<VOP_Node *, VOP_CodeVarMapperContext *>;
	SYS_HashType	operator()(KeyType const &s) const
	{
	    SYS_HashType	h = 0;
	    SYShashCombine(h, s.first);
	    SYShashCombine(h, s.second);
	    return h;
	}
    };

    VOP_CodeVarMapper();
    ~VOP_CodeVarMapper();

    VOP_CodeVarMapperEntry		*findOrAddEntry(VOP_Node *node,
					    VOP_CodeVarMapperContext *context);
    VOP_CodeVarMapperEntry		*findEntry(VOP_Node *node,
					    VOP_CodeVarMapperContext *context) 
					    const;
    const VOP_CodeVarMapperEntry	*getEntry(int idx) const;
    int					 getNumEntries() const;

    /// Some nodes use temporary variables with custom types, and types are
    /// inspected and automatically defined (if necessary) when declaring 
    /// variables for the outputs. If no output is of that custom type, 
    /// the type won't be defined. So, VOPs can use extra types list 
    /// to indicate which custom types need definition in the generated code.
    void		addExtraType(const VOP_TypeInfo &type_info);
    const VOP_TypeInfo &getExtraType(int idx) const;
    int			getNumExtraTypes() const;

private:
    // Use an ArrayMap since there are fewer allocations
    using MapType = UT::ArrayMap<KeyType,
			VOP_CodeVarMapperEntry *,
			false,
			128,
			UT::MapKeyClearer<KeyType, VOP_CodeVarMapperEntry *>,
			KeyHasher>;

    MapType				myMap;
    UT_Array<VOP_CodeVarMapperEntry *>	myMapEntries;
    UT_Array<VOP_TypeInfo>		myExtraTypesToDefine;
};

/// An aggregation of all the information about variable names and other
/// info needed during variable replacement process.
class VOP_API VOP_CodeVarLookupData
{
public:
    VOP_CodeVarLookupData(VOP_Node *vop,
                          UT_StringSet &varTable,
			VOP_NodeVarMapsTable &localVarTables,
			VOP_VariableMap &localVarTable,
			VOP_CodeVarMapper &varMapper,
			VOP_CodeVarMapperEntry *varMapperEntry,
			const VOP_CodeGenContext &codegen_context,
			VOP_CodeVarMapperContext *varmap_context,
			VOP_Node *function_subnet = NULL,
			UT_SortedStringMap<UT_String> *is_connected_map = NULL)
	: myVop(vop), myVarTable(varTable),
	  myLocalVarTables(localVarTables),
	  myLocalVarTable(localVarTable), myVarMapper(varMapper),
	  myVarMapperEntry(varMapperEntry), myDoingOuterCode(true),
	  myCodegenContext(codegen_context),
	  myCodeVarMapperContext(varmap_context),
	  myFunctionSubnet(function_subnet),
	  myIsConnectedMap(is_connected_map)
    { }
    ~VOP_CodeVarLookupData()
    { }

    VOP_Node			*myVop;
    UT_StringSet		&myVarTable;
    VOP_NodeVarMapsTable	&myLocalVarTables;
    VOP_VariableMap		&myLocalVarTable;
    VOP_CodeVarMapper		&myVarMapper;
    VOP_CodeVarMapperEntry	*myVarMapperEntry;
    bool			 myDoingOuterCode;
    const VOP_CodeGenContext	&myCodegenContext;
    VOP_CodeVarMapperContext	*myCodeVarMapperContext;
    VOP_Node			*myFunctionSubnet;
    UT_SortedStringMap<UT_String> *myIsConnectedMap;
    UT_StringHolder		myVariableString;
};

#endif
