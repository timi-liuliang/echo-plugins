
#ifndef __CE_API_h__
#define __CE_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef CE_EXPORTS
#define CE_API SYS_VISIBILITY_EXPORT
#else
#define CE_API SYS_VISIBILITY_IMPORT
#endif

#ifdef OPENCL_ENABLED

#define CE_ENABLED

#define __CL_ENABLE_EXCEPTIONS
#include "cl.hpp"

#endif

#endif
