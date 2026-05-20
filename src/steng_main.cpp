#include "steng_config.hpp"
#include "steng_mainWindow.hpp"
#include "st_system.hpp"

int main()
{
    EngineConfig::getInstance().
        loadEngineConfig(std::string{st::getThisProgramLocation()
            + "steng_config.txt"});
    MainWindow mainWindow;
    mainWindow.init();

    return 0;
}
