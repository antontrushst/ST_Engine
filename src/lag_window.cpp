#include "lag_window.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "lag_screens.hpp"
#include "lag_screen_greet.hpp"
#include "lag_screen_editor.hpp"

lag::Window::Window()
    : window(sf::RenderWindow(sf::VideoMode(
        {static_cast<unsigned int>(EngineConfig::getInstance().window_width)
        ,static_cast<unsigned int>(EngineConfig::getInstance().window_height)})
        ,"Lagnia"))
    , textures{{st::getThisProgramLocation() /= "graphics"}}
    , screen_greet{std::make_unique<lag::Screen_Greet>(
        this->window, this->textures["lagnia_sketch_transp"])}
    , screen_editor{std::make_unique<lag::Screen_Editor>(this->window)}
{
    this->window.setFramerateLimit(EngineConfig::getInstance().framerate);
}

lag::Window::~Window() = default;

void lag::Window::init()
{
    if(!ImGui::SFML::Init(this->window))
        st::msg_err("ImGUI initialization failed!", true);
    // MAIN LOOP................................................................
    sf::Clock deltaClock;
    while(this->window.isOpen())
    {
        this->update();
        ImGui::SFML::Update(this->window, deltaClock.restart());
        this->draw();
    }
    //..........................................................................
    ImGui::SFML::Shutdown();
}

void lag::Window::update()
{
    while(const auto event = this->window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(this->window, *event);
        if(event->is<sf::Event::Closed>())
        {
            this->window.close();
        }
        else if(const auto* resized = event->getIf<sf::Event::Resized>())
        {
            sf::FloatRect visibleArea{{0, 0},
                {(float)resized->size.x, (float)resized->size.y}};
            this->window.setView(sf::View(visibleArea));
        }

        switch(lag::CurrentScreen)
        {
            case lag::Screens::Greet: {
                this->screen_greet->update(event); break;}
            case lag::Screens::Editor: {
                this->screen_editor->update(event); break;}
        }
    }
}

void lag::Window::draw()
{
    this->window.clear(EngineConfig::getInstance().color_back);
    switch(lag::CurrentScreen)
    {
        case lag::Screens::Greet: {this->screen_greet->draw(); break;}
        case lag::Screens::Editor: {this->screen_editor->draw(); break;}
    }
    ImGui::SFML::Render(this->window);
    this->window.display();
}
