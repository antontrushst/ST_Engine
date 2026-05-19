#include "steng_screen_greet.hpp"

Screen_Greet::Screen_Greet(
    const EngineConfig &config,
    const sf::Texture  &image)
    : title{config.titleFont, "LAGNIA", 96}
    , subTitle{config.titleFont, "game engine", 36}
    , LagniaImage{image}
    , buttons{{
        {"newProject",
         {config.mainWindow_width / 2.f, config.mainWindow_height / 2.f},
         {400.f, 50.f}, config.font, "new", 32, config.backColor,
         config.secondaryColor
        }
    }}
{}

Screen_Greet& Screen_Greet::handleEvents(const auto &event)
{
    return *this;
}

Screen_Greet& Screen_Greet::draw(sf::RenderWindow &window)
{
    for(int i{0}; i < this->buttons.buttons.size(); i++)
        window.draw(this->buttons[i]);

    return *this;
}
