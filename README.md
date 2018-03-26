本模块属于大型项目一部分，
现分享源码到github与大家交流，
请自行编写Android.mk，可参考代码中的Android.mk根据你的项目修改亦可，
使用下面代码编译链接生成可执行的二进制文件，
根据你所部署的Android平台版本进行集成。
注意Android 5.0之后需要配置SELinux权限策略。


/*********************main.cpp*********************/
#include <stdio.h> 
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <binder/IPCThreadState.h>  
#include <binder/ProcessState.h>  
#include <binder/IServiceManager.h> 
#include "Event.h"
#include "Types.h"
#include "Debug.h"
using namespace android;
int main(int arge1,char** arge2)
{
	
	sp<ProcessState> proc(ProcessState::self());
	sp<IServiceManager> sm = defaultServiceManager();
	OCEANUS_LOGI("ServiceManager: %p", sm.get());
	Event::instantiate();
	ProcessState::self()->startThreadPool();
	IPCThreadState::self()->joinThreadPool(); 
}
/*********************main.cpp*********************/