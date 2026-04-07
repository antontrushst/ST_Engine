#include "st_engine.h"
#include <fstream>
#include <map>
#include <thread>

using namespace steng;

void Engine::init()
{
    if(m_mainWin == nullptr)
        m_mainWin = new sf::RenderWindow
        (
            sf::VideoMode(*m_mainWin_width, *m_mainWin_height),
            *m_mainWin_name,
            sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize
        );
    m_mainWin->setFramerateLimit(*m_mainWin_frameLimit);
    
    if(m_mainView == nullptr)
        m_mainView = new sf::View(sf::FloatRect(0, 0, *m_mainWin_width,
            *m_mainWin_height));

    m_mainWin->setView(*m_mainView);

    // MAIN LOOP ///////////////////////////////////////////////////////////////
    while(m_mainWin->isOpen())
    {
        
        update_MainWinProp();

        sf::Event event;
        while(m_mainWin->pollEvent(event))
        {
            if(event.type == sf::Event::Resized)
                m_mainWin->setView(sf::View(sf::FloatRect(0, 0,
                    event.size.width,
                    event.size.height)));
            
            registerInput_editor(event);
            registerInput_game(event);
        }

        outliner.draw(m_mainWin);
        grid.draw(m_mainWin);
    }
    ////////////////////////////////////////////////////////////////////////////
}

void Engine::config(std::string config_file_path)
{
    m_confFile_path = new std::string(config_file_path);
    std::ifstream confFile(*m_confFile_path);
    if(confFile.is_open())
        std::cout << "\033[32m" << "\tConfig file found." << "\033[0m"
                  << std::endl;
    else
    {
        std::cerr << "\033[31m" << "\tERROR: Couldn't open config file!\n"
                  << "\033[33m" << "\tPlease, check if provided path is correct "
                  << "(path\\\\to\\\\config.txt)" << "\033[0m" << std::endl;
        exit(1);
    }

    std::string confFile_line;
    while(std::getline(confFile, confFile_line))
    {
        processConfFile_line(confFile_line);
    }
}

void Engine::processConfFile_line(std::string line)
{
    std::string key = "";
    std::string value = "";
    bool equalSign_encountered = false;
    for(char c : line)
    {
        if(c == ' ')
            continue;
        if(c == '#')
            break;
        if(c == '=')
        {
            equalSign_encountered = true;
            continue;
        }

        if(!equalSign_encountered)
            key += c;
        else
            value += c;
    }

    if(key.empty() || value.empty())
        return;

    enum KeyWords
    {
        Game_Name,
        MainWindow_Width,
        MainWindow_Height,
        MainWindow_Name,
        Framerate_Limit
    };

    std::map<std::string, KeyWords> map;
    map["MainWindow_Width"] = KeyWords::MainWindow_Width;
    map["MainWindow_Height"] = KeyWords::MainWindow_Height;
    map["MainWindow_Name"] = KeyWords::MainWindow_Name;
    map["Game_Name"] = KeyWords::Game_Name;
    map["Framerate_Limit"] = KeyWords::Framerate_Limit;
    std::map<std::string, KeyWords>::iterator it;
    it = map.find(key);
    switch(it->second)
    {
        case KeyWords::Game_Name:
            *m_gameName = value;
            break;
        case KeyWords::MainWindow_Width:
            *m_mainWin_width = std::stoi(value);
            break;
        case KeyWords::MainWindow_Height:
            *m_mainWin_height = std::stoi(value);
            break;
        case KeyWords::MainWindow_Name:
            *m_mainWin_name = value;
            break;
        case KeyWords::Framerate_Limit:
            *m_mainWin_frameLimit = std::stoi(value);
            break;
        default:
            std::cout << "\033[33m" << "\tWarning: config file keyword '"
                      << key << "' was not recognized!" << "\033[0m"
                      << std::endl;
            break;
    }
}

void Engine::update_MainWinProp()
{
    *m_mainWin_width = m_mainWin->getSize().x;
    *m_mainWin_height = m_mainWin->getSize().y;
}

void Engine::registerInput_editor(sf::Event& event)
{
    if(m_game_ON)
        return;

    switch(event.type)
    {
        case sf::Event::Closed:
            m_mainWin->close(); break;
        case sf::Event::MouseButtonPressed:
            if(event.mouseButton.button == sf::Mouse::Left)
                outliner.toggle(); break;
        case sf::Event::KeyPressed:
            switch(event.key.code)
            {
                case sf::Keyboard::G: grid.toggle(); break;
                default: break;
            }
        default: break;
    }
}

void Engine::registerInput_game(sf::Event &event)
{
    if(!m_game_ON)
        return;

    switch(event.type)
    {
        case sf::Event::Closed:
            m_mainWin->close();
            break;
        default:
            break;
    }
}

void Engine::pan_View()
{
    
}