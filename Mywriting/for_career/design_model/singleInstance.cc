#include <iostream>
#include <mutex>
using namespace std;

// 饿汉模式：在创建这个类的时候直接就创建出来了单例对象
// 多线程的时候没有线程安全问题，因为只有一个实例对象
class Taskqueue
{
public:
    // 要使得类只有一个单例对象，则必须保证该类不能构造出其他的实例，所以要删除掉
    // 类的无参构造和拷贝构造，因为移动构造只是对象资源所有权的转移，所有移动构造
    // 不需要删除

    Taskqueue(const Taskqueue &t) = delete;
    Taskqueue &operator=(const Taskqueue &t) = delete;
    static Taskqueue *getinstance()
    {
        return m_taskq;
    }

private:
    Taskqueue() = default;
    static Taskqueue *m_taskq;
};
Taskqueue *Taskqueue::m_taskq = new Taskqueue();

// 懒汉模式：创建类的时候不创建出对象，当要获取单例的时候才创建出对象
// 懒汉模式在多线程的情况下会有线程安全问题，因为每个线程来都会创建出类的一个
// 实例对象
class Taskqueue
{
public:
    // 要使得类只有一个单例对象，则必须保证该类不能构造出其他的实例，所以要删除掉
    // 类的无参构造和拷贝构造，因为移动构造只是对象资源所有权的转移，所有移动构造
    // 不需要删除

    Taskqueue(const Taskqueue &t) = delete;
    Taskqueue *operator=(const Taskqueue &t) = delete;
    static Taskqueue *getinstance()
    {
        /*
           if (m_taskq == nullptr)
        {
            //进行加互斥锁可以保证线程安全，每次只允许一个线程进行访问，但是这样的效率比较低
            m_mutex.lock();
            m_taskq = new Taskqueue();
            m_mutex.unlock();
        }
        */

        // 双重检查锁定,当多个线程进行访问时发现m_taskq不为空则直接返回唯一实例对象，效率高
        if (m_taskq == nullptr)
        {
            // 进行加互斥锁可以保证线程安全，每次只允许一个线程进行访问，但是这样的效率比较低
            m_mutex.lock();
            if (m_taskq == nullptr)
            {
                m_taskq = new Taskqueue();
            }
            m_mutex.unlock();
        }
        return m_taskq;
    }

private:
    Taskqueue() = default;
    static Taskqueue *m_taskq;
    static mutex m_mutex;
};
Taskqueue *Taskqueue::m_taskq = nullptr;

// 懒汉模式下解决多线程访问安全问题可以使用静态局部变量,因为静态局部变量的生命周期是整个程序的运行
//
class Taskqueue
{
public:
    // 要使得类只有一个单例对象，则必须保证该类不能构造出其他的实例，所以要删除掉
    // 类的无参构造和拷贝构造，因为移动构造只是对象资源所有权的转移，所有移动构造
    // 不需要删除

    Taskqueue(const Taskqueue &t) = delete;
    Taskqueue &operator=(const Taskqueue &t) = delete;
    static Taskqueue &getinstance()
    {
        /*
        c++11特性之后，静态局部变量可以保证线程安全并且满足单例模式的需求，静态局部变量只会在
        第一次调用该函数时初始化一次，后续不会进行初始化，并且在多线程中保证只有一个线程执行初始化
        其他线程阻塞等待初始化完成
        */
        static Taskqueue m_taskq;
        return m_taskq;
    }

private:
    Taskqueue() = default;
};

int main()
{

    // Taskqueue* tsq = Taskqueue::getinstance();
    // cout << tsq;
}
