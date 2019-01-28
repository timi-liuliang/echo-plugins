
#ifndef __MSS_API_h__
#define __MSS_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef MSS_EXPORTS
#define MSS_API SYS_VISIBILITY_EXPORT
#else
#define MSS_API SYS_VISIBILITY_IMPORT
#endif

#endif
