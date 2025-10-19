/*
GDB调试：https://blog.csdn.net/weixin_45031801/article/details/134399664


什么叫段错误：段错误是进程崩溃的一种特定类型，本质是程序试图访问不属于它的内存区域，
或对内存区域进行了不允许的操作（如写只读内存）

GDB：GDB 即 GNU 调试器，在 Linux 下可用于加载程序崩溃时产生的核心转储文件。
通过它的 “info threads”指令可以查看当前所有线程的列表，使用“thread <id>”
指令能切换到特定线程，再配合 “backtrace” 指令，就能查看具体线程的调用栈，判断线程崩溃的情况 。

pstack：pstack 是一款用于查看进程线程栈的工具，安装后，只需指定对应的进程 ID，
它即可快速呈现出该进程所有线程的调用栈，帮助使用者了解线程运行状态，排查崩溃问题。

以下面的代码例子进行举例讲解怎么进行Debug
1.通过GDB调试进行-》run之后代码停在报错的位置，可以bt打印函数调用栈,如然后可以frame+栈帧号进入具体
栈帧->list来展示出错附近代码，也可以通过print来打印变量进行查错

2.也可以通过 ps aux | grep 文件名,来找到奔溃的进程号，然后通过pstack+进程号可以展示进程的调用栈信息

注意gdb调试中可以进行回退，采用reverse-step(rs),reverse-next(rn)等等

3.如果是对于内存泄漏的场景，可以使用valgrind来进行检测内存泄漏：
首先编译生成可调试的可执行文件，然后使用valgrind --leak-check=full ./可执行文件名
会输出如下的泄漏报告：
==12345== 4 bytes in 1 blocks are definitely lost in loss record 1 of 2
==12345==    at 0x4C2B0F3: operator new(unsigned long) (vgpreload_memcheck-amd64-linux.so)
==12345==    by 0x400686: basic_leak() (simple_leak.cpp:6)
==12345==    by 0x400706: main (simple_leak.cpp:19)

==12345== 20 bytes in 1 blocks are definitely lost in loss record 2 of 2
==12345==    at 0x4C2B1E8: operator new[](unsigned long) (vgpreload_memcheck-amd64-linux.so)
==12345==    by 0x4006B6: array_leak() (simple_leak.cpp:13)
==12345==    by 0x400718: main (simple_leak.cpp:22)

会详细说明发生内存泄漏的函数和代码行号

*/

#if 1

#include <iostream>
#include <thread>
#include <chrono>

// 线程函数：会导致崩溃
void crash_thread_func() {
    std::cout << "线程开始运行..." << std::endl;

    // 休眠1秒，确保主线程有时间输出信息
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 故意访问空指针，导致线程崩溃
    int* null_ptr = nullptr;
    *null_ptr = 42;  // 这里会发生段错误（Segmentation fault）

    std::cout << "线程正常结束" << std::endl;  // 这行不会执行
}

int main() {
    std::cout << "主线程启动" << std::endl;

    // 创建并启动线程
    std::thread t(crash_thread_func);

    // 等待线程结束
    t.join();

    std::cout << "主线程结束" << std::endl;  // 如果线程崩溃，这行可能不会执行

    return 0;
}


#endif

/*
gdb调试core dump文件：

#include <iostream>

void bad_function() {
    int* ptr = nullptr;
    *ptr = 42;  // 这里会产生段错误，因为访问了空指针
}

int main() {
    std::cout << "程序开始运行..." << std::endl;
    bad_function();
    std::cout << "程序正常结束" << std::endl;  // 这行不会执行
    return 0;
}

# 启动gdb调试，指定程序和core文件
需要注意的是：虽然调试可执行文件和调试core文件最终都能查错，但是调试可执行文件是启动程序并逐步运行
而core文件是程序崩溃是生成的内存快照，不需要手动运行到错误的位置

gdb ./segfault_example core

# 在gdb中，首先查看崩溃位置
(gdb) bt  # 打印调用栈回溯

# 典型输出会类似这样：
# #0  0x000055f8b77f214 in bad_function () at segfault_example.cpp:5
# #1  0x000055f8b77f235 in main () at segfault_example.cpp:10

# 查看崩溃位置的代码
(gdb) list  # 显示当前位置附近的代码

# 查看变量信息
(gdb) frame 0  # 切换到第0帧
(gdb) print ptr  # 查看ptr变量的值，会显示它是nullptr

# 退出gdb
(gdb) quit


*/


/*
CPU占用率100%，怎么定位？
可能出现这样的情况的原因：
1.循环次数过多或者陷入了死循环
2.频繁的锁竞争或者是自旋锁
3.不合理的递归条件

怎么定位：
1.首先可以通过top,然后按下p键使得进程按照CPU占用率进行排序，找到占用CPU最多的进程
2.top -H -p <PID>  # -H显示线程，-p指定进程ID，找到占用CPU最多的线程
3.gdb -p <PID>  # 附加到目标进程
4.info threads,列出所有线程
5.thread <线程号>,进入指定线程
6.bt，打印函数调用栈
*/

/*
gdb如何挂载到一个正在运行的程序上，如何打断点以及进入某个具体的栈空间
1.首先找到正在运行的程序的进程号: ps aux | grep 文件名
2.挂载到该进程：启动gdb时直接挂载==> gdb -p 12345
               先启动gdb,再挂载===> gdb
                                   attach 12345
3.设置断点,继续运行停到断点处
3.bt打印调用栈，通过frame进入具体的某个栈帧

*/