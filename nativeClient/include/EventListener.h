 /**************************************************************************
 * Author: heji
 * Date:2015-12-18
 * Description: oceanus system basic event listener
 **************************************************************************/
#ifndef __BASIC_EVENTLISTENER_H__
#define __BASIC_EVENTLISTENER_H__
#include <IEventInfo.h>
#include <utils/threads.h>
#include <utils/SortedVector.h>
#include <IEventListener.h>

#include "Types.h"
using namespace std;
class EventListener : public BnEventListener
{
public:
	EventListener(OString name);
 	virtual ~EventListener();
	virtual OString getListenerName();
 	virtual void OnEvent(const sp<IEventInfo>& info);
private:
	OString m_sName;
	
	Mutex mExecuteLock;

};

#endif //__BASIC_EVENTLISTENER_H__

