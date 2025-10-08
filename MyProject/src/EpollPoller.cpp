#include "EpollPoller.hpp"
#include "channel.hpp"

#include <unistd.h>
#include <strings.h>

// channel未添加到poller中
const int KNew = -1;
// channel已经添加到poller中
const int KAdded = 1;
// channel已经从poller中删除
const int KDeleted = 2;

EpollPoller::EpollPoller(Eventloop *loop)
    : Poller(loop), epollfd_(::epoll_create1(EPOLL_CLOEXEC)), events_(kInitEventListSize)
{
}

EpollPoller::~EpollPoller()
{
    ::close(epollfd_);
}

Timestamp EpollPoller::poll(int timeoutMs, ChannelList *activeChannels)
{
    int numEvents = ::epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs);
    int saveErrno = errno;
    Timestamp now(Timestamp::now());

    if (numEvents > 0)
    {
        fillActiveChannels(numEvents, activeChannels);
        if (numEvents == events_.size())
        {
            events_.resize(events_.size() * 2);
        }
    }
    else if (numEvents == 0)
    {
    }
    else
    {
        if (saveErrno != EINTR)
        {
            errno = saveErrno;
        }
    }
}
void EpollPoller::updateChannel(channel *channel)
{
    const int index = channel->index();
    if (index == KNew || index == KDeleted)
    {
        if (index == KNew)
        {
            int fd = channel->fd();
            channels_[fd] = channel;
        }
        channel->set_index(KAdded);
        update(EPOLL_CTL_ADD, channel);
    }
    else
    {
        int fd = channel->fd();
        if (channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL, channel);
            channel->set_index(KDeleted);
        }
        else
        {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

void EpollPoller::removeChannel(channel *channel)
{
    int fd = channel->fd();
    channels_.erase(fd);

    int index = channel->index();
    if (index == KAdded)
    {
        update(EPOLL_CTL_DEL, channel);
    }
    channel->set_index(KNew);
}

// 填写活跃的连接
void EpollPoller::fillActiveChannels(int numEvents, ChannelList *activeChannels) const
{
    for (int i = 0; i < numEvents; i++)
    {
        channel *channel_ = static_cast<channel *>(events_[i].data.ptr);
        channel_->set_revents(events_[i].events);
        activeChannels->push_back(channel_); // poller在这里填写好了活跃的channel,eventloop就能拿到这个活跃的连接
    }
}

// 更新channel通道
void EpollPoller::update(int operation, channel *channel)
{
#if 0

#include <sys/epoll.h>

    struct epoll_event {
        uint32_t     events;      /* 感兴趣的事件，或已发生的事件 */
        epoll_data_t data;        /* 用户数据 */
    };

    /* data 成员的联合体类型 */
    typedef union epoll_data {
        void    *ptr;  /* 指向自定义数据结构的指针 */
        int      fd;   /* 文件描述符 */
        uint32_t u32;  /* 32位整数 */
        uint64_t u64;  /* 64位整数 */
    } epoll_data_t;

#endif

    epoll_event event;
    bzero(&event, sizeof(event));

    int fd = channel->fd();
    event.events = channel->events();
    event.data.fd = channel->fd();
    event.data.ptr = channel;

    epoll_ctl(epollfd_, operation, fd, &event);
}