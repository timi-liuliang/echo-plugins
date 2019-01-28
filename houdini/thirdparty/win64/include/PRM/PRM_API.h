
#ifndef __PRM_API_h__
#define __PRM_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef PRM_EXPORTS
#define PRM_API SYS_VISIBILITY_EXPORT
#else
#define PRM_API SYS_VISIBILITY_IMPORT
#endif

#endif
