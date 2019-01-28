
#ifndef __DEP_API_h__
#define __DEP_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef DEP_EXPORTS
#define DEP_API SYS_VISIBILITY_EXPORT
#else
#define DEP_API SYS_VISIBILITY_IMPORT
#endif

#endif
