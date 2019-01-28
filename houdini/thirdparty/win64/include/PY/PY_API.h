
#ifndef __PY_API_h__
#define __PY_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef PY_EXPORTS
#define PY_API SYS_VISIBILITY_EXPORT
#else
#define PY_API SYS_VISIBILITY_IMPORT
#endif

#endif
