/**************************************************************************
* Author: heji
* Date:2015-12-13
* Description: system event
**************************************************************************/
#ifndef __EVENT_H__
#define __EVENT_H__
#include "Types.h"

typedef enum
{
	E_SYSTEM_EVENT_ADD_EVENT = 0,
	E_SYSTEM_EVENT_TEST_EVENT_1,
	E_SYSTEM_EVENT_TEST_EVENT_2,
	E_SYSTEM_EVENT_COUNT
}EN_OSYSTEM_EVENT_LIST;


typedef enum
{
	E_SYSTEM_TIME_EVENT_TEST_TIME_EVENT_1 = E_SYSTEM_EVENT_COUNT+1,// off auto 10min 15min 30min etc..
	E_SYSTEM_EVENT_MAX
}EN_OSYSTEM_TIME_EVENT_LIST;

typedef struct
{
	/* data */
	S32 param1;
	S32 param2;
}ST_POSTEVENT_PARAMS;


#endif//__EVENT_H__

