
#ifndef __CVEX_API_h__
#define __CVEX_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef CVEX_EXPORTS
#define CVEX_API SYS_VISIBILITY_EXPORT
#else
#define CVEX_API SYS_VISIBILITY_IMPORT
#endif

#endif
