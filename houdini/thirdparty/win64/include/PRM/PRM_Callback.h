/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_Callback.h (PRM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __PRM_CALLBACK_H_INCLUDED__
#define __PRM_CALLBACK_H_INCLUDED__

#include "PRM_API.h"

#include <UT/UT_Assert.h>
#include <SYS/SYS_Types.h>

#include <stddef.h>


class PRM_Template;
class UT_Options;

/// This is a callback function that can be used to modify data.
/// It is invoked when the user uses a parameter of type PRM_CALLBACK
/// (this might be a button, or a menu).
/// @param data	    Pointer to the node it was called on
/// @param index    The index of the menu entry
/// @param time	    Current evaluation time
/// @param tplate   Pointer to the PRM_Template of the parameter it was
///		    triggered for.
/// @return It should return 1 if you want the dialog to refresh
///	    (ie if you changed any values) and 0 otherwise.
class PRM_Callback
{
public:
    typedef int (*PRM_Callback32)(void *data, int index, fpreal32 time,
				  const PRM_Template *tplate);
    typedef int (*PRM_Callback64)(void *data, int index, fpreal64 time,
				  const PRM_Template *tplate);
    typedef int (*PRM_CallbackOpt)(void *data, int index, fpreal64 time,
				  const PRM_Template *tplate,
				  const UT_Options *options);

    PRM_Callback()
    {
	myCallback64 = NULL;
	myCallback32 = NULL;
	myCallbackOpt = NULL;
    }
    PRM_Callback(int ptr)
    {
	UT_ASSERT(ptr == 0);
	myCallback64 = NULL;
	myCallback32 = NULL;
	myCallbackOpt = NULL;
    }
    PRM_Callback(long int ptr)
    {
	UT_ASSERT(ptr == 0);
	myCallback64 = NULL;
	myCallback32 = NULL;
	myCallbackOpt = NULL;
    }
    PRM_Callback(std::nullptr_t)
    {
        myCallback64 = NULL;
        myCallback32 = NULL;
        myCallbackOpt = NULL;
    }
    PRM_Callback(PRM_Callback32 cb)
    {
	myCallback64 = NULL;
	myCallback32 = cb;
	myCallbackOpt = NULL;
    }
    PRM_Callback(PRM_Callback64 cb)
    {
	myCallback64 = cb;
	myCallback32 = NULL;
	myCallbackOpt = NULL;
    }
    PRM_Callback(PRM_CallbackOpt cb)
    {
	myCallback64 = NULL;
	myCallback32 = NULL;
	myCallbackOpt = cb;
    }

    int operator()(void *data, int index, fpreal64 time,
		   const PRM_Template *tplate)
    {
	if (myCallbackOpt)
	    return myCallbackOpt(data, index, time, tplate, 0);
	else if (myCallback64)
	    return myCallback64(data, index, time, tplate);
	else if (myCallback32)
	    return myCallback32(data, index, fpreal32(time), tplate);
	return 0;
    }
    int operator()(void *data, int index, fpreal64 time,
		   const PRM_Template *tplate,
		   const UT_Options *options)
    {
	if (myCallbackOpt)
	    return myCallbackOpt(data, index, time, tplate, options);
	else if (myCallback64)
	    return myCallback64(data, index, time, tplate);
	else if (myCallback32)
	    return myCallback32(data, index, fpreal32(time), tplate);
	return 0;
    }
    bool operator==(const PRM_Callback &other)
    {
	return (myCallbackOpt == other.myCallbackOpt
		&& myCallback64 == other.myCallback64
		&& myCallback32 == other.myCallback32);
    }

    bool isNotNull() const
    {
	return (myCallbackOpt || myCallback64 || myCallback32);
    }
    bool isNull() const
    {
	return !isNotNull();
    }

private:
    PRM_CallbackOpt  myCallbackOpt;
    PRM_Callback64  myCallback64;
    PRM_Callback32  myCallback32;
};

#endif // __PRM_CALLBACK_H_INCLUDED__
