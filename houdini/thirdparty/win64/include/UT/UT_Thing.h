#ifndef __UT_Thing__
#define __UT_Thing__

#include "UT_API.h"

#include <SYS/SYS_Deprecated.h>

SYS_DEPRECATED_PUSH_DISABLE()

class SYS_DEPRECATED(16.0) UT_API UT_Thing {
public:
    UT_Thing(void *v=0)	 { value.voidp = v; }
    UT_Thing(long  v)	 { value.longv = v; }

    operator	void *() const { return value.voidp; }
    operator	long  () const { return value.longv; }

    template <typename T> T    *asPointer() const { return (T *)value.voidp; }
    long			asLong() const { return value.longv; }

    UT_Thing &operator=(void *v) { value.voidp = v; return *this; }
    UT_Thing &operator=(long  v) { value.longv = v; return *this; }

    UT_Thing &operator=(const UT_Thing &v)
    {
	value.voidp = v.value.voidp;
	return *this;
    }

    union {
	void	*voidp;
	long	 longv;
    } value;
    
private:
    // This is to prevent accidental assignments of a UT_Thing * to a UT_Thing
    // parameter, which will happily compile but produce incorrect results.
    UT_Thing(UT_Thing *) { ; }
};

SYS_DEPRECATED_POP_DISABLE()

#endif
