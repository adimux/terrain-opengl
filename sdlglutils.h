#ifndef SDLGLUTILS_H
#define SDLGLUTILS_H

#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

using namespace std;

class Color
{
	public:
		Color();
		Color(int, int, int);
	
		int r, v, b;
};

class Pos
{
	public:
		Pos();
		Pos(const Pos &a_copier);
		Pos(double x_f, double y_f, double z_f);
		Pos difference(Pos autre_position);
		void operator=(Pos a_copier);
		
		float x, y, z;
};

class Dim3
{
	public:
		Dim3();
		Dim3(float _w, float _l, float _h);
		void operator=(Dim3 a_copier);
		
		float w, l, h;
};

class Speed
{
	public:
		Speed();
		Speed(double distance, Uint32 per);
		Speed(const Speed &a_copier);

		void start();
		void stop();
		double current_speed();
		bool increase_or_not();

		void operator=(Speed a_copier);

		double m_distance;
		Uint32 m_per_ms;

	private:
		Uint32 m_last_time;
		bool m_stopped;
};

GLuint loadTexture(SDL_Surface *picture_surface, bool useMipMap = true);
GLuint loadTexture(const char * filename,bool useMipMap = true);

int takeScreenshot(const char * filename);
void drawAxis(double scale = 1);
int initFullScreen(unsigned int * width = NULL,unsigned int * height = NULL);
int XPMFromImage(const char * imagefile, const char * XPMfile);
SDL_Cursor * cursorFromXPM(const char * xpm[]);

#endif //SDLGLUTILS_H
