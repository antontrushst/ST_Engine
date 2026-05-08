#include "steng_mainWindow.hpp"

MainWindow::MainWindow(EngineConfig &engineConfig)
    : engineConfig{&engineConfig}
    , mainWindow(sf::RenderWindow(sf::VideoMode(
         {static_cast<unsigned int>(engineConfig.mainWindow_width)
        , static_cast<unsigned int>(engineConfig.mainWindow_height)})
        , "ST_Engine"))
    , uiBoxes{1}
{
    this->uiBoxes.assign(1, "globalFrame", {640.f, 360.f}, {1000.f, 600.f}
        , color::Toxic);
}

void MainWindow::init()
{
    // MAIN LOOP
    while(this->mainWindow.isOpen())
    {
        this->handleEvents();
        this->draw();
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
    this->mainWindow.clear(this->engineConfig->backColor);
    this->mainWindow.draw(this->uiBoxes);
    this->mainWindow.display();
}
