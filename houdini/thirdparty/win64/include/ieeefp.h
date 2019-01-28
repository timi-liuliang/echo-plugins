/* Dummy includy file for NT */
/* Just the empty file needed for Linux, which has isnan in mathcalls.h  */

#ifdef WIN32

    #include <float.h>
    #define isnanf(x) _isnan(x)

#endif

#ifdef MBSD

    #include <math.h>
    #define isnanf __inline_isnanf

#endif
