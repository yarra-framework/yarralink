#ifndef YARRALINK_VER_h
#define YARRALINK_VER_h 1


#ifdef VER_VB
    #define VER_PTR(A) A
    #define VER_REF(A) A
#else
    #define VER_PTR(A) *A
    #define VER_REF(A) &A
#endif


#endif // YARRA_VER_h
