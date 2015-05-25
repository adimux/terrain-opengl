#include "Utils.h"

using namespace std;
using namespace SDL;

void lire_matrice(vector <vector<char> > matrice)
{
	for(int i = 0; i < matrice.size(); i++)
	{
		for(int j = 0; j < matrice.front().size(); j++)
		{
			cout << (char)matrice[i][j];
		}
			
		cout << endl;
	}	
}

namespace SDL
{
	/* ************** */
	/* La classe Rect */
	/* ************** */

	Rect::Rect()
	{
		w = 0;
		h = 0;
		x = 0;
		y = 0;
	}

	Rect::Rect(double f_x, double f_y, double f_w, double f_h)
	{
		x = f_x;
		y = f_y;
		w = f_w;
		h = f_h;
	}

	Rect::Rect(const Rect &a_copier)
	{
		x = a_copier.x;
		y = a_copier.y;
		w = a_copier.w;
		h = a_copier.h;
	}

	Rect::Rect(const SDL_Rect &rect)
	{
		x = rect.x;
		y = rect.y;
		w = rect.w;
		h = rect.h;
	}

	void Rect::operator=(Rect a_copier)
	{
		x = a_copier.x;
		y = a_copier.y;
		w = a_copier.w;
		h = a_copier.h;
	}

	void Rect::operator=(SDL_Rect &rect)
	{
		x = rect.x;
		y = rect.y;
		w = rect.w;
		h = rect.h;
	}

	Rect Rect::operator+(Rect a_ajouter)
	{
		Rect to_return;
		to_return = *this;
	
		to_return.x += a_ajouter.x;
		to_return.y += a_ajouter.y;
		to_return.w += a_ajouter.w;
		to_return.h += a_ajouter.h;
	
		return to_return;
	}

	Rect Rect::operator+(SDL_Rect &a_ajouter)
	{
		Rect to_return;
		to_return = *this;
	
		to_return.x += a_ajouter.x;
		to_return.y += a_ajouter.y;
		to_return.w += a_ajouter.w;
		to_return.h += a_ajouter.h;
	
		return to_return;
	}

	void Rect::afficher()
	{
		cout << "Rect :" << endl << " x : " << x << " ; y : " << y << " ; w : " << w << " ; h : " << h << endl;
	}

	bool Rect::zero()
	{
		if(x == 0 && y == 0 && w == 0 && h == 0)
	    	return true;
	    
		return false;
	}

	SDL_Surface *LoadImage(string chemin_image)
	{
		SDL_Surface *image;
		image = IMG_Load( chemin_image.c_str() );
		
		if(image == NULL)
		{
			cerr << "Error in loading an image : " << chemin_image << "." << endl;

			return NULL;
		}
		
		SDL_Surface *to_return = SDL_DisplayFormat(image);
		
		cerr << chemin_image << " loaded Succefelly." << endl;
		
		SDL::FreeSurface(image);
		
		return to_return;
	}

	/* ************** */
	/* La classe RGB */
	/* ************** */

	RGB::RGB(int rouge, int vert, int bleu)
	{
		r = rouge;
		g = vert;
		b = bleu;
	}

	Uint32 RGB::toMapRGB(SDL_PixelFormat *format)
	{
		return SDL_MapRGB(format, r, g, b);
	}

	bool RGB::operator==(RGB color)
	{
		if(r == color.r && g == color.g && b == color.b)
			return true;
		
		return false;	
	}
	
	bool RGB::operator!=(RGB color)
	{
		if(r == color.r && g == color.g && b == color.b)
			return false;
		
		return true;
	}

	int RGB::moyenne()
	{
		return (r + g + b) / 3;
	}

	/* *************************** */
	/* Des fonctions puissantes :p */
	/* *************************** */

	bool RotoZoomSurface(SDL_Surface *src, SDL_Surface *dest, double angle, double zoomx, double zoomy, bool anti_aliasing)
	{	/*
		//SDL_Surface *zoomed = rotozoomSurfaceXY(src, angle, zoomx, zoomy, (int)anti_aliasing);
		SDL_Surface *zoomed = NULL;	
		
		if(dest != NULL)
			SDL_FreeSurface(dest);
			
		if(zoomed != NULL)
			dest = zoomed;
		else
			return false;
		
		return true;*/
		return true;
	}

	bool ApplySurface(SDL_Surface *source, SDL_Surface *dest, Rect source_rect, Rect dest_rect)
	{
		if(source == NULL || dest == NULL)
		{
			cerr << "Tentative of blitting an empty surface or in an empty surface. Error..." << endl;

			return false;
		}

	    SDL_Rect *source_SDL_rect = new SDL_Rect;
     	source_SDL_rect->x = (Uint16)source_rect.x;
	    source_SDL_rect->y = (Uint16)source_rect.y;
     	source_SDL_rect->w = (Uint16)source_rect.w;
		source_SDL_rect->h = (Uint16)source_rect.h;
     	
     	if(source_rect.zero() )
     	{
			delete source_SDL_rect;
			source_SDL_rect = NULL;
		}
     	
		SDL_Rect *dest_SDL_rect = new SDL_Rect;
		dest_SDL_rect->x = (Uint16)dest_rect.x;
	    dest_SDL_rect->y = (Uint16)dest_rect.y;
     	dest_SDL_rect->w = (Uint16)dest_rect.w;
	    dest_SDL_rect->h = (Uint16)dest_rect.h;
			
     	if(dest_rect.zero() )
     	{
			delete dest_SDL_rect;
			
			dest_SDL_rect = NULL;
		}

		if( SDL_BlitSurface(source, source_SDL_rect, dest, dest_SDL_rect ) != 0 )
		{
			if(source_SDL_rect )
				delete source_SDL_rect;
			if(dest_SDL_rect )			
				delete dest_SDL_rect;

			cerr << "Error in blitting a surface" << endl;
	
			return false;
		}
		
		if(source_SDL_rect )
			delete source_SDL_rect;
		if(dest_SDL_rect )
			delete dest_SDL_rect;
		
		return true;
	}

	bool FillSurface(SDL_Surface *to_fill, RGB color, Rect to_fill_rect)
	{
		if(to_fill == NULL)
			return false;

		SDL_Rect *to_fill_SDL_rect = new SDL_Rect;
		to_fill_SDL_rect->x = (Sint16)to_fill_rect.x;
		to_fill_SDL_rect->y = (Sint16)to_fill_rect.y;
		to_fill_SDL_rect->w = (Sint16)to_fill_rect.w;
		to_fill_SDL_rect->h = (Sint16)to_fill_rect.h;

		if(to_fill_rect.zero() )
		{
			delete to_fill_SDL_rect;
			
			to_fill_SDL_rect = NULL;
		}

		if( SDL_FillRect(to_fill, to_fill_SDL_rect, SDL_MapRGB(to_fill->format, color.r, color.g, color.b) ) != 0 )
		{
			if(to_fill_SDL_rect != NULL)
				delete to_fill_SDL_rect;
			
			cerr << "Error in filling a surface." << endl;
			return false;
		}
	
		if(to_fill_SDL_rect != NULL)
		    delete to_fill_SDL_rect;
	
		return true;
	}

	SDL_Surface* RGB_Surface(Rect dimensions, int depth, Uint32 Gmask, Uint32 Rmask, Uint32 Bmask, Uint32 Amask)
	{
		if(dimensions.w == 0 || dimensions.h == 0)
			return NULL;
		
		SDL_Surface *toReturn = SDL_CreateRGBSurface(SDL_HWSURFACE, (Uint32)dimensions.w, (Uint32)dimensions.h, depth, Gmask, Rmask, Bmask, Amask);
	
		if(toReturn == NULL)
			cerr << "Error in creating a surface." << endl;
	
		return toReturn;
	}

	SDL_Surface *copySurface(SDL_Surface *to_copy)
	{
		cerr << "COPYING A SURFACE" << endl;
		SDL_Surface *toReturn = SDL_DisplayFormatAlpha(to_copy);

		if(toReturn == NULL)
			cerr << "Error in making a copy of a surface." << endl;

		return toReturn;
	}

	bool SetColorKey( SDL_Surface *surface, RGB transparent_color )
	{
		if(surface == NULL)
			return false;

		if( SDL_SetColorKey(surface, SDL_SRCCOLORKEY | SDL_RLEACCEL, transparent_color.toMapRGB(surface->format) ) != 0)
		{
			cerr << "Error in setting transpareance to a surface." << endl;
			return false;
		}

		return true;
	}
	
	bool SaveBMP(SDL_Surface *to_save, std::string chemin)
	{
		if(to_save == NULL)
			return false;
			
		if( SDL_SaveBMP(to_save, chemin.c_str() ) != 0)
			return false;
			
		return true;
	}

	bool FreeSurface(SDL_Surface *to_free)
	{
		if(to_free == NULL)
			return false;

		SDL_FreeSurface(to_free);

		return true;
	}

	/* ********************************************************************* */
	/* getPixel : permet de récupérer la couleur d'un pixel
	Paramètres d'entrée/sortie :
		SDL_Surface *surface : la surface sur laquelle on va récupérer la couleur d'un pixel
		position : les coordonnées du pixel à récupérer

		Uint32 resultat : la fonction renvoie le pixel aux coordonnées (x, y) dans la surface
	*/

	Uint32 getPixel(SDL_Surface *surface, Rect position)
	{
	    /*nbOctetsParPixel représente le nombre d'octets utilisés pour stocker un pixel.
    	En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur
	    de l'image : 8, 16, 24 ou 32 bits.*/
	    int nbOctetsParPixel = surface->format->BytesPerPixel;
    	/* Ici adresse_pixel est l'adresse du pixel que l'on veut connaitre */
	    /* surface->pixels contient l'adresse du premier pixel de l'image donc si on ajoute y * surface->pitch qui veut dire [nbOctetsParPixel] * [largeur de la surface] */
    	Uint8 *adresse_pixel = (Uint8 *)surface->pixels + (int)position.y * surface->pitch + (int)position.x * nbOctetsParPixel;

	    /*Gestion différente suivant le nombre d'octets par pixel de l'image*/
	    switch(nbOctetsParPixel)
    	{
	        case 1:
    	        return *adresse_pixel;

	        case 2: // Si c'est 2 (2 * 8 = 16 donc 16bytes alors que *adresse_pixel est un Uint32)
    	        return *(Uint16 *)adresse_pixel; // On convertit

	        case 3:
    	        /*Suivant l'architecture de la machine*/
	            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            	    return adresse_pixel[0] << 16 | adresse_pixel[1] << 8 | adresse_pixel[2];
        	    else
    	            return adresse_pixel[0] | adresse_pixel[1] << 8 | adresse_pixel[2] << 16;

	        case 4:
        	    return *(Uint32 *)adresse_pixel;

    	    /*Ne devrait pas arriver, mais évite les erreurs*/
	        default:
        	    return 0; 
    	};
	}

	/* ********************************************************************* */
	/*  setPixel : permet de modifier la couleur d'un pixel
		Paramètres d'entrée/sortie :
		SDL_Surface *surface : la surface sur laquelle on va modifier la couleur d'un pixel
		Rect position : les coordonnées en x du pixel à modifier
		Uint32 pixel : le pixel à insérer
	*/

	void setPixel(SDL_Surface *surface, Rect position, Uint32 pixel)
	{
    	/*nbOctetsParPixel représente le nombre d'octets utilisés pour stocker un pixel.
	    En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur
    	de l'image : 8, 16, 24 ou 32 bits.*/
	    int nbOctetsParPixel = surface->format->BytesPerPixel;
    	/*Ici p est l'adresse du pixel que l'on veut modifier*/
	    /*surface->pixels contient l'adresse du premier pixel de l'image*/
    	Uint8 *p = (Uint8 *)surface->pixels + (int)position.y * surface->pitch + (int)position.x * nbOctetsParPixel;

	    /*Gestion différente suivant le nombre d'octets par pixel de l'image*/
    	switch(nbOctetsParPixel)
	    {
    	    case 1:
        	    *p = pixel;
            break;

	        case 2:
    	        *(Uint16 *)p = pixel;
            break;

	        case 3:
    	        /*Suivant l'architecture de la machine*/
        	    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            	{
                	p[0] = (pixel >> 16) & 0xff;
	                p[1] = (pixel >> 8) & 0xff;
    	            p[2] = pixel & 0xff;
        	    }
            	else
	            {
    	            p[0] = pixel & 0xff;
        	        p[1] = (pixel >> 8) & 0xff;
            	    p[2] = (pixel >> 16) & 0xff;
	            }
            break;

    	    case 4:
        	    *(Uint32 *)p = pixel;
            break;
	    };
	}

	/* ************** */
	/* La class Pixel */
	/* ************** */

	Pixel::Pixel()
	{
		m_pixel = 0;
	}
	
	Pixel::Pixel(const Pixel &to_copy)
	{
		m_pixel = to_copy.m_pixel;
		m_format = to_copy.m_format;	
	}
		
	Pixel::Pixel(Uint32 pixel, SDL_PixelFormat &format)
	{
		m_pixel = pixel;
		m_format = format;
	}

	Pixel::Pixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_PixelFormat &format)
	{
		m_format = format; // Le format du pixel doit être récupéré sinon on en pourra pas le changer ni prendre les valeurs r, g, b et a
		setPixelInfo(r, g, b, a, format);
	}

	void Pixel::setPixelInfo(Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_PixelFormat &format)
	{
		m_pixel = SDL_MapRGBA(&format, r, g, b, a);
	}

	void Pixel::setPixel(Uint32 pixel)
	{
		m_pixel = pixel;
	}
		
	void Pixel::setR(Uint8 new_r)
	{
		Uint8 r, g, b, a;
	
		getPixelInfo(&r, &g, &b, &a);
		setPixelInfo(new_r, g, b, a, m_format);
	}

	void Pixel::setG(Uint8 new_g)
	{
		Uint8 r, g, b, a;
			
		getPixelInfo(&r, &g, &b, &a);	
		setPixelInfo(r, new_g, b, a, m_format);
	}

	void Pixel::setB(Uint8 new_b)
	{
		Uint8 r, g, b, a;
		
		getPixelInfo(&r, &g, &b, &a);
		setPixelInfo(r, g, new_b, a, m_format);
	}

	void Pixel::setA(Uint8 new_a)
	{
		Uint8 r, g, b, a;
		
		getPixelInfo(&r, &g, &b, &a);
		setPixelInfo(r, g, b, new_a, m_format);
	}

	Uint32 Pixel::getPixel()
	{
		return m_pixel;	
	}

	void Pixel::getPixelInfo(Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a)
	{
		SDL_GetRGBA(m_pixel, &m_format, r, g, b, a); // On extrait les valeurs de r g b et a
	}

	Uint8 Pixel::getR()
	{
		Uint8 r, g, b, a;
		
		getPixelInfo(&r, &g, &b, &a);

		return r;
	}

	Uint8 Pixel::getG()
	{
		Uint8 r, g, b, a;
		
		getPixelInfo(&r, &g, &b, &a);
	
		return g;
	}

	Uint8 Pixel::getB()
	{
		Uint8 r, g, b, a;
		
		getPixelInfo(&r, &g, &b, &a);
	
		return b;
	}

	Uint8 Pixel::getA()
	{
		Uint8 r, g, b, a;
		
		getPixelInfo(&r, &g, &b, &a);
		
		return a;
	}

	/* ***************** */
	/* La classe Surface */
	/* ***************** */

	Surface::Surface()
	{
		m_surface = NULL;
	}

	Surface::Surface(const Surface &surf)
	{
		m_surface = surf.m_surface; // On ne copie que le pointeur pas la surface sinon ça va être trop lent
	}

	Surface::Surface(const SDL_Surface *surf)
	{
		m_surface = (SDL_Surface *)surf;
	}

	Surface::Surface(const string path)
	{
		m_surface = NULL; // :D Précaution

		load(path); // Fonction membre
	}

	Surface::Surface(Rect dimensions, int depth, Uint32 Gmask, Uint32 Rmask, Uint32 Bmask, Uint32 Amask)
	{
		m_surface = NULL;

		rgb_surface(dimensions, depth, Gmask, Rmask, Bmask, Amask); // C'est une fonction membre de cette classe
	}

	Surface::~Surface()
	{
	}

	bool Surface::rgb_surface(Rect dimensions, int depth, Uint32 Gmask, Uint32 Rmask, Uint32 Bmask, Uint32 Amask )
	{
		free();
	
		m_surface = SDL::RGB_Surface(dimensions, depth, Gmask, Rmask, Bmask, Amask);
	}

	bool Surface::load(string path)
	{
		m_path = path;

		free();

		m_surface = SDL::LoadImage(path);

		if(m_surface == NULL)
			return false;

		return true;
	}

	bool Surface::apply(Surface blit_in, Rect SurfRect, Rect blit_in_rect ) const // Blitte la surface sur une autre
	{
		if( !SDL::ApplySurface(m_surface, blit_in.m_surface, SurfRect, blit_in_rect) )
			return false;

		return true;
	}

	bool Surface::fill(RGB color, Rect rect_to_fill) // "Colore" la surface
	{
		if( !SDL::FillSurface(m_surface, color, rect_to_fill) )
			return false;

		return true;
	}

	bool Surface::setColorKey(RGB transpareant_color) // Pour donner une couleur transpareante à la surface
	{
		return SDL::SetColorKey(m_surface, transpareant_color);
	}

	bool Surface::saveBMP(std::string chemin) // Pour sauvegarder la surface dans un fichier BMP
	{
		return SDL::SaveBMP(m_surface, chemin);	
	}

	bool Surface::flip() const // Flip pour indiquer à la carte graphique que la prochaine surface est prête et qu'il peut l'afficher en tant que screen
	{
		if(m_surface != NULL)
		{
			if( SDL_Flip(m_surface) != 0)
				return false;
		}

		else
			return false;

		return true;
	}

	bool Surface::lock() // Bloque la surface pour pouvoir changer ses pixels
	{
		if(m_surface != NULL)
		{
			if(SDL_LockSurface(m_surface) != 0)
				return false;
		}

		else
			return false;

		return true;
	}

	bool Surface::unlock()
	{
		if(m_surface != NULL)
			SDL_UnlockSurface(m_surface);

		else
			return false;

		return true;
	}

	void Surface::setPixel(Rect position, Pixel &pixel) // On change le pixel à la coordonnée position à un autre pixel
	{
		Uint32 new_pixel = pixel.getPixel(); // On convertit de Pixel à Uint32
	
		SDL::setPixel(m_surface, position, new_pixel); // Puis on change le pixel à la coordonnée position
	}

	Pixel Surface::getPixel(Rect position)
	{
		return Pixel(SDL::getPixel(m_surface, position), *m_surface->format ); // Retourne un objet de type Pixel
	}
		
	void Surface::setPixelColor(Rect position, RGB color)
	{
		// Cette fonction ne change que les couleurs d'un pixel mais pas son canal alpha
		
		Pixel pixel(SDL::getPixel(m_surface, position), *m_surface->format);
		Uint8 a = pixel.getA(); // Donc on doit récupérer la valeur de a
	
		Pixel new_pixel(color.r, color.g, color.b, a, *m_surface->format);
	
		SDL::setPixel(m_surface, position, new_pixel.getPixel() );
	}

	RGB Surface::getPixelColor(Rect position)
	{
		RGB pixel_color;
	
		Uint8 r, g, b, a;
	
		Pixel pixel( SDL::getPixel(m_surface, position), *m_surface->format );
	
		pixel.getPixelInfo(&r, &g, &b, &a);
	
		pixel_color.r = (int)r;
		pixel_color.g = (int)g;
		pixel_color.b = (int)b;
	
		return pixel_color;
	}
	
	Uint8 Surface::getPixelOneComposant(Rect position, char color_number)
	{
		int bytesPerPixel = m_surface->format->BytesPerPixel;
		Uint8 composant = ( (Uint8*) m_surface->pixels )[ (int)position.x * bytesPerPixel  +  (int)position.y * m_surface->pitch  +  color_number];
	
		return composant;
	}

	bool Surface::free()
	{
		if(m_surface)
			SDL::FreeSurface(m_surface); // On free la surface

		m_surface = NULL;
	}
	
	void *Surface::pixels()
	{
		return m_surface->pixels;	
	}
	
	int Surface::pitch()
	{
		return m_surface->pitch;
	}

	int Surface::width() const // Retourne la largeur de la surface
	{
		if(m_surface)
			return m_surface->w;
		else
			return -1;
	}

	int Surface::height() const
	{
		if(m_surface)
			return m_surface->h;
		else
			return -1;
	}

	int Surface::BitsPerPixel() const // Retourne les BitsPerPixel (pas bytes per pixel qui est égal à BitsPerPixel/8, attention aux confusions)
	{
		if(m_surface != NULL)
			return m_surface->format->BitsPerPixel;
		else
			return -1;
	}

	SDL_PixelFormat Surface::PixelFormat() const // Retourne le format des pixels la surface
	{
		if(m_surface != NULL)
			return *m_surface->format;
	}
	
	SDL_PixelFormat *Surface::PixelFormatAddr() const // Retourne le format des pixels la surface
	{
		if(m_surface != NULL)
			return m_surface->format;
	}

	void Surface::copySurface(Surface surface) // Copie une Surface
	{
		free();

		m_surface = SDL::copySurface(surface.m_surface);
	}

	void Surface::copySurface(SDL_Surface *surface) // Copie une SDL_Surface
	{
		free();

		m_surface = SDL::copySurface(surface);
	}

	bool Surface::is_null() const // is_null retourne true si le pointeur m_surface == NULL sinon false
	{
		if(m_surface == NULL)
			return true;

		return false;
	}

	bool Surface::operator=(Surface surf)
	{
		free();

		m_surface = surf.m_surface;
	}

	bool Surface::operator=(SDL_Surface *surf)
	{
		free();

		m_surface = surf;
	}

	Surface::operator bool()
	{
		if(m_surface == NULL)
			return false;
		else
			return true;
	}

	bool Surface::operator==(bool boolean) const // Operator== pour savoir si m_surface est NULL ou non
	{
		if(m_surface == NULL)
		{
			if(boolean == true)
				return false;
			else
				return true;
		}
		else
		{
			if(boolean == true)
				return true;
			else
				return false;
		}
	}

	bool Surface::operator!=(bool boolean) const
	{
		if(m_surface == NULL)
		{
			if(boolean == true)
				return true;
			else
				return false;
		}
		else
		{
			if(boolean == true)
				return false;
			else
				return true;
		}
	}
}

/*SDL_Rect Rect::to_SDL_rect()
{
	SDL_Rect toReturn;
	
	toReturn.x = x;
	toReturn.y = y;
	toReturn.w = w;
	toReturn.h = h;
	
	return toReturn;
}*/

/* **************** */
/* La classe MapPos */
/* **************** */

MapPos::MapPos( int L, int l, int rL, int rl, double plus_x, double plus_y)
{
	m_L = L;
	m_l = l;
	m_rL = rL;
	m_rl = rl;
	m_plus_percent_x = (int) ( (double)plus_x / (double)m_L * 100);
	m_plus_percent_y = (int) ( (double)plus_y / (double)m_L * 100);
}

MapPos::MapPos(const MapPos &a_copier)
{
	m_L = a_copier.m_L;
	m_l = a_copier.m_l;
	m_rL = a_copier.m_rL;
	m_rl = a_copier.m_rl;
	m_plus_percent_x = a_copier.m_plus_percent_x;
	m_plus_percent_y = a_copier.m_plus_percent_y;
}

MapPos::MapPos(int L, int l, const Rect a_copier)
{
	m_L = L;
	m_l = l;
	m_rL = (int)a_copier.x/m_L;
	m_rl = (int)a_copier.y/m_l;
	m_plus_percent_x = (int)a_copier.x % m_L;
	m_plus_percent_x = (int) ( (double)m_plus_percent_x / (double)m_L * 100);
	m_plus_percent_y = (int)a_copier.y % m_l;
	m_plus_percent_y = (int) ( (double)m_plus_percent_y / (double)m_l * 100);
}

void MapPos::operator=(MapPos a_copier)
{
	m_L = a_copier.m_L;
	m_l = a_copier.m_l;
	m_rL = a_copier.m_rL;
	m_rl = a_copier.m_rl;
	//cerr << a_copier.m_plus_percent_x << "  " << a_copier.m_plus_percent_y << endl;
	m_plus_percent_x = a_copier.m_plus_percent_x;
	m_plus_percent_y = a_copier.m_plus_percent_y;
}

void MapPos::operator=(Rect a_copier)
{
	m_rL = (int)a_copier.x/m_L;
	m_rl = (int)a_copier.y/m_l;
	m_plus_percent_x = (int)a_copier.x % m_L;
	m_plus_percent_x = (int) ( (double)m_plus_percent_x / (double)m_L * 100);
	m_plus_percent_y = (int)a_copier.y % m_l;
	m_plus_percent_y = (int) ( (double)m_plus_percent_y / (double)m_l * 100);
}

MapPos MapPos::operator+(Rect a_ajouter)
{
	MapPos to_return;
	to_return = *this;
	
	double plus_percent_x = (double) ( (double)a_ajouter.x / (double)m_L * (double)100 );
	double plus_percent_y = (double) ( (double)a_ajouter.y / (double)m_l * (double)100 );
	//cout << plus_percent_x << " " << plus_percent_y << endl;
	
	to_return.m_plus_percent_x += plus_percent_x;
	to_return.m_plus_percent_y += plus_percent_y;
	
	if(to_return.m_plus_percent_x > 100)
	{
		to_return.m_rL += (int) ( (double)to_return.m_plus_percent_x / (double)100 );
		to_return.m_plus_percent_x = (int)to_return.m_plus_percent_x % 100;
	}
	
	if(to_return.m_plus_percent_y > 100)
	{
		to_return.m_rl += (int) ( (double)to_return.m_plus_percent_y / (double)100 );
		to_return.m_plus_percent_y = (int)to_return.m_plus_percent_y % 100;
	}
	
	//
	
	if(to_return.m_plus_percent_x < 0)
	{
		to_return.m_rL += ( (int) ( (double)to_return.m_plus_percent_x / (double)100 ) - 1 );
		
		if(m_plus_percent_x < -100)
			to_return.m_plus_percent_x = 100 + ((int)to_return.m_plus_percent_x % 100);
		else
		{
			if(to_return.m_rL >= 0)
				to_return.m_plus_percent_x = 100 + to_return.m_plus_percent_x;
			else
			{
				to_return.m_rL = 0;
				to_return.m_plus_percent_x = 0;
			}
		}
	}
	
	if(to_return.m_plus_percent_y < 0)
	{
		to_return.m_rl += ( (int) ( (double)to_return.m_plus_percent_y / (double)100 ) + -1 );
		
		if(m_plus_percent_y < -100)
			to_return.m_plus_percent_y = 100 + ((int)to_return.m_plus_percent_y % 100);
		else
		{
			if(to_return.m_rl >= 0)
				to_return.m_plus_percent_y = 100 + to_return.m_plus_percent_y;
			else
			{
				to_return.m_rl = 0;
				to_return.m_plus_percent_y = 0;
			}
		}
	}
	
	return to_return;
}

void MapPos::add(double plus_x, double plus_y)
{
	MapPos new_pos;
	new_pos = *this;
	
	double plus_percent_x = (double) ( (double)plus_x / (double)m_L * (double)100 );
	double plus_percent_y = (double) ( (double)plus_y / (double)m_l * (double)100 );
	//cout << plus_percent_x << " " << plus_percent_y << endl;
	
	new_pos.m_plus_percent_x += plus_percent_x;
	new_pos.m_plus_percent_y += plus_percent_y;
	
	if(new_pos.m_plus_percent_x > 100)
	{
		new_pos.m_rL += (int) ( (double)new_pos.m_plus_percent_x / (double)100 );
		new_pos.m_plus_percent_x = (int)new_pos.m_plus_percent_x % 100;
	}
	
	if(new_pos.m_plus_percent_y > 100)
	{
		new_pos.m_rl += (int) ( (double)new_pos.m_plus_percent_y / (double)100 );
		new_pos.m_plus_percent_y = (int)new_pos.m_plus_percent_y % 100;
	}
	
	//
	
	if(new_pos.m_plus_percent_x < 0)
	{
		new_pos.m_rL += ( (int) ( (double)new_pos.m_plus_percent_x / (double)100 ) - 1 );
		
		if(m_plus_percent_x < -100)
			new_pos.m_plus_percent_x = 100 + ((int)new_pos.m_plus_percent_x % 100);
		else
		{
			if(new_pos.m_rL >= 0)
				new_pos.m_plus_percent_x = 100 + new_pos.m_plus_percent_x;
			else
			{
				new_pos.m_rL = 0;
				new_pos.m_plus_percent_x = 0;
			}
		}
	}
	
	if(new_pos.m_plus_percent_y < 0)
	{
		new_pos.m_rl += ( (int) ( (double)new_pos.m_plus_percent_y / (double)100 ) + -1 );
		
		if(m_plus_percent_y < -100)
			new_pos.m_plus_percent_y = 100 + ((int)new_pos.m_plus_percent_y % 100);
		else
		{
			if(new_pos.m_rl >= 0)
				new_pos.m_plus_percent_y = 100 + new_pos.m_plus_percent_y;
			else
			{
				new_pos.m_rl = 0;
				new_pos.m_plus_percent_y = 0;
			}
		}
	}
	
	*this = new_pos;
}

void MapPos::operator+=(Rect a_ajouter)
{
	*this = *this + a_ajouter;
}


Rect MapPos::to_Rect()
{
	return Rect( (m_L * m_rL) + (m_L * m_plus_percent_x/100),  (m_l * m_rl) + (m_l * m_plus_percent_y/100), m_L, m_l);
}

int MapPos::getPlusX()
{
	return (int) ( (float)m_L * (float)m_plus_percent_x / (float)100 );
}

int MapPos::getPlusY()
{
	return (int) ( (float)m_l * (float)m_plus_percent_y / (float)100 );
}

/* **************************************** */
/* Des fonctions qui nous serviront parfois */
/* **************************************** */

int string_to_int(string nombre_str)
{
	int intReturn;
  
	std::istringstream iss2(nombre_str);
	iss2 >> intReturn;

	return intReturn;
}

void aller_a_ligne(ifstream &fichier, int num_ligne)
{
	fichier.seekg(0, ios::beg);
	
	int i;
	char c;
	
	for(i = 1; i < num_ligne; i++)
	{
		do
		{
			fichier.get(c);
		}
		while( c != '\n');	
	}
}
