#ifndef STENG_CONFIG_HPP
#define STENG_CONFIG_HPP

#include <fstream>
#include <map>
#include <sstream>
#include <SFML/Graphics.hpp>

#include "st_system.hpp"
#include "steng_colors.hpp"
#include "st_sfml.hpp"

struct EngineConfig
{
    int mainWindow_width = 1280;
    int mainWindow_height = 720;
    int mainWindow_minWidth = 300;
    int mainWindow_minHeight = 300;
    sf::Font font{std::string{
        st::getThisProgramLocation() + "fonts\\Power_Pixel-7.ttf"}};
    st_sfml::Textures buttonIcons{std::string{
        st::getThisProgramLocation() + "graphics"}};
    sf::Color mainColor = color::Toxic;
    sf::Color secondaryColor = color::LightGrey;
    sf::Color backColor = color::SoftBlack;
};

inline EngineConfig loadEngineConfig(const std::string &path)
{
    EngineConfig config;

    enum class keys
    {
        mainWindow_width, mainWindow_height,
        mainColor, secondaryColor, backColor
    };

    std::map<std::string, keys> keysMap =
    {
        {"mainWindow_width", keys::mainWindow_width},
        {"mainWindow_height", keys::mainWindow_height},
        {"mainColor", keys::mainColor},
        {"secondaryColor", keys::secondaryColor},
        {"backColor", keys::backColor}
    };

    if(std::filesystem::exists(path))
    {
        std::ifstream configFile(path);
        std::string line;
        while(std::getline(configFile, line))
        {
            std::string value = line.substr(line.find('=')+1);
            std::string key = line.erase(line.find('='));
            //std::cout << key << "\t" << value << std::endl;
            auto it = keysMap.find(key);
            if(it == keysMap.end())
                continue;
            
            switch(it->second)
            {
                case keys::mainWindow_width:{
                    config.mainWindow_width = std::stoi(value); break;}
                case keys::mainWindow_height:{
                    config.mainWindow_height = std::stoi(value); break;}
                case keys::mainColor:{
                    std::stringstream ss(value);
                    std::string subValue;
                    for(int i{0}; i < 4; i++)
                    {
                        std::getline(ss, subValue, ',');
                        if(i == 0)
                            config.mainColor.r = std::stoi(subValue);
                        else if (i == 1)
                            config.mainColor.g = std::stoi(subValue);
                        else if (i == 2)
                            config.mainColor.b = std::stoi(subValue);
                        else
                            config.mainColor.a = std::stoi(subValue);
                    } break;}
                case keys::secondaryColor:{
                    std::stringstream ss(value);
                    std::string subValue;
                    for(int i{0}; i < 4; i++)
                    {
                        std::getline(ss, subValue, ',');
                        if(i == 0)
                            config.secondaryColor.r = std::stoi(subValue);
                        else if (i == 1)
                            config.secondaryColor.g = std::stoi(subValue);
                        else if (i == 2)
                            config.secondaryColor.b = std::stoi(subValue);
                        else
                            config.secondaryColor.a = std::stoi(subValue);
                    } break;}
                case keys::backColor:{
                    std::stringstream ss(value);
                    std::string subValue;
                    for(int i{0}; i < 4; i++)
                    {
                        std::getline(ss, subValue, ',');
                        if(i == 0)
                            config.backColor.r = std::stoi(subValue);
                        else if (i == 1)
                            config.backColor.g = std::stoi(subValue);
                        else if (i == 2)
                            config.backColor.b = std::stoi(subValue);
                        else
                            config.backColor.a = std::stoi(subValue);
                    } break;}
                default:{
                    st::msg_warn(
                        std::string{"Config: Parameter " + key
                            + " with value " + value + " not recognized."});
                    break;}
            }
        }
        configFile.close();
        return config;
    }

    std::ofstream configFile(path);
    if(configFile.is_open())
    {
        configFile << "mainWindow_width="
                   << std::to_string(config.mainWindow_width) << std::endl;
        configFile << "mainWindow_height="
                   << std::to_string(config.mainWindow_height) << std::endl;
        configFile << "mainColor="
                   << std::to_string(config.mainColor.r) << ","
                   << std::to_string(config.mainColor.g) << ","
                   << std::to_string(config.mainColor.b) << ","
                   << std::to_string(config.mainColor.a) << std::endl;
        configFile << "secondaryColor="
                   << std::to_string(config.secondaryColor.r) << ","
                   << std::to_string(config.secondaryColor.g) << ","
                   << std::to_string(config.secondaryColor.b) << ","
                   << std::to_string(config.secondaryColor.a) << std::endl;
        configFile << "backColor="
                   << std::to_string(config.backColor.r) << ","
                   << std::to_string(config.backColor.g) << ","
                   << std::to_string(config.backColor.b) << ","
                   << std::to_string(config.backColor.a) << std::endl;
        configFile.close();
    }

    return config;
}

#endif
