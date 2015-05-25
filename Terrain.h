#ifndef CLASS_TERRAIN
#define CLASS_TERRAIN

#include <iostream>
#include "Utils.h"
#include "Clip.h"
#include "sdlglutils.h"
#include "Vector3D.h"
#include "Light.h"

class Terrain
{
	public:
		Terrain();
		Terrain(SDL::Surface carte_terrain_surf, Dim3 dimensions, float precision_x, float precision_y, bool avec_ombre, Clip *textures_terrain, unsigned char hauteur_tex, ...);
		~Terrain();
		
		/*! Changer les dimensions du terrain */
		void setDimensions(Dim3 nouvelles);
		
		void addLight(Light position_light);
		
		void setLight(Light light, int i);
		Light getLight(int i);
		
		/*! Changer de carte topographique */
		void setCarte(bool supprimer_ancienne, SDL::Surface carte_terrain_surf);
		/*! Changer les textures utilisées dans le terrain */
		void setTextures(bool supprimer_ancienne, Clip *textures_terrain, unsigned char hauteur_tex, ...);
		
		/*! Crée la texture finale du terrain */
		int creerTerrainTex(SDL::Rect resolution_texture);
		
		/*! Dessin du terrain */
		bool draw(SDL::Rect pos, bool avec_texture = true, float lineWidth = 0.1, SDL::RGB couleur_lignes = SDL::RGB(0, 255, 0) );
		
		void freeAll();
	
	private:
		SDL::RGB getColor(unsigned char hauteur, SDL::Rect position);
		unsigned char getHauteur();
		
		void calcLightMap(unsigned int w, unsigned int h);
		
		float calcLightMap_Simple(Vector3D<float> position, unsigned int max_x, unsigned int max_y);
		bool filtreLightMap(int taille_patch);
		
		void CreateDisplayLists();
		
	private:	
		/*! La carte topographique du terrain (intensité de gris = hauteur) */
		SDL::Surface m_carte_terrain_surf; 
		
		std::vector< std::vector<float> > m_lightmap;
		
		/*! Index vers une texture OpenGL, c'est la texture générée à partir du clip de surfaces et de la carte topographique qui est en noir et blanc
		 Ce qui veut dire que, en fonction de la hauteur, un pixel est séléctionné d'une des textures pour remplir la texture du terrain finale (représente tout le terrain d'en haut avec par exemple de l'herbe, des roches, de la neige etc...) */
		GLuint m_texture_finale_terrain;
		
		SDL::Rect m_resolution_texture;
		
		/*! Les dimensions du terrain */
		Dim3 m_dimensions;
		
		/*! Indique si nous dessinons le terrain avec une ombre ou non */
		bool m_avec_ombre;
		
		std::vector<Light> m_lights;
		float m_min_lightning, m_max_lightning;
		
		/*! Clip de surfaces principales (textures d'herbe, de roche etc...) utilisées dans le terrain */
		Clip *m_textures_terrain;
		
		/*! Est de même nombre de surfaces que m_texture mais est un tableau de unsigned char qui montre la limite de chaque texture dans le terrain
		en hauteur. Ce qui veut dire par exemple qui si on est en hauteur 255 (maximale), on va choisir telle ou telle texture (neige par exemple dans une montagne), si la hauteur est faible (par ex 20), cela voudra dire la première texture
		qui est l'herbe (c'est un exemple) */
		std::vector<unsigned char> m_hauteurs_textures;
		
		GLuint m_display_list;
		GLuint m_display_list_with_tex;
		
		/*! La carte topographique du terrain en fonction de ses dimensions (m_dimensions) */
		std::vector< std::vector<unsigned char> > m_carte_terrain;
};

#endif
