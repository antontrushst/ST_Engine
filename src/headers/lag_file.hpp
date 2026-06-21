#ifndef LAG_FILE_HPP
#define LAG_FILE_HPP

// class that manages .lag file, which stores all the data the engine needs
// to operate

#include <filesystem>
#include <utility>
#include <string>
#include <map>
#include <fstream>
#include <iostream>

#include "st_system.hpp"

class LagFile
{
    std::filesystem::path location = "";
    std::string gameName = "";

    LagFile() = default;
    
public:
    static LagFile& get()
    {
        static LagFile instance;
        return instance;
    }
    LagFile(const LagFile&) = delete;
    void operator =(const LagFile&) = delete;

    void create(std::filesystem::path path, std::string name);
    void load(std::filesystem::path path);
    void print()
    {
        std::cout << "location = " << this->location << "\n";
        std::cout << "gameName = " << this->gameName << "\n";
    };
};

// FUNCTIONS INPLEMENTATION ....................................................
inline void LagFile::create(std::filesystem::path path, std::string name)
{
    std::ofstream lag_file{path /= std::string{name + ".lag"}};
    lag_file << "GameName=" << name << "\n";
    lag_file << "ProjectLocation=" << path << "\n";

    this->location = std::move(path);
    this->gameName = std::move(name);
    st::msg_ok(".lag file successfully created.");
}

inline void LagFile::load(std::filesystem::path path)
{
    std::ifstream lag_file(path);
    if(!lag_file.is_open())
    {
        st::msg_err("Failed to open .lag file!");
        exit(-1);
    }

    enum class keys
    {
        GameName, ProjectLocation
    };

    std::map<std::string, keys> keyMap
    {
        {"GameName", keys::GameName},
        {"ProjectLocation", keys::ProjectLocation}
    };

    std::string line;
    while(std::getline(lag_file, line))
    {
        std::string value{line.substr(line.find('=') + 1)};
        std::string key{line.erase(line.find('='))};
        auto it{keyMap.find(key)};
        if(it == keyMap.end())
        {
            st::msg_warn("The key '" + key + "' is not found in registry.");
            continue;
        }

        switch(it->second)
        {
            case keys::GameName:{this->gameName = value; break;}
            case keys::ProjectLocation:{break;};
            default:{
                st::msg_warn({"Parameter '" + key + "' with value '" + value +
                    "' is not recognised."}); break;}
        }
    }

    this->location = std::move(path);
    st::msg_ok(".lag file successfuly loaded.");
}

#endif
