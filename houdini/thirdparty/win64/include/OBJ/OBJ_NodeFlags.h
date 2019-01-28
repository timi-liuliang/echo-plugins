/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Flags for a sop (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __OBJ_NodeFlags_h__
#define __OBJ_NodeFlags_h__

#include "OBJ_API.h"
#include <iosfwd>

class UT_IStream;

class OBJ_API OBJ_NodeFlags
{
public:
    OBJ_NodeFlags()	{ setDefaults(); }

    void		setDefaults();

    void		save(std::ostream &os, int binary = 0) const;
    bool		load(UT_IStream &is);

    int			getDisplayOriginFlag() const
			{ return myDisplayOriginFlag; } 
    void		setDisplayOriginFlag(int o)
			{ myDisplayOriginFlag = o; }

    int			getParentingErrorFlag() const
			{ return myParentingErrorFlag; }

    void		setParentingErrorFlag(int o)
			{ myParentingErrorFlag = o; }

private:
    void		loadBinaryFlag(int id, int onOff);
    void		loadAsciiFlag(const char *name, int onOff);

    char		myDisplayOriginFlag;
    char		myParentingErrorFlag;
};

#endif
