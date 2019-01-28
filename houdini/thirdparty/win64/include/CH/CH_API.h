
#ifndef __CH_API_h__
#define __CH_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef CH_EXPORTS
#define CH_API SYS_VISIBILITY_EXPORT
#else
#define CH_API SYS_VISIBILITY_IMPORT
#endif

#endif
