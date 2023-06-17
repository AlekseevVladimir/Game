#pragma once

#include <map>
#include <memory>
#include <utility>

template<typename... Args>
class ICallback;
template<typename... Args>
class Callback;

template<typename... Args>
class Event 
{
public:
	void subscribe(Callback<Args...>&& callback) 
	{
		m_callbacks.insert({ m_currentHandle++, std::move(callback) });
	}

	void unsubscribe(unsigned int handle) 
	{
		auto found = m_callbacks.find(handle);
		if (found != m_callbacks.end()) 
		{
			m_callbacks.erase(found);
		}
	}

	void fire(Args&&... args) 
	{
		for (auto& callback : m_callbacks) 
		{
			callback.second(std::forward<Args>(args)...);
		}
	}

private:
	unsigned int m_currentHandle = 0;
	std::map<unsigned int, Callback<Args...>> m_callbacks;
};

template<typename... Args>
class ICallback 
{
public:
	virtual void call(Args&&... args) = 0;
};

template<typename ObjectT, typename... Args>
class MethodCallback : public ICallback<Args...> 
{
public:
	using callback_t = void (ObjectT::* )(Args...);
	MethodCallback(ObjectT* object, callback_t callback) : m_object(object), m_callback(callback) {};

	void call(Args&&... args) override 
	{
		(m_object->*m_callback)(std::forward<Args>(args)...);
	}

protected:
	ObjectT* m_object;
	callback_t m_callback;
};

template<typename... Args>
class Callback 
{
public:
	explicit Callback(std::unique_ptr<ICallback<Args...>>&& callback) : m_callback(std::move(callback)) {}

	void operator()(Args&&... args) 
	{
		m_callback->call(std::forward<Args>(args)...);
	}

private:
	std::unique_ptr<ICallback<Args...>> m_callback;
};

template<typename ObjectT, typename... Args>
Callback<Args...> getCallback(ObjectT* object, void (ObjectT::* callback)(Args...)) 
{
	return Callback<Args...>{ std::make_unique<MethodCallback<ObjectT, Args...>>(object, callback) };
}
