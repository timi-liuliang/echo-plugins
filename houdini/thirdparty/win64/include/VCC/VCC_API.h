
#ifndef __VCC_API_h__
#define __VCC_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef VCC_EXPORTS
#define VCC_API SYS_VISIBILITY_EXPORT
#else
#define VCC_API SYS_VISIBILITY_IMPORT
#endif

#endif
