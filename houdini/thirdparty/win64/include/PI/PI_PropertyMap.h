/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PI_PropertyMap.h ( OP Library, C++)
 *
 * COMMENTS:	When loading in old .hip files, there are some operators which
 *		want to handle obsolete parameters by creating spare
 *		properties.  This class helps manage that process.
 */

#ifndef __PI_PropertyMap__
#define __PI_PropertyMap__

#include "PI_API.h"
#include <UT/UT_SymbolTable.h>

class PRM_ParmNameMap;
class PI_EditScriptedParms;

class PI_API PI_PropertyMapDef {
public:
    PI_PropertyMapDef(const char *parmname,
		      const char *propertyfile,
		      const char *propertyname,
		      const char *ch0=0, const char *ch1=0,
		      const char *ch2=0, const char *ch3=0)
    {
	myParm = parmname;
	myPropFile = propertyfile;
	myProperty = propertyname;
	if (!ch0)
	    ch0 = myParm;
	myChannels[0] = ch0;
	myChannels[1] = ch1;
	myChannels[2] = ch2;
	myChannels[3] = ch3;
	setRename(parmname, ch0, ch1, ch2, ch3);
    }

    // If the parameter is re-named during loading, you can specify the NEW
    // name (and channel names) here.
    void	setRename(const char *r, const char *ch0=0, const char *ch1=0,
					const char *ch2=0, const char *ch3=0)
		{
		    myRename = r;
		    myChRename[0] = ch0;
		    myChRename[1] = ch1;
		    myChRename[2] = ch2;
		    myChRename[3] = ch3;
		}

    const char	*getParm() const		{ return myParm; }
    const char	*getRename() const		{ return myRename; }
    const char	*getPropFile() const		{ return myPropFile; }
    const char	*getProperty() const		{ return myProperty; }
    const char	*getCh(int i) const		{ return myChannels[i]; }
    const char	*getChRename(int i) const	{ return myChRename[i]; }
private:
    const char	*myParm;
    const char	*myPropFile;
    const char	*myProperty;
    const char	*myRename;
    const char	*myChannels[4];
    const char	*myChRename[4];
};

class PI_API PI_PropertyMap {
public:
	     PI_PropertyMap();
    virtual ~PI_PropertyMap();

    void	setMap(int n, const PI_PropertyMapDef map[])
		{
		    myMap = map;
		    myEntries = n;
		    createTokenMap();
		}

    bool	renameParameter(const char *oldname, const char *newname,
			    const char *newch0=0, const char *newch1=0,
			    const char *newch2=0, const char *newch3=0);
    bool	createFromChannel(PI_EditScriptedParms &editparms,
				    PRM_ParmNameMap *nmap,
				    const char *chname);

private:
    void	createTokenMap();

    UT_SymbolMap<const PI_PropertyMapDef *> myTokenMap;
    const PI_PropertyMapDef	*myMap;
    int				 myEntries;
};

#endif
