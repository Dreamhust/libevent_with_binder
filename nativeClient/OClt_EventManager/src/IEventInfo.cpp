/**************************************************************************
* Author: heji
* Date:2015-12-18
* Description: system basic event listener interface for binder
**************************************************************************/
#include "IEventInfo.h"
#include "Debug.h"
using namespace android;
enum
{
	EN_EVENT_INFO_GET_LISTENER_NAME = IBinder::FIRST_CALL_TRANSACTION,
	EN_EVENT_INFO_SET_LISTENER_NAME,
	EN_EVENT_INFO_GET_STRING,
	EN_EVENT_INFO_GET_U32,
	EN_EVENT_INFO_GET_OBJECT,
	EN_EVENT_INFO_GET_INFO_TYPE,
	EN_EVENT_INFO_GET_EVENT_TYPE,
	EN_EVENT_INFO_SET_MESSAGE_TYPE,
	EN_EVENT_INFO_GET_MESSAGE_TYPE
};

class BpEventInfo: public BpInterface<IEventInfo> 
{
public:
    BpEventInfo(const sp<IBinder>& impl):BpInterface<IEventInfo>(impl){};
	OString getListenerName(void)
	{
		Parcel data, reply;
	    data.writeInterfaceToken(IEventInfo::getInterfaceDescriptor());
	    remote()->transact(EN_EVENT_INFO_GET_LISTENER_NAME, data, &reply);
		return reply.readCString();
	}
	void setListenerName(OString& name)
	{
		Parcel data, reply;
		data.writeInterfaceToken(IEventInfo::getInterfaceDescriptor());
		data.writeCString(name.c_str());
	    remote()->transact(EN_EVENT_INFO_SET_LISTENER_NAME, data, &reply);
	}
	OString getString(void)
	{
		Parcel data, reply;
	    data.writeInterfaceToken(IEventInfo::getInterfaceDescriptor());
	    remote()->transact(EN_EVENT_INFO_GET_STRING, data, &reply);
		return reply.readCString();
	}
	U32 getU32Number(void)
	{
		Parcel data, reply;
	    data.writeInterfaceToken(IEventInfo::getInterfaceDescriptor());
	    remote()->transact(EN_EVENT_INFO_GET_U32, data, &reply);
		return reply.readInt32();
	}
	EN_EVENT_INFO_TYPE getInfoType()
	{
		Parcel data, reply;
		data.writeInterfaceToken(IEventInfo::getInterfaceDescriptor());
		remote()->transact(EN_EVENT_INFO_GET_INFO_TYPE, data, &reply);
		return (EN_EVENT_INFO_TYPE)reply.readInt32();
	}
	U8 getEventType()
	{
		Parcel data, reply;
		data.writeInterfaceToken(IEventInfo::getInterfaceDescriptor());
		remote()->transact(EN_EVENT_INFO_GET_EVENT_TYPE, data, &reply);
		return (U8)reply.readInt32();
	}
	void setMsgType(BOOL type)
	{
		Parcel data, reply;
		data.writeInterfaceToken(IEventInfo::getInterfaceDescriptor());
		data.writeInt32(type);
		remote()->transact(EN_EVENT_INFO_SET_MESSAGE_TYPE, data, &reply);
	}
	BOOL getMsgType()
	{
		Parcel data, reply;
		data.writeInterfaceToken(IEventInfo::getInterfaceDescriptor());
		remote()->transact(EN_EVENT_INFO_GET_MESSAGE_TYPE, data, &reply);
		return (BOOL)reply.readInt32();
	}
};

IMPLEMENT_META_INTERFACE(EventInfo, "Oceanus.os.EventInfo");

status_t BnEventInfo::onTransact(uint32_t code, const Parcel& data,Parcel* reply, uint32_t flags)
{
	switch(code)
		{
			case EN_EVENT_INFO_GET_LISTENER_NAME:
				{
					CHECK_INTERFACE(IEventInfo,data,reply);
					reply->writeCString(getListenerName().c_str());
					return NO_ERROR;
				}
			break;
			case EN_EVENT_INFO_SET_LISTENER_NAME:
				{
					CHECK_INTERFACE(IEventInfo,data,reply);
					OString name = data.readCString();
					setListenerName(name);
					return NO_ERROR;
				}
			break;
			case EN_EVENT_INFO_GET_STRING:
				{
					CHECK_INTERFACE(IEventInfo,data,reply);
					reply->writeCString(getString().c_str());
					return NO_ERROR;
				}
			break;
			case EN_EVENT_INFO_GET_U32:
				{
					CHECK_INTERFACE(IEventInfo,data,reply);
					reply->writeInt32(getU32Number());
					return NO_ERROR;
				}
			break;
			case EN_EVENT_INFO_GET_OBJECT:
				{
					OCEANUS_LOGI(" ");
					CHECK_INTERFACE(IEventInfo,data,reply);
					return NO_ERROR;
				}
			break;
			case EN_EVENT_INFO_GET_INFO_TYPE:
				{
					CHECK_INTERFACE(IEventInfo,data,reply);
					reply->writeInt32((U32)getInfoType());
					return NO_ERROR;
				}
			break;
			case EN_EVENT_INFO_GET_EVENT_TYPE:
				{
					CHECK_INTERFACE(IEventInfo,data,reply);
					reply->writeInt32((U8)getEventType());
					return NO_ERROR;
				}
			break;
			case EN_EVENT_INFO_SET_MESSAGE_TYPE:
				{
					CHECK_INTERFACE(IEventInfo,data,reply);
					setMsgType((BOOL)data.readInt32());
					return NO_ERROR;
				}
			break;
			case EN_EVENT_INFO_GET_MESSAGE_TYPE:
				{
					CHECK_INTERFACE(IEventInfo,data,reply);
					reply->writeInt32((BOOL)getMsgType());
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

