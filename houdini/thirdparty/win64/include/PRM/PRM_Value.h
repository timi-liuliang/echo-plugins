/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_Value.h
 *
 * COMMENTS:
 *
 */

#ifndef __PRM_Value_h__
#define __PRM_Value_h__

#include "PRM_API.h"
#include <UT/UT_Defines.h>
#include <UT/UT_ValArray.h>

class PRM_Parm;
class PRM_EventHandler;

typedef	void	(*PRM_ValueCallback)(void *callee, void *value);

class PRM_API PRM_Value
{
public:
    PRM_Value();
    PRM_Value(const PRM_Value &src);
    ~PRM_Value();

    void		    addInterest(void *callee,
					PRM_ValueCallback callback);
    void		 removeInterest(void *callee,
					PRM_ValueCallback callback);
    bool		    hasInterest(void *callee) const;


    void		 changed();

	  void		 setValue(void *value)	{ myValue = value; }
    PRM_Value		&operator=(void *value)
					    { myValue = value; return *this; }

	  void		*getValue()		{ return myValue; }
    const void		*getValue() const	{ return myValue; }
    operator		 void*()		{ return myValue; }
    operator const	 void*() const		{ return myValue; }

    int64		 getMemoryUsage(bool inclusive) const;

private:
    void		*myValue;
    UT_ValArray<PRM_EventHandler *>	 myHandlers;
};

#endif
