#include "Camera.h"
#define ERROR 2000

int unicode_ifprintable(SDL_KeyboardEvent *key)
{
	// Cette fonction retourne la touche du clavier appuyée à partir d'un pointeur d'une variable SDL_KeyboardEvent, elle gère aussi bien le clavier QWERTY que le clavier AZERTY donc pas de problème chez n'importe quelle personne ayant un QWERTY ou AZERTY
	if( key->keysym.unicode < 0x80 && key->keysym.unicode > 0 )
        return key->keysym.unicode;
        
    else
    	return ERROR;
}

CameraFreeFly::CameraFreeFly(Pos position, Pos lookAt)
{
	SDL_ShowCursor(SDL_DISABLE); // Pour ne pas que la souris soit vue par l'utilisateur
    SDL_GrabMode error = SDL_WM_GrabInput(SDL_GRAB_ON); // SDL_WM_GrabInput sert à empêcher la souris de sortir des bords de la fenêtre
    
	if (error == SDL_GRAB_ON);
	else
	{
		cout << "Grab Fails [" << error << " " << SDL_GRAB_OFF << "]" << endl;
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	// Maintenant, nous allons calculer tout les cosinus et sinus des angles entiers entre 0 et 360 car les fonctions cos et sin sont un peu lentes, le mieux est de calculer tout au début
	// PS : Tout cela a pour but d'optimisation

	// On alloue dynamiquement les tableaux de cos et sin des angles entre 0° et 360°
	cos_angles = new float[361];
	sin_angles = new float[361];

	for(int angle = 0; angle < 361; angle++)
	{
		cos_angles[angle] = cos(angle*M_PI/180);
		sin_angles[angle] = sin(angle*M_PI/180);
	}

	m_position = Pos(position);
	m_lookAt = Pos(lookAt);
	m_difference = Pos(0, 0, 0);

	lookAt.x -= m_position.x;
	lookAt.y -= m_position.y;
	lookAt.z -= m_position.z;
	
	// On convertit de coordonnées cartésiennes à coordonnées sphériques pour savoir l'ange theta (entre X et Y) et l'angle phi (entre Z et X), pour en déduire l'orientation de la caméra
	
	double rayon = sqrt( (lookAt.x*lookAt.x) + (lookAt.y*lookAt.y) + (lookAt.z*lookAt.z) );
	
	m_phi = asin(lookAt.z / rayon);
/*	m_phi = m_phi/M_PI;
	m_phi = m_phi*180;

	if(m_phi > 89)
		m_phi = -89;
	if(m_phi < -89)
		m_phi = 89;

	m_theta = (double) ( lookAt.x / rayon / cos(m_phi*M_PI/180) );
	m_theta = acos(m_theta);
	m_theta = m_theta/M_PI;
	m_theta = m_theta*180;

*/
	m_theta = asin( lookAt.x / rayon*( cos(m_phi) ) ) / M_PI * 180;
	m_theta = lookAt.y < 0 ? 180 + m_theta : -m_theta;
	m_phi /= M_PI;
	m_phi *= 180;

//	m_theta = 0;
//	m_phi = 0;
	changeLookAt(); // Après avoir déduit les angles theta et phi on réoriente la caméra, juste pour vérification ;)

	m_speed = Speed(1, 0); // Vitesse initiale de la caméra
	m_speed.start();
	m_sensitivity = Speed(1, 0); // Sensitivité initiale de la caméra
	m_sensitivity.start();
	
	m_sdl_keys = new bool[SDLK_LAST]; // Tableau de bool des touches appuyées

	for(int i = 0; i < SDLK_LAST; i++)
		m_sdl_keys[i] = false;
}

CameraFreeFly::~CameraFreeFly()
{
	delete [] m_sdl_keys;
	delete [] cos_angles;
	delete [] sin_angles;

    SDL_WM_GrabInput(SDL_GRAB_OFF);
    SDL_ShowCursor(SDL_ENABLE);
}

void CameraFreeFly::SDLKeys(SDL_Event *event)
{
	int key;
			unsigned char ch = 0;
	
	switch(event->type)
	{
		case SDL_KEYDOWN:
			key = unicode_ifprintable(&event->key);
			unicode_table[event->key.keysym.sym] = event->key.keysym.unicode;
			
			if(key != ERROR)
				m_sdl_keys[key] = true;
			else
				cout << "Error in events of key." << endl;
		break;
		
		case SDL_KEYUP:
			m_sdl_keys[ unicode_table[event->key.keysym.sym] ] = false;
		break;
		
		default: break;
	};
}

void CameraFreeFly::input_handle(SDL_Event *event, Uint32 timestep)
{
	// Evénements de la souris
	
	switch(event->type)
	{
		case SDL_MOUSEMOTION:
			int mouse_xrel, mouse_yrel;

			SDL_GetRelativeMouseState(&mouse_xrel, &mouse_yrel);
			m_difference = Pos(mouse_xrel, mouse_yrel, 0);

			if(m_difference.x == 0 && m_difference.y == 0)
				break;
			
			changerAngles(); // Changer les angles en fonction de la différence de position de souris
			changeLookAt(); // Puisqu'on a change l'angle de la caméra, on doit changer le point qu'elle voit en fonction de ces angles
			
			m_difference = Pos(0, 0, 0);
		break;
		
		default:
			// Pour ne pas avoir de warning et aussi éviter que le switch ne sache pas quoi faire si les autres conditions sont fausses
		break;
	};
	
	// Evènements du clavier
	
	SDLKeys(event); // Pour remplir le tableau de booléens de touches activées ou non

	if(1)//m_speed.increase_or_not() == true)
	{
		if( !m_sdl_keys[SDLK_z] && !m_sdl_keys[SDLK_s] && !m_sdl_keys[SDLK_q] && !m_sdl_keys[SDLK_d] )
			m_speed.stop();
		else
		{
			m_speed.start();
			
			//float current_speed = m_speed.current_speed();
			float current_speed = m_speed.current_speed();
			if(current_speed > m_speed.m_distance)
				current_speed = m_speed.m_distance;
/*			if(timestep >= (float)m_speed.m_per_ms / m_speed.m_distance)
				current_speed = m_speed.m_distance;
			else
				current_speed = m_speed.m_distance * ( (float)timestep/( (float)m_speed.m_per_ms/m_speed.m_distance) );*/
	
			if(m_sdl_keys[SDLK_z] )
			{
				avancer( -current_speed );
				changeLookAt();
			}

			if(m_sdl_keys[SDLK_s] )
			{
				avancer( current_speed );
				changeLookAt();
			}

			if(m_sdl_keys[SDLK_d] )
			{
				double theta_old = m_theta;
				double phi_old = m_phi;

				if(m_theta <= 360-90)
					m_theta += 90;
				else
					m_theta = 90 - (360 - m_theta);
		
				m_phi = 0;

				avancer( current_speed );

				m_theta = theta_old;
				m_phi = phi_old;

				changeLookAt();
			}
	
			if(m_sdl_keys[SDLK_q] )
			{
				double theta_old = m_theta;
				double phi_old = m_phi;
	
				if(m_theta < 90)
					m_theta = 360 - (90 - m_theta);
				else if(m_theta >= 90)
					m_theta -= 90;

				m_phi = 0;
	
				avancer( current_speed );

				m_theta = theta_old;
				m_phi = phi_old;

				changeLookAt();
			}
		}
	}

}

void CameraFreeFly::av(float distance, int direction)
{
	if(direction == EN_AVANT)
	{
		avancer( -distance );
		changeLookAt();
	}

	if(direction == EN_ARRIERE)
	{
		avancer( distance );
		changeLookAt();
	}

	if(direction == DROITE)
	{
		double theta_old = m_theta;
		double phi_old = m_phi;

		if(m_theta <= 360-90)
			m_theta += 90;
		else
			m_theta = 90 - (360 - m_theta);

		m_phi = 0;

		avancer( distance );

		m_theta = theta_old;
		m_phi = phi_old;

		changeLookAt();
	}
	
	if(direction == GAUCHE)
	{
		double theta_old = m_theta;
		double phi_old = m_phi;

		if(m_theta < 90)
			m_theta = 360 - (90 - m_theta);
		else if(m_theta >= 90)
			m_theta -= 90;

		m_phi = 0;
	
		avancer( distance );

		m_theta = theta_old;
		m_phi = phi_old;

		changeLookAt();
	}
}

void CameraFreeFly::changerAngles()
{
	if(1)//m_sensitivity.increase_or_not() == true)
	{
//		m_difference.x *= m_sensitivity.current_speed();
//		m_difference.y *= m_sensitivity.current_speed();
		
		m_theta -= (m_difference.x * m_sensitivity.m_distance ); // m_difference est la différence entre la position de la souris du dernier SDL_MOUSEMOTION et de la nouvelle position de la souris

		if(m_theta > 360) // Si l'angle dépasse la limite, on le remet à 0
			m_theta = 0;
		else if(m_theta < 0)
			m_theta = 360;

		m_phi -= (m_difference.y * m_sensitivity.m_distance );
	
		if(m_phi < -88)
			m_phi = -88;
		else if(m_phi > 89) // Pour éviter certains problèmes
			m_phi = 89;

		m_difference = Pos(0, 0, 0);
	}
}

void CameraFreeFly::changeLookAt()
{
	// Réellement, ce n'est pas une rotation mais un changement du point que la caméra voit, mais c'est presque la même chose
	// Donc on initialise lookAt à la même position que la caméra puis on change encore une fois en fonction des angles
	m_lookAt.x = m_position.x;
	m_lookAt.y = m_position.y;
	m_lookAt.z = m_position.z;
	
	float rayon = 4;

	// Quelques calculs 
	// PS : m_theta est l'angle entre X et Y
	// et m_phi est l'angle entre Z et X (on peut dire aussi entre Z et Y, même chose)
	m_lookAt.x -= (double) ( rayon * (float)cos_angles[ (int)m_phi + 179 ] * (float)cos_angles[ (int)m_theta ] );
	m_lookAt.y -= (double) ( rayon * (float)cos_angles[ (int)m_phi + 179 ] * (float)sin_angles[ (int)m_theta ] );
	m_lookAt.z -= (double) ( rayon * (float)sin_angles[ (int)m_phi + 179 ] );
}

void CameraFreeFly::avancer(float distance)
{
	m_position.x += /*(double)distance * */(double)cos_angles[ (int)m_phi + 179 ] * (double)cos_angles[ (int)m_theta ] * distance;
	m_position.y += /*(double)distance * */(double)cos_angles[ (int)m_phi + 179 ] * (double)sin_angles[ (int)m_theta ] * distance;
	m_position.z += /*(double)distance * */(double)sin_angles[ (int)m_phi + 179 ] * distance;
}

void CameraFreeFly::look()
{
	gluLookAt(m_position.x, m_position.y, m_position.z, m_lookAt.x, m_lookAt.y, m_lookAt.z, 0, 0, 1);
}
		
void CameraFreeFly::setSensitivity(Speed sensitivity)
{
	m_sensitivity = sensitivity;
}

void CameraFreeFly::setSpeed(Speed speed)
{
	m_speed = speed;
}
