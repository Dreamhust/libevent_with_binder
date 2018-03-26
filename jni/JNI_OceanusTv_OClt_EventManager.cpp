/**************************************************************************
* Author: heji
* Date:2016-07-04
* Description: Java Native Interface for EventService 
**************************************************************************/
#include <utils/Log.h>
#include "JNI_OceanusTv_OClt_EventManager.h"
#define DEBUG 0
#if DEBUG
#define JNI_EVENT_DEBUG(fmt, arg...) OCEANUS_LOGD(fmt,##arg)
#else
#define JNI_EVENT_DEBUG(fmt, arg...) do{}while(0)
#endif

jobject CreateTv_EventInfo(JNIEnv *env,jclass infoClass,jint type);
jobject CreateTv_EventInfo(JNIEnv *env,jclass infoClass,jint type,jstring info,jboolean bIsJson);
jobject CreateTv_EventInfo(JNIEnv *env,jclass infoClass,jint type,jlong info);

inline jobject CreateTv_EventInfo(JNIEnv *env,jclass infoClass,jint type)
{
	jmethodID Tv_EventInfo = env->GetMethodID(infoClass,"<init>","(I)V");//构造函数ID
	if(Tv_EventInfo == NULL)
		{
			OCEANUS_LOGE("Get Java Method: Tv_EventInfo Constructer Fialed");
	        if(env->ExceptionCheck() != 0)
				{
		            env->ExceptionDescribe();
		            env->ExceptionClear();
	        	}
			return NULL;
		}
	jobject EventInfo=env->NewObject(infoClass,Tv_EventInfo,type);
	return EventInfo;
}

inline jobject CreateTv_EventInfo(JNIEnv *env,jclass infoClass,jint type,jstring info,jboolean bIsJson)
{
	jmethodID Tv_EventInfo = env->GetMethodID(infoClass,"<init>","(ILjava/lang/String;Z)V");//构造函数ID
	if(Tv_EventInfo == NULL)
		{
			OCEANUS_LOGE("Get Java Method: Tv_EventInfo Constructer Fialed");
	        if(env->ExceptionCheck() != 0)
				{
		            env->ExceptionDescribe();
		            env->ExceptionClear();
	        	}
			return NULL;
		}
	jobject EventInfo=env->NewObject(infoClass,Tv_EventInfo,type,info,bIsJson);
	return EventInfo;
}

inline jobject CreateTv_EventInfo(JNIEnv *env,jclass infoClass,jint type,jlong info)
{
	jmethodID Tv_EventInfo = env->GetMethodID(infoClass,"<init>","(IJ)V");//构造函数ID
	if(Tv_EventInfo == NULL)
		{
			OCEANUS_LOGE("Get Java Method: Tv_EventInfo Constructer Fialed");
	        if(env->ExceptionCheck() != 0)
				{
		            env->ExceptionDescribe();
		            env->ExceptionClear();
	        	}
			return NULL;
		}
	jobject EventInfo=env->NewObject(infoClass,Tv_EventInfo,type,info);
	return EventInfo;
}
inline sp<IEventInfo> CreateNativeEventInfo(JNIEnv *env,jclass info_class,jobject info)
{
	sp<IEventInfo> m_pInfo = NULL;
	jmethodID Java_Info_getEventType = env->GetMethodID(info_class,"getEventType","()I");
	if(Java_Info_getEventType == NULL)
		{
			OCEANUS_LOGE("get Java jmethodID getEventType filed!");
			return NULL;
		}
	U8 EventType = env->CallIntMethod(info,Java_Info_getEventType);
	JNI_EVENT_DEBUG("==========EventType : %d",EventType);
	jmethodID Java_Info_getInfoType = env->GetMethodID(info_class,"getInfoTypeIndex","()I");
	if(Java_Info_getInfoType == NULL)
		{
			OCEANUS_LOGE("get Java jmethodID getInfoTypeIndex filed!");
			return NULL;
		}
	EN_EVENT_INFO_TYPE info_type = (EN_EVENT_INFO_TYPE)env->CallIntMethod(info,Java_Info_getInfoType);
	JNI_EVENT_DEBUG("==========info_type : %d",info_type);
	switch(info_type)
		{
			case E_EVENT_INFO_TYPE_NULL:
					{
						JNI_EVENT_DEBUG("E_EVENT_INFO_TYPE_NULL==========EventType : %d",EventType);
						m_pInfo = new EventInfo(EventType);
					}
				break;
			case E_EVENT_INFO_TYPE_STRING:
					{
						JNI_EVENT_DEBUG("E_EVENT_INFO_TYPE_STRING==========EventType : %d",EventType);
						jmethodID Java_Info_getString = env->GetMethodID(info_class,"getString","()Ljava/lang/String;");
						if(Java_Info_getString == NULL)
							{
								OCEANUS_LOGE("get Java jmethodID getString filed!");
								return NULL;
							}
						jstring Java_Info_String = (jstring)env->CallObjectMethod(info,Java_Info_getString);
						if(Java_Info_String == NULL)
							{
								OCEANUS_LOGE("call Java jmethodID getString filed!");
								return NULL;
							}
						OString Info_String = jstring2str(env,Java_Info_String);
						m_pInfo = new EventInfo(EventType,Info_String,FALSE);
					}
				break;
			case E_EVENT_INFO_TYPE_NUMBER:
					{
						JNI_EVENT_DEBUG("E_EVENT_INFO_TYPE_NUMBER==========EventType : %d",EventType);
						jmethodID Java_Info_getNumber = env->GetMethodID(info_class,"getInfoNumber","()J");
						if(Java_Info_getNumber == NULL)
							{
								OCEANUS_LOGE("get Java jmethodID getInfoNumber filed!");
								return NULL;
							}
						jlong Java_Info_Number = env->CallLongMethod(info,Java_Info_getNumber);
						m_pInfo = new EventInfo(EventType,(U32)Java_Info_Number);
					}
				break;
			case E_EVENT_INFO_TYPE_JSON:
					{
						JNI_EVENT_DEBUG("E_EVENT_INFO_TYPE_JSON==========EventType : %d",EventType);
						jmethodID Java_Info_getString = env->GetMethodID(info_class,"getString","()Ljava/lang/String;");
						if(Java_Info_getString == NULL)
							{
								OCEANUS_LOGE("get Java jmethodID get Json String filed!");
								return NULL;
							}
						jstring Java_Info_String = (jstring)env->CallObjectMethod(info,Java_Info_getString);
						if(Java_Info_String == NULL)
							{
								OCEANUS_LOGE("call Java jmethodID get Json String filed!");
								return NULL;
							}
						OString Info_String = jstring2str(env,Java_Info_String);
						JNI_EVENT_DEBUG("JNI JSON STRING:%s",Info_String.c_str());
						m_pInfo = new EventInfo(EventType,Info_String,TRUE);
					}
				break;
			default:
				break;
		}
	return m_pInfo;
}
void onCreate(JNIEnv * env,jobject thiz)
{
	JNI_EVENT_DEBUG(" onCreate JNI~");
	m_mListenerMap.clear();
}
void onDestroy()
{
	JNI_EVENT_DEBUG(" onDestroy JNI~");
	m_mListenerMap.clear();
}

JNI_OceanusTv_EventListener::JNI_OceanusTv_EventListener(JNIEnv *env, jobject listener,OString& name):EventListener(name)
	{
		JNI_EVENT_DEBUG("JNI_OceanusTv_EventListener Created:[%s] ",name.c_str());
		m_stJniInfo.listener = env->NewGlobalRef(listener);
		jclass listenerClass = env->GetObjectClass(m_stJniInfo.listener);
		if (listenerClass != NULL)
			{
				m_stJniInfo.listenerClass = (jclass)env->NewGlobalRef(listenerClass);
			}
		else
			{
				OCEANUS_LOGE("get Listener class error~!");
				m_stJniInfo.listenerClass = NULL;
			}
	}
JNI_OceanusTv_EventListener::~JNI_OceanusTv_EventListener()
	{
		JNI_EVENT_DEBUG("JNI_OceanusTv_EventListener Destroy:[%s] ",getListenerName().c_str());
		JNIEnv* env = AndroidRuntime::getJNIEnv();
		if(env != NULL)
			{
				if(m_stJniInfo.listenerClass!=NULL)
					{
						env->DeleteGlobalRef(m_stJniInfo.listenerClass);
						m_stJniInfo.listenerClass = NULL;
					}
				if(m_stJniInfo.listener!=NULL)
					{
						env->DeleteGlobalRef(m_stJniInfo.listener);
						m_stJniInfo.listener = NULL;
					}
			}
		else
			{
				OCEANUS_LOGE("get env in current thread error ~~~");
			}
	}
jobject JNI_OceanusTv_EventListener::GetJavaObject()
{
	return m_stJniInfo.listener;
}

void JNI_OceanusTv_EventListener::OnEvent(const sp < IEventInfo > & info)
	{
		Mutex::Autolock _l(m_onEventLock);
		OCEANUS_LOGV("OnEvent in JNI %p",this);
		JNIEnv* env = AndroidRuntime::getJNIEnv();
		if(env == NULL)
			{
				OCEANUS_LOGE("get env in current thread error ~~~");
				return;
			}
		if(m_stJniInfo.listenerClass == NULL)
			{
				OCEANUS_LOGE("get listenerClass in current EventListener error ~~~");
				return;
			}
		m_stJniInfo.onEvent = env->GetMethodID(m_stJniInfo.listenerClass,"onEvnet","(LOceanus/Tv/Service/EventManager/Tv_EventInfo;)V");
		if(m_stJniInfo.onEvent == NULL)
			{
				OCEANUS_LOGE("get Listener class 'function  onEvent error~!");
	         	return;
			}
		jclass Tv_EventInfo_Class = env->FindClass("Oceanus/Tv/Service/EventManager/Tv_EventInfo");
		if(Tv_EventInfo_Class == NULL)
			{
				OCEANUS_LOGE("Get Java Class: Oceanus/Tv/Service/EventManager/Tv_EventInfo Fialed");
				return;
			}
		jobject Java_Info = NULL;
		switch(info->getInfoType())
			{
				case E_EVENT_INFO_TYPE_NULL:
					{
						JNI_EVENT_DEBUG("CreateTv_EventInfo JAVA E_EVENT_INFO_TYPE_NULL");
						Java_Info = CreateTv_EventInfo(env,Tv_EventInfo_Class,(jint)info->getEventType());
					}
				break;
				case E_EVENT_INFO_TYPE_STRING:
					{
						JNI_EVENT_DEBUG("CreateTv_EventInfo JAVA E_EVENT_INFO_TYPE_STRING");
						jstring jsonstr = str2jstring(env,info->getString().c_str());
						Java_Info = CreateTv_EventInfo(env,Tv_EventInfo_Class,(jint)info->getEventType(),jsonstr,FALSE);
						env->DeleteLocalRef(jsonstr);
					}
				break;
				case E_EVENT_INFO_TYPE_NUMBER:
					{
						JNI_EVENT_DEBUG("CreateTv_EventInfo JAVA E_EVENT_INFO_TYPE_NUMBER");
						Java_Info = CreateTv_EventInfo(env,Tv_EventInfo_Class,(jint)info->getEventType(),(jlong)info->getU32Number());
					}
				break;
				case E_EVENT_INFO_TYPE_JSON:
					{
						JNI_EVENT_DEBUG("CreateTv_EventInfo JAVA E_EVENT_INFO_TYPE_JSON %s",info->getString().c_str());
						jstring jsonstr = str2jstring(env,info->getString().c_str());
						Java_Info = CreateTv_EventInfo(env,Tv_EventInfo_Class,(jint)info->getEventType(),jsonstr,TRUE);
						env->DeleteLocalRef(jsonstr);
					}
				break;
				default:
					OCEANUS_LOGE("Unknow event info type");
				break;
			}
		if(Java_Info != NULL)
			{
				JNI_EVENT_DEBUG("CALL JAVA onEvent");
				env->CallVoidMethod(m_stJniInfo.listener,m_stJniInfo.onEvent,Java_Info);
				env->DeleteLocalRef(Java_Info);
			}
		env->DeleteLocalRef(Tv_EventInfo_Class);
		usleep(50);
	}
void JNI_OceanusTv_OClt_EventManager_SendEvent(JNIEnv *env, jobject thiz, jobject info,jstring name)
{
	//OCEANUS_LOGV("JNI_OceanusTv_OSrv_EventService_SendEvent");
	OString m_sName = jstring2str(env,name);
	jclass Java_Info_Class = env->GetObjectClass(info);
	if(Java_Info_Class == NULL)
		{
			OCEANUS_LOGE("get Java Info Class filed!");
			return;
		}
	sp<IEventInfo> m_pInfo = CreateNativeEventInfo(env,Java_Info_Class,info);
	if(m_pInfo != NULL)
		{
			OCEANUS_JNI_CALL_FUNCTION(OClt_EventManager,SendEvent(m_pInfo,m_sName));
		}
	env->DeleteLocalRef(Java_Info_Class);
}
void JNI_OceanusTv_OClt_EventManager_SendBroadcast(JNIEnv *env, jobject thiz, jobject info)
{
	jclass Java_Info_Class = env->GetObjectClass(info);
	if(Java_Info_Class == NULL)
		{
			OCEANUS_LOGE("get Java Info Class filed!");
			return;
		}
	sp<IEventInfo> m_pInfo = CreateNativeEventInfo(env,Java_Info_Class,info);
	if(m_pInfo != NULL)
		{
			OCEANUS_JNI_CALL_FUNCTION(OClt_EventManager,SendBroadcast(m_pInfo));
		}
	env->DeleteLocalRef(Java_Info_Class);
}

void JNI_OceanusTv_OClt_EventManager_RegisteEventListener(JNIEnv *env, jobject thiz, jobject listener,jstring name ,jint event)
{
	OCEANUS_LOGV("JNI_OceanusTv_OSrv_EventService_RegisteEventListener");
	OString m_sName = jstring2str(env,name);
	sp<IEventListener> m_pListener = NULL;
	if(m_mListenerMap.empty())
		{
			m_pListener = new JNI_OceanusTv_EventListener(env,listener,m_sName);
			m_mListenerMap.insert(map<OString,sp<IEventListener> >::value_type(m_sName,m_pListener));
		}
	else
		{
			map<OString,sp<IEventListener> >::iterator l_it;
			l_it=m_mListenerMap.find(m_sName);
			if(l_it!=m_mListenerMap.end())
				{
					m_pListener = l_it->second;
					
					if(env->IsSameObject(((JNI_OceanusTv_EventListener*)(m_pListener.get()))->GetJavaObject(),listener) == JNI_TRUE)
						{
							JNI_EVENT_DEBUG("same object as same name~");
						}
					else
						{
							JNI_EVENT_DEBUG("dif object with same name is illegal~");
						}
					
				}
			else
				{
					m_pListener = new JNI_OceanusTv_EventListener(env,listener,m_sName);
					m_mListenerMap.insert(make_pair(m_sName,m_pListener));
				}
		}
	OCEANUS_JNI_CALL_FUNCTION(OClt_EventManager,RegisterEventListener((U8)event,m_pListener));
}

void JNI_OceanusTv_OClt_EventManager_UnregisteEventListener(JNIEnv *env, jobject thiz,jstring name,jint event)
{
	OCEANUS_LOGV("JNI_OceanusTv_OClt_EventManager_UnregisteEventListener");
	OString m_sName = jstring2str(env,name);
	map<OString,sp<IEventListener> >::iterator l_it;
	if(m_mListenerMap.empty())
		{
			return;
		}
	l_it=m_mListenerMap.find(m_sName);
	if(l_it!=m_mListenerMap.end())
		{
			OCEANUS_JNI_CALL_FUNCTION(OClt_EventManager,UnregisterEventListener((U8)event,l_it->second));
			l_it->second = NULL;
			m_mListenerMap.erase(l_it);
			JNI_EVENT_DEBUG("UnregisteEventListener %s Success!!",m_sName.c_str());
		}
	else
		{
			JNI_EVENT_DEBUG("%s is not registed~",m_sName.c_str());
		}
}
void JNI_OceanusTv_OClt_EventManager_DisableSystemTimeEvent(JNIEnv *env, jobject thiz,jint event)
{
	OCEANUS_JNI_CALL_FUNCTION(OClt_EventManager,DisableSystemTimeEvent((U8)event));
}
void JNI_OceanusTv_OClt_EventManager_EnableSystemTimeEvent(JNIEnv *env, jobject thiz,jobject info,jint time_usec)
{
	U32 time = (U32)time_usec;
	jclass Java_Info_Class = env->GetObjectClass(info);
	if(Java_Info_Class == NULL)
		{
			OCEANUS_LOGE("get Java Info Class filed!");
			return;
		}
	sp<IEventInfo> m_pInfo = CreateNativeEventInfo(env,Java_Info_Class,info);
	if(m_pInfo != NULL)
		{
			OCEANUS_JNI_CALL_FUNCTION(OClt_EventManager,EnableSystemTimeEvent(m_pInfo,time));
		}
	env->DeleteLocalRef(Java_Info_Class);
}



