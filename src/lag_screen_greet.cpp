#include "lag_screen_greet.hpp"
#include "lag_config.hpp"
#include "lag_file.hpp"
#include "lag_screens.hpp"

lag::Screen_Greet::Screen_Greet(
        sf::RenderWindow &window, const sf::Texture &image)
    : window{window}
    , title{EngineConfig::getInstance().titleFont, "LAGNIA", 96}
    , subTitle{EngineConfig::getInstance().titleFont, "game engine", 36}
    , LagniaImage{image}
    , buttons{{
        {"newProject",
            {this->window.getSize().x * 0.5f,
            (this->window.getSize().y * 0.01f) * 85 - 55},
            {400.f, 50.f},
            EngineConfig::getInstance().font, "new", 32,
            EngineConfig::getInstance().color_back,
            EngineConfig::getInstance().color_second},
        {"openProject",
            {this->window.getSize().x * 0.5f,
            (this->window.getSize().y * 0.01f) * 85},
            {400.f, 50.f},
            EngineConfig::getInstance().font, "open", 32,
            EngineConfig::getInstance().color_back,
            EngineConfig::getInstance().color_second}
    }}
{
    // center Lagnia image origin and position it
    sf::Vector2f centeredOrigin{this->LagniaImage.getGlobalBounds().size * .5f +
        this->LagniaImage.getLocalBounds().position};
    this->LagniaImage.setOrigin(centeredOrigin);
    this->LagniaImage.setPosition({
        this->window.getSize().x * 0.5f, this->window.getSize().y * 0.5f});
    // center title and subTitle texts, color and position it
    centeredOrigin = this->title.getGlobalBounds().size * 0.5f +
        this->title.getLocalBounds().position;
    this->title.setOrigin({std::round(centeredOrigin.x),
        std::round(centeredOrigin.y)});
    this->title.setPosition({
        this->window.getSize().x * 0.5f,
        (this->window.getSize().y * 0.01f) * 15});
    this->title.setFillColor(EngineConfig::getInstance().color_second);

    centeredOrigin = this->subTitle.getGlobalBounds().size * 0.5f +
        this->subTitle.getLocalBounds().position;
    this->subTitle.setOrigin({std::round(centeredOrigin.x),
        std::round(centeredOrigin.y)});
    this->subTitle.setPosition({
        this->window.getSize().x * 0.5f,
        this->title.getPosition().y + this->title.getLocalBounds().size.y +
        5.f});
    this->subTitle.setFillColor(EngineConfig::getInstance().color_second);
}

void lag::Screen_Greet::update(const std::optional<sf::Event> &event)
{
    this->buttons["newProject"].
        setColor(EngineConfig::getInstance().color_second);
    this->buttons["openProject"].
        setColor(EngineConfig::getInstance().color_second);
    sf::Vector2i mousePosition{sf::Mouse::getPosition(this->window)};

    if(this->buttons["newProject"].contains({(float)mousePosition.x,
        (float)mousePosition.y}))
    {
        this->buttons["newProject"].setColor(
            EngineConfig::getInstance().color_main);

        if(const auto *mouseButtonPressed =
            event->getIf<sf::Event::MouseButtonPressed>())
            if(mouseButtonPressed->button == sf::Mouse::Button::Left)
            {
                if(std::optional<std::string> gameName{st_sfml::inputPopup(
                    this->window, EngineConfig::getInstance().font,
                    "Enter name of your game:",
                    EngineConfig::getInstance().color_second,
                    EngineConfig::getInstance().color_back,
                    EngineConfig::getInstance().color_main,
                    EngineConfig::getInstance().color_alt)};
                    gameName.has_value())
                {
                    std::filesystem::path newProject_path{
                        st::getFolder("Select a folder for your game project "
                        "directory to be created in").value_or("")};
                    if(newProject_path.empty())
                        return;

                    std::filesystem::create_directory(
                        newProject_path /= gameName.value());
                    LagFile::get().create(newProject_path, gameName.value());
                    CurrentScreen = Screens::Editor;
                }
            }
    }

    if(this->buttons["openProject"].contains({(float)mousePosition.x,
        (float)mousePosition.y}))
    {
        this->buttons["openProject"].setColor(
            EngineConfig::getInstance().color_main);

        if(const auto *mouseButtonPressed =
            event->getIf<sf::Event::MouseButtonPressed>())
            if(mouseButtonPressed->button == sf::Mouse::Button::Left)
            {
                std::filesystem::path lagFile_path{
                    st::getFile("Select your game project .lag file",
                    {"*.lag"}, {"Lagnia files (*.lag)"}).value_or("")};
                if(lagFile_path.empty())
                    return;

                LagFile::get().load(lagFile_path);
                CurrentScreen = Screens::Editor;
            }
        return;
    }
}

void lag::Screen_Greet::draw()
{
    for(int i{0}; i < this->buttons.buttons.size(); i++)
        this->window.draw(this->buttons[i]);

    this->window.draw(this->LagniaImage);
    this->window.draw(this->title);
    this->window.draw(this->subTitle);
}

void lag::Screen_Greet::updatePositions()
{
    this->LagniaImage.setPosition({
        (float)this->window.getSize().x * 0.5f,
        (float)this->window.getSize().y * 0.5f});
    this->title.setPosition({
        (float)this->window.getSize().x * 0.5f,
        (this->window.getSize().y * 0.01f) * 15});
    this->subTitle.setPosition({
        (float)this->window.getSize().x * 0.5f,
        this->title.getPosition().y +
            this->title.getLocalBounds().size.y + 5.f});

    this->buttons["openProject"].setPosition({
        (float)this->window.getSize().x * 0.5f,
        ((float)this->window.getSize().y * 0.01f) * 85});
    this->buttons["newProject"].setPosition({
        (float)this->window.getSize().x * 0.5f,
        this->buttons["openProject"].getPosition().y -
            (this->buttons["newProject"].getSize().y + 30)});
}
