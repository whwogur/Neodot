#pragma once 

#define ND_INTERNAL_CAT(x,y) x ## y  
#define ND_CAT(x,y) ND_INTERNAL_CAT(x,y)  
#define ND_INTERNAL_STR(x) #x  
#define ND_STR(x) ND_INTERNAL_STR(x)  
#define ND_WSTR(x) ND_CAT(L,ND_STR(x))

#define ND_INTERNAL_NUM_ARGS_(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, TOTAL, ...) TOTAL 
#define ND_NUM_ARGS(...) ND_INTERNAL_NUM_ARGS_(__VA_ARGS__,12_,11_,10_,9_,8_,7_,6_,5_,4_,3_,2_,1_) 
#define ND_DISPATCH_VA(macro, ...) ND_CAT(macro,ND_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__) 