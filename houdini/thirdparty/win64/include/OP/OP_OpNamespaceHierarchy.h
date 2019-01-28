/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_OpNamespaceHierarchy.h ( OP Library, C++)
 *
 * COMMENTS:	A class that manages the hierarchy (precedence order)
 *		of the operator type namespaces.
 */

#ifndef __OP_OpNamespaceHierarchy__
#define __OP_OpNamespaceHierarchy__

#include "OP_API.h"
#include <UT/UT_ArrayStringMap.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_UniquePtr.h>

class UT_StringArray;
class op_OpPrecedenceList;


/// Implements an operator name resolution mechanism for unquallified (or
/// not fully qualified) operator names (ie, finds a full operator name with a
/// namespace and version tags, given a partial operatorn name, like just the
/// core name, or core name with a namespace but no version, where several
/// candidates may exist).
class OP_API OP_OpNamespaceHierarchy : UT_NonCopyable
{
public:
    // Constructor and destructor.
    		    OP_OpNamespaceHierarchy();
    		    ~OP_OpNamespaceHierarchy();

    /// Builds the operator definition hierarchy.
    ///
    /// @param	hierarchy   The string enumerating the namespace hierarchy.
    /// @parm	table_name  The name of the operator table for which the
    ///			    hierarchy is build. 
    void	    buildHierarchy( const char *hierarchy,
				    const char *table_name );

    /// Updates the hierarchy when an operator has been added to the table.
    void	    handleOperatorAdded( const char *op_name );
    
    /// Updates the hierarchy when an operator has been removed from the table.
    void	    handleOperatorRemoved( const char *op_name );

    /// Resolves a given optype name to a preferred optype name, in case
    /// the given name is ambiguous and has several operator definition 
    /// candidates. For fully qualified or unambiguous names the method
    /// will return the given argument.
    ///
    /// @param	op_name		The operator type name to resolve.
    /// @param	network_scope_stack	The array of subnet optype names (
    ///				with the table name) inside which the operator 
    ///				node would be created. The first element in the
    ///				array is the immediate parent, the second is
    ///				the granparent subnet, etc).
    ///
    /// @return	The preferred operator type name for the given name.
    const char *    findPreferredOpName( const char *op_name,
				const UT_StringArray *network_scope_stack);

    /// Obtains a list of available operator names that have the same base 
    /// (core) name as the given operator.
    /// If scope network name is not NULL, the list includes only operators
    /// whose nodes can be created in one of these networks (since they
    /// represent the stack of nested scopes). Otherwise all operators are 
    /// included.
    /// The list is sorted according to the descending precedence order.
    void	    getPrecedenceOrder( UT_StringArray &precedence_order,
				const char *op_name,
				const UT_StringArray *network_scope_stack);
private:
    /// @{
    /// Helper method to access the precedence list for an operator.
    op_OpPrecedenceList * getPrecedenceList( const char *op_name );
    op_OpPrecedenceList * getOrCreatePrecedenceList( const char *op_name );
    /// @}

    void	    buildPreferenceOrder( const char *hierarchy,
					  const char *table_name );

    void	    appendPrefIfApplicable( const char * pref, 
					    const char * table_name );


private:
    /// Map from the core operator name to the list of operators ordered
    /// in descending preference order.
    UT_ArrayStringMap<UT_UniquePtr<op_OpPrecedenceList>> myPreferenceLists;

    /// A preference order list (list of op_PrefEntry objects)
    /// constructed from the HOUDINI_OPNAMESPACE_HIERARCHY envvar. This list
    /// represents the user's wishes as to which operators should take
    /// precedence over which other ones. When sorting real operators
    /// according to their precedence, we use this list as a template
    /// against which we test which op should appear first in that precedence
    /// list.
    class op_PrefEntry;
    UT_Array<op_PrefEntry>	myPrefOrderList;

    friend class op_OpPrefEntryComparator;
};

// ===========================================================================
/// Class representing an entry in an preference list.
/// It is different from op_OpEntry because it does not represent an operator
/// from the op table, but rather a pattern against which operators are 
/// compared (many operators may match that entry, but also the pattern may 
/// be narrow enough that it  matchs only one operator).
class OP_OpNamespaceHierarchy::op_PrefEntry
{
public:
    op_PrefEntry(const UT_StringHolder &scope_pattern, 
		 const UT_StringHolder &namespace_pattern,
		 const UT_StringHolder &core_name_pattern,
		 const UT_StringHolder &version_pattern);

    /// Returns true if the given op entry matches this preference entry.
    bool matches(const UT_WorkBuffer &scope_opname, 
		 const UT_WorkBuffer &namespace_name,
		 const UT_WorkBuffer &core_name, 
		 const UT_WorkBuffer &version) const;

private:
    // patterns for matching the operator name components
    UT_StringHolder myScopePattern;
    UT_StringHolder myNamespacePattern;
    UT_StringHolder myCoreNamePattern;
    UT_StringHolder myVersionPattern;
};

#endif

