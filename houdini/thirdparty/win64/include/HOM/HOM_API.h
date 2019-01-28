
#ifndef __HOM_API_h__
#define __HOM_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef SWIG
    #define HOM_API
#else
    #ifdef HOM_EXPORTS
    #define HOM_API SYS_VISIBILITY_EXPORT
    #else
    #define HOM_API SYS_VISIBILITY_IMPORT
    #endif
#endif

#endif
