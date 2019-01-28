/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	(UT_ColorTable.h, C++)
 *
 * COMMENTS:
 */

#ifndef _UT_ColorTable_
#define _UT_ColorTable_

#include "UT_API.h"
#include "UT_StringMap.h"
#include "UT_Color.h"
#include <iosfwd>

class UT_IStream;

class UT_API UT_ColorTable
{
public:
    UT_ColorTable();
    ~UT_ColorTable();

    bool	 empty() const;
    bool	 getColor(const UT_StringRef &colorname, UT_Color &clr) const;
    const UT_StringMap<UT_Color> &getTable() const;

    void	 clear();
    void	 clearColor(const UT_StringRef &colorname);
    void	 setColor(const UT_StringRef &colorname, const UT_Color &clr);
    void	 setTable(const UT_StringMap<UT_Color> &color_map);

    bool	 save(std::ostream &os, int binary) const;
    bool	 load(UT_IStream &is);

private:
    UT_StringMap<UT_Color>	 myColorMap;
};

#endif
