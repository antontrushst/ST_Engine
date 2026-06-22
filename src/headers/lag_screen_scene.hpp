#ifndef LAG_SCREEN_SCENE_HPP
#define LAG_SCREEN_SCENE_HPP

#include <SFML/Graphics.hpp>
#include "lag_inGame_display.hpp"

class Screen_Scene
{
    sf::RenderWindow &window;
    InGameDisplay display;

public:
    Screen_Scene(sf::RenderWindow &window);

    void draw();
};

#endif
