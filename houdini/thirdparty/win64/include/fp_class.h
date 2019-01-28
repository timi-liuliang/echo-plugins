/* Dummy includy file for NT and Linux */

#ifndef __fp_class_f__
#define __fp_class_f__

#ifdef WIN32
    #include <float.h>
    #define FP_SNAN _FPCLASS_SNAN
    #define FP_QNAN _FPCLASS_QNAN
    #define FP_NEG_INF _FPCLASS_NINF
    #define FP_NEG_NORM _FPCLASS_NN
    #define FP_NEG_DENORM _FPCLASS_ND
    #define FP_NEG_ZERO _FPCLASS_NZ
    #define FP_POS_ZERO _FPCLASS_PZ
    #define FP_POS_DENORM _FPCLASS_PD
    #define FP_POS_NORM _FPCLASS_PN
    #define FP_POS_INF _FPCLASS_PINF
    #define fp_class_f(f) _fpclass(f)
    #define fp_class_d(f) _fpclass(f)
#elif defined(LINUX) || defined(MBSD)
    #include <math.h>
    // FP_SNAN/FP_QNAN are defined by default on OSX to FP_NAN
    #undef FP_SNAN
    #define FP_SNAN		0
    #undef FP_QNAN
    #define FP_QNAN		1
    #define FP_POS_INF		2
    #define FP_NEG_INF 		3
    #define FP_POS_NORM		4
    #define FP_NEG_NORM		5
    #define FP_POS_DENORM	6
    #define FP_NEG_DENORM	7
    #define FP_POS_ZERO		8
    #define FP_NEG_ZERO		9
#endif

#endif
