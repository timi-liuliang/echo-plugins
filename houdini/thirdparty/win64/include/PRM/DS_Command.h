/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DS library (C++)
 *
 * COMMENTS:	Dialog Scripted Command
 *	Basically, this is an interface to building a command line
 *	with arguments.  Each of the arguments is represented internally
 *	as a PRM parameter list.  The dialog is built automatically using
 *	PSI.
 */


#ifndef __DS_Command_h__
#define __DS_Command_h__

#include "PRM_API.h"
#include "PRM_ChoiceList.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_String.h>

class PRM_Template;
class PRM_ParmList;
class PRM_Default;
class PRM_Name;
class PRM_Parm;
class PRM_PresetInfo;
class PRM_SpareData;
class DS_Stream;
class DS_Page;
class DS_Parm;

class PRM_API DS_Command {
public:
      DS_Command(const char *commandlistfile);
     ~DS_Command();

    const UT_String	&getCommandName()	{ return myCmdName; }
    const UT_String	&getLabel()		{ return myLabel; }
    const UT_String	&getIcon()		{ return myIcon; }

    int			 parse(DS_Stream &is);

    void		 getCommand(UT_String &str);

    // To handle when there have been expressions overridden, we need to call
    // the parmChanged() method before getCommand() is called.  This allows
    // parameters to know whether their expressions are being overwritten.
    // After the getCommand() returns whether there was an override or not, we
    // then have to either axe the expressions (i.e. use float values) or keep
    // them.  If the axe/keep methods aren't called, then the parameter will
    // continue to think that it's being changed.
    // The parmChanged() method will return 1 if the parameter currently has
    // expressions which will be overridden by the parameter change.
    int			 parmChanged(int index);
    int			 parmChanged(const char *token);
    void		 axeExpressions();
    void		 keepExpressions();
    void		 enforceOptions();

    void		 setDefaultCommand();
    void		 setCommand(const char *str);
    int			 isValid() { return myTemplateSize; }
    const char		*getHelp();
    PRM_ParmList	*getParmList() const { return myParmList; }
    PRM_Template	*getTemplateList() const { return myTemplateList; }
    int			 disableParms();

    void		 setRManSyntax(int onOff) { myRManSyntax = onOff;}
    int			 getRManSyntax() const	  { return myRManSyntax; }
    void		 setVexSyntax(int onoff)  { myVexSyntax = onoff; }
    int			 getVexSyntax() const	  { return myVexSyntax; }

    PRM_Template	*addTemplate(DS_Parm *parm, bool addToggle = 1);

    static void		 setObjectMenuBuilder(PRM_ChoiceGenFunc callback);
    static void		 setRenderMenuBuilder(PRM_ChoiceGenFunc callback);

private:
    UT_String			 myCommandListFile;
    UT_String			 myCmdName;
    UT_String			 myLabel;
    UT_String			 myIcon;
    UT_String			 myHelp;

    DS_Page			*myPage;
    UT_ValArray<PRM_Name *>	 myNames;
    UT_ValArray<PRM_Default *>	 myDefaults;

    PRM_ParmList		*myParmList;
    PRM_Template		*myTemplateList;
    PRM_PresetInfo		*myPresetInfo;
    int				 myTemplateSize;
    int				 myRManSyntax;
    int				 myVexSyntax;

    int				 mySwitcherCount;
};

#endif
