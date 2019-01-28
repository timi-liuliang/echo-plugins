
#ifndef __TS_API_h__
#define __TS_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef TS_EXPORTS
#define TS_API SYS_VISIBILITY_EXPORT
#else
#define TS_API SYS_VISIBILITY_IMPORT
#endif

#endif
