
#ifndef __STY_API_h__
#define __STY_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef STY_EXPORTS
#define STY_API SYS_VISIBILITY_EXPORT
#else
#define STY_API SYS_VISIBILITY_IMPORT
#endif

#endif
