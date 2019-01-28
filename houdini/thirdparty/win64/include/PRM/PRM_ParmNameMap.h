/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_ParmNameMap.h ( PRM Library, C++)
 *
 * COMMENTS:	A symbol table used to perform parameter name mapping when
 *		loading old parameters.  This is really just a symbol table
 *		which knows how to properly destruct itself, and is cheaper to
 *		construct, destruct since the table is only created if there's
 *		a mapping created.
 */

#ifndef __PRM_ParmNameMap__
#define __PRM_ParmNameMap__

#include "PRM_API.h"

class UT_StringHolder;
template<typename T> class UT_SymbolMap;

class PRM_API PRM_ParmNameMap {
public:
     PRM_ParmNameMap()	{ myTable = 0; }
    ~PRM_ParmNameMap();

    void		 addMap(const char *oldname,
			        const char *newname);
    const char		*findName(const char *oldname) const;

private:
    UT_SymbolMap<UT_StringHolder> *myTable;
};

#endif

