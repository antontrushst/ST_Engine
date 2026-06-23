#include "lag_screen_scene.hpp"
#include <iostream>

Screen_Scene::Screen_Scene(sf::RenderWindow &window)
    : window{window}
    , display{{window.getSize().x * 0.5f,
               window.getSize().y * 0.5f}, {300.f, 250.f}}
{
}

void Screen_Scene::draw()
{
    this->window.draw(this->display);
}

void Screen_Scene::handleEvents(const std::optional<sf::Event> &event)
{

}
