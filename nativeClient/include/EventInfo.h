/**************************************************************************
* Author: heji
* Date:2015-12-18
* Description: oceanus system basic event info
**************************************************************************/
#ifndef __BASIC_EVENTINFO_H__
#define __BASIC_EVENTINFO_H__
#include <utils/threads.h>
#include <IEventInfo.h>
#include "Types.h"
#include "Debug.h"

using namespace std;

class EventInfo : public BnEventInfo
{
public:
	EventInfo(U8 event);
	EventInfo(U8 event,OString& info_str,BOOL bIsJson);
	EventInfo(U8 event,U32 info_u32);
	EventInfo(IEventInfo* EventInfo);
	virtual ~EventInfo();
	OString getString(void);
	OString getListenerName(void);
	void setListenerName(OString& name);
	U32 getU32Number();
	EN_EVENT_INFO_TYPE getInfoType();
	U8 getEventType();
	void setMsgType(BOOL type);
	BOOL getMsgType(); 
private:
	union
	{
		char* info_str;
		char* info_obj;//json string
		U32 info_u32;
	}info;
	U8 m_u8EventType;
	BOOL m_bIsPrivateMsg;
	EN_EVENT_INFO_TYPE m_u8InfoType;
	OString listener_name;
};

#endif //__BASIC_EVENTINFO_H__

