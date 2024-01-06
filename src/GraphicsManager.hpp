#ifndef GRAPHICSMANAGER_HPP
#define GRAPHICSMANAGER_HPP

#include <SFML/Graphics.hpp>
#include "GameState.hpp"
#include "AssetsManager.hpp"

class GraphicsManager
{
    public:
        GraphicsManager(sf::RenderWindow & window, const GameState & gameState, const AssetsManager & assetsManager);
        void displayScreen();

    private:
        void displayLevelSelectionScreen();
        void displayLevel();

        sf::RenderWindow & window;
        const GameState &  gameState;
        const AssetsManager &  assetsManager;

        sf::Sprite levelSelectionScreenBackground, levelBackground;
        sf::Text levelSelectionTitle;

};


#endif