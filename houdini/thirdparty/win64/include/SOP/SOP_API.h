
#ifndef __SOP_API_h__
#define __SOP_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef SOP_EXPORTS
#define SOP_API SYS_VISIBILITY_EXPORT
#else
#define SOP_API SYS_VISIBILITY_IMPORT
#endif

#endif
