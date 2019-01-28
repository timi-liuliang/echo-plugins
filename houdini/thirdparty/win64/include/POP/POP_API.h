
#ifndef __POP_API_h__
#define __POP_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef POP_EXPORTS
#define POP_API SYS_VISIBILITY_EXPORT
#else
#define POP_API SYS_VISIBILITY_IMPORT
#endif

#endif
