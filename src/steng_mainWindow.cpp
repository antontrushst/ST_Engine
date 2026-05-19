#include "steng_mainWindow.hpp"

MainWindow::MainWindow(EngineConfig &engineConfig)
    : engineConfig{&engineConfig}
    , mainWindow(sf::RenderWindow(sf::VideoMode(
        {static_cast<unsigned int>(engineConfig.mainWindow_width)
            , static_cast<unsigned int>(engineConfig.mainWindow_height)})
        , "ST_Engine"))
    , textures{{st::getThisProgramLocation() + "graphics"}}
    , screen_greet{engineConfig, this->textures["test"]}
{
    this->mainWindow.setFramerateLimit(engineConfig.framerate);
}

void MainWindow::init()
{
    // MAIN LOOP
    while(this->mainWindow.isOpen())
    {
        this->handleEvents();

        this->mainWindow.clear(this->engineConfig->backColor);
    //  CLEARED SCREEN  ////////////////////////////////////////////////////////
        this->screen_greet.draw(this->mainWindow);
    //  READY TO DISPLAY  //////////////////////////////////////////////////////
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

