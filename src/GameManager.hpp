///////////////////////
//  GameManager.hpp  //
///////////////////////

//Contiens la déscription de la classe GameManager
//cette classe n'est instanciée qu'une fois, et permet
//de gérer les évènements et de faire avancer le jeu.
//Elle agit notemment directement sur l'objet Gamestate.

#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <SFML/Graphics.hpp>
#include "GameState.hpp"

#define FRAMERATE 45

class GameManager
{
    public:
        GameManager(sf::RenderWindow & window, GameState & gameState) : window(window), gameState(gameState) { };

        //Doit être appelé à chaque itération de la boucle principale
        void update();

    private:

        //Appelée quand le jeu se trouve dans le menu de selection
        void updateLevelSelectionScreen();
        //Appelée quand le jeu se trouve dans un niveau
        void updateLevel();

        //Essaie de déplacer un joueur dans une direction donnée. 
        //Dans le cas où ce n'est pas possible, lui fait simplement regarder dans la direction.
        void tryMoove(Player * player, Direction direction);
        //Si le joueur en regarde un autre, donne son objet
        bool tryGive(Player * giver);
        //Si le joueur tiens un objet, le lâche
        void tryDrop(Player * dropper);
        //Teste si un joueur possède la torche et a atteint la sortie.
        bool winTest(Player * player);

        

        sf::RenderWindow & window;
        GameState & gameState;
        sf::Clock clock;
};

#endif