/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ScopeExit.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_SCOPEEXIT_H_INCLUDED__
#define __UT_SCOPEEXIT_H_INCLUDED__

/// @file
///
/// UT_SCOPE_EXIT/UT_AT_SCOPE_EXIT provides convenient mechanisms to do RAII
/// without writing a special struct does cleans up upon destruction. The code
/// within the scope exit block is executed in reverse order of declaration as
/// the innermost enclosing scope ends. If execution never passes through a
/// scope exit block in the first place, then it won't be executed.
///

#include "UT_API.h"
#include <utility>  // for std::move()

/// UT_ScopeExit which runs the given lambda on destruction
template <typename F>
class UT_ScopeExit
{
public:
    explicit	UT_ScopeExit(F& action)
		    : myAction(action) { }
    explicit	UT_ScopeExit(const F&& action)
		    : myAction(action) { }
    explicit	UT_ScopeExit(F&& action)
		    : myAction(std::move(action)) { }
		UT_ScopeExit(UT_ScopeExit&& other)
		    : myAction(std::move(other.myAction)) { }

		~UT_ScopeExit()
		    { myAction(); }
private:
    F		myAction;
};

// Taken from the CppCon 2015 presentation, "Declarative Control Flow", by
// Andrei Alexandrescu.
// https://github.com/CppCon/CppCon2015/tree/master/Presentations/Declarative%20Control%20Flow
namespace UT_ScopeGuard
{

// Helper type for the operator+() below
enum class OnExit { };

// The operator+() is used to allow type deduction while leaving the function
// body at end of the statement.
// NB: To support a wider class of functors F, we can use
//     UT_ScopeExit<typename std::decay<F>::type> but that would need to pull
//     in <type_traits> which do not want to do.
template <typename F>
UT_ScopeExit<F>
operator+(OnExit, F&& fn)
{
    return UT_ScopeExit<F>(std::forward<F>(fn));
}

} // namespace UT_ScopeGuard

// UT_SCOPE_EXIT_VAR(name) creates a unique variable name<X> where <X> is
// different everytime the macro is used.
// TODO: Move this somewhere more generic for use
#define UT_SCOPE_EXIT_CONCAT_IMPL(x, y)	x ## y
#define UT_SCOPE_EXIT_CONCAT(x, y)	UT_SCOPE_EXIT_CONCAT_IMPL(x, y)
#define UT_SCOPE_EXIT_VAR(name)		UT_SCOPE_EXIT_CONCAT(name, __COUNTER__)

// Taken from the CppCon 2014 presentation,
// "C++11 in the Wild - Techniques from a Real Codebase", by Arthur O'Dwyer.
// https://github.com/CppCon/CppCon2014/tree/master/Presentations/C%2B%2B11%20in%20the%20Wild%20-%20Techniques%20from%20a%20Real%20Codebase
#define UT_SCOPE_EXIT_RUN_IMPL(func_name, scope_name, ...) \
	    auto func_name = [&]() { __VA_ARGS__; }; \
	    UT_ScopeExit<decltype(func_name)> scope_name(func_name) \
	    /**/

/// Runs given statement within the parentheses when the scope exits.
/// @note Put the semi-colon outside the parentheses.
/// Example
/// @code
///	FILE *fp = ::fopen("foo", "r");
///	if (fp)
///	{
///	    UT_AT_SCOPE_EXIT(::fclose(fp)); // <= NOTE: SEMI-COLON OUTSIDE
///	    ...
///	}
/// @endcode
#define UT_AT_SCOPE_EXIT(...) \
	    UT_SCOPE_EXIT_RUN_IMPL(UT_SCOPE_EXIT_VAR(scope_exit_lambda), \
				   UT_SCOPE_EXIT_VAR(scope_exit_scope), \
				   __VA_ARGS__) \
	    /**/

/// Runs given statement block after it when the current scope exits.
/// @note Put a semi-colon after the statement block!
/// Example
/// @code
///	FILE *fp = ::fopen("foo", "r");
///	UT_SCOPE_EXIT
///	{
///	    if (fp)
///		::fclose(fp);
///	}; // <= NOTE: SEMI-COLON ENDS THE BLOCK
/// @endcode
#define UT_SCOPE_EXIT \
	    auto UT_SCOPE_EXIT_VAR(scope_exit_scope) \
		    = UT_ScopeGuard::OnExit() + [&]() noexcept \
	    /**/

#endif // __UT_SCOPEEXIT_H_INCLUDED__
