/* Dummy includy file for NT */

#ifdef WIN32
    #define PROT_READ   1
    #define PROT_WRITE  2
    #define MAP_PRIVATE 4
    #define mmap(a, b, c, d, e, f) (1)
#endif
