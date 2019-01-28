
#ifndef __CHOP_API_h__
#define __CHOP_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef CHOP_EXPORTS
#define CHOP_API SYS_VISIBILITY_EXPORT
#else
#define CHOP_API SYS_VISIBILITY_IMPORT
#endif

#endif
