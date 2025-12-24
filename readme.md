# All of Interview

## 高频题

### 排序

1. 快速排序
2. 归并排序

### 链表

[反转链表](src/list.cpp)

### 树
#### 二叉树遍历
层序遍历
前中后序遍历

### 基础轮子

1. [手写String类](src/MyString.cpp)
2. [手写线程池](src/ThreadPool.cpp)
3. [单例模式优雅实现](src/Singleton.cpp) / [双重检查实现](src/DoubelLockSingleton.cpp)
4. [内存相关函数(memcpy / memmove)](src/mem.cpp)

   `  memcpy (Memory Copy) `
    - 原型：void *memcpy(void *dest, const void *src, size_t n);
    - 功能：从源内存地址 src 复制 n 个字节到目标内存地址 dest。
    - 特点：不处理内存重叠（Overlap）。如果 src 和 dest 有重叠区域，行为是**未定义（Undefined Behavior**的。
    - 速度：通常比 memmove 快一点点（因为它**`不需要检查重叠`**，且可以进行激进的硬件优化）。
    
   `  memmove (Memory Move)`
    - 原型：void *memmove(void *dest, const void *src, size_t n);
    - 功能：同上，从 src 复制 n 个字节到 dest。
    - 特点：**`安全处理内存重叠`**。它保证复制结果是正确的，表现得就像是先将 src 复制到一个临时缓冲区，再从临时缓冲区复制到 dest
      一样（实际上通常通过控制复制方向来实现，不分配额外内存）。
    - 注意：尽管名字叫 move，但源数据不会被清空或删除，它本质上还是复制。
5. 手写智能指针

[LRU Cache (LeetCode 146) - 必会](src/LRU.cpp)

1. 快速排序 & 归并排序 - 必背。
2. 反转链表 (LeetCode 206) - 必须 1 分钟内写完且 0 Bug。
3. C++ String 类实现 - 必须掌握深拷贝和 Move 语义。
4. 单例模式 - 必须掌握线程安全写法。
   把这 5 个搞定，心里就有底了，然后再去刷 Hot 100 中的 Tree 和 DP 题目。

![img.png](img.png)

![img_1.png](img_1.png)