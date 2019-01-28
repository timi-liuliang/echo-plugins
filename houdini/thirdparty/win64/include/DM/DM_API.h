
#ifndef __DM_API_h__
#define __DM_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef DM_EXPORTS
#define DM_API SYS_VISIBILITY_EXPORT
#else
#define DM_API SYS_VISIBILITY_IMPORT
#endif

#endif
