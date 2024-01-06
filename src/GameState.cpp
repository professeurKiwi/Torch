
#include "GameState.hpp"
#include "Button.hpp"
#include <fstream>
#include "Entity.hpp"

using namespace std;

sf::Color doorColors[MAXDOOR] =
{
    sf::Color(200,255,255),
    sf::Color(255,200,255),
    sf::Color(255,255,200),
    sf::Color(220,220,255),
    sf::Color(220,255,220)
};

GameState::GameState(const sf::RenderWindow & window, const AssetsManager & assetsManager) : window(window), assetsManager(assetsManager)
{
    initLevelSelectionButtons();
}

void GameState::initLevelSelectionButtons()
{
    
    levelSelectionButtons.push_back(Button(100, 500, 1, assetsManager));
    levelSelectionButtons.push_back(Button(250, 700, 2, assetsManager));
    levelSelectionButtons.push_back(Button(400, 500, 3, assetsManager));
    levelSelectionButtons.push_back(Button(550, 700, 4, assetsManager));
    levelSelectionButtons.push_back(Button(700, 500, 5, assetsManager));
    levelSelectionButtons.push_back(Button(850, 700, 6, assetsManager));
    levelSelectionButtons.push_back(Button(1000, 500, 7, assetsManager));
    levelSelectionButtons.push_back(Button(1150, 700, 8, assetsManager));
    levelSelectionButtons.push_back(Button(1300, 500, 9, assetsManager));
    levelSelectionButtons.push_back(Button(1450, 700, 10, assetsManager));
    levelSelectionButtons.push_back(Button(1600, 500, 11, assetsManager));
    
}

void GameState::loadLevel(int levelNum)
{
    freeLevelRessources();

    ifstream file("Assets/Levels/" + to_string(levelNum) + ".torch");

    if(!file.is_open())
        throw exception();

    createGrid(file);

    file.clear();
    file.seekg(0);
    if(!file.good())
        throw exception();

    createEntities(file);
    
}

void GameState::createGrid(ifstream & file)
{
    char ch;
    vector<vector<Tile>> vectorGrid;
    vectorGrid.push_back(vector<Tile>());
    int i = 0;
    
    while(!file.eof())
    {
        
        ch = file.get();
        switch (ch)
        {
            case '\n':
                vectorGrid.emplace_back(vector<Tile>());
                i++;
                break;
            case 'W':
                vectorGrid[i].emplace_back(Tile::wall);
                break;
            case '|':
                vectorGrid[i].emplace_back(Tile::piercedWall);
                break;
            case '~':
                vectorGrid[i].emplace_back(Tile::goal);
                break;
            default:
                vectorGrid[i].emplace_back(Tile::path);
                break;
        }
    }
    //une ligne de trop est lue à cause du caractère de terminaison, on la retire.
    vectorGrid.pop_back();

    grid = new TileGrid(window, assetsManager, vectorGrid);
}


void GameState::createEntities(ifstream & file)
{
    Door * doors[MAXDOOR];
    for(int i = 0; i < MAXDOOR; i++)
        doors[i] = nullptr;
    
    vector<PressurePlate *> pressurePlates[MAXDOOR];

    vector<Jar *> jars;
    Torch* torch;



    char ch;
    int x, y = 0;
    while(!file.eof())
    {
        ch = file.get();
        switch (ch)
        {
            case '\n':
                y++;
                x = -1; //-1 pour compenser l'incrément de fin de boucle, et retomber sur 0
                break;
            case 'W':
            case '~':
            case '|':
            case ' ':
                break;
            case '1':
                {
                    torch = new Torch(assetsManager, sf::Vector2i(x,y), grid->getDrawOrigin());
                    player1 = new Player(assetsManager, sf::Vector2i(x,y), grid->getDrawOrigin(), true, torch, entities);
                }
                
                break;
            case '2':
                player2 = new Player(assetsManager, sf::Vector2i(x,y), grid->getDrawOrigin(), false, nullptr, entities);
                break;
            case '*':
                jars.emplace_back(new Jar(assetsManager, sf::Vector2i(x,y),grid->getDrawOrigin()));
                break;
            default:
                if(ch > 96) //caractère minuscure -> plaque de pression
                {
                    int index = ch - 97; //a = 0, b = 1, etc...
                    if(doors[index] == nullptr)
                        doors[index] = new Door(assetsManager, sf::Vector2i(0,0), grid->getDrawOrigin(), doorColors[index]);
                    pressurePlates[index].emplace_back(new PressurePlate(assetsManager, sf::Vector2i(x,y), grid->getDrawOrigin(), *doors[index],  doorColors[index], entities));
                
                }
                else if(ch > 64)//caractère majuscule -> porte
                {
                    int index = ch -65; //A = 0, B = 1, etc...
                    if(doors[index] == nullptr)
                        doors[index] = new Door(assetsManager, sf::Vector2i(x,y), grid->getDrawOrigin(), doorColors[index]);
                    else
                    {
                        doors[index]->setCoordonates(sf::Vector2i(x,y));
                    }
                }
                break;
        }
        x++;
    }
    for(int i = 0; i < MAXDOOR; i++)
    {
        if(doors[i] != nullptr)
        {
            entities.push_back(doors[i]);
            for(PressurePlate * pressurePlate : pressurePlates[i])
                entities.push_back(pressurePlate);
        }
        
    }
    entities.push_back(torch);
    for(Jar * jar : jars)
        entities.push_back(jar);
    entities.push_back(player1);
    entities.push_back(player2);


    for(int i = 0; i < MAXDOOR; i++)
    {
        if(doors[i] != nullptr)
        {
            for(PressurePlate* pp : pressurePlates[i])
            {
                doors[i]->addPressurePlate(pp);
                pp->updatePressed();
            }
            doors[i]->updateOpen();
        }
    }

    return;
}

void GameState::freeLevelRessources()
{
    if(grid != nullptr)
        delete grid;
    grid = nullptr;
    for(int i = 0; i < entities.size(); i++)
    {
        delete entities[i];
    }
    entities.clear();
}

GameState::~GameState()
{
    freeLevelRessources();
}