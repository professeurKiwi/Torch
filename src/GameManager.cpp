#include "GameManager.hpp"
#include "Button.hpp"


GameManager::GameManager(sf::RenderWindow & window, GameState & gameState) : window(window), gameState(gameState)
{

}

void GameManager::update()
{
    if(gameState.getInSelectionScreen())
        updateLevelSelectionScreen();
    else
    {
        updateLevel();
    }
    


}

void GameManager::updateLevelSelectionScreen()
{
    sf::Vector2i cursorPos = sf::Mouse::getPosition(window); 

    //On met à jour tous les boutons, et on mémorise le niveau selectionné s'il y en a un
    int selectedLevel = -1;
    for(Button& button : gameState.getLevelSelectionButtons())
        if(button.updateSelect(cursorPos.x, cursorPos.y))
        {
            selectedLevel = button.getCorrespondingLevel();
            break;
        }


    //On gère les évènements
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        //si le bouton de la sourie est relevée alors qu'un bouton est survolé, alors le niveau est sélectionné
        else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && selectedLevel != -1)
        {
            gameState.setInSelectionScreen(false);
            gameState.loadLevel(selectedLevel);
            clock.restart();
        }
    }
}

void GameManager::updateLevel()
{
    
    static int milliseconds = 0;
    milliseconds += clock.restart().asMilliseconds();
    if(milliseconds >= FRAMERATE)
    {
        
        milliseconds -= FRAMERATE;
        for(Entity * entity : gameState.getEntities())
        {
            Animated * animated = dynamic_cast<Animated *>(entity);
            if(animated != nullptr)
            {
                animated->update();
            }
            PressurePlate* pressurePlate = dynamic_cast<PressurePlate *>(entity);
            if(pressurePlate != nullptr)
            {
                if(pressurePlate->updatePressed())
                    ((Door &)(pressurePlate->getLinkedDoor())).updateOpen();
            }
        }

        if(winTest(gameState.getPlayer(true)) || winTest(gameState.getPlayer(false)))
        {
            gameState.setInSelectionScreen(true);
            return;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            tryMoove(gameState.getPlayer(true), Direction::up);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            tryMoove(gameState.getPlayer(true), Direction::left);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            tryMoove(gameState.getPlayer(true), Direction::down);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            tryMoove(gameState.getPlayer(true), Direction::right);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::O))
            tryMoove(gameState.getPlayer(false), Direction::up);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::K))
            tryMoove(gameState.getPlayer(false), Direction::left);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
            tryMoove(gameState.getPlayer(false), Direction::down);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::M))
            tryMoove(gameState.getPlayer(false), Direction::right);
    }

    
        

    //On gère les évènements
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Escape)
            {
                gameState.setInSelectionScreen(true);
                return;
            }
            else if(event.key.code == sf::Keyboard::A)
            {
                if(!tryGive(gameState.getPlayer(true)))
                    tryDrop(gameState.getPlayer(true));
            }
            else if(event.key.code == sf::Keyboard::I)
            {
                if(!tryGive(gameState.getPlayer(false)))
                    tryDrop(gameState.getPlayer(false));
            }
        }
    }



}

void GameManager::tryMoove(Player * player, Direction direction)
{
    if(player->getIsMooving())
        return;
    sf::Vector2i coordonates = player->getCoordonates();
    switch (direction)
    {
        case Direction::up:
            coordonates += sf::Vector2i(0,-1);
            break;
        case Direction::down:
            coordonates += sf::Vector2i(0,1);
            break;
        case Direction::left:
            coordonates += sf::Vector2i(-1,0);
            break;
        case Direction::right:
            coordonates += sf::Vector2i(1,0);
            break;
    }
    if(gameState.getGrid()[coordonates.y][coordonates.x] != Tile::path && gameState.getGrid()[coordonates.y][coordonates.x] != Tile::goal)
    {
        player->look(direction);
        return;
    }
    for(Entity * entity : gameState.getEntities())
    {
        Door* door = dynamic_cast<Door *>(entity);
        Player* pl = dynamic_cast<Player *>(entity);
        if(entity->getCoordonates() == coordonates && 
        ((door != nullptr && !door->getOpen()) || pl != nullptr))
        {
            player->look(direction);
            return;
        }
        
        
    }
    player->moove(direction);
}

bool GameManager::tryGive(Player * giver)
{
    sf::Vector2i coordonates = giver->getCoordonates();
    switch (giver->getDirection())
    {
        case Direction::up:
            coordonates += sf::Vector2i(0,-1);
            break;
        case Direction::down:
            coordonates += sf::Vector2i(0,1);
            break;
        case Direction::left:
            coordonates += sf::Vector2i(-1,0);
            break;
        case Direction::right:
            coordonates += sf::Vector2i(1,0);
            break;
    }
    if(gameState.getGrid()[coordonates.y][coordonates.x] == Tile::piercedWall)
        switch (giver->getDirection())
        {
            case Direction::up:
                coordonates += sf::Vector2i(0,-1);
                break;
            case Direction::down:
                coordonates += sf::Vector2i(0,1);
                break;
        }

    for(Entity * entity : gameState.getEntities())
    {
        Player* pl = dynamic_cast<Player *>(entity);
        if(entity->getCoordonates() == coordonates && pl != nullptr && pl->getHoldenEntity() == nullptr)
        {
            pl->setHoldenEntity(giver->getHoldenEntity());
            giver->setHoldenEntity(nullptr);
            return true;
        }
    }
    return false;
}

void GameManager::tryDrop(Player * dropper)
{
    if(dropper->getHoldenEntity() == nullptr)
        return;
    for(Entity * entity : gameState.getEntities())
    {
        Door* door = dynamic_cast<Door *>(entity);
        if(entity->getCoordonates() == dropper->getCoordonates() && door != nullptr)
        {
            return;
        }
    }
    
    dropper->getHoldenEntity()->released();
    dropper->setHoldenEntity(nullptr);
}

bool GameManager::winTest(Player * player)
{
    if(dynamic_cast<Torch *>(player->getHoldenEntity()) == nullptr)
        return false;
    if(gameState.getGrid()[player->getCoordonates().y][player->getCoordonates().x] == Tile::goal)
        return true;
    return false;
}