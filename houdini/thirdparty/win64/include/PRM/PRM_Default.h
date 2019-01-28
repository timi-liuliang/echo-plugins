/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_Default.h (Parameter Library)
 *
 * COMMENTS:
 */

#ifndef __PRM_Default__
#define __PRM_Default__

#include "PRM_API.h"
#include <SYS/SYS_Types.h>
#include <CH/CH_ExprLanguage.h>

// These options can be OR'd and are processed in order from top to bottom.
enum PRM_DefaultType {
    PRM_DEFAULT_LOCK	= 0x00000001,
    PRM_DEFAULT_USER	= 0x00000002,
    PRM_DEFAULT_EXPR	= 0x00000004,
    PRM_DEFAULT_FCTY	= 0x00000008,
    PRM_DEFAULT_MASK	= 0x0000000F
};

#define PRM_FACTORY_DEF		PRM_DefaultType(	\
				    PRM_DEFAULT_LOCK | PRM_DEFAULT_FCTY)
#define PRM_USER_DEF		PRM_DefaultType(	\
				    PRM_FACTORY_DEF | PRM_DEFAULT_USER)
#define PRM_TRUE_FACTORY_DEF	PRM_DefaultType(	\
				    PRM_FACTORY_DEF | PRM_DEFAULT_EXPR)
#define PRM_FACTORY_UI_DEF	PRM_DefaultType(	\
				    PRM_TRUE_FACTORY_DEF & ~PRM_DEFAULT_LOCK)

//
// For now, ordinal and animated parameters are both stored in a floating
// point number.
//

class PRM_API PRM_Default
{
public:
    /// PRM_Default has two parts: a float and a string. For floating point
    /// parameters, if the default string is defined, then the parameter will
    /// start off being animated and have the channel expression defined by the
    /// string. Otherwise, the floating point default is used for float and
    /// integer types, while the string is used to initialize string types. 
		 PRM_Default(fpreal thefloat=0,
		             const char *thestring = 0,
			     CH_StringMeaning string_meaning =
				CH_STRING_LITERAL);

    fpreal	 getFloat() const	{ return myFloat; }
    int		 getOrdinal() const	{ return (int)myFloat; }
    bool	 gotString() const	{ return (myString) ? true : false; }
    const char	*getString() const	{ return (myString) ? myString : ""; }
    CH_StringMeaning getStringMeaning() const
					{ return myStringMeaning; }

    PRM_Default	&operator=(const PRM_Default &dft);

    void	 set(fpreal thefloat, const char *thestring,
		     CH_StringMeaning string_meaning);

    void	 setFloat(fpreal v)	{ myFloat = v; }
    void	 setOrdinal(int v)	{ myFloat = (fpreal)v; }
    void	 setString(const char *v, bool free_existing = false);
    void	 setStringMeaning(CH_StringMeaning string_meaning);

    /// NOTE: Based on the lack of destructor, PRM_Default does NOT
    ///       own *myString, but ownership semantics are bonkers in
    ///       these mysterious lands of PRM.
    int64	 getMemoryUsage(bool inclusive) const
    { return inclusive ? sizeof(*this) : 0; }

private:
    fpreal	 myFloat;
    const char	*myString;
    CH_StringMeaning myStringMeaning;
};
#endif
