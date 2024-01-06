#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <iostream>
#include "AssetsManager.hpp"
#include "Button.hpp"
#include "Grid.hpp"
#include "Entity.hpp"

#define MAXDOOR 5

class GameState
{
    public:
        GameState(const sf::RenderWindow & window, const AssetsManager & assetsManager);
        ~GameState();

        void loadLevel(int levelNum);

        bool getInSelectionScreen() const {return inSelectionScreen;};
        void setInSelectionScreen(bool value) {inSelectionScreen = value;};

        const vector<Button> & getLevelSelectionButtonsConst() const {return levelSelectionButtons;};
        vector<Button> & getLevelSelectionButtons() {return levelSelectionButtons;};

        const vector<Entity *> & getEntitiesConst() const {return entities;};
        vector<Entity *> & getEntities() {return entities;};

        const TileGrid & getGrid() const {return *grid;};

        Player * getPlayer(bool playerOne) {return playerOne? player1 : player2;};

    private:
        void initLevelSelectionButtons();
        void createGrid(ifstream & file);
        void createEntities(ifstream & file);
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