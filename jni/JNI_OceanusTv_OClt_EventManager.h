/**************************************************************************
* Author: heji
* Date:2016-07-04
* Description: Java Native Interface for NativeEventClient 
**************************************************************************/
#ifndef __JNI_TV_EVENT_CLIENT_H__
#define __JNI_TV_EVENT_CLIENT_H__
#include "JNI_Common.h"
#include "OClt_EventManager.h"
#include <map>
typedef struct
{
	jobject listener;
	jclass listenerClass;
	jmethodID onEvent;
}ST_JNI_LISTENER_INFO;

class JNI_OceanusTv_EventListener:public EventListener
	{
		public:
			JNI_OceanusTv_EventListener(JNIEnv *env,jobject listener,OString& name);
			virtual ~JNI_OceanusTv_EventListener();
			jobject GetJavaObject();
		private:
			void OnEvent(const sp<IEventInfo>& info);
		private:
			Mutex m_onEventLock;
			ST_JNI_LISTENER_INFO m_stJniInfo;
	};
static map<OString,sp<IEventListener> > m_mListenerMap;
/*******************************************************interfaces********************************************/
void JNI_OceanusTv_OClt_EventManager_SendEvent(JNIEnv *env, jobject thiz, jobject info,jstring name);
void JNI_OceanusTv_OClt_EventManager_SendBroadcast(JNIEnv *env, jobject thiz, jobject info);
void JNI_OceanusTv_OClt_EventManager_RegisteEventListener(JNIEnv *env, jobject thiz, jobject listener,jstring name,jint event); 
void JNI_OceanusTv_OClt_EventManager_UnregisteEventListener(JNIEnv *env, jobject thiz,jstring name,jint event);
void JNI_OceanusTv_OClt_EventManager_DisableSystemTimeEvent(JNIEnv *env, jobject thiz,jint event);
void JNI_OceanusTv_OClt_EventManager_EnableSystemTimeEvent(JNIEnv *env, jobject thiz,jobject info,jint time_usec);

/*******************************************************interfaces********************************************/

OCEANUS_JNI_DEFINITIONS(OClt_EventManager);

static JNINativeMethod method_table[] =
	{
		OCEANUS_JNI_DEFINITIONS_METHOD_TABLE(OClt_EventManager),
		{"SendEvent","(LOceanus/Tv/Service/EventManager/Tv_EventInfo;Ljava/lang/String;)V", (void *)JNI_OceanusTv_OClt_EventManager_SendEvent},
		{"SendBroadcast","(LOceanus/Tv/Service/EventManager/Tv_EventInfo;)V", (void *)JNI_OceanusTv_OClt_EventManager_SendBroadcast},
		{"RegisteEventListener", "(LOceanus/Tv/Service/EventManager/Tv_EventListener;Ljava/lang/String;I)V", (void *)JNI_OceanusTv_OClt_EventManager_RegisteEventListener},
		{"UnregisteEventListener", "(Ljava/lang/String;I)V", (void *)JNI_OceanusTv_OClt_EventManager_UnregisteEventListener},
		{"DisableSystemTimeEvent", "(I)V", (void *)JNI_OceanusTv_OClt_EventManager_DisableSystemTimeEvent},
		{"EnableSystemTimeEvent", "(LOceanus/Tv/Service/EventManager/Tv_EventInfo;I)V", (void *)JNI_OceanusTv_OClt_EventManager_EnableSystemTimeEvent},
	};
ONCEANUS_JNI_LOAD(OClt_EventManager,EventManager);



#endif//__JNI_OCEANUS_TV_EVENT_CLIENT_H__

