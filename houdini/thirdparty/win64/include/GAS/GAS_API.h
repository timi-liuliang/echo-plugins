
#ifndef __GAS_API_h__
#define __GAS_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef GAS_EXPORTS
#define GAS_API SYS_VISIBILITY_EXPORT
#else
#define GAS_API SYS_VISIBILITY_IMPORT
#endif

#endif
