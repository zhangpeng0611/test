#include <iostream>
#include<vector>
#include<queue>
#include<mutex>
#include<condition_variable>
#include<memory>
#include <atomic>
using namespace std;
	
#if 0
关于enable_shared_from_this和shared_from_this的讲解：https://www.cnblogs.com/S1mpleBug/p/16770731.html
智能指针：https://www.cnblogs.com/S1mpleBug/p/16759502.html

auto_ptr:可以随意转移资源的所有权，但只支持最后一个智能指针持有资源，其他的智能指针全部被置为了
nullptr，通过这样解决了智能指针浅拷贝的问题。
scope_ptr:通过私有化了拷贝构造函数和赋值构造函数来杜绝了智能指针浅拷贝的问题，不支持转移资源的所有权。
unique_ptr:删除了拷贝构造函数和赋值构造函数，提供了右值引用的拷贝构造函数和赋值构造函数


以上就是对enable_shared_from_this和shared_from_this机制的介绍。这东西主要解决了该问题：
当返回某对象时，由于智能指针调用常规的构造函数导致引用计数类的多次构造，从而导致在释放内存时，
多个智能指针对同一块内存进行多次释放，出现Core dump。使用该机制则可返回指向某对象的智能指针，
这样就调用的是智能指针的拷贝构造函数而非常规的构造函数，使得引用计数类不会被多次构造，
避免出现同一内存多次释放的情况。

enable_shared_from_this和shared_from_this机制的底层原理：
enable_shared_from_this类中有一个指向资源的弱智能指针的成员变量，子类继承enable_shared_from_this
后会继承这个成员变量，当初始化时会初始化这个weak_ptr，调用shared_from_this返回一个指向类自身的
强智能指针时，实际底层如下：返回一个从weak_ptr构造而来的shared_ptr。
shared_ptr<_Ty> shared_from_this()
{	// return shared_ptr
	return (shared_ptr<_Ty>(_Wptr));  //实际上这个过程会判断弱智能指针是否能提升成功，提升成功
	                                  //则返回shared_ptr这个对象
}

综上所说，所有过程都没有再使用shared_ptr的普通构造函数，没有在产生额外的引用计数对象，
不会存在把一个内存资源，进行多次计数的过程；更关键的是，通过weak_ptr到shared_ptr的提升，
还可以在多线程环境中判断对象是否存活或者已经析构释放，在多线程环境中是很安全的，
通过this裸指针进行构造shared_ptr，不仅仅资源会多次释放，而且在多线程环境中也不确定this指向的对象
是否还存活

通过make_shared来创建智能指针的好处是什么：
1.减少内存分配次数，提高性能
std::shared_ptr<int>(new int(4)),1.分配int对象的内存  2.分配控制块的内存，需要两次内存分配
而make_shared可以一次性分配一块连续的内存块，同时容纳对象和控制块

2.避免潜在的内存泄漏的风险
当使用new进行创建的时候，在new分配完内存后，shared_ptr进行构造前如果出现异常，可能会出现内存泄漏
而std::make_shared会在内部完成对象构造和shared_ptr初始化，整个过程是原子的，不存在中间状态：

#endif



#if 0
//智能指针基础知识

template <typename T>
class SmartPtr
{
public:
	SmartPtr(T* ptr = nullptr) :m_ptr(ptr){}
	~SmartPtr() { delete m_ptr; }

	T& operator*() { return *m_ptr; }
	T* operator->() { return m_ptr; }

private:
	T* m_ptr;
};
int main()
{
	SmartPtr<int>ptr(new int);
	*ptr = 29;

	class test
	{
	public:
		void myprintf()
		{
			cout << "test::myprintf!" << endl;
		}
	};
	SmartPtr<test>ptr1(new test);
	ptr1->myprintf();

}
#endif


#if 0


//带有引用计数的类
template<typename T>
class RfCnt
{
public:
	RfCnt(T* ptr = nullptr) :m_ptr(ptr)
	{
		if (m_ptr != nullptr)
		{
			m_count = 1;
		}
	}

	void add()
	{
		m_count++;
   }
	int del()
	{
		return --m_count;
   }

private:
	T* m_ptr;
	atomic_int m_count;
};



//带有引用计数的智能指针
template<typename T>
class SmPtr
{
public:
	SmPtr(T* ptr = nullptr) :m_ptr(ptr)
	{
	    //每次调用构造函数就会产生一个计数类，如果多个指向同一块资源的智能指针调用构造
		//函数就会对同一块资源产生多个引用计数类，释放资源时会对同一块资源进行多次释放
		rfcnt = new RfCnt<T>(m_ptr);
	}
	~SmPtr()
	{
		if (0 == rfcnt->del())
		{
			delete m_ptr;
			m_ptr = nullptr;
		}
	}

	T& operator*() { return *m_ptr; }
	T* operator->() { return m_ptr; }

	//重写拷贝构造函数
	SmPtr<T>(const SmPtr<T>& src)
		:m_ptr(src.m_ptr),rfcnt(src.rfcnt)
	{
		rfcnt->add();
	}

	//重写赋值构造函数
	SmPtr<T>& operator=(const SmPtr<T>& src)
	{
		if (this == &src)
			return *this;
		if (0 == rfcnt->del())
			delete m_ptr;

		m_ptr = src.m_ptr;
		rfcnt = src.rfcnt;
		rfcnt->add();
		return *this;
	}

private:
	T* m_ptr;
	RfCnt<T>* rfcnt;
};

int main()
{
	SmPtr<int>ptr1(new int);
	SmPtr<int>ptr2(ptr1);
	SmPtr<int>ptr3;
	ptr3 = ptr2;
	
}

#endif

