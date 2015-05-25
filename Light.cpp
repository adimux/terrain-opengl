#include "Light.h"

Light::Light(Type _x, Type _y, Type _z, float adouc, unsigned char type) : Vector3D<float>(_x, _y, _z), m_adouc(adouc), m_type(type)
{
}

Light::~Light()
{
}

void Light::operator=(Light to_copy)
{
	x = to_copy.x;
	y = to_copy.y;
	z = to_copy.z;
	m_type = to_copy.m_type;
	m_adouc = to_copy.m_adouc;
}

float Light::get_adouc()
{
	return m_adouc;
}

void Light::set_adouc(float new_adouc)
{
	m_adouc = new_adouc;
}

unsigned char Light::get_type()
{
	return m_type;	
}

void Light::set_type(unsigned char new_type)
{
	m_type = new_type;	
}
