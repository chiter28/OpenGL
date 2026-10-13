#pragma once

class Camera;
class Event;

class Demo
{
public:
	virtual ~Demo() = default;
	
	virtual void OnAttach() {}

	virtual void OnDetach() {}

	virtual void OnUpdate() {}

	virtual void OnRender(Camera& camera) {}

	virtual void OnEvent(Event& event) {}

	virtual void SetLightDirection() {};
};