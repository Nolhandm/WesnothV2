

#ifndef WESNOTHV3_NODE_HPP
#define WESNOTHV3_NODE_HPP
#include "Types.hpp"
#include "math.h"
class Node {
public:
    Node();
    Node(State s);
    void addFils(Node* n);
    std::list<Node*> getFils();
    Node* getParent();
    void setParent(Node* n);
    State getState();
    int getT();
    int getN();
    void addT(int add);
    void increN();
    double UCB1(int N);
    void setMove(Move v);
    Move getMove();
    ~Node();
private:
    int t;
    int n;
    State state;
    std::list<Node*> fils;
    Node* parent;
    Move move;
};


#endif //WESNOTHV3_NODE_HPP
