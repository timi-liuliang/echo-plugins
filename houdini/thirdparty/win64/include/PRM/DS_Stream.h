/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DS library (C++)
 *
 * COMMENTS:	Handle's stream IO for a Dialog Script
 */

#ifndef __DS_Stream_h__
#define __DS_Stream_h__

#include "PRM_API.h"
#include <FS/CPP_TokenStream.h>
#include <UT/UT_ErrorManager.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_String.h>

#define OBRACE	"{"
#define CBRACE	"}"

class CPP_Manager;

class PRM_API DS_Stream : public CPP_TokenStream {
public:
     DS_Stream(const char *filename, int pathvar=-1);
     DS_Stream(UT_IStream &is, const char *filename="<internal>",
	       bool fordisablewhen = false);
    ~DS_Stream();

    //
    // Paths are specified in standard Houdini form (i.e. "@/vop/include")
    //
    int		getOpenBrace();
    int		parseBlock(UT_String &block, char delimiter = '\n',
			   bool quote_tokens = false);
    int		getToken(UT_String &token);

//
//  Error handling stuff
//
    int		getErrorMessages(UT_String &str,
				 UT_ErrorSeverity s = UT_ERROR_NONE)
		{ return myErrorManager.getErrorMessages(str, s); }
    void	clearErrors()
		{ UTgetErrorManager()->clearAndDestroyErrors();
		  myErrorManager.clearAndDestroyErrors(); }
    void	stealErrors(DS_Stream &src)
		{ myErrorManager.stealErrors(src.myErrorManager); }

    void	message(const char *m); //	{ UTaddWarning("DS", 0, m); }
    void	warning(const char *m); //	{ UTaddWarning("DS", 0, m); }
    void	error(const char *m); //	{ UTaddWarning("DS", 0, m); }

private:
    UT_ErrorManager     	 myErrorManager;
};

#endif
