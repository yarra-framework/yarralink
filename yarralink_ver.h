#ifndef YARRALINK_VER_h
#define YARRALINK_VER_h 1


#ifndef VER_VD
    #define VER_PTR(A) A
    #define VER_REF(A) A
#else
    #define VER_PTR(A) *A
    #define VER_REF(A) &A
#endif


#endif // YARRA_VER_h
