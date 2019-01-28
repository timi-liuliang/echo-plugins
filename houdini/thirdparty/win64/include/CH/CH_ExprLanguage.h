/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	This file contains enums and helper functions related to expression
 *	languages and meanings of strings in parameters.
 */

#ifndef __CH_ExprLanguage_h__
#define __CH_ExprLanguage_h__

#include "CH_API.h"

// These enum values are the languages we support in expressions.
enum CH_ExprLanguage
{
    CH_OLD_EXPR_LANGUAGE,
    CH_PYTHON
};


// These enum values are the languages we support in chunks of script.
enum CH_ScriptLanguage
{
    CH_HSCRIPT,
    CH_PYTHON_SCRIPT,

    CH_NUM_SCRIPT_LANGUAGES
};


// These enum values are used when setting string values in parms.
enum CH_StringMeaning
{
    // The following interpretation applies for CH_STRING_LITERAL:
    // Parameter Type  String Contents                       Intepretation
    // --------------  ---------------                       -------------
    // string          anything                              literal
    // ordinal         anything looking like a menu token    literal
    // ordinal         integer                               literal
    // ordinal         anything else                         hscript expression
    // float           float or integer                      literal
    // float           anything else                         hscript expression
    CH_STRING_LITERAL = 0,

    // An hscript expression is always made into an expression and set to use
    // that language.
    CH_OLD_EXPRESSION = 1,

    // A python expression is always made into an expression and set
    // to use that language.
    CH_PYTHON_EXPRESSION = 2,

    // This option will always set it to an expression, but it will use
    // the default language (either old-style expressions or python).
    CH_EXPRESSION_IN_DEFAULT_LANGUAGE = 4
};


// These functions return a string literal corresponding to the language.
CH_API const char *CHexprLanguageAsString(CH_ExprLanguage language);
CH_API const char *CHscriptLanguageAsString(CH_ScriptLanguage language);

// These functions return the language enum value corresponding to the string.
// True is returned if the conversion succeeded.
CH_API bool CHstringToExprLanguage(const char *str, CH_ExprLanguage &language);
CH_API bool CHstringToScriptLanguage(
	const char *str, CH_ScriptLanguage &language);

// This function returns a string literal of the label for a language.
CH_API const char *CHscriptLanguageAsLabel(CH_ScriptLanguage language);


// This function returns the unique character corresponding to the language.
CH_API char CHexprLanguageAsChar(CH_ExprLanguage language);

// This function returns the language corresponding to the unique character
// by setting "language".  True is returned if the conversion succeeded.
CH_API bool CHcharToExprLanguage(char char_value, CH_ExprLanguage &language);


// This function converts from an expression language to a meaning that says
// the string is definitely an expression in that langauge.
CH_API CH_StringMeaning CHexprLanguageToStringMeaning(CH_ExprLanguage language);

// This function converts from a string meaning that says it's definitely
// an expressin in a some language to that language.
CH_API CH_ExprLanguage CHstringMeaningToExprLanguage(CH_StringMeaning meaning);


// This function returns a string literal corresponding to the meaning.
CH_API const char *CHmeaningToString(CH_StringMeaning meaning);

// This function returns the meaning corresponding to the string.  True is
// returned if the conversion succeeded.
CH_API bool CHstringToMeaning(const char *meaning_str,
			      CH_StringMeaning &meaning);

#endif
