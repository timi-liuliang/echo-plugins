/* Dummy includy file for NT */

#ifdef WIN32
    #include <io.h>

    #define LOCK_UN 1
    #define LOCK_EX 2
    #define LOCK_NB 4
    #define LOCK_SH 8
    #define flock( a, b ) (1)
#endif
