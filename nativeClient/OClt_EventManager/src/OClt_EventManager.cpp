/**************************************************************************
* Author: heji
* Date:2016-05-18
* Description: EventService
**************************************************************************/
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include "OClt_EventManager.h"
#include "Debug.h"

Mutex OClt_EventManager::m_FunctionLock;

sp<IEvent> OClt_EventManager::m_pEventService = NULL;
sp<OClt_EventManager::DeathNotifier> OClt_EventManager::m_pDeathNotifier = NULL;

OClt_EventManager::OClt_EventManager()
{
	OCEANUS_LOGI(" ");
	if(getEventService()!=NULL)
		{
			if(m_pDeathNotifier == NULL)
				{
					m_pDeathNotifier = new DeathNotifier();
				}
			AS_BINDER(m_pEventService)->linkToDeath(m_pDeathNotifier);
		}
}
OClt_EventManager::~OClt_EventManager()
{
	OCEANUS_LOGI(" ");
	m_pEventService = NULL;
    IPCThreadState::self()->flushCommands();
	
}


const sp<IEvent> & OClt_EventManager::getEventService()
{
    Mutex::Autolock _l(m_FunctionLock);
	if (m_pEventService.get() == 0)
		{
			sp < IServiceManager > sm = defaultServiceManager();
			sp < IBinder > binder;
			do {
				binder = sm->getService(String16("Event"));
				if (binder != 0)
					{
						OCEANUS_LOGI("OSrv_EventService get Event Service success\n");
						break;
					}
				OCEANUS_LOGE("OSrv_EventService client is inavailable\n");
				usleep(500000);
			} while (true);
			m_pEventService = interface_cast<IEvent>(binder);
		}
		OCEANUS_LOGV("Partition client getPartitionService	out.\n");
		return m_pEventService;
}

void OClt_EventManager::RegisterEventListener(U8 event,sp<IEventListener>& listener)
{
	if(m_pEventService!=0)
		{
			m_pEventService->RegisterEventListener(event,listener);
		}
}
void OClt_EventManager::UnregisterEventListener(U8 event,sp<IEventListener>& listener)
{
	if(m_pEventService!=0)
		{
			m_pEventService->UnregisterEventListener(event,listener);
		}
}

void OClt_EventManager::SendEvent(const sp<IEventInfo>& info,OString& listener)
{
	if(m_pEventService!=0)
		{
			info->setListenerName(listener);
			m_pEventService->SendEvent(info);
		}
}

void OClt_EventManager::SendBroadcast(const sp<IEventInfo>& info)
{
	if(m_pEventService!=0)
		{
			info->setMsgType(FALSE);
			m_pEventService->SendEvent(info);
		}
}
void OClt_EventManager::DisableSystemTimeEvent(U8 event)
{
	if(m_pEventService!=0)
		{
			m_pEventService->DisableSystemTimeEvent(event);
		}
}
void OClt_EventManager::EnableSystemTimeEvent(const sp<IEventInfo>& info,U32 time_usec)
{
	if(m_pEventService!=0)
		{
			m_pEventService->EnableSystemTimeEvent(info,time_usec);
		}
}

	
OClt_EventManager::DeathNotifier::~DeathNotifier()
{
    OCEANUS_LOGV("DeathNotifier::~DeathNotifier");
    Mutex::Autolock _l(m_FunctionLock);
    if(m_pEventService != NULL)
    {
		AS_BINDER(m_pEventService)->unlinkToDeath(this);
    }
}

void OClt_EventManager::DeathNotifier::binderDied(const wp<IBinder>& who)
{
    OCEANUS_LOGV("NativeEventService died!\n");
    Mutex::Autolock _l(m_FunctionLock);
    m_pEventService.clear();
}




