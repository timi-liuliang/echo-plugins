
#ifndef __LOP_API_h__
#define __LOP_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef LOP_EXPORTS
#define LOP_API SYS_VISIBILITY_EXPORT
#else
#define LOP_API SYS_VISIBILITY_IMPORT
#endif

#endif
