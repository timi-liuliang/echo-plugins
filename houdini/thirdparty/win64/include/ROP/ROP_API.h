
#ifndef __ROP_API_h__
#define __ROP_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef ROP_EXPORTS
#define ROP_API SYS_VISIBILITY_EXPORT
#else
#define ROP_API SYS_VISIBILITY_IMPORT
#endif

#endif
