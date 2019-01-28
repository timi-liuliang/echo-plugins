/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DS_Parm.h ( PRM Library, C++)
 *
 * COMMENTS:	Scripted version of a parameter.  This class is typically
 *		created by reading a script.  It contains all the relevant
 *		information to create a PRM_Template.
 */

#ifndef __PRM_ScriptParm__
#define __PRM_ScriptParm__

#include "PRM_API.h"
#include "PRM_Template.h"
#include "PRM_Default.h"
#include "PRM_Conditional.h"
#include <UT/UT_Array.h>
#include <UT/UT_StringArray.h>

class DS_Stream;
class PRM_SpareData;
class PRM_ScriptPage;
class PRM_ScriptParm;
class PRM_Default;
class PRM_Range;
class PRM_ChoiceList;
class PRM_ScriptImports;

class PRM_API PRM_ScriptGroup {
public:
				 PRM_ScriptGroup(PRM_ScriptParm *owner,
						 bool asmultiparm = false);
    virtual 			~PRM_ScriptGroup();

    void		 	 addPage(PRM_ScriptPage *page)
				 { myPages.append(page); }

    int			 	 getPageCount() const
				 { return myPages.entries(); }
    PRM_ScriptPage		*getPage(int idx)
				 { return myPages(idx); }

    PRM_Default			*getDefaults()
				 {
				    if (!mySizes) buildSizes();
				    return mySizes;
				 }

    bool		 	 isMultiParm() const
				 { return myMultiParmFlag; }

    PRM_MultiType	 	 getMultiType() const
				 { return myMultiType; }
    void		 	 setMultiType(PRM_MultiType multitype)
				 { myMultiType = multitype; }

    int			 	 computeTemplateSize();
    
    void		 	 fillTemplate(PRM_Template *array, int &idx,
					      PRM_ScriptImports *&imports,
					      int nest_level);

    PRM_ConditionalSwitcher	*getGroupConditional()
				 {
				    if (!myGroupConditional)
					buildConditionals();
				    return myGroupConditional;
				 }
				
protected:
    // This will build the defaults by counting the number of parameters in
    // each page and allocating a correct default.
    void		 buildSizes();
    void		 buildConditionals();

    UT_ValArray<PRM_ScriptPage *>	 myPages;
    PRM_ScriptParm			*myOwner;
    PRM_Default				*mySizes;
    PRM_ConditionalSwitcher		*myGroupConditional;
    PRM_MultiType			 myMultiType;
    bool				 myMultiParmFlag;
};

class PRM_API PRM_ScriptParm {
public:
	     PRM_ScriptParm();
    virtual ~PRM_ScriptParm();

    enum {
	PRM_SPARM_REQUIRED	= 0x001,// Parm is required (not an option)
	PRM_SPARM_DIRECTION	= 0x002,// Represents a direction jack
	PRM_SPARM_COLOR		= 0x004,// Represents a color
	PRM_SPARM_OBJECT	= 0x008,// Object parameter (needs menu)
	PRM_SPARM_RENDERER	= 0x010,// Renderer parameter (needs menu)
	PRM_SPARM_ANIMSTRING	= 0x020,// Int/Flt parm has a string default
	PRM_SPARM_OBJECTLIST	= 0x040,// Object list
	PRM_SPARM_EMBED		= 0x080,// Embedded parmlist
	PRM_SPARM_LOGARITHMIC	= 0x100,// Logarithmic slider
	PRM_SPARM_OBSOLETE	= 0x200,// Obsolete parameter
	PRM_SPARM_ALLFLAGS	= 0x3FF	// All flags
    };

    int			 parse(DS_Stream &is, int multiparm_depth);
    int			 parseGroup(DS_Stream &is,
				    PRM_ScriptPage *page,
				    const char *grouptype,
				    int multiparm_depth);
    bool		 parseMultiParm(DS_Stream &is,
					PRM_MultiType multitype,
					PRM_ScriptPage *page,
					int multiparm_depth);
    int			 parseGroupAttribute(DS_Stream &is);
    int			 parseRange(DS_Stream &is);

    void		 appendScriptParms(
					UT_ValArray<PRM_ScriptParm *> &parms);

    void		 setFlag(unsigned flag)		{ myFlag |= flag; }
    void		 clearFlag(unsigned flag)	{ myFlag &= ~flag; }
    unsigned		 getFlag(unsigned bit) const	{ return myFlag & bit;}

    PRM_ScriptGroup	*getGroup() const	{ return myGroup; }
    PRM_Name		&getName()		{ return myName; }
    const PRM_Name	&getName() const	{ return myName; }
    const PRM_Type	&getType() const	{ return myType; }
    PRM_TypeExtended	 getTypeExtended() const{ return myTypeExtended; }
    PRM_MultiType	 getMultiType() const	{ return myMultiType; }
    const PRM_Default	*getDefaults() const
	{ return myDefaults.entries()	? &myDefaults(0)
					: (const PRM_Default*)NULL; }
    PRM_Default		*getDefaults()
	{ return myDefaults.entries()	? &myDefaults(0)
					: (PRM_Default*)NULL; }
    PRM_Range		*getRange() const	{ return myRange; }
    int			 getSize() const	{ return myVectorSize; }
    PRM_ChoiceList	*getMenu() const	{ return myMenu; }
    const UT_String	&getOption() const	{ return myOption; }
    const UT_String	&getHelpText() const	{ return myHelpText; }
    PRM_ConditionalGroup&getConditionalGroup()	{ return myConditionalGroup; }
    PRM_Template	*getMultiTemplate();
    bool		 getBaseParm() const	{ return myBaseParm; }

    PRM_SpareData	*getSpareData(bool create=false);
    
    bool		 hasObsoleteGroup() const { return myObsoleteGroup!=0; }
    PRM_ScriptParm	*createObsoleteGroupParm();

    /// Clear any type or extended type settings related to joining.  Returns
    /// true if any settings were changed.
    bool		 clearJoinNext();

    /// Some parameter dialogs require options for every parameter.
    void		 enforceOptions();

    /// Inserts double backslashes to protect verbatim strings in script files.
    static const char	*protectString(const char *s, int handlenull = 0);
    static void		 protectStrcat(UT_WorkBuffer &dest, const char *s,
					int handlenull = 0);

    int			 computeTemplateSize();
    void		 fillTemplate(PRM_Template *array, int &idx,
					PRM_ScriptImports *&imports,
					int nest_level);
    
    static bool			 parseConditional(DS_Stream &is,
						  PRM_Conditional &cond);
protected:
    virtual PRM_ScriptGroup		*newGroup(bool asmultiparm = false);
    virtual PRM_ScriptPage		*newPage();

    virtual int			 handleUnknown(DS_Stream &is, UT_String &token);
    virtual PRM_Callback	 getScriptParmCallback() const;
    virtual bool		 wantsParametersAnimatable() const;
    int				 buildMultiParmTemplate(
					PRM_ScriptImports *&imports,
					int nest_level);

    int				 parseMenu(DS_Stream &is);
    int				 parseAttribute(DS_Stream &is);
    int				 parseDefaults(DS_Stream &is, int isstring);
    void			 clearMenu();
    void			 clearDefaults();
    
    static bool			 parseConditionalInfo(DS_Stream &is,
						      PRM_Conditional &cond);
    
    PRM_ScriptGroup		 *myGroup;
    PRM_ScriptGroup		 *myObsoleteGroup;
    PRM_ChoiceList		 *myMenu;
    PRM_Name			 *myMenuNames;
    PRM_Template		 *myMultiTemplate;
    int				  myMenuType;
    PRM_Name			  myName;
    PRM_Type			  myType;
    PRM_TypeExtended		  myTypeExtended;
    PRM_MultiType		  myMultiType;
    int				  myVectorSize;
    UT_Array<PRM_Default>	  myDefaults;
    UT_StringArray		  myDefaultStrings;
    PRM_Range			 *myRange;
    PRM_SpareData		 *mySpareData;
    PRM_ConditionalGroup	  myConditionalGroup;
    unsigned			  myFlag;
    UT_String			  myHelpText;
    int				  myToolboxLevel;
    bool			  myBaseParm;

    // This option string is used for command building (DS_Command).
    UT_String			  myOption;

    static const char 		 *theTokens[];
    static int			  theNumParms[];
};

#endif

