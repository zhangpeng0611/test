#if 1

//读优先锁
#include <iostream>
#include <semaphore.h>
using namespace std;

sem_t wSem_;
sem_t rCountSem_;
int rCount=0;

void P(sem_t sem);
void V(sem_t sem);
void write();
void read();

void Write()
{
    while(true)
    {
        P(wSem_);
        write();
        V(wSem_);
    }
}

void Read()
{
    while(true)
    {
        //读于读操作线程个数的增加这个过程需要通过信号量进行控制，而多个线程进行读操作这个动作不需要进行控制
        P(rCountSem_);
        if(rCount==0)
        {
            P(wSem_);
        }
        rCount++;
        V(rCountSem_);

        read();

        P(rCountSem_);
        rCount--;
        if(rCount==0)
        {
            V(wSem_);
        }
        V(rCountSem_);
    }
}


int main()
{
    return 0;
}


#endif



#if 1

//读写公平锁
#include <iostream>
#include <semaphore.h>
using namespace std;

sem_t wSem_;
sem_t rCountSem_;
sem_t flag;
int rCount=0;

void P(sem_t sem);
void V(sem_t sem);
void write();
void read();

void Write()
{
    while(true)
    {
        P(flag);
        P(wSem_);
        write();
        V(wSem_);
        V(flag);
    }
}

void Read()
{
    while(true)
    {
        //读于读操作线程个数的增加这个过程需要通过信号量进行控制，而多个线程进行读操作这个动作不需要进行控制
        P(flag);
        P(rCountSem_);
        if(rCount==0)
        {
            P(wSem_);
        }
        rCount++;
        V(rCountSem_);
        V(flag);

        read();

        P(rCountSem_);
        rCount--;
        if(rCount==0)
        {
            V(wSem_);
        }
        V(rCountSem_);
    }
}


int main()
{
    return 0;
}


#endif