/////////////////////////
//  AssetsManager.hpp  //
/////////////////////////

//Contiens l'implémentation des méthodes de la classe AssetsManager


#include "AssetsManager.hpp"



bool AssetsManager::loadAssets()
{
    

    if (!mainFont.loadFromFile("Assets/GreekHouse Symbolized_Free.ttf"))
        return false;
    
    bool ret = true;

    ret &= loadTexture(TextureName::levelSelectionBackground, "Assets/LevelSelectionBackground.png");
    ret &= loadTexture(TextureName::levelBackground, "Assets/LevelBackground.png");
    ret &= loadTexture(TextureName::buttonBackground, "Assets/ButtonBackground.png");
    ret &= loadTexture(TextureName::buttonBorder, "Assets/ButtonBorder.png");
    ret &= loadTexture(TextureName::wall, "Assets/Tiles/Wall.png");
    ret &= loadTexture(TextureName::fullWall, "Assets/Tiles/FullWall.png");
    ret &= loadTexture(TextureName::path, "Assets/Tiles/Path.png");
    ret &= loadTexture(TextureName::piercedWall, "Assets/Tiles/PiercedWall.png");
    ret &= loadTexture(TextureName::goal, "Assets/Tiles/Goal.png");
    ret &= loadTexture(TextureName::jar, "Assets/Entities/Jar.png");
    ret &= loadTexture(TextureName::pressurePlate, "Assets/Entities/PressurePlate.png");
    ret &= loadTexture(TextureName::torch, "Assets/Entities/Torch.png");
    ret &= loadTexture(TextureName::door, "Assets/Entities/Door.png");
    ret &= loadTexture(TextureName::player1, "Assets/Players/Player1.png");
    ret &= loadTexture(TextureName::player1WithJar, "Assets/Players/Player1WithJar.png");
    ret &= loadTexture(TextureName::player1WithTorch, "Assets/Players/Player1WithTorch.png");
    ret &= loadTexture(TextureName::player1Mooving, "Assets/Players/Player1Mooving.png");
    ret &= loadTexture(TextureName::player1MoovingWithJar, "Assets/Players/Player1MoovingWithJar.png");
    ret &= loadTexture(TextureName::player1MoovingWithTorch, "Assets/Players/Player1MoovingWithTorch.png");
    ret &= loadTexture(TextureName::player2, "Assets/Players/Player2.png");
    ret &= loadTexture(TextureName::player2WithJar, "Assets/Players/Player2WithJar.png");
    ret &= loadTexture(TextureName::player2WithTorch, "Assets/Players/Player2WithTorch.png");
    ret &= loadTexture(TextureName::player2Mooving, "Assets/Players/Player2Mooving.png");
    ret &= loadTexture(TextureName::player2MoovingWithJar, "Assets/Players/Player2MoovingWithJar.png");
    ret &= loadTexture(TextureName::player2MoovingWithTorch, "Assets/Players/Player2MoovingWithTorch.png");

    return ret;
}

bool  AssetsManager::loadTexture(TextureName name, string path)
{
    textures.insert(std::pair<TextureName, sf::Texture>(name, sf::Texture()));
    textures[name].setSmooth(false);
    if (!textures[name].loadFromFile(path))
        return false;
    return true;
}

