#include <iostream>
#include <unordered_map>
using namespace std;

struct node
{
    int key;
    int val;
    node *next;
    node *pre;
    node(int x, int y) : key(x), val(y), next(nullptr), pre(nullptr) {}
};

class LRU
{
public:
    unordered_map<int, node *> mp;
    int size;
    int capacity;
    node *tail;
    node *head;

    void moveToHead(node *nd)
    {
        remove(nd);
        insertToHead(nd);
    }

    void insertToHead(node *nd)
    {
        auto temp = head->pre;
        nd->next = head;
        head->pre = nd;
        nd->pre = temp;
        temp->next = nd;
    }

    void remove(node *nd)
    {
        nd->pre->next = nd->next;
        nd->next->pre = nd->pre;
    }

    LRU(int capacity_)
    {
        this->size = 0;
        this->capacity = capacity_;
        tail = new node(-1, -1);
        head = new node(-1, -1);
        tail->next = head;
        head->pre = tail;
    }

    int get(int key)
    {
        auto it = mp.find(key);
        if (it != mp.end())
        {
            moveToHead((*it).second);
            return (*it).second->val;
        }else
        return -1;
    }

    void put(int key, int val)
    {
        auto it = mp.find(key);
        if (it != mp.end())
        {
            mp[key]->val = val;
            moveToHead((*it).second);
        }
        else
        {
            node *tmep = new node(key, val);
            insertToHead(tmep);
            mp[key]=tmep;
            this->size++;
            if (this->size > this->capacity)
            {
                auto nd = tail->next;
                remove(nd);
                mp.erase(nd->key);
            }
        }
    }
};

int main()
{

    LRU lru(3);
    lru.put(1, 5);
    lru.put(2, 6);
    lru.put(3, 7);


    cout << "lru的大小为:" << lru.size << endl;
    cout << "查询1:" << lru.get(1) << endl;

    lru.put(4, 8);
    cout << "继续插入一个数据后" << endl;
    cout << "查询2:" << lru.get(2) << endl;
    cout << "查询4:" << lru.get(4) << endl;
    return 0;

}