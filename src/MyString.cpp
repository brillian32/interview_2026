//
// Created by 17305 on 2025/12/24.
//
#include <cstring> // for strlen, strcpy
#include <doctest/doctest.h>

class String {
private:
    char* m_data;
    size_t m_len;

public:
    // 1. 普通构造函数 (Default Constructor)
    explicit String(const char* str = nullptr) {
        if (str == nullptr) {
            m_len = 0;
            m_data = new char[1]; // 即使是空串，也分配一个空间放 '\0'
            // *m_data = '\0';
        	m_data[0] = '\0';
        } else {
            m_len = std::strlen(str);
            m_data = new char[m_len + 1]; // +1 for terminator
            strcpy_s(m_data,m_len+1, str);
        }
    }

    // 2. 拷贝构造函数 (Copy Constructor) - 深拷贝
    String(const String& other) {
        m_len = other.m_len;
        m_data = new char[m_len + 1];
        std::strcpy(m_data, other.m_data);
        // std::cout << "Copy Constructor called" << std::endl;
    }

    // 3. 移动构造函数 (Move Constructor) - C++11
    // 接管资源，不进行内存拷贝
    String(String&& other) noexcept {
        m_data = other.m_data;
        m_len = other.m_len;

        // 必须将源对象置空，防止析构时释放资源
        other.m_data = nullptr;
        other.m_len = 0;
        // std::cout << "Move Constructor called" << std::endl;
    }

    // 4. 拷贝赋值运算符 (Copy Assignment)
    String& operator=(const String& other) {
        // 关键点：检查自赋值
        if (this == &other) {
            return *this;
        }

        // 释放原有内存
        delete[] m_data;

        // 分配新内存并复制
        m_len = other.m_len;
        m_data = new char[m_len + 1];
        std::strcpy(m_data, other.m_data);

        return *this;
    }

    // 5. 移动赋值运算符 (Move Assignment) - C++11
    String& operator=(String&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        // 释放自己的旧资源
        delete[] m_data;

        // 窃取资源
        m_data = other.m_data;
        m_len = other.m_len;

        // 置空源对象
        other.m_data = nullptr;
        other.m_len = 0;

        return *this;
    }

    // 6. 析构函数 (Destructor)
    ~String() {
        if (m_data) {
            delete[] m_data;
            m_data = nullptr; // 良好的编程习惯
        }
    }

    // --- 辅助功能 ---

    size_t size() const { return m_len; }
    const char* c_str() const { return m_data; }

    char& operator[](size_t index) { return m_data[index]; } // 可修改
    const char& operator[](size_t index) const { return m_data[index]; } // 只读

    // 重载输出流
    friend std::ostream& operator<<(std::ostream& os, const String& str) {
        os << str.m_data;
        return os;
    }



	//att 面试加分项：Copy-and-Swap 技术
	// 统一的赋值运算符 (取代了上面的 拷贝赋值 和 移动赋值)
	// 注意参数是传值 (String other)，这会调用构造函数（拷贝或移动）
	// att 只要你的类涉及资源管理，请务必将移动构造、移动赋值以及自定义的 swap 函数全部标记为 noexcept。
	// 在类内部添加
	// void swap(String& other) noexcept {
 //    	std::swap(m_data, other.m_data);
 //    	std::swap(m_len, other.m_len);
 //    }
 //
	// String& operator=(String other) noexcept{
 //    	// 交换 this 和 临时对象 other 的资源
 //    	swap(other);
 //    	// 函数结束时，other 析构，自动释放了 this 原有的内存
 //    	return *this;
 //    }
};

// -----------------------------------------------

class MString
{
	int m_len = 0;
	char* m_str = nullptr;
public:
	explicit MString(const char * str = nullptr)
	{
		if (str == nullptr) {
			m_len = 0;
			m_str = new char[1];
			m_str[0] = '\0';
		}
		else {
			m_len = strlen(str);
			m_str = new char[m_len+1];
			strcpy(m_str,str);
		}
	}

	MString(const MString & other)
	{
		m_len = other.m_len;
		m_str = new char[m_len+1];
		strcpy(m_str,other.m_str);
	}

	MString(MString&& other) noexcept
  	{
		m_len = other.m_len;
		m_str = other.m_str;

		other.m_str = nullptr;
		other.m_len = 0;
	}

	MString& operator=(const MString& other)
	{
		if (&other==this) {
			return *this;
		}

		delete[] m_str;

		m_str = new char[other.m_len+1];
		strcpy(m_str,other.m_str);
		m_len = other.m_len;
		return *this;
	}

	MString& operator=(MString&& other) noexcept
	{
		if (&other==this) {
			return *this;
		}

		delete[] m_str;

		m_str = other.m_str;
		m_len = other.m_len;

		other.m_str = nullptr;
		other.m_len = 0;

		return *this;
	}

	~MString()
	{
		if (m_str) {
			delete[] m_str;
			m_str = nullptr;
		}
		// m_len = 0;
	}
};


// 测试代码
TEST_CASE("my string") {
    String s1("Hello");
    String s2 = s1;             // 调用拷贝构造
    String s3("World");
	s3[2]='R';
	std::cout << "s3: " << s3 << std::endl;
    s3 = s1;                    // 调用拷贝赋值

    String s4 = std::move(s1);  // 调用移动构造 (s1 变为空)
    s2 = std::move(s3);         // 调用移动赋值

    std::cout << "s2: " << s2 << std::endl;
    std::cout << "s4: " << s4 << std::endl;
}



