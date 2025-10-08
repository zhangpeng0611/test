#pragma once

#include "nocopyable.hpp"
#include "channel.hpp"
#include "Timestamp.hpp"
#include "Poller.hpp"
#include "CurrentThread.h"

#include <vector>
#include <atomic>
#include <memory>
#include <functional>
#include <mutex>

class EventLoop : nocopyable
{
public:
    using Functor = std::function<void()>;

    EventLoop();
    ~EventLoop();

    // 开启事件循环
    void loop();
    // 退出事件循环
    void quit();

    Timestamp pollReturnTime() const { return pollReturnTime_; }

    // 在当前Loop中执行cb
    void runInLoop(Functor cb);
    // 把cb放入队列中，唤醒loop所在的线程,执行cb
    void queueInLoop(Functor cb);

    // 唤醒loop所在的线程
    void wakeup();

    // Eventloop的方法 => poller调用
    void updateChannel(channel *cha);
    void removeChannel(channel *cha);
    bool hasChannel(channel *cha);

    // 判断eventloop对象是否在自己的线程里
    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }

private:
    void handleRead();        // wake up
    void doPendingFunctors(); // 执行回调

    using ChannelList = std::vector<channel *>;

    std::atomic_bool looping_;
    std::atomic_bool quit_;

    const pid_t threadId_; // 记录当前Loop所在的线程的id

    Timestamp pollReturnTime_; // poller返回发生事件的channels的时间点
    std::unique_ptr<Poller> poller_;

    int wakeupfd_; // 主要作用：当mainLoop获取一个新用户的channel，通过轮询算法选择一个subloop，通过该成员唤醒subloop
    std::unique_ptr<channel> wakeupChannel_;

    ChannelList activeChannels_;

    std::atomic_bool callingPendingFunctors_; // 用来标识当前loop是否有需要执行的回调操作
    std::vector<Functor> pendingFunctors_;    // 存储loop需要执行的回调操作
    std::mutex mutex_;
};