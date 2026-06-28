#include "lag_inGame_display.hpp"
#include <algorithm>

InGameDisplay::InGameDisplay(sf::RenderWindow &window,
    const sf::Vector2f &position, const sf::Vector2f &size,
    const sf::Vector2f &inGamePos,
    const sf::Color &color_back, const sf::Color &color_outline,
    const sf::Color &color_grid)
    : window{window}
    , view{[&]()-> sf::View
        {
            // define which part of the world to get for displaying
            sf::View temp{sf::FloatRect{
                inGamePos - size * 0.5f, size}};
            // define position and size of the display
            temp.setViewport(sf::FloatRect{
                {(position.x / (float)window.getSize().x) *
                    (size.x / (float)window.getSize().x)
                ,(position.y / (float)window.getSize().y) *
                    (size.y / (float)window.getSize().y)},
                {size.x / (float)window.getSize().x
                ,size.y / (float)window.getSize().y}});
            return temp;
        }()}
    , shape{[&]()-> sf::RectangleShape
        {
            sf::RectangleShape temp{size};
            temp.setOrigin(size * 0.5f);
            temp.setPosition(position);
            temp.setFillColor(color_back);
            temp.setOutlineThickness(1.f);
            temp.setOutlineColor(color_outline);
            return temp;
        }()}
    , grid{inGamePos, 20.f, std::max(size.x, size.y), color_grid, 70}
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
bool InGameDisplay::isHoveredOver() const
{
    sf::Vector2i mousePos{sf::Mouse::getPosition(this->window)};
    sf::FloatRect viewport{this->view.getViewport()};
    sf::FloatRect viewPixels{{
        viewport.position.x * this->window.getSize().x,
        viewport.position.y  * this->window.getSize().y},
        {viewport.size.x * this->window.getSize().x,
        viewport.size.y * this->window.getSize().y}};
    return viewPixels.contains({
        static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)});
}
