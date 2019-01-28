
#ifndef __CL_API_h__
#define __CL_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef CL_EXPORTS
#define CL_API SYS_VISIBILITY_EXPORT
#else
#define CL_API SYS_VISIBILITY_IMPORT
#endif

#endif
