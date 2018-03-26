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
	static  vector<_Thread*> m_vecTaskList;/** 任务列表 */
	static  bool shutdown;/** 线程退出标志 */
	int     m_iThreadNum;/** 线程池中启动的线程数 */
	pthread_t   *pthread_id;
	static pthread_mutex_t m_pthreadMutex;/** 线程同步锁 */
	static pthread_cond_t m_pthreadCond;/** 线程同步的条件变量 */
protected:
		static void* ThreadFunc(void * threadData);/** 新线程的线程回调函数 */
		static int MoveToIdle(pthread_t tid);/** 线程执行结束后，把自己放入到空闲线程中 */
		static int MoveToBusy(pthread_t tid);/** 移入到忙碌线程中去 */
		int Create();/** 创建线程池中的线程 */
public:
	ThreadPool(int threadNum = THREAD_NUMBER);
	int AddTask(_Thread *task);/** 把任务添加到任务队列中 */
	int StopAll();/** 使线程池中的线程退出 */
	int getTaskSize();/** 获取当前任务队列中的任务数 */  
};


#endif//__OCEANUS_BASIC_THREADPOOL_H__

