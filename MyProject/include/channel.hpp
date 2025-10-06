#pragma once

#include "nocopyable.hpp"
#include "Timestamp.hpp"

#include <functional>
#include <memory>
#include <iostream>

class Eventloop;

class channel : nocopyable
{
public:
    using ReadEventCallback = std::function<void(Timestamp)>;
    using EventCallback = std::function<void()>;

    channel(Eventloop *loop, int fd);
    ~channel();

    // fd得到poller通知之后，用来处理事件的函数
    void handleEvent(Timestamp receiveTime);

    // 设置回调函数对象
    void setReadCallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); };
    void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }

    int fd() const { return fd_; }
    int events() const { return events_; }
    int set_revents(int revt) { revents_ = revt; }

    //防止channel被手动remove掉了，channel还在执行回调操作
    void tie(const std::shared_ptr<void>&);

    // 设置fd相应的事件状态
    void enableReading()
    {
        events_ |= KReadEvent;
        update();
    }
    void enableWriting()
    {
        events_ |= KWriteEvent;
        update();
    }
    void disableReading()
    {
        events_ &= ~KReadEvent;
        update();
    }
    void disableWriting()
    {
        events_ &= ~KWriteEvent;
        update();
    }
    void disableAll()
    {
        events_ = KNonEvent;
        update();
    }

    // 返回fd当前的事件状态
    bool isNoneEvent() const { return events_ == KNonEvent; }
    bool isWriting() const { return events_ & KWriteEvent; }
    bool isReading() const { return events_ & KReadEvent; }

    Eventloop* getOwnerLoop() {return loop_;}
    void remove();

private:
    static const int KNonEvent;
    static const int KReadEvent;
    static const int KWriteEvent;

    void update();
    void handleEventWithGUard(Timestamp receiveTime);

    Eventloop *loop_; // 时间循环
    const int fd_;   // fd,poller监听的对象
    int events_;     // 注册fd感兴趣的事件
    int revents_;    // poller返回的具体发生的事件

    std::weak_ptr<void> tie_;
    bool tied_;

    // 因为channel能够获取到fd最终发生的具体事件revents,所以它负责调用具体事件的回调操作
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;
};