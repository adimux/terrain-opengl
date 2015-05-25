#ifndef CLIP_CLASS
#define CLIP_CLASS

#include <iostream>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "Utils.h"

class Clip
{
	public:
		Clip();
		Clip(const Clip &a_copier);
		Clip(SDL::Surface *img, ...);
		Clip(char *chemin_img, ...);
		Clip(std::string chemin_feuille_sprites, int largeur_sprite, int hauteur_sprite, SDL::RGB transparent_color=SDL::RGB(255, 0, 255) );
		~Clip();
		
		int get_nbr_sprites();
		bool empty();
		
		SDL::Surface to_feuille_sprites(SDL::Rect=SDL::Rect(), int depth=32);
		
		void operator=(Clip a_copier);
		bool operator<<(SDL::Surface to_add);
		bool operator<<(std::string to_add );

		SDL::Surface get(const int number);
		SDL::Surface operator[](const int number);
	
	private:
		std::vector<SDL::Surface> m_clip; // Tableaux d'objets Surface des images du clip
};

#endif
