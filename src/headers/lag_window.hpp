#ifndef LAG_WINDOW_HPP
#define LAG_WINDOW_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>

#include "st_sfml.hpp"
#include "lag_config.hpp"

namespace lag {

class Screen_Greet;
class Screen_Editor;

class Window
{
    friend struct std::default_delete<Window>;
    
    sf::RenderWindow window;
    st_sfml::Textures textures;
    std::unique_ptr<Screen_Greet> screen_greet;
    std::unique_ptr<Screen_Editor> screen_editor;

    Window();
    ~Window();

public:

    Window(const Window&) = delete;
    void operator  =(const Window&) = delete;
    static Window& get()
    {
        static Window instance;
        return instance;
    }

    void init();
    void update();
    void draw();
};

}
#endif
