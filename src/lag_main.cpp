#include "lag_config.hpp"
#include "lag_window.hpp"
#include "st_system.hpp"

int main()
{
    lag::EngineConfig::getInstance().
        loadEngineConfig(st::getThisProgramLocation() /= "lag_config.cfg");
    lag::Window::get().init();

    return EXIT_SUCCESS;
}
