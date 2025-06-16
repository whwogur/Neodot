#pragma once 

#define ND_INTERNAL_CAT(x,y) x ## y  
#define ND_CAT(x,y) ND_INTERNAL_CAT(x,y)  
#define ND_INTERNAL_STR(x) #x  
#define ND_STR(x) ND_INTERNAL_STR(x)  
#define ND_WSTR(x) ND_CAT(L,ND_STR(x))