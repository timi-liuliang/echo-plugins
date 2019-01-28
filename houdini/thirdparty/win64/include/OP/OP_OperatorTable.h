/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_OperatorTable.h (C++)
 *
 * COMMENTS:	This class is simply a symbol table of OP_Operator's.
 *
 */

#ifndef __OP_OperatorTable_h__
#define __OP_OperatorTable_h__

#include "OP_API.h"
#include "OP_OTLDefinition.h"
#include "OP_Operator.h"
#include "OP_OpNamespaceHierarchy.h"
#include <UT/UT_NonCopyable.h>
#include <UT/UT_String.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Color.h>
#include <iosfwd>

class OP_Node;
class OP_Network;
class OP_OperatorTable;
class OP_ScriptOperator;
class OP_ScriptIndexFile;
class OP_UpdateTableSink;

typedef UT_ValArray<OP_OperatorTable *>	 OP_OperatorTableList;
typedef UT_SymbolMap<OP_OperatorTable *> OP_OperatorTableMap;
typedef OP_ScriptOperator *(*OP_ScriptCreatorFunc)
			   (const OP_OTLDefinition &definition);
typedef bool (*OP_ScriptCanReuseFunc)
			   (const OP_ScriptOperator *script_operator,
			    const OP_OTLDefinition &new_definition);

class OP_API OP_OperatorTable : UT_NonCopyable
{
public:
     OP_OperatorTable(const char *table_name, const char *script_directory);
    ~OP_OperatorTable();

    static int	getAllOperatorTables(OP_OperatorTableList &list);

    /// Creates a new node of a given type inside a parent and names it
    /// as a give name.
    /// @param	exact_type   If true, the operator name parameter 'type',
    ///			is used verbatim to lookup the operator. Otherwise,
    ///			a preferred operator name that matches 'type'
    ///			specification is found first and then is used to lookup
    ///			the operator. For example, "hda" may match "hda::2.0",
    ///			which is the preferred operator definition.
    OP_Node		*createNode(OP_Network *parent, const char *type,
				const char *name = 0,
				int *aliasedScriptedOp = 0,
				bool exact_type = false);

    OP_Operator		*getOperator(const char *name) const;
    bool		 addOperator(OP_Operator *op,
				std::ostream *err = nullptr);
    void		 removeOperator(OP_Operator *op);

    OP_Operator		*getPrimarySubnetOperator()
			 { return myPrimarySubnet; }

    void		 setScriptCreator(OP_ScriptCreatorFunc creator_func,
				OP_ScriptCanReuseFunc reuse_func);
    void		 addScriptIndexFile(const char *indexpath,
				const char *indexfile,
				const char *classid,
				const char *extrainfo,
				int defaultMinInputs,
				int defaultMaxInputs,
				bool issubnet,
				bool ispython);
    bool		 loadScriptOperator(const char *opname, UT_IStream &is,
				OP_OTLLibrary *addToLib);
    bool		 addOperatorIfNew(const OP_OTLDefinition &definition);
    bool		 canReuseOperator(
				const OP_OTLDefinition &new_definition,
				const OP_Operator *op = 0) const;

    void		 requestReload();

    // Runs the DSO installation function on our currently loaded dso's inthe
    // dso search path.
    void		 runDSOInstall();

    // Loads the given dso file and runs the installation function 
    bool		 loadDSO(const char *dso_file);

    int			 entries() const
			 { return myOpCount; }

    void		 getOperatorList(UT_StringArray &list,
				bool english=true) const;
    int			 getOperators(OP_OperatorList &list,
				OP_Network *net = 0,
				bool filterhidden = false) const;

    // This returns the optype id for the given table.
    OP_OpTypeId		 getOpTypeID() const;

    // This returns a unique integer for each OP_OperatorTable created
    int			 getUniqueOpTypeID() const
			 { return myUniqueOpTypeID; }
    // Before sorting, you should getOperators(list);
    void		 sortOperators(OP_OperatorList &list,
				OP_Operator::OP_OperatorCompare method =
				OP_Operator::OP_COMPARE_GEN_NAME);

    const UT_String	&getName() const
			 { return myName; }
    const UT_String	&getScriptPath() const
			 { return myScriptDirectory; }

    unsigned		 getOperatorStatus(OP_Operator *op) const;
    unsigned		 getStatus(void) const;
    void		 setDefaultType(const char *type);
    const char		*getDefaultType() const;

    // Add an operator alias (from the OPcustomize file).
    bool		 setOpRename(const char *optype, const char *newname,
        		             std::ostream *err = nullptr);
    // Display all the oprename commands.
    void		 outputOpRenames(std::ostream &os) const;

    // Add an operator alias (from the OPcustomize file).
    bool		 setOpAlias(const char *optype, 
        		        const char *alias,
        		        std::ostream *err = nullptr);

    // Sets what the inital default name of the op will be.
    // null first name will disable, reverting to the usual rules.
    bool		 setOpFirstName(const char *type,
				const char *firstname,
        		        std::ostream *err = nullptr);
    // Display all the opalias commands.
    void		 outputOpAliases(std::ostream &os) const;
    // Display all the firstname commands.
    void		 outputOpFirstNames(std::ostream &os) const;
    // Get all the aliases for a particular operator type.
    void		 getOpAliases(const char *opname,
				UT_StringArray &opaliases) const;
    // Get the operator type represented by the supplied alias (if any).
    const char		*getOpFromAlias(const char *alias) const;
    // Add an operator to the excluded op list (from the OPcustomize file).
    bool		 addOpExcluded(const char *opname, 
        		        std::ostream *err = nullptr);
    // Is operator excluded?
    bool		 isOpExcluded(const char *opname) const;
    // Display all the opexclude commands.
    void		 outputOpExcluded(std::ostream &os) const;
    // Add an operator to the hidden op list (from the OPcustomize file).
    bool		 addOpHidden(const char *opname,
        		             std::ostream *err = nullptr);
    // Delete an operator from the hidden op list (from the OPcustomize file).
    void		 delOpHidden(const char *opname);
    // Is operator hidden?
    bool		 isOpHidden(const char *opname) const;
    // Get all hidden ops.
    void		 getOpHidden(UT_StringArray &opnames) const;
    
    // Mark the operator as deprecated. 
    bool		 setOpDeprecated(const char *opname,
        		        const char *version, 
        		        const char *replacement = 0,
        		        std::ostream *err = nullptr);
    // Is the operator deprecated?
    bool 		 isOpDeprecated(const char *opname) const;
    
    // Retrieve the deprecation version and possible replacement operator.
    bool		 getDeprecationInfo(const char *opname,
        		        UT_StringHolder &version, 
        		        UT_StringHolder &replacement) const;
    // Display all the opdeprecate commands.
    void		 outputOpDeprecated(std::ostream &os) const;
    
    // Sets the default color for an operator type.
    bool		 setOpDefaultColor(const UT_StringHolder &optype,
				const UT_Color &clr,
        		        std::ostream *err = nullptr);
    // Gets the default color for the supplied operator type.
    UT_Color		 getOpDefaultColor(const UT_StringRef &optype) const;
    // Clear all the opdefaultcolor commands.
    void		 clearOpDefaultColors();
    // Display all the opdefaultcolor commands.
    void		 outputOpDefaultColors(std::ostream &os) const;

    // Sets the default shape for an operator type.
    bool		 setOpDefaultShape(const UT_StringHolder &optype,
				const UT_StringHolder &shape,
        		        std::ostream *err = nullptr);
    // Gets the default shape for the supplied operator type.
    const UT_StringHolder &getOpDefaultShape(const UT_StringRef &optype) const;
    // Clear all the opdefaultshape commands.
    void		 clearOpDefaultShapes();
    // Display all the opdefaultshape commands.
    void		 outputOpDefaultShapes(std::ostream &os) const;

    // Get or set the wire style for networks with this child node type.
    const UT_StringHolder &getDefaultWireStyle() const;
    void		 setDefaultWireStyle(const UT_StringHolder &wirestyle);

    // When no name is specified for a node, we typically use the type as the
    // prefix for the name.  However, we may want to provide a method for
    // creating alternate default names for operators.
    void		 getDefaultNodeName(const char *type, UT_String &name);

    // If there is an index file that contains scripted subnets for this op
    // table, this function returns a pointer to that file name.
    const char		*getScriptedSubnetIndex() const;

    // Notify those who are interested that our table contents have changed.
    // Don't do this in addOperator - only after bulk operations like
    // loadScriptOperators or OP_Netowkr::loadScriptedOpDefs.
    void		 notifyUpdateTableSinksOfUpdate();

    static inline int	 getLoadDSOFlag()
			 { return theLoadDSOFlag; }
    static inline void	 setLoadDSOFlag(int i)
			 { theLoadDSOFlag = i; }
    static unsigned	 getPermissionMask(const OP_Operator *op);
    static void		 clearPermissionMask(const OP_Operator *op);

    /// Called once all basic operator types are loaded to call the python
    /// code which will initialize node color and shape themes.
    static void		 initializeOpThemes();

    /// Builds (or rebuilds) the operator type namespace hierarchy.
    /// The optype precedense is given by the environment variable
    /// HOUDINI_OPTYPE_NAMESPACE_HIERARCHY, which is processed by this method.
    static void		 buildOpTypeNamespaceHierarchy();

    /// Obtains the value of the environment variable used to construct
    /// the hierarchy.
    static const char	*getOpTypeNamespaceHierarchyPref();

    /// Obtains a list of available operator names that have the same base 
    /// (core) name as the given operator.
    /// If scope network name is not NULL, the list includes only operators
    /// whose nodes can be created in that network (otherwise all
    /// operators are included).
    /// The list is sorted according to the descending precedence order.
    void		 getCandidateOperatorNamesInPrecedenceOrder(
				UT_StringArray &precedence_order,
				const char *op_name,
				const UT_StringArray *scope_network_stack);

    /// Obtains the preferred operator name that matches the given op_name.
    /// Any name component included in the op_name must match the returned op
    /// type name, and any component not present in op_name is assumed to match
    /// the returned op type. For example 'hda' will match any scope, namespace,
    /// or version, while 'userA::hda' will match any scope and version, but the
    /// namespace must be 'userA'. For global namespace use '::hda' and for
    /// versionless opname use 'hda::'. 
    /// If the scope_network_stack is also given (ie, non-null) then the
    /// returned opname must match one of the scopes listed in that array too.
    /// Returns the name of the highest precedence operator that matches 
    /// the given op_name.
    const char		*getPreferredOperatorName(const char *opname,
				const UT_StringArray *scope_network_stack);

    /// Returns true if the provided node name is "close enough" to the
    /// operator type name, english name, or first name to imply what the
    /// operator type is.
    bool		 nodeNameImpliesType(OP_Operator *op,
				const UT_String &node_name) const;

    /// Convenience functions for the second callback in setScriptCreator().
    static bool		 alwaysReuseScriptOperatorCallback(
				const OP_ScriptOperator *script_operator,
				const OP_OTLDefinition &new_definition);
    static bool		 neverReuseScriptOperatorCallback(
				const OP_ScriptOperator *script_operator,
				const OP_OTLDefinition &new_definition);

private:
    // Functions for adding or removing a table sink.
    void		 addUpdateTableSink(OP_UpdateTableSink *sink);
    void		 removeUpdateTableSink(OP_UpdateTableSink *sink);
    // Called when the operator is deleted.
    void		 notifyUpdateTableSinksOfDelete();

    // Functions for adding new operator types using an OP_OTLDefinition.
    OP_ScriptOperator	*addNewOperator(const OP_OTLDefinition &definition,
                     	        std::ostream *err = nullptr);
    int			 loadScriptIndexFile(OP_ScriptIndexFile &sif,
				bool checkdup);
    /// Returns the table's operator namespace hierarchy (ie, the hierarchy
    /// of operator definitions for the optype associated with this table).
    OP_OpNamespaceHierarchy &getOpNamespaceHierarchy()
			     { return myOpNamespaceHierarchy; }

    /// Returns the operator named 'opname' for instantiating a node
    /// inside the parent. If 'opname' is an ambiguous and unqualified opname 
    /// (ie core name witouth namespace), the preferred operator matching
    /// that opname is returned. However when loading a network or if
    /// the exact_name flag is true, then an operator with the exact 'opname'
    /// is returned (or NULL).
    OP_Operator		*getPreferredOperator(OP_Network *parent,
				const char *opname, bool exact_opname);

    UT_String		 myName;
    UT_String		 myScriptDirectory;
    UT_String		 myDefaultType;
    UT_String		 myScriptedSubnetIndex;
    UT_Color		 myDefaultColor;
    UT_StringHolder	 myDefaultShape;
    UT_StringHolder	 myDefaultWireStyle;
    OP_Operator		*myPrimarySubnet;
    int			 myOpCount;
    int			 myUniqueOpTypeID;
    static int		 theLoadDSOFlag;

    OP_ScriptCreatorFunc		 myScriptCreatorFunc;
    OP_ScriptCanReuseFunc		 myScriptCanReuseFunc;
    UT_ValArray<OP_ScriptIndexFile *>	 myScriptIndexFiles;
    UT_ValArray<OP_UpdateTableSink *>	 myUpdateTableSinks;
    
    // Data structure to store information on operators, without affecting
    // the operator definition itself. This also allows us to store overrides
    // for operators that have not been loaded yet.
    class OpInfo
    {
    public:
	OpInfo();
	bool isEmpty() const;

	OP_Operator		*myOperator;
	UT_StringHolder		 myCreationName;
	UT_StringHolder		 myOriginalEnglishName;

	// Deprecation support
	UT_StringHolder		 myDeprecatedVersion;
	UT_StringHolder		 myDeprecatedReplacement;

	// Flags
	bool			 myIsHidden:1;
	bool			 myIsExcluded:1;
    };

    UT_SymbolMap<OpInfo>		 myOperators;
    UT_SymbolMap<UT_StringHolder> 	 myOpAliases;
    UT_StringMap<UT_Color>		 myOpDefaultColors;
    UT_StringMap<UT_StringHolder>	 myOpDefaultShapes;

    OP_OpNamespaceHierarchy		 myOpNamespaceHierarchy;
    static UT_String			 theOpTypeNamespaceHierarchyPref;
    static bool				 theOpThemesInitialized;

    friend class	 OP_UpdateTableSink;
};

class OP_API OP_UpdateTableSink
{
public:
		 OP_UpdateTableSink()	{ }
    virtual 	~OP_UpdateTableSink()
		 { removeAllTableSinks(); }

    virtual void tableUpdated(OP_OperatorTable *table) = 0;
    virtual void tableDeleted(OP_OperatorTable *table)
		 { removeUpdateTableSink(table); }

protected:
    void	 addUpdateTableSink(OP_OperatorTable *table)
		 {
		     if( !table ) return;
		     table->addUpdateTableSink(this);
		     myOpTables.append(table, 1);
		 }
    void	 removeUpdateTableSink(OP_OperatorTable *table)
		 {
		     if( !table ) return;
		     table->removeUpdateTableSink(this);
		     myOpTables.findAndRemove(table);
		 }
    void	 removeAllTableSinks()
		 {
		     for( int i = myOpTables.entries(); i --> 0; )
			 removeUpdateTableSink(myOpTables(i));
		 }

private:
    OP_OperatorTableList	 myOpTables;
};

#endif
