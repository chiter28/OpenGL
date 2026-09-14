#pragma once

class Camera;
class Event;

class Sandbox
{
public:
	virtual ~Sandbox() = default;
	
	virtual void OnAttach() {}

	virtual void OnDetach() {}

	virtual void OnUpdate() {}

	virtual void OnRender(Camera& camera) {}

	virtual void OnEvent(Event& event) {}

	virtual void SetLightDirection() {};
};