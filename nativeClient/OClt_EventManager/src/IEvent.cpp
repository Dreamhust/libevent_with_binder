/**************************************************************************
* Author: heji
* Date:2016-05-20
* Description: Event Interface
**************************************************************************/
#include "IEvent.h"
#include "Debug.h"
using namespace android;
using namespace Oceanus;


enum
{
    EN_EVENT_REG_EVENT_LISTENER = IBinder::FIRST_CALL_TRANSACTION,
    EN_EVENT_UNREG_EVENT_LISTENER,
    EN_EVENT_SEND_EVENT,
    EN_EVENT_SEND_BORADCAST,
    EN_EVENT_ENABLE_TIME_EVENT,
    EN_EVENT_DISABLE_TIME_EVENT
};

class BpEvent: public BpInterface<IEvent> {
public:
    BpEvent(const sp<IBinder>& impl):BpInterface<IEvent> (impl){};
	void RegisterEventListener(U8 event,const sp<IEventListener>& listener)
		{
			Parcel data, reply;
		    data.writeInterfaceToken(IEvent::getInterfaceDescriptor());
			data.writeInt32(event);
			data.writeStrongBinder(AS_BINDER(listener));
		    remote()->transact(EN_EVENT_REG_EVENT_LISTENER, data, &reply);
		}
	void UnregisterEventListener(U8 event,const sp<IEventListener>& listener)
		{
			Parcel data, reply;
		    data.writeInterfaceToken(IEvent::getInterfaceDescriptor());
			data.writeInt32(event);
			data.writeStrongBinder(AS_BINDER(listener));
		    remote()->transact(EN_EVENT_UNREG_EVENT_LISTENER, data, &reply);
		}
	void SendEvent(const sp<IEventInfo>& info)
		{
			Parcel data, reply;
		    data.writeInterfaceToken(IEvent::getInterfaceDescriptor());
			data.writeStrongBinder(AS_BINDER(info));
		    remote()->transact(EN_EVENT_SEND_EVENT, data, &reply);
		}
	void EnableSystemTimeEvent(const sp<IEventInfo>& info,U32 time_usec)
		{
			Parcel data, reply;
		    data.writeInterfaceToken(IEvent::getInterfaceDescriptor());
			data.writeStrongBinder(AS_BINDER(info));
			data.writeInt32(time_usec);
		    remote()->transact(EN_EVENT_ENABLE_TIME_EVENT, data, &reply);
		}
	
	void DisableSystemTimeEvent(U8 event)
		{
			Parcel data, reply;
		    data.writeInterfaceToken(IEvent::getInterfaceDescriptor());
			data.writeInt32(event);
		    remote()->transact(EN_EVENT_DISABLE_TIME_EVENT, data, &reply);
		}
};


IMPLEMENT_META_INTERFACE(Event, "Oceanus.os.EventDispatcher");

status_t BnEvent::onTransact(uint32_t code, const Parcel& data,Parcel* reply, uint32_t flags)
{
	switch(code)
		{
			case EN_EVENT_REG_EVENT_LISTENER:
				{
					CHECK_INTERFACE(IEvent,data,reply);
					U8 event = (U8)data.readInt32();
					sp<IEventListener> listener = interface_cast<IEventListener>(data.readStrongBinder());
					RegisterEventListener(event,listener);
					return NO_ERROR;
				}
			break;
			case EN_EVENT_UNREG_EVENT_LISTENER:
				{
					CHECK_INTERFACE(IEvent,data,reply);
					U8 event = (U8)data.readInt32();
					sp<IEventListener> listener = interface_cast<IEventListener>(data.readStrongBinder());  
					UnregisterEventListener(event,listener);
					return NO_ERROR;
				}
			break;
			case EN_EVENT_SEND_EVENT:
				{
					CHECK_INTERFACE(IEvent,data,reply);
					sp<IEventInfo> info = interface_cast<IEventInfo>(data.readStrongBinder());
					IEventInfo* _info = info.get();//to break out cycle refance
					sp<EventInfo> pInfo = new EventInfo(_info);//pass new coyp of info to event service
					SendEvent(pInfo);
					info.clear();
					return NO_ERROR;
				}
			break;
			case EN_EVENT_ENABLE_TIME_EVENT:
				{
					CHECK_INTERFACE(IEvent,data,reply);
					sp<IEventInfo> info = interface_cast<IEventInfo>(data.readStrongBinder());
					IEventInfo* _info = info.get();//to break out cycle refance
					sp<EventInfo> pInfo = new EventInfo(_info);//pass new coyp of info to event service
					int32_t time = data.readInt32();
					EnableSystemTimeEvent(pInfo,time);
					info.clear();
					return NO_ERROR;
				}
			case EN_EVENT_DISABLE_TIME_EVENT:
				{
					CHECK_INTERFACE(IEvent,data,reply);
					U8 event = (U8)data.readInt32();
					DisableSystemTimeEvent(event);
					return NO_ERROR;
				}
			default:
				{
					OCEANUS_LOGE("Receive unknown code(%08x)\n", code);
				}
			return BBinder::onTransact(code, data, reply, flags);
		}
}

