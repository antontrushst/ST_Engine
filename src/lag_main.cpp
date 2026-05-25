#include "lag_config.hpp"
#include "lag_mainWindow.hpp"
#include "st_system.hpp"

int main()
{
    EngineConfig::getInstance().
        loadEngineConfig(std::string{st::getThisProgramLocation()
            + "lag_config.cfg"});
    MainWindow mainWindow;
    mainWindow.init();

    return 0;
}
