/////////////////////
//  GameState.cpp  //
/////////////////////

//Contiens l'implémentation des méthodes de la classe GameState

#include "GameState.hpp"
#include "Button.hpp"
#include <fstream>
#include "Entity.hpp"

using namespace std;

//Différentes couleurs  de portes et boutons associés
sf::Color doorColors[MAXDOOR] =
{
    sf::Color(180,255,255),
    sf::Color(255,180,255),
    sf::Color(255,255,180),
    sf::Color(200,200,255),
    sf::Color(200,255,150)
};

GameState::GameState(const sf::RenderWindow & window, const AssetsManager & assetsManager) : window(window), assetsManager(assetsManager)
{
    initLevelSelectionButtons();
}

void GameState::initLevelSelectionButtons()
{
    
    levelSelectionButtons.push_back(Button(146, 500, 1, assetsManager));
    levelSelectionButtons.push_back(Button(296, 700, 2, assetsManager));
    levelSelectionButtons.push_back(Button(446, 500, 3, assetsManager));
    levelSelectionButtons.push_back(Button(596, 700, 4, assetsManager));
    levelSelectionButtons.push_back(Button(746, 500, 5, assetsManager));
    levelSelectionButtons.push_back(Button(896, 700, 6, assetsManager));
    levelSelectionButtons.push_back(Button(1046, 500, 7, assetsManager));
    levelSelectionButtons.push_back(Button(1196, 700, 8, assetsManager));
    levelSelectionButtons.push_back(Button(1346, 500, 9, assetsManager));
    levelSelectionButtons.push_back(Button(1496, 700, 10, assetsManager));
    levelSelectionButtons.push_back(Button(1646, 500, 11, assetsManager));
    
}

void GameState::loadLevel(int levelNum)
{
    //On libère les ressources du niveau précédent
    freeLevelRessources();

    //On charge le fichier du niveau
    ifstream file("Assets/Levels/" + to_string(levelNum) + ".torch");

    if(!file.is_open())
        throw exception();

    //Création de la grille
    createGrid(file);

    //On reviens au début du fichier
    file.clear();
    file.seekg(0);
    if(!file.good())
        throw exception();

    //Création de la liste des entités
    createEntities(file);
    
}

void GameState::createGrid(ifstream & file)
{
    char ch;
    vector<vector<Tile>> vectorGrid;
    vectorGrid.push_back(vector<Tile>());
    int i = 0;
    
    //On lit chaque caractère et on associe la tuile correspondante.
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

    //création de la grille
    grid = new TileGrid(window, assetsManager, vectorGrid);
}


void GameState::createEntities(ifstream & file)
{
    //Tableau des portes
    Door * doors[MAXDOOR];
    for(int i = 0; i < MAXDOOR; i++)
        doors[i] = nullptr;
    
    //Vecteurs des plaques de pressions correspondant aux portes
    vector<PressurePlate *> pressurePlates[MAXDOOR];

    //Vecteur des jarres
    vector<Jar *> jars;
    //La torche
    Torch* torch;



    char ch;
    int x, y = 0;
    //Pour chaque caractère, s'il correspond à une entitée, on la crée,
    //et on stoque un pointeur vers elle dans le conteneur addapté, ci dessus
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

    //On met toutes les entités crées dans le vecteur d'entité, dans l'ordre d'affichage.
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


    //On associe les portes avec les plaques de pression, et on met à jour leur état
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