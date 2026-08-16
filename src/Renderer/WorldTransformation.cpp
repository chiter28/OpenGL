#include "WorldTransformation.h"


void WorldTransformation::SetScale(vec3 scale)
{
	m_Scale = scale;
}

void WorldTransformation::SetRotation(vec3 anglesDegree)
{
	m_Rotation = anglesDegree;
}

void WorldTransformation::SetPosition(vec3 position)
{
	m_Position = position;
}


void WorldTransformation::Rotate(vec3 anglesDegree)
{
	m_Rotation.x += anglesDegree.x;
	m_Rotation.y += anglesDegree.y;
	m_Rotation.z += anglesDegree.z;
}

mat4 WorldTransformation::GetMatrix() const
{
	mat4 Scale;
	Scale.scale(m_Scale);

	mat4 Rotation;
	Rotation.rotate(m_Rotation);

	mat4 Translation;
	Translation.translate(m_Position);

	return (Translation * Rotation * Scale);
}





