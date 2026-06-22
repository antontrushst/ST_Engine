#ifndef LAG_INGAME_DISPLAY_HPP
#define LAG_INGAME_DISPLAY_HPP

#include <SFML/Graphics.hpp>

class InGameDisplay : public sf::Drawable
{
    sf::RectangleShape shape;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    
public:
    InGameDisplay(const sf::Vector2f &position, const sf::Vector2f &size);
};

#endif
