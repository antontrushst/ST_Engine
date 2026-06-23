#ifndef LAG_MAINWINDOW_HPP
#define LAG_MAINWINDOW_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

#include "st_sfml.hpp"
#include "lag_config.hpp"
#include "lag_screen_greet.hpp"
#include "lag_screen_scene.hpp"

class MainWindow
{
    sf::RenderWindow window;
    st_sfml::Textures textures;
    Screen_Greet screen_greet;
    Screen_Scene screen_scene;

    MainWindow();

public:

    MainWindow(const MainWindow&) = delete;
    void operator  =(const MainWindow&) = delete;
    static MainWindow& get()
    {
        static MainWindow instance;
        return instance;
    }

    void init();
    void handleEvents();
};

#endif
