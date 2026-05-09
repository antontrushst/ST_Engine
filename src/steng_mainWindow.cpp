#include "steng_mainWindow.hpp"

MainWindow::MainWindow(EngineConfig &engineConfig)
    : engineConfig{&engineConfig}
    , mainWindow(sf::RenderWindow(sf::VideoMode(
         {static_cast<unsigned int>(engineConfig.mainWindow_width)
        , static_cast<unsigned int>(engineConfig.mainWindow_height)})
        , "ST_Engine"))
    , box{{640.f,360.f},{1000.f,600.f},color::Toxic,2}
{}

void MainWindow::init()
{
    sf::View view1{{100.f,100.f},{100.f,100.f}};
    view1.setViewport(sf::FloatRect({0.f,0.f},{.25f,.25f}));

    // MAIN LOOP
    while(this->mainWindow.isOpen())
    {
        this->handleEvents();

        this->mainWindow.clear(this->engineConfig->backColor);
        //---CLEARED SCREEN---////////////////////////////////
        this->mainWindow.setView(this->mainWindow.getDefaultView());
        this->draw();
        this->mainWindow.setView(view1);
        this->draw();
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
    this->mainWindow.draw(this->box);
}
