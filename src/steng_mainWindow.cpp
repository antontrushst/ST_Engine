#include "steng_mainWindow.hpp"

MainWindow::MainWindow(EngineConfig &engineConfig)
    : engineConfig{&engineConfig}
    , mainWindow(sf::RenderWindow(sf::VideoMode(
         {static_cast<unsigned int>(engineConfig.mainWindow_width)
        , static_cast<unsigned int>(engineConfig.mainWindow_height)})
        , "ST_Engine"))
    , UIQuads{1}
    , buttons{std::vector<st_sfml::Button>{{"ping",
        engineConfig.buttonIcons["test"],
        {100.f,100.f}, {100.f,100.f}}}}
{
    this->mainWindow.setFramerateLimit(engineConfig.framerate);
    st_sfml::print(engineConfig.buttonIcons);
}

void MainWindow::init()
{

    UIQuads.assign(1, "test", {100.f,100.f}, {300.f, 100.f}, color::Milk);

    // MAIN LOOP
    while(this->mainWindow.isOpen())
    {
        this->handleEvents();

        this->mainWindow.clear(this->engineConfig->backColor);
        //---CLEARED SCREEN---////////////////////////////////

//        this->mainWindow.setView(this->mainWindow.getDefaultView());
        this->draw();
//        this->mainWindow.setView(view1);
//        this->draw();

        //---READY TO DISPLAY---//////////////////////////////
        this->mainWindow.display();
    }
}

void MainWindow::handleEvents()
{
    while(const std::optional event = this->mainWindow.pollEvent())
    {
        if(event->is<sf::Event::Closed>())
        {
            this->mainWindow.close();
        }
        else if(const auto* resized = event->getIf<sf::Event::Resized>())
        {

        }
    }
}

void MainWindow::draw()
{
    this->mainWindow.draw(this->UIQuads);
    for(int i{0}; i < this->buttons.buttons.size(); i++)
        this->mainWindow.draw(this->buttons[i]);
}
