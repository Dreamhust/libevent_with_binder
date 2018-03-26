/**************************************************************************
* Copyright:sky
* Author: heji@skyworth.com
* Date:2016-05-18
* Description: Oceanus Binder example service
**************************************************************************/
#include <Event.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include "OEvent.h"

#define DEBUG 0
#if DEBUG
#define EVENT_DEBUG(fmt, arg...) OCEANUS_LOGD(fmt,##arg)
#else
#define EVENT_DEBUG(fmt, arg...) do{}while(0)
#endif

namespace Oceanus{
//map< U32,sp<Event::DeathNotifier> > Event::m_pListenerDeathNotifierMap;
sp<Event::DeathNotifier> Event::m_pDeathNotifier;

struct event_base* Event::m_pOceanus_event_base = NULL;
struct event **Event::m_pOcanus_System_event = NULL;
vector< sp<IEventInfo> > Event::m_vEventInfo;
Event* Event::m_pThis = NULL;
pthread_mutex_t Event::m_pEventMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t m_pDeathMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Event::m_pEventSendCond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t *Event::m_pCbLinstenerMutex = NULL;
vector<sp<IEventListener> > *Event::m_pLinstenerList = NULL;
ThreadPool *Event::m_pEventThreadPool = NULL;
EventLooper* Event::m_pEventLooper = NULL;
class EventLooper:public Oceanus::Thread<EventLooper>
{
	public:
		EventLooper(struct event_base* base,ThreadPool* threadpool):Thread<EventLooper>(this),m_pBase(base),m_pThreadPool(threadpool)
			{
				SetProcesser(&EventLooper::Loop,NULL);
			};
		void Start()
			{
				EVENT_DEBUG("Start EventLooper~");
				m_pThreadPool->AddTask(this);
			}
		void Stop()
			{
				EVENT_DEBUG("Stop EventLooper~");
				event_base_loopbreak(m_pBase);
			}
	private:
		void Loop(void* arg)
			{
				EVENT_DEBUG("loop~");
				event_base_dispatch(m_pBase);
			}
	private:
		struct event_base* m_pBase;
		ThreadPool* m_pThreadPool;		
};
class PostEvent:public Oceanus::Thread<PostEvent>
{
public:
	PostEvent(sp<IEventListener>& listener,sp<IEventInfo>& info):Thread<PostEvent>(this)
			{
				m_pListeneer = listener;
				m_pEventInfo = info;
			}
	virtual ~PostEvent()
		{
			m_pListeneer = NULL;
			m_pEventInfo = NULL;
		};
private:
	void Run(void * arg) override
		{
			pthread_mutex_lock(&m_pDeathMutex);
			if(AS_BINDER(m_pListeneer)->pingBinder() != DEAD_OBJECT)
				{
					m_pListeneer->OnEvent(m_pEventInfo);
				}
			else
				{
					OCEANUS_LOGW("########################################################");
					OCEANUS_LOGW("listener's processer hais dead give up to call: onEvent!");
					OCEANUS_LOGW("########################################################");
				}
			pthread_mutex_unlock(&m_pDeathMutex);
			delete this;
		}
private:
	sp<IEventListener> m_pListeneer;
	sp<IEventInfo> m_pEventInfo;
};

Event* Event::getInstance()
{
	if(m_pThis!=NULL)
		{
			return m_pThis;
		}
	return NULL;
}
Event::Event():Thread<Event>(this)
{
	m_pThis = this;
	m_bEnableEventInfoQueen = TRUE;
	m_uEventCount = 0;
	m_vEventInfo.clear();
	EVENT_DEBUG("-------------->>Oceanus_Event create![start]");
	m_pEventThreadPool = new (std::nothrow) ThreadPool(17);
	SetProcesser(&Event::EventDispatcher,NULL);
	m_pEventThreadPool->AddTask(this);
	evthread_use_pthreads();
	m_pOceanus_event_base = event_base_new();
	InitEvents();
	//m_pSocketListener = new (std::nothrow)Oceanus_SocketListener(this);
	m_pEventLooper = new (std::nothrow) EventLooper(m_pOceanus_event_base,m_pEventThreadPool);
	m_pEventLooper->Start();
	if(m_pDeathNotifier==NULL)
		{
			m_pDeathNotifier = new (std::nothrow) DeathNotifier();
		}
	//m_pListenerDeathNotifierMap.clear();
	EVENT_DEBUG("-------------->>Oceanus_Event create![down]");
}
Event::~Event()
{
	m_bEnableEventInfoQueen = FALSE;
	pthread_cond_signal(&m_pEventSendCond);
	printf("wait for event info queen exit.....\n");
	printf("free all event.....\n");
	for(int i=0;i<E_SYSTEM_EVENT_MAX;i++)
			{
				if(m_pOcanus_System_event[i]!=NULL)
					{
						printf("Oceanus_Event ===================del event\n");
						event_del(m_pOcanus_System_event[i]);
						event_free(m_pOcanus_System_event[i]);
					}
			}
	printf("stop dispacth event ....\n");
	m_pEventLooper->Stop();
	pthread_mutex_destroy(&m_pEventMutex);
	pthread_cond_destroy(&m_pEventSendCond);
	if(m_pCbLinstenerMutex!=NULL)
		{
			for(int i = 0;i<E_SYSTEM_EVENT_MAX;i++)
				{
					pthread_mutex_destroy(&m_pCbLinstenerMutex[i]);
				}
			delete m_pCbLinstenerMutex;
		}
	printf("wait for ThreadPool exit ....\n");
	m_pEventThreadPool->StopAll();
	printf("free all memory for event ....\n");
	if(!m_vEventInfo.empty())
		m_vEventInfo.clear();
	delete m_pOcanus_System_event;
	delete m_pThis;
	m_pOcanus_System_event = NULL;
	m_pThis = NULL;
	delete m_pEventLooper;
	m_pEventLooper = NULL;
	delete m_pEventThreadPool;
	m_pEventThreadPool = NULL;
}

void Event::EventDispatcher (void* arg)
{ 
	U8 ev_type = 0;
	while(1)
		{
			pthread_mutex_lock(&m_pEventMutex);
			while(m_vEventInfo.empty()&& (m_bEnableEventInfoQueen))
				{
					EVENT_DEBUG("onEventSendThread ===================>wait for Event!!\n");
					pthread_cond_wait(&m_pEventSendCond, &m_pEventMutex);
				}
			if(m_bEnableEventInfoQueen == FALSE)
				{
					pthread_mutex_unlock(&m_pEventMutex);
					EVENT_DEBUG("onEventSendThread ===================>EXIT!!\n");
					pthread_exit(NULL);
				}
			for(vector< sp<IEventInfo> >::iterator it=m_vEventInfo.begin(); it!=m_vEventInfo.end();it++)
				{
					ev_type = (*it)->getEventType();
					if(ev_type > m_uEventCount)
						{
							OCEANUS_LOGE("event [%d] not found!!\n",ev_type);
							it = m_vEventInfo.erase(it);
							continue;
						}
					if(m_pOcanus_System_event[ev_type]->ev_flags & EVLIST_ACTIVE)
						{
							EVENT_DEBUG("onEventSendThread ===================>[%d]is EVLIST_ACTIVE!!\n",ev_type);
							continue;
						}
					if(m_pOcanus_System_event[ev_type]->ev_arg != NULL)
						{
							EVENT_DEBUG("onEventSendThread ===================>[%d]is not NULL!!\n",ev_type);
							continue;
						}
					EVENT_DEBUG("onEventSendThread ===================>[%d]is active!![%d] events is processing!\n",ev_type,m_vEventInfo.size());
					CALL_BACK_ARG *arg = new (std::nothrow) CALL_BACK_ARG;
					arg->info = (*it);
					arg->type = ev_type;
					m_pOcanus_System_event[ev_type]->ev_arg = arg;// use struct!!!
					event_active(m_pOcanus_System_event[ev_type],m_pOcanus_System_event[ev_type]->ev_flags,1);
					(*it).clear();
					it = m_vEventInfo.erase(it);
					if(it == m_vEventInfo.end())
						break;
				}
			pthread_mutex_unlock(&m_pEventMutex);
		};
}


void Event::InitEvents()
{
	m_pLinstenerList = new (std::nothrow) vector<sp<IEventListener> > [E_SYSTEM_EVENT_MAX];
	m_pOcanus_System_event = new (std::nothrow) event* [E_SYSTEM_EVENT_MAX];
	m_pCbLinstenerMutex = new (std::nothrow) pthread_mutex_t[E_SYSTEM_EVENT_MAX];
	for(int i = 0 ;i<E_SYSTEM_EVENT_MAX;i++)
		{
			m_pLinstenerList[i].clear();
			pthread_mutex_init(&m_pCbLinstenerMutex[i], NULL);
			if((i>E_SYSTEM_EVENT_COUNT) && (i<=E_SYSTEM_EVENT_MAX))
				{//time_event
					m_pOcanus_System_event[i] = Otimer_event_new(m_pOceanus_event_base,mainCallBackFunction,NULL);
					OSRV_ALLOC_CHK(m_pOcanus_System_event[i]);
				}
			else
				{//normal event
					m_pOcanus_System_event[i] = Osys_event_new(m_pOceanus_event_base,mainCallBackFunction,NULL);
					OSRV_ALLOC_CHK(m_pOcanus_System_event[i]);
					Osys_event_add(m_pOcanus_System_event[i]);
				}
			m_uEventCount=i+1;
			EVENT_DEBUG("%d",m_uEventCount);
		}
	return;
	CREATE_ERR_EXIT:
		for(int i=0;i<E_SYSTEM_EVENT_MAX;i++)
			{
				if(m_pOcanus_System_event[i]!=NULL)
					{
						OCEANUS_LOGE("Event create error ===================del event[%d]\n",i);
						event_del(m_pOcanus_System_event[i]);
						event_free(m_pOcanus_System_event[i]);
					}
			}
		if(!m_vEventInfo.empty())
		m_vEventInfo.clear();
		delete m_pOcanus_System_event;
		m_pOcanus_System_event = NULL;
		return;
}

void Event::defaultCallBackListener(CALL_BACK_ARG* arg)
{
	OCEANUS_LOGI(" ");
	U8 ev = arg->type;
	sp<IEventInfo> info = arg->info;
	switch(ev)
		{
			case E_SYSTEM_EVENT_ADD_EVENT:
				{
					
				}
			break;
			/*
			case E_SYSTEM_EVENT_ADD_SOCKET_EVENT:
				{
					OCEANUS_LOGI("add new event");
					sleep(1);
					OCEANUS_SOCKET_EVENT* ev;
					U32 size = info->getObj((void**)(&ev));
					struct event* new_event = NULL;
					if(ev->type == EN_SOCKET_UDP_EVENT)
						{
							new_event = event_new(m_pOceanus_event_base,ev->fd,ev->flag,ev->cb.udp_ev_cb,(void*)new_event);
						}
					if(new_event!=NULL)
						{
							if(Osys_event_add(new_event)<0)
								{
									OCEANUS_LOGE("add event filed");
								}
						}
					
				}
			break;
			*/
			case E_SYSTEM_EVENT_REMOVE_EVENT:
				{
					OCEANUS_LOGI("remove event");
				}
			break;
			default: break;
		}
}

void Event::mainCallBackFunction(int sock, short event, void *arg)
{
	CALL_BACK_ARG *info_arg =  (CALL_BACK_ARG*)arg;
	OString name = info_arg->info->getListenerName();
	U8 ev = info_arg->type;
	if(ev == E_SYSTEM_EVENT_ADD_EVENT|| ev == E_SYSTEM_EVENT_ADD_SOCKET_EVENT)
		{
			m_pThis->defaultCallBackListener(info_arg);
			if(info_arg!=NULL)
				{
					//OCEANUS_LOGI("release arg~");
					info_arg->info.clear();
					info_arg->type = 0;
					delete info_arg;
					info_arg = NULL;
				}
			return;
		}
		int cb_num = m_pLinstenerList[ev].size();
		EVENT_DEBUG("event:[%d]has:%d listeners~~",ev,cb_num);
		if(cb_num != 0)
			{
				int i = 0;
				pthread_mutex_lock(&m_pCbLinstenerMutex[ev]);
				for(vector<sp<IEventListener> >::iterator it=m_pLinstenerList[ev].begin(); it!=m_pLinstenerList[ev].end(); ++it)
				{
					if(info_arg->info->getMsgType())
						{
							if(name != (*it)->getListenerName())
							{
								continue;
							}
						}
					PostEvent* event = new (std::nothrow) PostEvent((*it),info_arg->info);
					m_pEventThreadPool->AddTask(event);
					i++;
				}
				pthread_mutex_unlock(&m_pCbLinstenerMutex[ev]);
			}
		pthread_mutex_lock(&m_pEventMutex);
		m_pOcanus_System_event[info_arg->info->getEventType()]->ev_arg = NULL;
		pthread_mutex_unlock(&m_pEventMutex);
	if(info_arg!=NULL)
		{
			//EVENT_DEBUG("release arg~");
			info_arg->info.clear();
			info_arg->type = 0;
			delete info_arg;
			info_arg = NULL;
		}
	return;
}

void Event::EnableSystemTimeEvent(const sp<IEventInfo>& info,U32 time_usec)
	{
		if(info.get()!=NULL)
			{
				pthread_mutex_lock(&m_pEventMutex);
				U8 event = info->getEventType();
				if(info->getListenerName() == "")
					{
						info->setMsgType(FALSE);
					}
				if((event>E_SYSTEM_EVENT_COUNT) && (event<=E_SYSTEM_EVENT_MAX))
					{
						if(event_pending(m_pOcanus_System_event[event],m_pOcanus_System_event[event]->ev_flags,NULL))
							{
								OCEANUS_LOGE("enable error the event might already start or not exsit!");
								pthread_mutex_unlock(&m_pEventMutex);
								return;
							}
						struct timeval tv;
						tv.tv_sec = (long)time_usec/1000;
						tv.tv_usec = (long)time_usec%1000;
						CALL_BACK_ARG *arg = new (std::nothrow) CALL_BACK_ARG;
						arg->info = info;
						arg->type = event;
						m_pOcanus_System_event[event]->ev_arg = arg;
						Otimer_event_add(m_pOcanus_System_event[event],&tv);
						pthread_mutex_unlock(&m_pEventMutex);
					}
			}
		
	}
void Event::DisableSystemTimeEvent(U8 event)
{
	if((event>E_SYSTEM_EVENT_COUNT) && (event<=E_SYSTEM_EVENT_MAX))
		{
			pthread_mutex_lock(&m_pEventMutex);
			event_del(m_pOcanus_System_event[event]);
			pthread_mutex_unlock(&m_pEventMutex);
		}
}

void Event::RegisterEventListener(U8 event,const sp<IEventListener>& listener)
	{
		OCEANUS_LOGI("(callingPid %d)\n", IPCThreadState::self()->getCallingPid());
		if(event>m_uEventCount|| (listener.get()==NULL))
			{
				return;
			}
		if(listener.get()!=NULL)
			{
				pthread_mutex_lock(&m_pCbLinstenerMutex[event]);
				m_pLinstenerList[event].push_back(listener);
				AS_BINDER(listener)->linkToDeath(m_pDeathNotifier);
				pthread_mutex_unlock(&m_pCbLinstenerMutex[event]);
			}
		else
			{
				OCEANUS_LOGE(" register filed");
				return;
			}
	}
void Event::UnregisterEventListener(U8 event,const sp<IEventListener>& listener)
{
	if(event>m_uEventCount|| (listener==NULL))
		{
			return;
		}
	if(listener.get()!=NULL)
		{
			OString name = listener->getListenerName();
			pthread_mutex_lock(&m_pCbLinstenerMutex[event]);
			for(vector<sp<IEventListener> >::iterator iter = m_pLinstenerList[event].begin();iter!=m_pLinstenerList[event].end();iter++)
				{
					if(((*iter)->getListenerName() == name))
						{
							OCEANUS_LOGI("unregister [%s]",name.c_str());
							AS_BINDER(listener)->unlinkToDeath(m_pDeathNotifier);
							iter = m_pLinstenerList[event].erase(iter);
							if(iter == m_pLinstenerList[event].end())
								break;
						}
				}
			pthread_mutex_unlock(&m_pCbLinstenerMutex[event]);
		}
	else
		{
			OCEANUS_LOGE(" unregister filed");
			return;
		}
}
void Event::SendEvent(const sp<IEventInfo>& info)
{
	if(info.get()!=NULL)
		{
			pthread_mutex_lock(&m_pEventMutex);
			EVENT_DEBUG("SendEvent");
			m_vEventInfo.push_back(info);//add event info
			pthread_mutex_unlock(&m_pEventMutex);
			pthread_cond_signal(&m_pEventSendCond);
		}
	else
		{
			OCEANUS_LOGE("Send Error~~");
		}
}
void Event::SendEvent(const sp<IEventInfo>& info,OString& listener)
{
	if(info.get()!=NULL)
		{
			pthread_mutex_lock(&m_pEventMutex);
			EVENT_DEBUG("SendEvent PASS Listener");
			info->setListenerName(listener);
			m_vEventInfo.push_back(info);//add event info
			pthread_mutex_unlock(&m_pEventMutex);
			pthread_cond_signal(&m_pEventSendCond);
		}
	else
		{
			OCEANUS_LOGE("Send Error~~");
		}
}
void Event::SendBroadcast(const sp<IEventInfo>& info)
{
	if(info.get()!=NULL)
		{
			pthread_mutex_lock(&m_pEventMutex);
			info->setMsgType(FALSE);
			m_vEventInfo.push_back(info);//add event info
			pthread_mutex_unlock(&m_pEventMutex);
			pthread_cond_signal(&m_pEventSendCond);
		}
	else
		{
			OCEANUS_LOGE("Send Error~~");
		}
}

Event::DeathNotifier::~DeathNotifier()
{
	OCEANUS_LOGI(" ");
}

void Event::DeathNotifier::binderDied(const wp<IBinder>& who)
{
   m_pEventThreadPool->AddTask(new GarbageCollection());
}


Event::DeathNotifier::GarbageCollection::GarbageCollection():Thread<GarbageCollection>(this)
{
	OCEANUS_LOGE(" ");
}


void Event::DeathNotifier::GarbageCollection::Run(void * arg)
{
	OCEANUS_LOGE("####################################[GarbageCollection]#################################################");
	OCEANUS_LOGE(" This information shown because [there are some event listeners not unregiste], in this circumstance system will \
		auto collect them and will take much more time .so You'd better check your code out to solve this information!");
	pthread_mutex_lock(&m_pEventMutex);
	pthread_mutex_lock(&m_pDeathMutex);
	for(int i = 0 ;i<E_SYSTEM_EVENT_MAX;i++)
		{
			pthread_mutex_lock(&m_pCbLinstenerMutex[i]);
			 for(vector<sp<IEventListener> >::iterator iter = m_pLinstenerList[i].begin();iter!=m_pLinstenerList[i].end();iter++)
			   {
					if(AS_BINDER((*iter))->pingBinder() == DEAD_OBJECT)
						{
							OCEANUS_LOGE("####### DEAD OBJECT FOUND! ###########");
							AS_BINDER((*iter))->unlinkToDeath(m_pDeathNotifier);
							iter = m_pLinstenerList[i].erase(iter);
							OCEANUS_LOGE("####### DEAD OBJECT COOLECTED! ###########");
							if(iter == m_pLinstenerList[i].end())
								break;
						}
			   }  
			pthread_mutex_unlock(&m_pCbLinstenerMutex[i]);
		}
	pthread_mutex_unlock(&m_pEventMutex);
	pthread_mutex_unlock(&m_pDeathMutex);
	OCEANUS_LOGE("####################################[GarbageCollection]#################################################");
	delete this;
}

}
/*

*/


