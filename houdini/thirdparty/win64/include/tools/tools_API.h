
#ifndef __tools_API_h__
#define __tools_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef tools_EXPORTS
#define tools_API SYS_VISIBILITY_EXPORT
#else
#define tools_API SYS_VISIBILITY_IMPORT
#endif

#endif
