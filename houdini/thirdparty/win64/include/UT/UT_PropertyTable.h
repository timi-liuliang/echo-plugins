/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	(UT_PropertyTable.h, C++)
 *
 * COMMENTS:
 */

#ifndef _UT_PropertyTable_
#define _UT_PropertyTable_

#include "UT_API.h"
#include "UT_Options.h"
#include "UT_Map.h"
#include "UT_SharedPtr.h"
#include "UT_SymbolTable.h"

/// This class represents a table of (mantra) properties. A property consists
/// of a name (the name used by mantra), a parm name (the name of the houdini
/// parameter that corresponds to the mantra property), the data type of the
/// property, and the default value of the property.
///
/// This information is somewhat scattered right now, with the parameter
/// definition in the $HH/properties/soho/IFDmantra.ds file, the translation
/// from houdini to mantra names in $HH/soho/base/IFDmantra.py, and the
/// defaults as mantra understands them (which should but may not match
/// the defaults in the parameter definitions) in $RAY/RAY_Settings.C.
/// At some point all references to render properties should be through
/// this class.
///
/// The property table is loaded from a JSON array where each item in the array
/// is a dictionary defining a property.  The items to define the property are:
///	- "string class" (required): Specifies the rendering class of the
///	   property (i.e. "renderer", "object"  or "option")
///	- "string name" (required): The renderer name for the property
///	- "string hname" (required): The name of the Houdini parameter
///	- "<type> default" (required): The default value.  This may be one of:
///		- bool
///		- number
///		- string
///		- array of numbers
///		- array of strings
///	- "string label" (optional): The label of the Houdini parameter
///	- "float min" (optional): Suggested minimum value for numeric types
///	- "float max" (optional): Suggested maximum value for numeric types
///
class UT_API UT_PropertyTable
{
public:
    using OptionEntryPtr = UT_SharedPtr<UT_OptionEntry>;

    UT_PropertyTable(const char *table_name);
    UT_PropertyTable(UT_JSONParser &p,
		    const char *filename=nullptr);
    ~UT_PropertyTable();

    /// Return the load status of the table in the c-tor.
    bool			 loadStatus() const { return myLoadStatus; }

    SYS_DEPRECATED(16.0)	// Use size()
    int				 getNumProps() const { return size(); }
    SYS_DEPRECATED(16.0)	// Use findRName()
    int				 getPropIndex(const UT_StringRef &name)
					{ return findRName(name); }

    SYS_DEPRECATED(16.0)
    const UT_StringHolder	&getPropName(int i) const { return rname(i); }
    SYS_DEPRECATED(16.0)
    const UT_StringHolder	&getPropLabel(int i) const { return label(i); }
    SYS_DEPRECATED(16.0)
    const UT_StringHolder	&getPropParm(int i) const { return hname(i); }
    SYS_DEPRECATED(16.0)
    OptionEntryPtr		 getPropDefault(int i) const { return value(i);}
    SYS_DEPRECATED(16.0)
    fpreal			 getMin(int i) const { return minValue(i); }
    SYS_DEPRECATED(16.0)
    fpreal			 getMax(int i) const { return maxValue(i); }

    /// Find a property by the renderer property name.  This will find names
    /// like "global:lod", but also "lod".  The first usage (with class
    /// specifier) avoids name aliasing between different rendering classes.
    /// The method returns @c -1 if the property isn't found.
    int			findRName(const UT_StringRef &name) const;
    /// Find property by the Houdini parameter name
    /// The method returns @c -1 if the parameter isn't found.
    int			findHName(const UT_StringRef &name) const;

    /// Number of properties stored in the table
    exint			 size() const { return myProperties.size(); }
    /// Return the class of the rendering property (i.e. "object")
    const UT_StringHolder	&rclass(int i) const;
    /// Return the name of the rendering property (i.e. "coving")
    const UT_StringHolder	&rname(int i) const;
    /// Return the name of the houdini parameter (i.e. "vm_coving")
    const UT_StringHolder	&hname(int i) const;
    /// Return the label of the houdini parameter (i.e. "Coving")
    const UT_StringHolder	&label(int i) const;
    /// Return the default value of the houdini parameter (i.e. 1)
    OptionEntryPtr		 value(int i) const;
    /// @{
    /// For numeric values, return the expected range of the values.  These is
    /// typically a suggested range.
    fpreal			 minValue(int i) const;
    fpreal			 maxValue(int i) const;
    /// @}

    /// Debug the settings
    bool	save(UT_JSONWriter &w, const UT_IntArray *select=nullptr) const;
    void	dump(const UT_IntArray *select=nullptr) const;

    /// Relatively lightweight representation of a subset of a UT_PropertyList.
    class PropertySubsetList
    {
    public:
	 PropertySubsetList(const UT_PropertyTable &table)
	     : myTable(table)
	 {
	 }
	~PropertySubsetList() {}

	/// @{
	/// Interface to properties
	exint			 size() const	{ return myProperties.size(); }
	const UT_StringHolder	&rclass(int i) const
				    { return myTable.rclass(map(i)); }
	const UT_StringHolder	&rname(int i) const
				    { return myTable.rname(map(i)); }
	const UT_StringHolder	&hname(int i) const
				    { return myTable.hname(map(i)); }
	const UT_StringHolder	&label(int i) const
				    { return myTable.label(map(i)); }
	OptionEntryPtr		 value(int i) const
				    { return myTable.value(map(i)); }
	fpreal			 minValue(int i) const
				    { return myTable.minValue(map(i)); }
	fpreal			 maxValue(int i) const
				    { return myTable.maxValue(map(i)); }

	bool	save(UT_JSONWriter &w) const
		    { return myTable.save(w, &myProperties); }
	void	dump() const
		    { return myTable.dump(&myProperties); }
	/// @}

	/// Get the index in the parent table
	exint	parentIndex(int i) const	{ return map(i); }

	/// Add a property using the index from the base table
	void	appendProperty(int id)	{ myProperties.append(id); }

    private:
	int	map(int i) const
	{
	    return i >= 0 && i < myProperties.size() ? myProperties(i) : -1;
	}
	const UT_PropertyTable	&myTable;
	UT_IntArray		 myProperties;
    };

    const PropertySubsetList	&getSubset(const UT_StringRef &name)
				    { return getSubset(&name, 1); }
    const PropertySubsetList	&getSubset(const UT_StringRef names[],
					    exint arraylen);

private:
    bool			 doInclude(const UT_StringRef &filename);
    bool			 load(UT_IStream &is);
    bool			 load(UT_JSONParser &p,
					const char *filename=nullptr);
    const PropertySubsetList	&buildSubset(const UT_StringRef names[],
					    exint arraylen,
					    const UT_StringHolder &key);

    using PropertySubsetListMap = UT_Map<UT_StringHolder, PropertySubsetList>;
    class PropDef
    {
    public:

	PropDef(const UT_StringHolder &rclass,	// Render class ("object")
		const UT_StringHolder &rname,	// Renderer name ("coving")
		const UT_StringHolder &hname,	// Houdini name ("vm_coving")
		const UT_StringHolder &label,	// Parm label ("Coving")
		const OptionEntryPtr &value,	// Default value
		fpreal min=-SYS_FPREAL_MAX,	// Min/Max range
		fpreal max= SYS_FPREAL_MAX)
	    : myRClass(rclass)
	    , myRName(rname)
	    , myHName(hname)
	    , myLabel(label)
	    , myValue(value)
	    , myMin(min)
	    , myMax(max)
	{
	    if (!myLabel)
		myLabel = myRName;
	}

	/// Saves out a dictionary for the property
	bool	save(UT_JSONWriter &w,
		    UT_Int64Array &istore,
		    UT_Fpreal64Array &fstore,
		    UT_StringArray &sstore) const;

	const UT_StringHolder	&rclass() const	{ return myRClass; }
	const UT_StringHolder	&rname() const	{ return myRName; }
	const UT_StringHolder	&hname() const	{ return myHName; }
	const UT_StringHolder	&label() const	{ return myLabel; }
	const OptionEntryPtr	&value() const	{ return myValue; }
	fpreal			 minValue() const	{ return myMin; }
	fpreal			 maxValue() const	{ return myMax; }

	UT_StringHolder myRClass;
	UT_StringHolder myRName;
	UT_StringHolder myHName;
	UT_StringHolder myLabel;
	OptionEntryPtr	myValue;
	fpreal		myMin;
	fpreal		myMax;
    };

    UT_Array<PropDef>		myProperties;
    UT_SymbolMap<int>		myRNameMap;
    UT_SymbolMap<int>		myHNameMap;
    PropertySubsetListMap	mySubsets;
    bool			myLoadStatus;
};

#endif
