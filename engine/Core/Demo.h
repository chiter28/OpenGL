#pragma once

class Camera;
struct Event;

class Demo
{
public:
	virtual ~Demo() = default;
	
	virtual void OnAttach() {}

	virtual void OnDetach() {}

	virtual void OnUpdate(float) {}

	virtual void OnRender(const Camera&) {}

	virtual void OnEvent(Event&) {}
};