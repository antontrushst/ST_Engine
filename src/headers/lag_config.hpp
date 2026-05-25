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
    sf::Font font{std::string{
        st::getThisProgramLocation() + "fonts\\Proletarsk.ttf"}};
    sf::Font titleFont{std::string{
        st::getThisProgramLocation() + "fonts\\logo_2K12.ttf"}};
    st_sfml::Textures buttonIcons{std::string{
        st::getThisProgramLocation() + "graphics"}};
    sf::Color mainColor = color::Toxic;
    sf::Color secondaryColor = color::SoftBlack;
    sf::Color backColor = color::Milk;

    EngineConfig& loadEngineConfig(const std::string &path);

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

inline EngineConfig& EngineConfig::loadEngineConfig(const std::string &path)
{
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

            auto it = keysMap.find(key);
            if(it == keysMap.end())
                continue;
            
            switch(it->second)
            {
                case keys::mainWindow_width:{
                    this->mainWindow_width = std::stoi(value); break;}
                case keys::mainWindow_height:{
                    this->mainWindow_height = std::stoi(value); break;}
                case keys::mainColor:{
                    std::stringstream ss(value);
                    std::string subValue;
                    for(int i{0}; i < 4; i++)
                    {
                        std::getline(ss, subValue, ',');
                        if(i == 0)
                            this->mainColor.r = std::stoi(subValue);
                        else if (i == 1)
                            this->mainColor.g = std::stoi(subValue);
                        else if (i == 2)
                            this->mainColor.b = std::stoi(subValue);
                        else
                            this->mainColor.a = std::stoi(subValue);
                    } break;}
                case keys::secondaryColor:{
                    std::stringstream ss(value);
                    std::string subValue;
                    for(int i{0}; i < 4; i++)
                    {
                        std::getline(ss, subValue, ',');
                        if(i == 0)
                            this->secondaryColor.r = std::stoi(subValue);
                        else if (i == 1)
                            this->secondaryColor.g = std::stoi(subValue);
                        else if (i == 2)
                            this->secondaryColor.b = std::stoi(subValue);
                        else
                            this->secondaryColor.a = std::stoi(subValue);
                    } break;}
                case keys::backColor:{
                    std::stringstream ss(value);
                    std::string subValue;
                    for(int i{0}; i < 4; i++)
                    {
                        std::getline(ss, subValue, ',');
                        if(i == 0)
                            this->backColor.r = std::stoi(subValue);
                        else if (i == 1)
                            this->backColor.g = std::stoi(subValue);
                        else if (i == 2)
                            this->backColor.b = std::stoi(subValue);
                        else
                            this->backColor.a = std::stoi(subValue);
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
        configFile << "mainColor="
                   << std::to_string(this->mainColor.r) << ","
                   << std::to_string(this->mainColor.g) << ","
                   << std::to_string(this->mainColor.b) << ","
                   << std::to_string(this->mainColor.a) << std::endl;
        configFile << "secondaryColor="
                   << std::to_string(this->secondaryColor.r) << ","
                   << std::to_string(this->secondaryColor.g) << ","
                   << std::to_string(this->secondaryColor.b) << ","
                   << std::to_string(this->secondaryColor.a) << std::endl;
        configFile << "backColor="
                   << std::to_string(this->backColor.r) << ","
                   << std::to_string(this->backColor.g) << ","
                   << std::to_string(this->backColor.b) << ","
                   << std::to_string(this->backColor.a) << std::endl;
        configFile.close();
    }

    return *this;
}

#endif
