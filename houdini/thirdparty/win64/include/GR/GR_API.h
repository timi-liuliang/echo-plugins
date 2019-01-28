
#ifndef __GR_API_h__
#define __GR_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef GR_EXPORTS
#define GR_API SYS_VISIBILITY_EXPORT
#else
#define GR_API SYS_VISIBILITY_IMPORT
#endif

#endif
