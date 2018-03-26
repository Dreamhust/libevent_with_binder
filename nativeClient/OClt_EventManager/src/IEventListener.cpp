/**************************************************************************
* Author: heji
* Date:2015-12-18
* Description: system basic event listener interface for binder
**************************************************************************/
#include "IEventListener.h"
#include "Oceanus_Debug.h"
using namespace android;

enum
{
	EN_EVENTLISTENER_ONEVENT = IBinder::FIRST_CALL_TRANSACTION,
	EN_EVENTLISTENER_GETLISTENER_NAME,
};



class BpEventListener: public BpInterface<IEventListener> {
public:
    BpEventListener(const sp<IBinder>& impl):BpInterface<IEventListener> (impl){};
	void OnEvent(const sp<IEventInfo>& info)
		{
			Parcel data, reply;
		    data.writeInterfaceToken(IEventListener::getInterfaceDescriptor());
			data.writeStrongBinder(AS_BINDER(info));
		    remote()->transact(EN_EVENTLISTENER_ONEVENT, data, &reply);
		}
	OString getListenerName()
		{
			Parcel data, reply;
		    data.writeInterfaceToken(IEventListener::getInterfaceDescriptor());
		    remote()->transact(EN_EVENTLISTENER_GETLISTENER_NAME, data, &reply);
			return reply.readCString();
		}
};


IMPLEMENT_META_INTERFACE(EventListener, "Oceanus.os.EventListener");

status_t BnEventListener::onTransact(uint32_t code, const Parcel& data,Parcel* reply, uint32_t flags)
{
	switch(code)
		{
			case EN_EVENTLISTENER_ONEVENT:
				{
					CHECK_INTERFACE(IEventListener,data,reply);
					sp<IEventInfo> info = interface_cast<IEventInfo>(data.readStrongBinder());
					OnEvent(info);
					return NO_ERROR;
				}
			break;
			case EN_EVENTLISTENER_GETLISTENER_NAME:
				{
					CHECK_INTERFACE(IEventListener,data,reply);
					reply->writeCString(getListenerName().c_str());
					return NO_ERROR;
				}
			break;
			default:
				{
					OCEANUS_LOGE("Receive unknown code(%08x)\n", code);
				}
			return BBinder::onTransact(code, data, reply, flags);
		}
}

















