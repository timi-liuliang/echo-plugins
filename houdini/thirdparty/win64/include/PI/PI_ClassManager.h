/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Base Direct manipulation library (C++)
 *
 * COMMENTS:
 *	Manages the registration and linking of PI classes to PI names.
 *
 */
#ifndef __PI_ClassManager_H__
#define __PI_ClassManager_H__

#include "PI_API.h"
#include <UT/UT_ValArray.h>

class PI_API PI_ClassManager
{
public:
			 PI_ClassManager() {}
			~PI_ClassManager() {}

    void		 addPI(const char *name, void *cs);
    void		 addSelector(const char *name, void *data, void *cs);

    int			 getNumPIs() const
			 { return myPINames.entries(); }
    const char		*getPIName(int i) const
			 { return myPINames(i); }
    void		*getPIConstructor(int i) const
			 { return myPIConstructors(i); }

    int			 getNumSelectors() const
			 { return mySelectorNames.entries(); }
    const char		*getSelectorName(int i) const
			 { return mySelectorNames(i); }
    void		*getSelectorData(int i) const
			 { return mySelectorData(i); }
    void		*getSelectorConstructor(int i) const
			 { return mySelectorConstructors(i); }

protected:
    UT_ValArray<const char *>	myPINames;
    UT_ValArray<void *>		myPIConstructors;
    UT_ValArray<const char *>	mySelectorNames;
    UT_ValArray<void *>		mySelectorConstructors;
    UT_ValArray<void *>		mySelectorData;
};

PI_API PI_ClassManager *PIgetClassManager();

#define PI_SETUP()				\
class  init { public: init(); int dummy; };	\
static init initializer;

#define PI_REGISTER(name, classname)		\
classname::init classname::initializer;		\
classname::init::init()				\
{ PIgetClassManager()->addPI(name, (void *)classname::ourConstructor); }

class PI_API PI_SelectorTable
{
    public:
	PI_SelectorTable(const char *name, void *data, void *constructor)
	    : myName(name), myData(data), myConstructor(constructor) { }
	const char	*myName;
	void		*myData;
	void		*myConstructor;
};

#define SELECTOR_SETUP()			\
class  init { public: init(); int dummy; };	\
static init initializer;

#define SELECTOR_REGISTER(name, data, classname)\
classname::init classname::initializer;		\
classname::init::init()				\
{ PIgetClassManager()->addSelector(name, data,	\
		       (void *)classname::ourConstructor); }

#define SELECTOR_SETUP_TABLE()						\
class  init { public: init(); int dummy; };				\
static init initializer;

#define SELECTOR_REGISTER_TABLE_BEGIN(classname)			\
classname::init classname::initializer;					\
classname::init::init()							\
{   PI_SelectorTable	 table[] = {
    
#define SELECTOR_REGISTER_TABLE_ENTRY(name, data, constructor)		\
    PI_SelectorTable(name, (void *)data, (void *)constructor),

#define SELECTOR_REGISTER_TABLE_END()					\
    PI_SelectorTable(0, 0, 0) };					\
    for( int i = 0; table[i].myName; i++ )				\
	PIgetClassManager()->addSelector(table[i].myName,		\
			     table[i].myData,				\
			     table[i].myConstructor); }

// This enum must be kept in sync with the geometry_type.val radio strip 
// in $FUSE/ViewExtra.ui!
enum PI_GeometryType
{
    PI_GEOTYPE_INVALID = -1,
    PI_GEOTYPE_POINTS = 0,
    PI_GEOTYPE_EDGES,
    PI_GEOTYPE_PRIMITIVES,
    PI_GEOTYPE_VERTICES,
    PI_GEOTYPE_BREAKPOINTS,
    PI_NUM_GEOTYPES	// sentinel
};

typedef UT_Array<PI_GeometryType> PI_GeometryTypeArray;

class PI_API PI_SelectionEntry
{
public:
    PI_SelectionEntry()
		    : myGeometryType(PI_GEOTYPE_INVALID),
		      myMenuEntryIndex(0),
		      myListTerminatorFlag(1) {}
    PI_SelectionEntry(PI_GeometryType geo_type,
		      int menu_entry_index = -1)
		    : myGeometryType(geo_type),
		      myMenuEntryIndex(menu_entry_index),
		      myListTerminatorFlag(0) {}

    PI_GeometryType	 myGeometryType;	// what type of selection?
    int			 myMenuEntryIndex;	// value to set menu to
    int			 myListTerminatorFlag;	// used to mark end of list?
};

#endif
