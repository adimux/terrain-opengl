#ifndef UTILS_HEADER
#define UTILS_HEADER

#include <iostream>
#include <sstream>
#include <limits>
#include <vector>
#include <fstream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

void lire_matrice(std::vector <std::vector<char> > matrice);

namespace SDL
{
	class Rect
	{
		public:
			Rect();
			Rect( double f_x, double f_y, double f_w=0, double f_h=0);
			Rect(const SDL::Rect &a_copier);
			Rect(const SDL_Rect &rect);
		
			void operator=(SDL::Rect a_copier);
			void operator=(SDL_Rect &rect);
			SDL::Rect operator+(SDL::Rect a_ajouter);
			SDL::Rect operator+(SDL_Rect &a_ajouter);
	
			void afficher();

			bool zero();
			//SDL_Rect* to_rect();
		
			float w, h;
			float x, y;
	};

	class RGB
	{
		public:
			RGB(int=0, int=0, int=0);

			Uint32 toMapRGB(SDL_PixelFormat *format);
		
			bool operator==(RGB color);
			bool operator!=(RGB color);
		
			int moyenne();
	
			int r, g, b;
	};

	SDL_Surface * LoadImage(std::string chemin_image);
	SDL_Surface * RGB_Surface(SDL::Rect dimensions, int depth=32, Uint32 Gmask=0, Uint32 Rmask=0, Uint32 Bmask=0, Uint32 Amask=0);
	SDL_Surface * copySurface(SDL_Surface *);
	
	bool FreeSurface(SDL_Surface *to_free);
	bool RotoZoomSurface(SDL_Surface *src, SDL_Surface *dest, double angle, double zoomx, double zoomy, bool anti_aliasing=false);
	bool ApplySurface(SDL_Surface *source, SDL_Surface *dest, SDL::Rect source_rect=Rect(), SDL::Rect dest_rect=Rect() );
	bool FillSurface(SDL_Surface *to_fill, RGB=RGB(255, 255, 255), SDL::Rect to_fill_rect=Rect() );
	bool SetColorKey( SDL_Surface *, RGB transparent_color );
	bool SaveBMP(SDL_Surface *, std::string chemin);

	Uint32 getPixel(SDL_Surface *surface, SDL::Rect position);
	void setPixel(SDL_Surface *surface, SDL::Rect position, Uint32 pixel);

	class Pixel
	{
		public:
			Pixel();
			Pixel(const Pixel &to_copy);
			Pixel(Uint32 pixel, SDL_PixelFormat &format);
			Pixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_PixelFormat &format);
		
			Uint32 getPixel();
			void getPixelInfo(Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a);
		
			void setPixelInfo(Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_PixelFormat &format);
			void setPixel(Uint32 pixel);
		
			void setR(Uint8 R);
			void setG(Uint8 G);
			void setB(Uint8 B);
			void setA(Uint8 A);

			Uint8 getR();
			Uint8 getG();
			Uint8 getB();
			Uint8 getA();

		private:
			Uint32 m_pixel;
			SDL_PixelFormat m_format;
	};
	
	class Surface
	{
		public:
			Surface();
			Surface(const Surface &);
			Surface(const SDL_Surface *);
			Surface(const std::string);
			Surface(SDL::Rect dimensions, int depth=32, Uint32 Gmask=0, Uint32 Rmask=0, Uint32 Bmask=0, Uint32 Amask=0);
			~Surface();
	
			bool rgb_surface(SDL::Rect dimensions, int depth=32, Uint32 Gmask=0, Uint32 Rmask=0, Uint32 Bmask=0, Uint32 Amask=0);
			bool load(std::string);
			bool apply(Surface, Rect=Rect(), Rect=Rect() ) const;
			bool fill(RGB=RGB(255, 255, 255), Rect=Rect() );
			bool setColorKey(RGB transparent_color );
			bool saveBMP(std::string chemin);
			bool flip() const; // const (à la fin de la déclaration de la fonction) Veut dire que cette fonction ne change pas les variables de la classe

			bool lock();
			bool unlock();

			void setPixel(SDL::Rect position, Pixel &pixel);
			Pixel getPixel(SDL::Rect position);
		
			void setPixelColor(SDL::Rect position, RGB color);
			RGB getPixelColor(SDL::Rect position);
			Uint8 getPixelOneComposant(SDL::Rect position, char color_number);

			bool free(); // Fait un free de la surface
			
			void *pixels();
			int pitch();

			int width() const;
			int height() const;
			int BitsPerPixel() const;
			SDL_PixelFormat PixelFormat() const;
			SDL_PixelFormat *PixelFormatAddr() const;

			void copySurface(Surface);
			void copySurface(SDL_Surface *);
			bool is_null() const;
		
			bool operator=(Surface); // Ne copie pas la surface mais le pointeur de surface, attention aux confusions
			bool operator=(SDL_Surface *); // IDEM :D

			operator bool();
			bool operator==(bool) const;
			bool operator!=(bool) const;

		private:
			SDL_Surface *m_surface; // Le pointeur de SDL_Surface
			std::string m_path; // Si c'est une image loadée d'un fichier, le path n'est pas vide
	};
}

class Camera;

class MapPos
{
	public:
		MapPos( int L=0, int l=0, int rL=0, int rl=0, double plus_x=0, double plus_y=0);
		MapPos(const MapPos &a_copier);
		MapPos(int L, int l, const SDL::Rect a_copier);
		
		void add(double, double);

		friend class Camera;

		void operator=(MapPos a_copier);
		void operator=(SDL::Rect a_copier);
		void operator+=(SDL::Rect a_ajouter);
		MapPos operator+(SDL::Rect a_ajouter);
		
		SDL_Rect* to_SDL_Rect();
		SDL::Rect to_Rect();
		int getPlusX();
		int getPlusY();
		
	private:
		int m_L, m_l;
		int m_rL, m_rl;
		double m_plus_percent_x, m_plus_percent_y;
};

//char **explode(char *str, char separator, int *number_exploded);
int string_to_int(std::string nombre_str);

void aller_a_ligne(std::ifstream &fichier, int num_ligne);

#endif
