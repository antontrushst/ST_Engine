#ifndef LAG_FILE_HPP
#define LAG_FILE_HPP

// class that manages .lag file, which stores all the data the engine needs
// to operate

#include <filesystem>
#include <utility>

class LagFile
{
    std::filesystem::path location;

    LagFile() = default;
    
public:
    static LagFile& get()
    {
        static LagFile instance;
        return instance;
    }
    LagFile(const LagFile&) = delete;
    void operator =(const LagFile&) = delete;

    LagFile& create(std::filesystem::path path, std::string name);
};

// FUNCTIONS INPLEMENTATION ....................................................
inline LagFile& LagFile::create(std::filesystem::path path, std::string name)
{
    std::ofstream lag_file{path / std::string{name + ".lag"}};
    lag_file << "GameName=" << name << "\n";
    lag_file << "ProjectLocation=" << path << "\n";
    lag_file.close();

    this->location = std::move(path);
    return *this;
}

#endif
