
#ifndef __BV_API_h__
#define __BV_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef BV_EXPORTS
#define BV_API SYS_VISIBILITY_EXPORT
#else
#define BV_API SYS_VISIBILITY_IMPORT
#endif

#endif
