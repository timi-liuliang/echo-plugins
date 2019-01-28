/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SHOP_Data.h ( SHOP Library, C++)
 *
 * COMMENTS:	This class is used for passing data between the SHOP library
 *		and the IFD library.  Because the data passed to renderers can
 *		be arbitrary, we need a nice clean mechanism which doesn't
 *		encourage memory leaks.
 *
 *		This class is meant to be sub-classed.
 */

#ifndef __SHOP_Data__
#define __SHOP_Data__

#include "SHOP_API.h"
class SHOP_Clerk;

class SHOP_API SHOP_Data
{
public:
	     SHOP_Data(const SHOP_Clerk *clerk);
    virtual ~SHOP_Data();

    virtual const void	*getRenderData() = 0;
    virtual const char	*getStringData() = 0;

    int			 matchKey(const SHOP_Clerk *clerk,
				  int caller_obj, int caller_sop)
			 {
			     return clerk == myClerk;
			 }
    const SHOP_Clerk	*getClerk() const	{ return myClerk; }
protected:
    const SHOP_Clerk	*myClerk;

private:
};

class SHOP_API SHOP_StringData : public SHOP_Data
{
public:
	     SHOP_StringData(const SHOP_Clerk *clerk);
    virtual ~SHOP_StringData();

    virtual const void	*getRenderData()	{ return (const void *)myData; }
    virtual const char	*getStringData()	{ return myData; }

    void	setString(const char *str);
    void	adoptString(char *str);

protected:
    char	*myData;
};

#endif
