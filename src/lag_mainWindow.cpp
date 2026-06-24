#include "lag_mainWindow.hpp"
#include "lag_screens.hpp"

MainWindow::MainWindow()
    : window(sf::RenderWindow(sf::VideoMode(
        {static_cast<unsigned int>(EngineConfig::getInstance().mainWindow_width)
            , static_cast<unsigned int>(
                EngineConfig::getInstance().mainWindow_height)})
        , "Lagnia"))
    , textures{{st::getThisProgramLocation() /= "graphics"}}
    , screen_greet{this->window, this->textures["lagnia_sketch_transp"]}
    , screen_scene{this->window}
{
    this->window.setFramerateLimit(EngineConfig::getInstance().framerate);
    CurrentScreen = Screens::Scene;
}

void MainWindow::init()
{
    // MAIN LOOP
    while(this->window.isOpen())
    {
        this->handleEvents();

        this->window.clear(EngineConfig::getInstance().color_back);
    //  CLEARED SCREEN  ////////////////////////////////////////////////////////
        switch(CurrentScreen)
        {
            case Screens::Greet:{this->screen_greet.draw(); break;}
            case Screens::Scene:{this->screen_scene.draw(); break;}
        }
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

            this->screen_greet.updatePositions();
        }

        switch(CurrentScreen)
        {
            case Screens::Greet:{this->screen_greet.handleEvents(event); break;}
            case Screens::Scene:{this->screen_scene.handleEvents(event); break;}
        }
    }
}
