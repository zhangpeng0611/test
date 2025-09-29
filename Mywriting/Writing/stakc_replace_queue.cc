#if 1

#include <iostream>
#include <stack>
using namespace std;

// 使用栈来实现一个队列
class MyQueue
{
public:
    void Enqueue(int val)
    {
        inst.push(val);
    }

    void Dequeue()
    {
        if (!outst.empty())
        {
            // 出口栈不为空，则可以直接
            outst.pop();
        }
        else
        {
            // 出口栈为空，则需要通过中间操作
            Midoperator();
            outst.pop();
        }
    }

    int Peek()
    {
        if (!outst.empty())
        {
            return outst.top();
        }
        else
        {
            Midoperator();
            return outst.top();
        }
    }

private:
    stack<int> inst;
    stack<int> outst;

    void Midoperator()
    {
        if (outst.empty())
        {
            // 循环将入口栈的数据拿出来并且压入到出口栈中
            while (!inst.empty())
            {
                int tmep = inst.top();
                inst.pop();
                outst.push(tmep);
            }
        }
    }
};

int main()
{
    MyQueue mq;
    mq.Enqueue(1);
    mq.Enqueue(2);
    mq.Enqueue(3);
    mq.Enqueue(4);
    mq.Enqueue(5);

    cout << "入队了1,2,3,4,5五个元素,其队首元素为:" << mq.Peek() << endl;

    mq.Dequeue();
    cout << "出队一个元素后，队首的元素为:" << mq.Peek()<<endl;

    return 0;
}

#endif