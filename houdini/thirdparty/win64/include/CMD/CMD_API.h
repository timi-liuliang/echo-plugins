
#ifndef __CMD_API_h__
#define __CMD_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef CMD_EXPORTS
#define CMD_API SYS_VISIBILITY_EXPORT
#else
#define CMD_API SYS_VISIBILITY_IMPORT
#endif

#endif
