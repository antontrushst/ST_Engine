#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "steng_outliner.h"
#include "steng_grid.h"

namespace steng
{
    class Engine
    {
        std::string* m_confFile_path = nullptr;
        std::string* m_gameName = new std::string("default");

        // WINDOW
        sf::RenderWindow* m_mainWin = nullptr;
        int* m_mainWin_width = new int(300);
        int* m_mainWin_height = new int(300);
        std::string* m_mainWin_name = new std::string("default");
        int* m_mainWin_frameLimit = new int(60);

        // VIEW
        sf::View* m_mainView = nullptr;

        // SYSTEMS
        Outliner& outliner = Outliner::instance();
        Grid& grid = Grid::instance();

        // TOGGLES
        bool m_game_ON = false;
        bool m_grid_ON = false;
        bool m_outliner_open = false;

        // UI COLORS
        sf::Color* mainWin_background_color = new sf::Color(128,128,128);

        Engine() = default;
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        void processConfFile_line(std::string line);
        void update_MainWinProp();
        void pan_View();
        void registerInput_editor(sf::Event& event);
        void registerInput_game(sf::Event& event);
    
    public:
        static Engine& instance()
        {
            static Engine instance;
            return instance;
        }
        
        void config(std::string = "config.txt");
        void init();


    };
};