 /**************************************************************************
 * Copyright:sky
 * Author: heji@skyworth.com
 * Date:2015-12-18
 * Description: oceanus system basic event info interface for binder
 **************************************************************************/
#ifndef __OCEANUS_BASIC_EVENTINFO_INTERFACE_H__
#define __OCEANUS_BASIC_EVENTINFO_INTERFACE_H__
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <binder/IPCThreadState.h>
#include "Oceanus_Types.h"

using namespace android;

typedef enum
{
	E_EVENT_INFO_TYPE_STRING = 0,
	E_EVENT_INFO_TYPE_NUMBER,
	E_EVENT_INFO_TYPE_JSON,
	E_EVENT_INFO_TYPE_NULL
}EN_EVENT_INFO_TYPE;

class IEventInfo : public IInterface
{
public:
	DECLARE_META_INTERFACE(EventInfo);
	virtual OString getListenerName(void) = 0;
	virtual void setListenerName(OString& name) = 0;
	virtual OString getString(void) = 0;
	virtual U32 getU32Number(void) = 0;
	virtual EN_EVENT_INFO_TYPE getInfoType() = 0;
	virtual U8 getEventType() = 0;
	virtual void setMsgType(BOOL type) = 0;
	virtual BOOL getMsgType() = 0;
};

class BnEventInfo: public BnInterface<IEventInfo>
{
public:
 virtual status_t onTransact(uint32_t code, const Parcel& data,Parcel* reply, uint32_t flags= 0 );
};

#endif//__OCEANUS_BASIC_EVENTINFO_INTERFACE_H__

