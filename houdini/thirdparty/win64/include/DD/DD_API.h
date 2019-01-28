
#ifndef __DD_API_h__
#define __DD_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef DD_EXPORTS
#define DD_API SYS_VISIBILITY_EXPORT
#else
#define DD_API SYS_VISIBILITY_IMPORT
#endif

#endif
