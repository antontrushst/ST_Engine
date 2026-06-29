#ifndef LAG_SCREEN_GREET_HPP
#define LAG_SCREEN_GREET_HPP

#include <SFML/Graphics.hpp>
#include "st_sfml.hpp"
#include "imgui.h"
namespace lag {

class Screen_Greet
{
    sf::RenderWindow &window;
    sf::Font         titleFont;
    sf::Font         subTitleFont;
    sf::Text         title;
    sf::Text         subTitle;
    sf::Sprite       LagniaImage;
    st_sfml::Buttons buttons;
public:
    Screen_Greet(sf::RenderWindow &window, const sf::Texture &image);

    void handleEvents(const std::optional<sf::Event> &event);
    void update(ImFont *font);
    void draw();
    void updatePositions();
};

}
#endif
