
#ifndef __RBD_API_h__
#define __RBD_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef RBD_EXPORTS
#define RBD_API SYS_VISIBILITY_EXPORT
#else
#define RBD_API SYS_VISIBILITY_IMPORT
#endif

#endif
