/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_Group.h (GA Library, C++)
 *
 * COMMENTS:	Base class for groups.
 *
 */

#pragma once

#ifndef __GA_Group_h__
#define __GA_Group_h__

#include "GA_API.h"
#include "GA_Types.h"

#include <UT/UT_StringHolder.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>

class GA_Detail;
class GA_LoadMap;
class GA_SaveMap;

class UT_JSONParser;
class UT_JSONWriter;
class UT_WorkBuffer;
class UT_MemoryCounter;

class GA_API GA_Group
{
protected:
    GA_Group(const UT_StringHolder &name, bool internal, GA_GroupType type, bool detached);
public:
    virtual ~GA_Group();

    const UT_StringHolder &getName() const { return myName; }
    virtual const GA_Detail &getDetail() const = 0;

    bool isInternal() const { return myInternal; }
    bool getInternal() const { return myInternal; }
    void setInternal(bool onOff) { myInternal = onOff; }

    /// Detached groups are not owned by a GA_Detail.  This means
    /// they will not update if their GA_Detail's element count changes.
    /// It also means they must be explicitly deleted
    bool isDetached() const { return myDetached; }

    GA_GroupType	 classType(void) const
			    { return GA_GroupType(myType); }
    GA_GroupMaskType	 classMaskType() const
			    { return GA_GroupMaskType(1 << myType); }
    static const char	*groupType(GA_GroupType type)
			    { return GAgroupType(type); }
    static GA_GroupType	 groupType(const char *type)
			    { return GAgroupType(type); }

    bool isElementGroup() const
    {
        return (classMaskType() & (GA_GMASK_POINT | GA_GMASK_VERTEX | GA_GMASK_PRIMITIVE)) != 0;
    }

    /// Report memory usage
    /// NOTE: GA_ElementGroup DOES NOT include the memory for the attribute
    ///       unless the attribute is detached, because the GA_ElementGroup
    ///       only owns the attribute if it is detached.
    virtual int64	 getMemoryUsage(bool inclusive) const = 0;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    virtual void	 countMemory(UT_MemoryCounter &counter,
				bool inclusive) const = 0;

    virtual GA_Size	 entries() const	    = 0;

    SYS_FORCE_INLINE
    bool isEmpty() const
    {
        return entries() == 0;
    }
    virtual void 	 clear()		    = 0;
    virtual void	 addAll()		    = 0;
    virtual bool	 isOrdered() const	    = 0;
    virtual bool	 isMixed() const	    = 0;

    virtual bool	 combine(const GA_Group * inputGroup);

    void                 clearEntries() { clear(); }

    /// Fill out information about the group.  The default method fills does
    /// the following:
    /// @code
    ///	   info.sprintf("%s::%s", groupType(classType()), getName());
    ///    memory = 0;
    /// @endcode
    virtual bool	 stat(UT_WorkBuffer &info, uint level) const;

    /// Check to see whether group should be saved or not.
    /// The default behaviour checks the map options to see if the group should
    /// be saved (internal groups, specific types, etc.).
    virtual  bool	 jsonShouldSave(const GA_SaveMap &map) const;

    /// Each group saves its data in a different format.
    ///
    /// @section JSON-GA_Group JSON Schema: GA_Group
    /// The schema for a group consistes of an array of two entries.  The first
    /// entry contains the definition of the group.  The second is "private"
    /// data for the group.
    /// @code
    /// {
    ///     "name"        : "GA_Group",
    ///     "description" :
    ///           "A group is stored in an array of two items.  The first
    ///            item stores the definition (or run information).  The second
    ///            entry stores the private information for the group.",
    ///     "type"  : "array",
    ///     "items" : [
    ///        { "$ref" : "GA_Group-Definition" },  // Definition of primitive
    ///        { "$ref" : "GA_Group-Data" },        // Single primitive data
    ///     ],
    /// }
    /// @endcode
    ///
    /// @section JSON-GA_Group-Definition JSON Schema: GA_Group-Definition
    /// The group definition stores the information needed to create a group,
    /// but does not define the data associated with the group.
    /// @code
    /// {
    ///     "name"        : "GA_Group-Definition",
    ///     "description" : "Definition of a group",
    ///     "type"        : "orderedmap",
    ///     "properties":
    ///     {
    ///         "type":
    ///         {
    ///             "type" : "string"
    ///             "description"   : "Type of group (i.e. 'point')",
    ///         }
    ///         "name":
    ///         {
    ///             "type" : "string"
    ///             "description"   : "Name of group",
    ///         }
    ///         "internal":
    ///         {
    ///             "type"        : "boolean"
    ///             "description" : "Internal or externally visible group",
    ///             "optional"    : true,
    ///             "default"     : false,
    ///         }
    ///     },
    /// }
    /// @endcode
    bool	 jsonSaveDefinition(UT_JSONWriter &w, const char *type) const;

    /// Load the GA_Group-Definition
    static bool	 jsonLoadDefinition(UT_JSONParser &p,
			    UT_WorkBuffer &type, UT_WorkBuffer &name,
			    bool &internal);

    /// Save the private group data
    virtual bool	 jsonSaveData(UT_JSONWriter &w,
				const GA_SaveMap &map) const = 0;

    /// Load the private group data
    virtual bool	 jsonLoadData(UT_JSONParser &p,
				const GA_LoadMap &map) = 0;

    /// A convenience function to do a JSON dump of the group contents.
    void 	dump(std::ostream &os);
protected:
    /// Only called by GA_GroupTable::rename(const char *, const char *).
    /// Though sub-classes can override the behaviour
    virtual bool setName(const UT_StringHolder &n)
    {
        myName = n;
        return true;
    }

private:
    UT_StringHolder myName;
    unsigned myInternal:1,
             myType:5,
             myDetached:1;

    friend class GA_GroupTable;
};
#endif
