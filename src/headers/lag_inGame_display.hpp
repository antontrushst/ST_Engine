#ifndef LAG_INGAME_DISPLAY_HPP
#define LAG_INGAME_DISPLAY_HPP

#include <SFML/Graphics.hpp>
#include "lag_grid.hpp"

class InGameDisplay
{
    sf::RenderWindow &window;
    sf::View view;
    sf::RectangleShape shape;
    Grid grid;
    bool gridVisibility;
    
public:
    InGameDisplay(sf::RenderWindow &window, const sf::Vector2f &position,
        const sf::Vector2f &size, const sf::Vector2f &inGamePos,
        const sf::Color &color_back,
        const sf::Color &color_outline, const sf::Color &color_grid);

    void draw();
    Grid& getGrid() {return this->grid;}
    bool isGridVisible() const {return this->gridVisibility;}
    void setGridVisibility(bool visibility);
};

#endif
