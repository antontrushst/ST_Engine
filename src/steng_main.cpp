#include "steng_config.hpp"
#include "steng_mainWindow.hpp"
#include "st_system.hpp"

int main()
{
    EngineConfig engineConfig{
        loadEngineConfig(std::string{st::getThisProgramLocation()
            + "steng_config.txt"})};
    MainWindow mainWindow{engineConfig};
    mainWindow.init();

    return 0;
}
