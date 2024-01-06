#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <SFML/Graphics.hpp>
#include "GameState.hpp"

#define FRAMERATE 7

class GameManager
{
    public:
        GameManager(sf::RenderWindow & window, GameState & gameState);
        void update();

    private:
        void tryMoove(Player * player, Direction direction);
        bool tryGive(Player * giver);
        void tryDrop(Player * dropper);
        bool winTest(Player * player);
        void updateLevelSelectionScreen();
        void updateLevel();

        sf::RenderWindow & window;
        GameState & gameState;
        sf::Clock clock;
};

#endif