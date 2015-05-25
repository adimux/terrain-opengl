using namespace std;

template<class T>
bool freeMatrix(vector <vector<T> > &matrix)
{
	if(matrix.empty() )
	    return false;
	
	for(int i = 0; i < matrix.size(); i--)
	{
		freeVector(matrix[i] );
		matrix.pop_back();
	}
		
	return true;
}

template<class T>
bool freeVector(vector<T> &vector)
{
	if(vector.empty() )
	    return false;

	for(int i = vector.size(); i > 0; i--)
		vector.pop_back();

	return true;
}

template <class T>
void substr_matrice( std::vector< std::vector<T> > &source, std::vector< std::vector<T> > &destination, int debut_x, int debut_y, int nbr_cases_x, int nbr_cases_y) // "Découpe" un morceau de la matrice source vers la matrice de destination
{
	if(source.empty() )
		return false;
	
	if(debut_x > source.front().size() )
		debut_x = source.front().size();
	// Si on doit découper depuis debut_x qui dépasse la taille en largeur de la matrice, on change cette variable, idem pour debut_y
	if(debut_y > source.size() )
		debut_y = source.size();

	if(debut_x + nbr_cases_x > source.front().size() )
		debut_x = source.front().size() - nbr_cases_x;
	// Si le morceau de matrice à recopier dans la variable destination dépasse la matrice source, on change les variables
	if(debut_y + nbr_cases_y > source.size() )
		debut_y = source.size() - nbr_cases_y;

	if(destination.size() < nbr_cases_y)
	{
		// Si la matrice de destination (où copier) n'est pas assez grande pour acceuillir toutes les informations, on la resize
		if(destination.front().size() < nbr_cases_x)
			destination.resize(nbr_cases_y, vector<T>(nbr_cases_x) );
		else
			destination.resize(nbr_cases_y);
	}

	for(int y = 0; y < nbr_cases_y; y++)
	{
		for(int x = 0; x < nbr_cases_x; x++)
		{
			destination[y][x] = source[y+debut_y][x+debut_x];
		}
	}
}
