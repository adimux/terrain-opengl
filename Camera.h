#ifndef CAMERA_CLASS
#define CAMERA_CLASS

#include <iostream>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "ecran.h"
#include "sdlglutils.h"

#define EN_AVANT 1
#define EN_ARRIERE -1
#define DROITE 2
#define GAUCHE -2

using namespace std;

int Traduction(SDL_KeyboardEvent *key);

class CameraFreeFly
{
	public:
		CameraFreeFly(Pos position, Pos lookAt=Pos(0, 0, 0) );
		~CameraFreeFly();

		void SDLKeys(SDL_Event *event);
		void input_handle(SDL_Event *event, Uint32 timestep);
		void changerAngles();
		void changeLookAt();
		void avancer(float distance);
		void look();
		
		void av(float distance, int direction);
		
		void setSensitivity(Speed sensitivity);
		void setSpeed(Speed speed);
		
	private:
		Pos m_position, m_lookAt, m_difference;
		
		Speed m_speed;
		Speed m_sensitivity;
		
		double m_theta;
		double m_phi, m_phi_min, m_phi_max;	
		
		bool shift_holded;
		bool *m_sdl_keys;

		float *cos_angles; // Un tableau de cosinus de tout les angles entiers entre 0 et 360
		float *sin_angles; // Un tableau de sinus de tout les angles entier entre 0 et 360
		
		Uint16 unicode_table[SDLK_LAST];
};

#endif
