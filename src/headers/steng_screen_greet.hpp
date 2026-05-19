#ifndef STENG_SCREEN_GREET_HPP
#define STENG_SCREEN_GREET_HPP

#include <SFML/Graphics.hpp>
#include "st_sfml.hpp"
#include "steng_config.hpp"

class Screen_Greet
{
    sf::Text         title;
    sf::Text         subTitle;
    sf::Sprite       LagniaImage;
    st_sfml::Buttons buttons;
public:
    Screen_Greet(const EngineConfig &config, const sf::Texture &image);

    Screen_Greet& handleEvents(const auto &event);
    Screen_Greet& draw(sf::RenderWindow &window);
};

#endif
