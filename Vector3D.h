#ifndef CLASS_VECTOR_3D
#define CLASS_VECTOR_3D

#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include "Utils.h"

template<class T>
class Vector3D
{
	public:
		Vector3D(T _x = (T)0, T _y = (T)0, T _z = (T)0) : x(_x), y(_y), z(_z)
		{
		}

		template<class Y>
		Vector3D(Vector3D<Y> &to_copy) : x(to_copy.x), y(to_copy.y), z(to_copy.z)
		{
		}
		
		~Vector3D()
		{
		}

		Vector3D VectorialProd(Vector3D vector)
		{
			return Vector3D(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x);
		
		}
		float ScalarProd(Vector3D vector)
		{
			return Vector3D(x * vector.x + y * vector.y + z * vector.z);
		}
		
		float Distance(Vector3D B)
		{
			return sqrt( (x - B.x) * (x - B.x)+(y - B.y) * (y - B.y)+(z - B.z) * (z - B.z) );
		}
		
		Vector3D InSameLine(Vector3D C, float distance) // Cette fonction recherche un vecteur B appartennant à la droite AC (A est ce Vector3D et C est l'autre vecteur déterminant la droite avec A)
		// La distance est celle entre A et le point recherché (la distance est par rapport a AC)
		{
			Vector3D B; // Le vecteur qu'on recherche est B qui appartient à la droite AC
			
			float AC = Distance(C);
			
			if(AC == 0) // Si la distance est 0 donc les deux vecteurs sont les mêmes...
				return C; // ...et par conséquent on renvoie l'un deux
			
			B.x = (float)x + ( ((float)C.x - (float)x) / AC) * (distance*AC);
			B.y = (float)y + ( ((float)C.y - (float)y) / AC) * (distance*AC);
			B.z = (float)z + ( ((float)C.z - (float)z) / AC) * (distance*AC);
			
			//std::cout << "FOEL : " << AC << std::endl;
			return B;
		}
		
		void Normalize()
		{
			float longueur_vector = sqrt( x*x + y*y + z*z );
			
			x /= longueur_vector;
			y /= longueur_vector;
			z /= longueur_vector;
		}
		
		void draw(SDL::RGB color)
		{
			glLineWidth(10);
			
			glDisable(GL_TEXTURE_2D);
			
			glBegin(GL_LINES);
				glColor3ub(color.r, color.g, color.b);
				
				glVertex3f(0, 0, 0);
				glVertex3f(x, y, z);
				
				glVertex3f(x, y, z);
				glVertex3f(x-1, y, z+1);
				
				glVertex3f(x, y, z);
				glVertex3f(x-1, y, z-1);
			glEnd();
			
			glEnable(GL_TEXTURE_2D);
		}
		
		void operator=(Vector3D to_copy)
		{
			x = to_copy.x;
			y = to_copy.y;
			z = to_copy.z;
		}
		
		Vector3D operator+(Vector3D vector)
		{
			Vector3D resultat(x+vector.x, y+vector.y, z+vector.z);
		}
		
		Vector3D operator-(Vector3D vector)
		{
			Vector3D resultat(x-vector.x, y-vector.y, z-vector.z);
		}
		
		void operator-=(Vector3D vector)
		{
			x -= vector.x;
			y -= vector.y;
			z -= vector.z;
		}
		
		void operator+=(Vector3D vector)
		{
			x += vector.x;
			y += vector.y;
			z += vector.z;
		}
		
		Vector3D operator/(T nbr)
		{
			return Vector3D(x/nbr, y/nbr, z/nbr);
		}
		
		Vector3D operator*(T nbr)
		{
			return Vector3D(x*nbr, y*nbr, z*nbr);
		}
		
		void operator/=(T nbr)
		{
			x /= nbr;
			y /= nbr;
			z /= nbr;
		}
		
		void operator*=(T nbr)
		{
			x *= nbr;
			y *= nbr;
			z *= nbr;
		}
	
		T x, y, z;
};

#endif
