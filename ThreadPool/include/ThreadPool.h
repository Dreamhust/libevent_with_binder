/**************************************************************************
* Copyright:sky
* Author: heji@skyworth.com
* Date:2015-12-18
* Description: oceanus system basic thread pool
**************************************************************************/
#ifndef __OCEANUS_BASIC_THREADPOOL_H__
#define __OCEANUS_BASIC_THREADPOOL_H__
#include <vector>  
#include <string>  
#include <functional>
#include <pthread.h>  
#include "OEvent.h"
#define THREAD_NUMBER 64
using namespace std;
namespace Oceanus
{
	class _Thread
	{
		public:	
			virtual ~_Thread(){};
			virtual void Run(void* arg) = 0;
			void* m_pArguments;
	};

template <typename TProcesser>
	
class Thread : public _Thread
{
public:
	Thread(TProcesser* processer):processer_(processer){};
	virtual ~Thread(){};
	void SetProcesser(void (TProcesser::*threadFun)(void*),void* arg);
	BOOL Start();
private:
	static void* ThreadFunction(void* context)
		{
			Thread<TProcesser>* obj = (Thread<TProcesser>*)context;
			obj->Run(obj->m_pArguments);
			pthread_exit(NULL);
			return (void*)0;
		};
	virtual void Run(void* arg);
private:
	static Thread<TProcesser>* m_pThis;
	TProcesser* processer_;
	void (TProcesser::*processerFun_)(void*);
	void(*cb)(void);
};
template <typename TProcesser>
Thread<TProcesser>* Thread<TProcesser>::m_pThis = NULL;

template <typename TProcesser>
void Thread<TProcesser>::SetProcesser(void (TProcesser::*threadFun)(void*),void* arg)
{
	processerFun_ = threadFun;
	m_pArguments = arg;
}

template <typename TProcesser>
BOOL Thread<TProcesser>::Start()
{
	int temp;
	pthread_t m_IThreadId;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if((temp = pthread_create(&m_IThreadId,&attr,ThreadFunction,this)) != 0)
    	{
    		OCEANUS_LOGE("Create Thread fail !\n");
			return FALSE;
    	}
	return TRUE;
}

template <typename TProcesser>
void Thread<TProcesser>::Run(void* arg)
{
	if(processerFun_!=NULL)
		{
			#if ( (__cplusplus <= 199711L) || (TARGET_OS == Android))
			typedef std::mem_fun1_t<void, TProcesser,void*> CallBackFuntion;
			typedef std::binder1st<CallBackFuntion> BoundFunction;
			BoundFunction function = std::bind1st(std::mem_fun(processerFun_),processer_);
			function(arg);
			#else
			auto cb = std::bind(processerFun_,processer_,arg);
			cb(arg);
			#endif
		}
}

}
using namespace Oceanus;
class ThreadPool
{
private:
	static  vector<_Thread*> m_vecTaskList;/** �����б� */
	static  bool shutdown;/** �߳��˳���־ */
	int     m_iThreadNum;/** �̳߳����������߳��� */
	pthread_t   *pthread_id;
	static pthread_mutex_t m_pthreadMutex;/** �߳�ͬ���� */
	static pthread_cond_t m_pthreadCond;/** �߳�ͬ������������ */
protected:
		static void* ThreadFunc(void * threadData);/** ���̵߳��̻߳ص����� */
		static int MoveToIdle(pthread_t tid);/** �߳�ִ�н����󣬰��Լ����뵽�����߳��� */
		static int MoveToBusy(pthread_t tid);/** ���뵽æµ�߳���ȥ */
		int Create();/** �����̳߳��е��߳� */
public:
	ThreadPool(int threadNum = THREAD_NUMBER);
	int AddTask(_Thread *task);/** ��������ӵ���������� */
	int StopAll();/** ʹ�̳߳��е��߳��˳� */
	int getTaskSize();/** ��ȡ��ǰ��������е������� */  
};


#endif//__OCEANUS_BASIC_THREADPOOL_H__

