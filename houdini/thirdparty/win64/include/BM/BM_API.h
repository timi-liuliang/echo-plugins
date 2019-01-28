
#ifndef __BM_API_h__
#define __BM_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef BM_EXPORTS
#define BM_API SYS_VISIBILITY_EXPORT
#else
#define BM_API SYS_VISIBILITY_IMPORT
#endif

#endif
