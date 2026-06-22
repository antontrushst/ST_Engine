#include "lag_inGame_display.hpp"

InGameDisplay::InGameDisplay(
    const sf::Vector2f &position, const sf::Vector2f &size)
    : shape{[&]()-> sf::RectangleShape
        {
            sf::RectangleShape temp{size};
            temp.setOrigin(size * 0.5f);
            temp.setPosition(position);
            temp.setFillColor(sf::Color::Green);
            return temp;
        }()}
{}

void InGameDisplay::draw(sf::RenderTarget &target,
    sf::RenderStates states) const
{
    target.draw(this->shape, states);
}
