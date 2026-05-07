#include "steng_mainWindow.hpp"
#include "st_system.hpp"
#include "steng_config.hpp"

int main()
{
    std::cout << st::getThisProgramLocation() << std::endl;
    EngineConfig config =
    loadEngineConfig(std::string{st::getThisProgramLocation() + "config.txt"});
    MainWindow mainWindow;
    mainWindow.init();
    std::cout << config.mainWindow_width << "\t" << config.mainWindow_height;
    return 0;
}
