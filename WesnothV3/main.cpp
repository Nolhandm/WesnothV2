#include <iostream>
#include "Utilitaire.hpp"
#include <SFML/Graphics.hpp>
#include "Types.hpp"
#include "Game.hpp"
#include "ctime"
#include "cstdlib"
#include "MCTS.hpp"

#define TILE_SIZE 80
using namespace std;

int main() {
    // crétion de la fenêtre de jeu
    sf::RenderWindow window(sf::VideoMode(1200, 800), "SFML Application", sf::Style::Close);
    //window.setVerticalSyncEnabled(true); Pour activer la synchro de la fréquence de rafraichissement avec celle de l'écran

    // création de la map

    int map2D[10][10] = {{4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
                         {4, 4, 4, 1, 4, 4, 4, 1, 4, 4},
                         {4, 4, 0, 4, 4, 4, 4, 4, 4, 4},
                         {4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
                         {4, 1, 4, 4, 4, 1, 4, 4, 4, 4},
                         {4, 4, 4, 4, 1, 4, 4, 4, 1, 4},
                         {4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
                         {4, 4, 4, 4, 4, 4, 4, 0, 4, 4},
                         {4, 4, 1, 4, 4, 4, 1, 4, 4, 4},
                         {4, 4, 4, 4, 4, 4, 4, 4, 4, 4},};

    // création de l'état initial
    State init;
    // conversion de la map en un tableau 1 dimension
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            init.map[i * 10 + j] = map2D[i][j];
        }
    }
    // création des héros
    Unit h1;
    Unit h2;

    h1.i = 0;
    h1.j = 0;
    h1.id =0;
    h1.pv = 20;
    h1.pa = 10;
    h1.hero = true;

    h2.i = 9;
    h2.j = 9;
    h2.id =1;
    h2.pv = 20;
    h2.pa = 10;
    h2.hero = true;

    init.player1[0] = h1;
    init.player2[1] = h2;

    init.nb_units = 2;
    init.nbVillagesJ1 =0;
    init.nbVillagesJ2 = 0;
    init.goldJ1 = 100;
    init.goldJ2 = 100;
    init.currentPlayer = 1;
    Game my_game(init);

    // Chargement de la police
    sf::Font font;
    font.loadFromFile("c:/Users/nolha/Documents/GitHub/WesnothV3/Font/arial.ttf");

    // chargement des textures pour les Map et les unités
    sf::Texture textureMap;
    textureMap.loadFromFile("C:/Users/nolha/Documents/GitHub/WesnothV3/idees_map/brouillon.png");
    sf::Texture textureUnit1;
    textureUnit1.loadFromFile("c:/Users/nolha/Documents/GitHub/WesnothV3/idees_map/perso.png");
    sf::Texture textureUnit2;
    textureUnit2.loadFromFile("c:/Users/nolha/Documents/GitHub/WesnothV3/idees_map/perso2.png");

    // contient l'id de l'unit selected
    int selected = -1 ;

    // initialisation des sprites
    sf::Sprite sprite_map[10*10];
    for(int i=0;i<10;i++){
        for(int j = 0; j<10;j++){
            sprite_map[i*10+j].setTexture(textureMap);
            sprite_map[i*10+j].setTextureRect(sf::IntRect(TILE_SIZE*init.map[i*10+j],0,TILE_SIZE,TILE_SIZE));
            pair<int,int> position = coordonnesIJtoXY(i,j);
            sprite_map[i*10+j].setPosition(position.first,position.second);
        }
    }
    map<int,sf::Sprite> spritePersos;
    for(pair unit : init.player1){
        sf::Sprite sprite;
        sprite.setTexture(textureUnit1);
        sprite.setTextureRect(sf::IntRect(0, TILE_SIZE*unit.second.hero, TILE_SIZE, TILE_SIZE));
        pair<int,int> position = coordonnesIJtoXY(unit.second.i,unit.second.j);
        sprite.setPosition(position.first,position.second);
        spritePersos[unit.first]= sprite;
    }
    for(pair unit : init.player2){
        sf::Sprite sprite;
        sprite.setTexture(textureUnit2);
        sprite.setTextureRect(sf::IntRect(0, TILE_SIZE*unit.second.hero, TILE_SIZE, TILE_SIZE));
        pair<int,int> position = coordonnesIJtoXY(unit.second.i,unit.second.j);
        sprite.setPosition(position.first,position.second);
        spritePersos[unit.first] = sprite;
    }

    while (window.isOpen()) {
        State actual = my_game.getState();
        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
        if(actual.currentPlayer==2){
            MCTS ai(my_game);
            Move m = ai.play();
            my_game.setState(my_game.next_state(actual,m));
        }
        else {
            sf::Event event;
            while (window.pollEvent(event)) {
                // gestion de la souris
                if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (my_game.current_player() == 1) {
                            for (pair unit: actual.player1) {
                                // on clique sur notre unité
                                if (hitbox(unit.second.i, unit.second.j, localPosition.x, localPosition.y)) {
                                    // on selectionne / deselectionne l'unité
                                    if (selected == -1) {
                                        selected = unit.first;
                                    } else {
                                        selected = -1;
                                    }
                                }
                                    // on clique sur une autre case
                                else if (selected == unit.first) {
                                    // case atteignable par l'unité
                                    for (pair dest: atteignable(unit.second.i, unit.second.j)) {
                                        if (hitbox(dest.first, dest.second, localPosition.x, localPosition.y)) {
                                            // on peut se déplacer sur la case ou attaquer (géré par next state de game)
                                            if (my_game.is_free(actual, dest.first, dest.second)) {
                                                Move m;
                                                m.unit = unit.second;
                                                m.i = dest.first;
                                                m.j = dest.second;
                                                my_game.setState(my_game.next_state(actual, m));
                                                selected = -1;
                                            }
                                        }
                                    }
                                }

                            }
                        }

                    }
                }
                if (event.type == sf::Event::KeyReleased) {
                    if (event.key.code == sf::Keyboard::N) {
                        if (my_game.current_player() == 1) {
                            if (my_game.canRecrute(actual)) {
                                // le hero de j1 a tjrs l'id 0
                                for (pair v: voisins(actual.player1[0].i, actual.player1[0].j)) {
                                    if (my_game.is_free(actual, v.first, v.second)) {
                                        Move m;
                                        m.unit = actual.player1[0];
                                        m.i = -1;
                                        m.j = -1;
                                        m.fi = v.first;
                                        m.fj = v.second;
                                        my_game.setState(my_game.next_state(actual, m));
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }
        }
        // update
        for(pair unit : my_game.getState().player1){
            spritePersos[unit.first].setTexture(textureUnit1);
            spritePersos[unit.first].setTextureRect(sf::IntRect(TILE_SIZE*(selected==unit.first), TILE_SIZE*unit.second.hero, TILE_SIZE, TILE_SIZE));
            pair<int,int> position = coordonnesIJtoXY(unit.second.i,unit.second.j);
            spritePersos[unit.first].setPosition(position.first,position.second);
        }
        for(pair unit : my_game.getState().player2){
            spritePersos[unit.first].setTexture(textureUnit2);
            spritePersos[unit.first].setTextureRect(sf::IntRect(TILE_SIZE*(selected==unit.first), TILE_SIZE*unit.second.hero, TILE_SIZE, TILE_SIZE));
            pair<int,int> position = coordonnesIJtoXY(unit.second.i,unit.second.j);
            spritePersos[unit.first].setPosition(position.first,position.second);
        }

        // on supprime les unités mortes des sprites
        for(int id : my_game.getState().deadunits){
            spritePersos.erase(spritePersos.find(id));
            my_game.suplisteMorts(id);
        }

        for(int i=0;i<10;i++){
            for(int j = 0; j<10;j++){
                sprite_map[i*10+j].setTextureRect(sf::IntRect(TILE_SIZE*my_game.getState().map[i*10+j],0,TILE_SIZE,TILE_SIZE));
            }
        }
        window.clear();
        // draw
        for(int i=0;i<10;i++) {
            for (int j = 0; j < 10; j++) {
                window.draw(sprite_map[i*10+j]);
            }
        }
        for(pair sprite : spritePersos){
            window.draw(sprite.second);
        }
        window.display();
        int win = my_game.win(my_game.getState());
        if(win != 0){
             window.close();
            std::cout << "win de : " << win << std::endl;
        }
    }

    return 0;
}
