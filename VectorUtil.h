#ifndef VECTOR_UTILS
#define VECTOR_UTILS

#include <vector>

template<class T>
bool freeMatrix(std::vector <std::vector<T> > &matrix); // Free une matrix, elle fait appel � free vector de chaque ligne de la matrice

template<class T>
bool freeVector(std::vector<T> &vector); // Free un vector, le d�truit m�me :D

template <class T>
bool substr_matrice( std::vector< std::vector<T> > *source, std::vector< std::vector<T> > *destination, int debut_x, int debut_y, int nbr_cases_x, int nbr_cases_y); // "D�coupe" un morceau de la matrice source vers la matrice de destination

#include "VectorUtil.tpp" // L'extensions .tpp de ce fichier est faites pour pouvoir diff�rencier des fichiers d�finissant des fonctions templates ou classes templates des .cpp classiques

#endif
