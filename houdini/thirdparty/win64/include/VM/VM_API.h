
#ifndef __VM_API_h__
#define __VM_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef VM_EXPORTS
#define VM_API SYS_VISIBILITY_EXPORT
#else
#define VM_API SYS_VISIBILITY_IMPORT
#endif

#endif
