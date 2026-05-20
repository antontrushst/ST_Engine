#include "steng_mainWindow.hpp"

MainWindow::MainWindow()
    : mainWindow(sf::RenderWindow(sf::VideoMode(
        {static_cast<unsigned int>(EngineConfig::getInstance().mainWindow_width)
            , static_cast<unsigned int>(
                EngineConfig::getInstance().mainWindow_height)})
        , "ST_Engine"))
    , textures{{st::getThisProgramLocation() + "graphics"}}
    , screen_greet{this->textures["lagnia_sketch_transp"]}
{
    this->mainWindow.setFramerateLimit(EngineConfig::getInstance().framerate);
}

void MainWindow::init()
{
    // MAIN LOOP
    while(this->mainWindow.isOpen())
    {
        this->handleEvents();

        this->mainWindow.clear(EngineConfig::getInstance().backColor);
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

        this->screen_greet.handleEvents(event, this->mainWindow);
    }
}
