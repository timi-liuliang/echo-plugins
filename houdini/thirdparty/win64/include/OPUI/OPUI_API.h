
#ifndef __OPUI_API_h__
#define __OPUI_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef OPUI_EXPORTS
#define OPUI_API SYS_VISIBILITY_EXPORT
#else
#define OPUI_API SYS_VISIBILITY_IMPORT
#endif

#endif
