
#ifndef __FS_API_h__
#define __FS_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef FS_EXPORTS
#define FS_API SYS_VISIBILITY_EXPORT
#else
#define FS_API SYS_VISIBILITY_IMPORT
#endif

#endif
