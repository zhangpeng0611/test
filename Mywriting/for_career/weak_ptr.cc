#include <iostream>
#include <memory>
#include <unordered_map>

// 介绍weak_ptr除了解决循环引用问题之外的其他作用

/*
weak_ptr没有默认的构造函数，只能通过shared_ptr进行构造，并且weak_ptr没有*和->运算符的使用
*/

// 1.使用weak_ptr来实现缓存对象
/*假设有一个Widgt类，现在如果直接从文件中加载Widgt需要的时间成本很大
所以希望将该Widgt先缓存起来，当下次需要使用的时候就可以不需要从文件中
进行加载，可以直接使用。
*/
class Widgt
{
};
// 从文件中加载所需要的时间成本很大
std::shared_ptr<Widgt> loadingWidgtFromFiles(int id);

std::shared_ptr<Widgt> fastLoadingWidgt(int id)
{
    /*
     这里为什么unordered_map中要使用weak_ptr，而不直接使用shared_ptr,
     因为这里unordered_map是静态的，是长期存在的，如果使用shared_ptr，则因为
     至少unoredered_map中对shared_ptr的使用，所以shared_ptr的引用计数永远不会为0，
     会导致缓存中的对象永远不会销毁，而weak_ptr不会增加引用计数，所以当缓存中的对象
     没有被其他地方使用的时候，缓存对象会被销毁。
    */
    static std::unordered_map<int, std::weak_ptr<Widgt>> cache;
    auto object = cache[id].lock();
    if (!object)
    {
        object = loadingWidgtFromFiles(id);
        cache[id] = object;
    }
    return object;
}

// 使用weak_ptr；来实现单例模式
class singleton
{
public:
    std::shared_ptr<singleton> getinstance()
    {
        std::shared_ptr<singleton> instance = m_instance.lock();
        if (!instance)
        {
            instance = std::shared_ptr<singleton>(new singleton());
            m_instance = instance;
        }
        return instance;
    }

private:
    singleton() {};
    static std::weak_ptr<singleton> m_instance;
};
// 静态成员变量类内声明，类外定义
std::weak_ptr<singleton> singleton::m_instance;

// weak_ptr用于实现enable_shared_from_this模板类
/*
  std::enable_shared_from_this是一个模板类，它的作用是为了解决在类成员函数中获取std::shared_ptr的问题。
它提供了一个成员函数shared_from_this()，该函数返回一个指向当前对象的std::shared_ptr

解决问题： 如果通过this指针创建shared_ptr时，相当于通过一个裸指针创建shared_ptr，
多次创建会导致多个shared_ptr对象管理同一个内存。当shared_ptr对象销毁时，会释放this指向的内存，
但是this指针可能还会被使用，导致程序崩溃。

*/

// 错误示例,如果多次创建就会造成多个shared_ptr管理同一个内存
class A
{
public:
    std::shared_ptr<A> get_shared_ptr()
    {
        return std::shared_ptr<A>(this);
    }
};

// 正确使用方法,继承enable_shared_from_this类；通过shared_from_this()方法返回。
class B : public std::enable_shared_from_this<B>
{
public:
    std::shared_ptr<B> get_shared_ptr()
    {
        return shared_from_this();
    }
};

// enable_shared_from_this模板的实现原理

/*
 当一个对象被 std::shared_ptr 创建时，如果该对象继承自 enable_shared_from_this，std::shared_ptr
 会自动对 _M_weak_this 进行初始化。

 只有通过 std::shared_ptr 来创建对象（例如 std::make_shared 或者 new std::shared_ptr），
 _M_weak_this 才会被正确初始化。如果对象是直接实例化的（比如栈上的对象或者使用 new 创建的原始指针）
 ，_M_weak_this 就不会被初始化，此时调用 shared_from_this() 会引发未定义行为。
*/

template <typename _Tp>
class enable_shared_from_this
{
protected:
    // 构造函数和析构函数都受保护，只能被派生类调用
    constexpr enable_shared_from_this() noexcept = default;
    enable_shared_from_this(const enable_shared_from_this &) noexcept = default;
    enable_shared_from_this &operator=(const enable_shared_from_this &) noexcept = default;
    ~enable_shared_from_this() = default;

public:
    // 获取指向当前对象的 shared_ptr
    shared_ptr<_Tp> shared_from_this()
    {
        return shared_ptr<_Tp>(_M_weak_this);
    }
    shared_ptr<const _Tp> shared_from_this() const;

    // C++17 新增：获取指向当前对象的 weak_ptr
    weak_ptr<_Tp> weak_from_this()
    {
        return _M_weak_this;
    } noexcept;
    weak_ptr<const _Tp> weak_from_this() const noexcept;

private:
    // 关键成员：存储对当前对象的弱引用
    mutable weak_ptr<_Tp> _M_weak_this;

    // 允许 shared_ptr 访问和修改 _M_weak_this
    template <typename _Up>
    friend class shared_ptr;
};

int main()
{
}