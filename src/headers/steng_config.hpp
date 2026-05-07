#ifndef STENG_CONFIG_HPP
#define STENG_CONFIG_HPP

#include <fstream>
#include <map>

#include "st_system.hpp"

struct EngineConfig
{
    int mainWindow_width = 300;
    int mainWindow_height = 300;
};

inline EngineConfig loadEngineConfig(const std::string &path)
{
    EngineConfig config;

    enum class keys
    {
        mainWindow_width,
        mainWindow_height
    };

    std::map<std::string, keys> keysMap =
    {
        {"mainWindow_width", keys::mainWindow_width},
        {"mainWindow_height", keys::mainWindow_height}
    };

    if(std::filesystem::exists(std::string{path+ "steng_config.txt"}))
    {
        std::ifstream configFile(std::string{path + "steng_config.txt"});
        std::string line;
        while(std::getline(configFile, line))
        {
            std::string value = line.substr(line.find('=')+1);
            std::string key = line.erase(line.find('='));
            std::cout << key << "\t" << value << std::endl;
            auto it = keysMap.find(key);
            if(it == keysMap.end())
                continue;
            
            switch(it->second)
            {
                case keys::mainWindow_width:{
                    config.mainWindow_width = std::stoi(value); break;}
                case keys::mainWindow_height:{
                    config.mainWindow_height = std::stoi(value); break;}
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

    std::ofstream configFile(std::string{path + "steng_config.txt"});
    if(configFile.is_open())
    {
        configFile << "mainWindow_width="
                   << std::to_string(config.mainWindow_width) << std::endl;
        configFile << "mainWindow_height="
                   << std::to_string(config.mainWindow_height) << std::endl;
        configFile.close();
    }

    return config;
}

#endif
