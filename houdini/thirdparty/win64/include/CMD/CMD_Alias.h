/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CMD library (C++)
 *
 * COMMENTS:	Alias handling for CMD library
 *
 */

#ifndef __CMD_Alias_H__
#define __CMD_Alias_H__

#include "CMD_API.h"
#include "CMD_SymbolList.h"
#include <iosfwd>

class CMD_Alias;

class CMD_API CMD_AliasTable : public CMD_SymbolList
{
public:
	     CMD_AliasTable();
    virtual ~CMD_AliasTable();

    bool		 hasAlias(const char *name)
			 {
			     return hasValue(name);
			 }
    bool		 getAlias(const char *name, UT_String &value)
			 {
			     return getValue(name, value);
			 }
    void		 setAlias(const char *name, const char *value)
			 {
			     setValue(name, value, /*export_var=*/false);
			 }
    bool		 destroyAlias(const char *name)
			 {
			     return destroySymbol(name);
			 }
    exint		 getAliasNames(UT_StringArray &nameList)
			 {
			     return getSymbolNames(nameList);
			 }

private:
    bool		 isValidSymbolName(const char *name) const override;

    void		 save(CMD_Manager *cman, std::ostream &os);
    friend class	 CMD_Manager;	// For save
};

#endif
