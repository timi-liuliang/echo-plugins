
#ifndef __GQ_API_h__
#define __GQ_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef GQ_EXPORTS
#define GQ_API SYS_VISIBILITY_EXPORT
#else
#define GQ_API SYS_VISIBILITY_IMPORT
#endif

#endif
