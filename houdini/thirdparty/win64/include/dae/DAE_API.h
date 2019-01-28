
#ifndef __DAE_API_h__
#define __DAE_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef DAE_EXPORTS
#define DAE_API SYS_VISIBILITY_EXPORT
#else
#define DAE_API SYS_VISIBILITY_IMPORT
#endif

#endif
