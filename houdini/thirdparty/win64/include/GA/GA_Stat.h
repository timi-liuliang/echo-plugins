/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_Stat.h ( GA Library, C++)
 *
 * COMMENTS:	Information filled out by stat() operation on GA_Detail.
 *
 */

#ifndef __GA_Stat__
#define __GA_Stat__

#include "GA_API.h"

#include "GA_Types.h"

#include <UT/UT_Array.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_Options.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_ArrayStringMap.h>

class GA_IO;

class UT_WorkBuffer;


/// Enum to define different levels of verbosity for GA_Detail::stat()
enum GA_STAT_LEVEL
{
    /// Fill minimal information (element counts).  When accessing a file with
    /// this verbosity level, it's possible that the file doesn't need to be
    /// completely loaded.
    GA_STAT_BRIEF	= 0x00,

    /// Fill out attribute & group information.  When accessing a file with
    /// this verbosity level, the library may perform simple parsing (without
    /// intepreting the all the data).
    GA_STAT_NORMAL	= 0x01,

    /// Compute primitive type and bounding box information.
    GA_STAT_FULL	= 0x02,
};

/// @brief Class to return information about a GA_Detail
///
/// This class is filled out by the GA_Detail::stat() methods.
class GA_API GA_Stat 
{
public:
     GA_Stat();
    ~GA_Stat();
    
    /// Simple class to store attribute definition (without data)
    class ga_StatAttribute
    {
    public:
	ga_StatAttribute(const char *name="",
			const char *type_name="",
			GA_AttributeScope scope = GA_SCOPE_PUBLIC,
			GA_TypeInfo typeinfo = GA_TYPE_VOID,
			int tuple_size = 1,
			const char *description="")
	    : myName(name)
	    , myTypeName(type_name)
	    , myScope(scope)
	    , myTypeInfo(typeinfo)
	    , myTupleSize(tuple_size)
	    , myOptions()
	    , myDescription(description)
	{
	}
	~ga_StatAttribute() {}

	const char		*getName() const	{ return myName; }
	const char		*getTypeName() const	{ return myTypeName; }
	GA_AttributeScope	 getScope() const	{ return myScope; }
	GA_TypeInfo		 getTypeInfo() const	{ return myTypeInfo; }
	int			 getTupleSize() const	{ return myTupleSize; }
	const UT_Options	&getOptions() const	{ return myOptions; }
	const char		*getDescription() const	{ return myDescription; }

	void	setName(const UT_StringHolder &n)	{ myName = n; }
	void	setTypeName(const UT_StringHolder &n)	{ myTypeName = n; }
	void	setScope(GA_AttributeScope s)	{ myScope = s; }
	void	setTypeInfo(GA_TypeInfo t)	{ myTypeInfo = t; }
	void	setTupleSize(int ts)		{ myTupleSize = ts; }
	void	setOptions(const UT_Options &o)	{ myOptions = o; }
	void	setDescription(const UT_StringHolder &d) { myDescription = d; }

    private:
	UT_StringHolder		myName;
	UT_StringHolder		myTypeName; // Type name
	GA_AttributeScope	myScope;
	GA_TypeInfo		myTypeInfo;
	UT_Options		myOptions;
	UT_StringHolder		myDescription;
	int			myTupleSize;
    };

    class ga_StatGroup
    {
    public:
	// If the number of entries in the group is unknown, entries can be -1.
	ga_StatGroup(const char *name="",
		    GA_Size entries=-1,
		    const char *description="")
	    : myName(name)
	    , myEntries(entries)
	    , myDescription(description)
	{}
	~ga_StatGroup() {}

	const char	*getName() const	{ return myName; }
	GA_Size		 entries() const	{ return myEntries; }
	const char	*getDescription() const	{ return myDescription; }

	void	setName(const UT_StringHolder &n)	{ myName = n; }
	void	setEntries(GA_Size e)			{ myEntries = e; }
	void	setDescription(const UT_StringHolder &n) { myDescription = n; }
    private:
	UT_StringHolder	myName;
	UT_StringHolder	myDescription;
	GA_Size		myEntries;
    };

    class ga_StatVolume
    {
    public:
	ga_StatVolume(GA_Index idx = -1, 
		      const char *name="", int rx=0, int ry=0, int rz=0)
	    : myIndex(idx),
	      myName(name),
	      myResX(rx),
	      myResY(ry),
	      myResZ(rz)
	{}
	~ga_StatVolume() {}

	const char	*getName() const	{ return myName; }
	const UT_StringHolder	&getNameHolder() const	{ return myName; }
	GA_Index	 getIndex() const	{ return myIndex; }
	GA_Size		 xres() const		{ return myResX; }
	GA_Size		 yres() const		{ return myResY; }
	GA_Size		 zres() const		{ return myResZ; }

	void	setName(const UT_StringHolder &n)	{ myName.harden(n); }
	void	setIndex(GA_Index idx)		{ myIndex = idx; }
	void	setRes(GA_Size rx, GA_Size ry, GA_Size rz)
			{ myResX = rx; myResY = ry; myResZ = rz; }
    private:
	UT_StringHolder	myName;
	GA_Index	myIndex;
	GA_Size		myResX, myResY, myResZ;
    };

    void	clear()
		{
		    setPointCount(0);
		    setVertexCount(0);
		    setPrimitiveCount(0);
		    clearAllAttributes();
		    clearAllGroups();
		    clearPrimitiveTypes();
		    clearVolumes();
		}

    /// @{
    /// Get/Set the number of elements in the detail
    /// Availability:  level >= GA_STAT_BRIEF
    GA_Size	getPointCount() const		{ return myPointCount; }
    GA_Size	getVertexCount() const		{ return myVertexCount; }
    GA_Size	getPrimitiveCount() const	{ return myPrimitiveCount; }

    void	setPointCount(GA_Size sz)	{ myPointCount = sz; }
    void	setVertexCount(GA_Size sz)	{ myVertexCount = sz; }
    void	setPrimitiveCount(GA_Size sz)	{ myPrimitiveCount = sz; }
    /// @}

    /// @{
    /// Attribute status.
    /// Availability: level >= GA_STAT_NORMAL
    void	clearAllAttributes()
		{
		    clearAttributes(GA_ATTRIB_VERTEX);
		    clearAttributes(GA_ATTRIB_POINT);
		    clearAttributes(GA_ATTRIB_PRIMITIVE);
		    clearAttributes(GA_ATTRIB_GLOBAL);
		}
    void	clearAttributes(GA_AttributeOwner owner)
		{
		    myAttributes[owner].clear();
		}
    void	appendAttribute(GA_AttributeOwner owner,
			const ga_StatAttribute &attrib)
		{
		    myAttributes[owner].append(attrib);
		}
    GA_Size	getAttributeCount(GA_AttributeOwner owner) const
		{
		    return myAttributes[owner].entries();
		}
    const ga_StatAttribute	&getAttribute(GA_AttributeOwner owner,
					GA_Size i) const
		{
		    return myAttributes[owner](i);
		}
    /// @}

    /// @{
    /// Group information.
    /// Availability: level >= GA_STAT_NORMAL
    void	clearAllGroups()
		{
		    clearGroups(GA_GROUP_POINT);
		    clearGroups(GA_GROUP_PRIMITIVE);
		    clearGroups(GA_GROUP_EDGE);
		    clearGroups(GA_GROUP_BREAKPOINT);
		    clearGroups(GA_GROUP_VERTEX);
		}
    void	clearGroups(GA_GroupType gtype)
		{
		    myGroups[gtype].setCapacity(0);
		}
    void	appendGroup(GA_GroupType gtype, const ga_StatGroup &group)
		{
		    myGroups[gtype].append(group);
		}
    GA_Size	getGroupCount(GA_GroupType gtype) const
		{
		    return myGroups[gtype].entries();
		}
    const ga_StatGroup	&getGroup(GA_GroupType gtype, GA_Size i) const
		{
		    return myGroups[gtype](i);
		}
    /// @}


    /// Get the info block options which were set by the stat() operation
    /// Special options that will be used for quick stats (when the stat level
    /// is STAT_LEVEL_BRIEF.
    ///   - string primcount_summary
    ///   - string attribute_summary
    ///   - string group_summary
    /// If possible, the file format should set these from any header
    /// information.
    const UT_Options	&getInfoOptions() const	{ return myInfoOptions; }

    /// Set the info block options stored in the file
    /// If a float array option is found with the @c bounds_name, it will be
    /// used to initialize the bounding box of the stat.
    ///
    /// The file stat should set these options if possible.
    void		 setInfoOptions(const UT_Options &info,
			    const char *bounds_name="bounds");

    /// @{
    /// Primitive interface
    /// Availability: level >= GA_STAT_FULL
    void	clearPrimitiveTypes()
		{
		    myPrimitiveTypes.clear();
		}
    void	appendPrimitive(const UT_StringHolder &type_name)
		{
		    myPrimitiveTypes[type_name]++;
		}
    void	setPrimitiveCount(const UT_StringHolder &type_name,
			GA_Size count)
		{
		    myPrimitiveTypes[type_name] = count;
		}
    GA_Size	getPrimitiveTypeCount(const UT_StringRef &type_name) const
		{
		    auto it = myPrimitiveTypes.find(type_name);
		    if (it == myPrimitiveTypes.end())
			return 0;
		    return it->second;
		}
    void	getPrimitiveTypes(UT_StringArray &type_names) const
		{
		    type_names.clear();
		    for (auto it = myPrimitiveTypes.begin(); 
			it != myPrimitiveTypes.end(); ++it)
		    {
			type_names.append(it->first);
		    }
		    type_names.sort();
		}
    /// @}

    /// @{
    /// Volume interface
    /// Availability: level >= GA_STAT_FULL
    void	clearVolumes()
		{
		    myVolumes.clear();
		}
    void	appendVolume(GA_Index idx, const char *name, int rx, int ry, int rz)
		{
		    myVolumes.append(ga_StatVolume(idx, name, rx, ry, rz));
		}
    int		getNumVolumes() const { return myVolumes.entries(); }
    const ga_StatVolume &getVolume(int idx) const { return myVolumes(idx); }
    /// @}

    /// @{
    /// Get bounding box
    /// Availability: level >= GA_STAT_FULL
    const UT_BoundingBox	&getBounds() const	{ return myBox; }
    void	setBounds(const UT_BoundingBox &b)	{ myBox = b; }
    /// @}

    /// Common output
    void	output(UT_WorkBuffer &buffer, GA_STAT_LEVEL level,
			bool pretty_print) const;

    /// Output attribute information only.  This appends to the work buffer.
    void	outputAttributes(UT_WorkBuffer &buffer,
			bool pretty_print) const;
    /// Output attribute information only.  This appends to the work buffer
    void	outputGroups(UT_WorkBuffer &buffer,
			bool pretty_print) const;
    /// Output volume information only.  This appends to the work buffer
    void	outputVolumes(UT_WorkBuffer &buffer,
			bool pretty_print) const;

    /// @{
    /// Set/get the GA_IO which performed the stat
    void	 setIO(const GA_IO *io)	{ myIO = io; }
    const GA_IO	*getIO() const		{ return myIO; }


private:
    GA_Size	myPointCount;
    GA_Size	myVertexCount;
    GA_Size	myPrimitiveCount;

    UT_Array<ga_StatAttribute>	myAttributes[GA_ATTRIB_OWNER_N];
    UT_Array<ga_StatGroup>	myGroups[GA_GROUP_N];
    UT_Array<ga_StatVolume>	myVolumes;

    UT_ArrayStringMap<GA_Size>	myPrimitiveTypes;
    UT_Options			myInfoOptions;
    UT_BoundingBox		myBox;

    const GA_IO			*myIO;
};

#endif
