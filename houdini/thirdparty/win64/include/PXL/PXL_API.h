
#ifndef __PXL_API_h__
#define __PXL_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef PXL_EXPORTS
#define PXL_API SYS_VISIBILITY_EXPORT
#else
#define PXL_API SYS_VISIBILITY_IMPORT
#endif

#endif
