#pragma once
#include "Event.h"



class EventDispatcher
{
public:
	EventDispatcher(Event& event)
		: m_Event(event)
	{}

	/*template<typename Type, typename Func>
	bool Dispatch(const Func& func)
	{

		if (m_Event.GetEventType() == Type::GetStaticType()) {
			m_Event.m_Handled |= func(static_cast<Type&>(m_Event));
			return true;
		}
		return false;

	}*/


	template<typename T, typename Func>
	bool Dispatch(Func&& func)
	{
		// является ли Data(variant) типом Т.
		if (T* eventData = std::get_if<T>(&m_Event.Data)) 
		{
			if (!m_Event.Handled) {
				// тип возвращаемого значения функции == bool?
				if constexpr (std::is_same_v<decltype(func(*eventData)), bool>)
				{
					m_Event.Handled |= func(*eventData);
				} else {
					func(*eventData);
					m_Event.Handled = true;
				}
			}
			return true;
		}
		return false;
	}

private:
	Event& m_Event;
};

