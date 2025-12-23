//
// Created by 17305 on 2025/12/23.
//


#include <doctest/doctest.h>

class Singleton {
public:
	// 获取单例引用的静态方法
	static Singleton& getInstance() {
		// C++11 保证：局部静态变量初始化是线程安全的
		static Singleton instance;
		return instance;
	}

	// 禁止拷贝构造和赋值操作
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	// 业务方法示例
	void doSomething() {
		// ...
		std::cout << "Singleton doing something..." << std::endl;
	}

private:
	// 构造函数私有化
	Singleton() {
		// 初始化代码
	}

	// 析构函数私有化（可选，通常设为私有以防外部 delete，但在栈上分配时不需要）
	~Singleton() = default;
};


template <typename T>
class SingletonBase {

protected:
	SingletonBase() = default;
	~SingletonBase() = default;

public:
	// 禁止拷贝和赋值
	SingletonBase(const SingletonBase&) = delete;
	SingletonBase& operator=(const SingletonBase&) = delete;

	static T& getInstance() {
		static T instance;
		return instance;
	}
};

// 使用方法：
class MyManager : public SingletonBase<MyManager> {
	// 为了让基类能访问私有构造函数
	friend class SingletonBase<MyManager>;
private:
	MyManager() { /* ... */ }

public:
	void doSomething()
	{
		std::cout << "MyManager doing something..." << std::endl;
	}
};

TEST_CASE("Singleton")
{
	// 获取单例实例
	Singleton& instance = Singleton::getInstance();
	// 使用单例实例
	instance.doSomething();

	MyManager& manager = MyManager::getInstance();
	manager.doSomething();
}