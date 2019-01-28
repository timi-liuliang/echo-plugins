/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GDT Library. (Geo Delta)
 *
 * COMMENTS:	A Token describes changes to a gdp attribute or position,
 *              on points, primitives or vertices.
 *
 */
#ifndef __GDT_Token_h__
#define __GDT_Token_h__

#include "GDT_API.h"
#include <SYS/SYS_Types.h>

class GEO_Detail;
class GDT_TokenList;
class GDT_JSON;
class UT_IStream;
class UT_JSONWriter;
class UT_JSONParser;

class GA_AIFEditDeltaMap;

// This is the base class for all tokens.
class GDT_API GDT_Token
{
public:
    GDT_Token() {}
    GDT_Token(const GDT_Token &) {}
    virtual ~GDT_Token() {}
    virtual int64   getMemoryUsage(bool inclusive=false) const = 0;

    // TODO: Should we wrap GA_AIFEditDeltaMap so that we're passing around a
    //       slightly less attribute-related mapping?
    virtual void	 	 applyDeltas(const GDT_TokenList &list,
					     GEO_Detail &gdp,
					     const GA_AIFEditDeltaMap &map,
					     bool inverse=false) const = 0;

    virtual bool		 legacyLoad(UT_IStream &is,
					    unsigned int numtotal) = 0;

    virtual bool		 save(UT_JSONWriter &w,
				      const GDT_JSON &json) const = 0;
    virtual bool		 load(UT_JSONParser &p,
				      const GDT_JSON &json) = 0;
};

#endif
