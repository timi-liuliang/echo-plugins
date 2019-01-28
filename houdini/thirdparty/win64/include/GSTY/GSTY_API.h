
#ifndef __GSTY_API_h__
#define __GSTY_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef GSTY_EXPORTS
#define GSTY_API SYS_VISIBILITY_EXPORT
#else
#define GSTY_API SYS_VISIBILITY_IMPORT
#endif

#endif
