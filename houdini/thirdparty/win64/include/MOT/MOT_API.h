
#ifndef __MOT_API_h__
#define __MOT_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef MOT_EXPORTS
#define MOT_API SYS_VISIBILITY_EXPORT
#else
#define MOT_API SYS_VISIBILITY_IMPORT
#endif

#endif
