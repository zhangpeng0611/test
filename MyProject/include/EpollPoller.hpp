#pragma once

#include "Poller.hpp"
#include "Timestamp.hpp"

#include <vector>
#include <sys/epoll.h>

class Eventloop;

class EpollPoller : public Poller
{
public:
    EpollPoller(Eventloop *lopp);
    ~EpollPoller() override;

    // 重写基类poller的抽象方法
    Timestamp poll(int timeoutMs, ChannelList *activeChannels) override;
    void updateChannel(channel *channel) override;
    void removeChannel(channel *channel) override;

private:
    static const int kInitEventListSize = 16;

    // 填写活跃的连接
    void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;

    // 更新通道
    void update(int operation, channel *channel);

    using EventList = std::vector<epoll_event>;

    int epollfd_;
    EventList events_;
};