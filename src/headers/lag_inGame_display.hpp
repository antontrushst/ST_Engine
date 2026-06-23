#ifndef LAG_INGAME_DISPLAY_HPP
#define LAG_INGAME_DISPLAY_HPP

#include <SFML/Graphics.hpp>
#include "lag_grid.hpp"

class InGameDisplay : public sf::Drawable
{
    sf::RectangleShape shape;
    Grid grid;
    bool gridVisibility;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    
public:
    InGameDisplay(const sf::Vector2f &position, const sf::Vector2f &size);

    Grid& getGrid() {return this->grid;}
    bool isGridVisible() const {return this->gridVisibility;}
    void setGridVisibility(bool visibility);
};

#endif
