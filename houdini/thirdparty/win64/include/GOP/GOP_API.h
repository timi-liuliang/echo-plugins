
#ifndef __GOP_API_h__
#define __GOP_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef GOP_EXPORTS
#define GOP_API SYS_VISIBILITY_EXPORT
#else
#define GOP_API SYS_VISIBILITY_IMPORT
#endif

#endif
