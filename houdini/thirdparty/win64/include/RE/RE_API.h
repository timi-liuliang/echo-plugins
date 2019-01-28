
#ifndef __RE_API_h__
#define __RE_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef RE_EXPORTS
#define RE_API SYS_VISIBILITY_EXPORT
#else
#define RE_API SYS_VISIBILITY_IMPORT
#endif

#endif
