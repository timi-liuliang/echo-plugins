
#ifndef __OP3D_API_h__
#define __OP3D_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef OP3D_EXPORTS
#define OP3D_API SYS_VISIBILITY_EXPORT
#else
#define OP3D_API SYS_VISIBILITY_IMPORT
#endif

#endif
