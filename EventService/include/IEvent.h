/**************************************************************************
* Copyright:sky
* Author: heji@skyworth.com
* Date:2016-05-18
* Description: Oceanus Event Interface DECLARE
**************************************************************************/
#ifndef __IOCEANUS_EVENT_INTERFACE_H__
#define __IOCEANUS_EVENT_INTERFACE_H__
#include <binder/Parcel.h>
#include <binder/IPCThreadState.h>
#include <binder/IInterface.h>
#include "IEventListener.h"
#include "EventInfo.h"

#include <ThreadPool.h>
#include "Oceanus_Types.h"
#include <vector>
#include <algorithm>
#include "event.h"
#include "event2/thread.h"

using namespace android;
class IEvent : public IInterface
{
	public:
		DECLARE_META_INTERFACE(Event);
		virtual void RegisterEventListener(uint8_t event,const sp<IEventListener>& listener) = 0;
		virtual void UnregisterEventListener(uint8_t event,const sp<IEventListener>& listener) = 0;
		virtual void SendEvent(const sp<IEventInfo>& info) = 0;
		virtual void DisableSystemTimeEvent(uint8_t event) = 0;
		virtual void EnableSystemTimeEvent(const sp<IEventInfo>& info,uint32_t time_usec) = 0;
	private:
};

class BnEvent: public BnInterface<IEvent>
{
public:
 virtual status_t onTransact(uint32_t code, const Parcel& data,Parcel* reply, uint32_t flags= 0 );
};


#endif //__IOCEANUS_EVENT_INTERFACE_H__
