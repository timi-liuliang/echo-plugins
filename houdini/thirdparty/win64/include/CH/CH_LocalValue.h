/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Channel Library (C++)
 *
 * COMMENTS:	Value class for local variable values.
 *
*/

#ifndef __CH_LocalValue_h__
#define __CH_LocalValue_h__

#include "CH_API.h"
#include <SYS/SYS_Types.h>

class CH_API CH_LocalValue
{
public:
    CH_LocalValue(fpreal val = 0) { value.val = val; }
    CH_LocalValue(void *ptr) { value.ptr = ptr; }
    CH_LocalValue(char *str) { value.str = str; }

    fpreal	 getFloat()  const { return value.val; }
    void	*getData()   const { return value.ptr; }
    char	*getString() const { return value.str; }

    CH_LocalValue	&operator=(const CH_LocalValue &d)
			 {
			    value.ptr = d.value.ptr;
			    return *this;
			 }
    fpreal		 operator=(fpreal f) { value.val = f; return f; }
    char		*operator=(char *s) { value.str = s; return s; }
    void		*operator=(void *v) { value.ptr = v; return v; }
    unsigned		 operator==(const CH_LocalValue &d)
			 {
			    return (value.ptr == d.value.ptr);
			 }

    // Public data
    union {
	void	*ptr;
	char	*str;
	fpreal	 val;
    } value;
};

#endif

