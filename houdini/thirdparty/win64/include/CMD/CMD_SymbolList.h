/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CMD_SymbolList.h ( CMD Library, C++)
 *
 * COMMENTS:	Used by aliases and variables.  The symbol list contains a
 *		symbol and a value
 */

#ifndef __CMD_SymbolList__
#define __CMD_SymbolList__

#include "CMD_API.h"
#include <UT/UT_Lock.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_String.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_SymbolTable.h>
#include <SYS/SYS_Compiler.h>
#include <utility>

class CMD_Alias;
class CMD_Manager;

class CMD_API CMD_SymbolEntry : UT_NonCopyable
{
public:
    CMD_SymbolEntry(const char *name, const char *value, bool export_var=false)
	: myName(name)
	, myValue(value)
	, myDirtyFlag(true)
	, myExportFlag(export_var)
    {
	if (myExportFlag)
	    exportSymbol();
    }
    ~CMD_SymbolEntry()
    {
	if (myExportFlag)
	    unexportSymbol();
    }

    // CMD_SymbolEntry is movable but not copyable
    CMD_SymbolEntry(CMD_SymbolEntry &&movable) SYS_NOEXCEPT
    {
	operator=(std::move(movable));
    }
    CMD_SymbolEntry &
    operator=(CMD_SymbolEntry &&movable)
    {
	myName = movable.myName;
	myValue = movable.myValue;
	myDirtyFlag = movable.myDirtyFlag;
	myExportFlag = movable.myExportFlag;
	movable.myExportFlag = false;
	return *this;
    }

    const UT_StringHolder   &name() const	{ return myName; }
    const UT_StringHolder   &value() const	{ return myValue; }
    bool		     isDirty() const	{ return myDirtyFlag; }
    bool		     isExport() const	{ return myExportFlag; }

    void setValue(const UT_StringHolder &value)
    {
	myValue = value;
	if (myExportFlag)
	    exportSymbol();
	myDirtyFlag = true;
    }

    void setDirty(bool f)
    {
	myDirtyFlag = f;
    }

private:
    void exportSymbol();
    void unexportSymbol();

    UT_StringHolder	myName;
    UT_StringHolder	myValue;
    bool		myDirtyFlag;
    bool		myExportFlag;
};

class CMD_API CMD_SymbolList
{
public:
    virtual ~CMD_SymbolList();

    bool		 hasValue(const char *name) const;

    bool		 getValue(const char *name, UT_String &value);
    bool		 getValue(const char *name, fpreal32 &value);
    bool		 getValue(const char *name, fpreal64 &value);
    bool		 getValue(const char *name, int64 &value);
    bool		 getValue(const char *name, int32 &value);

    bool		 setValue(const char *name, const char *value,
				  bool export_var);

    // Sets the value only if it's different. Returns true if set.
    bool		 setValueIfDifferent(
				  const char *name, const char *value,
				  bool export_var);

    virtual bool	 destroySymbol(const char *name);

    exint		 getSymbolNames(UT_StringArray &nameList);

    bool		 setDirty(const char *name, bool dirty);

protected:
    virtual bool	 isValidSymbolName(const char *name) const;

    UT_Lock &		 getLock() const
			    { return myLock; }

    typedef UT_StringMap<CMD_SymbolEntry> TableMap;

    const TableMap	&table() const	{ return myTable; }
    TableMap		&table()	{ return myTable; }
    
private:
    const char		    *getRawValue(const UT_StringRef &name) const;
    CMD_SymbolEntry	    *getSymbolPtr(const UT_StringRef &name);

private:
    TableMap		 myTable;
    mutable UT_Lock	 myLock;
};

#endif

