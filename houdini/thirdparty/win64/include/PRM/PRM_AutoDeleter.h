/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *      This class provides a way to track a set of objects that were allocated
 *      and then delete them later on.  It is particularly useful for deleting
 *      elements pointed to by a PRM_Template.
 *
 *      Note that the objects will be deleted in the *same* order they are
 *      added to the PRM_AutoDeleter.
 *
 *      Sample usage:
 *
 *          static void
 *          deleteDefaults(PRM_Default *defaults)
 *          {
 *              for (int i=0; i<defaults->getVectorSize(); ++i)
 *                  defaults[i].setString(nullptr, true);// free existing string
 *              delete defaults;
 *          }
 *     
 *          PRM_Template *
 *          newTemplate(PRM_AutoDeleter &deleter)
 *          {
 *              return deleter.append(new PRM_Template(
 *                  ...,
 *                  deleter.append(new PRM_Range(...)),
 *                  deleter.appendCallback(
 *                      deleteDefaults, new PRM_Default(...)),
 *                  ...));
 *          }
 *     
 *          void
 *          foo()
 *          {
 *              PRM_AutoDeleter deleter;
 *              PRM_Template *prm_template = newTemplate(deleter);
 *     
 *              ...do something with prm_template...
 *     
 *              // When "deleter" is destroyed, it will delete the PRM_Template
 *              // and the things earlier allocated.
 *          }
 *
 *      Note that if you want a callback to take additional arguments, you
 *      can use lambdas.
 */

#ifndef __PRM_AutoDeleter_h__
#define __PRM_AutoDeleter_h__

#include "PRM_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_UniquePtr.h>
#include <type_traits>
#include <utility>


class PRM_AutoDeleter : UT_NonCopyable
{
private:
    // This base class exists so we can have an array of different types of
    // deleters.
    class prm_Deleter : UT_NonCopyable
    {
    public:
	virtual ~prm_Deleter()
	{}
    };

    // This templated class calls delete on a pointer.
    template <typename T>
    class prm_ValueDeleter : public prm_Deleter
    {
    public:
	prm_ValueDeleter(T *value)
	: myPtr(value)
	{}
    private:
	UT_UniquePtr<T> myPtr;
    };

    // This templated class calls delete [] on a pointer.
    template <typename T>
    class prm_ArrayDeleter : public prm_Deleter
    {
    public:
	prm_ArrayDeleter(T *array)
	: myArrayPtr(array)
	{}
    private:
	UT_UniquePtr<T[]> myArrayPtr;
    };

    // This templated class runs a callback with a pointer.  It is up to
    // the callback to call delete if it wants to.
    template <typename T, typename CB>
    class prm_CallbackDeleter : public prm_Deleter
    {
    public:
	prm_CallbackDeleter(const CB &callback, T *value)
	: myCallback(callback)
	, myValue(value)
	{}

	virtual ~prm_CallbackDeleter()
	{ myCallback(myValue); }

    private:
	CB myCallback;
	T *myValue;
    };

public:
    // Note that we want objects to be deleted in the order they're added
    // to the std::vector, so we purposely append them to the end.

    template <typename T>
    T *append(T *value)
    {
	if (!value)
	    return nullptr;

	myDeleters.emplace_back(UTmakeUnique<prm_ValueDeleter<T>>(value));
	return value;
    }

    template <typename T>
    T *appendArray(T *array)
    {
	if (!array)
	    return nullptr;

	myDeleters.emplace_back(UTmakeUnique<prm_ArrayDeleter<T>>(array));
	return array;
    }

    // Note that appendCallback does not delete the pointer.  It's up to you
    // to delete it from within the callback if that's what you want.
    template <typename T, typename CB>
    T *appendCallback(CB &&callback, T *pointer)
    {
	if (!pointer)
	    return nullptr;

	// We need to decay the CB type since it can be a REFERENCE to the 
	// intended callback and we really need to make a copy of it here.
	using F = typename std::decay<CB>::type;
	myDeleters.emplace_back(UTmakeUnique<prm_CallbackDeleter<T,F>>(
					std::forward<CB>(callback), pointer));
	return pointer;
    }

private:
    UT_Array<UT_UniquePtr<prm_Deleter> > myDeleters;
};

#endif
