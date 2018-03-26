/*************************************************
* Date:2015-12-3
* Description:
**************************************************/
#ifndef __OCEANUS_TYPES_H__
#define __OCEANUS_TYPES_H__
#if 0//GOOGLE_4_8 == 1 
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
#endif
#include "Debug.h"
#include "Base.h"
#include <stdlib.h>
#include <cstring>
#include <errno.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <pthread.h>
#include <string>
#include <sstream>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "mxml.h"
#if Android
#if (Android_5_X == 1 || Android_4_X ==1 )
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#else
#include <sys/system_properties.h>
#endif
#endif

using namespace std;
using namespace rapidjson;
using std::string;

#define OString std::string

#if 0//AndroidSdkVersion<23 
#define AS_BINDER(IInterface) IInterface->asBinder()
#else
#define	AS_BINDER(IInterface) IInterface->asBinder(IInterface)
#endif
#define EMPTY_STR ""
#endif //__OCEANUS_TYPES_H__

