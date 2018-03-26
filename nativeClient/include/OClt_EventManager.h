/**************************************************************************
* Author: heji
* Date:2016-05-18
* Description: Oceanus EventService
**************************************************************************/
#ifndef __OCLT_EVENT_MANAGER_H__
#define __OCLT_EVENT_MANAGER_H__

#include <utils/threads.h>
#include "EventListener.h"
#include "IEvent.h"

#include "Types.h"
using namespace android; 
class OClt_EventManager : public RefBase
{
	public:
		OClt_EventManager();
		virtual ~OClt_EventManager();
		void RegisterEventListener(U8 event,sp<IEventListener>& listener);
		void UnregisterEventListener(U8 event,sp<IEventListener>& listener);
		void SendEvent(const sp<IEventInfo>& info,OString& listener);
		void SendBroadcast(const sp<IEventInfo>& info);
		void DisableSystemTimeEvent(U8 event);
		void EnableSystemTimeEvent(const sp<IEventInfo>& info,U32 time_usec);
		
	private:
		
		static Mutex m_FunctionLock;
		static const sp<IEvent> & getEventService();
		static sp<IEvent> m_pEventService;
		class DeathNotifier : public IBinder::DeathRecipient
		    {
			    public:
			        ~DeathNotifier();
			        virtual void binderDied(const wp<IBinder>& who);
		    };
    	static sp<DeathNotifier> m_pDeathNotifier;
};

#endif //__OCLT_EVENT_MANAGER_H__


