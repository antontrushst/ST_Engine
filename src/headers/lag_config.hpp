#ifndef LAG_CONFIG_HPP
#define LAG_CONFIG_HPP

#include <fstream>
#include <map>
#include <sstream>
#include <SFML/Graphics.hpp>

#include "st_system.hpp"
#include "lag_colors.hpp"
#include "st_sfml.hpp"

struct EngineConfig
{
    int mainWindow_width = 1280;
    int mainWindow_height = 720;
    int mainWindow_minWidth = 300;
    int mainWindow_minHeight = 300;
    int framerate = 60;
    sf::Font font{st::getThisProgramLocation() /= "fonts/Proletarsk.ttf"};
    sf::Font titleFont{st::getThisProgramLocation() /= "fonts/logo_2K12.ttf"};
    st_sfml::Textures buttonIcons{st::getThisProgramLocation() /= "graphics"};
    sf::Color color_main = color::Toxic;
    sf::Color color_alt = color::Crimson;
    sf::Color color_second = color::SoftBlack;
    sf::Color color_back = color::Milk;
    sf::Color color_outline = color::SoftBlack;
    sf::Color color_inGameBack = sf::Color::Black;
    sf::Color color_grid = sf::Color::White;

    EngineConfig& loadEngineConfig(const std::filesystem::path &path);

    static EngineConfig& getInstance()
    {
        static EngineConfig instance;
        return instance;
    }
    EngineConfig(const EngineConfig&) = delete;
    void operator =(const EngineConfig&) = delete;

private:
    EngineConfig() = default;
};

inline EngineConfig& EngineConfig::loadEngineConfig(
    const std::filesystem::path &path)
{
    enum class keys
    {
        mainWindow_width, mainWindow_height,
        color_main, color_alt, color_second, color_back,
        color_outline, color_inGameBack, color_grid
    };

    std::map<std::string, keys> keysMap =
    {
        {"mainWindow_width", keys::mainWindow_width},
        {"mainWindow_height", keys::mainWindow_height},
        {"color_main", keys::color_main},
        {"color_alt", keys::color_alt},
        {"color_second", keys::color_second},
        {"color_back", keys::color_back},
        {"color_outline", keys::color_outline},
        {"color_inGameBack", keys::color_inGameBack},
        {"color_grid", keys::color_grid}
    };

    if(std::filesystem::exists(path))
    {
        std::ifstream configFile(path);
        std::string line;
        while(std::getline(configFile, line))
        {
            std::string value = line.substr(line.find('=')+1);
            std::string key = line.erase(line.find('='));

            auto it = keysMap.find(key);
            if(it == keysMap.end())
                continue;
            
            switch(it->second)
            {
                case keys::mainWindow_width:{
                    this->mainWindow_width = std::stoi(value); break;}
                case keys::mainWindow_height:{
                    this->mainWindow_height = std::stoi(value); break;}
                case keys::color_main:{
                    std::stringstream ss(value);
                    std::string subValue;
                    for(int i{0}; i < 4; i++)
                    {
                        std::getline(ss, subValue, ',');
                        if(i == 0)
                            this->color_main.r = std::stoi(subValue);
                        else if (i == 1)
                            this->color_main.g = std::stoi(subValue);
                        else if (i == 2)
                            this->color_main.b = std::stoi(subValue);
                        else
                            this->color_main.a = std::stoi(subValue);
                    } break;}
                case keys::color_alt:{
                    std::stringstream ss(value);
                    std::string subValue;
                    for(int i{0}; i < 4; i++)
                    {
                        std::getline(ss, subValue, ',');
                        if(i == 0)
                            this->color_alt.r = std::stoi(subValue);
                        else if (i == 1)
                            this->color_alt.g = std::stoi(subValue);
                        else if (i == 2)
                            this->color_alt.b = std::stoi(subValue);
                        else
                            this->color_alt.a = std::stoi(subValue);
                    } break;}
                case keys::color_second:{
                    std::stringstream ss(value);
                    std::string subValue;
                    for(int i{0}; i < 4; i++)
                    {
                        std::getline(ss, subValue, ',');
                        if(i == 0)
                            this->color_second.r = std::stoi(subValue);
                        else if (i == 1)
                            this->color_second.g = std::stoi(subValue);
                        else if (i == 2)
                            this->color_second.b = std::stoi(subValue);
                        else
                            this->color_second.a = std::stoi(subValue);
                    } break;}
                case keys::color_back:{
                    std::stringstream ss(value);
                    std::string subValue;
                    for(int i{0}; i < 4; i++)
                    {
                        std::getline(ss, subValue, ',');
                        if(i == 0)
                            this->color_back.r = std::stoi(subValue);
                        else if (i == 1)
                            this->color_back.g = std::stoi(subValue);
                        else if (i == 2)
                            this->color_back.b = std::stoi(subValue);
                        else
                            this->color_back.a = std::stoi(subValue);
                    } break;}
                case keys::color_outline:{
                    std::stringstream ss(value);
                    std::string subValue;
                    for(int i{0}; i < 4; i++)
                    {
                        std::getline(ss, subValue, ',');
                        if(i == 0)
                            this->color_outline.r = std::stoi(subValue);
                        else if (i == 1)
                            this->color_outline.g = std::stoi(subValue);
                        else if (i == 2)
                            this->color_outline.b = std::stoi(subValue);
                        else
                            this->color_outline.a = std::stoi(subValue);
                    } break;}
                case keys::color_inGameBack:{
                    std::stringstream ss(value);
                    std::string subValue;
                    for(int i{0}; i < 4; i++)
                    {
                        std::getline(ss, subValue, ',');
                        if(i == 0)
                            this->color_inGameBack.r = std::stoi(subValue);
                        else if (i == 1)
                            this->color_inGameBack.g = std::stoi(subValue);
                        else if (i == 2)
                            this->color_inGameBack.b = std::stoi(subValue);
                        else
                            this->color_inGameBack.a = std::stoi(subValue);
                    } break;}
                case keys::color_grid:{
                    std::stringstream ss(value);
                    std::string subValue;
                    for(int i{0}; i < 4; i++)
                    {
                        std::getline(ss, subValue, ',');
                        if(i == 0)
                            this->color_grid.r = std::stoi(subValue);
                        else if (i == 1)
                            this->color_grid.g = std::stoi(subValue);
                        else if (i == 2)
                            this->color_grid.b = std::stoi(subValue);
                        else
                            this->color_grid.a = std::stoi(subValue);
                    } break;}
                default:{
                    st::msg_warn(
                        std::string{"Config: Parameter " + key
                            + " with value " + value + " not recognized."});
                    break;}
            }
        }
        configFile.close();
        return *this;
    }

    std::ofstream configFile(path);
    if(configFile.is_open())
    {
        configFile << "mainWindow_width="
                   << std::to_string(this->mainWindow_width) << std::endl;
        configFile << "mainWindow_height="
                   << std::to_string(this->mainWindow_height) << std::endl;
        configFile << "color_main="
                   << std::to_string(this->color_main.r) << ","
                   << std::to_string(this->color_main.g) << ","
                   << std::to_string(this->color_main.b) << ","
                   << std::to_string(this->color_main.a) << std::endl;
        configFile << "color_alt="
                   << std::to_string(this->color_alt.r) << ","
                   << std::to_string(this->color_alt.g) << ","
                   << std::to_string(this->color_alt.b) << ","
                   << std::to_string(this->color_alt.a) << std::endl;
        configFile << "color_second="
                   << std::to_string(this->color_second.r) << ","
                   << std::to_string(this->color_second.g) << ","
                   << std::to_string(this->color_second.b) << ","
                   << std::to_string(this->color_second.a) << std::endl;
        configFile << "color_back="
                   << std::to_string(this->color_back.r) << ","
                   << std::to_string(this->color_back.g) << ","
                   << std::to_string(this->color_back.b) << ","
                   << std::to_string(this->color_back.a) << std::endl;
        configFile << "color_outline="
                   << std::to_string(this->color_outline.r) << ","
                   << std::to_string(this->color_outline.g) << ","
                   << std::to_string(this->color_outline.b) << ","
                   << std::to_string(this->color_outline.a) << std::endl;
        configFile << "color_inGameBack="
                   << std::to_string(this->color_inGameBack.r) << ","
                   << std::to_string(this->color_inGameBack.g) << ","
                   << std::to_string(this->color_inGameBack.b) << ","
                   << std::to_string(this->color_inGameBack.a) << std::endl;
        configFile << "color_grid="
                   << std::to_string(this->color_grid.r) << ","
                   << std::to_string(this->color_grid.g) << ","
                   << std::to_string(this->color_grid.b) << ","
                   << std::to_string(this->color_grid.a) << std::endl;
        configFile.close();
    }

    return *this;
}

#endif
