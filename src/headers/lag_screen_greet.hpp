#ifndef LAG_SCREEN_GREET_HPP
#define LAG_SCREEN_GREET_HPP

#include <SFML/Graphics.hpp>
#include "st_sfml.hpp"
#include "lag_config.hpp"

class Screen_Greet
{
    sf::Text         title;
    sf::Text         subTitle;
    sf::Sprite       LagniaImage;
    st_sfml::Buttons buttons;
public:
    Screen_Greet(const sf::Texture &image);

    void handleEvents(const std::optional<sf::Event> &event,
        sf::RenderWindow &window);
    void draw(sf::RenderWindow &window);
    void updatePositions(const sf::RenderWindow &window);
    void create_lag_file(const std::filesystem::path &path,
        const std::string &name);
};

#endif
