#include <iostream>
#include <utility>   //forward的头文件
using namespace std;

//模板类型推导+引用折叠+std::forward

//学习网址：https://www.cnblogs.com/S1mpleBug/p/16703328.html

//const int&是左值引用，但是可以绑定右值,const int& a=40;可以用来延长右值的生命周期
// 内部原理如下： const int temp=40;
//                const int&a=temp;
// 
//const int&不能修改绑定的右值，但是int&&可以修改绑定的右值
//右值引用的变量还是一个左值int &&x=40;x为左值

/*
std::move底层实现原理：
template <typename T>
typename std::remove_reference<T>::type&& move(T&& t) noexcept {
	return static_cast<typename std::remove_reference<T>::type&&>(t);
}

std::move 的实现依赖两个关键步骤：去除引用修饰和强制类型转换；
去除引用修饰:std::remove_reference<T> 是一个模板元函数，作用是 “剥离类型 T 的引用修饰符”
若 T 是 int&（左值引用），则 std::remove_reference<T>::type 是 int；
若 T 是 int&&（右值引用），则 std::remove_reference<T>::type 是 int；
若 T 是 int（非引用），则结果仍是 int。

强制类型转换：static_cast<T>，将输入强制转换为右值引用的类型.

std::move 的名字具有一定迷惑性 —— 它不会实际移动任何数据，也不会修改原始对象的内容或生命周期。
它的唯一作用是：将表达式的 “左值属性” 转换为 “右值属性”，让编译器认为该对象 “即将被销毁”，
从而优先调用移动构造函数 / 移动赋值运算符（而非拷贝版本）。

*/


/*
万能引用 ：既可以接收左值，又可以接收右值
实参是左值，他就是左值引用（引用折叠）
实参是右值，他就是右值引用
PS：万能引用还有另一种叫法：引用折叠 ，就是当其传入参数为左值时，&& 会折叠成& ；
当传入参数为右值时，&& 不折叠照常接收

模板中的 && 不代表右值引用，而是 万能引用 ，其既能接收左值又能接收右值。
模板的万能引用只是提供了能够接收同时接收左值引用和右值引用的能力
但是引用类型的唯一作用就是—— 限制了接收的类型 ，后续使用中都退化成了左值

引用折叠的规则：T&& &&为右值引用（当T为非引用类型），其他折叠均为左值引用
*/

void func(int& t) { cout << "左值引用" << endl; }
void func(const int& t) { cout << "const 左值引用" << endl; }
void func(int&& t) { cout << "右值引用" << endl; }
void func(const int&& t) { cout << "const 右值引用" << endl; }

template<typename T>
void PerfectForward(T&& t) {
	//func(t);

	//std::forward 完美转发 在传参的过程中保留 对象原生类型属性，即保持它的左值或者右值的属性
	func(std::forward<T>(t));
}

int main() {
	//这里都会输出左值引用，因为在PerfectForward中t是变量，所以是左值，因此无论PerfectForward传递的
	//是左值还是右值，func(t)都会走左值引用
	PerfectForward(10);
	int a;
	PerfectForward(a);



	return 0;
}