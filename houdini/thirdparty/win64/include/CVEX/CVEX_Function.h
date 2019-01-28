/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CVEX_Function.h ( CVEX Library, C++)
 *
 * COMMENTS:	C++ interface to VEX.  This class is used to hold
 *		a generated function.
 */

#ifndef __CVEX_Function__
#define __CVEX_Function__

#include "CVEX_API.h"
#include "CVEX_Value.h"	// for CVEX_Type
#include <UT/UT_IntrusivePtr.h>

class VEX_Assemble;
using VEX_AssemblePtr = UT_IntrusivePtr<VEX_Assemble>;
class UT_StringArray;
template <typename T> class UT_Array;

/// @brief A class holding a VEX function.
///
/// This will be created by something like the CVEX_Context::preloadFunction.
/// It can then be passed directly to loadFunction() to be used rather
/// than going to the global vex context function table.
class CVEX_API CVEX_Function
{
public:
		 CVEX_Function();
		 CVEX_Function(const CVEX_Function &src);
		~CVEX_Function();

    const CVEX_Function &operator=(const CVEX_Function &src);

    bool	isValid() const { return myAssembly.get() != nullptr; }

    // Returns true if any export/import has this as a prefix.
    bool	hasParameterWithPrefix(const char *prefix);

    /// Returns the function parameters boundable by adding CVEX input.
    bool	getParameters(UT_StringArray &parm_names,
				UT_Array<CVEX_Type> &parm_types,
				UT_IntArray &parm_exports) const;

private:
    VEX_AssemblePtr	myAssembly;
    friend class	CVEX_Context;
};

#endif

