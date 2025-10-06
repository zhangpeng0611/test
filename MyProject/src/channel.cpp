#include "channel.hpp"
#include <sys/epoll.h>

//
const int channel::KNonEvent = 0;
const int channel::KReadEvent = EPOLLIN | EPOLLPRI;
const int channel::KWriteEvent = EPOLLOUT;

channel::channel(Eventloop *loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0)
{
}

// 当改表了channel所表示的fd的events事件后，update负责在poller里面更改fd的相应的事件
// Evnetloop=>channelList Poller

void channel::update()
{
    // 通过channel所属的eventlopp，调用poller的相应方法，注册fd的events事件
    //todo
}

void channel::remove()
{
    // 通过eventloop调用相应的方法，将当前的channel删除掉
    //todo
}

//将channel与TcpConnection持有者进行绑定
void channel::tie(const std::shared_ptr<void> &obj)
{
    tie_=obj;
    tied_=true;
}

// fd得到poller通知之后，需要判断一下这个channel是否与所持有者(TcpConnection)
// 所绑定，如果绑定的话需要判断这个channel是否还存在
void channel::handleEvent(Timestamp receiveTime)
{
    if (tied_)
    {
        std::shared_ptr<void> guard = tie_.lock();
        if (guard)
        {
            handleEventWithGUard(receiveTime);
        }
    }
    else
    {
        handleEventWithGUard(receiveTime);
    }
}

// 根据poller通知channel发生的具体事件，由channel调用之前注册的具体回调函数
void channel::handleEventWithGUard(Timestamp receiveTime)
{
    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN))
    {
        if (closeCallback_)
        {
            closeCallback_();
        }
    }

    if (revents_ & EPOLLERR)
    {
        if (errorCallback_)
        {
            errorCallback_();
        }
    }

    if (revents_ & (EPOLLIN | EPOLLPRI))
    {
        if (readCallback_)
        {
            readCallback_(receiveTime);
        }
    }

    if (revents_ & EPOLLOUT)
    {
        if (writeCallback_)
        {
            writeCallback_();
        }
    }
}