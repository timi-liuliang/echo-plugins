
#ifndef __DOP_API_h__
#define __DOP_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef DOP_EXPORTS
#define DOP_API SYS_VISIBILITY_EXPORT
#else
#define DOP_API SYS_VISIBILITY_IMPORT
#endif

#endif
