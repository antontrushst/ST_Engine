#include "lag_screen_greet.hpp"

Screen_Greet::Screen_Greet(
    const sf::Texture  &image)
    : title{EngineConfig::getInstance().titleFont, "LAGNIA", 96}
    , subTitle{EngineConfig::getInstance().titleFont, "game engine", 36}
    , LagniaImage{image}
    , buttons{{
        {"newProject",
            {EngineConfig::getInstance().mainWindow_width * 0.5f,
                (EngineConfig::getInstance().mainWindow_height * 0.01f) *
                85 - 55},
            {400.f, 50.f},
            EngineConfig::getInstance().font, "new", 32,
            EngineConfig::getInstance().backColor,
            EngineConfig::getInstance().secondaryColor},
        {"openProject",
            {EngineConfig::getInstance().mainWindow_width * 0.5f,
                (EngineConfig::getInstance().mainWindow_height * 0.01f) * 85},
            {400.f, 50.f},
            EngineConfig::getInstance().font, "open", 32,
            EngineConfig::getInstance().backColor,
            EngineConfig::getInstance().secondaryColor}
    }}
{
    // center Lagnia image origin and position it
    sf::Vector2f centeredOrigin{this->LagniaImage.getGlobalBounds().size * .5f +
        this->LagniaImage.getLocalBounds().position};
    this->LagniaImage.setOrigin(centeredOrigin);
    this->LagniaImage.setPosition({
        EngineConfig::getInstance().mainWindow_width * 0.5f,
        EngineConfig::getInstance().mainWindow_height * 0.5f});
    // center title and subTitle texts, color and position it
    centeredOrigin = this->title.getGlobalBounds().size * 0.5f +
        this->title.getLocalBounds().position;
    this->title.setOrigin({std::round(centeredOrigin.x),
        std::round(centeredOrigin.y)});
    this->title.setPosition({
        EngineConfig::getInstance().mainWindow_width * 0.5f,
        (EngineConfig::getInstance().mainWindow_height * 0.01f) * 15});
    this->title.setFillColor(EngineConfig::getInstance().secondaryColor);

    centeredOrigin = this->subTitle.getGlobalBounds().size * 0.5f +
        this->subTitle.getLocalBounds().position;
    this->subTitle.setOrigin({std::round(centeredOrigin.x),
        std::round(centeredOrigin.y)});
    this->subTitle.setPosition({
        EngineConfig::getInstance().mainWindow_width * 0.5f,
        this->title.getPosition().y + this->title.getLocalBounds().size.y +
        5.f});
    this->subTitle.setFillColor(EngineConfig::getInstance().secondaryColor);
}

void Screen_Greet::handleEvents(const std::optional<sf::Event> &event,
    sf::RenderWindow &window)
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

        if(const auto* mouseButtonPressed =
            event->getIf<sf::Event::MouseButtonPressed>())
            if(mouseButtonPressed->button == sf::Mouse::Button::Left)
            {
                if(std::optional<std::string> gameName{st_sfml::inputPopup(
                    window, EngineConfig::getInstance().font,
                    "Enter name of your game:",
                    EngineConfig::getInstance().secondaryColor,
                    EngineConfig::getInstance().backColor,
                    EngineConfig::getInstance().mainColor,
                    EngineConfig::getInstance().altColor)};
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
                }
            }
    }

    if(this->buttons["openProject"].contains({(float)mousePosition.x,
        (float)mousePosition.y}))
    {
        this->buttons["openProject"].setColor(
            EngineConfig::getInstance().mainColor);
        return;
    }
}

void Screen_Greet::draw(sf::RenderWindow &window)
{
    for(int i{0}; i < this->buttons.buttons.size(); i++)
        window.draw(this->buttons[i]);

    window.draw(this->LagniaImage);
    window.draw(this->title);
    window.draw(this->subTitle);
}

void Screen_Greet::updatePositions(const sf::RenderWindow &window)
{
    this->LagniaImage.setPosition({
        (float)window.getSize().x * 0.5f,
        (float)window.getSize().y * 0.5f});
    this->title.setPosition({
        (float)window.getSize().x * 0.5f, (window.getSize().y * 0.01f) * 15});
    this->subTitle.setPosition({
        (float)window.getSize().x * 0.5f, this->title.getPosition().y +
        this->title.getLocalBounds().size.y + 5.f});

    this->buttons["openProject"].setPosition({(float)window.getSize().x * 0.5f,
        ((float)window.getSize().y * 0.01f) * 85});
    this->buttons["newProject"].setPosition({(float)window.getSize().x * 0.5f,
        this->buttons["openProject"].getPosition().y -
        (this->buttons["newProject"].getSize().y + 30)});
}

/*void Screen_Greet::create_lag_file(const std::filesystem::path &path,
    const std::string &name)
{
    std::ofstream lag_file{path.string() + "/" + name + ".lag"};
    lag_file << "GameName=" << name << "\n";
    lag_file << "ProjectLocation=" << path << "\n";
    lag_file.close();
}*/
