
#ifndef __IMG_API_h__
#define __IMG_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef IMG_EXPORTS
#define IMG_API SYS_VISIBILITY_EXPORT
#else
#define IMG_API SYS_VISIBILITY_IMPORT
#endif

#endif
