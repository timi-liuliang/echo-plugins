
#ifndef __MGR_API_h__
#define __MGR_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef MGR_EXPORTS
#define MGR_API SYS_VISIBILITY_EXPORT
#else
#define MGR_API SYS_VISIBILITY_IMPORT
#endif

#endif
