
#ifndef __DEBUG_H__
#define __DEBUG_H__
#include <stdio.h>
#if Android
#include <utils/Log.h>
#endif
#define LOG_COLOR_END "\033[0m"
#define LOG_FONT_GREEN "\033[32m"
#define LOG_FONT_PURULE "\033[35m"

#define LOG_FONT_BLUE "\033[34m"
#define LOG_FRONT_RED "\033[31m"
#define LOG_FONT_YELLOW "\033[33m"
#if Android
#if DEBUG_LOG
#if AUTO_TEST_LOG
#define OCEANUS_TAG "Oceanus"
#else
#define OCEANUS_TAG "Oceanus"
#endif
#define _string(x) #x
#define __string(x) _string(x)

#define OCEANUS_LOG(type,tag,file,line) __android_log_print(type,tag,_string(file:[line]))
#define OCEANUS_LOGV(...) OCEANUS_LOG(ANDROID_LOG_VERBOSE,OCEANUS_TAG,__FILE__,__LINE__);\
						  __android_log_print(ANDROID_LOG_VERBOSE,OCEANUS_TAG,__VA_ARGS__)
#define OCEANUS_LOGD(...) OCEANUS_LOG(ANDROID_LOG_DEBUG,OCEANUS_TAG,__FILE__,__LINE__);\
						  __android_log_print(ANDROID_LOG_DEBUG,OCEANUS_TAG,__VA_ARGS__)
#define OCEANUS_LOGI(...) OCEANUS_LOG(ANDROID_LOG_INFO,OCEANUS_TAG,__FILE__,__LINE__);\
						  __android_log_print(ANDROID_LOG_INFO,OCEANUS_TAG,__VA_ARGS__)
#define OCEANUS_LOGW(...) OCEANUS_LOG(ANDROID_LOG_WARN,OCEANUS_TAG,__FILE__,__LINE__);\
						  __android_log_print(ANDROID_LOG_WARN,OCEANUS_TAG,__VA_ARGS__)
#define OCEANUS_LOGE(...) OCEANUS_LOG(ANDROID_LOG_ERROR,OCEANUS_TAG,__FILE__,__LINE__);\
						  __android_log_print(ANDROID_LOG_ERROR,OCEANUS_TAG,__VA_ARGS__)

#else
#define OCEANUS_LOGV(fmt,arg...) do{}while(0)
#define OCEANUS_LOGD(fmt,arg...) do{}while(0)
#define OCEANUS_LOGI(fmt,arg...) do{}while(0)
#define OCEANUS_LOGW(fmt,arg...) do{}while(0)
#define OCEANUS_LOGE(fmt,arg...) __android_log_print(ANDROID_LOG_ERROR,OCEANUS_TAG,__VA_ARGS__)
#endif
#else//Linux
#if DEBUG_LOG
#if AUTO_TEST_LOG
	#define OCLOG(level, fmt, arg...) printf("\n [Oceanus_Test][%s]%s::%s:[%d]", level, __FILE__, __FUNCTION__,__LINE__);\
 								  	  printf(fmt, ##arg); \
 								   	  printf(LOG_COLOR_END"\n")
#else
 	#define OCLOG(level, fmt, arg...) printf("\n"); \
 								   	  printf(fmt, ##arg); \
 									  printf(LOG_COLOR_END"\n")				   
#endif

 #define OCEANUS_LOGV(fmt,arg...) OCLOG(LOG_FONT_PURULE"Verbose",fmt,##arg)
 #define OCEANUS_LOGD(fmt,arg...) OCLOG(LOG_FONT_GREEN"Debug",fmt,##arg)
 #define OCEANUS_LOGI(fmt,arg...) OCLOG(LOG_FONT_BLUE"Info",fmt,##arg)
 #define OCEANUS_LOGW(fmt,arg...) OCLOG(LOG_FONT_YELLOW"Warning",fmt,##arg)
 #define OCEANUS_LOGE(fmt,arg...) OCLOG(LOG_FRONT_RED"Error",fmt,##arg)
#else
	#define OCLOG(level, fmt, arg...) 
 	#define OCEANUS_LOGV(fmt,arg...) do{}while(0)
 	#define OCEANUS_LOGD(fmt,arg...) do{}while(0)
    #define OCEANUS_LOGI(fmt,arg...) do{}while(0)
 	#define OCEANUS_LOGW(fmt,arg...) do{}while(0)
 	#define OCEANUS_LOGE(fmt,arg...) printf(LOG_FRONT_RED"\n[Error]"); \
									 printf("%s::%s[%d]",__FILE__,__FUNCTION__,__LINE__); \
 								   	 printf(fmt, ##arg); \
 									 printf(LOG_COLOR_END"\n")	
#endif
#endif
#endif //__DEBUG_H__
