/**************************************************************************
* Copyright:sky
* Author: heji@skyworth.com
* Date:2016-05-18
* Description: Oceanus Event service
**************************************************************************/
#ifndef __OCEANUS_EVENT_SERVICE_H__
#define __OCEANUS_EVENT_SERVICE_H__
#include <utils/threads.h>
#include <utils/SortedVector.h>
#include <binder/BinderService.h> 
#include <IEvent.h>
#include <IEventListener.h>
using namespace std;

#define Osys_event_new(base,cb,arg) event_new((base),-1,EV_PERSIST|EV_WRITE,(cb),(arg))
#define Osys_event_add(event) event_add((event),0)
#define Otimer_event_new(base,cb,arg) event_new((base),-1,EV_TIMEOUT,(cb),(arg))
#define Otimer_event_add(event,tv) event_add((event),(tv))

typedef struct
{
	sp<IEventInfo> info;
	U8 type;
}CALL_BACK_ARG;
namespace Oceanus
{
	class EventLooper;
	class Event : public BnEvent,public Thread<Event>,public BinderService<Event>
	{
		friend class BinderService<Event>;
	private:
		Event();
		virtual ~Event();
		static const char* getServiceName()
			{
				return "Event"; 
			};
	public:
		static Event* getInstance();
		void RegisterEventListener(U8 event,const sp<IEventListener>& listener);
		void UnregisterEventListener(U8 event,const sp<IEventListener>& listener);
		void SendEvent(const sp<IEventInfo>& info);
		void SendEvent(const sp<IEventInfo>& info,OString& listener);
		void SendBroadcast(const sp<IEventInfo>& info);
		void EnableSystemTimeEvent(const sp<IEventInfo>& info,U32 time_usec);
		void DisableSystemTimeEvent(U8 event);
	private:
		void InitEvents();
		void EventDispatcher(void* arg);
		void defaultCallBackListener(CALL_BACK_ARG* arg);
		static void mainCallBackFunction(int sock,short event,void * arg);
	private:
		static EventLooper* m_pEventLooper;
		static ThreadPool* m_pEventThreadPool;
		static struct event_base* m_pOceanus_event_base;
		static struct event **m_pOcanus_System_event;
		static vector<sp<IEventListener> > *m_pLinstenerList;
		static Event* m_pThis;
		static vector<sp<IEventInfo> > m_vEventInfo;
		BOOL m_bEnableEventInfoQueen;
		U32 m_uEventCount;
		static pthread_cond_t m_pEventSendCond;
		static pthread_mutex_t m_pEventMutex;
		static pthread_mutex_t *m_pCbLinstenerMutex;

	private:
	    class DeathNotifier : public IBinder::DeathRecipient
	    {
		    public:
		        ~DeathNotifier();
		        virtual void binderDied(const wp<IBinder>& who);
			private:
				class GarbageCollection:public Thread<GarbageCollection>
					{
						public:
							GarbageCollection();
							virtual ~GarbageCollection(){};
							void Run(void* arg);
					};
	    };
	    static sp<DeathNotifier> m_pDeathNotifier;
		};
}




#endif //__OCEANUS_EVENT_SERVICE_H__

