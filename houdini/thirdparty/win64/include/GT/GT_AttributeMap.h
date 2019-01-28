/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_AttributeMap.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_AttributeMap__
#define __GT_AttributeMap__

#include "GT_API.h"
#include "GT_Handles.h"
#include "GT_Types.h"

#include <UT/UT_ArrayStringMap.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringHolder.h>

class UT_WorkBuffer;

/// @brief Used to filter out entries when constructing an attribute map.
class GT_API GT_AttributeMapFilter
{
public:
    GT_AttributeMapFilter();
    virtual ~GT_AttributeMapFilter();
    /// Override this method when creating a sub-set of a attribute map
    /// @param name  @n The name of the attribute
    /// @param index @n The attributes index in the current map
    virtual bool	accept(const UT_StringHolder &name, int index) const = 0;
};

/// @brief A symbol table for attribute data
///
/// Attribute data is stored in two structures.  The GT_AttributeMap keeps a
/// mapping of names to integer values.  The integer values are the offsets
/// into the GT_AttributeList, which stores the actual data.
class GT_API GT_AttributeMap
    : public UT_IntrusiveRefCounter<GT_AttributeMap>
{
public:
    typedef UT_ArrayStringMap<UT_StringHolder>	ExportMapType;
    typedef ExportMapType::const_iterator	const_export_iterator;
    typedef UT_ArrayStringMap<int>		NamesMapType;
    typedef NamesMapType::const_iterator	const_names_iterator;

    /// Default constructor
    explicit GT_AttributeMap(int capacity = 0);
    /// Copy constructor
    GT_AttributeMap(const GT_AttributeMap &src);
    /// Destructor
    ~GT_AttributeMap();

    /// Create a new attribute map by filtering the entries.  Each entry in the
    /// @c src is filtered by the @c filter and if accepted, added to the new
    /// map.
    static const GT_AttributeMapHandle filter(
					const GT_AttributeMapHandle &src,
					const GT_AttributeMapFilter &filter);

    /// Compare two maps
    bool	operator==(const GT_AttributeMap &map) const;
    bool	operator!=(const GT_AttributeMap &map) const
		    { return !(*this == map); }

    /// Clear the map
    void	clear();

    /// How many symbols are there
    int		entries() const	{ return myNames.entries(); }

    /// Return approximate memory used by map
    int64	getMemoryUsage(bool inclusive) const;

    /// Find the symbol index (or -1 if no symbol exists)
    int		get(const UT_StringRef &name) const
		{
		    auto item = myNamesMap.find(name);
		    return (item != myNamesMap.end()) ? item->second : -1;
		}

    /// Test if a name exists
    bool	hasName(const UT_StringRef &name) const
			{ return myNamesMap.contains(name); }

    /// Find the name for the given index
    const UT_StringHolder	&getName(int index) const
				    { return myNames(index); }

    /// Get a label which includes the export name and other properties
    /// This returns the string stored in the @c label_storage
    const char	*getLabel(int index, UT_WorkBuffer &label_storage) const;

    /// @{
    /// Add export mapping (from parameter name to export name)
    void	 addExportName(int idx, const UT_StringHolder &export_name)
			{ addExportName(getName(idx), export_name); }
    void	 addExportName(const UT_StringHolder &name,
			const UT_StringHolder &export_name);
    /// @}
    /// Get the exported name for a parameter
    const UT_StringHolder	&getExportName(int index) const;

    /// Since GT can move attribute storage classes around, especially compared
    /// with GEO, it's sometimes useful to know what the original attribute
    /// owner was.  This is not always guaranteed to be set to something other
    /// that GT_OWNER_INVALID, but it will likely be set if the attribute
    /// originates from a GA_Attribute.
    GT_Owner	getOriginalOwner(int idx) const	{ return myOwners(idx); }

    /// Set the original owner
    void	setOriginalOwner(int i, GT_Owner o)	{ myOwners(i) = o; }

    /// Get the symbol names
    const UT_StringArray	&getNames() const { return myNames; }

    /// Add a new symbol to the index.  If this is a duplicate symbol, setting
    /// @c replace_existing will return the previous index.  Otherwise, -1 will
    /// be returned.
    int		add(const UT_StringHolder &name, bool replace_existing);

    /// Access to a symbol table traverser.
    const_names_iterator	begin()	const { return myNamesMap.begin(); }

private:
    // Remove a symbol, returning the index that was removed, or -1.
    // @warning  If the map is referenced by an attribute lists, this will
    // cause the arrays in the attribute list to be mis-matched.
    int		remove(const UT_StringRef &name);
    friend class GT_AttributeList;	// Access to remove()
    
    ExportMapType	myExportMap;
    UT_StringArray	myNames;
    NamesMapType	myNamesMap;
    UT_Array<GT_Owner>	myOwners;
};

#endif

