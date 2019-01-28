
#ifndef __GT_API_h__
#define __GT_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef GT_EXPORTS
#define GT_API SYS_VISIBILITY_EXPORT
#define GT_API_TINST SYS_VISIBILITY_EXPORT_TINST
#else
#define GT_API SYS_VISIBILITY_IMPORT
#define GT_API_TINST SYS_VISIBILITY_IMPORT_TINST
#endif

#endif
