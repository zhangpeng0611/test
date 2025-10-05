// https://www.cnblogs.com/linuxAndMcu/category/1048923.html?page=2
// c++11新特性大全、

#if 0
// 指针传递
#include <iostream>
using namespace std;

// 改变的是实参地址所指向的变量值
void mySwap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{
    int x = 1;
    int y = 2;
    cout << "x的地址为:" << &x << " x地址所指向的值为" << x << endl;
    cout << "y的地址为:" << &y << " y地址所指向的值为" << y << endl;

    cout << "经过mySwap函数交换之后:" << endl;
    mySwap(&x, &y);
    cout << "x的地址为:" << &x << " x地址所指向的值为" << x << endl;
    cout << "y的地址为:" << &y << " y地址所指向的值为" << y << endl;

    return 0;
}

#endif

#if 0

#include <iostream>
using namespace std;

class B
{
    int m_iNum;
    virtual void foo() {};
};
class D : public B
{
    char *m_szName[100];
};

int main()
{

    cout << "class B的大小:" << sizeof(B) << endl;
    cout << "class D的大小:" << sizeof(D) << endl;

    return 0;
}

#endif

#if 0

#include <iostream>

using namespace std;

int main(){

    time_t now=time(0);
    char* dt=ctime(&now);
    cout<<"本地时间为:"<<dt<<endl;

    struct tm* ltm=localtime(&now);
    cout<<"年:"<<1900+ltm->tm_year<<endl;
    cout<<"月:"<<1+ltm->tm_mon<<endl;
    cout<<"日:"<<ltm->tm_mday<<endl;
    cout<<"时间:"<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec<<endl;

    return 0;
}

#endif

#if 0

#include <iostream>
using namespace std;

double division(int x, int y)
{
    if (y == 0)
    {
        throw "second parameter cannot be zero!";
    }
    return static_cast<double>(x) / y;
}

int main()
{
    int x = 4, y = 3;
    try
    {
        cout << division(x, y) << endl;
    }
    catch (const char *msg)
    {
        cerr << msg << endl;
    }

    return 0;
}

#endif

#if 0

#include <iostream>
using namespace std;

void myFunc(int a){
    int* p=0;
    *p=a;
}

int main(){

    myFunc(1);
    return 0;
}

#endif

#if 0

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

int main()
{
    cout << "多线程调试" << endl;

    int pid = fork();
    if (pid != 0)
    {
        cout << "在主进程中,PID:" << getpid() << endl;
        for (int i = 0; i < 10; i++)
        {
            sleep(1);
            cout << i << endl;
        }
    }
    else
    {
        cout << "在子进程中,PID:" << getpid() << ",PPID:" << getppid() << endl;
        for (int i = 0; i < 10; i++)
        {
            sleep(1);
            cout << i << endl;
        }
    }

    return 0;
}

#endif

#if 0

//模拟多态运行机制

#include <iostream>
using namespace std;

class Base1
{
public:
    Base1() : base1_1(11) {}
    int base1_1;
    virtual void base1_fun1()
    {
        std::cout << "Base1::base1_fun1()" << std::endl;
    }
};

class Base2
{
public:
    Base2() : base2_1(21) {}
    int base2_1;
};

class Base3
{
public:
    Base3() : base3_1(31) {}
    int base3_1;
    virtual void base3_fun1()
    {
        std::cout << "Base3::base3_fun1()" << std::endl;
    }
};

class Derive1 : public Base1, public Base2, public Base3
{
public:
    Derive1() : derive1_1(11) {}
    int derive1_1;

    virtual void base3_fun1()
    {
        std::cout << "Derive1::base3_fun1()" << std::endl;
    }
    virtual void derive1_fun1()
    {
        std::cout << "Derive1::derive1_fun1()" << std::endl;
    }
};

void foo(Base1 *pb1, Base2 *pb2, Base3 *pb3, Derive1 *pd1)
{
    std::cout << "Base1::\n"
              << "    pb1->base1_1 = " << pb1->base1_1 << "\n"
              << "    pb1->base1_fun1(): ";
    pb1->base1_fun1();

    std::cout << "Base2::\n"
              << "    pb2->base2_1 = " << pb2->base2_1
              << std::endl;

    std::cout << "Base3::\n"
              << "    pb3->base3_1 = " << pb3->base3_1 << "\n"
              << "    pb3->base3_fun1(): ";
    pb3->base3_fun1();

    std::cout << "Derive1::\n"
              << "    pd1->derive1_1 = " << pd1->derive1_1 << "\n"
              << "    pd1->derive1_fun1(): ";
    pd1->derive1_fun1();
    std::cout << "    pd1->base3_fun1(): ";
    pd1->base3_fun1();

    std::cout << std::endl;
}

int main()
{
    Derive1 d1;
    foo(&d1, &d1, &d1, &d1);

    return 0;
}

#endif


