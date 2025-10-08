#pragma once

#include "nocopyable.hpp"
#include "Timestamp.hpp"

#include <unordered_map>
#include <vector>

class channel;
class Eventloop;

class Poller:nocopyable
{
    public:
    using ChannelList=std::vector<channel*>;

    Poller(Eventloop *loop);
    virtual ~Poller()=default;

    //给所有IO复用保留统一的接口
    virtual Timestamp poll(int timeoutMs,ChannelList *activeChannels)=0;
    virtual void updateChannel(channel *channel)=0;
    virtual void removeChannel(channel *channel)=0;

    //判断参数channel是否在当前poller中
    bool hasChannel(channel *channel) const;

    //默认的IO多路复用的实现
    static Poller* newDefaultPoller(Eventloop *loop);
    
    protected:
    using ChannelMap=std::unordered_map<int,channel*>;
    ChannelMap channels_;
    
    private:
    Eventloop *ownerLoop_;
};