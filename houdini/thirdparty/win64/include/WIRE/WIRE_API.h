
#ifndef __WIRE_API_h__
#define __WIRE_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef WIRE_EXPORTS
#define WIRE_API SYS_VISIBILITY_EXPORT
#else
#define WIRE_API SYS_VISIBILITY_IMPORT
#endif

#endif
