/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_InfoTree.h
 *
 * COMMENTS: 	This class is a tree implemented recursively (i.e. each node in
 * 		the tree is itself another tree). Each UT_InfoTree has a name, 
 * 		an array of property arrays, and finally an array of children 
 * 		which are also UT_InfoTree objects.
 *
 * 		Put simply, this is a tree in which each node can have as many 
 * 		children as it likes, and at each node we can store data in the 
 * 		form of strings (which are stored in arrays/rows, in case we 
 * 		want to display each row in a spreadsheet table).		
 *
 * 		SEE UT_InfoTree.C doc for how to create an UT_InfoTree.
 *
 * 		NOTE: Each direct child must have a unique name amongst the
 * 		      other children directly beneath us- see declaration of 
 * 		      addChildBranch() for more details.
 *
 */

#ifndef __UT_InfoTree_h__
#define __UT_InfoTree_h__

#include "UT_API.h"
#include "UT_FloatArray.h"
#include "UT_Array.h"
#include "UT_StringArray.h"
#include "UT_WorkBuffer.h"
#include <SYS/SYS_Types.h>
#include <iosfwd>

class UT_StringHolder;

#define	 TREE_TOP_DEFAULT_NAME	"Head of Tree"

////////////////////////////////////////////////////////////////////////////////
//
//	NAME: ut_PropertyRow
//
//	Simply a UT_StringArray, except we provide an append method which takes
//	an int as a parameter to make the life of our clients easier
//
////////////////////////////////////////////////////////////////////////////////

class UT_API ut_PropertyRow : public UT_StringArray
{
public:
    ut_PropertyRow();
    ~ut_PropertyRow();

    void append(const UT_StringHolder &property);
    void append(const UT_String &property);
    void append(const char *property);

    template<class ValType>
    void append(const ValType &property);

    int  appendSprintf(const char *fmt, ...) SYS_PRINTF_CHECK_ATTRIBUTE(2, 3);
};

////////////////////////////////////////////////////////////////////////////////
//
//	NAME: UT_InfoTree
//
///  	This is a tree in which each node can have as many children as it likes,
///	and at each node we can store data in the form of strings (which are 
///	stored in arrays/rows, in case we want to display each row in a 
///	spreadsheet table).		
//
////////////////////////////////////////////////////////////////////////////////

class UT_API UT_InfoTree
{
public:
    /// User should always call this constructor with no parameters specified. 
    /// The parameters are only needed when branches are created internally.
    UT_InfoTree (UT_InfoTree *tree_top = NULL, UT_InfoTree *parent = NULL, 
		 const UT_StringHolder &name = TREE_TOP_DEFAULT_NAME);

    /// Recursively deletes all my data and all children, which in turn delete 
    /// all their data and children, etc...
    ~UT_InfoTree ();

    /// Clears all our row and column information. Doesn't affect child
    /// branches.
    void			 clearProperties();
    /// Clears all child branches. Doesn't affect our properties.
    void			 clearChildren();

    /// Create a child or subtree under this tree with the name 'branch_name'
    /// The caller should NOT call delete on the pointer returned when they are 
    /// done with it, since this class' destructor will take care of any
    /// children created in addChildBranch(..).
    ///
    /// IMPORTANT NOTE: It is assumed that all branches on the same level will 
    /// have *unique* names, BUT this is NOT enforced to prevent a performance 
    /// hit since these trees can get rebuilt pretty often to accurately reflect
    /// the data they are containing. BOTTOM LINE: Make sure 'branch_name' is 
    /// unique among 'myChildBranches' (otherwise there'll be trouble when we 
    /// decide to traverse a tree given a path of branches - system won't crash,
    /// but will always choose the first available branch on a level with the 
    /// name it is looking for).
    UT_InfoTree			*addChildBranch(const UT_StringHolder &name);
    /// Adds a child branch with "Property" and "Value" headings.
    UT_InfoTree			*addChildMap(const UT_StringHolder &name);

    /// Create a new empty row.
    ut_PropertyRow		*addProperties();

    /// HOW TO USE: You may add up to 4 properties to a single row at once using
    /// these methods. If you still need to add more properties to this SAME 
    /// row/array after that, simply use the returned ut_PropertyRow* to call 
    /// 'append()' on and add more properties. To add properties that are
    /// ints, use the same technique, since there is an 'append()' method in 
    /// ut_PropertyRow that takes an int parameter (or check the overridden 
    /// version of addProperties below). BOTTOM LINE - if you want maximum 
    /// flexibility, call this with no parameters and then append each property 
    /// manually to the returned ut_PropertyRow*. 
    template<class ValType1>
    ut_PropertyRow		*addProperties(const ValType1 &v1);
    template<class ValType1, class ValType2>
    ut_PropertyRow		*addProperties(const ValType1 &v1,
					const ValType2 &v2);
    template<class ValType1, class ValType2, class ValType3>
    ut_PropertyRow		*addProperties(const ValType1 &v1, 
					const ValType2 &v2,
					const ValType3 &v3);
    template<class ValType1, class ValType2, class ValType3, class ValType4>
    ut_PropertyRow		*addProperties(const ValType1 &v1, 
					const ValType2 &v2,
					const ValType3 &v3,
					const ValType4 &v4);

    /// Add column headings for this node to be used when the info is displayed
    /// in a table. THE NUMBER OF HEADINGS ADDED HERE WILL DETERMINE THE NUMBER
    /// OF COLUMNS DISPLAYED. Note: This class does NOT ensure that the # of 
    /// column headings matches the width of each property array in 
    /// 'myProperties'. Instead, we only display as many properties per row as
    /// we have number of columns.
    ///
    /// The width parameter must either be specified on all columns or on
    /// none. If set on all columns, it represents the fraction of the total
    /// width that gets allocated to that column (the values get normalized).
    /// If any column has a width specified of <= 0.0, the whole thing
    /// reverts to the default behavior of allocating all columns with
    /// equal width.
    void			 addColumnHeading(const UT_StringHolder &label,
					fpreal width = -1.0);
    void			 addColumnHeading(int label,
					fpreal width = -1.0);

    /// Get a pointer to my parent
    UT_InfoTree			*getParentBranch() const;

    /// Get a pointer to the very top node in the tree
    UT_InfoTree			*getTreeTop() const;

    /// Return this branch's name
    const UT_StringHolder	&getName() const;
    /// In case we decide to change the name of this branch
    void			 setName(const UT_StringHolder &new_name);

    /// Return this branch's "type" idwentifier
    const UT_StringHolder	&getInfoType() const;
    /// In case we decide to change the "type" of this branch
    void			 setInfoType(const UT_StringHolder &new_type);
   
    /// Get a ptr to a specific descendent branch of this tree, given a 
    /// UT_StringArray of branch names. e.g. if you want to traverse down the 
    /// tree through a child branch named "Antigen" and get a pointer to that
    /// branch's child called "Rocks", then your UT_StringArray should simply
    /// have 2 elements, "Antigen" and "Rocks", respectively in that order.
    /// Method returns NULL if no such valid path found.
    ///
    /// NOTE: Again, this assumes all branches at the same level have unique
    /// names, as stated above for 'addChildBranch()'.
    UT_InfoTree			*getDescendentPtr(UT_StringArray &path);
    
    //Accessors for my child branches and data properties
    const UT_Array<UT_InfoTree *> 	&getChildBranches() const;
    const UT_Array<ut_PropertyRow *>	&getProperties() const;
    const ut_PropertyRow 		&getColumnHeadings() const;
    const UT_FloatArray 		&getColumnWidths() const;

    static const UT_StringHolder	&getGeometryInfoType();
    static const UT_StringHolder	&getFieldInfoType();

private:
    UT_InfoTree			*myParent;
    UT_InfoTree			*myTreeTop;
    UT_StringHolder	 	 myName;
    UT_StringHolder	 	 myInfoType;
    UT_Array<UT_InfoTree *>  	 myChildBranches;
    UT_Array<ut_PropertyRow *>	 myProperties;
    ut_PropertyRow		 myColumnHeadings;
    UT_FloatArray		 myColumnWidths;
};

template<class ValType>
void
ut_PropertyRow::append(const ValType &property)
{
    UT_WorkBuffer s;
    s.format("{}", property);
    UT_StringArray::append(s);
}

template<class ValType1>
ut_PropertyRow*
UT_InfoTree::addProperties(const ValType1 &v1)
{
    //Create the array
    ut_PropertyRow	*row = addProperties();

    //Append ALL properties passed to us
    row->append(v1);

    //Return a ptr to the array so the user may add more properties if 
    //necessary
    return row;
}

template<class ValType1, class ValType2>
ut_PropertyRow*
UT_InfoTree::addProperties(const ValType1 &v1,
	const ValType2 &v2)
{
    //Create the array
    ut_PropertyRow	*row = addProperties();

    //Append ALL properties passed to us
    row->append(v1);
    row->append(v2);

    //Return a ptr to the array so the user may add more properties if 
    //necessary
    return row;
}

template<class ValType1, class ValType2, class ValType3>
ut_PropertyRow*
UT_InfoTree::addProperties(const ValType1 &v1,
	const ValType2 &v2,
	const ValType3 &v3)
{
    //Create the array
    ut_PropertyRow	*row = addProperties();

    //Append ALL properties passed to us
    row->append(v1);
    row->append(v2);
    row->append(v3);

    //Return a ptr to the array so the user may add more properties if 
    //necessary
    return row;
}

template<class ValType1, class ValType2, class ValType3, class ValType4>
ut_PropertyRow*
UT_InfoTree::addProperties(const ValType1 &v1,
	const ValType2 &v2,
	const ValType3 &v3,
	const ValType4 &v4)
{
    //Create the array
    ut_PropertyRow	*row = addProperties();

    //Append ALL properties passed to us
    row->append(v1);
    row->append(v2);
    row->append(v3);
    row->append(v4);

    //Return a ptr to the array so the user may add more properties if 
    //necessary
    return row;
}

#endif
