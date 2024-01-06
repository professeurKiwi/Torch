///////////////////////////
//  GraphicsManager.hpp  //
///////////////////////////

//Contiens la déscription de la classe GraphicsManager
//Cette classe n'est instanciée qu'une fois, elle gère l'affichage
//des différents éléments du jeu.

#ifndef GRAPHICSMANAGER_HPP
#define GRAPHICSMANAGER_HPP

#include <SFML/Graphics.hpp>
#include "GameState.hpp"
#include "AssetsManager.hpp"

class GraphicsManager
{
    public:
        GraphicsManager(sf::RenderWindow & window, const GameState & gameState, const AssetsManager & assetsManager);
        //Doit être appelée à chaque itération de la boucle principale pour raffréchir l'affichage de l'écran
        void displayScreen();

    private:
        //Appelée quand le jeu se trouve dans le menu de selection
        void displayLevelSelectionScreen();
        //Appelée quand le jeu se trouve dans un niveau
        void displayLevel();

        sf::RenderWindow & window;
        const GameState &  gameState;
        const AssetsManager &  assetsManager;

        sf::Sprite levelSelectionScreenBackground, levelBackground;
        sf::Text levelSelectionTitle;

};


#endif