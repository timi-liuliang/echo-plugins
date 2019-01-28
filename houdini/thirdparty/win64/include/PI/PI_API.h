
#ifndef __PI_API_h__
#define __PI_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef PI_EXPORTS
#define PI_API SYS_VISIBILITY_EXPORT
#else
#define PI_API SYS_VISIBILITY_IMPORT
#endif

#endif
