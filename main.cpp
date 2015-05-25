#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include "ecran.h"
#include "sdlglutils.h"
#include "Camera.h"
#include "main.h"

using namespace std;
using namespace SDL;

/*typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

void setVSync(int interval=1)
{
  const char *extensions = glGetString( GL_EXTENSIONS );

  if( strstr( extensions, "WGL_EXT_swap_control" ) == 0 )
    return; // Error: WGL_EXT_swap_control extension not supported on your computer.\n");
  else
  {
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );

    if( wglSwapIntervalEXT )
      wglSwapIntervalEXT(interval);
  }
}*/

void dessiner_repere(double=3, double=6);
void Dessiner();
Terrain creer_terrain();
void dessiner_terrain(Terrain &terrain);

Vector3D<float> GetOGLPos(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	return Vector3D<float>(posX, posY, posZ);
}

CameraFreeFly *camera; // On crée une caméra FreeFly

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_EnableUNICODE(1);

    SDL_WM_SetCaption("SDL GL Application", NULL);
    SDL_SetVideoMode(LARGEUR_ECRAN, HAUTEUR_ECRAN, 32, SDL_OPENGL);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(70, (double)LARGEUR_ECRAN/HAUTEUR_ECRAN, 1, 1000);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    
    bool continuer = true;
    
	Uint32 last_time = SDL_GetTicks(), start_time;
    Uint32 current_time;

	camera = new CameraFreeFly(Pos(10, 10, 0), Pos(0, 0, 0) ); // On crée une caméra FreeFly
	
	camera->setSpeed(Speed(1.0, (Uint32)30) );
	camera->setSensitivity(Speed(0.13, (Uint32)10) );
	
	Clip olel("data/textures/Best/GrassTexture.png", "data/textures/Best/RockTextureBW.jpg", "data/textures/Best/snow.bmp", NULL);
//	Clip olel("tranche.jpg", NULL);
	Surface heightmap("data/heightmaps/favorites/bave/heightmap.bmp");
	
	Terrain terrain(heightmap, Dim3(80, 80, 18), 0.3 * (512.0/(float)heightmap.width()),
	0.3 * (512.0/(float)heightmap.height()), true, &olel,
	60, /*80,*/ 130, /*150,*/ 180, 220, 255);
	//255);
	
	Light my_light(-16, 20, 10, 0.01, Light_DiffuseLight);
	Light my_other_light(2, -80, 40, 0.01, Light_DiffuseLight);
	terrain.addLight(my_other_light);	
//	terrain.addLight(my_light);

	Vector3D<double> dolel(0, 0, 0);
	Vector3D<double> zolel(10, 0, 3);
	Vector3D<double> folel = dolel.InSameLine(zolel, 1.0);
	
	cout << "Folel : " << folel.x << " " << folel.y << " " << folel.z << endl;
	
	GLUquadric *sphere;
	sphere = gluNewQuadric();

	int avant = SDL_GetTicks();
	terrain.creerTerrainTex( Rect(0, 0, 1200, 1200) );
	cout << "Temps : " << SDL_GetTicks() - avant << endl;
	
	Sint32 last_frame = SDL_GetTicks();
	SDL_Event event;
	
	int avante = SDL_GetTicks(), daba;
	
	bool ok = false;
	Vector3D<float> xyz_selectionne;
	
	float x_water = 0;
	
	int mimi = 0;
	bool affichage_filaire = false;

	Speed courant_eau(0.1, 100);
	courant_eau.start();
	
	Uint32 camera_3D_timer = SDL_GetTicks();
	bool gauche = true;

    while(continuer)
    {
		daba = SDL_GetTicks();
		
        if( SDL_PollEvent(&event) ) // S'il y a de nouveaux évènements
        {
    	    switch(event.type)
	        {
            	case SDL_QUIT:
        	    	continuer = false;
    	        break;

	            case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE)
						continuer = false;
					else if(event.key.keysym.sym == SDLK_f)
					{
						if(affichage_filaire)
							affichage_filaire = false;
						else
							affichage_filaire = true;	
					}
				break;
				
				case SDL_MOUSEBUTTONDOWN:
				break;
	        };
		}
		
		if( SDL_GetTicks() - camera_3D_timer >= 50)
		{
			camera_3D_timer = SDL_GetTicks();
			
			if(gauche)
			{
				camera->av(0.35, DROITE);
				
				gauche = false;
			}
			else
			{
				camera->av(0.35, GAUCHE);
				
				gauche = true;
			}
		}
		
		if(daba - avante >= 4000)
		{
			//my_light.xsssss += 4;
			//terrain.setLight(my_light, 0);
			//terrain.creerTerrainTex( Rect(0, 0, 512, 512) );
			//avante = daba;
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	glMatrixMode( GL_MODELVIEW );
	    glLoadIdentity( );
	    
	    camera->look();

		dessiner_repere(20);
		mimi = SDL_GetTicks();
		
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		terrain.draw( Rect(0, 0), affichage_filaire );
		glPopMatrix();
		
		glDisable(GL_TEXTURE_2D);
		glColor3ub(255, 255, 224);
		
		glPushMatrix();
		glTranslated(my_light.x, my_light.y, my_light.z);
		//gluSphere(sphere, 3.25, 20, 20);
		glPopMatrix();
		
		glPushMatrix();
		glTranslated(my_other_light.x, my_other_light.y, my_other_light.z);
		gluSphere(sphere, 6, 20, 20);
		glPopMatrix();
			
	    glFlush();
		SDL_GL_SwapBuffers();
		
        current_time = SDL_GetTicks();
		Uint32 elapsed_time = current_time - last_frame;
		last_frame = current_time;
		daba = SDL_GetTicks();
		
		if(daba - avant >= 2000)
		{
			cout << "FPS : " << 1000/elapsed_time << endl;
			avant = daba;
		}

		camera->input_handle(&event, elapsed_time);
    }
	
	gluDeleteQuadric(sphere);
	terrain.freeAll();

	SDL_Quit();
	
	delete camera;

    return 0;
}

void dessiner_repere(double longueur_ligne, double lineWidth)
{
	glDisable(GL_TEXTURE_2D);
    glLineWidth(lineWidth);

    glBegin(GL_LINES);
    	glColor3ub(255, 0, 0);
    		glVertex3d(0, 0, 0);
    		glVertex3d(longueur_ligne, 0, 0);
    	glColor3ub(0, 255, 0);
    		glVertex3d(0, 0, 0);
    		glVertex3d(0, longueur_ligne, 0);
    	glColor3ub(0, 0, 255);
    		glVertex3d(0, 0, 0);
    		glVertex3d(0, 0, longueur_ligne);
    glEnd();

	glEnable(GL_TEXTURE_2D);
	glColor3ub(255, 255, 255);
}
/*
unsigned char Terrain_GetHauteur(SDL_Surface *terrain, int i, int j, int maxi, int maxj)
{
	float posx = (float)j * ( (float)terrain->w/maxj );
	float posy = (float)i * ( (float)terrain->h/maxi );
	
	return ((unsigned char*) terrain->pixels)[(int)posy*terrain->w*3 + (int)posx*3];
}

void Terrain_RemplitPerc(float *perc, unsigned char haut)
{
        // Que de la prairie 
        if(haut<60)
                {
                perc[0] = 1.0f;
                perc[1] = 0.0f;
                perc[2] = 0.0f;
                }
        // Melange entre prairie et roche
        else if(haut<130)
                {
                perc[0] = 1.0f - (haut-60.0f)/70.0f;
                perc[1] = (haut-60.0f)/70.0f;
                perc[2] = 0.0f;
                }
        // Que de la roche
        else if(haut<180)
                {
                perc[0] = 0.0f;
                perc[1] = 1.0f;
                perc[2] = 0.0f;
                }
        // Melange entre roche et la neige
        else if(haut<220)
                {
                perc[0] = 0.0f;
                perc[1] = 1.0f - (haut-180.0f)/40.0f;
                perc[2] = (haut-180.0f)/40.0f;
                }
        // Que de la neige
        else
                {
                perc[0] = 0.0f;
                perc[1] = 0.0f;
                perc[2] = 1.0f;
                }
}

unsigned char Terrain_GetPixelColor(SDL_Surface *image, int i, int j, int k)
{
	if(i >= image->h || j >= image->w)
		return 0;
	else
		return ((unsigned char*)image->pixels)[i*image->w*3+j*3+k];
}

Terrain creer_terrain()
{
	SDL_Surface *prairies = IMG_Load("prairie.jpg");
	SDL_Surface *rocheuses = IMG_Load("pierres.jpg");
	SDL_Surface *neige = IMG_Load("neige.jpg");
	
	Clip olel("prairie.jpg", "pierres.jpg", "neige.jpg", NULL);
	
	TTerrain ter( Surface("heightmap.jpg"), &olel, 60, 130, 180, 220, 255);
	ter.creerTerrainTex(1);
	cout << "koko" << endl;
	ter.freeAll();
	
	/*Terrain le_terrain;
	
	cout << "pff" << endl;
	
	SDL_Surface *heightmap = IMG_Load("heightmap.jpg");
	SDL_Surface *terrain = SDL_CreateRGBSurface(SDL_HWSURFACE, 512, 512, 32, 0, 0, 0, 0);
	
	if(heightmap == NULL || prairies == NULL || rocheuses == NULL || neige == NULL)
	{
		cout << "Error in loading image" << endl;
		
		return le_terrain;
	}
	
	cout << "mimi" << endl;
	
	float perc[3];
	float b = 0, g = 0, r = 0;
	int x = 0, y = 0;
	
	vector< vector<unsigned char> > *hauteurs = new vector< vector<unsigned char> >(heightmap->h, vector<unsigned char>(heightmap->w) );
	
	for(y = 0; y < heightmap->h; y++)
	{
		for(x = 0; x < heightmap->w; x++)
		{
			hauteurs->operator[](y)[x] = Terrain_GetHauteur(heightmap, y, x, heightmap->h, heightmap->w );
		}	
	}
	cout << "ni" << endl;
	
	for(y = 0; y < terrain->h; y++)
	{
		for(x = 0; x < terrain->w; x++)
		{
			// Recuperation des participations des couleurs pour le pixel courant
			unsigned char hauteur = hauteurs->operator[]( (int)( (float)y * ((float)heightmap->h/terrain->h) ) ) [ (int)( (float)x * ((float)heightmap->w/terrain->w) ) ];// Terrain_GetHauteur(heightmap, y, x, terrain->w, terrain->h);
			
			Terrain_RemplitPerc(perc, hauteur);
//cout << "caycay" << endl;
			// Recuperation des couleurs par image de base
			b = perc[0] * Terrain_GetPixelColor(prairies,(int)( y*((float)prairies->h/(float)terrain->h) ), (int)( x*((float)prairies->w/(float)terrain->w) ), 0);
			g = perc[0] * Terrain_GetPixelColor(prairies,(int)( y*((float)prairies->h/(float)terrain->h) ), (int)( x*((float)prairies->w/(float)terrain->w) ), 1);
			r = perc[0] * Terrain_GetPixelColor(prairies,(int)( y*((float)prairies->h/(float)terrain->h) ), (int)( x*((float)prairies->w/(float)terrain->w) ), 2);
//cout << "boulay" << endl;
			b += perc[1] * Terrain_GetPixelColor(rocheuses, (int)(y*((float)rocheuses->h/(float)terrain->h) ), (int)( x*((float)rocheuses->w/(float)terrain->w) ), 0);
			g += perc[1] * Terrain_GetPixelColor(rocheuses, (int)(y*((float)rocheuses->h/(float)terrain->h) ), (int)( x*((float)rocheuses->w/(float)terrain->w) ), 1);
			r += perc[1] * Terrain_GetPixelColor(rocheuses, (int)(y*((float)rocheuses->h/(float)terrain->h) ), (int)( x*((float)rocheuses->w/(float)terrain->w) ), 2);
//cout << "flouay" << endl;
			b += perc[2] * Terrain_GetPixelColor(neige, (int)( y*((float)neige->h/(float)terrain->h) ), (int)( x*((float)neige->h/(float)terrain->h) ), 0);
			g += perc[2] * Terrain_GetPixelColor(neige, (int)( y*((float)neige->h/(float)terrain->h) ), (int)( x*((float)neige->h/(float)terrain->h) ), 1);
			r += perc[2] * Terrain_GetPixelColor(neige, (int)( y*((float)neige->h/(float)terrain->h) ), (int)( x*((float)neige->h/(float)terrain->h) ), 2);
			
//cout << "ouf" << endl;
			setPixel(terrain, Rect(x, y), SDL_MapRGBA(terrain->format, (int)r, (int)g, (int)b, 0) );
			
			//cout << x << " " << y << " ET " << r << " " << g << " " << b << endl;
		}
	}
	cout << "ope" << endl;
	SDL_FreeSurface(prairies);
	SDL_FreeSurface(rocheuses);
	SDL_FreeSurface(neige);
	SDL_FreeSurface(heightmap);
	cout << "fowl !" << endl;
	
	le_terrain.hauteurs = hauteurs;
	le_terrain.terrain = terrain;
	//le_terrain.terrain_tex = loadTexture(terrain);
	
	glEnable(GL_TEXTURE_2D);
	
	glGenTextures(1, &(le_terrain.terrain_tex) );
	glBindTexture(GL_TEXTURE_2D, le_terrain.terrain_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, le_terrain.terrain->w, le_terrain.terrain->h, GL_RGBA, GL_UNSIGNED_BYTE, le_terrain.terrain->pixels);
	
	SDL_FreeSurface(le_terrain.terrain);
	
	SDL_SaveBMP(le_terrain.terrain, "terrain.bmp");
	
	return le_terrain;*/
/*}

void dessiner_terrain(Terrain &terrain)
{
	//SDL_SaveBMP(terrain.terrain, "terrain.bmp");
	
	unsigned int x = 0, y = 0;
	float z = 0, posx = 0, posy = 0;
	
	float ajout = 40.0 / (float)MAX_POINTS;
	
	int hauteur = 24;
	
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, terrain.terrain_tex);

/*	glGenTextures(1, &(terrain.terrain_tex));
	glBindTexture(GL_TEXTURE_2D, terrain.terrain_tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, terrain.terrain->w, terrain.terrain->h, GL_RGB, GL_UNSIGNED_BYTE, terrain.terrain->pixels);*/
	/*

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_TRIANGLES);
	//glColor3ub(0, 255, 0);
	
	float facteur_y = terrain.hauteurs->size() / (float)MAX_POINTS;
	float facteur_x = terrain.hauteurs->front().size() / (float)MAX_POINTS;

	//glColor3ub(0, 255, 0);
	
	for(y = 0, posy = -ajout; y < MAX_POINTS - 1; y++, posy += ajout)
	{
		for(x = 0, posx = -ajout; x < MAX_POINTS - 1; x++, posx += ajout)
		{		    
		    //glTexCoord2f( (float)y/MAX_POINTS, (float)x/MAX_POINTS );
		    
		    cout << "MI : " << x << " " << y << endl;
		    
		    glTexCoord2f( x/(float)MAX_POINTS, y/(float)MAX_POINTS);
		    glVertex3f(posx, posy, terrain.hauteurs->operator[]( (int)(facteur_y*y) )[ (int)(facteur_x*x) ]/hauteur );
		    
		    glTexCoord2f( x/(float)MAX_POINTS, y/(float)MAX_POINTS+1/(float)MAX_POINTS);
			glVertex3f(posx, posy+ajout, terrain.hauteurs->operator[]( (int)( (facteur_y*y) + 1*facteur_y) )[ (int)(facteur_x*x) ]/hauteur );
			
			glTexCoord2f( x/(float)MAX_POINTS+1/(float)MAX_POINTS, y/(float)MAX_POINTS+1/(float)MAX_POINTS);
//		    if(x == terrain.hauteurs->front().size()-2 )
			glVertex3f(posx+ajout, posy+ajout, terrain.hauteurs->operator[]( (int)( (facteur_y*y) + 1*facteur_y) )[ (int)( (facteur_x*x) + 1*facteur_x) ]/hauteur );
//			else
//				glVertex3f(posx+ajout, posy+ajout, terrain.hauteurs->operator[](facteur_y*y)[facteur_x*x+1]/hauteur );
			

			glTexCoord2f( x/(float)MAX_POINTS, y/(float)MAX_POINTS);			
		    glVertex3f(posx, posy, terrain.hauteurs->operator[]( (int)(facteur_y*y) )[ (int)(facteur_x*x) ]/hauteur );
		    
			glTexCoord2f( x/(float)MAX_POINTS+1/(float)MAX_POINTS, y/(float)MAX_POINTS+1/(float)MAX_POINTS);
			glVertex3f(posx+ajout, posy+ajout, terrain.hauteurs->operator[]( (int)( (facteur_y*y) + 1*facteur_y) )[ (int)(facteur_x*x + 1*facteur_x) ]/hauteur );
			
			glTexCoord2f( x/(float)MAX_POINTS+1/(float)MAX_POINTS, y/(float)MAX_POINTS);
			glVertex3f(posx+ajout, posy, terrain.hauteurs->operator[]( (int)(facteur_y*y) )[ (int)( (facteur_x*x) + 1*facteur_x) ]/hauteur );
		}
	}
	
	glEnd();
}*/

void Dessiner()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

	camera->look();
    
	dessiner_repere(6);
	/*
	dessiner_sol(20, Pos(0, 0, 0), sol );
    dessiner_cube(1.5, Pos(0, -4, 0), glass, 1);
    //dessiner_cube(80, Pos(-40, -40, -0.1), mur, 20);
	dessiner_terre_et_satellite(Pos(-7, 0, 6), 3, 0, 60);
	//dessiner_terre_et_satellite(Pos(7, 0, 5), 3, 0, 35);*/
	//dessinerScene();
	
    glFlush();
    SDL_GL_SwapBuffers();
}
