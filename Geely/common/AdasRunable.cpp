#include "AdasRunable.h"

AdasRunable::AdasRunable(const char* strName)
{
    ALOGI("AdasRunable :: AdasRunable");
    m_strThreadName = strName;
    m_bQuit = false;
}


AdasRunable::~AdasRunable()
{
    ;
}

//debug perpose

void AdasRunable::start()
{
    printf("*****AdasRunable::start [%s] thread*******\n", m_strThreadName);
    AutoMutex _l(m_lock);
    run(m_strThreadName, m_eThreadPriority);
    m_signal.signal();
}

void AdasRunable::quit()
{
     AutoMutex _l(m_lock);
     m_bQuit = true;
     requestExit();
}

void AdasRunable::join()
{
     join();
}

void AdasRunable::pause()
{
    m_signal.wait(m_lock);
}

void AdasRunable::resume()
{
   m_signal.signal();
}

bool AdasRunable::isRunning()
{
    return isRunning();
}

bool AdasRunable::threadLoop()
{
    init();

    while(!m_bQuit){
        
        update();
        usleep(50000);
    }

    return true;
}


