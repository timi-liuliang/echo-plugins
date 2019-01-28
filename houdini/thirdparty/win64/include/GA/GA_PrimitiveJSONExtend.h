/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_PrimitiveJSONExtend.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_PrimitiveJSONExtend__
#define __GA_PrimitiveJSONExtend__

#include "GA_API.h"
#include "GA_PrimitiveJSON.h"


class GA_LoadMap;
class GA_Primitive;
class GA_SaveMap;

class UT_JSONParser;
class UT_JSONValue;
class UT_JSONWriter;


/// Class which extends an existing JSON schema
class GA_API GA_PrimitiveJSONExtend : public GA_PrimitiveJSON 
{
public:
	     GA_PrimitiveJSONExtend(const GA_PrimitiveJSON *base);
    virtual ~GA_PrimitiveJSONExtend();

    /// @{
    /// Methods implemented from base GA_PrimitiveJSON.  These methods are all
    /// implemented as: @code
    ///	    if (isBase(i))
    ///		myBase->method(i, ...);
    ///	    else
    ///		localMethod(getLocal(i), ...);
    /// @endcode
    /// Return the number of data fields in the primitive private schema
    virtual int		getEntries() const;
    virtual const UT_StringHolder	&getKeyword(int i) const;
    virtual bool	saveField(const GA_Primitive *prim, int i,
				UT_JSONWriter &w, const GA_SaveMap &map) const;
    virtual bool	saveField(const GA_Primitive *prim, int i,
				UT_JSONValue &value,
				const GA_SaveMap &map) const;
    virtual bool	loadField(GA_Primitive *prim, int i,
				UT_JSONParser &p, const GA_LoadMap &map) const;
    virtual bool	loadField(GA_Primitive *prim, int i,
				UT_JSONParser &p, const UT_JSONValue &value,
				const GA_LoadMap &map) const;
    virtual bool	isEqual(int i,
				const GA_Primitive *prim1,
				const GA_Primitive *prim2) const;
    /// @}

    /// @{
    /// The local methods return information about the local schema (i.e. the
    /// extending schema.  All indices (@c i) are 0 based.
    virtual int		localEntries() const = 0;
    virtual const UT_StringHolder	&localKeyword(int i) const = 0;
    virtual bool	localSave(const GA_Primitive *prim, int i,
				UT_JSONWriter &w,
				const GA_SaveMap &map) const = 0;
    virtual bool	localSave(const GA_Primitive *prim, int i,
				UT_JSONValue &value,
				const GA_SaveMap &map) const = 0;
    virtual bool	localLoad(GA_Primitive *prim, int i,
				UT_JSONParser &p,
				const GA_LoadMap &map) const = 0;
    virtual bool	localLoad(GA_Primitive *prim, int i,
				UT_JSONParser &p, const UT_JSONValue &value,
				const GA_LoadMap &map) const = 0;
    virtual bool	localIsEqual(int i,
				const GA_Primitive *prim1,
				const GA_Primitive *prim2) const = 0;
    /// @}

protected:
    bool	isBase(int i) const	{ return i < myBaseEntries; }
    int		getLocal(int i) const	{ return i - myBaseEntries; }

    const GA_PrimitiveJSON	*myBase;
    int				 myBaseEntries;
};

#endif
