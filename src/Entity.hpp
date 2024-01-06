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
        virtual void setSpritePosition();
        
        const sf::Vector2f & topLeftOfGrid;
        sf::Vector2i coordonates;
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
        Entity & linkedDoor;
};


class Door : public Entity
{
    public:
        Door(const AssetsManager & assetsManager, sf::Vector2i coordonates,  const sf::Vector2f & topLeftOfGrid, const sf::Color & color);

        void updateOpen();

        bool getOpen() const {return isOpen;};
        void addPressurePlate(PressurePlate * pressurePlate) {pressurePlates.emplace_back(pressurePlate);};

    private:
        bool isOpen = false;
        vector<PressurePlate *> pressurePlates;
};


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
        const AssetsManager & assetsManager;
        const vector<Entity *> & entities;
        void setTexture();
        void lookForObjects();
        bool isPlayer1;
        bool isMooving;
        Direction direction;
        CatchableEntity * holdenEntity;
};





#endif