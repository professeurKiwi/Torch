/////////////////////////
//  AssetsManager.hpp  //
/////////////////////////

//Contiens la déscription de la classe AssetManager
//cette classe permet de charger toutes les ressources du jeu,
//et rend ces ressources accessibles au reste du programme.

#ifndef ASSETS_MANAGER
#define ASSETS_MANAGER

#include <SFML/Graphics.hpp>
#include <string.h>

using namespace std;

//contiens les noms de toutes les textures qui peuvent être demandées à AssetsManager via getTexture
enum class TextureName {
    levelSelectionBackground,
    levelBackground,
    buttonBackground,
    buttonBorder,
    wall,
    fullWall,
    piercedWall,
    path,
    goal,
    jar,
    torch,
    door,
    pressurePlate,
    player1,
    player1WithTorch,
    player1WithJar,
    player1Mooving,
    player1MoovingWithTorch,
    player1MoovingWithJar,
    player2,
    player2WithTorch,
    player2WithJar,
    player2Mooving,
    player2MoovingWithTorch,
    player2MoovingWithJar};

class AssetsManager
{
    public :
        //doit être appelé une fois au début du programme pour charger les ressources. Renvoie true si le chargement à réussi.
        bool loadAssets();
        //permet de récupérer un pointeur en lecture seule vers une texture dont le nom est donné en paramètre
        const sf::Texture & getTexture(TextureName name) const {return textures.at(name);};
        const sf::Font & getFont() const {return mainFont;};
    private :
        bool loadTexture(TextureName name, string path);
        map<TextureName, sf::Texture> textures;
        sf::Font mainFont;


};

#endif