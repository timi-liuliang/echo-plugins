
#ifndef __OP_API_h__
#define __OP_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef OP_EXPORTS
#define OP_API SYS_VISIBILITY_EXPORT
#else
#define OP_API SYS_VISIBILITY_IMPORT
#endif

#endif
