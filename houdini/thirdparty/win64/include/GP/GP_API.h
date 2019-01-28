
#ifndef __GP_API_h__
#define __GP_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef GP_EXPORTS
#define GP_API SYS_VISIBILITY_EXPORT
#else
#define GP_API SYS_VISIBILITY_IMPORT
#endif

#endif
