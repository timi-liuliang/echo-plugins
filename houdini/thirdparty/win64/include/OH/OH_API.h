
#ifndef __OH_API_h__
#define __OH_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef OH_EXPORTS
#define OH_API SYS_VISIBILITY_EXPORT
#else
#define OH_API SYS_VISIBILITY_IMPORT
#endif

#endif
