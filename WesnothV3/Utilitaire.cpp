

#include "Utilitaire.hpp"

using namespace std;

list<pair<int,int>>voisins(int i, int j){
    std::list<std::pair<int,int>> res;
    //ligne impaire
    if(i%2 != 0){
        if(i>0)res.emplace_back(i-1,j);
        if(j>0)res.emplace_back(i,j-1);
        if(i<10-1)res.emplace_back(i+1,j);
        if(i<10-1 and j<10-1)res.emplace_back(i+1,j+1);
        if(j<10-1)res.emplace_back(i,j+1);
        if(i>0 and j<10-1)res.emplace_back(i-1,j+1);
    }
    else{
        if(i>0 and j>0)res.emplace_back(i-1,j-1);
        if(j>0)res.emplace_back(i,j-1);
        if(i<10-1 and j>0)res.emplace_back(i+1,j-1);
        if(i<10-1) res.emplace_back(i+1,j);
        if(j<10-1)res.emplace_back(i,j+1);
        if(i>0)res.emplace_back(i-1,j);
    }
    return res;
}

list<pair<int,int>> atteignable(int i, int j){
    list<pair<int,int>> res;
    for(pair v: voisins(i,j)){
        for(pair v2 : voisins(v.first,v.second)){
            res.push_back(v2);
        }
    }
    res.sort();
    res.unique();
    return res;
}

pair<int,int>coordonnesIJtoXY(int i,int j){

    int t_x, t_y = 0;
    double pi = 3.14;

    pair<int,int> res ;

    // On soustrait en y une cst pour que les hexagones s'emboitent = tan(60°)*(T_SIZE/2)
    if (i % 2 == 0) {
        t_x = TILE_SIZE * j;
        t_y = TILE_SIZE * i - i * 15;//(int)(std::tan(pi/6)*TILE_SIZE*0.5);
    }
    else {
        t_x = TILE_SIZE * j + TILE_SIZE * 0.5;
        t_y = TILE_SIZE * i - i * 15;//(int)(std::tan(pi/6)*TILE_SIZE*0.5);
    }
    res.first = t_x;
    res.second = t_y;
    return res;
}
bool hitbox(int i, int j,int x, int y){
    pair<int,int> position = coordonnesIJtoXY(i,j);
    int t_x = position.first;
    int t_y = position.second;

    if (x >= t_x and x <= t_x + TILE_SIZE and y >= t_y and y <= t_y + TILE_SIZE) {
        if (x < t_x + TILE_SIZE / 2) {
            if (y >= 15 - (15 / TILE_SIZE * 0.5) * x and y <= TILE_SIZE - 15 + (15 / TILE_SIZE * 0.5) * x);
            return true;
        }
        else if (x >= t_x + TILE_SIZE / 2) {
            if (y >= 15 - (15 / TILE_SIZE * 0.5) * (TILE_SIZE - x) and
                y <= TILE_SIZE - 15 + (15 / TILE_SIZE * 0.5) * (TILE_SIZE - x));
            return true;
        }
    }
    return false;
}

