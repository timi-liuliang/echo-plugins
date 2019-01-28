
#ifndef __TIL_API_h__
#define __TIL_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef TIL_EXPORTS
#define TIL_API SYS_VISIBILITY_EXPORT
#else
#define TIL_API SYS_VISIBILITY_IMPORT
#endif

#endif
