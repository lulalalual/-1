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
	//���캯������private���棬����ģʽ��ǰ�����������ʵ���������ⲿ���ɴ���
	~Manager() = default;
	Manager(const Manager&) = delete;
	Manager& operator=(const Manager&) = delete;

};

template<typename T>
T* Manager<T>::manager = nullptr;