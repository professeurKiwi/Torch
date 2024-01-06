////////////////
//  Grid.hpp  //
////////////////

//Contiens l'implémentation des méthodes des classes TileLine et TileGrid

#include "Grid.hpp"

TileLine::TileLine(const AssetsManager & assestsManager, const vector<vector<Tile>> & grid, int lineNb, const sf::Vector2f & bottomLeftCoordonate) : line(grid[lineNb])
{
    renderTexture = new sf::RenderTexture();

    renderTexture->create(16 * grid.at(0).size(), 32);
    renderTexture->setSmooth(false);
    renderTexture->clear(sf::Color(0,0,0,0));
    
    //pour chaque tuile de la ligne, on affiche le sprite correspondant sur la renderTexture
    for(int i = 0; i <  line.size(); i++)
    {
        sf::Sprite spriteTmp;
        switch (line[i])
        {
        case Tile::path:
            spriteTmp.setTexture(assestsManager.getTexture(TextureName::path));
            break;
        case Tile::piercedWall:
            spriteTmp.setTexture(assestsManager.getTexture(TextureName::piercedWall));
            break;
        case Tile::goal:
            spriteTmp.setTexture(assestsManager.getTexture(TextureName::goal));
            break;
        case Tile::wall:
            if(grid.size() > lineNb + 1 && grid[lineNb + 1][i] == Tile::wall)
                spriteTmp.setTexture(assestsManager.getTexture(TextureName::fullWall));
            else
                spriteTmp.setTexture(assestsManager.getTexture(TextureName::wall));
            break;
        }
        spriteTmp.setOrigin(sf::Vector2f(0, spriteTmp.getLocalBounds().height));
        spriteTmp.setPosition(i * 16, 32);
        renderTexture->draw(spriteTmp);
    }
    renderTexture->display();

    //On crée le sprite de la ligne avec la texture
    sprite = sf::Sprite(renderTexture->getTexture());

    sprite.setOrigin(sf::Vector2f(0, 32));
    sprite.setPosition(bottomLeftCoordonate);
    sprite.setScale(4,4);
}


void TileLine::draw(sf::RenderTarget& target, sf::RenderStates) const
{
    target.draw(sprite);    
}


TileGrid::TileGrid(const sf::RenderWindow & window, const AssetsManager & assestsManager, vector<vector<Tile>> & gr) : grid(gr)
{
    size = sf::Vector2i(grid.at(1).size(), grid.size());
    topLeftCoordonnates = sf::Vector2f(window.getSize().x / 2 - 64 * size.x /2, window.getSize().y / 2 - 64 * size.y / 2);


    lines.clear();
    for(int i = 0; i < grid.size(); i++)
    {
        lines.emplace_back(assestsManager, grid, i, topLeftCoordonnates + sf::Vector2f(0, 64 * (i + 1)));
    }
}

TileGrid::~TileGrid()
{
    for(TileLine line : lines)
    {
        line.freeResources();
    }
}

void TileGrid::print(ostream & stream) const
{
    stream << "dimentions: " << size.x << "x" << size.y << "\n";
    for(int i = 0; i < size.y; i++)
    {
        for(int j = 0; j < size.x; j++)
        {
            switch(grid[i][j])
            {
                case Tile::wall:
                    stream << 'W';
                    break;
                case Tile::goal:
                    stream << '~';
                    break;
                case Tile::piercedWall:
                    stream << '|';
                    break;
                case Tile::path:
                    stream << ' ';
                    break;
            }
        }
        stream << '\n';
    }
}