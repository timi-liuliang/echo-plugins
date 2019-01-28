/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TAKE_StringSaver.h ( TAK Library, C++)
 *
 * COMMENTS:	Since many strings in takes are shared, during the saving
 *		process we assign each unique string an id (basically, an array
 *		index).  These strings are shared between all layers in the
 *		take.
 *		To keep track of these strings as we save out the data, we use
 *		this class to manage the string tables.
 */

#ifndef __TAKE_StringSaver__
#define __TAKE_StringSaver__

#include "TAKE_API.h"
#include <UT/UT_SymbolTable.h>

class TAKE_Packet;

class TAKE_API TAKE_StringSaver : public UT_SymbolMap<int> {
public:
	     TAKE_StringSaver();
    virtual ~TAKE_StringSaver();

    int		getStringId(TAKE_Packet &os, const char *string);

private:
    int		myCount;
};

#endif

