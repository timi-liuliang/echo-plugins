
#ifndef __UI_API_h__
#define __UI_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef UI_EXPORTS
#define UI_API SYS_VISIBILITY_EXPORT
#else
#define UI_API SYS_VISIBILITY_IMPORT
#endif

#endif
