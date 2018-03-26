/**************************************************************************
* Copyright:sky
* Author: heji@skyworth.com
* Date:2015-12-18
* Description: oceanus system basic thread pool
**************************************************************************/
#include <iostream>
#include "ThreadPool.h"

vector<_Thread*> ThreadPool::m_vecTaskList;         //�����б�
bool ThreadPool::shutdown = false;
	
pthread_mutex_t ThreadPool::m_pthreadMutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t ThreadPool::m_pthreadCond = PTHREAD_COND_INITIALIZER;

/**
 * �̳߳ع����๹�캯��
 */
ThreadPool::ThreadPool(int threadNum)
{
	this->m_iThreadNum = threadNum;
	OCEANUS_LOGI("I will create %d threads",threadNum);
	Create();
}

/**
 * �̻߳ص�����
 */
void* ThreadPool::ThreadFunc(void* threadData)
{
	//pthread_t tid = pthread_self();
	while (1)
	{
		pthread_mutex_lock(&m_pthreadMutex);
		while (m_vecTaskList.size() == 0 && !shutdown)
		{
			pthread_cond_wait(&m_pthreadCond, &m_pthreadMutex);
		}
		
		if (shutdown)
		{
			pthread_mutex_unlock(&m_pthreadMutex);
			OCEANUS_LOGI("thread %lu will exit", pthread_self());
			pthread_exit(NULL);	
		}
		
		//OCEANUS_LOGI("tid %lu run", tid);
		vector<_Thread*>::iterator iter = m_vecTaskList.begin();
		
		/**
		* ȡ��һ�����񲢴���֮
		*/
		_Thread* task = *iter;
		if (iter != m_vecTaskList.end())
		{
			task = *iter;
			m_vecTaskList.erase(iter);
		}
		
		pthread_mutex_unlock(&m_pthreadMutex);
		
 	 	task->Run(task->m_pArguments); /** ִ������ */
		//OCEANUS_LOGI("tid:%lu idle", tid);
	}
	return (void*)0;
}

/**
 * ������������������񲢷����߳�ͬ���ź�
 */
int ThreadPool::AddTask(_Thread *task)
{
	pthread_mutex_lock(&m_pthreadMutex);
	this->m_vecTaskList.push_back(task);
	pthread_mutex_unlock(&m_pthreadMutex);
	pthread_cond_signal(&m_pthreadCond);
	return 0;
}

/**
 * �����߳�
 */
int ThreadPool::Create()
{
	pthread_id = (pthread_t*)malloc(sizeof(pthread_t) * m_iThreadNum);
	for(int i = 0; i < m_iThreadNum; i++)
	{
		pthread_create(&pthread_id[i], NULL, ThreadFunc, NULL);
	}
	return 0;
}

/**
 * ֹͣ�����߳�
 */
int ThreadPool::StopAll()
{
	/** �����ظ����� */
	if (shutdown)
	{
		return -1;	
	}
	OCEANUS_LOGI("Now I will end all threads!!");
	/** �������еȴ��̣߳��̳߳�Ҫ������ */
	shutdown = true;
    pthread_cond_broadcast(&m_pthreadCond);
    
    /** �����ȴ��߳��˳�������ͳɽ�ʬ�� */
    for (int i = 0; i < m_iThreadNum; i++)
    {
    	pthread_join(pthread_id[i], NULL);	
    }
	
	free(pthread_id);
	pthread_id = NULL;
	
 	/** �������������ͻ����� */
 	pthread_mutex_destroy(&m_pthreadMutex);
    pthread_cond_destroy(&m_pthreadCond);
    
	return 0;
}

/**
 * ��ȡ��ǰ������������
 */
int ThreadPool::getTaskSize()
{
	return m_vecTaskList.size();	
}

