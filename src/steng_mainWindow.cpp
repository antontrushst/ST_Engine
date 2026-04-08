#include "steng_mainWindow.hpp"

MainWindow::MainWindow()
    : mainWindow(sf::RenderWindow(sf::VideoMode({300,400}), "main"))
{}

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
    this->mainWindow.clear(sf::Color::Green);
    this->mainWindow.display();
}
