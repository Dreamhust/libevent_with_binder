/*************************************************
* Date:2015-12-3
* Description:
**************************************************/
#ifndef __BASE_H__
#define __BASE_H__
#include <stdio.h>
#define U8 unsigned char
#define U16 unsigned short
#define U32 unsigned int
#define U64 unsigned long long
#define S8 signed char
#define S16 signed short
#define S32 signed int
#define S64 signed long long
#define BOOL unsigned char
#define TRUE 1
#define FALSE 0
#define ASSERT(_x_)                                                                         \
    do  {                                                                                   \
    if ( ! ( _x_ ) )                                                                    \
    {                                                                                   \
        printf("ASSERT FAIL: %s %s %d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);     \
        abort();                                                                        \
    }                                                                                   \
} while (0)
#define OSRV_ALLOC_CHK(x)           \
    {                                       \
        ASSERT((x) != NULL);                \
        if((x) == NULL)                     \
            goto CREATE_ERR_EXIT;           \
    }
#define PTH_RET_CHK(_pf_) \
    ({ \
        int r = _pf_; \
        if ((r != 0) && (r != ETIMEDOUT)) \
            printf("[PTHREAD] %s: %d: %s: %s\n", __FILE__, __LINE__, #_pf_, strerror(r)); \
        r; \
    })

#define  __stringify(x)   #x
#define _stringify(x) __stringify(x)
#ifdef OCEANUS_ROOT
#define OCEANUS_ROOT_PATH _stringify(OCEANUS_ROOT/Device/Common/)
#define OCEANUS_DATA_PATH _stringify(OCEANUS_ROOT/Device/Common/data/)
#else
#define OCEANUS_ROOT_PATH "/system/Oceanus/"
#define OCEANUS_DATA_PATH "/data/Oceanus/"
#endif
#define NS_EXPORT

extern "C" {
/*
 * To work around http://code.google.com/p/android/issues/detail?id=23203
 * we don't link with the crt objects. In some configurations, this means
 * a lack of the __dso_handle symbol because it is defined there, and
 * depending on the android platform and ndk versions used, it may or may
 * not be defined in libc.so. In the latter case, we fail to link. Defining
 * it here as weak makes us provide the symbol when it's not provided by
 * the crt objects, making the change transparent for future NDKs that
 * would fix the original problem. On older NDKs, it is not a problem
 * either because the way __dso_handle was used was already broken (and
 * the custom linker works around it).
 */
  NS_EXPORT __attribute__((weak)) void *__dso_handle;
}
#endif//__OCEANUS_BASE_H__
