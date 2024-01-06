//////////////////
//  Entity.hpp  //
//////////////////

//Contiens la définition des classes Entity et héritantes
//Ces classes représentent les différentes entités qui
//peuplent les niveau, et décrivent leur comportement.
//Liste des entités:
//
// -Porte
// -Plaque de pression
// -Jarre
// -Torche
// -Joueur

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "AssetsManager.hpp"

class Entity : public sf::Drawable
{
    public:
        Entity(sf::Vector2i coordonates,  const sf::Vector2f & topLeftOfGrid) : coordonates(coordonates), topLeftOfGrid(topLeftOfGrid) {setSpritePosition();};
        virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

        const sf::Vector2i & getCoordonates() const {return coordonates;};
        void setCoordonates(sf::Vector2i coord) {coordonates = coord; setSpritePosition();};

        virtual int getDisplayLine() const {return coordonates.y;};
    protected:
        //Permet de changer les coordonnées d'affichage du sprite pour qu'il s'affiche sur la bonne tuile
        virtual void setSpritePosition();
        
        const sf::Vector2f & topLeftOfGrid;
        sf::Vector2i coordonates;
        //Le sprite de l'entité, il est mis à jour au fil du jeu, et affiché à chaque itération de la boucle principale
        sf::Sprite sprite;
};

class PressurePlate : public Entity
{
    public:
        PressurePlate(const AssetsManager & assetsManager, sf::Vector2i coordonates,  const sf::Vector2f & topLeftOfGrid, Entity & linkedDoor, const sf::Color & color, const vector<Entity *> & entities);

        //return true if there is a modification
        bool updatePressed();

        bool getIsPressed() const {return isPressed;};

        Entity & getLinkedDoor() {return linkedDoor;};

    private:
        const vector<Entity *> & entities;
        bool isPressed = false;
        //La porte qu'actionne le bouton
        Entity & linkedDoor;
};


class Door : public Entity
{
    public:
        Door(const AssetsManager & assetsManager, sf::Vector2i coordonates,  const sf::Vector2f & topLeftOfGrid, const sf::Color & color);

        //vérifie si toutes les plaques de pressions associées sont actionnées, dans ce cas, ouvre la porte, sinon, la ferme
        void updateOpen();

        void addPressurePlate(PressurePlate * pressurePlate) {pressurePlates.emplace_back(pressurePlate);};

        bool getOpen() const {return isOpen;};
        
    private:
        bool isOpen = false;
        vector<PressurePlate *> pressurePlates;
};

//Ce sont les entités qui peuvent être attrapées par un joueur (Torche et Jarre)
class CatchableEntity : public Entity
{
    public:
        CatchableEntity(sf::Vector2i coordonates, const sf::Vector2f & topLeftOfGrid) : Entity(coordonates, topLeftOfGrid) {};
        void draw(sf::RenderTarget & target, sf::RenderStates) const;
        void released();
        void grabed(Entity * holder);
        bool getHolden() const {return isHolden;};
    private:
        bool isHolden = false;
        Entity * holder;
};

class Jar : public CatchableEntity
{
    public:
        Jar(const AssetsManager & assetsManager, sf::Vector2i coordonates, const sf::Vector2f & topLeftOfGrid);
};

//Représente les éléments animés
class Animated
{
    public:
        virtual void update() = 0;
    protected:
        uint8_t animationState = 0;
};


class Torch : public Animated, public CatchableEntity
{
    public:
        Torch(const AssetsManager & assetsManager, sf::Vector2i coordonates, const sf::Vector2f & topLeftOfGrid);
        void update();
};

enum Direction{
    down = 0,
    up = 1,
    left = 2,
    right = 3
};

class Player : public Animated, public Entity
{
    public:
        Player(const AssetsManager & assetsManager,sf::Vector2i coordonates,  const sf::Vector2f & topLeftOfGrid, bool isPlayer1, CatchableEntity * holdenEntity, const vector<Entity *> & entities);
        
        void look(Direction dir) {direction = dir;};
        void moove(Direction dir);
        void update();

        bool getIsMooving() const {return isMooving;};

        int getDisplayLine() const;

        Direction getDirection() const {return direction;};

        CatchableEntity * getHoldenEntity() {return holdenEntity;};
        void setHoldenEntity(CatchableEntity * holdEn);
    protected:
        void setSpritePosition();
    private:
        
        //Permet de choisir la bonne texture à afficher, suivant si le joueur se déplace, et s'il tiens un objet
        void setTexture();
        void lookForObjects();

        const AssetsManager & assetsManager;
        const vector<Entity *> & entities;
        bool isPlayer1;
        bool isMooving;
        Direction direction;
        CatchableEntity * holdenEntity;
};





#endif