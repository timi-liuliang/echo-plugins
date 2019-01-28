
#ifndef __VOP_API_h__
#define __VOP_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef VOP_EXPORTS
#define VOP_API SYS_VISIBILITY_EXPORT
#else
#define VOP_API SYS_VISIBILITY_IMPORT
#endif

#endif
