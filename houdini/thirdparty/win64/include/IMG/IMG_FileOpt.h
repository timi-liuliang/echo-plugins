/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_FileOpt.h ( IMG Library, C++)
 *
 * COMMENTS:	File options and tags.
 *
 *	For those familiar with the PRM library, there's sort of a
 *	correspondance between the PRM library and this simple implementation.
 *
 *	The obvious similarity is between IMG_FileTag and PRM_Name.  However,
 *	the similarities continue...
 *
 *	A PRM_ParmList which contains parameter values is represented here as a
 *	IMG_FileTagList.  Each entry in the array corresponds to a name/value
 *	pair.  Thus, the tag-list can be appended to, removed, re-ordered etc.
 *	with no effect on how the parameters are interpreted.
 *
 *	The PRM_TemplateList is similar to the IMG_FileOptionList.  Neither
 *	data structure contains any data, but contains the "definitions" for
 *	the available parameters.
 */

#ifndef __IMG_FileOpt__
#define __IMG_FileOpt__

#include "IMG_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_String.h>

class IMG_FileOptionList;

// Hints to file options about data types etc.
enum IMG_FileOptionHint {
	IMG_OPTION_STRING,	// Generic string parameter
	IMG_OPTION_INTEGER,	// Integer parameter
	IMG_OPTION_FLOAT,	// Floating point parameter

	IMG_OPTION_TERMINATOR
};

enum IMG_FileOptionScope {
    IMG_FILE_SCOPE_IN	= 0x1,		// used for input
    IMG_FILE_SCOPE_OUT  = 0x2,		// used for output
    IMG_FILE_SCOPE_NORMAL_RASTER = 0x4  // used where deep rasters aren't supp.
};

#define IMG_FILE_SCOPE_INOUT \
	(IMG_FileOptionScope)((IMG_FILE_SCOPE_IN|IMG_FILE_SCOPE_OUT))

//
// Generic token/value pair
class IMG_API IMG_FileTag {
public:
     IMG_FileTag(const char *token=0, const char *value=0)
     {
	myToken = token;
	myValue = value;
     }
    ~IMG_FileTag() {}

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myToken.getMemoryUsage(false);
        mem += myValue.getMemoryUsage(false);
        return mem;
    }

    const char	*getToken() const	{ return myToken; }
    const char	*getValue() const	{ return myValue; }

    void	 setToken(const char *t)	{ myToken = t; }
    void	 setValue(const char *v)	{ myValue = v; }

    void	 harden()
		 {
		    myToken.harden();
		    myValue.harden();
		 }
private:
    UT_String	myToken;
    UT_String	myValue;
};

class IMG_API IMG_FileTagList {
public:
     IMG_FileTagList();
     IMG_FileTagList(IMG_FileTag *list);
    ~IMG_FileTagList();

    int64 getMemoryUsage(bool inclusive) const;

    void		 setTag(const char *name, const char *value,
				const IMG_FileOptionList *options=0);
    void		 removeTag(const char *name);

    const char		*getTag(const char *name) const;
    const IMG_FileTag	*getTag(int i) const	{ return myList(i); }
    int			 getTagIndex(const char *name) const;
    int			 entries() const	{ return myList.entries(); }
    void		 clear();

private:
    UT_ValArray<IMG_FileTag *>	myList;
    unsigned			myOwnListFlag:1,
				mySortedFlag:1;
};

class IMG_API IMG_FileOption {
public:
    IMG_FileOption(IMG_FileOptionHint hint = IMG_OPTION_TERMINATOR,
		   const char  *name=0, const char *label=0,
		   const char  *defvalue = 0,
		   IMG_FileTag *menu=0, fpreal min=1, fpreal max=0,
		   IMG_FileOptionScope scope = IMG_FILE_SCOPE_OUT)
	: myName(name, label), myMenu(menu)
    {
	myHint = hint;
	myMin = min;
	myMax = max;
	myDefault = defvalue;
	myScope = scope;
    }
    ~IMG_FileOption();

    const char		*getToken() const	{ return myName.getToken(); }
    const char		*getLabel() const	{ return myName.getValue(); }

    const char		*getDefault() const	{ return myDefault; }
    void		 setDefault(const char *str)
			 {
			     myDefault.harden(str);
			 }

    IMG_FileOptionHint	 getHint() const	{ return myHint; }

    int			 hasMenu() const	{ return myMenu.entries(); }
    const IMG_FileTagList	&getMenu() const { return myMenu; }

    // For the option to have a range defined, we require the minimum value to
    // be less than the maximum value.
    int			 hasRange() const	{ return myMin<myMax; }
    fpreal		 getMin() const		{ return myMin; }
    fpreal		 getMax() const		{ return myMax; }

    IMG_FileOptionScope  getScope() const	{ return myScope; }

private:
    IMG_FileTag		 myName;
    IMG_FileTagList	 myMenu;
    UT_String		 myDefault;
    fpreal		 myMin, myMax;
    IMG_FileOptionHint	 myHint;
    IMG_FileOptionScope  myScope;
};

class IMG_API IMG_FileOptionList {
public:
     IMG_FileOptionList(IMG_FileOption *list);
    ~IMG_FileOptionList();

    const IMG_FileOption	*findOption(const char *token) const;
    const IMG_FileOption	*getOption(int i) const { return myList(i); }
    int				 entries() const  { return myList.entries(); }

private:
    UT_ValArray<IMG_FileOption *>	myList;
};

#endif
