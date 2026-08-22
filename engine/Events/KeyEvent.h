#pragma once
#include "Event.h"
//
//class KeyEvent : public Event
//{
//public:
//	inline uint32_t GetKeyCode() const { return m_KeyCode; }
//
//protected:
//	KeyEvent(uint32_t keyCode)
//		: m_KeyCode(keyCode) {}
//
//	uint32_t m_KeyCode;
//};
//
//
//
//
//class KeyPressedEvent : public KeyEvent
//{
//public:
//	KeyPressedEvent(uint32_t keyCode, uint32_t repeatCount)
//		: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}
//
//	inline uint32_t GetRepeatCount() const { return m_RepeatCount; }
//
//	static EventType GetStaticType() { return EventType::KeyPressed; }
//	EventType GetEventType() override { return GetStaticType(); }
//
//private:
//	uint32_t m_RepeatCount;
//};
//
//
//
//
//class KeyReleasedEvent : public KeyEvent
//{
//public:
//	KeyReleasedEvent(uint32_t keyCode)
//		: KeyEvent(keyCode)
//	{}
//	
//	static EventType GetStaticType() { return EventType::KeyReleased; }
//	EventType GetEventType() override { return GetStaticType(); }
//
//};
