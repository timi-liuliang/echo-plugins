
#ifndef __CV_API_h__
#define __CV_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef CV_EXPORTS
#define CV_API SYS_VISIBILITY_EXPORT
#else
#define CV_API SYS_VISIBILITY_IMPORT
#endif

#endif
