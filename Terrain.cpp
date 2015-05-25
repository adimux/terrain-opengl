#include "Terrain.h"

using namespace std;
using namespace SDL;

Terrain::Terrain() : m_texture_finale_terrain(0), m_textures_terrain(NULL), m_avec_ombre(false), m_dimensions(0,0,0), m_min_lightning(0.0), m_max_lightning(1.0)
{
	/*m_param.hautmax = 10.0;
	m_param.filaire = 1;
	m_param.angle = -65.0;
	m_param.angle2 = 0.0;
	m_param.trans = -6.0;
	m_param.water = 0;
	m_param.w_cur = 3.0;
	m_param.light = L_AUCUN;
	m_param.l_veci = -1;
	m_param.l_vecj = 0;
	m_param.l_vecz = -0.02;
	m_param.l_min = 0.2;
	m_param.l_max = 1.0;
	m_param.l_adouc = 0.01;
	m_param.l_ldotnmultiple = 47.0;*/
	
	m_display_list = glGenLists(1);
	
	srand(time(NULL));
}

Terrain::Terrain(SDL::Surface carte_terrain_surf, Dim3 dimensions, float precision_x, float precision_y, bool avec_ombre, Clip *textures_terrain, unsigned char hauteur_tex, ...) : m_min_lightning(0.2), m_max_lightning(1.0)
{
	srand(time(NULL));
	
	m_carte_terrain_surf = carte_terrain_surf;
	m_textures_terrain = textures_terrain;
	
	m_dimensions = dimensions;
	m_avec_ombre = avec_ombre;
	
	int x = 0, y = 0;
	
	if(precision_x > 1.0)
		precision_x = 1.0;
	if(precision_y > 1.0)
		precision_y = 1.0;
		
	m_carte_terrain = vector< vector<unsigned char> >( (int)( precision_y * (float)carte_terrain_surf.height() ), vector<unsigned char>( (int)( precision_x * (float)carte_terrain_surf.width() ), (char)0 ) );
	
	if(carte_terrain_surf == true)
	{
		for(y = 0; y < m_carte_terrain.size(); y++)
		{
			for(x = 0; x < m_carte_terrain.front().size(); x++)
			{
				m_carte_terrain[y][x] = carte_terrain_surf.getPixelOneComposant(Rect( (float)x/precision_x, (float)y/precision_y), 0);
			}
		}
	}
	
	va_list param; // Liste de paramètres optionnels fonction

	va_start(param, hauteur_tex); // Initialisation de ap
	
	m_hauteurs_textures.push_back(hauteur_tex);
	
	unsigned char hauteur_texture = 0, i = 0;
	
	for(int i = 0; i < m_textures_terrain->get_nbr_sprites()-1 + (m_textures_terrain->get_nbr_sprites() - 1); i++)
	{
		hauteur_texture = va_arg(param, int);
		
		m_hauteurs_textures.push_back(hauteur_texture);
	}
	
	va_end(param);
}

Terrain::~Terrain()
{
	glDeleteLists(m_display_list, 1);
}

void Terrain::setDimensions(Dim3 nouvelles)
{
	m_dimensions = nouvelles;
	
	if(m_texture_finale_terrain != 0)
	{
		if(m_avec_ombre && !m_lights.empty() )
		{
			creerTerrainTex(m_resolution_texture);
		}
	}
}

void Terrain::addLight(Light light)
{
	Vector3D<float> source_vector(0, 0, 0);
float old_z = light.z;
	light.Normalize();
	cout << "Position Light : " << light.x << " " << light.y << " " << light.z << endl;
//	Light direction_light(position_light_vector);
//	direction_light =  source_vector.InSameLine(position_light_vector, 1.0);
	light.x *= -1.5;
	light.y *= -1.5;
//	light *= 1.5;

//	light.z = old_z;
	
	cout << "Direction light : " << light.x << " " << light.y << " " << light.z << endl;

	m_lights.push_back(light);
}

void Terrain::setLight(Light light, int i)
{
//	Vector3D<float> source_vector(0, 0, 0);
//	Vector3D<float> position_light_vector(light);
//	position_light_vector.Normalize();
	light.Normalize();
	cout << "Position Light : " << light.x << " " << light.y << " " << light.z << endl;
//	Light direction_light(position_light_vector);
//	direction_light =  source_vector.InSameLine(position_light_vector, 1.0);
	light.x *= -1.5;
	light.y *= -1.5;
	
	cout << "Direction light : " << light.x << " " << light.y << " " << light.z << endl;

	m_lights[i] = light;
}

Light Terrain::getLight(int i)
{
	return m_lights[i];
}

void Terrain::freeAll()
{
	m_carte_terrain_surf.free();
	
	delete m_textures_terrain;
	
	glDeleteTextures(1, &m_texture_finale_terrain);
}

/*! Changer de carte topographique */
void Terrain::setCarte(bool supprimer_ancienne, Surface carte_terrain_surf)
{
	if(supprimer_ancienne)
		m_carte_terrain_surf.free();
		
	m_carte_terrain_surf = carte_terrain_surf;

	if(m_texture_finale_terrain != 0)
	{
		if(m_avec_ombre && !m_lights.empty() )
		{
			creerTerrainTex(m_resolution_texture);
		}
	}
	
	if(m_carte_terrain_surf == true)
	{
		m_carte_terrain.assign(m_carte_terrain_surf.height(), vector<unsigned char>(m_carte_terrain_surf.width(), (unsigned char)0 ) );
		
		int x = 0, y = 0;
		
		for(y = 0; y < m_carte_terrain.size(); y++)
		{
			for(x = 0; x < m_carte_terrain.front().size(); x++)
			{
				m_carte_terrain[y][x] = m_carte_terrain_surf.getPixelOneComposant(Rect( (float)x/((float)m_carte_terrain_surf.width() / (float)m_carte_terrain.front().size() ), (float)y/((float)m_carte_terrain_surf.height() / (float)m_carte_terrain.size() ) ), 0);
			}
		}
	}
}

/*! Changer les textures utilisées dans le terrain */
void Terrain::setTextures(bool supprimer_ancienne, Clip *textures_terrain, unsigned char hauteur_tex, ...)
{
	if(supprimer_ancienne)
		delete m_textures_terrain;
	
	m_textures_terrain = textures_terrain;
	
	va_list param; // Liste de paramètres optionnels fonction

	va_start(param, hauteur_tex); // Initialisation de ap
	
	m_hauteurs_textures.push_back(hauteur_tex);
	
	unsigned char hauteur_texture = 0, i = 0;
	
	for(int i = 0; i < m_textures_terrain->get_nbr_sprites()-1 + (m_textures_terrain->get_nbr_sprites() - 1); i++)
	{
		hauteur_texture = va_arg(param, int);
		
		m_hauteurs_textures.push_back(hauteur_texture);
	}
	
	va_end(param);
}


RGB Terrain::getColor(unsigned char hauteur, Rect position)
{
	int add = hauteur + (rand() % 20) - 10;
	
	if( add < 0)
		add = 0;
	else if(add > 255)
		add = 255;
		
//	hauteur = add;
	
	float *perc = new float[ m_textures_terrain->get_nbr_sprites() ];
	int i = 0;
	
	for(i = 0; i < m_textures_terrain->get_nbr_sprites(); i++ )
		perc[i] = 0;
	
	for(i = 0; i < m_hauteurs_textures.size(); i++)
	{
		if( hauteur <= m_hauteurs_textures[i] )
		{
			if(i == 0)
			{
				perc[i] = 1.0f;
				
				break;
			}
			else if( i >= m_hauteurs_textures.size() - 1 )
			{
				perc[ m_textures_terrain->get_nbr_sprites() - 1 ] = 1.0f;
				
				break;	
			}
			else if( hauteur > m_hauteurs_textures[i-1] && hauteur <= m_hauteurs_textures[i] )
			{
				if( (i+1) & 1 )
				{
					// i est impair donc cette texture seule
					
					float j = (float)(i+1)/2;
					
					if( (j - (float)( (int)j ) ) >= 0.5)
						j = (int)j + 1;
					else
						j = (int)j;
						
					j -= 1;
					perc[ (int)j ] = 1.f;
					
					break;
				}
				else
				{
					// i est pair donc mélange entre la texture précédente et la suivante
					
					int j = (i+1)/2;
					j--;
					
					perc[j] = 1.0f - (float)( hauteur - m_hauteurs_textures[i-1] ) / (float)( m_hauteurs_textures[i] - m_hauteurs_textures[i-1] );
					perc[j+1] = 1.0f - perc[j];
					
					break;
				}
			}
		}
	}

	
	RGB adequate_color(0, 0, 0);
	
	Uint8 *adresse_pixel;
	Pixel pixel;
	Uint8 r, g, b, a;
	
	int posx, posy;
	
	for(i = 0; i < m_textures_terrain->get_nbr_sprites(); i++)
	{
		if(perc[i] != 0)
		{
//			pos.x = (int)position.x * ( (float)m_textures_terrain->get(i).width() / position.w );
//			pos.y = (int)position.y * ( (float)m_textures_terrain->get(i).height() / position.h );
			posx = (int)position.x % (int)m_textures_terrain->get(i).width();
			posy = (int)position.y % (int)m_textures_terrain->get(i).height();
	
//			adresse_pixel = (Uint8*)m_textures_terrain->get(i).pixels() + posx*(m_textures_terrain->get(i).pitch()/m_textures_terrain->get(i).width() ) + posy * m_textures_terrain->get(i).pitch();
			
			pixel = m_textures_terrain->get(i).getPixel( Rect(posx, posy) );
		
			pixel.getPixelInfo(&r, &g, &b, &a);
//			
//			pixel.getPixelInfo(&r, &g, &b, &a);
			//int bytesPerPixel = m_textures_terrain->get(i).BitsPerPixel() / 8;

			//olel = ( (Uint8*) m_textures_terrain->get(i).pixels() ) +  (int)position.x*bytesPerPixel + (int)position.y*m_textures_terrain->get(i).pitch();
			//pff = m_textures_terrain->get(i).getPixelColor(pos);		
//			pixel = *( (Uint32*) olel );
			
//			SDL_GetRGB( *( (Uint32*)adresse_pixel ), m_textures_terrain->get(i).PixelFormatAddr(), &r, &g, &b);
			
			adequate_color.r += (int)( (float)perc[i] * (float)r );
			adequate_color.g += (int)( (float)perc[i] * (float)g );
			adequate_color.b += (int)( (float)perc[i] * (float)b );
		}
	}
	
	free(perc);
	
	return adequate_color;
}

float Terrain::calcLightMap_Simple(Vector3D<float> position, unsigned int max_x, unsigned int max_y)
{
	Rect carte_topo_pos;
	float result = 0.0f;
	
	for(int i = 0; i < m_lights.size(); i++)
	{
		carte_topo_pos = Rect( (float)( (int)position.x ) * ( (float)m_carte_terrain_surf.width()/(float)max_x ) - m_lights[i].x, (float)( (int)position.y ) * ( (float)m_carte_terrain_surf.height()/(float)max_y ) - m_lights[i].y);
	
		if( carte_topo_pos.x >= 0 && carte_topo_pos.y >= 0 && carte_topo_pos.x < m_carte_terrain_surf.width() && carte_topo_pos.y < m_carte_terrain_surf.height() )
		{
			float to_add = (1.0 - ( (float)(m_carte_terrain_surf.getPixelOneComposant(carte_topo_pos, 0) - (float)position.z) / m_lights[i].get_adouc() ) );
			
			if(to_add < 0.0)
				to_add = 0;
			else if(to_add > 1.0)
				to_add = 1.0;
			
			result += to_add;
			/*if(i != 0)
			{
				if(to_add > 0)
					result += to_add;
			}
			else
				result += to_add;*/
		}
		else
		{
			result += 1.0f;
		}
	}
	
	if(result > 1.0f)
		result = 1.0f;
	else if(result < 0.0f)
		result = 0.0f;
	
	return result;
}

void Terrain::calcLightMap(unsigned int w, unsigned int h)
{
	m_lightmap.assign(h, vector<float>(w, 1.0) );
	
	int x = 0, y = 0;
	float facteur_x = (float)m_carte_terrain_surf.width()/(float)w;
	float facteur_y = (float)m_carte_terrain_surf.height()/(float)h;
	
	for(y = 0; y < h, y*facteur_y < m_carte_terrain_surf.height(); y++)
	{
		for(x = 0; x < w, x*facteur_x < m_carte_terrain_surf.width(); x++)
		{
			if(facteur_x*x < m_carte_terrain_surf.width() && facteur_y*y < m_carte_terrain_surf.height() )
			{
				m_lightmap[y][x] = calcLightMap_Simple(Vector3D<float>(x, y, m_carte_terrain_surf.getPixelOneComposant( Rect(x*facteur_x, y*facteur_y), 0) ), w, h);
				
				if(m_lightmap[y][x] < m_min_lightning)
					m_lightmap[y][x] = m_min_lightning;
				else if(m_lightmap[y][x] > m_max_lightning)
					m_lightmap[y][x] = m_max_lightning;
			}
		}
	}
}

bool Terrain::filtreLightMap(int taille_patch)
{
	if(m_lightmap.empty() || m_lightmap.size() == 0 || m_lightmap.front().size() == 0)
		return false;
		
	int i = 0, j = 0, x = 0, y = 0, cnt = 0;
	
	vector< vector<float> > tmp_lightmap(m_lightmap.size(), vector<float>(m_lightmap.front().size(), 0.0) );
		
	for(j = 0; j < m_lightmap.size(); j++)
	{
		for(i = 0; i < m_lightmap.front().size(); i++)
		{
			cnt = 0;
			
			for(y = j - taille_patch; y <= j + taille_patch; y++)
			{
				for(x = i - taille_patch; x <= i + taille_patch; x++)
				{
					if(x >= 0 && y >= 0 && y < m_lightmap.size() && x < m_lightmap.front().size() )
					{
						tmp_lightmap[j][i] += m_lightmap[y][x];
						cnt++;
					}
				}
			}
			
			if(cnt)
			{
				tmp_lightmap[j][i] /= cnt;
			}
			else
				tmp_lightmap[j][i] = 1.0f;
		}	
	}
	
	m_lightmap = tmp_lightmap;
	
	return true;	
}

int Terrain::creerTerrainTex(Rect resolution_texture)
{
	m_resolution_texture = resolution_texture;
	
	int i = 0, x = 0, y = 0;
	Uint8 hauteur = 0;
	Pixel pixel_actuel;
	
	for(int i = 0; i < m_textures_terrain->get_nbr_sprites(); i++)
	{
		if( m_textures_terrain->get(i) == false )
			return -1;
		else
			m_textures_terrain->get(i).lock();
	}
	
	if( m_carte_terrain_surf == false )
		return -2;
		
		int la_fin;
	
	SDL_Surface *terrain_texture_surface = SDL_CreateRGBSurface(SDL_HWSURFACE, (int)m_resolution_texture.w, (int)m_resolution_texture.h, 32, 0, 0, 0, 0);
	m_carte_terrain_surf.lock();
	
	SDL_LockSurface(terrain_texture_surface);
	int kp = 0;
	
	float fact_x = (float)m_carte_terrain_surf.width()/(float)terrain_texture_surface->w;
	float fact_y = (float)m_carte_terrain_surf.height()/(float)terrain_texture_surface->h;
	
	Uint8 *adresse_pixel = (Uint8*) m_carte_terrain_surf.pixels(), *p = (Uint8*) terrain_texture_surface->pixels;
	
	int carte_octetsParPixel = m_carte_terrain_surf.BitsPerPixel() / 8;
	Uint8 g, b;
	
	float facteur = ((float)(terrain_texture_surface->w*terrain_texture_surface->h)/(float)(m_carte_terrain_surf.width()*m_carte_terrain_surf.height()) );
cout << facteur << endl;
	int n = 0, cx = 0, cy = 0;
	Uint32 pixel;
	SDL_PixelFormat *pixel_format = m_carte_terrain_surf.PixelFormatAddr();
	
	Rect sauv, true_pos;
	RGB couleur_pixel_actuel;
	
	if(m_avec_ombre)
	{
		calcLightMap( m_carte_terrain_surf.width(), m_carte_terrain_surf.height() );
		filtreLightMap(3); // Application du filtre pour embellir la lightmap
	}
	
	float diff;
	
	float lightmap_facteur_x = (float)m_lightmap.front().size() / (float)terrain_texture_surface->w, lightmap_facteur_y = (float)m_lightmap.size() / (float)terrain_texture_surface->h;
	
	for(y = 0; y < terrain_texture_surface->h; y++)
	{
		for(x = 0; x < terrain_texture_surface->w; x++)
		{

			//hauteur = m_carte_terrain_surf.getPixelColor( Rect( (int)( (float)x*(fact_x) ), (int)( (float)y*(fact_y) ) ) ).r; // Le niveau de gris est la hauteur dans ce pixel car la carte topographique est en noir et
			//cout << "x : " << x << " y : " << y << " n : " << n/4 << endl;
//			SDL_GetRGB( *( (Uint*) pixel), m_carte_terrain_surf.PixelFormatAddr(), &hauteur, &g, &b);
		    /*switch(carte_octetsParPixel)
    		{
	    	    case 1:
	    	        pixel = *adresse_pixel;
				break;
				
		        case 2: // Si c'est 2 (2 * 8 = 16 donc 16bytes alors que *adresse_pixel est un Uint32)
    		        pixel = *(Uint16 *)adresse_pixel; // On convertit
				break;
				
		        case 3:
    		        // Suivant l'architecture de la machine
	    	        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            		    pixel = adresse_pixel[0] << 16 | adresse_pixel[1] << 8 | adresse_pixel[2];
        	    	else
    	            	pixel = adresse_pixel[0] | adresse_pixel[1] << 8 | adresse_pixel[2] << 16;
				break;
				
		        case 4:
    	    	    pixel = *(Uint32 *)adresse_pixel;
				break;
				
    		    // Ne devrait pas arriver, mais évite les erreurs
	        	default:
        	    break;	
	    	};*/
	   
	    	  //SDL_GetRGB(pixel, pixel_format, &hauteur, &g, &b);
			//memcpy(&hauteur, (Uint8*)&pixel, 1);//cout << (int)hauteur << endl;

			hauteur = (int) m_carte_terrain_surf.getPixelOneComposant(
			Rect( (int)( (float)x*((float)m_carte_terrain_surf.width()/(float)terrain_texture_surface->w) ),
			(int)( (float)y*((float)m_carte_terrain_surf.height()/(float)terrain_texture_surface->h) ) ), 0);
//			hauteur = m_carte_terrain[ (int)( (float)y*((float)m_carte_terrain.size()/(float)terrain_texture_surface->h) ) ][ (int)( (float)x*((float)m_carte_terrain.front().size()/(float)terrain_texture_surface->w) ) ];
			// blanc et chaque pixel indique une hauteur allant de 0 jusqu'à 255
			//cout << hauteur << endl;
//			int kiki = SDL_GetTicks();
			//sauv = true_pos;
			//true_pos.x = x / facteur;
			
			//if( (int)true_pos.x != (int)sauv.x || (int)true_pos.y != (int)sauv.y )
			couleur_pixel_actuel = getColor(hauteur, Rect(x, y, terrain_texture_surface->w, terrain_texture_surface->h) );
//			pixel_actuel = Pixel( (Uint8)couleur_pixel_actuel.r, (Uint8)couleur_pixel_actuel.g, (Uint8)couleur_pixel_actuel.b, (Uint8)0, *terrain_texture_surface->format);
//			kp += SDL_GetTicks() - kiki;
//cout << "foel" << endl;
//			diff = calcLightMap_Simple(Vector3D<float>(x, y, hauteur), terrain_texture_surface->w, terrain_texture_surface->h);
			if(m_avec_ombre)
				diff = m_lightmap[ (int)(y* lightmap_facteur_y) ][ (int)(x*lightmap_facteur_x) ];
			else
				diff = 1.0;
					
			if( (float)couleur_pixel_actuel.r * diff >= 255)
				couleur_pixel_actuel.r = 255;
			else
				couleur_pixel_actuel.r = (int)( (float)couleur_pixel_actuel.r * diff );
				
			if( (float)couleur_pixel_actuel.g * diff >= 255)
				couleur_pixel_actuel.g = 255;
			else
				couleur_pixel_actuel.g = (int)( (float)couleur_pixel_actuel.g * diff );
				
			if( (float)couleur_pixel_actuel.b * diff >= 255)
				couleur_pixel_actuel.b = 255;
			else
				couleur_pixel_actuel.b = (int)( (float)couleur_pixel_actuel.b * diff );
			
			pixel = SDL_MapRGB(terrain_texture_surface->format, (Uint8)couleur_pixel_actuel.r, (Uint8)couleur_pixel_actuel.g, (Uint8)couleur_pixel_actuel.b);
			//memcpy( (Uint8*) &pixel, (Uint8*)&couleur_pixel_actuel.r, 1);
	//					cout << "koko" << endl;
			//memcpy( ( (Uint8*) &pixel) + 1, (Uint8*)&couleur_pixel_actuel.g, 1);
//						cout << "koko" << endl;
			//memcpy( ( (Uint8*) &pixel) + 2, (Uint8*)&couleur_pixel_actuel.b, 1);
//						cout << "koko" << endl;

    	switch(terrain_texture_surface->format->BytesPerPixel)
	    {
    	    case 1:
        	    *p = pixel;
            break;

	        case 2:
    	        *(Uint16 *)p = pixel;
            break;

	        case 3:
    	        // Suivant l'architecture de la machine
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

			//setPixel(terrain_texture_surface, Rect(x, y), SDL_MapRGB(terrain_texture_surface->format, (Uint8)couleur_pixel_actuel.r, (Uint8)couleur_pixel_actuel.g, (Uint8)couleur_pixel_actuel.b) );
			
			/*if((x+1) % 2 == 0  && x <= (terrain_texture_surface->w-1) && cy < m_carte_terrain_surf.height() )
			{
				adresse_pixel += carte_octetsParPixel;
				n++;
				cx++;
				if(cx >= m_carte_terrain_surf.width() )
					cx = 0;
				//cout << "Nolel ! " << x << endl;
			}
			if( (y+1) % 2  == 0 && (y+1) > 1 && x >= (terrain_texture_surface->w-1) && cy < m_carte_terrain_surf.height() )
			{
				adresse_pixel += m_carte_terrain_surf.pitch();
				n += m_carte_terrain_surf.width();
				cy++;
				cout << "Yolel ! " << cy << " " << cy % 2 << endl;
			}*/
			
			p += terrain_texture_surface->format->BytesPerPixel;
		}
	}
	
	cout << "... " << kp << endl;
	
	SDL_UnlockSurface(terrain_texture_surface);
	
	m_carte_terrain_surf.unlock();
	
	for(int i = 0; i < m_textures_terrain->get_nbr_sprites(); i++)
	{
		m_textures_terrain->get(i).unlock();
	}
	
	glEnable(GL_TEXTURE_2D);
	
	int avant = SDL_GetTicks();
	
	//m_texture_finale_terrain = loadTexture(terrain_texture_surface);
	
	glGenTextures(1, &(m_texture_finale_terrain) );
	glBindTexture(GL_TEXTURE_2D, m_texture_finale_terrain);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, terrain_texture_surface->w, terrain_texture_surface->h, GL_RGBA, GL_UNSIGNED_BYTE, terrain_texture_surface->pixels);
	
	cout << SDL_GetTicks() - avant << endl;
	
	glDisable(GL_TEXTURE_2D);
	
	SDL_SaveBMP(terrain_texture_surface, "terrain.bmp");

	SDL_FreeSurface(terrain_texture_surface);

	CreateDisplayLists();
}

void Terrain::CreateDisplayLists()
{
	Rect position;
	int i, j;
	
	float ajoutx = m_dimensions.w / (float)m_carte_terrain.front().size(); // L'ajout de position varie
	float ajouty = m_dimensions.l / (float)m_carte_terrain.size();
	float division_hauteur = 255 / m_dimensions.h; // Puisque la carte peut enregistrer des hauteurs plus grandes que l'espérance de l'utilisateur, on divise n'importe quelle hauteur dans la carte par cette variable
	
	glNewList(m_display_list, GL_COMPILE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glBegin(GL_TRIANGLES);

	for(j = 0, position.y = 0; j < m_carte_terrain.size() - 1; j++, position.y += ajouty)
	{
		for(i = 0, position.x = 0; i < m_carte_terrain.front().size() - 1; i++, position.x += ajoutx)
		{
			glVertex3f(position.x, position.y, m_carte_terrain[j][i]/division_hauteur );
			glVertex3f(position.x, position.y + ajouty, m_carte_terrain[j+1][i]/division_hauteur );
			glVertex3f(position.x + ajoutx, position.y + ajouty, m_carte_terrain[j+1][i+1]/division_hauteur );
			
			glVertex3f(position.x, position.y, m_carte_terrain[j][i]/division_hauteur );
			glVertex3f(position.x + ajoutx, position.y + ajouty, m_carte_terrain[j+1][i+1]/division_hauteur );
			glVertex3f(position.x + ajoutx, position.y, m_carte_terrain[j][i+1]/division_hauteur );
		}
	}
	
	glEnd();
	
	glEndList();
	
	glNewList(m_display_list_with_tex, GL_COMPILE);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture_finale_terrain); // On indique à OpenGL qu'on utilisera la texture du terrain précédemment créée
	
	glColor3ub(255, 255, 255);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glBegin(GL_TRIANGLES);
	
	for(j = 0, position.y = 0; j < m_carte_terrain.size() - 1; j++, position.y += ajouty)
	{
		for(i = 0, position.x = 0; i < m_carte_terrain.front().size() - 1; i++, position.x += ajoutx)
		{
			glTexCoord2f( (float)i / (float)m_carte_terrain.front().size(), (float)j / (float)m_carte_terrain.size() );
			glVertex3f(position.x, position.y, m_carte_terrain[j][i]/division_hauteur );
			
			glTexCoord2f( (float)i / (float)m_carte_terrain.front().size(), (float)(j + 1) / (float)m_carte_terrain.size() );
			glVertex3f(position.x, position.y + ajouty, m_carte_terrain[j+1][i]/division_hauteur );
			
			glTexCoord2f( (float)(i + 1) / (float)m_carte_terrain.front().size(), (float)(j + 1) / (float)m_carte_terrain.size());
			glVertex3f(position.x + ajoutx, position.y + ajouty, m_carte_terrain[j+1][i+1]/division_hauteur );
			
			
			glTexCoord2f( (float)i / (float)m_carte_terrain.front().size(), (float)j / (float)m_carte_terrain.size());
			glVertex3f(position.x, position.y, m_carte_terrain[j][i]/division_hauteur );

			glTexCoord2f( (float)(i + 1) / (float)m_carte_terrain.front().size(), (float)(j + 1) / (float)m_carte_terrain.size());
			glVertex3f(position.x + ajoutx, position.y + ajouty, m_carte_terrain[j+1][i+1]/division_hauteur );
			
			glTexCoord2f( (float)(i + 1) / (float)m_carte_terrain.front().size(), (float)j / (float)m_carte_terrain.size());
			glVertex3f(position.x + ajoutx, position.y, m_carte_terrain[j][i+1]/division_hauteur );
		}
	}
	
	glEnd();
	
	glEndList();
}

bool Terrain::draw(Rect pos, bool avec_texture, float lineWidth, RGB couleur_lignes)
{
	if(m_carte_terrain_surf == false) // Si la carte du terrain est corrompue (c'est une image)
		return false; // On retourne erreur
	if(avec_texture && m_texture_finale_terrain == 0) // Si la texture de terrain est égale à 0 (donc pas de texture :D ) et qu'en plus on doit dessiner avec texture
		return false; // On retourner false
	if(m_hauteurs_textures.size() == 0)
		return false;
	
	glPushMatrix(); // On sauvegarde la matrix
	
	m_lights[0].draw( RGB(255, 0, 0) );
	
	glTranslated(pos.x - m_dimensions.w/2, pos.y - m_dimensions.l/2, 0); // On translate pour que le terrain soit dessiné au milieu du point donné (seulement en x et y)
	
	Rect position;
	unsigned int i = 0, j = 0;
	
	if(!avec_texture)
		glLineWidth(lineWidth);



//	float ajoutx = m_dimensions.w / (float)m_carte_terrain.front().size(); // L'ajout de position varie
//	float ajouty = m_dimensions.l / (float)m_carte_terrain.size();
//	float division_hauteur = 255 / m_dimensions.h; // Puisque la carte peut enregistrer des hauteurs plus grandes que l'espérance de l'utilisateur, on divise n'importe quelle hauteur dans la carte par cette variable
	
//	if(avec_texture) // Si on dessine le terrain avec texture
//	{
//		glEnable(GL_TEXTURE_2D);
//		glBindTexture(GL_TEXTURE_2D, m_texture_finale_terrain); // On indique à OpenGL qu'on utilisera la texture du terrain précédemment créée
//	}
	
//	if(!avec_texture)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//glBegin(GL_TRIANGLES);
	
	if(!avec_texture)
	{
		glColor3ub(couleur_lignes.r, couleur_lignes.g, couleur_lignes.b);
		glCallList(m_display_list);	
	}
	else
	{
		//glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_texture_finale_terrain); // On indique à OpenGL qu'on utilisera la texture du terrain précédemment créée
	
		glCallList(m_display_list_with_tex);	
	}
	/*for(j = 0, position.y = 0; j < m_carte_terrain.size() - 1; j++, position.y += ajouty)
	{
		for(i = 0, position.x = 0; i < m_carte_terrain.front().size() - 1; i++, position.x += ajoutx)
		{
			
			if(avec_texture)
				glTexCoord2f( (float)i / (float)m_carte_terrain.front().size(), (float)j / (float)m_carte_terrain.size() );
			glVertex3f(position.x, position.y, m_carte_terrain[j][i]/division_hauteur );
			
			if(avec_texture)
				glTexCoord2f( (float)i / (float)m_carte_terrain.front().size(), (float)(j + 1) / (float)m_carte_terrain.size() );
			glVertex3f(position.x, position.y + ajouty, m_carte_terrain[j+1][i]/division_hauteur );
			
			if(avec_texture)
				glTexCoord2f( (float)(i + 1) / (float)m_carte_terrain.front().size(), (float)(j + 1) / (float)m_carte_terrain.size());
			glVertex3f(position.x + ajoutx, position.y + ajouty, m_carte_terrain[j+1][i+1]/division_hauteur );
			
			
			if(avec_texture)
				glTexCoord2f( (float)i / (float)m_carte_terrain.front().size(), (float)j / (float)m_carte_terrain.size());
			glVertex3f(position.x, position.y, m_carte_terrain[j][i]/division_hauteur );

			if(avec_texture)
				glTexCoord2f( (float)(i + 1) / (float)m_carte_terrain.front().size(), (float)(j + 1) / (float)m_carte_terrain.size());
			glVertex3f(position.x + ajoutx, position.y + ajouty, m_carte_terrain[j+1][i+1]/division_hauteur );
			
			if(avec_texture)
				glTexCoord2f( (float)(i + 1) / (float)m_carte_terrain.front().size(), (float)j / (float)m_carte_terrain.size());
			glVertex3f(position.x + ajoutx, position.y, m_carte_terrain[j][i+1]/division_hauteur );
		}
	}*/
	
	//glEnd();
	
	glPopMatrix();
}
