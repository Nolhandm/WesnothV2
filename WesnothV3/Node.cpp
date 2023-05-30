

#include "Node.hpp"
using namespace std;
Node::Node(State s) {
    state = s;
    n = 0;
    t =0;
    parent = nullptr;
}

void Node::setMove(Move v) {
    move = v;
}

Node::Node() {

}

int Node::getT() {
    return t;
}

Move Node::getMove() {
    return move;
}

void Node::addFils(Node* n) {
    fils.push_back(n);
}

list<Node*> Node::getFils() {
    return fils;
}

void Node::setParent(Node *n) {
    parent = n;
}

Node* Node::getParent() {
    return parent;
}

State Node::getState() {
    return state;
}

int Node::getN() {
    return n;
}
void Node::increN() {
    if(parent!= nullptr){
        n++;
        parent->increN();
    }
    else{
        n++;
    }
}

void Node::addT(int add) {
    if(parent!= nullptr){
        t+=add;
        parent->addT(add);
    }
    else{
        t+=add;
    }
}

double Node::UCB1(int N) {
    if(n == 0){
        return -1;
    }
    return t/n + 2 * sqrt(log(N)/n);
}

Node::~Node() {
}

