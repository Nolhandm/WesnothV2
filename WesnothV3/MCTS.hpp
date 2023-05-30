

#ifndef WESNOTHV3_MCTS_HPP
#define WESNOTHV3_MCTS_HPP


#include "Game.hpp"
#include "Node.hpp"
#include "list"
#include "cstdlib"
#include "ctime"

class MCTS {
public:
    MCTS();
    MCTS(Game g);
    void simulation(Node* init);
    Move play();

private:
    Game game;
    Node racine;
};


#endif //WESNOTHV3_MCTS_HPP
