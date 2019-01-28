
#ifndef __VIS_API_h__
#define __VIS_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef VIS_EXPORTS
#define VIS_API SYS_VISIBILITY_EXPORT
#else
#define VIS_API SYS_VISIBILITY_IMPORT
#endif

#endif
