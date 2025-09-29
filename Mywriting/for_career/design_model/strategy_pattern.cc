/*
策略模式（Strategy Pattern）是一种行为型设计模式，它的核心思想是将算法（或行为）封装成独立的策略类，
使它们可以相互替换，从而让算法的变化独立于使用算法的客户端。这种模式通过分离算法的定义与使用，
提高了代码的灵活性和可维护性。

策略模式的核心角色：
1.环境类(Context):负责使用策略类的客户端，持有策略类的引用，调用策略对象的方法来完成
业务逻辑，可以动态切换使用的策略

2.抽象策略接口：所有具体的策略类必须要实现的方法，一般会包含一些执行的方法

3.具体的策略类：实现抽象策略接口，每一个策略类对应一个实现方法

*/

#include <iostream>
using namespace std;

// 抽象策略接口：支付方式
class PaymentStrategy
{
public:
    virtual void pay(double amount) = 0; // 抽象支付方法
    virtual ~PaymentStrategy() = default;
};

// 具体策略1：支付宝支付
class AlipayStrategy : public PaymentStrategy
{
public:
    void pay(double amount) override
    {
        std::cout << "使用支付宝支付：" << amount << "元" << std::endl;
        // 支付宝宝支付的具体逻辑（调用API、加密等）
    }
};

// 具体策略2：微信支付
class WechatPayStrategy : public PaymentStrategy
{
public:
    void pay(double amount) override
    {
        std::cout << "使用微信支付：" << amount << "元" << std::endl;
        // 微信支付的具体逻辑
    }
};

// 具体策略3：银行卡支付
class BankCardStrategy : public PaymentStrategy
{
public:
    void pay(double amount) override
    {
        std::cout << "使用银行卡支付：" << amount << "元" << std::endl;
        // 银行卡支付的具体逻辑
    }
};

// 环境类：订单（使用支付策略）
class Order
{
private:
    PaymentStrategy *paymentStrategy; // 持有策略对象
    double amount;                    // 订单金额

public:
    // 构造时指定金额和支付策略
    Order(double amt, PaymentStrategy *strategy)
        : amount(amt), paymentStrategy(strategy) {}

    // 动态切换支付策略
    void setPaymentStrategy(PaymentStrategy *strategy)
    {
        paymentStrategy = strategy;
    }

    // 统一支付接口（委托给策略对象）
    void pay()
    {
        paymentStrategy->pay(amount);
    }
};

int main()
{
    // 创建具体策略对象
    PaymentStrategy *alipay = new AlipayStrategy();
    PaymentStrategy *wechat = new WechatPayStrategy();

    // 创建订单，使用支付宝支付
    Order order1(199.9, alipay);
    order1.pay(); // 输出：使用支付宝支付：199.9元

    // 切换策略为微信支付
    order1.setPaymentStrategy(wechat);
    order1.pay(); // 输出：使用微信支付：199.9元

    // 释放资源
    delete alipay;
    delete wechat;
    return 0;
}