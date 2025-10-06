#include <iostream>
#include <cstddef> // For std::size_t
#include <cstring> // For std::memset

// 内存池类
class MemoryPool {
private:
    // 空闲块结构体
    // 这个结构体本身会占用一些内存（即“管理开销”）
    struct FreeBlock {
        std::size_t size;       // 块的大小
        FreeBlock* next;        // 指向下一个空闲块
    };

public:
    /**
     * @brief 构造函数，初始化内存池
     * @param poolSize 内存池的总大小（字节）
     */
    explicit MemoryPool(std::size_t poolSize) : totalSize(poolSize) {
        // 1. 向操作系统申请一块连续的内存
        //    这块内存需要同时容纳管理结构和用户数据
        rawMemory = new char[poolSize];

        // 2. 将整块内存初始化为一个大的空闲块
        //    空闲块的大小 = 总大小 - FreeBlock结构体本身的大小
        initialFreeBlock.size = poolSize - sizeof(FreeBlock);
        initialFreeBlock.next = nullptr;

        // 3. 将这个初始空闲块作为空闲链表的头
        freeList = &initialFreeBlock;

        std::cout << "MemoryPool created with size: " << poolSize << " bytes." << std::endl;
    }

    /**
     * @brief 析构函数，释放内存池
     */
    ~MemoryPool() {
        delete[] rawMemory;
        std::cout << "MemoryPool destroyed. " << totalSize << " bytes returned to OS." << std::endl;
    }

    /**
     * @brief 从内存池中分配内存
     * @param size 需要分配的内存大小（字节）
     * @return 指向分配内存的指针，如果失败则返回 nullptr
     */
    void* allocate(std::size_t size) {
        // 加上管理结构的开销，因为我们需要在释放时知道块的大小
        const std::size_t allocSize = size + sizeof(FreeBlock);

        FreeBlock* prev = nullptr;
        FreeBlock* curr = freeList;

        // 遍历空闲链表，寻找第一个足够大的块 (First-Fit)
        while (curr != nullptr) {
            // 如果找到一个足够大的块
            if (curr->size >= allocSize) {
                // 1. 从空闲链表中移除这个块
                if (prev == nullptr) {
                    freeList = curr->next; // 如果是头节点
                } else {
                    prev->next = curr->next;
                }

                // 2. 检查是否需要分裂
                //    剩余空间 > sizeof(FreeBlock) 才值得分裂，否则就把整块给用户
                if (curr->size - allocSize > sizeof(FreeBlock)) {
                    // 计算新空闲块的起始地址
                    FreeBlock* newBlock = reinterpret_cast<FreeBlock*>(reinterpret_cast<char*>(curr) + allocSize);
                    newBlock->size = curr->size - allocSize;
                    newBlock->next = curr->next; // 新块的next指向原块的next

                    // 将新块插入回空闲链表
                    if (prev == nullptr) {
                        freeList = newBlock;
                    } else {
                        prev->next = newBlock;
                    }
                }

                // 3. 返回给用户的是数据区的指针，而不是FreeBlock的指针
                //    所以需要将指针向后移动 sizeof(FreeBlock)
                void* userPtr = reinterpret_cast<void*>(reinterpret_cast<char*>(curr) + sizeof(FreeBlock));
                
                std::cout << "Allocated " << size << " bytes. Pool remaining: " << getFreeSize() << " bytes." << std::endl;
                return userPtr;
            }

            prev = curr;
            curr = curr->next;
        }

        // 如果找不到足够大的块
        std::cerr << "Allocation failed! Not enough memory in pool for " << size << " bytes." << std::endl;
        return nullptr;
    }

    /**
     * @brief 释放先前分配的内存
     * @param ptr 指向要释放的内存的指针
     */
    void deallocate(void* ptr) {
        if (ptr == nullptr) {
            return;
        }

        // 1. 从用户指针回溯到 FreeBlock 的起始地址
        FreeBlock* blockToFree = reinterpret_cast<FreeBlock*>(reinterpret_cast<char*>(ptr) - sizeof(FreeBlock));

        // 2. 将释放的块插入到空闲链表的头部（简单高效）
        blockToFree->next = freeList;
        freeList = blockToFree;

        // 3. 合并相邻的空闲块，防止碎片
        mergeFreeBlocks();

        std::cout << "Deallocated " << blockToFree->size << " bytes. Pool remaining: " << getFreeSize() << " bytes." << std::endl;
    }

    /**
     * @brief 获取当前内存池的总大小
     */
    std::size_t getTotalSize() const {
        return totalSize;
    }

    /**
     * @brief 计算并返回当前空闲内存的总量
     */
    std::size_t getFreeSize() const {
        std::size_t freeSize = 0;
        FreeBlock* curr = freeList;
        while (curr != nullptr) {
            freeSize += curr->size;
            curr = curr->next;
        }
        return freeSize;
    }

private:
    char* rawMemory;           // 指向从OS申请的原始内存块
    std::size_t totalSize;     // 内存池总大小
    FreeBlock initialFreeBlock;// 初始的、静态的空闲块，用于管理
    FreeBlock* freeList;       // 空闲块链表的头指针

    /**
     * @brief 合并所有相邻的空闲块
     */
    void mergeFreeBlocks() {
        FreeBlock* curr = freeList;
        while (curr != nullptr && curr->next != nullptr) {
            // 检查当前块和下一个块是否相邻
            // 当前块的末尾地址 == 下一个块的起始地址
            if (reinterpret_cast<char*>(curr) + curr->size + sizeof(FreeBlock) == reinterpret_cast<char*>(curr->next)) {
                // 合并块
                curr->size += curr->next->size + sizeof(FreeBlock);
                curr->next = curr->next->next;
            } else {
                // 如果不相邻，继续向后查找
                curr = curr->next;
            }
        }
    }
};

// --- 使用示例 ---
struct MyData {
    int id;
    double value;
    char name[32];
};

int main() {
    // 创建一个 1MB 的内存池
    const std::size_t poolSize = 1024 * 1024; // 1MB
    MemoryPool pool(poolSize);

    std::cout << "Initial free memory: " << pool.getFreeSize() << " bytes." << std::endl;

    // 1. 分配一个 MyData 结构
    MyData* data1 = static_cast<MyData*>(pool.allocate(sizeof(MyData)));
    if (data1) {
        data1->id = 1;
        data1->value = 3.14;
        std::strcpy(data1->name, "Object 1");
        std::cout << "Created: " << data1->name << std::endl;
    }

    // 2. 分配一个大数组
    const std::size_t arraySize = 100;
    int* bigArray = static_cast<int*>(pool.allocate(arraySize * sizeof(int)));
    if (bigArray) {
        for (std::size_t i = 0; i < arraySize; ++i) {
            bigArray[i] = static_cast<int>(i);
        }
        std::cout << "Created array of " << arraySize << " integers." << std::endl;
    }

    // 3. 分配一个小块内存
    char* smallBuffer = static_cast<char*>(pool.allocate(16));
    if (smallBuffer) {
        std::strcpy(smallBuffer, "Hello, Pool!");
        std::cout << "Buffer content: " << smallBuffer << std::endl;
    }

    std::cout << "\nMemory status before deallocation:" << std::endl;
    std::cout << "Total size: " << pool.getTotalSize() << " bytes." << std::endl;
    std::cout << "Free size:  " << pool.getFreeSize() << " bytes." << std::endl;

    std::cout << "\n--- Deallocating ---" << std::endl;

    // 4. 释放数组（它和 smallBuffer 不相邻）
    if (bigArray) {
        pool.deallocate(bigArray);
    }

    // 5. 释放 smallBuffer（它和 data1 不相邻）
    if (smallBuffer) {
        pool.deallocate(smallBuffer);
    }

    // 6. 释放 data1
    if (data1) {
        pool.deallocate(data1);
    }

    std::cout << "\nMemory status after all deallocations:" << std::endl;
    std::cout << "Total size: " << pool.getTotalSize() << " bytes." << std::endl;
    std::cout << "Free size:  " << pool.getFreeSize() << " bytes." << std::endl;

    // 注意：在实际应用中，你需要确保所有分配的内存都在内存池销毁前被释放。
    // 这里我们手动释放了，所以析构时不会有问题。

    return 0;
}