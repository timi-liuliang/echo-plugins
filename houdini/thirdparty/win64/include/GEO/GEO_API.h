
#ifndef __GEO_API_h__
#define __GEO_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef GEO_EXPORTS
#define GEO_API SYS_VISIBILITY_EXPORT
#else
#define GEO_API SYS_VISIBILITY_IMPORT
#endif

#endif
