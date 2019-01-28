
#ifndef __RU_API_h__
#define __RU_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef RU_EXPORTS
#define RU_API SYS_VISIBILITY_EXPORT
#else
#define RU_API SYS_VISIBILITY_IMPORT
#endif

#endif
