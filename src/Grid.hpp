////////////////
//  Grid.hpp  //
////////////////

//Contiens la définition des classes TileGrid et TileLine
//TileGrid représente la grille de tuiles qui compose le terrain d'un niveau
//TileLine resprésente une ligne de tuile, cette subdivision à de l'intéret car
//l'affichage de la grille se fait ligne par ligne

#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "AssetsManager.hpp"

using namespace std;

//Liste des tuiles composant un niveau
enum class Tile{
    wall,
    piercedWall,
    path,
    goal
};

class TileLine : public sf::Drawable
{
    public:
        TileLine(const AssetsManager & assestsManager, const vector<vector<Tile>> & grid, int lineNb, const sf::Vector2f & upLeftCoordonate);
  
        void draw(sf::RenderTarget & target, sf::RenderStates) const;

        void freeResources() { delete renderTexture; };

        //Permet de récupérer la nième tuile de la ligne
        const Tile & operator[](int i) const {return line[i];};

    private:

        //Cette renderTexture permet de générer un unique sprite pour l'affichage de toute la
        //ligne, de cette façon, on économise des ressources pour l'affichage
        sf::RenderTexture * renderTexture;
        sf::Sprite sprite;
        const vector<Tile> & line;
};

class TileGrid
{
    public:
        TileGrid(const sf::RenderWindow & window, const AssetsManager & assestsManager, vector<vector<Tile>> & grid);
        ~TileGrid();

        //accesseurs et mutateurs
        const sf::Vector2i & getSize() const {return size;};
        const vector<vector<Tile>> & getGrid() const {return grid;};
        const sf::Vector2f & getDrawOrigin() const {return topLeftCoordonnates;};

        //permet d'accéder à la nième ligne de la grille
        const TileLine & operator[](int i) const {return lines[i];};
        //permet d'afficher la grille dans un stream. Cette fonction peut être utilisée en débug pour vérifier la grille dans le terminal.
        //Exemple de sortie:
        //WWWWWWW
        //W     ~
        //W|WWWWW
        //W WWW W
        //W     W
        //WWWWWWW
        friend ostream& operator<< (std::ostream& stream, const TileGrid& tileGrid) { tileGrid.print(stream); return stream;};

    private:
        void print(ostream & stream) const;

        //taille (x,y) de la grille
        sf::Vector2i size;

        //Coordonnées de l'origine de la grille (pour que l'affichage de tous les niveaux soit centré, même si la grille ne prend pas tout l'écran)
        sf::Vector2f topLeftCoordonnates;

        //Attention! Il s'agit d'un tableau de lignes (l'affichage se fait ligne à ligne)
        //On parcours donc avec grid[coordY][coordX]
        vector<vector<Tile>> grid;

        //Tableau des objets TileLines servant à l'affichage
        vector<TileLine> lines;

};

#endif