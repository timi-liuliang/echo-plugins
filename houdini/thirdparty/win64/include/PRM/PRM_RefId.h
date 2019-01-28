/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_RefId.h (PRM Library, C++)
 *
 * COMMENTS:	A simple pair class of parm index and sub index.
 */

#ifndef __PRM_REFID_H_INCLUDED__
#define __PRM_REFID_H_INCLUDED__

#include "PRM_API.h"
#include <UT/UT_Assert.h>
#include <SYS/SYS_Math.h>

class UT_String;
class CH_Channel;
class PRM_ParmList;

class PRM_API PRM_RefId
{
public:
    PRM_RefId()
    {
	myParmIndex = -1;
	myParmSubIndex = -1;
    }
    PRM_RefId(int pi, int vi)
	: myParmIndex(pi)
	, myParmSubIndex(vi)
    {
    }

    PRM_RefId( const PRM_RefId &copy ) = default;

    PRM_RefId &operator =( const PRM_RefId &copy ) = default;

    bool isValid() const
	    { return myParmIndex >= 0; }

    void setParmRef( int parm_index, int sub_index = -1 )
    {
	myParmIndex = parm_index;
	myParmSubIndex = sub_index;
    }
    int getParmRef() const
    {
	UT_ASSERT_P( isValid() );
	return myParmIndex;
    }
    void setParmSubIndex(int sub_index)
    {
	myParmSubIndex = sub_index;
    }
    int getParmSubIndex() const
    {
	UT_ASSERT_P( isValid() );
	return myParmSubIndex;
    }

    unsigned	hash() const
		{
		    return SYSwang_inthash(
			    myParmIndex * 1024 + myParmSubIndex
			);
		}
    int operator==( const PRM_RefId &other) const
    {
	return( myParmIndex == other.myParmIndex &&
		myParmSubIndex == other.myParmSubIndex );
    }
    // matches() is like operator==() except it also takes into account -1
    // subindices to mean there's no info about them
    bool matches(const PRM_RefId &other) const
    {
	if (myParmIndex != other.myParmIndex)
	    return false;
	if (myParmSubIndex == -1 || other.myParmSubIndex == -1)
	    return true;
	return (myParmSubIndex == other.myParmSubIndex);
    }

    void getToken(const PRM_ParmList &parmlist, UT_String &token) const;
    void getAlias(const PRM_ParmList &parmlist, UT_String &alias) const;

private:
    int		    myParmIndex;
    int		    myParmSubIndex;
};

#endif // __PRM_REFID_H_INCLUDED__
