#ifndef ADAS_CORE_RUNABLE_H_
#define ADAS_CORE_RUNABLE_H_

#include <utils/Thread.h>
#include <utils/Condition.h>
#include <utils/Mutex.h>
#include <utils/ThreadDefs.h>
#include <fcntl.h>
#include "AdasDefine.h"

using namespace android;

class AdasRunable : public Thread
{
public:

	enum AdasThreadPriority
	{
	    ThreadPriority_Default =   PRIORITY_DEFAULT,
	    ThreadPriority_Lowest  =   PRIORITY_LOWEST,
	    ThreadPriority_Normal =   PRIORITY_NORMAL,
	    ThreadPriority_Height =   PRIORITY_HIGHEST,
	};

	AdasRunable(const char* strName);
	virtual ~AdasRunable();
	virtual void start();
	virtual void start(void* pData)	{;}
	virtual void quit();
	virtual void join();
	virtual void pause();
	virtual void resume();
	virtual bool isRunning();
	virtual const char* getName() {return m_strThreadName;};
	virtual void setPriority(AdasThreadPriority priority) { m_eThreadPriority = priority;};
	//virtual bool initialize()	{ return false; };
	//virtual bool unInitialize(){ return false;};
	virtual void update()=0;
	virtual void init() =0;

	Mutex m_lock;

private:

	virtual bool threadLoop();


private:
	Condition m_signal;
	const char* m_strThreadName ;
	AdasThreadPriority  m_eThreadPriority;
	bool m_bQuit;

};

#endif  //_CRUNABLESERVICE_H
