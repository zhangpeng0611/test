
#if 0

学习网站：https://blog.csdn.net/nihao_2014/article/details/131208348

lock_guard与unique_lock的区别：
lock_guard是一种轻量级的锁，当创建lock_guard之后，mutex就会被锁定，不支持手动释放锁，出
作用域之后自动释放锁。
unique_lock是重锁，可以手动释放锁，实现锁的延迟锁定和重新锁定，具有比较大的灵活性，
C++ 条件变量（std::condition_variable）的 wait() 方法要求传入 std::unique_lock，
cond.wait(lock,...),执行到wait函数的时候会释放锁，此时会进入阻塞状态，其他线程会获得锁，只有当其他线程notify_all或者
notify_one的时候才会唤醒，然后检查第二个参数是否为true，为true则继续向下执行，为false则继续阻塞

为什么在条件变量调用wait函数的时候必须传入的是uniqu_lock，而不是lock_guard:
因为wait函数的内部逻辑是：
1.先检查条件，如果不满足就释放锁，进入等待状态
2.当被唤醒时重新获得锁，再判断条件，这一步是阻塞的，需要与其他可能竞争该锁的线程
（包括被唤醒的其他线程或新线程）进行竞争。只有成功获取锁后，才会进入下一步，并且这一步的获取锁可能会
是失败的，因为要与其他线程竞争锁。
在这个过程中锁需要被释放然后再重新被获取，lock_guard不支持手动解锁,所有不能出传入lock_guard

需要注意的是，当第一次进入wait的时候是会先检查等待条件，只要等待条件满足就会被唤醒，第一次不需要notify就能
被唤醒

#endif

/*

#include <condition_variable>

std::condition_variable cv;
bool ready = false;

void consumer() {
    std::unique_lock<std::mutex> lock(mtx);
    // 等待条件满足（内部会解锁，被唤醒后重新加锁）
    cv.wait(lock, [] { return ready; });
    // 条件满足后执行操作
    shared_data = 1;
}

void producer() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    } // 解锁，让消费者可以获取锁
    cv.notify_one(); // 唤醒消费者
}

*/