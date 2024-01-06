#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <filesystem>
#include <iostream>
#include <unistd.h>
#include <string.h>

#include "AssetsManager.hpp"
#include "GameState.hpp"
#include "GraphicsManager.hpp"
#include "GameManager.hpp"


void changeWorkingDirectory()
{
    try
    {
        char buff[100];
        int num = readlink("/proc/self/exe", buff, 95);
        //On supprime le nom de l'executable (Torch) du path
        string str(buff, 0, num);
        str = str.substr(0, str.find_last_of('/'));
        std::filesystem::current_path(str);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

int main()
{
    {
        //On rend le chemin du répertoire de travail égal au chemin de l'executable 
        //(pour que les chemins relatifs fonctionnent même quand l'executable est lancé depuis un autre répertoire)
        changeWorkingDirectory();

        //création de l'objet qui gère les assets, et chargement des assets
        AssetsManager assetsManager;
        if(!assetsManager.loadAssets())
            return -1;

        //Taille de la fenêtre correspondant à une grille de 28 par 15 tuiles.
        sf::RenderWindow window(sf::VideoMode(64 * 28,64 * 15), "Torch", sf::Style::Titlebar | sf::Style::Close);

        window.setFramerateLimit(60);

        //Création de l'objet qui gère l'état du jeu
        GameState gameState(window, assetsManager);

        //Création de l'objet qui gère l'affichage du jeu
        GraphicsManager graphicsManager(window, gameState, assetsManager);

        //Création de l'objet qui gère les évènements et l'évolution du jeu
        GameManager gameManager(window, gameState);


        while (window.isOpen())
        {
            //Gérer les évènements, faire avancer l'état du jeu
            gameManager.update();
            //Afficher le jeu dans son état actuel
            graphicsManager.displayScreen();
        }
    }
    for(int i = 0; i <10000; i++);
    return 0;
}