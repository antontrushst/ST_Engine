#ifndef LAG_WINDOW_HPP
#define LAG_WINDOW_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>
#include <imgui.h>

#include "st_sfml.hpp"
#include "lag_config.hpp"

namespace lag {

class Screen_Greet;
class Screen_Editor;

class Window
{
    friend class std::default_delete<Window>;
   
    std::unique_ptr<Screen_Greet> screen_greet;
    std::unique_ptr<Screen_Editor> screen_editor;
    sf::RenderWindow window;
    st_sfml::Textures textures;
    ImFont *font_small;
    ImFont *font_regular;
    ImFont *font_large;

public:
    Window();
    ~Window();
    
    void init();
    void handleEvents();
    void update();
    void draw();
};

}
#endif
