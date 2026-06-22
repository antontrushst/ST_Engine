#include "lag_mainWindow.hpp"

MainWindow::MainWindow()
    : window(sf::RenderWindow(sf::VideoMode(
        {static_cast<unsigned int>(EngineConfig::getInstance().mainWindow_width)
            , static_cast<unsigned int>(
                EngineConfig::getInstance().mainWindow_height)})
        , "Lagnia"))
    , textures{{st::getThisProgramLocation() /= "graphics"}}
    , screen_greet{this->textures["lagnia_sketch_transp"]}
{
    this->window.setFramerateLimit(EngineConfig::getInstance().framerate);
}

void MainWindow::init()
{
    // MAIN LOOP
    while(this->window.isOpen())
    {
        this->handleEvents();

        this->window.clear(EngineConfig::getInstance().backColor);
    //  CLEARED SCREEN  ////////////////////////////////////////////////////////
        this->screen_greet.draw(this->window);
    //  READY TO DISPLAY  //////////////////////////////////////////////////////
        this->window.display();
    }
}

void MainWindow::handleEvents()
{
    while(const std::optional event = this->window.pollEvent())
    {
        if(event->is<sf::Event::Closed>())
        {
            this->window.close();
        }
        else if(const auto* resized = event->getIf<sf::Event::Resized>())
        {
            sf::FloatRect visibleArea{{0, 0},
                {(float)resized->size.x, (float)resized->size.y}};
            this->window.setView(sf::View(visibleArea));

            this->screen_greet.updatePositions(this->window);
        }

        this->screen_greet.handleEvents(event, this->window);
    }
}
