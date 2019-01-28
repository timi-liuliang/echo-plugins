
#ifndef __AU_API_h__
#define __AU_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef AU_EXPORTS
#define AU_API SYS_VISIBILITY_EXPORT
#else
#define AU_API SYS_VISIBILITY_IMPORT
#endif

#endif
