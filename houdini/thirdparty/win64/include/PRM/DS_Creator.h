/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DS_Creator.h ( vexds Library, C++)
 *
 * COMMENTS:	Writes dialog scripts according to the parameter
 *		information obtained from a DS_ParmReader object
 */

#ifndef __DS_Creator__
#define __DS_Creator__

#include "PRM_API.h"
#include <iosfwd>
#include <UT/UT_String.h>
#include <UT/UT_StringArray.h>
#include "DS_ParmReader.h"

class DS_SelectorBinding;

#define DS_DEFNAME	"DScreator"
#define DS_DEFDIR	"."

class PRM_API DS_Creator
{
public:
	     DS_Creator( DS_ParmReader &pr,
			 const char *appName = DS_DEFNAME ); 
    virtual ~DS_Creator();

    int			 openOutput( std::ostream &os );
    int			 closeOutput();

    int			 prepareToWriteOutput();
    int			 writeOutput();

    void		 setParmDSOverride(const char *parm_ds)
			    { myParmDSOverride.harden(parm_ds); }

    static void		 setUseOldGrouping(bool useold);
    static bool		 getUseOldGrouping();

    /// Returns an indentation string for the given indent level.
    ///
    /// The string is kept in a static buffer, which has the advantage of
    /// automatically caching it, but you are thus meant to call getIndent 
    /// pretty much every time you want to indent something
    static const char	*getIndent(int level);

    static const char	*theToolboxLabels[];

    static void		 dumpParm(const DS_ParmInfo *pi);

protected:
    void		 writeUnboundSelectors(DS_SelectorList &selectors);
    void		 writeReservedSelectors(DS_SelectorList &selectors);
    void		 writeReservedHandles(DS_HandleList &handles);
    void		 writeBoundHandles(DS_HandleList &handles);
    void		 writeMetaInfo();
    void		 writeParms() const;
    virtual void	 writeContextSpecific();
    virtual int		 writeInfo();
    virtual void	 writeHeader(const char *appname, const char *script);
    virtual void	 writeTrailer(const char *appname, const char *script);

    static void		 startGroup(std::ostream &os, int &id, const char *name);
    static void		 printParm(std::ostream &os, const DS_ParmInfo *pi);

    static void		 printHelp( std::ostream &os, const char *help );
    static void		 printInfo( std::ostream &os, const char *info );

    DS_ParmReader	&myPR;
    UT_String		 myAppName;
    std::ostream	*myOS;
    UT_String		 myParmDSOverride;

    static UT_StringArray	 theGroupStack;
    static bool			 theUseOldGrouping;
};

#endif
