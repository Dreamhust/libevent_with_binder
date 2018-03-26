/**************************************************************************
* Author: heji
* Date:2015-12-18
* Description: oceanus system basic event info
**************************************************************************/

#include "EventInfo.h"

EventInfo::EventInfo(U8 event)
{
	m_bIsPrivateMsg = TRUE;
	m_u8EventType = event;
	m_u8InfoType = E_EVENT_INFO_TYPE_NULL;
}

EventInfo::EventInfo(U8 event,U32 info_u32)
{
	m_bIsPrivateMsg = TRUE;
	m_u8EventType = event;
	m_u8InfoType = E_EVENT_INFO_TYPE_NUMBER;
	info.info_u32 = info_u32;
}

EventInfo::EventInfo(U8 event,OString& info_str,BOOL bIsJson)
{
	m_bIsPrivateMsg = TRUE;
	m_u8EventType = event;
	if(bIsJson)
		{
			m_u8InfoType = E_EVENT_INFO_TYPE_JSON;
		}
	else
		{
			m_u8InfoType = E_EVENT_INFO_TYPE_STRING;
		}
	int size = info_str.length();
	info.info_str = (char*) malloc(sizeof(char)*(size+1));
	info_str.copy(info.info_str, size, 0);
	info.info_str[size] = '\0';
}

EventInfo::EventInfo(IEventInfo* m_pEventInfo)
{
	m_u8InfoType = m_pEventInfo->getInfoType();
	switch(m_u8InfoType)
		{
			case E_EVENT_INFO_TYPE_STRING:
			case E_EVENT_INFO_TYPE_JSON:
				{
					int size = m_pEventInfo->getString().length();
					info.info_str = (char*) malloc(sizeof(char)*(size+1));
					m_pEventInfo->getString().copy(info.info_str, size, 0);
					info.info_str[size] = '\0';
				}
			break;
			case E_EVENT_INFO_TYPE_NUMBER:
				{
					info.info_u32 = m_pEventInfo->getU32Number();
				}
			break;
			default:break;
		}
	m_bIsPrivateMsg = m_pEventInfo->getMsgType();
	m_u8EventType = m_pEventInfo->getEventType();
	listener_name = m_pEventInfo->getListenerName();
}


EventInfo::~EventInfo()
{
	if(m_u8InfoType ==E_EVENT_INFO_TYPE_STRING)
		{
			if(info.info_str!=NULL)
				{
					free(info.info_str);
				}
		}
}


OString EventInfo::getListenerName(void)
{
	return listener_name;
}
void EventInfo::setListenerName(OString& name)
{
	listener_name = name;
}

OString EventInfo::getString(void)
{
	if(m_u8InfoType == E_EVENT_INFO_TYPE_STRING || m_u8InfoType == E_EVENT_INFO_TYPE_JSON)
	{
		return info.info_str;
	}
	return "";
}

U32 EventInfo::getU32Number(void)
{
	if(m_u8InfoType == E_EVENT_INFO_TYPE_NUMBER)
		return info.info_u32;
	return 0;
}

EN_EVENT_INFO_TYPE EventInfo::getInfoType()
{
	return m_u8InfoType;
}

U8 EventInfo::getEventType()
{
	return m_u8EventType;
}
void EventInfo::setMsgType(BOOL type)
{
	m_bIsPrivateMsg = type;
}
BOOL EventInfo::getMsgType()
{
	return m_bIsPrivateMsg;
}


