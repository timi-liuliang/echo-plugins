
#ifndef __POPNET_API_h__
#define __POPNET_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef POPNET_EXPORTS
#define POPNET_API SYS_VISIBILITY_EXPORT
#else
#define POPNET_API SYS_VISIBILITY_IMPORT
#endif

#endif
