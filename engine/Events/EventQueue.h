#pragma once
#include "Event.h"
#include <functional>
#include <vector>
#include <memory>



class EventQueue
{
public:
	EventQueue() = default;
	using DispatchFn = std::function<void(Event&)>;



	template<typename T, typename... Args>
	void Push(Args&&... args)
	{
		m_PendingQueue.emplace_back(Event{ T{ std::forward<Args>(args)... }, false });
	}



	void Flush(const DispatchFn& dispatchFunc)
	{
		if (m_PendingQueue.empty())
			return;

		std::vector<Event> proccessingQueue;
		proccessingQueue.swap(m_PendingQueue);
		
		for (Event& event : proccessingQueue) {
			dispatchFunc(event);
		}
	}

private:
	std::vector<Event> m_PendingQueue;
};