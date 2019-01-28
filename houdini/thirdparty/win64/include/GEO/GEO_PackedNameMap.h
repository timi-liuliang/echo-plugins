/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_PackedNameMap.h (GEO Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GEO_PackedNameMap__
#define __GEO_PackedNameMap__

#include "GEO_API.h"
#include "GEO_PackedTypes.h"
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_StringMMPattern.h>
#include <GA/GA_Types.h>
#include <GA/GA_SharedLoadData.h>

class UT_JSONWriter;
class UT_JSONParser;
class UT_MemoryCounter;

class GEO_PackedNameMap;
using GEO_PackedNameMapPtr = UT_IntrusivePtr<GEO_PackedNameMap>;

/// Map to translate from Alembic attribute names to Houdini names
class GEO_API GEO_PackedNameMap
    : public UT_IntrusiveRefCounter<GEO_PackedNameMap>
{
public:
    typedef UT_SymbolMap<UT_String>	MapType;

    class LoadContainer : public GA_SharedLoadData
    {
    public:
	LoadContainer(const char* key): myKey(key) {}
	virtual ~LoadContainer() {}

	virtual UT_StringHolder getSharedDataKey() const
	{
	    return myKey;
	}

	UT_StringHolder myKey;
	GEO_PackedNameMapPtr myNameMap;
    };

     GEO_PackedNameMap();
    ~GEO_PackedNameMap();

    /// Compare equality
    bool	isEqual(const GEO_PackedNameMap &src) const;

    /// Compute a hash for the name map
    uint	hash() const;

    /// Is the name map default (allow all attributes and no renaming)
    bool	isDefault() const
    {
	for (int i = 0; i < GA_ATTRIB_OWNER_N; ++i)
	    if (myPatterns[i] != "*")
		return false;
	return myMap.entries() == 0 && myTypeInfoMap.entries() == 0;
    }

    /// @{
    /// Equality operator
    bool	operator==(const GEO_PackedNameMap &src) const
		    { return isEqual(src); }
    bool	operator!=(const GEO_PackedNameMap &src) const
		    { return !isEqual(src); }
    /// @}

    /// Get the name mapping.  If the name isn't mapped, the original name
    /// will be returned.
    /// If the attribute should be skipped, a NULL pointer will be returned.
    const char	*getName(const char *name) const;
    const char	*getName(const std::string &name) const
			{ return getName(name.c_str()); }
    /// Get the typeinfo mapping.  If the name isn't mapped, nullptr will be
    /// returned.
    const char	*getTypeInfo(const char *name) const;
    const char	*getTypeInfo(const std::string &name) const
			{ return getTypeInfo(name.c_str()); }

    /// Add a translation from the abcName to the houdini attribute name
    void		 addMap(const char *abcName, const char *houdiniName);

    /// Add a translation from the abcName to the attribute's type info
    void		 addTypeInfo(const char *abcName, const char *typeInfo);

    /// Avoid adding an attribute of the given name.  This is done by 
    void		 skip(const char *abcName)
			    { addMap(abcName, NULL); }

    /// Set a pattern for load attributes.  Only attributes which match the
    /// given pattern should be created.  The default pattern is "*" (allowing
    /// all attributes).
    void		setPattern(GA_AttributeOwner owner,
				const char *pattern);

    /// Return the attribute type pattern
    const char		*pattern(GA_AttributeOwner owner) const
			    { return myPatterns[owner].nonNullBuffer(); }

    /// Check to see whether an attribute can be added.  This matches the
    /// pattern for the given owner.
    bool		matchPattern(GA_AttributeOwner owner,
				const char *name) const;

    /// @{
    /// JSON I/O.  It's up to the caller to determine which version of the name
    /// map to load.  This is typically done by using a different keyword when
    /// loading/saving.
    ///
    /// - Version 1: @n
    ///  Saves a single map for the attribute name map
    /// - Version 2: @n
    ///   Saves a keyword map with different components.  The name map along
    ///   with the patterns.
    bool	save(UT_JSONWriter &w, int version=2) const;
    static bool	load(GEO_PackedNameMapPtr &map, UT_JSONParser &p, int version);
    /// @}

    /// Report memory usage (includes all shared memory)
    int64 getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    void	dump() const;

private:
    UT_String		myPatterns[GA_ATTRIB_OWNER_N];
    UT_StringMMPattern	myCompiled[GA_ATTRIB_OWNER_N];
    MapType		myMap;
    MapType		myTypeInfoMap;
};

#endif
