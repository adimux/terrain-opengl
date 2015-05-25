#include "Clip.h"
#include "VectorUtil.h"

using namespace SDL;
using namespace std;

Clip::Clip()
{
}

Clip::Clip(Surface *img, ...) // Fonction à nombre variable de paramètres
{
	if( *img == true )
		m_clip.push_back(*img);
	else
		return;
	
	va_list param; // Déclaration de ap

	va_start(param, img); // Initialisation de ap
	
	bool fin = false;
	int i = 0;
	
	while(!fin)
	{
		// va_arg() donne le paramètre courant
		Surface *surface = va_arg(param, Surface* );
		
		cout << i << endl;
		
		if( *surface == true)
			m_clip.push_back(*surface);
		else
			fin = true;
		i++;
	}
		
	va_end(param); // On a fini
}

Clip::Clip(char *chemin_img, ...) // Fonction à nombre variable de paramètres
{
	if(chemin_img != NULL)
	{
		m_clip.push_back( Surface( chemin_img ) );

		if( m_clip.back() == false )
			return;
	}
	
	else
		return;
	
	va_list param; // Déclaration de ap

	va_start(param, chemin_img); // Initialisation de ap
	
	bool fin = false;

	while(!fin)
	{
		char *chemin_vers_image = va_arg(param, char*);
		
		if(chemin_vers_image)
		{
			m_clip.push_back( Surface(chemin_vers_image) );
			
			if( m_clip.back() == false )
			{
				m_clip.pop_back();

				break;
			}
		}
		
		else
			fin = true;
	}
}

Clip::Clip(string chemin_feuille_sprites, int largeur_sprite, int hauteur_sprite, RGB transparent_color)
{
	cout << "Chemin feuille de sprites : " << chemin_feuille_sprites << endl;
	Surface feuille_sprites( chemin_feuille_sprites );

	int repeat_horiz = (int) ( feuille_sprites.width() / largeur_sprite );
	int repeat_verti = (int) ( feuille_sprites.height() / hauteur_sprite );
	// Les deux variables précédentes délimitent le "tableau de sprites" à copier depuis la feuille de sprites

	Rect rect_sprite(0, 0, largeur_sprite, hauteur_sprite); // largeur_sprite et hauteur_sprite est la même pour chaque sprite puisque cette fonction fait une copie de sprites égaux de taille

	cout << endl << "YEAH : " << endl;
	
	cout << repeat_horiz << " " << repeat_verti << endl;
	cout << largeur_sprite << " " << hauteur_sprite << endl;
	cout << (rect_sprite.y+rect_sprite.h) << " " << (repeat_verti*rect_sprite.h) << endl;
	cout << (rect_sprite.x+rect_sprite.w) << " " << (repeat_horiz*rect_sprite.w) << endl;

	cout << endl << endl;

	while( (rect_sprite.y+rect_sprite.h) <= (repeat_verti*rect_sprite.h) )
	{
		while( (rect_sprite.x+rect_sprite.w) <= (repeat_horiz*rect_sprite.w) )
		{
			cout << "Ca arrive là puis ça marche pas !" << endl;

			m_clip.push_back( Surface( Rect(0, 0, rect_sprite.w, rect_sprite.h), (int)feuille_sprites.BitsPerPixel() ) ); // On ajoute au vector un objet Surface créé avec RGB_Surface
	
			cout << "Sauf si..." << endl;
			
			if( m_clip.back() == false ) // Si la dernière surface n'a pas été bien créée
			{
				cout << "Mais c'est quoi ça ?!" << endl;
				m_clip.pop_back(); // On la supprime

				return; // Et on quitte la fonction
			}
			
			if( !feuille_sprites.apply( m_clip.back(), rect_sprite ) ) // Si la partie à copier de la feuille de sprites ne s'est pas blittée correctement dans l'image du clip
			{
				m_clip.back().free(); // On la free
				m_clip.pop_back(); // Et on supprime

				return;
			}

			m_clip.back().setColorKey(transparent_color); // Et finalement, On donne une couleur de transpareance

			rect_sprite.x += rect_sprite.w;
		}
		
		rect_sprite.y += rect_sprite.h;
	}

	feuille_sprites.free(); // Et bien sur, on libère la feuille de sprites qui ne nous servira à rien
}

Clip::Clip(const Clip &a_copier)
{
	cerr << "QUOI ?" << endl;
	int size_clip = a_copier.m_clip.size();
	
	if(size_clip > 0)
	{
		for(int i = 0; i < size_clip; i++)
		{
			if( a_copier.m_clip[i] == true )
			{
				cerr << i << endl;
				
				Surface surface;
				cerr << "AVANT : MAIS ??" << endl;
				surface.copySurface( a_copier.m_clip[i] );
				cerr << "APRES : MAIS ??" << endl;

				m_clip.push_back( surface );
			}
			else
				break;
		}
	}
}

Clip::~Clip()
{
	cerr << "Destruction clip." << endl;
	// On soulage la mémoire :D
	
	for(int i = 0; i < m_clip.size(); i++)
		m_clip[i].free(); // On libère la surface

	freeVector(m_clip);
}

int Clip::get_nbr_sprites()
{
	return m_clip.size();
}

bool Clip::empty()
{
	return m_clip.empty();
}

Surface Clip::to_feuille_sprites(Rect dimensions, int depth) // Sauvegarde le clip dans une surface dont l'utilisateur indique la largeur et hauteur
{
	if(dimensions.zero() )
	{
		for(int i = 0; i < m_clip.size(); i++)
		{
			dimensions.w += m_clip[i].width();
			dimensions.h += m_clip[i].height();
		}
	}

	Surface to_return(dimensions, depth);
	
	Rect position;
	int i = 0;
	
	for(position.y = 0; position.y < dimensions.h && i < m_clip.size(); position.y += m_clip[i].height() )
	{
		for(position.x = 0; position.x < dimensions.w && i < m_clip.size(); position.x += m_clip[i].width() )
		{
			cerr << i << endl;

			if( !m_clip[i].apply(to_return, Rect(), position) )
				return Surface();

			i++;	
		}
	}
	
	return to_return;
}

Surface Clip::get(const int number)
{
	if(number > m_clip.size() )
		return Surface();
		
	if(m_clip.empty() ) // Précaution comme toujours ;)
		return Surface();

	return m_clip[number];
}


Surface Clip::operator[](const int number)
{
	if(number > m_clip.size() )
		return Surface();
		
	if(m_clip.empty() ) // Précaution comme toujours ;)
		return Surface();

	return m_clip[number];
}

void Clip::operator=(Clip a_copier)
{
	int size_clip = a_copier.m_clip.size();
	m_clip.resize(size_clip);

	if(size_clip > 0)
	{
		for(int i = 0; i < size_clip; i++)
		{
			if( a_copier.m_clip[i] == true )
			{
				Surface surface;
				surface.copySurface(a_copier.m_clip[i] );
				m_clip.push_back( surface );
			}
			else
				break;
		}
	}
}

bool Clip::operator<<(Surface to_add)
{
	if(to_add == false)
		return false;
	
	m_clip.push_back( to_add );
	
	if( m_clip.back() == false )
	{
		m_clip.pop_back();

		return false;
	}
	
	return true;
}

bool Clip::operator<<(string to_add )
{
	if(to_add.length() < 1)
		return false;

	m_clip.push_back( to_add );

	if( m_clip.back() == false )
	{
		m_clip.pop_back();
		return false;
	}
	
	return true;
}
