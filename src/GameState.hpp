/////////////////////
//  GameState.hpp  //
/////////////////////

//Contiens la définition de classe GameState
//Cette classe contiens toutes les informations sur
//l'état courrant du jeu. Elle contiens aussi les methodes
//permettant le chargement d'un niveau depuis un fichier.

#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <iostream>
#include "AssetsManager.hpp"
#include "Button.hpp"
#include "Grid.hpp"
#include "Entity.hpp"

//Nombre max de portes différentes pouvant être en jeu en même temps
#define MAXDOOR 5

class GameState
{
    public:
        GameState(const sf::RenderWindow & window, const AssetsManager & assetsManager);
        ~GameState();

        //Permet de charger un niveau depuis un fichier
        void loadLevel(int levelNum);

        //accesseurs et mutateurs
        bool getInSelectionScreen() const {return inSelectionScreen;};
        void setInSelectionScreen(bool value) {inSelectionScreen = value;};

        const vector<Button> & getLevelSelectionButtonsConst() const {return levelSelectionButtons;};
        vector<Button> & getLevelSelectionButtons() {return levelSelectionButtons;};

        const vector<Entity *> & getEntitiesConst() const {return entities;};
        vector<Entity *> & getEntities() {return entities;};

        const TileGrid & getGrid() const {return *grid;};

        Player * getPlayer(bool playerOne) {return playerOne? player1 : player2;};

    private:
        //Crée tous les objets bouton de sélection
        void initLevelSelectionButtons();
        //Crée la grille des tuiles à partir d'un fichier de lecture
        void createGrid(ifstream & file);
        //Crée la liste des entitées à partir d'un fichier de lecture
        void createEntities(ifstream & file);
        //Libère les ressources mobilisées lors du chargement du précédent niveau
        void freeLevelRessources();

        
        const sf::RenderWindow & window;
        const AssetsManager & assetsManager;

        bool inSelectionScreen = true;

        vector<Button> levelSelectionButtons;
        
        TileGrid* grid = nullptr;
        vector<Entity *> entities;
        Player * player1, * player2;

};

#endif