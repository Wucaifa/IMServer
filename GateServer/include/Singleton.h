#pragma once
#include <memory>
#include <mutex>
#include <iostream>

using namespace std;

template <typename T>
class Singleton {
protected:
	Singleton() = default;
	Singleton(const Singleton<T>&) = delete;
	Singleton& operator=(const Singleton<T>& st) = delete;

	static std::shared_ptr<T> _instance;
public:
	static std::shared_ptr<T> GetInstance() {
		static std::once_flag s_flag;
		std::call_once(s_flag, [&]() {
			_instance = shared_ptr<T>(new T);
		});

		return _instance;
	}
	void PrintAddress() {
		std::cout << _instance.get() << endl;
	}
	~Singleton() {
		std::cout << "this is singleton destruct" << std::endl;
	}
};

// 定义一个模板类Singleton，用于实现单例模式
template <typename T>
// 定义一个静态成员变量_instance，类型为std::shared_ptr<T>，用于保存单例实例
std::shared_ptr<T> Singleton<T>::_instance = nullptr;