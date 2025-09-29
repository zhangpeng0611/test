#include <iostream>
#include <functional>
using namespace std;

// 学习地址:https://www.cnblogs.com/S1mpleBug/p/16793438.html

#if 1

void hello(string arg)
{
    cout << arg << endl;
}

void myprintf(string arg1, string arg2)
{
    cout << arg1 << arg2 << endl;
}

template <typename T>
class myfunction
{
};

// function实现原理
template <typename T, typename A>
class myfunction<T(A)>
{
public:
    using PFUNC = T (*)(A);
    myfunction(PFUNC pfunc) : _pfunc(pfunc) {};
    T operator()(A arg)
    {
        return _pfunc(arg);
    }

private:
    PFUNC _pfunc;
};

// 针对可变参的情况
template <typename T, typename... A>
class myfunction<T(A...)>
{
public:
    using PFUNC = T (*)(A...);
    myfunction(PFUNC pfunc) : _pfunc(pfunc) {};
    T operator()(A... arg)
    {
        return _pfunc(arg...);
    }

private:
    PFUNC _pfunc;
};

int main()
{
    hello("hello world");
    // function用法
    function<void(string)> myfun = hello;
    myfun("hello world");

    // 使用自己实现的function的原理
    myfunction<void(string)> func(hello);
    func("hello world");

    // 可变参的情况
    myfunction<void(string, string)> func1(myprintf);
    myprintf("hello ", "myfriend");
    return 0;
}

#endif
