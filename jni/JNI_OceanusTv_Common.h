/**************************************************************************
* Author: heji
* Date:2016-07-04
* Description: Java Native Interface for EventService 
**************************************************************************/
#ifndef __JNI_TV_COMMON_H__
#define __JNI_TV_COMMON_H__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h> 
#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h" 
#include "Types.h"
#include "Debug.h"

using namespace android;

#define _service_class_path(class) Oceanus/Tv/Service/class/class
#define class_service_path(x)  _service_class_path(x)

#define ONCEANUS_JNI_LOAD(NativeClient,JavaClass) 		static int register_JNI_OceanusTv_##NativeClient(JNIEnv *env)  \
														{  \
															return android::AndroidRuntime::registerNativeMethods(env,_stringify(class_service_path(JavaClass)), method_table, NELEM(method_table));  \
														} \
												extern "C" jint JNI_OnLoad(JavaVM *vm, void *reserved)  \
														{  \
															OCEANUS_LOGI("JNI ON LOAD!!!!!");\
															JNIEnv *env = NULL; \
															if (vm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK)\
																{  \
																	OCEANUS_LOGE("Error GetEnv\n");  \
																	return -1;  \
																}  \
															OCEANUS_LOGI("Get Env Success~!");\
															assert(env != NULL);  \
															if (register_JNI_OceanusTv_##NativeClient(env) < 0)\
																{  \
																	OCEANUS_LOGE("register JNI_Oceanus_Tv_%s error.\n",__stringify(NativeClient));  \
																	return -1;  \
																}  \
															OCEANUS_LOGI("register Success~!");\
															return JNI_VERSION_1_4;\
														}

#define OCEANUS_JNI_DEFINITIONS(NativeClient)  static sp<NativeClient> m_p##NativeClient = NULL; \
												void onCreate(JNIEnv *env, jobject thiz); \
												void onDestroy(); \
												void JNI_OceanusTv_##NativeClient##_ConnectNativeService(JNIEnv *env, jobject thiz); \
												void JNI_OceanusTv_##NativeClient##_DisconnectNativeService(JNIEnv *env, jobject thiz); \
												void JNI_OceanusTv_##NativeClient##_ConnectNativeService(JNIEnv *env, jobject thiz) \
													{ \
														m_p##NativeClient = new NativeClient();\
														if(m_p##NativeClient!=NULL) \
															{ \
																OCEANUS_LOGI("Connect to [%s] Success!",_stringify(NativeClient)); \
																onCreate(env,thiz); \
																ProcessState::self()->startThreadPool(); \
															} \
													}\
												void JNI_OceanusTv_##NativeClient##_DisconnectNativeService(JNIEnv *env, jobject thiz) \
													{ \
														if(m_p##NativeClient!=NULL) \
															{ \
																OCEANUS_LOGI("Disconnect to [%s] Success!",_stringify(NativeClient)); \
																onDestroy(); \
																m_p##NativeClient.clear(); \
															} \
													}
													
#define OCEANUS_JNI_DEFINITIONS_METHOD_TABLE(NativeClient) {"Connect","()V",(void *)JNI_OceanusTv_##NativeClient##_ConnectNativeService},\
															{"Disconnect","()V",(void *)JNI_OceanusTv_##NativeClient##_DisconnectNativeService}


#define _Function(Function) Function

#define OCEANUS_JNI_CALL_FUNCTION(NativeClient,Function) 	if(m_p##NativeClient!=NULL) \
																{ \
																	m_p##NativeClient->_Function(Function);\
																}\
															else \
																{ \
																	OCEANUS_LOGE("jni call %s::%s error~",__stringify(NativeClient),__stringify(Function));\
																}
#define OCEANUS_JNI_CALL_FUNCTION_BY_RESULT(NativeClient,Function,Result) 	if(m_p##NativeClient!=NULL) \
																			{ \
																				Result = m_p##NativeClient->_Function(Function);\
																			}\
																		else \
																			{ \
																				OCEANUS_LOGE("jni call %s::%s error~",__stringify(NativeClient),__stringify(Function));\
																			}

//Debug only
//#pragma message(_stringify(ONCEANUS_JNI_LOAD(NativeClient,JavaClass)))
//#pragma message(_stringify(OCEANUS_JNI_CALL_FUNCTION(NativeClient,SendEvent(1,2))))
//#pragma message(_stringify(OCEANUS_JNI_DEFINITIONS(NativeClient)))

//Debug only end
static jstring str2jstring(JNIEnv* env,const char* c_str);
static std::string jstring2str(JNIEnv* env, jstring jstr);

jstring str2jstring(JNIEnv* env,const char* c_str)
{
    //定义javaString类 strClass
    jclass strClass = (env)->FindClass("java/lang/String");
    //获取String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
    jmethodID ctorID = (env)->GetMethodID(strClass, "<init>","([BLjava/lang/String;)V");
    //建立byte数组
    jbyteArray bytes = (env)->NewByteArray(strlen(c_str));
    //将char* 转换为byte数组
    (env)->SetByteArrayRegion(bytes,0,strlen(c_str), (jbyte*)c_str);
    //设置String, 保存语言类型,用于byte数组转换至String时的参数
    jstring encoding = (env)->NewStringUTF("GB2312");
    //将byte数组转换为javaString,并输出
    jstring result = (jstring)(env)->NewObject(strClass,ctorID, bytes, encoding);
	env->DeleteLocalRef(bytes);
	env->DeleteLocalRef(encoding);
	env->DeleteLocalRef(strClass);
    return result;
}
 
 
OString jstring2str(JNIEnv* env, jstring jstr)
{  
    char*  rtn   =   NULL;   
    jclass  clsstring   =   env->FindClass("java/lang/String");  
    jstring  strencode   =   env->NewStringUTF("GB2312");  
    jmethodID  mid   =   env->GetMethodID(clsstring,   "getBytes",  "(Ljava/lang/String;)[B");  
    jbyteArray  barr=   (jbyteArray)env->CallObjectMethod(jstr,mid,strencode);   
    jsize  alen   =   env->GetArrayLength(barr);   
    jbyte*  ba   =   env->GetByteArrayElements(barr,JNI_FALSE);   
    if(alen > 0)  
    {  
        rtn = (char*)malloc(alen+1);
        memcpy(rtn,ba,alen);
        rtn[alen]=0;  
    }  
    env->ReleaseByteArrayElements(barr,ba,0);  
    OString stemp(rtn);
    free(rtn);
	env->DeleteLocalRef(clsstring);
	env->DeleteLocalRef(strencode);
    return stemp;  
} 
static void checkAndClearException(JNIEnv* env, const char* methodName)
{  
    if (env->ExceptionCheck())
		{  
	        OCEANUS_LOGE("An exception was thrown by callback '%s'.", methodName);  
	        env->ExceptionClear();  
	    }  
} 

#endif //__JNI_TV_COMMON_H__

