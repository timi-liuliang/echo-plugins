/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DS_ParmReader.h ( PRM Library, C++ )
 *
 * COMMENTS:	Generic parameter reader for the generation of dialog scripts
 *		for script shaders
 */

#ifndef __DS_ParmReader__
#define __DS_ParmReader__

#include "PRM_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_String.h>
#include "PRM_Type.h"
#include <CH/CH_ExprLanguage.h>

class UT_StringArray;

class PRM_Range;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class PRM_API DS_DefaultValue
{
public:

			 DS_DefaultValue( const fpreal32 *fp, int size );
			 DS_DefaultValue( const fpreal64 *fp, int size );
			 DS_DefaultValue( const int32 *ip, int size );
			 DS_DefaultValue( const int64 *ip, int size );
			 DS_DefaultValue( const char *sp[],
					  const CH_StringMeaning *meanings,
					  int size );
			 DS_DefaultValue(const UT_Fpreal32Array &values);
			 DS_DefaultValue(const UT_Fpreal64Array &values);
			 DS_DefaultValue(const UT_Int32Array &values);
			 DS_DefaultValue(const UT_Int64Array &values);
			 DS_DefaultValue(const UT_StringArray &values,
					 const CH_StringMeaning *meaning);

			~DS_DefaultValue();

    int			 copy( const fpreal32 *fp, int size );
    int			 copy( const fpreal64 *fp, int size );
    int			 copy( const int32 *ip, int size );
    int			 copy( const int64 *ip, int size );
    int			 copy( const char *sp[],
				const CH_StringMeaning *meanings,
			        int size);
    int			 copy( const UT_Int32Array &v);
    int			 copy( const UT_Int64Array &v);
    int			 copy( const UT_Fpreal32Array &v);
    int			 copy( const UT_Fpreal64Array &v);
    int			 copy( const UT_StringArray &v,
				const CH_StringMeaning *meanings);

    fpreal		*getFloat()	const	  { return myFloat; }
    int			*getInteger()	const	  { return myInteger; }
    const char		*getString(int idx) const { return myStrings[idx]; }
    CH_StringMeaning	 getStringMeaning(int idx) const
					    { return myStringMeanings[idx]; }

    int			 getSize()	const	{ return mySize; };

private:
    void		 initZero();
    void		 cleanup();

    fpreal	 *myFloat;
    int		 *myInteger;
    char	**myStrings;
    CH_StringMeaning *myStringMeanings;

    int		  mySize;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class PRM_API DS_MenuChoice {
public:
     DS_MenuChoice( const char *value, const char *label)
	: myValue( 0 ),
	  myLabel( 0 )
	{
	    if ( value ) myValue = ::strdup( value );
	    if ( label ) myLabel = ::strdup( label );
	};
    ~DS_MenuChoice()
	{
	    if ( myValue ) ::free( myValue );
	    if ( myLabel ) ::free( myLabel );
	};

    const char			*getValue()	const	{ return myValue; }
    const char			*getLabel()	const	{ return myLabel; }

private:
    char	*myValue;
    char	*myLabel;
};

class PRM_API DS_AttributeValue {
public:
    DS_AttributeValue(const char *token, const char *value)
	: myToken(0), myValue(0)
	{
	    if (token) myToken = ::strdup(token);
	    if (value) myValue = ::strdup(value);
	}
    ~DS_AttributeValue()
	{
	    if (myToken) ::free(myToken);
	    if (myValue) ::free(myValue);
	}

    const char			*getToken()	const	{ return myToken; }
    const char			*getValue()	const	{ return myValue; }

private:
    char	*myToken;
    char	*myValue;
};

class PRM_API DS_HandleBinding {
public:
    DS_HandleBinding(const char *channelname, const char *type,
		     const char *desc, const char *label,
		     const char *default_settings)
	: myChannelName(0), myType(0), myDesc(0), myLabel(0)
	{
	    if (channelname) myChannelName = ::strdup(channelname);
	    if (type) myType = ::strdup(type);
	    if (desc) myDesc = ::strdup(desc);
	    if (label) myLabel = ::strdup(label);
	    if (default_settings)
		myDefaultSettings = ::strdup(default_settings);
	}
    ~DS_HandleBinding()
	{
	    if (myChannelName) ::free(myChannelName);
	    if (myType) ::free(myType);
	    if (myDesc) ::free(myDesc);
	    if (myLabel) ::free(myLabel);
	    if (myDefaultSettings) ::free(myDefaultSettings);
	}

    const char	*getChannelName() const		{ return myChannelName; }
    const char	*getType() const		{ return myType; }
    const char	*getDesc() const		{ return myDesc; }
    const char	*getLabel() const		{ return myLabel; }
    const char	*getDefaultSettings() const	{ return myDefaultSettings; }

private:
    char	*myChannelName, *myType, *myDesc, *myLabel, *myDefaultSettings;
};

class PRM_API DS_SelectorBinding {
public:
    DS_SelectorBinding(const char *parm_name,
			const char *type, const char *prompt, const char *desc,
			const char *mask, int dragging,
			const char *menu, int asterisk,
			int input_index=-1, int input_required=0)
	: myParmName(0), myType(0), myDesc(0), myPrompt(0), myMask(0), myMenu(0)
    {
	myDragging = dragging;
	myAsterisk = asterisk;
	myInputIndex = input_index;
	myInputRequired = input_required;
	if (parm_name)	myParmName = ::strdup(parm_name);
	if (type)	myType = ::strdup(type);
	if (prompt)	myPrompt = ::strdup(prompt);
	if (desc)	myDesc = ::strdup(desc);
	if (mask)	myMask = ::strdup(mask);
	if (menu)	myMenu = ::strdup(menu);
    }
    ~DS_SelectorBinding()
    {
	if (myParmName) ::free(myParmName);
	if (myType)	::free(myType);
	if (myPrompt)	::free(myPrompt);
	if (myDesc)	::free(myDesc);
	if (myMenu)	::free(myMenu);
	if (myMask)	::free(myMask);
    }

    const char	*getParmName() const		{ return myParmName; }
    const char	*getType() const		{ return myType; }
    const char	*getPrompt() const		{ return myPrompt; }
    const char	*getDesc() const		{ return myDesc; }
    const char	*getPrimMask() const		{ return myMask; }
    int		 getDragging() const		{ return myDragging; }
    const char	*getMenu() const		{ return myMenu; }
    int		 getAsterisk() const		{ return myAsterisk; }
    int		 getInputIndex() const		{ return myInputIndex; }
    int		 getInputRequired() const	{ return myInputRequired; }

private:
    char	*myParmName, *myType, *myPrompt, *myDesc, *myMask, *myMenu;
    int		 myDragging, myAsterisk;
    int		 myInputIndex, myInputRequired;
};

class PRM_API DS_DisableInfo
{
public:
     DS_DisableInfo() {}
    ~DS_DisableInfo();

    void	addInfo(const char *parm, const char *value)
		{ myControlParm.append(  ::strdup(parm) );
		  myControlValue.append( ::strdup(value) ); }

    UT_Array<char *>		myControlParm;
    UT_Array<char *>		myControlValue;
};

typedef UT_ValArray< DS_MenuChoice * > DS_Menu;
typedef UT_ValArray< DS_AttributeValue * > DS_AttributeList;
typedef UT_ValArray< DS_HandleBinding * > DS_HandleList;
typedef UT_ValArray< DS_SelectorBinding * > DS_SelectorList;
typedef UT_ValArray< DS_DisableInfo * > DS_DisableList;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class PRM_API DS_ParmInfo
{
public:
     DS_ParmInfo( const char *token );
    ~DS_ParmInfo();

    void	addHelpText(const char *text);

    UT_String		 myToken;	// Actual parm name
    UT_String		 myLabel;
    UT_String		 myCallback;
    UT_String		 myHelpText;
    UT_String		 myRenderManType;
    UT_String		 myOpFilter;
    PRM_Type		 myType;
    PRM_MultiType	 myMultiType;
    unsigned		 myFlag;
    int			 myToolbox;

    UT_String		 myGroup;
    UT_String		 myMenuScript;
    CH_ScriptLanguage	 myMenuScriptLanguage;
    DS_Menu		 myMenu;
    int			 myMenuType;
    DS_AttributeList	 myAttributes;
    DS_HandleList	 myHandles;
    DS_SelectorBinding	*mySelector;
 
    unsigned int	 mySize;   	// Vector size (1, 3, 4, 9, 16, etc.)
    DS_DefaultValue	*myDefault;
    PRM_Range		*myRange;

    DS_DisableList	 myDisableInfo;
    UT_String		 myDisableWhen;	// New style (with {})
    UT_String		 myHideWhen;	// New style (with {})

    bool	 setTypeByToken( const char *token );
    const char	*getTypeToken() const;

    void	 setInvisible();
    bool	 isInvisible() const;

    void	 setLogarithmic(bool flag);
    bool	 isLogarithmic() const;

    // Arrays get translated into multi-parms
    bool	 getIsArray() const	{ return myIsArray; }
    void	 setIsArray(bool f)	{ myIsArray = f; }

    void	 setFlag( unsigned flag )	{ myFlag |= flag; }
    void	 clearFlag( unsigned flag = (unsigned)-1 ){ myFlag &= ~flag; }
    unsigned	 getFlag( unsigned flag ) const	{ return myFlag & flag; }
private:
    bool		 myIsArray;	// Represented as a multi-parm
};

typedef UT_ValArray<DS_ParmInfo*>	DS_ParmInfoList;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class PRM_API DS_ParmReader
{
public:

    // the constructor expects the parms "d:g:cl"
	     DS_ParmReader( unsigned doGroups = 0,
			    unsigned color = 0,
			    unsigned lightChan = 1 );
    virtual ~DS_ParmReader();

    virtual const char	*getClassName() const;

    // File IO
    // all return 0 to indicate error
    virtual int		 open( const char *fileName ) = 0;
    virtual void	 close() = 0;

    // parse will read the given script file, and extract information on
    // the parameters that are present in the script.
    // parse will return the number of parameters it has successfully found.
    // returns 0 to indicate error
    virtual int		 parse() = 0;

    // The following must be filled out by derived classes.
    // NAME and LABEL provide the name(s) for the scripted function.
    // HELP is the help messages that were included in the script to be
    // displayed in the UI to aid the user
    virtual const char	*getScriptTypeName() = 0;
    virtual const char	*getScriptName() = 0;
    virtual const char	*getScriptLabel() = 0;
    virtual const char	*getScriptHelp() = 0;
    virtual const char	*getScriptInfo() = 0;
    virtual const char	*getInputLabel(int /*idx*/)	{ return ""; }
    virtual int		 getNumInputLabels() const	{ return 0; }
    virtual const char 	*getRenderMask()	{ return ""; }

    // returns the number of parms the reader found from the script
    int			 numParms()	{ return myParms.entries(); }

    // given an index, return the information on the index'th parm
    // return 0 to indicate error
    virtual DS_ParmInfo	*getParmInfo( const int index );

    // let others know if we care about grouping of params
    int			 isDoingGroups() const	{ return myDoGroups; }
    void		 setGroupSize(int g)	{ myGroupSize = g; }
    int			 getGroupSize() const	{ return myGroupSize; }
    UT_StringList	&getGroups()		{ return myGroups; }
    DS_SelectorList	&getUnboundSelectors() 	{ return myUnboundSelectors; }
    DS_SelectorList	&getReservedSelectors() { return myReservedSelectors; }
    DS_HandleList	&getReservedHandles() 	{ return myReservedHandles; }
    DS_HandleList	&getBoundHandles()	{ return myBoundHandles; }

protected:
    DS_ParmInfoList		 myParms;
    UT_StringList		 myGroups;
    DS_SelectorList		 myUnboundSelectors;
    DS_SelectorList		 myReservedSelectors;
    DS_HandleList		 myReservedHandles;
    DS_HandleList		 myBoundHandles;

    short			 myDoGroups;
    short			 myGroupSize;
    short			 myColorFlag;
    short			 myLightChan;
};

#endif

