
#ifndef __OBJ_API_h__
#define __OBJ_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef OBJ_EXPORTS
#define OBJ_API SYS_VISIBILITY_EXPORT
#else
#define OBJ_API SYS_VISIBILITY_IMPORT
#endif

#endif
