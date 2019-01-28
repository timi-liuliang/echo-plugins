/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SharedString.h ( UT Library, C++)
 *
 * COMMENTS:	Container class for a shared string database.  This can be used
 *		when you need to allocate many strings where there are
 *		potentially a lot of duplicate string values.
 */

#ifndef __UT_SharedString__
#define __UT_SharedString__

#include "UT_API.h"
#include <iosfwd>
#include "UT_SymbolTable.h"
#include "UT_IndexedHashMapT.h"

class UT_API UT_SharedString {
public:
     UT_SharedString();
    
    const char	*get(const char *str)
		 {
		     if (!str || *str == '\0')
			 return theEmptySharedString;

		     UT_IndexedHashMapStringItem *ptr;
		     ptr = myMap.add(str);
		     return ptr->getKey().getString();
		 }

    void	 free(const char *str)
		 {
		     if (str && str != theEmptySharedString)
			 myMap.remove(str);
		 }
    void	 outputStats();
    void	 dumpTable(std::ostream &os);

    // Old-school
    const char	*allocString(const char *str) { return get(str); }
    void	 freeString(const char *str) { return free(str); }

private:
    static const char		*theEmptySharedString;
    UT_IndexedStringMap		 myMap;
};

#endif
