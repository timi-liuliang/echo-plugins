/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CMD library (C++)
 *
 * COMMENTS:	Variable for the command library
 *
 */

#ifndef __CMD_Variable_h__
#define __CMD_Variable_h__

#include "CMD_API.h"
#include "CMD_SymbolList.h"
#include <UT/UT_Functor.h>
#include <iosfwd>


class CMD_API CMD_VariableTable : public CMD_SymbolList
{
public:
	     CMD_VariableTable();
    virtual ~CMD_VariableTable();

    bool		 hasVariable(const char *name)
			    { return hasValue(name); }

    bool		 getVariable(const char *name, UT_String &value)
			    { return getValue(name, value); }
    bool		 getVariable(const char *name, fpreal32 &value)
			    { return getValue(name, value); }
    bool		 getVariable(const char *name, fpreal64 &value)
			    { return getValue(name, value); }
    bool		 getVariable(const char *name, int32 &value)
			    { return getValue(name, value); }
    bool		 getVariable(const char *name, int64 &value)
			    { return getValue(name, value); }

    void		 setVariable(const char *name, const char *value,
				     bool xport = true)
			    { setValue(name, value, xport); }

    bool		 destroyVariable(const char *name)
			 {
			     return destroySymbol(name);
			 }
    int			 getVariableNames(UT_StringArray &nameList,
					  int dirty=0);
    int			 clearDirtyFlags();

    virtual bool	 destroySymbol(const char *name) override;

private:
    void		 save(CMD_Manager *, std::ostream &, const char *loc="");
    friend class	 CMD_Manager;

private:
    int			 myExportFlag;
};

#endif
