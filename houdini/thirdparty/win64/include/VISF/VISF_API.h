
#ifndef __VISF_API_h__
#define __VISF_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef VISF_EXPORTS
#define VISF_API SYS_VISIBILITY_EXPORT
#else
#define VISF_API SYS_VISIBILITY_IMPORT
#endif

#endif
