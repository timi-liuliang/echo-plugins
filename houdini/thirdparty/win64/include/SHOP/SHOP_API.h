
#ifndef __SHOP_API_h__
#define __SHOP_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef SHOP_EXPORTS
#define SHOP_API SYS_VISIBILITY_EXPORT
#else
#define SHOP_API SYS_VISIBILITY_IMPORT
#endif

#endif
