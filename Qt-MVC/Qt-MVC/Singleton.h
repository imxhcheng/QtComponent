#ifndef SINGLETON_H
#define SINGLETON_H

/*
	Ô­ÎÄÁ´½Ó£ºhttps ://blog.csdn.net/panchang199266/article/details/104849986
*/

#include <QMutex>
#include <QScopedPointer>
#include <QScopedPointerDeleter>

template <typename T>
class Singleton
{
public:
	static T &getInstance();
	Singleton(const Singleton &other) = default;
	Singleton<T> &operator=(const Singleton &other) = default;

private:
	static QMutex mutex;
	static QScopedPointer<T> instance;
};

template <typename T> QMutex Singleton<T>::mutex;
template <typename T> QScopedPointer<T> Singleton<T>::instance;

template<typename T>
T &Singleton<T>::getInstance()
{
	if (instance.isNull()) {
		mutex.lock();
		if (instance.isNull()) {
			instance.reset(new T());
		}
		mutex.unlock();
	}
	return *instance.data();
}

#define SINGLETON(Class) \
    private: \
    Class(); \
    ~Class(); \
    Class(const Class &other); \
    Class &operator =(const Class &other); \
    friend class Singleton<Class>; \
    friend struct QScopedPointerDeleter<Class>;


#endif // SINGLETON_H
