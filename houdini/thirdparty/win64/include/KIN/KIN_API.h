
#ifndef __KIN_API_h__
#define __KIN_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef KIN_EXPORTS
#define KIN_API SYS_VISIBILITY_EXPORT
#else
#define KIN_API SYS_VISIBILITY_IMPORT
#endif

#endif
