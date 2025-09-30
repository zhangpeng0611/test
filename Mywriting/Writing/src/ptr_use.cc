#include <iostream>
using namespace std;

class Person
{
public:
    void sayHi()
    {
        printf("Hi!\n");
    }
    void selfIntroduction()
    {
        printf("My name is %s\n", name.c_str());
    }

private:
    string name;
};

int main()
{
    // 这里将0x12345强转为Person*的指针，意味着让指针指向内存地址0x12345，因为这里调用的sayHi方法中没有访问
    // 任何的成员变量，所以不需要通过this指针去读取对象数据，所以这里的执行是没有问题的，但是这样是不安全的
    ((Person *)0x12345)->sayHi();

    // 这里将指针指向空，当调用selfIntroduction方法时，因为要访问成员变量，所以需要通过this指针去访问，这里
    // 的this指针为NULL,程序会访问NULL地址的name成员变量，会触发内存访问错误
    ((Person *)NULL)->selfIntroduction();
    return 0;
}