
#ifndef __GD_API_h__
#define __GD_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef GD_EXPORTS
#define GD_API SYS_VISIBILITY_EXPORT
#else
#define GD_API SYS_VISIBILITY_IMPORT
#endif

#endif
