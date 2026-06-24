#include "lag_inGame_display.hpp"

InGameDisplay::InGameDisplay(sf::RenderWindow &window,
    const sf::Vector2f &position, const sf::Vector2f &size,
    const sf::Color &color_back, const sf::Color &color_outline,
    const sf::Color &color_grid)
    : window{window}
    , view{[&]()-> sf::View
        {
            // define which part of the world to get for displaying
            sf::View temp{sf::FloatRect{
                {0.f,0.f}, size}};
            // define position and size of the display
            temp.setViewport(sf::FloatRect{
                {position.x / (float)window.getSize().x
                ,position.y / (float)window.getSize().y},
                {size.x / (float)window.getSize().x
                ,size.y / (float)window.getSize().y}});
            return temp;
        }()}
    , shape{[&]()-> sf::RectangleShape
        {
            sf::RectangleShape temp{size};
            temp.setPosition(position);
            temp.setFillColor(color_back);
            temp.setOutlineThickness(1.f);
            temp.setOutlineColor(color_outline);
            return temp;
        }()}
    , grid{{100.f,100.f}, 10.f, 100.f, color_grid, 100}
    , gridVisibility{true}
{}

void InGameDisplay::draw()
{
    this->window.draw(this->shape);
    this->window.setView(this->view);
    if(this->gridVisibility)
        this->window.draw(this->grid);
    this->window.setView(this->window.getDefaultView());
}

void InGameDisplay::setGridVisibility(bool visibility)
{
    if(gridVisibility == visibility)
        return;
    this->gridVisibility = visibility;
}
