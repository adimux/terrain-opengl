#ifndef CLASS_LIGHT_H
#define CLASS_LIGHT_H

#include <iostream>
#include "Vector3D.h"

#define Light_AmbientLight 0
#define Light_DiffuseLight 1

typedef float Type;

class Light : public Vector3D<Type>
{
	public:
		Light(Type _x = 0, Type _y = 0, Type _z = 0, float adouc=1.0, unsigned char type=Light_AmbientLight);
		
		template<class C>
		Light(const Vector3D<C> &to_copy)
		{
			x = to_copy.x;
			y = to_copy.y;
			z = to_copy.z;
	
			m_adouc = 1.0;
			m_type = Light_AmbientLight;
		}
		
		~Light();
		
		void operator=(Light);
		
		float get_adouc();
		void set_adouc(float new_adouc);
		
		unsigned char get_type();
		void set_type(unsigned char new_type);
	
	protected:
		float m_adouc; // Facteur d'adoucissement de la lumière
		unsigned char m_type;
};

#endif
