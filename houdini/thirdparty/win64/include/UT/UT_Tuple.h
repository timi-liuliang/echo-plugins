/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	This file defines a number of functions named UTlhsTuple() (tuples
 *	that are valid left hand sides in assignments).  These functions
 *	are inspired by python, where it's very easy to return multiple values
 *	from a function:
 *
 *	    def foo():
 *	        return ([0, 1, 2], 6)
 *
 *	    (a, b) = foo()
 *	    ...do something with a and b...
 *
 *	The C++ equivalent works with boost tuples, and looks like:
 *
 *          static UT_Tuple<UT_Vector3, int> foo()
 *          { return UTmakeTuple(UT_Vector3(0, 1, 2), 6); }
 *
 *          UT_Vector3 a;
 *          int b;
 *          UTlhsTuple(a, b) = foo();
 *          ...do something with a and b...
 *
 *	One handy use of these functions is when you want to pass multiple
 *	values into a callback function that takes a void *.  Instead of
 *	creating a struct to hold the values, pass in a pointer to a
 *	UT_Tuple
 *          void iterateOverSomething(
 *              void (*callback)(void *), void *callback_data));
 *
 *          static void my_callback(void *data)
 *          {
 *              UT_Vector3 a;
 *              int b;
 *              UTlhsTuple(a, b) = *(UT_Tuple<UT_Vector3, int>*)data;
 *              ...do something with a and b...
 *          }
 *
 *          UT_Tuple<UT_Vector3, int> data(UT_Vector3(1,2,3), 4);
 *          iterateOverSomething(my_callback, &data);
 */

#ifndef __UT_TUPLE_H_INCLUDED__
#define __UT_TUPLE_H_INCLUDED__

#include <tuple>

template <class... Types> 
using UT_Tuple = std::tuple<Types...>;

#define UTtupleIgnore	std::ignore

#define UTtupleGet	std::get

#define UTmakeTuple	std::make_tuple

#define UTlhsTuple	std::tie

#endif // __UT_TUPLE_H_INCLUDED__
