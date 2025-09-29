#include <iostream>
#include <list>
#include <memory>
using namespace std;

/*

观察者模式：定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变时，所有依赖于它的对象都要得到通知并自动更新。
观察者模式从根本上讲必须包含两个角色：观察者和被观察对象。

被观察对象自身应该包含一个容器来存放观察者对象，当被观察者自身发生改变时通知容器内所有的观察者对象
自动更新。观察者对象可以注册到被观察者的中，完成注册后可以检测被观察者的变化，
接收被观察者的通知。当然观察者也可以被注销掉，停止对被观察者的监控。

*/

#if 1

// 观察者抽象类
class obserInterface
{
public:
    virtual void update() = 0;
};

// 被观察者抽象类
class beobseredInterface
{
public:
    virtual void addone(obserInterface *obj) = 0;
    virtual void removeone(obserInterface *obj) = 0;
    virtual void notify() = 0;
};

// 观察者类
class obser : public obserInterface
{
public:
    obser(const string &name) : m_name(name) {}
    void update()
    {
        cout << m_name << "得到了通知" << endl;
    }

private:
    const string m_name;
};

// 被观察者类
class beobsered : public beobseredInterface
{
public:
    ~beobsered()
    {
        m_list.clear();
    }
    virtual void addone(obserInterface *obj)
    {
        for (auto it = m_list.begin(); it != m_list.end(); it++)
        {
            if ((*it) == obj)
            {
                cout << "该观察者已经在队列中了" << endl;
                return;
            }
        }
        m_list.push_back(obj);
    }
    virtual void removeone(obserInterface *obj)
    {
        for (auto it = m_list.begin(); it != m_list.end(); it++)
        {
            if ((*it) == obj)
            {
                m_list.erase(it);
                cout << "该观察者已经被移除了" << endl;
                return;
            }
        }
    }
    virtual void notify()
    {
        for (auto it = m_list.begin(); it != m_list.end(); it++)
        {
            (*it)->update();
        }
    }

private:
    list<obserInterface *> m_list;
};

int main()
{
    obserInterface *obj1 = new obser("111");
    obserInterface *obj2 = new obser("222");
    obserInterface *obj3 = new obser("333");
    obserInterface *obj4 = new obser("444");

    beobsered *beobj1 = new beobsered();
    beobj1->addone(obj1);
    beobj1->addone(obj2);
    beobj1->addone(obj3);
    beobj1->addone(obj4);

    beobj1->notify();

    cout << "---------------------------------" << endl;
    beobj1->removeone(obj1);
    beobj1->notify();
}

#endif
