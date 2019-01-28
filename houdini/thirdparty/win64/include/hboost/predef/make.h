/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/
#include <hboost/predef/detail/test.h>

#ifndef HBOOST_PREDEF_MAKE_H
#define HBOOST_PREDEF_MAKE_H

/*
Shorthands for the common version number formats used by vendors...
*/

/*`
[heading `HBOOST_PREDEF_MAKE_..` macros]

These set of macros decompose common vendor version number
macros which are composed version, revision, and patch digits.
The naming convention indicates:

* The base of the specified version number. "`HBOOST_PREDEF_MAKE_0X`" for
  hexadecimal digits, and "`HBOOST_PREDEF_MAKE_10`" for decimal digits.
* The format of the vendor version number. Where "`V`" indicates the version digits,
  "`R`" indicates the revision digits, "`P`" indicates the patch digits, and "`0`"
  indicates an ignored digit.

Macros are:
*/
/*` `HBOOST_PREDEF_MAKE_0X_VRP(V)` */
#define HBOOST_PREDEF_MAKE_0X_VRP(V) HBOOST_VERSION_NUMBER((V&0xF00)>>8,(V&0xF0)>>4,(V&0xF))
/*` `HBOOST_PREDEF_MAKE_0X_VVRP(V)` */
#define HBOOST_PREDEF_MAKE_0X_VVRP(V) HBOOST_VERSION_NUMBER((V&0xFF00)>>8,(V&0xF0)>>4,(V&0xF))
/*` `HBOOST_PREDEF_MAKE_0X_VRPP(V)` */
#define HBOOST_PREDEF_MAKE_0X_VRPP(V) HBOOST_VERSION_NUMBER((V&0xF000)>>12,(V&0xF00)>>8,(V&0xFF))
/*` `HBOOST_PREDEF_MAKE_0X_VVRR(V)` */
#define HBOOST_PREDEF_MAKE_0X_VVRR(V) HBOOST_VERSION_NUMBER((V&0xFF00)>>8,(V&0xFF),0)
/*` `HBOOST_PREDEF_MAKE_0X_VRRPPPP(V)` */
#define HBOOST_PREDEF_MAKE_0X_VRRPPPP(V) HBOOST_VERSION_NUMBER((V&0xF000000)>>24,(V&0xFF0000)>>16,(V&0xFFFF))
/*` `HBOOST_PREDEF_MAKE_0X_VVRRP(V)` */
#define HBOOST_PREDEF_MAKE_0X_VVRRP(V) HBOOST_VERSION_NUMBER((V&0xFF000)>>12,(V&0xFF0)>>4,(V&0xF))
/*` `HBOOST_PREDEF_MAKE_0X_VRRPP000(V)` */
#define HBOOST_PREDEF_MAKE_0X_VRRPP000(V) HBOOST_VERSION_NUMBER((V&0xF0000000)>>28,(V&0xFF00000)>>20,(V&0xFF000)>>12)
/*` `HBOOST_PREDEF_MAKE_0X_VVRRPP(V)` */
#define HBOOST_PREDEF_MAKE_0X_VVRRPP(V) HBOOST_VERSION_NUMBER((V&0xFF0000)>>16,(V&0xFF00)>>8,(V&0xFF))
/*` `HBOOST_PREDEF_MAKE_10_VPPP(V)` */
#define HBOOST_PREDEF_MAKE_10_VPPP(V) HBOOST_VERSION_NUMBER(((V)/1000)%10,0,(V)%1000)
/*` `HBOOST_PREDEF_MAKE_10_VRP(V)` */
#define HBOOST_PREDEF_MAKE_10_VRP(V) HBOOST_VERSION_NUMBER(((V)/100)%10,((V)/10)%10,(V)%10)
/*` `HBOOST_PREDEF_MAKE_10_VRP000(V)` */
#define HBOOST_PREDEF_MAKE_10_VRP000(V) HBOOST_VERSION_NUMBER(((V)/100000)%10,((V)/10000)%10,((V)/1000)%10)
/*` `HBOOST_PREDEF_MAKE_10_VRPP(V)` */
#define HBOOST_PREDEF_MAKE_10_VRPP(V) HBOOST_VERSION_NUMBER(((V)/1000)%10,((V)/100)%10,(V)%100)
/*` `HBOOST_PREDEF_MAKE_10_VRR(V)` */
#define HBOOST_PREDEF_MAKE_10_VRR(V) HBOOST_VERSION_NUMBER(((V)/100)%10,(V)%100,0)
/*` `HBOOST_PREDEF_MAKE_10_VRRPP(V)` */
#define HBOOST_PREDEF_MAKE_10_VRRPP(V) HBOOST_VERSION_NUMBER(((V)/10000)%10,((V)/100)%100,(V)%100)
/*` `HBOOST_PREDEF_MAKE_10_VRR000(V)` */
#define HBOOST_PREDEF_MAKE_10_VRR000(V) HBOOST_VERSION_NUMBER(((V)/100000)%10,((V)/1000)%100,0)
/*` `HBOOST_PREDEF_MAKE_10_VV00(V)` */
#define HBOOST_PREDEF_MAKE_10_VV00(V) HBOOST_VERSION_NUMBER(((V)/100)%100,0,0)
/*` `HBOOST_PREDEF_MAKE_10_VVRR(V)` */
#define HBOOST_PREDEF_MAKE_10_VVRR(V) HBOOST_VERSION_NUMBER(((V)/100)%100,(V)%100,0)
/*` `HBOOST_PREDEF_MAKE_10_VVRRPP(V)` */
#define HBOOST_PREDEF_MAKE_10_VVRRPP(V) HBOOST_VERSION_NUMBER(((V)/10000)%100,((V)/100)%100,(V)%100)
/*` `HBOOST_PREDEF_MAKE_10_VVRR0PP00(V)` */
#define HBOOST_PREDEF_MAKE_10_VVRR0PP00(V) HBOOST_VERSION_NUMBER(((V)/10000000)%100,((V)/100000)%100,((V)/100)%100)
/*` `HBOOST_PREDEF_MAKE_10_VVRR0PPPP(V)` */
#define HBOOST_PREDEF_MAKE_10_VVRR0PPPP(V) HBOOST_VERSION_NUMBER(((V)/10000000)%100,((V)/100000)%100,(V)%10000)
/*` `HBOOST_PREDEF_MAKE_10_VVRR00PP00(V)` */
#define HBOOST_PREDEF_MAKE_10_VVRR00PP00(V) HBOOST_VERSION_NUMBER(((V)/100000000)%100,((V)/1000000)%100,((V)/100)%100)
/*`
[heading `HBOOST_PREDEF_MAKE_*..` date macros]

Date decomposition macros return a date in the relative to the 1970
Epoch date. If the month is not available, January 1st is used as the month and day.
If the day is not available, but the month is, the 1st of the month is used as the day.
*/
/*` `HBOOST_PREDEF_MAKE_DATE(Y,M,D)` */
#define HBOOST_PREDEF_MAKE_DATE(Y,M,D) HBOOST_VERSION_NUMBER((Y)%10000-1970,(M)%100,(D)%100)
/*` `HBOOST_PREDEF_MAKE_YYYYMMDD(V)` */
#define HBOOST_PREDEF_MAKE_YYYYMMDD(V) HBOOST_PREDEF_MAKE_DATE(((V)/10000)%10000,((V)/100)%100,(V)%100)
/*` `HBOOST_PREDEF_MAKE_YYYY(V)` */
#define HBOOST_PREDEF_MAKE_YYYY(V) HBOOST_PREDEF_MAKE_DATE(V,1,1)
/*` `HBOOST_PREDEF_MAKE_YYYYMM(V)` */
#define HBOOST_PREDEF_MAKE_YYYYMM(V) HBOOST_PREDEF_MAKE_DATE((V)/100,(V)%100,1)

#endif
