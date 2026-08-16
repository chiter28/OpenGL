#include "Utils.h"



class WorldTransformation
{
public:
	//void SetScale(float scale);
	void SetRotation(vec3 anglesDegree);
	void SetPosition(vec3 position);
	void SetScale(vec3 scale);

	void Rotate(vec3 anglesDegree);


	mat4 GetMatrix() const;

private:
	vec3 m_Scale = vec3(1.0f, 1.0f, 1.0f);
	vec3 m_Rotation = vec3(0.0f, 0.0f, 0.0f);
	vec3 m_Position = vec3(0.0f, 0.0f, 0.0f);
};