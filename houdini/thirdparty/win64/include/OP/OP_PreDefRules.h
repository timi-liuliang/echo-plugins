/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENT: This class encapsulates all the data required to build the
 *          "Predefined Rules" menu that is common to some OPs including
 *          Event and Group POPs. The data is read in from a default 
 *	    data file $HFS/houdini/OP<name>Rules, where 
 *	    name is the OP name (ie "Event"). If a similarly named
 *	    file is in the directory containing the user's houdini 
 *          preferences, (ie $HOME/houdini), the data in that file will
 *	    override the defaults.
 */

#ifndef __OP_PreDefRules__
#define __OP_PreDefRules__

#include "OP_API.h"
#include <UT/UT_StringArray.h>

class UT_IStream;

class OP_API OP_PreDefRules
{
public:
				 // Constructor. The file_name is the 
    				 // name of the file that will contain
    				 // the table data (ie OPEventRules).
    				 OP_PreDefRules(const char* file_name);

				 // Destructor does nothing.
    				~OP_PreDefRules();

				 // Number of rules.
    int				 getCount()       { return myTokens.entries(); }

				 // The identifier token.
    const char			*getToken(int i)  { return myTokens(i); }

    				 // This is what appears in the menu.
    const char			*getLabel(int i)  { return myLabels(i); }

    				 // This is the rule string that corresponds 
    				 // to the menu option. It ends up in the 
    				 // "Rule" parameter box. 
    const char			*getString(int i) { return myStrings(i); }

private:
				 // Parses a table file. '#' is a comment,
				 // and a rule is defined as:
				 // <token> <label> <string>
    bool			 parseFile(UT_IStream &is);

    				 // Adds a rule.
    void			 addRule(char* rule_token, 
	    				 char* rule_label,
	    				 char* rule_string);
    
    UT_StringArray		 myTokens;
    UT_StringArray		 myLabels;
    UT_StringArray		 myStrings;
};

#endif
