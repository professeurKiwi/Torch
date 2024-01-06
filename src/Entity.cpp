//////////////////
//  Entity.cpp  //
//////////////////

//Contiens l'implémentations de toutes les méthodes des classes Entity

#include "Entity.hpp"

void Entity::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    target.draw(sprite);
}

void Entity::setSpritePosition()
{
    sprite.setPosition(sf::Vector2f(topLeftOfGrid.x + coordonates.x * 64, 64 + topLeftOfGrid.y + coordonates.y * 64));
}


void CatchableEntity::draw(sf::RenderTarget & target, sf::RenderStates rs) const
{
    if(!isHolden)
        Entity::draw(target, rs);
}

void CatchableEntity::grabed(Entity * hld)
{
    isHolden = true;
    holder = hld;
    coordonates = sf::Vector2i(-1,-1);
}  

void CatchableEntity::released()
{
    if(holder != nullptr)
        coordonates = holder->getCoordonates();
    isHolden = false;
    setSpritePosition();
}

PressurePlate::PressurePlate(const AssetsManager & assetsManager, sf::Vector2i coordonates,  const sf::Vector2f & topLeftOfGrid, Entity & linkedDoor, const sf::Color & color, const vector<Entity *> & entities) : 
Entity(coordonates, topLeftOfGrid), linkedDoor(linkedDoor),
entities(entities)
{
    sprite.setTexture(assetsManager.getTexture(TextureName::pressurePlate));
    sprite.setTextureRect(sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(16,16)));
    sprite.setScale(4, 4);
    sprite.setColor(color);
    sprite.setOrigin(0, 16);
}

bool PressurePlate::updatePressed() 
{
    bool res = false, ret = false;
    //Si une autre entité est sur la même tuile, la plaque est pressée
    for(const Entity * en : entities)
    {
        if(en->getCoordonates() == coordonates && typeid(*en) != typeid(PressurePlate))
        {
            res = true;
            break;
        }
    }
    //si l'état la changé, on retourne vrai
    if(res != isPressed)
    {
        ret = true;
        if(res)
            sprite.setTextureRect(sf::IntRect(sf::Vector2i(16,0), sf::Vector2i(16,16)));
        else
            sprite.setTextureRect(sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(16,16)));
    }
        
    isPressed = res;
    return ret;
    
}

Door::Door(const AssetsManager & assetsManager, sf::Vector2i coordonates,  const sf::Vector2f & topLeftOfGrid, const sf::Color & color) : Entity(coordonates, topLeftOfGrid)
{
    pressurePlates = vector<PressurePlate *>();
    sprite.setTexture(assetsManager.getTexture(TextureName::door));
    sprite.setScale(4, 4);
    sprite.setColor(color);
    sprite.setOrigin(0, 22);
}

void Door::updateOpen()
{
    //Si toutes les plaques de pressions associées sont actionnées, la porte est ouverte
    //Sinon, elle est fermée
    for(PressurePlate * pr : pressurePlates)
    {
        if(!pr->getIsPressed())
        {
            isOpen = false;
            sprite.setTextureRect(sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(16,22)));
            return;
        }

    }
    isOpen = true;
    sprite.setTextureRect(sf::IntRect(sf::Vector2i(16,0), sf::Vector2i(16,22)));
    return;
}

Jar::Jar(const AssetsManager & assetsManager, sf::Vector2i coordonates,  const sf::Vector2f & topLeftOfGrid) : CatchableEntity(coordonates, topLeftOfGrid)
{
    sprite.setTexture(assetsManager.getTexture(TextureName::jar));
    sprite.setScale(4, 4);
    sprite.setOrigin(0, 16);
}

Torch::Torch(const AssetsManager & assetsManager, sf::Vector2i coordonates, const sf::Vector2f & topLeftOfGrid) : CatchableEntity(coordonates, topLeftOfGrid)
{
    sprite.setTexture(assetsManager.getTexture(TextureName::torch));
    sprite.setScale(4, 4);
    sprite.setOrigin(0,16);
    sprite.setTextureRect(sf::IntRect(sf::Vector2i(0 , 0), sf::Vector2i(16,16)));
}

void Torch::update()
{   
    animationState++;
    animationState %= 8;
    sprite.setTextureRect(sf::IntRect(sf::Vector2i(16 * animationState, 0), sf::Vector2i(16,16)));
}

Player::Player(const AssetsManager & assetsManager, sf::Vector2i coordonates,  const sf::Vector2f & topLeftOfGrid, bool isPlayer1, CatchableEntity * holdenEntity , const vector<Entity *> & entities) : 
Entity(coordonates, topLeftOfGrid), 
holdenEntity(holdenEntity), isPlayer1(isPlayer1),
assetsManager(assetsManager),
entities(entities),
isMooving(false),
direction(Direction::down)
{
    if(holdenEntity != nullptr)
        holdenEntity->grabed(this);

    setTexture();
    //pour décaler les animations des 2 joueurs
    if(isPlayer1)
        animationState = 4;
    sprite.setScale(4, 4);
    sprite.setOrigin(0, 32);
    sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(16, 32)));
    
}

void Player::setTexture()
{
    if(isPlayer1)
    {
        if(isMooving)
        {
            if(holdenEntity == nullptr)
                sprite.setTexture(assetsManager.getTexture(TextureName::player1Mooving));
            else if(typeid(*holdenEntity) == typeid(Torch))
                sprite.setTexture(assetsManager.getTexture(TextureName::player1MoovingWithTorch));
            else if(typeid(*holdenEntity) == typeid(Jar))
                sprite.setTexture(assetsManager.getTexture(TextureName::player1MoovingWithJar));
        }
        else
        {
            if(holdenEntity == nullptr)
                sprite.setTexture(assetsManager.getTexture(TextureName::player1));
            else if(typeid(*holdenEntity) == typeid(Torch))
                sprite.setTexture(assetsManager.getTexture(TextureName::player1WithTorch));
            else if(typeid(*holdenEntity) == typeid(Jar))
                sprite.setTexture(assetsManager.getTexture(TextureName::player1WithJar));
        }
    }
    else
    {
        if(isMooving)
        {
            if(holdenEntity == nullptr)
                sprite.setTexture(assetsManager.getTexture(TextureName::player2Mooving));
            else if(typeid(*holdenEntity) == typeid(Torch))
                sprite.setTexture(assetsManager.getTexture(TextureName::player2MoovingWithTorch));
            else if(typeid(*holdenEntity) == typeid(Jar))
                sprite.setTexture(assetsManager.getTexture(TextureName::player2MoovingWithJar));
        }
        else
        {
            if(holdenEntity == nullptr)
                sprite.setTexture(assetsManager.getTexture(TextureName::player2));
            else if(typeid(*holdenEntity) == typeid(Torch))
                sprite.setTexture(assetsManager.getTexture(TextureName::player2WithTorch));
            else if(typeid(*holdenEntity) == typeid(Jar))
                sprite.setTexture(assetsManager.getTexture(TextureName::player2WithJar));
        }
    }
}

void Player::update()
{
    //On passe à la frame suivante de l'animation
    animationState++;
    if(animationState >= 8)
    {
        animationState = 0;
        //Si on arrive à la fin d'un cycle d'animation et que l'on était en déplacement, on arrive sur la nouvelle tuile
        if(isMooving)
        {
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
            isMooving = false;
            setTexture();
            setSpritePosition();
            if(holdenEntity == nullptr)
                lookForObjects();
        }
    }
    //Le sprite affiche un rectangle de la texture qui correspond à la bonne frame d'animation
    sprite.setTextureRect(sf::IntRect(sf::Vector2(animationState * 16, direction * 32), sf::Vector2(16,32)));
    if(isMooving)
        setSpritePosition();
}

void Player::setSpritePosition()
{
    //On ajoute à la position normale du sprite l'avancement du mouvement du personnage
    sf::Vector2f additionVector(0,0);
    if(isMooving)
    {
        int advancment = (animationState + 1) * 8; 
        switch (direction)
        {
            
            case Direction::up:
                additionVector = sf::Vector2f(0,-advancment);
                break;
            case Direction::down:
                additionVector = sf::Vector2f(0, advancment);
                break;
            case Direction::left:
                additionVector = sf::Vector2f(-advancment, 0);
                break;
            case Direction::right:
                additionVector = sf::Vector2f(advancment, 0);
            
        }
    }
    sprite.setPosition(sf::Vector2f(topLeftOfGrid.x + coordonates.x * 64, 64 + topLeftOfGrid.y + coordonates.y * 64) + additionVector);
}

void Player::moove(Direction dir)
{
    animationState = 0;
    isMooving = true;
    direction = dir;
    setTexture();
}

void Player::lookForObjects()
{   
    //Si un objet attrapable se trouve sur la même case que le joueur, il l'attrape
    for(Entity * entity : entities)
    {
        CatchableEntity* catchableEntity = dynamic_cast<CatchableEntity *>(entity);
        if(entity->getCoordonates() == coordonates && catchableEntity != nullptr)
        {
            catchableEntity->grabed(this);
            holdenEntity = catchableEntity;
            setTexture();
            return;
        }
    }
}

int Player::getDisplayLine() const
{
    if(isMooving && direction == Direction::down)
    {
        return coordonates.y + 1;
    }
    return coordonates.y;
}

void Player::setHoldenEntity(CatchableEntity * holdEn)
{
    if(holdEn != nullptr)
    {
        holdEn->grabed(this);
    }
    holdenEntity = holdEn;
    setTexture();
}

