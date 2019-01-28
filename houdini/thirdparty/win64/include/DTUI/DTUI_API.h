
#ifndef __DTUI_API_h__
#define __DTUI_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef DTUI_EXPORTS
#define DTUI_API SYS_VISIBILITY_EXPORT
#else
#define DTUI_API SYS_VISIBILITY_IMPORT
#endif

#endif
