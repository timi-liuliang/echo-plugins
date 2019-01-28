
#ifndef __SIMZ_API_h__
#define __SIMZ_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef SIMZ_EXPORTS
#define SIMZ_API SYS_VISIBILITY_EXPORT
#else
#define SIMZ_API SYS_VISIBILITY_IMPORT
#endif

#endif
