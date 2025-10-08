#include "Poller.hpp"
#include "EpollPoller.hpp"

Poller* Poller::newDefaultPoller(Eventloop *loop)
{
    if(::getenv("MUDUO_USE_POLL"))
    {
        return nullptr;  //生成poll的实例
    }else
    {
        return new EpollPoller(loop);
    }
}