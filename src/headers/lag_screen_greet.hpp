#ifndef LAG_SCREEN_GREET_HPP
#define LAG_SCREEN_GREET_HPP

#include <SFML/Graphics.hpp>
#include "st_sfml.hpp"
#include "imgui.h"
namespace lag {

class Screen_Greet
{
    sf::RenderWindow &window;
    sf::Font         titleFont;
    sf::Font         subTitleFont;
    sf::Text         title;
    sf::Text         subTitle;
    sf::Sprite       LagniaImage;
    st_sfml::Buttons buttons;

    ImVec2 screen_center;
    ImVec2 button_new_size;
    ImVec2 button_new_pos;
    ImVec2 button_open_size;
    ImVec2 button_open_pos;

    ImColor button_color_text;
    ImColor button_color_idle;
    ImColor button_color_hovered;
    ImColor button_new_color_active;
    ImColor button_open_color_active;
public:
    Screen_Greet(sf::RenderWindow &window, const sf::Texture &image);

    void handleEvents(const std::optional<sf::Event> &event);
    void update(ImFont *font);
    void draw();
    void updatePositions();
};

}
#endif
