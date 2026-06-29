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
{
    this->window.setFramerateLimit(EngineConfig::getInstance().framerate);
}

lag::Window::~Window() = default;

void lag::Window::init()
{
    if(!ImGui::SFML::Init(this->window))
        st::msg_err("ImGUI initialization failed!", true);

//    std::cout << EngineConfig::getInstance().font << std::endl;
    ImGuiIO &io = ImGui::GetIO();
    this->font_small =
        io.Fonts->AddFontFromFileTTF(EngineConfig::getInstance().font
            .string().c_str(), 12.f);
    this->font_regular =
        io.Fonts->AddFontFromFileTTF(EngineConfig::getInstance().font
            .string().c_str(), 18.f);
    this->font_large =
        io.Fonts->AddFontFromFileTTF(EngineConfig::getInstance().font
            .string().c_str(), 24.f);
    this->font_huge =
        io.Fonts->AddFontFromFileTTF(EngineConfig::getInstance().font
            .string().c_str(), 32.f);
    if(!ImGui::SFML::UpdateFontTexture())
        st::msg_err("ImGui-SFML failed to update font texture!");

    this->screen_greet = std::make_unique<lag::Screen_Greet>(
        this->window, this->textures["lagnia_sketch_transp"]);
    this->screen_editor = std::make_unique<lag::Screen_Editor>(this->window);
    
    // MAIN LOOP................................................................
    sf::Clock deltaClock;
    while(this->window.isOpen())
    {
        this->handleEvents();
        ImGui::SFML::Update(this->window, deltaClock.restart());
        this->update();
        this->draw();
    }
    //..........................................................................
    ImGui::SFML::Shutdown();
}

void lag::Window::handleEvents()
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
                this->screen_greet->handleEvents(event); break;}
            case lag::Screens::Editor: {
                this->screen_editor->handleEvents(event); break;}
        }
    }
}

void lag::Window::update()
{
    switch(lag::CurrentScreen)
    {
        case lag::Screens::Greet: {
            this->screen_greet->update(this->font_huge); break;}
        case lag::Screens::Editor: {
            this->screen_editor->update(); break;}
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
