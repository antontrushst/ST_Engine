#ifndef LAG_SCREEN_EDITOR_HPP
#define LAG_SCREEN_EDITOR_HPP

#include <SFML/Graphics.hpp>
#include "lag_inGame_display.hpp"
namespace lag {

class Screen_Editor
{
    sf::RenderWindow &window;
    InGameDisplay display;

public:
    Screen_Editor(sf::RenderWindow &window);

    void handleEvents(const std::optional<sf::Event> &event);
    void update();
    void draw();
};

}
#endif
