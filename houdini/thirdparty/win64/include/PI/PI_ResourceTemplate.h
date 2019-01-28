/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        PI_ResourceTemplate.h
 *
 * COMMENTS:
 *
 */

#ifndef __PI_ResourceTemplate_H__
#define __PI_ResourceTemplate_H__

#include "PI_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_String.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_StringArray.h>

class PRM_Template;

// Viewer types that can have associated state templates.
// NB: The order here must correspond to theViewerTypeTokens in
//     PI_ResourceManager.C
enum PI_ViewerType
{
    PI_VIEWER_SCENE = 0,
    PI_VIEWER_COMPOSITE,
    PI_VIEWER_SCENEGRAPH,

    PI_NUM_VIEWER_TYPES		// sentinal value (not a valid value)
};

// Network types to further differentiate the states for a given viewer type.
enum PI_NetworkType
{
    PI_NETWORK_OBJ = 0,
    PI_NETWORK_SOP,
    PI_NETWORK_DOP,
    PI_NETWORK_COP2,
    PI_NETWORK_CHOP,
    PI_NETWORK_LOP,

    PI_NUM_NETWORK_TYPES
};

const unsigned		 PI_NETMASK_OBJ = 1 << (unsigned)PI_NETWORK_OBJ;
const unsigned		 PI_NETMASK_SOP = 1 << (unsigned)PI_NETWORK_SOP;
const unsigned		 PI_NETMASK_DOP = 1 << (unsigned)PI_NETWORK_DOP;
const unsigned		 PI_NETMASK_COP2= 1 << (unsigned)PI_NETWORK_COP2;
const unsigned		 PI_NETMASK_CHOP= 1 << (unsigned)PI_NETWORK_CHOP;
const unsigned		 PI_NETMASK_LOP = 1 << (unsigned)PI_NETWORK_LOP;

class OP_Operator;
class PI_StateTemplate;
class PI_PITemplate;
class PI_HandleTemplate;
class PI_SelectorTemplate;

class PI_API PI_ResourceTemplate
{
public:
    PI_ResourceTemplate(const char *name, const char *english,
			void *constructor);
    virtual ~PI_ResourceTemplate();

    void		 initialize(const char *name, const char *english,
				    void *constructor);

    const UT_String		&name() const	     { return myName; }
    const UT_String		&englishName() const { return myEnglishName; }
    void			*constructor() const { return myConstructor; }
    void			 constructor(void *c){ myConstructor = c; }

private:
    UT_String			 myName;
    UT_String			 myEnglishName;
    void			*myConstructor;
};

//------------------------------------------------------------------------

class PI_API PI_StateTemplate : public PI_ResourceTemplate
{
public:
    PI_StateTemplate(const char		*name,
		     const char		*english,
		     const char		*icon_name,
		     void		*constructor,
		     PRM_Template	*templates,
		     PI_ViewerType	 viewer_type,
		     unsigned		 network_types,
		     int		 op_independent = 1,
		     const char		*volatile_hotkey = 0,
		     OP_Operator	*op = 0);
    virtual ~PI_StateTemplate();

    void		 initialize(const char *name,
				const char *english,
				const char *icon_name,
				void *constructor,
				PRM_Template *templates,
				PI_ViewerType viewer_type,
				unsigned network_types,
				int op_independent = 1,
				const char *volatile_hotkey = 0,
				bool hidden = false);

    // Since myVolatileKeys and myVolatileKeysModifiers are
    // build during object initialization, they may no longer
    // match with the hotkeys if the user decides to change the settings,
    // so we need to rebuild. In some occasions (such as the use of the
    // alt key for volatile view), we may want to enable ("inject")
    // volatile that are determined at runtime out of outside of
    // hotkey settings.
    void		rebuildVolatileKeys(
    				UT_Array<int>* injectionKeys = 0,
    				UT_Array<int>* injectionKeysModifiers = 0);

	  PRM_Template	*templates()	{ return myTemplates; }
    const PRM_Template	*templates() const
					{ return myTemplates; }
    PI_ViewerType	 viewerType() const
					{ return myViewerType; }
    unsigned		 networkTypes() const
					{ return myNetworkTypes; }
    const char		*getIconName() const
					{ return myIconName; }
    int			 opIndependent() const
					{ return myOpIndependentFlag; }
    const UT_Array<int>	 &volatileKeys()
					{ return myVolatileKeys; }
    const UT_Array<int>	 &volatileKeysModifiers()
					{return myVolatileKeysModifiers;}
    const char		*getHotkeyString() const
					{ return myHotkeyString; }
    OP_Operator		*getOperator() const
					{ return myOp; }

    void		 setHidden(bool hide)
			    { myHidden = hide; }
    bool		 getHidden() const
			    { return myHidden; }

protected:
    void		 setVolatileKeys(const UT_Array<int> &keys, 
    					 const UT_Array<int> &modifiers)
			    {
				myVolatileKeys = keys;
				myVolatileKeysModifiers = modifiers;
			    }

private:
    UT_String			 myIconName;
    UT_String			 myHotkeyString;
    PRM_Template		*myTemplates;
    OP_Operator			*myOp;
    PI_ViewerType		 myViewerType;
    unsigned			 myNetworkTypes;
    int				 myOpIndependentFlag;
    UT_Array<int>		 myVolatileKeys;
    UT_Array<int>		 myVolatileKeysModifiers;
    bool			 myHidden;
};

//------------------------------------------------------------------------

class PI_API PI_HandleTemplate : public PI_ResourceTemplate
{
public:
    PI_HandleTemplate(const char		*name,
		      const char		*english,
		      void			*constructor,
		      PRM_Template		*templates);
    virtual ~PI_HandleTemplate();

    const PRM_Template		*templates() const   { return myTemplates; }
	  PRM_Template		*templates()	     { return myTemplates; }

private:
    PRM_Template		*myTemplates;
};

//------------------------------------------------------------------------

typedef UT_SymbolMap<int> PI_ParmNameIndexMap;

class PI_API PI_PITemplate : public PI_ResourceTemplate
{
public:
    PI_PITemplate(const char *name,
		  const char *english,
		  const char *types,
		  const char *icon_name,
		  const char *parms[],
		  short	      ethereal = 0, // 0 = vis; 1 = int; 2 = float);
		  unsigned    viewmask = 0xFFFFFFFF,
		  bool	      display_always = false); 
    virtual ~PI_PITemplate();

    const PI_ParmNameIndexMap	&parmNameTable() const
				 { return myParmNameTable; }
    const UT_StringArray	&parmNameList() const
				 { return myParmNameList; }
    const char			*iconName() const   { return myIconName; }

    bool			 isDisplayAlways() const
				 { return myDisplayAlwaysFlag; }

    short			 isEthereal() const { return myEtherealFlag; }
    bool			 supportsType(const char *type) const;
    unsigned			 getViewerMask() const { return myViewerMask;}

    // Not const as we use a cache.
    const char 			*getHelp();

private:
    UT_String			 myTypes;
    UT_String			 myIconName;
    int				 myHelpCached;
    UT_String			 myHelp;
    PI_ParmNameIndexMap		 myParmNameTable;
    UT_StringArray		 myParmNameList;
    short			 myEtherealFlag;
    unsigned			 myViewerMask;
    bool			 myDisplayAlwaysFlag;
};

//------------------------------------------------------------------------

class PI_API PI_SelectorTemplate : public PI_ResourceTemplate
{
public:
    PI_SelectorTemplate(const char *name,
			const char *english,
			const char *types,
			bool ordered = false);
    virtual ~PI_SelectorTemplate();

    bool			 isGeneric() const { return (myData != 0); }
    bool			 supportsType(const char *type) const;
    void			 data(void *d) { myData = d; }
    void			*data() const { return myData; }
    bool			 ordered() const { return myOrdered; } 

private:
    UT_String			 myTypes;
    void			*myData;
    bool			 myOrdered;
};

#endif
