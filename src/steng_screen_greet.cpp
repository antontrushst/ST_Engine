#include "steng_screen_greet.hpp"

Screen_Greet::Screen_Greet(
    const sf::Texture  &image)
    : title{EngineConfig::getInstance().titleFont, "LAGNIA", 96}
    , subTitle{EngineConfig::getInstance().titleFont, "game engine", 36}
    , LagniaImage{image}
    , buttons{{
        {"newProject",
            {EngineConfig::getInstance().mainWindow_width / 2.f, 575.f},
            {400.f, 50.f},
            EngineConfig::getInstance().font, "new", 32,
            EngineConfig::getInstance().backColor,
            EngineConfig::getInstance().secondaryColor},
        {"openProject",
            {EngineConfig::getInstance().mainWindow_width / 2.f, 635.f},
            {400.f, 50.f},
            EngineConfig::getInstance().font, "open", 32,
            EngineConfig::getInstance().backColor,
            EngineConfig::getInstance().secondaryColor}
    }}
{
    // center Lagnia image origin and position it
    sf::Vector2f centeredOrigin{this->LagniaImage.getGlobalBounds().size / 2.f +
        this->LagniaImage.getLocalBounds().position};
    this->LagniaImage.setOrigin(centeredOrigin);
    this->LagniaImage.setPosition({
        EngineConfig::getInstance().mainWindow_width / 2.f,
        EngineConfig::getInstance().mainWindow_height / 2.f});
    // center title and subTitle texts, color and position it
    centeredOrigin = this->title.getGlobalBounds().size / 2.f +
        this->title.getLocalBounds().position;
    this->title.setOrigin({std::round(centeredOrigin.x),
        std::round(centeredOrigin.y)});
    this->title.setPosition({
        EngineConfig::getInstance().mainWindow_width / 2.f, 120.f});
    this->title.setFillColor(EngineConfig::getInstance().secondaryColor);

    centeredOrigin = this->subTitle.getGlobalBounds().size / 2.f +
        this->subTitle.getLocalBounds().position;
    this->subTitle.setOrigin({std::round(centeredOrigin.x),
        std::round(centeredOrigin.y)});
    this->subTitle.setPosition({
        EngineConfig::getInstance().mainWindow_width / 2.f, 170.f});
    this->subTitle.setFillColor(EngineConfig::getInstance().secondaryColor);
}

Screen_Greet& Screen_Greet::handleEvents(const std::optional<sf::Event> &event,
    const sf::RenderWindow &window)
{
    this->buttons["newProject"].
        setColor(EngineConfig::getInstance().secondaryColor);
    this->buttons["openProject"].
        setColor(EngineConfig::getInstance().secondaryColor);
    sf::Vector2i mousePosition{sf::Mouse::getPosition(window)};

    if(this->buttons["newProject"].contains({(float)mousePosition.x,
        (float)mousePosition.y}))
    {
        this->buttons["newProject"].setColor(
            EngineConfig::getInstance().mainColor);
        return *this;
    }

    if(this->buttons["openProject"].contains({(float)mousePosition.x,
        (float)mousePosition.y}))
    {
        this->buttons["openProject"].setColor(
            EngineConfig::getInstance().mainColor);
        return *this;
    }

    return *this;
}

Screen_Greet& Screen_Greet::draw(sf::RenderWindow &window)
{
    for(int i{0}; i < this->buttons.buttons.size(); i++)
        window.draw(this->buttons[i]);

    window.draw(this->LagniaImage);
    window.draw(this->title);
    window.draw(this->subTitle);
    return *this;
}
