#include "lag_screen_greet.hpp"
#include "lag_config.hpp"
#include "lag_file.hpp"
#include "lag_screens.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

lag::Screen_Greet::Screen_Greet(
        sf::RenderWindow &window, const sf::Texture &image)
    : window{window}
    , titleFont{EngineConfig::getInstance().titleFont}
    , subTitleFont{EngineConfig::getInstance().font}
    , title{this->titleFont, "LAGNIA", 96}
    , subTitle{this->subTitleFont, "game engine", 36}
    , LagniaImage{image}
    , buttons{{
        {"newProject",
            {this->window.getSize().x * 0.5f,
            (this->window.getSize().y * 0.01f) * 85 - 55},
            {400.f, 50.f},
            this->subTitleFont, "new", 32,
            EngineConfig::getInstance().color_back,
            EngineConfig::getInstance().color_second},
        {"openProject",
            {this->window.getSize().x * 0.5f,
            (this->window.getSize().y * 0.01f) * 85},
            {400.f, 50.f},
            this->subTitleFont, "open", 32,
            EngineConfig::getInstance().color_back,
            EngineConfig::getInstance().color_second}
    }}
    , screen_center{window.getSize().x * 0.5f, window.getSize().y * 0.5f}
    , button_new_size{400.f, 50.f}
    , button_new_pos{
        this->screen_center.x - this->button_new_size.x * 0.5f,
        ((this->window.getSize().y * 0.01f) * 85 - 55) -
            this->button_new_size.y * 0.5f}
    , button_open_size{400.f, 50.f}
    , button_open_pos{this->button_new_pos.x,
        this->button_new_pos.y + 55.f}
    , button_color_text{
        EngineConfig::getInstance().color_back.r,
        EngineConfig::getInstance().color_back.g,
        EngineConfig::getInstance().color_back.b,
        EngineConfig::getInstance().color_back.a}
    , button_color_idle{
        EngineConfig::getInstance().color_second.r,
        EngineConfig::getInstance().color_second.g,
        EngineConfig::getInstance().color_second.b,
        150}
    , button_color_hovered{
        EngineConfig::getInstance().color_second.r,
        EngineConfig::getInstance().color_second.g,
        EngineConfig::getInstance().color_second.b,
        EngineConfig::getInstance().color_second.a}
    , button_new_color_active{
        EngineConfig::getInstance().color_main.r,
        EngineConfig::getInstance().color_main.g,
        EngineConfig::getInstance().color_main.b,
        EngineConfig::getInstance().color_main.a}
    , button_open_color_active{
        EngineConfig::getInstance().color_alt.r,
        EngineConfig::getInstance().color_alt.g,
        EngineConfig::getInstance().color_alt.b,
        EngineConfig::getInstance().color_alt.a}
{
    // center Lagnia image origin and position it
    sf::Vector2f centeredOrigin{this->LagniaImage.getGlobalBounds().size * .5f +
        this->LagniaImage.getLocalBounds().position};
    this->LagniaImage.setOrigin(centeredOrigin);
    this->LagniaImage.setPosition({
        this->window.getSize().x * 0.5f, this->window.getSize().y * 0.5f});
    // center title and subTitle texts, color and position it
    centeredOrigin = this->title.getGlobalBounds().size * 0.5f +
        this->title.getLocalBounds().position;
    this->title.setOrigin({std::round(centeredOrigin.x),
        std::round(centeredOrigin.y)});
    this->title.setPosition({
        this->window.getSize().x * 0.5f,
        (this->window.getSize().y * 0.01f) * 15});
    this->title.setFillColor(EngineConfig::getInstance().color_second);

    centeredOrigin = this->subTitle.getGlobalBounds().size * 0.5f +
        this->subTitle.getLocalBounds().position;
    this->subTitle.setOrigin({std::round(centeredOrigin.x),
        std::round(centeredOrigin.y)});
    this->subTitle.setPosition({
        this->window.getSize().x * 0.5f,
        this->title.getPosition().y + this->title.getLocalBounds().size.y +
        5.f});
    this->subTitle.setFillColor(EngineConfig::getInstance().color_second);
}

void lag::Screen_Greet::handleEvents(const std::optional<sf::Event> &event)
{
/*    this->buttons["newProject"].
        setColor(EngineConfig::getInstance().color_second);
    this->buttons["openProject"].
        setColor(EngineConfig::getInstance().color_second);
    sf::Vector2i mousePosition{sf::Mouse::getPosition(this->window)};

    if(this->buttons["newProject"].contains({(float)mousePosition.x,
        (float)mousePosition.y}))
    {
        this->buttons["newProject"].setColor(
            EngineConfig::getInstance().color_main);

        if(const auto *mouseButtonPressed =
            event->getIf<sf::Event::MouseButtonPressed>())
            if(mouseButtonPressed->button == sf::Mouse::Button::Left)
            {
                if(std::optional<std::string> gameName{st_sfml::inputPopup(
                    this->window, this->subTitleFont,
                    "Enter name of your game:",
                    EngineConfig::getInstance().color_second,
                    EngineConfig::getInstance().color_back,
                    EngineConfig::getInstance().color_main,
                    EngineConfig::getInstance().color_alt)};
                    gameName.has_value())
                {
                    std::filesystem::path newProject_path{
                        st::getFolder("Select a folder for your game project "
                        "directory to be created in").value_or("")};
                    if(newProject_path.empty())
                        return;

                    std::filesystem::create_directory(
                        newProject_path /= gameName.value());
                    LagFile::get().create(newProject_path, gameName.value());
                    CurrentScreen = Screens::Editor;
                }
            }
    }

    if(this->buttons["openProject"].contains({(float)mousePosition.x,
        (float)mousePosition.y}))
    {
        this->buttons["openProject"].setColor(
            EngineConfig::getInstance().color_main);

        if(const auto *mouseButtonPressed =
            event->getIf<sf::Event::MouseButtonPressed>())
            if(mouseButtonPressed->button == sf::Mouse::Button::Left)
            {
                std::filesystem::path lagFile_path{
                    st::getFile("Select your game project .lag file",
                    {"*.lag"}, {"Lagnia files (*.lag)"}).value_or("")};
                if(lagFile_path.empty())
                    return;

                LagFile::get().load(lagFile_path);
                CurrentScreen = Screens::Editor;
            }
        return;
    }*/
}

void lag::Screen_Greet::update(ImFont *font)
{
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos);
    ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size);
    ImGuiWindowFlags overlayFlags =
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoMove;

    if(!ImGui::Begin("GlobalOverlay", nullptr, overlayFlags))
        st::msg_err("Failed to create ImGui overlay on Screen_Greet!");
    ImGui::PushFont(font);
    // button NEW
    ImGui::SetCursorPos(this->button_new_pos);
    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)this->button_color_text);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)this->button_color_idle);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
        (ImVec4)this->button_color_hovered);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
        (ImVec4)this->button_new_color_active);
    if(ImGui::Button("new", this->button_new_size))
    {std::cout << "PRESSED!\n";}
    ImGui::PopStyleColor(4);
    // button OPEN
    ImGui::SetCursorPos(this->button_open_pos);
    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)this->button_color_text);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)this->button_color_idle);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
        (ImVec4)this->button_color_hovered);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
        (ImVec4)this->button_open_color_active);
    if(ImGui::Button("open", this->button_open_size))
    {std::cout << "PRESSED!\n";}
    ImGui::PopStyleColor(4);

    ImGui::PopFont();

    ImGui::End();
}

void lag::Screen_Greet::draw()
{
    this->window.draw(this->LagniaImage);
    this->window.draw(this->title);
    this->window.draw(this->subTitle);
}

void lag::Screen_Greet::updatePositions()
{
    this->LagniaImage.setPosition({
        (float)this->window.getSize().x * 0.5f,
        (float)this->window.getSize().y * 0.5f});
    this->title.setPosition({
        (float)this->window.getSize().x * 0.5f,
        (this->window.getSize().y * 0.01f) * 15});
    this->subTitle.setPosition({
        (float)this->window.getSize().x * 0.5f,
        this->title.getPosition().y +
            this->title.getLocalBounds().size.y + 5.f});

    this->buttons["openProject"].setPosition({
        (float)this->window.getSize().x * 0.5f,
        ((float)this->window.getSize().y * 0.01f) * 85});
    this->buttons["newProject"].setPosition({
        (float)this->window.getSize().x * 0.5f,
        this->buttons["openProject"].getPosition().y -
            (this->buttons["newProject"].getSize().y + 30)});
}
