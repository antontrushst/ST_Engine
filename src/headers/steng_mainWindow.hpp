#ifndef STENG_MAINWINDOW_HPP
#define STENG_MAINWINDOW_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

#include "st_sfml.hpp"
#include "st_system.hpp"
#include "st_vector.hpp"

class MainWindow
{
    sf::RenderWindow mainWindow;
    
public:
    MainWindow();

    void init();
    void handleEvents();
    void draw();
};

#endif
