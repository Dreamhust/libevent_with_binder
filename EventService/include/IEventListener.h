 /**************************************************************************
 * Copyright:sky
 * Author: heji@skyworth.com
 * Date:2015-12-18
 * Description: oceanus system basic event listener interface for binder
 **************************************************************************/
#ifndef __OCEANUS_BASIC_EVENTLISTENER_INTERFACE_H__
#define __OCEANUS_BASIC_EVENTLISTENER_INTERFACE_H__
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <binder/IPCThreadState.h>
#include "IEventInfo.h"

#include "Oceanus_Types.h"
using namespace android;

class IEventListener :public IInterface
{
	public:
		DECLARE_META_INTERFACE(EventListener);
		virtual OString getListenerName() = 0;
		virtual void OnEvent(const sp<IEventInfo>& info) = 0;
};


class BnEventListener: public BnInterface<IEventListener>
{
public:
 virtual status_t onTransact(uint32_t code, const Parcel& data,Parcel* reply, uint32_t flags= 0 );
};



#endif//__OCEANUS_BASIC_EVENTLISTENER_INTERFACE_H__

