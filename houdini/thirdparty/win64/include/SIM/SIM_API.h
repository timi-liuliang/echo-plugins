
#ifndef __SIM_API_h__
#define __SIM_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef SIM_EXPORTS
#define SIM_API SYS_VISIBILITY_EXPORT
#else
#define SIM_API SYS_VISIBILITY_IMPORT
#endif

#endif
