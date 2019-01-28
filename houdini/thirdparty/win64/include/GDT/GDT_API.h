
#ifndef __GDT_API_h__
#define __GDT_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef GDT_EXPORTS
#define GDT_API SYS_VISIBILITY_EXPORT
#else
#define GDT_API SYS_VISIBILITY_IMPORT
#endif

#endif
