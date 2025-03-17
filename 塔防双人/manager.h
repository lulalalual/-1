#pragma once

template<typename T>

class Manager
{
public:
	static T* instance()
	{
		if (!manager)
			manager = new T();
		return manager;
	}
private:
	static T* manager;

protected:
	Manager() = default;
	//构造函数放在private里面，单例模式的前置条件：类的实例不能在外部自由创建
	~Manager() = default;
	Manager(const Manager&) = delete;
	Manager& operator=(const Manager&) = delete;

};

template<typename T>
T* Manager<T>::manager = nullptr;