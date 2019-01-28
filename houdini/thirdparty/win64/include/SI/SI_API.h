
#ifndef __SI_API_h__
#define __SI_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef SI_EXPORTS
#define SI_API SYS_VISIBILITY_EXPORT
#else
#define SI_API SYS_VISIBILITY_IMPORT
#endif

#endif
