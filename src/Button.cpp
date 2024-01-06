
#include <math.h>
#include <iostream>
#include "Button.hpp"

//de https://stackoverflow.com/questions/19266018/converting-integer-to-roman-numeral
string int_to_roman(int a)
{
    string ans;
    string M[] = {"","m","mm","mmm"};
    string C[] = {"","c","cc","ccc","cd","d","dc","dcc","dccc","cm"};
    string X[] = {"","x","xx","xxx","xl","l","lx","lxx","lxxx","xc"};
    string I[] = {"","i","ii","iii","iv","v","vi","vii","viii","ix"};
    ans = M[a/1000]+C[(a%1000)/100]+X[(a%100)/10]+I[(a%10)];
    return ans;
}

Button::Button(int x, int y, int correspondingLevel,const AssetsManager & assetsManager) : posX(x), posY(y), levelNum(correspondingLevel), 
background(assetsManager.getTexture(TextureName::buttonBackground)),
border(assetsManager.getTexture(TextureName::buttonBorder)),
text(int_to_roman(levelNum), assetsManager.getFont(), 45)
{ 

    background.setOrigin(sf::Vector2f(BUTTON_SIZE/2, BUTTON_SIZE/2));
    border.setOrigin(sf::Vector2f(BUTTON_SIZE/2, BUTTON_SIZE/2));
    text.setOrigin(sf::Vector2f(text.getLocalBounds().width / 2, 4 * text.getLocalBounds().height / 5));
    background.setPosition(sf::Vector2f(posX, posY));
    border.setPosition(sf::Vector2f(posX, posY));
    text.setPosition(sf::Vector2f(posX, posY));
}


bool Button::updateSelect(int cursorPosX, int cursorPosY)
{
    isSelected = sqrt(pow((cursorPosY - posY), 2) + pow((cursorPosX - posX), 2)) <= BUTTON_SIZE / 2;
    if(isSelected)
    {
        border.setColor(sf::Color(80,255,80));
        border.setScale(1.1,1.1);
        text.setFillColor(sf::Color(80,255,80));
        text.setScale(1.1,1.1);
    } 
    else
    {
        border.setColor(sf::Color(255,255,255));
        border.setScale(1,1);
        text.setFillColor(sf::Color(255,255,255));
        text.setScale(1,1);
    }
        

    return isSelected;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates) const
{

    target.draw(background);
    target.draw(border);
    target.draw(text);
}