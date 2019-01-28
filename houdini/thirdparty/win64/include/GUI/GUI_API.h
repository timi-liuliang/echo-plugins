
#ifndef __GUI_API_h__
#define __GUI_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef GUI_EXPORTS
#define GUI_API SYS_VISIBILITY_EXPORT
#else
#define GUI_API SYS_VISIBILITY_IMPORT
#endif

#endif
