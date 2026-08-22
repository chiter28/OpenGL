#pragma once

#include <cstdint>
#include <variant>

namespace Mod
{
	enum : uint8_t
	{
		Shift      = 1 << 0,
		Ctrl       = 1 << 1,
		Alt        = 1 << 2,
		Super      = 1 << 3,
		CapsLock   = 1 << 4,
		NumLock    = 1 << 5

	};

	inline bool IsShift(uint8_t mod) { return (mod & Shift) != 0; }
	inline bool IsCtrl(uint8_t mod) { return (mod & Ctrl) != 0; }
	inline bool IsAlt(uint8_t mod) { return (mod & Alt) != 0; }
}

struct WindowResize
{
	int32_t Width, Height;
};

struct WindowClose {};

struct WindowFocus
{ 
	bool Focused;
};


struct KeyPress
{
	int32_t KeyCode;
	bool IsRepeat;
	uint8_t Mods = 0;
};

struct KeyRelease
{
	int32_t KeyCode;
	uint8_t Mods = 0;
};

struct MousePress
{
	int32_t Button;
	uint8_t Mods = 0;
};

struct MouseRelease
{
	int32_t Button;
	uint8_t Mods = 0;
};	

struct MouseMove
{
	float X, Y;
};

struct MouseScroll
{
	float XOffset, YOffset;
};



using EventData = std::variant<
	WindowResize,
	WindowClose,
	WindowFocus,
	KeyPress,
	KeyRelease,
	MousePress,
	MouseRelease,
	MouseMove,
	MouseScroll
>;

struct Event
{
	EventData Data;
	bool Handled = false;
};






