/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __GOP_AttribListParse_h__
#define __GOP_AttribListParse_h__

#include "GOP_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_StringHolder.h>
#include <GA/GA_Types.h>

class UT_String;
class GA_Attribute;
class GA_AttributeDict;
class GA_AttributeFilter;

enum GOP_AttribParseError
{
    GOP_AP_ERROR_NONE = 0,
    GOP_AP_ERROR_ATTRIB_UNKNOWN,
    GOP_AP_ERROR_ATTRIB_DUPLICATED,
    GOP_AP_ERROR_ATTRIB_REDUNDANT,
    GOP_AP_ERROR_PATTERN_MATCH_UNSUPPORTED,
    GOP_AP_ERROR_INCOMPATIBLE_ATTRIBUTE_LIST_STRING_PATTERNS
};

typedef void (*GOP_AttribParseErrorCallback) (GOP_AttribParseError,
                                              const char *, void *);
typedef bool (*GOP_AttribParseConfirmCallback) (const GA_Attribute *,
						void *);

//___________________________________________________________________________

class GOP_API GOP_AttribListParse
{
public:
    // Parses attribute list string to match "P" and "Pw".  Returns 0 if
    // no match, 3 if "P" matches but not "Pw", and 4 if "Pw" matches.
    // Also removes instances of "P" and "Pw" from the string.
    // ERRORS:
    //   GOP_AP_ERROR_ATTRIB_DUPLICATED
    //   GOP_AP_ERROR_ATTRIB_REDUNDANT
    static int parsePAndPw(UT_String &attriblist,
				  GOP_AttribParseErrorCallback errorCB = 0)
    {
	return parsePAndPw(attriblist, errorCB, 0);
    }

    static int parsePAndPw(UT_String &attriblist,
				  GOP_AttribParseErrorCallback errorCB,
				  void *errordata);

    // Parses attribute list string and verifies syntax as well as the
    // existence of the attributes specified.  Can return the nonexistent
    // attributes in the badlist parameter.
    // ERRORS:
    //   GOP_AP_ERROR_ATTRIB_UNKNOWN
    //   GOP_AP_ERROR_ATTRIB_DUPLICATED
    static bool isAttribListValid(const GA_AttributeDict &dict,
				  const UT_String &attriblist,
				  GOP_AttribParseErrorCallback errorCB = 0,
				  void *errordata = 0, UT_String *badlist = 0);

    // Parses the attribute list string and extracts the attribute pointers
    // into the supplied array.
    // ERRORS:
    //   GOP_AP_ERROR_PATTERN_MATCH_UNSUPPORTED (warning)
    static void parseAttribList(const GA_AttributeDict &dict,
				const UT_String &attriblist,
				UT_Array<GA_Attribute *> &attribarray,
				GOP_AttribParseConfirmCallback confirmCB = 0,
				void *confirmdata = 0,
				GOP_AttribParseErrorCallback errorCB = 0,
				void *errordata = 0,
				const GA_AttributeFilter *filter = 0);

    // Pattern matches the incoming attribute string with the input attribute
    // list string and returns the corresponding outgoing attribute string
    // from the output attribute list string.
    // ERRORS:
    //   GOP_AP_ERROR_INCOMPATIBLE_ATTRIBUTE_LIST_STRING_PATTERNS
    static char *renameAttrib(UT_String &attrib,
			      const UT_String &inattriblist, 
			      const UT_String &outattriblist,
			      GOP_AttribParseErrorCallback errorCB = 0,
			      void *errordata = 0);

    // Returns true if the attribute name is valid according to the
    // atrib list stirng.
    static bool isAttribAllowedByString(const UT_StringRef &attrib, const UT_StringRef &attriblist_ref);
};

#endif
