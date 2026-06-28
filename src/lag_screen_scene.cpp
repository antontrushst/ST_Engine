#include "lag_screen_scene.hpp"
#include "lag_config.hpp"
#include "lag_screens.hpp"

Screen_Scene::Screen_Scene(sf::RenderWindow &window)
    : window{window}
    , display{window, {window.getSize().x * 0.5f,
        window.getSize().y * 0.5f}, {640.f, 360.f}, {0.f, 0.f},
        EngineConfig::getInstance().color_inGameBack,
        EngineConfig::getInstance().color_outline,
        EngineConfig::getInstance().color_grid}
{
}

void Screen_Scene::draw()
{
    if(this->display.isHoveredOver())
        std::cout << "hovered!\n";
    this->display.draw();
}

void Screen_Scene::handleEvents(const std::optional<sf::Event> &event)
{

}
