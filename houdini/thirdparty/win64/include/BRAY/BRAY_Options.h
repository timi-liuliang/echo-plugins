/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BRAY_Options.h (BRAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __BRAY_Options__
#define __BRAY_Options__

#include "BRAY_API.h"
#include <VPRM/VPRM_Stack.h>

enum
{
    BRAY_OPT_EXPORTCOMPONENTS = 0,
    BRAY_OPT_RESOLUTION,
    BRAY_OPT_FOCAL,
    BRAY_OPT_APERTURE,
    BRAY_OPT_ABORTMISSINGTEXTURE,

    BRAY_OPT_MAX_OPTIONS
};

/// Renderer options
class BRAY_API BRAY_Options
{
public:
     BRAY_Options();
    ~BRAY_Options();

    void	push()	{ myStack.push(); }
    void	pop()	{ myStack.pop(); }
    VPRM_DataPtr data() const { return myStack.top(); }

    void	dump() const;
    void	dump(UT_JSONWriter &w) const;

    bool			 bval(int i) const
					{ return data()->getBool(i); }
    const int64			*ival(int i) const
					{ return data()->getInt(i); }
    const fpreal		*fval(int i) const
					{ return data()->getReal(i); }
    const UT_StringHolder	*sval(int i) const
					{ return data()->getString(i); }
    const VPRM_IntArray		*iarr(int i) const
					{ return data()->getVariadicInt(i); }
    const VPRM_RealArray	*farr(int i) const
					{ return data()->getVariadicReal(i); }
    const VPRM_StringArray	*sarr(int i) const
					{ return data()->getVariadicString(i); }


private:
    VPRM_Stack		myStack;
};

#undef GETSET

#endif
