
#ifndef WESNOTHV3_GAME_HPP
#define WESNOTHV3_GAME_HPP


#include "Types.hpp"
#include "list"
#include "Utilitaire.hpp"
class Game {
public:
    Game(State s);
    Game();
    State getState();
    void setState(State s);
    int current_player();
    // les moves sont définis par la case d'arrivée, pour une formation case -1,-1
    State next_state(State s, Move move);
    // renvoie faux si une troupe de joueur courant est sur la case
    bool is_free(State s, int i, int j);
    // renvoie l'id de l'unité (du joueur ennemi) sur cette case si présente sinon -1
    int is_ennemi(State s, int i, int j);
    std::list<Move> legal_plays(State s);
    bool canRecrute(State s);
    // permet de vider la liste manageant les sprites d'unité mortes une fois traité
    void suplisteMorts(int id);
    int getStateValue(State s, int player);
    // retourne l'id du joueur gagnant ou 0 si pas fini
    int win(State s);
private:
    State actual_state;
};


#endif //WESNOTHV3_GAME_HPP
