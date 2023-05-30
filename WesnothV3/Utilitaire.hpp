
#ifndef WESNOTHV3_UTILITAIRE_HPP
#define WESNOTHV3_UTILITAIRE_HPP
#define TILE_SIZE 80
#include <list>
#include "Types.hpp"

// voisins directs d'une position
// attention aux retours de coordonnées hors de la map à gérer
std::list<std::pair<int,int>>voisins(int i, int j);

// case atteignable par une unité depuis une position
std::list<std::pair<int,int>> atteignable(int i, int j);

std::pair<int,int> coordonnesIJtoXY(int i,int j);

bool hitbox(int i,int j,int x,int y);

#endif //WESNOTHV3_UTILITAIRE_HPP
