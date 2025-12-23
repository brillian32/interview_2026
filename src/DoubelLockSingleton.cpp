//
// Created by 17305 on 2025/12/23.
//
#include <atomic>
#include <doctest/doctest.h>
#include <iostream>
#include <mutex>

class Singleton {
private:
    // 1. 私有化构造函数，防止外部创建
    Singleton() {
        std::cout << "Singleton Constructed" << std::endl;
    }

    // 2. 删除拷贝构造和赋值操作符，防止复制
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // 3. 静态成员变量
    // 使用 std::atomic 保证内存可见性和防止指令重排
    static std::atomic<Singleton*> instance;
    static std::mutex mtx;

public:
    // 4. 全局访问点
    static Singleton* getInstance() {
        // 第一重检查：如果不为空，直接返回，避免不必要的加锁开销
        // memory_order_acquire 保证在此之前的读写操作不会被重排到此之后
        Singleton* tmp = instance.load(std::memory_order_acquire);

        if (tmp == nullptr) {
            // 加锁
            std::lock_guard<std::mutex> lock(mtx);

            // 第二重检查：再次读取，防止在等待锁的期间已被其他线程创建
            tmp = instance.load(std::memory_order_relaxed);

            if (tmp == nullptr) {
                // 创建实例
                tmp = new Singleton();

                // 赋值给 atomic 变量
                // memory_order_release 保证构造函数执行完毕后，才能将指针赋值给 instance
                instance.store(tmp, std::memory_order_release);
            }
        }
        return tmp;
    }

    // 示例功能函数
    void doSomething() {
        std::cout << "Doing something..." << std::endl;
    }

    // 可选：实现内存回收（通常单例随程序结束由操作系统回收，但也可用内部类实现显式析构）
    class GC {
    public:
        ~GC() {
            Singleton* tmp = instance.load(std::memory_order_acquire);
            if (tmp) {
                delete tmp;
                instance.store(nullptr, std::memory_order_release);
                std::cout << "Singleton Destroyed" << std::endl;
            }
        }
    };
    static GC gc; // 静态成员的析构函数会在程序退出时被调用
};

// 静态成员初始化
std::atomic<Singleton*> Singleton::instance{nullptr};
std::mutex Singleton::mtx;
Singleton::GC Singleton::gc;

TEST_CASE("Singleton Lock"){
    Singleton* s1 = Singleton::getInstance();
    Singleton* s2 = Singleton::getInstance();

	CHECK(s1 == s2);

    s1->doSomething();
}