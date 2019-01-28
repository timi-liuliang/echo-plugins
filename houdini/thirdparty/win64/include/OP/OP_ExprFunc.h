/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP library (C++)
 *
 * COMMENTS:	Expression function manager
 *		We handle expression functions at this level to support
 *		saving and loading of the functions...
 *
 */

#ifndef __OP_ExprFunc_h__
#define __OP_ExprFunc_h__

#include "OP_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_SymbolTable.h>
#include <iosfwd>

class UT_IStream;
class EX_ExprFunc;
class EV_FUNCTION;
class CMD_Manager;

class OP_API OP_ExprFunc {
public:
     OP_ExprFunc(CMD_Manager *cmd);
    ~OP_ExprFunc();

    int		 load(const char *filename, 
       		      std::ostream *err = 0, std::ostream *out = 0);
    int		 load(const char *filename, 
       		      std::ostream *err, std::ostream *out, bool notify_new);
    int		 load(UT_IStream &is, 
       		      std::ostream *err = 0, std::ostream *out = 0);
    int		 load(UT_IStream &is, 
       		      std::ostream *err, std::ostream *out, bool notify_new);
    int		 save(std::ostream &os) const;

    int		 entries() const	{ return myFunctions.entries(); }
    EX_ExprFunc	*getFunction(int idx)	{ return myFunctions(idx); }

public:
    // Methods used by commands
    void	ls(std::ostream &os);
    void	rm(const char *funcname, std::ostream *out = 0);
    void        removeAllFunctions();
    void	edit(const char *funcname, std::ostream *err, int reedit=0);
    int		cat(std::ostream &os, const char *funcname);

private:
    int		find(const char *name);
    void	sort();
    void	install(CMD_Manager *cman);
    void	printDeclaration(std::ostream &os, EX_ExprFunc *func);

    UT_ValArray<EX_ExprFunc *>	myFunctions;
    UT_SymbolMap<EV_FUNCTION *>	myOldFunctions;
};

#endif
