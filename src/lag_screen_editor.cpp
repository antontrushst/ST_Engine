#include "lag_screen_editor.hpp"
#include "lag_config.hpp"
#include "lag_screens.hpp"

lag::Screen_Editor::Screen_Editor(sf::RenderWindow &window)
    : window{window}
    , display{window, {window.getSize().x * 0.5f,
        window.getSize().y * 0.5f}, {640.f, 360.f}, {0.f, 0.f},
        EngineConfig::getInstance().color_inGameBack,
        EngineConfig::getInstance().color_outline,
        EngineConfig::getInstance().color_grid}
{
}

void lag::Screen_Editor::handleEvents(const std::optional<sf::Event> &event)
{

}

void lag::Screen_Editor::update()
{

}

void lag::Screen_Editor::draw()
{
    if(this->display.isHoveredOver())
        std::cout << "hovered!\n";
    this->display.draw();
}
