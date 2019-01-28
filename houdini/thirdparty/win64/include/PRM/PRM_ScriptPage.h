/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_ScriptPage.h ( PRM Library, C++)
 *
 * COMMENTS:	A page of scripted parameters.  These pages are used to define
 *		the folder tabs for each page in a std. switcher.
 */

#ifndef __PRM_ScriptPage__
#define __PRM_ScriptPage__

#include "PRM_API.h"
#include <UT/UT_String.h>
#include <UT/UT_ValArray.h>

#define ISERROR(msg)	{ is.error(msg); return 0; }
#define ISWARNING(msg)	{ is.warning(msg); }

class DS_Stream;
class PRM_ScriptParm;
class PRM_Template;
class PRM_ScriptImports;
class PRM_SpareData;
class PRM_ConditionalGroup;

class PRM_API PRM_ScriptPage {
public:
	     PRM_ScriptPage();
    virtual ~PRM_ScriptPage();

    enum {
	PRM_SCRIPT_RMAN		= 0x01,	// RenderMan style command script
	PRM_SCRIPT_SHELL	= 0x02,	// Shell style command script
	PRM_SCRIPT_VEX		= 0x04,	// Vex style command script
	PRM_SCRIPT_INVISIBLE	= 0x08,	// This group of parms is invisible
	PRM_SCRIPT_INVISIBLETAB = 0x10,	// This page of parms is invisible
	PRM_SCRIPT_OBSOLETE	= 0x20	// This group/page of parms is obsolete
    };

    /// normally, we are parsing the whole stream & thus expect the opening
    /// brace.  However, a derived Reader might want to read some meta data,
    /// then hand off to us, in which case expect_open_brace will be false.
    int			 parse(DS_Stream &is,
			       bool expect_open_brace,
			       PRM_ScriptParm *group_owner_parm,
			       bool parsing_import, 
			       bool fix_invalid_joins=true,
			       int multiparm_depth=0);

    void		 appendScriptParms(
					UT_ValArray<PRM_ScriptParm *> &parms);

    const char		*getToken() const	{ return myToken; }
    void		 setToken(const char *token) { myToken = token; }
    const char		*getLabel() const	{ return myLabel; }
    const char		*getDefault() const	{ return myDefault; }
    PRM_ScriptParm	*getParm(int i)		{ return myParms(i); }
    int			 getParmEntries() const	{ return myParms.entries(); }
    bool		 getBaseParm() const	{ return myBaseParm; }
    bool		 getTabBreak() const	{ return myTabBreak; }
    PRM_ConditionalGroup*getGroupConditional() const
			 { return myConditionalGroup; }
    PRM_ConditionalGroup*getPageConditional() const
			 { return myConditionalPage; }
    PRM_SpareData	*getSpareData(bool create=false);
    void		 enforceOptions();

    bool		 containsReservedName() const;

    // Set the default if there is no default defined...
    void		 setEmptyDefault(const char *str)
			 {
			     if (!myDefault.isstring())
				 myDefault.harden(str);
			 }

    void		 setFlag(unsigned flag)		{ myFlag |= flag; }
    void		 clearFlag(unsigned flag)	{ myFlag &= ~flag; }
    unsigned		 getFlag(unsigned bit) const	{ return myFlag & bit;}

    // By default, this will simply return the number of parameters in my page
    virtual int		 countParameters() const;

    // Gets the actual number of templates required by this page, including
    // all pages within pages and so on.
    int			 computeTemplateSize();
    // Fills an array of PRM_Templates from all our parms. The size of the
    // array must be at least computeTemplateSize() + 1. By passing in a
    // non-zero idx starting value the templates from this page can be
    // appended after some existing templates.
    void		 fillTemplate(PRM_Template *tplate, int &idx,
				      PRM_ScriptImports *&imports);

    // Obsolete template handling.
    int			 computeObsoleteTemplateSize();
    void		 fillObsoleteTemplate(PRM_Template *tplate);

    struct TokenParmCount
    {
	const char	*token;
	int		 parm_count;
    };
    
    /// a list of tokens not handled by PRM_ScriptPage, but which may be
    /// handled by my subclasses or should at least be ignored without
    /// generating an error. Each token contains the expected parameter count.
    /// Negative values indicate tokens that take brace-enclosed blocks.
    static const TokenParmCount	 theTokens[];
    
protected:
    friend class PRM_ScriptGroup;
    
    virtual PRM_ScriptParm	*newParm();
    virtual PRM_ScriptPage	*newImportPage();
    virtual int			 parseHelp(DS_Stream &is, UT_String &help);
    virtual int			 handleUnknown(DS_Stream &is, UT_String &token);
    int				 parseImport(DS_Stream &is,
					     int multiparm_depth);
    void			 processImportPage(PRM_ScriptPage *page);
    int				 parseAttribute(DS_Stream &is);

    void		 fillTemplate(PRM_Template *tplate, int &idx,
				      PRM_ScriptImports *&imports,
				      int nest_level, int switcher_idx,
				      int folder);

    
    UT_String				 myToken;
    UT_String				 myLabel;
    UT_String				 myDefault;
    UT_String				 myImportSource;
    UT_String				 myImportToken;
    UT_String				 myImportMask;
    bool				 myImportEnable;
    UT_ValArray<PRM_ScriptParm *>	 myParms;
    UT_ValArray<PRM_ScriptParm *>	 myObsoleteParms;
    PRM_ScriptImports			*myImports;
    PRM_ConditionalGroup		*myConditionalPage;
    PRM_ConditionalGroup		*myConditionalGroup;
    bool				 myBaseParm;
    bool				 myTabBreak;
    PRM_SpareData			*mySpareData;
    unsigned				 myFlag;

};

#endif
