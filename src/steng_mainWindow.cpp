#include "steng_mainWindow.hpp"

MainWindow::MainWindow(EngineConfig &engineConfig)
    : engineConfig{&engineConfig}
    , mainWindow(sf::RenderWindow(sf::VideoMode(
         {static_cast<unsigned int>(engineConfig.mainWindow_width)
        , static_cast<unsigned int>(engineConfig.mainWindow_height)})
        , "ST_Engine"))
    // initialize buttons here--------------------------------------------------
    , buttons{std::vector<st_sfml::Button>{{"ping",
        {engineConfig.mainWindow_width/2.f, engineConfig.mainWindow_height/2.f},
        {400.f,50.f},
        engineConfig.font, "new", 32, engineConfig.backColor,
        engineConfig.secondaryColor}}}
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
    for(int i{0}; i < this->buttons.buttons.size(); i++)
        this->mainWindow.draw(this->buttons[i]);
}
