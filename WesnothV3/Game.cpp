
#include "Game.hpp"

using namespace std;

Game::Game(State s) {
    actual_state = s;
}
Game::Game() {}
State Game::getState() {
    return actual_state;
}

void Game::setState(State s) {
    actual_state = s;
}

int Game::current_player() {
    return actual_state.currentPlayer;
}

State Game::next_state(State s, Move move) {

    State res = s;

    if(move.i == -1 and move.j == -1){
        // formation
        if(res.currentPlayer==1){
            Unit u;
            u.pv = 10;
            u.pa = 5;
            u.i = move.fi;
            u.j = move.fj;
            u.id = res.nb_units;
            u.hero = false;
            res.player1[res.nb_units] = u;
            res.nb_units++;
            res.goldJ1-=20;
        }else{
            Unit u;
            u.pv = 10;
            u.pa = 5;
            u.i = move.fi;
            u.j = move.fj;
            u.id = res.nb_units;
            u.hero = false;
            res.player2[res.nb_units] = u;
            res.nb_units++;
            res.goldJ2-=20;
        }
    }
    else {

        int id_ennemi = is_ennemi(res, move.i, move.j);
        // case libre donc déplacement
        if (id_ennemi == -1) {
            if (res.currentPlayer == 1) {
                res.player1[move.unit.id].i = move.i;
                res.player1[move.unit.id].j = move.j;
                // 1 correspond à un village vide et 3 à un village J2
                if (res.map[move.i * 10 + move.j] == 1 or res.map[move.i * 10 + move.j] == 3) {
                    res.map[move.i * 10 + move.j] = 2;
                    res.nbVillagesJ2--;
                    res.nbVillagesJ1++;
                }
            } else {
                res.player2[move.unit.id].i = move.i;
                res.player2[move.unit.id].j = move.j;
                // 1 correspond à un village vide et 2 à un village J1
                if (res.map[move.i * 10 + move.j] == 1 or res.map[move.i * 10 + move.j] == 2) {
                    res.map[move.i * 10 + move.j] = 3;
                    res.nbVillagesJ1--;
                    res.nbVillagesJ2++;
                }
            }
        }
            // case non libre donc attaque
        else {
            if (res.currentPlayer == 1) {
                res.player2[id_ennemi].pv -= move.unit.pa;
                // l'unité est morte ?
                if (res.player2[id_ennemi].pv <= 0) {
                    res.player2.erase(res.player2.find(id_ennemi));
                    res.deadunits.push_back(id_ennemi);
                }
            } else {
                res.player1[id_ennemi].pv -= move.unit.pa;
                if (res.player1[id_ennemi].pv <= 0) {
                    res.player1.erase(res.player1.find(id_ennemi));
                    res.deadunits.push_back(id_ennemi);
                }
            }
        }
    }
    // calcul de l'or pour le prochain joueur(inverse de actual player)
    if(res.currentPlayer==1){
        res.goldJ2 = res.goldJ2 + 2 + res.nbVillagesJ2 - max(0,(int)res.player2.size()-res.nbVillagesJ2);
    }
    else{
        res.goldJ1 = res.goldJ1 + 2 + res.nbVillagesJ1 - max(0,(int)res.player1.size()-res.nbVillagesJ1);
    }
    // changement de joueur
    if(res.currentPlayer==1) {
        res.currentPlayer=2;
    }
    else {
        res.currentPlayer=1;
    }

    return res;

}

bool Game::is_free(State s, int i, int j) {
    if(s.currentPlayer == 1){
        for(pair unit : s.player1){
            if(unit.second.i == i and unit.second.j == j)return false;
        }
    }
    else {
        for(pair unit : s.player2){
            if(unit.second.i == i and unit.second.j == j)return false;
        }
    }
    return true;
}

int Game::is_ennemi(State s, int i, int j) {
    if(s.currentPlayer == 1){
        for(pair unit : s.player2){
            if(unit.second.i == i and unit.second.j == j)return unit.first;
        }
    }
    else {
        for(pair unit : s.player1){
            if(unit.second.i == i and unit.second.j == j)return unit.first;
        }
    }
    return -1;
}

list<Move> Game::legal_plays(State s) {
    list<Move> res;
    if(s.currentPlayer==1){
        for(pair unit : s.player1){
            for(pair v : atteignable(unit.second.i,unit.second.j)){
                if(is_free(s,v.first,v.second)){
                    Move m;
                    m.unit = unit.second;
                    m.i = v.first;
                    m.j = v.second;
                    res.push_back(m);
                }

            }
        }
        // le héro a toujours l'id 0 pour J1
        if(canRecrute(s)){
            for(pair v : voisins(s.player1[0].i,s.player1[0].j)){
                if(is_free(s,v.first,v.second)){
                    Move m;
                    m.unit = s.player1[0];
                    m.i = -1;
                    m.j = -1;
                    m.fi = v.first;
                    m.fj = v.second;
                    res.push_back(m);
                    break;
                }
            }
        }
    }
    else{
        for(pair unit : s.player2){
            for(pair v : atteignable(unit.second.i,unit.second.j)){
                if(is_free(s,v.first,v.second)){
                    Move m;
                    m.unit = unit.second;
                    m.i = v.first;
                    m.j = v.second;
                    res.push_back(m);
                }

            }
            // le héro a toujours l'id 1 pour J2
            if(canRecrute(s)){
                for(pair v : voisins(s.player2[1].i,s.player2[1].j)){
                    if(is_free(s,v.first,v.second)){
                        Move m;
                        m.unit = s.player2[1];
                        m.i = -1;
                        m.j = -1;
                        m.fi = v.first;
                        m.fj = v.second;
                        res.push_back(m);
                        break;
                    }
                }
            }
        }
    }

    return res;
}

bool Game::canRecrute(State s) {
    if(s.currentPlayer == 1){
        // 0 correspond au chateau
        return s.map[s.player1[0].i*10+s.player1[0].j]==0 and s.goldJ1>=20;
    }else{
        // 0 correspond au chateau
        return s.map[s.player2[0].i*10+s.player2[0].j]==0 and s.goldJ2>=20;
    }

}

void Game::suplisteMorts(int id) {
    actual_state.deadunits.remove(id);
}

int Game::getStateValue(State s, int player) {
    if(player==1){
        return 100*(win(s) == 1) + 20*(s.player1.size()-s.player2.size())+ s.nbVillagesJ1*10;
    }
    else{
        return 100*(win(s) == 2) + 20*(s.player2.size()-s.player2.size())+ s.nbVillagesJ1*10;
    }
}

int Game::win(State s) {
    // On gagne si on est pas le player actuel (un move fini la partie et ça passe au joueur suivant)
    int res;
    if(s.currentPlayer==1){
        res = 2;
        for(pair unit : s.player1){
            if(unit.second.hero){
                res = 0;
            }
        }
    }
    else{
        res = 1;
        for(pair unit : s.player2){
            if(unit.second.hero){
                res = 0;
            }
        }
    }
    return res;
}