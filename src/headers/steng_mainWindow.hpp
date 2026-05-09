#ifndef STENG_MAINWINDOW_HPP
#define STENG_MAINWINDOW_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

#include "st_sfml.hpp"
#include "steng_config.hpp"

class MainWindow
{
    EngineConfig *engineConfig = nullptr;
    sf::RenderWindow mainWindow;
    st_sfml::RoundedQuad box;
    
public:
    MainWindow(EngineConfig &engineConfig);

    void init();
    void handleEvents();
    void draw();
};

#endif
