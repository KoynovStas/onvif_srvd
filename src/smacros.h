/*
 * smacros.h
 *
 *
 * version 1.0
 *
 *
 * Copyright (c) 2015, Koynov Stas - skojnov@yandex.ru
 *
 *
 * more details see https://github.com/KoynovStas/smacros
 *
 */

#ifndef SMACROS_HEADER
#define SMACROS_HEADER


#include <stdint.h>
#include <stddef.h>  //for offsetof





#ifdef  __cplusplus
        #define  EXTERN_PREFIX extern "C"
#else
        #define  EXTERN_PREFIX extern
#endif





#define  SET_BIT(reg, num_bit)   (reg |=  (1 << num_bit) )
#define  CLR_BIT(reg, num_bit)   (reg &= ~(1 << num_bit) )
#define  INV_BIT(reg, num_bit)   (reg ^=  (1 << num_bit) )



#define  SET_FLAG(reg, flag)   (reg |=  (flag) )
#define  CLR_FLAG(reg, flag)   (reg &= ~(flag) )
#define  INV_FLAG(reg, flag)   (reg ^=  (flag) )





//The count of elements in the array.
#define  COUNT_ELEMENTS(array) (sizeof(array) / sizeof(array[0]))





// For GCC 4.6 or higher, in C++ you can use a standard right static_assert(exp, msg) in *.c and in *.h files.
// For GCC 4.6 is required to add CFLAGS += -std="c++0x"
// Simple C (gcc) have not static_assert.
// A lot of variants, it is the most simple and intuitive
// It can be used in *.c and in *.h files. (macros that use function style can be used in *.c files only)
// This is not an array. It is the type of data !!!!
// Disadvantages: you can not be set msg to display the console when compiling
//
// Example:
//
//  STATIC_ASSERT( sizeof(char) == 1)  //good job
//  STATIC_ASSERT( sizeof(char) != 1)  //You will get a compilation error
//
#define  STATIC_ASSERT(EXPR) typedef char static_assert_failed[(EXPR) ? 1 : -1] __attribute__((unused))





// align must be a power of two.
// Alignment takes place upwards.
#define  ALIGNMENT(val, align)  ( ((val) + (align)) & ~( (align) - 1) )





// Macros to convert the time in microseconds
// t  have to have  struct timespec type
// TIME_IN_USEC2 is fast variant (have not mul)
#define  TIME_IN_USEC(t)     (uint32_t)(t.tv_usec + t.tv_sec * 1000000)
#define  TIME_IN_USEC2(t)    (uint32_t)(t.tv_usec + (t.tv_sec << 20) )





#define  FREE_AND_NULL(ptr_var)  ({                      \
                                     if(ptr_var)         \
                                     {                   \
                                         free(ptr_var);  \
                                         ptr_var = NULL; \
                                     }                   \
                                  })



#define  DELETE_AND_NULL(ptr_var) ({                        \
                                      if(ptr_var)           \
                                      {                     \
                                          delete (ptr_var); \
                                          ptr_var = NULL;   \
                                      }                     \
                                   })



#define  DELETE_ARRAY_AND_NULL(ptr_var) ({                           \
                                            if(ptr_var)              \
                                            {                        \
                                                delete [] (ptr_var); \
                                                ptr_var = NULL;      \
                                            }                        \
                                        })





#ifdef  DEBUG
        #include <stdio.h>

        #define  DEBUG_MSG(...)       printf(__VA_ARGS__)
        #define  FDEBUG_MSG(fp, ...) fprintf(fp, __VA_ARGS__)
#else
        #define  DEBUG_MSG(...)       ({})
        #define  FDEBUG_MSG(fp, ...)  ({})
#endif





// container_of - cast a member of a structure out to the containing structure
// ptr:    the pointer to the member.
// type:   the type of the container struct this is embedded in.
// member: the name of the member within the struct.
//
#define container_of( ptr, type, member ) \
    ({  (type *)( (const char *)ptr - offsetof(type, member) );  })





#endif //SMACROS_HEADER
