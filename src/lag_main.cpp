#include "lag_config.hpp"
#include "lag_mainWindow.hpp"
#include "st_system.hpp"

int main()
{
    EngineConfig::getInstance().
        loadEngineConfig(st::getThisProgramLocation() /= "lag_config.cfg");
    MainWindow::get().init();

    return 0;
}
