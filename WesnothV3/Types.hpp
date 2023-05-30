
#ifndef WESNOTHV3_TYPES_HPP
#define WESNOTHV3_TYPES_HPP

#include <map>
#include <list>

typedef struct {
    int id;
    int pv;
    int pa;
    int i;
    int j;
    bool hero;
}Unit;

typedef struct {
    int map[10*10];
    std::map<int,Unit> player1;
    std::map<int,Unit> player2;
    int currentPlayer;
    int nb_units;
    int nbVillagesJ1;
    int nbVillagesJ2;
    int goldJ1;
    int goldJ2;
    // permets d'effacer les sprites
    std::list<int> deadunits;
}State;

typedef struct {
    int i;
    int j;
    Unit unit;
    // utilisé pour savoir ou former la troupe
    int fi;
    int fj;
}Move;
#endif //WESNOTHV3_TYPES_HPP
