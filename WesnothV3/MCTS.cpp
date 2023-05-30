
#include <iostream>
#include "MCTS.hpp"

using namespace std;
MCTS::MCTS(Game g) {
    game = g;
    Node node (g.getState());
    racine = node;
}

MCTS::MCTS() {

}

Move MCTS::play() {
    //init de l'arbre

    State actual = racine.getState();
    for(Move m : game.legal_plays(actual)){
        Node* node = new Node(game.next_state(actual,m));
        node->setMove(m);
        node->setParent(&racine);
        racine.addFils(node);
        node;
    }

    for(int i= 0; i<100;i++){
        simulation(&racine);
    }

    Node* final = nullptr;
    for(Node* n : racine.getFils()){
        if(final == nullptr){
            final = n;
        }else{
            if(n->getN()!=0){
                if(final->getT()/ final->getN() < n->getT()/n->getN()){
                    final = n;
                }
            }
        }
    }
    Move res = final->getMove();
    return res;

}

void MCTS::simulation(Node *init) {
    srand(time(NULL));
    int N = racine.getN();
    for(Node* n : init->getFils()) {
        // on est à une feuille donc on simule direct
        if (n->getN() == 0) {
            State state = n->getState();
            for (int nbcoup = 0; nbcoup < 15; nbcoup++) {
                list<Move> coups = game.legal_plays(state);
                if(coups.size()!=0){
                    int rand = std::rand() % coups.size();
                    for (int i = 0; i < rand; i++) {
                        coups.pop_back();
                        state = game.next_state(state, coups.back());
                    }
                }
            }
            // backpropagation (on va dire que l'ia est joueur 2)
            int v = game.getStateValue(state, 2);
            n->increN();
            n->addT(v);

            return;
        } else {// pas une feuille on choisit le noeud qu'on veut explorer
            // Node expansion
            if (n->getN() == 1) {
                State state = n->getState();
                for (Move m: game.legal_plays(state)) {
                    Node* node = new Node(game.next_state(state, m));
                    node->setMove(m);
                    node->setParent(init);
                    n->addFils(node);
                }
            }
            Node *nchoisi = nullptr;
            for (Node* pick: n->getFils()) {
                if (nchoisi == nullptr) {
                    nchoisi = n;
                } else {
                    if (pick->UCB1(N) > nchoisi->UCB1(N)) {
                        nchoisi = pick;
                    }
                }
            }
            simulation(nchoisi);
        }
    }
}
