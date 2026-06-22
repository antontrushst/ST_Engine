#include "lag_screen_scene.hpp"

Screen_Scene::Screen_Scene(const sf::RenderWindow &window)
    : window{window}
    , display{this->window.size() * 0.5f, {300.f, 250.f}}
{
}

void Screen_Scene::draw()
{
    this->window.draw(this->display);
}
