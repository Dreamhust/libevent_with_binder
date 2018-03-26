/**************************************************************************
* Author: heji
* Date:2015-12-18
* Description: oceanus system basic event listener
**************************************************************************/
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <EventListener.h>
#include "Debug.h"

EventListener::EventListener(OString name):m_sName(name)
{
	
};
EventListener:: ~EventListener()
{
	//OCEANUS_LOGI(" ");
};

OString EventListener::getListenerName()
{
	return m_sName;
}
void EventListener::OnEvent(const sp<IEventInfo>& info)
{
}

