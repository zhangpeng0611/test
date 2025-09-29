
// 面向对象三大特性：封装，继承，多态

/*
继承：主要有三种继承访问限定符，public,protected,private。继承的时候将基类中的内容继承到子类并且根据访问限定符的
不通将基类中的内容的访问权限进行更改，例如如果是通过protected进行继承的，则将基类中的Public和protected的内容
的访问权限也全部修改为protected
*/

#if 0
#include <iostream>
using namespace std;


class Animal {
public:
    void eat() { std::cout << "Animal eats" << std::endl; }
protected:
    void sleep() { std::cout << "Animal sleeps" << std::endl; }
private:
    void breathe() { std::cout << "Animal breathes" << std::endl; }
};

class Dog : private Animal {
public:
    void bark() { 
        std::cout << "Dog barks" << std::endl;
        sleep(); // 可以访问基类的 protected 成员
    }
};

int main() {

    Animal an;
    an.eat();  //从类的外部通过对象进行访问的时候只能访问public限定符下的内容

    Dog dg;
    dg.bark();  //从类的外部通过对象只能访问public限定符下的内容，所以可以访问bark方法
    // dg.eat();   //因为是private继承，所以基类的public和protected下的内容全部变为子类private下的内容，所以不能访问

    // Dog myDog;
    // myDog.eat();  // OK, public 成员
    // myDog.bark(); // OK, 派生类自己的成员
    // myDog.sleep(); // Error, 在类外部不能访问 protected 成员
    // myDog.breathe(); // Error, 不能访问 private 成员
}

#endif

#if 1

#include <bits/stdc++.h>
using namespace std;

class Base
{
public:
    Base() : m_name("zhangpeng")
    {
        cout << "默认构造 Base constructor:" << m_name << endl;
    }
    Base(string name) : m_name(name)
    {
        cout << "有参构造 Base constructor:" << m_name << endl;
    }
    ~Base()
    {
        cout << "Base destructor" << endl;
    }

private:
    string m_name;
};

class Son:public Base
{
public:
    Son() : m_name("xiaofei"),Base("yangdi")
    {
        cout << "Son constructor:" << m_name << endl;
    }
    ~Son()
    {
        cout << "Son destructor" << endl;
    }

private:
    string m_name;
};

int main()
{
   Son s1;
   return 0;
}

#endif

/*

多态的概念：通俗来说，就是多种形态，具体点就是去完成某个行为，当不同的对象去完成时会产生出不同
的状态和结果

多态的实现：
静态多态：函数重载
动态的多态：父类/基类的指针或引用调用重写虚函数
1、父类指针或者引用指向父类，调用的就是父类的虚函数
2、父类指针或者引用指向子类，调用的就是子类的虚函数

三个概念对比：重载，重写，重定义（隐藏）
重载:两个函数作用域相同，函数名相同，(参数数量 | 参数类型 | 参数顺序 不同都属于重载)，注意返回值类型不能作为重载
的依据，这是因为编译器在调用时无法确定调用哪个，并且很多时候的使用不需要返回值，这个时候如果返回值类型不同也能作为
重载的话，则在调用的时候也无法分辨出来调用哪个

重写：
1.继承关系
2.函数签名完全相同：函数名必须相同，参数列表必须相同，返回值类型需要兼容
3.基类函数必须为虚函数，基类函数为虚函数才能实现多态，基类函数如果不为虚函数，这时重写派生类的函数会隐藏
基类的同名函数，即调用时只能根据指针/引用编译器的实际类型进行函数调用，而无法进行多态调用
4.访问权限可以不同，派生类重写函数的访问权限可以和基类函数的访问权限不同
总结：
重写：基类virtual+派生类同签名->支持多态调用
隐藏：基类无virtual+派生类同名函数→不支持多态（编译时绑定），基类函数被“覆盖”但未被“重写”
virtual 关键字是区分两者的核心，它告诉编译器：“这个函数可能在派生类中被重写，需要支持多态调用”。

重定义：两个函数分别位于基类和子类，函数名相同，两个基类和子类的同名函数不构成重写就是重定义

抽象类：
在虚函数的后面写上 =0 ，则这个函数为纯虚函数。包含纯虚函数的类叫做抽象类（也叫接口类），抽象类
不能实例化出对象。派生类继承后也不能实例化出对象，只有重写纯虚函数，派生类才能实例化出对象。
纯虚函数规范了派生类必须重写，另外纯虚函数更体现出了接口继承。


虚函数表：https://blog.csdn.net/Primeprime/article/details/80776625

当一个类包含虚函数（或继承了包含虚函数的基类）时，编译器会为该类生成一个虚表（vtbl)
一个存储虚函数地址的数组。同时，该类的每个对象会包含一个虚表指针（vptr），这个指针指向该类
对应的虚表。虚表指针的作用是实现多态：当通过基类指针 / 引用调用虚函数时，编译器会通过对象的虚
表指针找到实际类型（派生类）的虚表，进而调用正确的虚函数版本。

1. 虚表与虚表指针的基本机制
C++ 的多态通过虚函数（virtual function） 实现，而虚函数的调用依赖于两个关键结构：
虚表（vtable）：每个包含虚函数的类（或其派生类）会有一个独立的虚表，本质是一个函数指针数组，
存储该类所有虚函数的地址（包括继承自父类但未重写的，以及自身重写的）。
虚表指针（vptr）：每个对象实例中会隐含一个指向所属类虚表的指针（vptr），通常位于对象内存
布局的起始位置（编译器实现决定，目的是保证访问效率）。
当类声明了虚函数时，编译器会为该类生成虚表，并在对象构造时自动初始化 vptr，使其指向当前类的虚表。

2. 子类对象的内存布局与父类兼容
当子类继承父类（且父类含有虚函数）时，子类对象的内存布局会包含两部分：
父类部分：继承父类的成员变量和vptr（但 vptr 会被重写为指向子类的虚表）。
子类部分：子类新增的成员变量。
这种布局保证了 **“父类指针指向子类对象时，指针实际指向的是子类对象中‘父类部分’的起始地址”**，
而这个起始地址处恰好存放着子类的 vptr（因为 vptr 通常在对象最开头）。

虚函数表：
总结一下派生类的虚表生成：
a.先将基类中的虚表内容拷贝一份到派生类虚表中
b.如果派生类重写了基类中某个虚函数，用派生类自己的虚函数覆盖虚表中基类的虚函数
c.派生类自己新增加的虚函数按其在派生类中的声明次序增加到派生类虚表的最后。
d.虚表指针的物理位置是属于基类的，但是它指向的是派生类的虚表

虚函数存在哪的？ 注意虚表存的是虚函数指针，不是虚函数，虚函数和普通函数一样的，都是存在代码段的，
只是他的指针又存到了虚表中。另外对象中存的不是虚表，存的是虚表指针。


*/
