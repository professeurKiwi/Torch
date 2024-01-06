#include "GraphicsManager.hpp"

GraphicsManager::GraphicsManager(sf::RenderWindow & window, const GameState & gameState, const AssetsManager & assetsManager) : 
levelSelectionScreenBackground(assetsManager.getTexture(TextureName::levelSelectionBackground)), 
levelBackground(assetsManager.getTexture(TextureName::levelBackground)),
levelSelectionTitle("Torch", assetsManager.getFont(), 200),
window(window),
gameState(gameState),
assetsManager(assetsManager)
{
    levelSelectionTitle.setOrigin(sf::Vector2f(levelSelectionTitle.getLocalBounds().width / 2, 3 * levelSelectionTitle.getLocalBounds().height / 4));
    levelSelectionTitle.setPosition(sf::Vector2f(window.getSize().x / 2, 70));
    levelSelectionTitle.setFillColor(sf::Color(200, 80, 60));
    levelSelectionTitle.setOutlineThickness(15);
    levelSelectionTitle.setOutlineColor(sf::Color(25,5,2));
    
}

void GraphicsManager::displayScreen()
{
    window.clear();

    if(gameState.getInSelectionScreen())
        displayLevelSelectionScreen();
    else
        displayLevel();

    window.display();
}

void GraphicsManager::displayLevel()
{
    window.draw(levelBackground);
    for(int i = 0; i < gameState.getGrid().getSize().y; i++)
    {
        window.draw(gameState.getGrid()[i]);
        for(const Entity * entity : gameState.getEntitiesConst())
        {
            if(entity->getDisplayLine() == i )
                window.draw(*entity);
        }
    }

}
void GraphicsManager::displayLevelSelectionScreen()
{


    window.draw(levelSelectionScreenBackground);
    for(const Button & button : gameState.getLevelSelectionButtonsConst())
        window.draw(button);
    window.draw(levelSelectionTitle);


}
