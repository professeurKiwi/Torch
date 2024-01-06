#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include "AssetsManager.hpp"

//Le nombre du pixel du diamètre du bouton
#define BUTTON_SIZE 100

class Button : public sf::Drawable
{
    public:
        Button(int x, int y, int correspondingLevel,const AssetsManager & assetsManager);

        //test si le bouton est sélectionné par la sourie, si oui, retourne vrai. Met à jour le champ isSelected utile à l'affichage.
        bool updateSelect(int cursorPosX, int cursorPosY);

        int getCorrespondingLevel() const {return levelNum;};

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
    private:
        //Les coordonnées du centre du boutton
        int posX, posY;
        //Le numéro du niveau désigné par le bouton
        int levelNum;
        bool isSelected = false;

        sf::Sprite
            background,
            border;

        sf::Text text;
        

};

#endif