#ifndef ST_SFML_HPP
#define ST_SFML_HPP

// This part of ST_Library is designed to compliment SFML in a way of providing
// custom shapes and containers that are not present in SFML itself, but based
// on the classes from this beautiful library.

#include <SFML\Graphics.hpp>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <optional>
#include <filesystem>
#include <numbers>
#include <cmath>
#include "st_system.hpp"
#include "st_vector.hpp"
#include "st_string.hpp"

namespace st_sfml
{
// ROUNDED QUADS ///////////////////////////////////////////////////////////////
class RoundedQuad : public sf::Drawable
{
    sf::VertexArray verts;
    sf::VertexArray innerVerts;
    std::size_t numberOfVerts;
    float radius;
    int thickness;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        target.draw(this->verts, states);
        if(thickness)
            target.draw(this->innerVerts, states);
    }

public:
    RoundedQuad(sf::Vector2f center, sf::Vector2f size, const sf::Color &color
               ,int thickness = 0
               ,const sf::Color &innerColor = sf::Color::Black
               ,std::size_t numberOfVerts = 30, float radius = 100.f)
        : verts{sf::PrimitiveType::TriangleFan, numberOfVerts}
        , innerVerts{sf::PrimitiveType::TriangleFan
            , numberOfVerts * !!thickness}
        , numberOfVerts{numberOfVerts}
        , radius{radius}
        , thickness{thickness}
    {
        if(numberOfVerts < 10)
        {
            st::msg_err("ERROR: Trying to create a RoundedQuad");
            st::msg_err(" with less than 10 vertices!");
            exit(-1);
        }
        // placing vertices
        int cornerSwitch = 0;
        int circPointsSwitch = 1;
        double slice{2 * std::numbers::pi / (this->numberOfVerts - 6)};
        // clockwise starting from bottom-right
        sf::Vector2f cornerCenters[4]{
            {static_cast<float>((center.x + size.x * 0.5f) - this->radius)
            ,static_cast<float>((center.y + size.y * 0.5f) - this->radius)},
            {static_cast<float>((center.x - size.x * 0.5f) + this->radius)
            ,static_cast<float>((center.y + size.y * 0.5f) - this->radius)},
            {static_cast<float>((center.x - size.x * 0.5f) + this->radius)
            ,static_cast<float>((center.y - size.y * 0.5f) + this->radius)},
            {static_cast<float>((center.x + size.x * 0.5f) - this->radius)
            ,static_cast<float>((center.y - size.y * 0.5f) + this->radius)}};

        for(int i{0}; i < this->numberOfVerts; i++)
        {
            switch(i)
            {
                case 0:{this->verts[i].position = center; break;}
                case 1:{this->verts[i].position
                    = {cornerCenters[0].x + this->radius, cornerCenters[0].y};
                    break;}
                case 8:{this->verts[i].position
                    = {cornerCenters[1].x, cornerCenters[1].y + this->radius};
                    cornerSwitch++; break;}
                case 15:{this->verts[i].position
                    = {cornerCenters[2].x - this->radius, cornerCenters[2].y};
                    cornerSwitch++; break;}
                case 22:{this->verts[i].position
                    = {cornerCenters[3].x, cornerCenters[3].y - this->radius};
                    cornerSwitch++; break;}
                case 29:{this->verts[i].position
                    = {cornerCenters[0].x + this->radius, cornerCenters[0].y};
                    break;}
                default:{double angle{slice * circPointsSwitch};
                    this->verts[i].position
                        = {static_cast<float>(cornerCenters[cornerSwitch].x
                            + this->radius * std::cos(angle))
                        ,  static_cast<float>(cornerCenters[cornerSwitch].y
                            + this->radius * std::sin(angle))};
                    circPointsSwitch++; break;}
            }
        }
        // set color
        for(int i{0}; i < this->numberOfVerts; i++)
            this->verts[i].color = color;

        // create inner quad to make original quad appear as outline with
        // thickness
        if(thickness == 0)
            return;

        cornerSwitch = 0;
        for(int i{0}; i < this->numberOfVerts; i++)
        {
            this->innerVerts[i].color = innerColor;
            if(i == 0)
            {
                this->innerVerts[i].position = center;
                continue;
            }
            if(i == this->numberOfVerts - 1)
            {
                this->innerVerts[i].position = this->innerVerts[1].position;
                break;
            }

            st::Vector2D offset{
                cornerCenters[cornerSwitch].x - this->verts[i].position.x,
                cornerCenters[cornerSwitch].y - this->verts[i].position.y};
            offset = st::normalize(offset);
            offset *= thickness;
            this->innerVerts[i].position = {
                this->verts[i].position.x + offset.x,
                this->verts[i].position.y + offset.y};

            if(i % ((numberOfVerts - 2) / 4) == false)
                cornerSwitch++;
        }
    }

    sf::Vector2f getCenter()
    {
        return this->verts[0].position;
    }

    void setCenter(const sf::Vector2f &center)
    {
        sf::Vector2f position_difference(
            center.x - this->verts[0].position.x,
            center.y - this->verts[0].position.y);
        for(int i{0}; i < this->numberOfVerts; i++)
        {
            this->verts[i].position.x += position_difference.x;
            this->verts[i].position.y += position_difference.y;
        }
    }
};
// TEXTURES ////////////////////////////////////////////////////////////////////
struct Textures : public sf::Texture
{
    const std::vector<sf::Texture> textures;
    const std::vector<std::string> names;

    Textures(const std::string &folderPath)
        : textures([&]() -> std::vector<sf::Texture>
            {
                namespace fs = std::filesystem;
                std::vector<sf::Texture> temp;
                for(const auto &image : fs::directory_iterator(folderPath))
                {
                    std::string extension{image.path().string()};
                    extension = extension.substr(extension.find_last_of('.'));
                    if(extension != ".png" && extension != ".jpg")
                        continue;
                    sf::Texture texture{image.path()};
                    temp.push_back(texture);
                }
                return temp;
            }())
        , names{[&]() -> std::vector<std::string>
            {
                namespace fs = std::filesystem;
                std::vector<std::string> temp;
                for(const auto &image : fs::directory_iterator(folderPath))
                {
                    std::string name{image.path().string()};
                    name = name.substr(name.find_last_of("\\") + 1);
                    std::string extension{name.substr(name.find_last_of('.'))};
                    if(extension != ".png" && extension != ".jpg")
                        continue;
                    temp.push_back(name);
                }
                return temp;
            }()}
    {}

    const sf::Texture& operator [](const int &index)
    {
        return this->textures[index];
    }

    const sf::Texture& operator [](const std::string &name)
    {
        for(std::size_t i{0}; i < this->names.size(); i++)
        {
            std::string noextName =
                this->names[i].substr(0, this->names[i].find_last_of('.'));
            if(name == noextName)
                return this->textures[i];
        }
        st::msg_err({"Error: no texture with name " + name + "!"});
        exit(-1);
    }
};

inline void print(const Textures &textures)
{
    std::cout << "Textures:\n";
    for(size_t i{0}; i < textures.names.size(); i++)
    {
        std::cout << "\t" << i << ". " << textures.names[i] << "\n";
    }
    std::cout << "end of Textures." << std::endl;
}
// BUTTON //////////////////////////////////////////////////////////////////////
class Button : public sf::Drawable
{
    friend class Buttons;

    std::string name;
    std::optional<sf::Text> text;
    std::optional<const sf::Texture*> texture;
    std::optional<sf::Sprite> sprite;
    sf::VertexArray shape;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        target.draw(this->shape, states);
        if(this->text)
            target.draw(this->text.value(), states);
        else
            target.draw(this->sprite.value(), states);
    }

public:
    // Button with text constructor.............................................
    Button(const std::string  &name,
           const sf::Vector2f &position,
           const sf::Vector2f &size,
           const sf::Font     &font,
           const std::string  &text      = "",
           const unsigned int &textSize  = 0,
           const sf::Color    &textColor = sf::Color::Black,
           const sf::Color    &color     = sf::Color::Magenta)
        : name{name}
        , text{{font, text, textSize}}
        , shape{sf::VertexArray{sf::PrimitiveType::Triangles, 6}}
    {
        // compose collider-shape
        this->shape[0].position = position - size / 2.f;
        this->shape[1].position = {position.x + size.x / 2.f,
            position.y - size.y / 2.f};
        this->shape[2].position = {position.x + size.x / 2.f,
            position.y + size.y / 2.f};
        this->shape[3].position = this->shape[0].position;
        this->shape[4].position = {position.x - size.x / 2.f,
            position.y + size.y / 2.f};
        this->shape[5].position = this->shape[2].position;
        for(int i{0}; i < this->shape.getVertexCount(); i++)
            this->shape[i].color = color;

        // set text color and position
        if(this->text->getString().isEmpty())
            return;

        this->text->setFillColor(textColor);
        sf::Vector2f centeredOrigin =
            {this->text->getGlobalBounds().size / 2.f +
                this->text->getLocalBounds().position};
        this->text->setOrigin(
            {std::round(centeredOrigin.x), std::round(centeredOrigin.y)});
        this->text->setPosition(position);
    }
    // Button with icon constructor ............................................
    Button(const std::string  &name,
           const sf::Vector2f &position,
           const sf::Vector2f &size,
           const sf::Texture  *icon,
           const sf::Color    &color     = sf::Color::Magenta)
        : name{name}
        , texture(icon)
        , sprite{*this->texture.value()}
        , shape{sf::VertexArray{sf::PrimitiveType::Triangles, 6}}
    {
        // compose collider-shape
        this->shape[0].position = position - size / 2.f;
        this->shape[1].position = {position.x + size.x / 2.f,
            position.y - size.y / 2.f};
        this->shape[2].position = {position.x + size.x / 2.f,
            position.y + size.y / 2.f};
        this->shape[3].position = this->shape[0].position;
        this->shape[4].position = {position.x - size.x / 2.f,
            position.y + size.y / 2.f};
        this->shape[5].position = this->shape[2].position;
        for(int i{0}; i < this->shape.getVertexCount(); i++)
            this->shape[i].color = color;
        // set texture size
        sf::Vector2f centeredOrigin =
            {this->sprite->getGlobalBounds().size / 2.f +
                this->sprite->getLocalBounds().position};
        this->sprite->setOrigin(
            {std::round(centeredOrigin.x), std::round(centeredOrigin.y)});
        this->sprite->setPosition(position);
        float scaleFactor_x = size.x /
            float(this->texture.value()->getSize().x);
        float scaleFactor_y = size.y /
            float(this->texture.value()->getSize().y);
        this->sprite->scale({scaleFactor_x, scaleFactor_y});
    }

    Button& setSpriteColor(const sf::Color &color)
    {
        if(this->sprite)
        {
            this->sprite->setColor(color);
            return *this;
        }
        st::msg_err("ERROR: Trying to set sprite on text button!");
        exit(-1);
    }

    Button& setColor(const sf::Color &color)
    {
        for(int i{0}; i < this->shape.getVertexCount(); i++)
            this->shape[i].color = color;
        return *this;
    }

    bool contains(sf::Vector2f point_position) const
    {
        bool hor_overlap{point_position.x >= this->shape[0].position.x &&
            point_position.x <= this->shape[2].position.x};
        bool ver_overlap{point_position.y >= this->shape[0].position.y &&
            point_position.y <= this->shape[2].position.y};
        return hor_overlap && ver_overlap;
    }

    Button& setPosition(sf::Vector2f position)
    {
        sf::Vector2f position_diff = position - (this->shape[0].position +
            (this->shape[1].position - this->shape[0].position) / 2.f);
        for(int i{0}; i < this->shape.getVertexCount(); i++)
        {
            this->shape[i].position += position_diff;
        }
        if(this->sprite)
            this->sprite->setPosition(position);
        else
            this->text->setPosition(this->text->getPosition() + position_diff);
        return *this;
    }

    sf::Vector2f getPosition()
    {
        return
        {
            this->shape[0].position.x +
                (this->shape[2].position.x - this->shape[0].position.x) * 0.5f,
            this->shape[0].position.y +
                (this->shape[2].position.y - this->shape[0].position.y) * 0.5f
        };
    }

    sf::Vector2f getSize()
    {
        return {this->shape[2].position.x - this->shape[0].position.x,
                this->shape[2].position.y - this->shape[0].position.y};
    }

    Button& setSize(const sf::Vector2f &size)
    {
        sf::Vector2f sizeDiff = size - this->getSize();
        this->shape[0].position =
            {this->shape[0].position.x - sizeDiff.x * 0.5f,
             this->shape[0].position.y - sizeDiff.y * 0.5f};
        this->shape[1].position =
            {this->shape[1].position.x + sizeDiff.x * 0.5f,
             this->shape[1].position.y - sizeDiff.y * 0.5f};
        this->shape[2].position =
            {this->shape[2].position.x + sizeDiff.x * 0.5f,
             this->shape[2].position.y + sizeDiff.y * 0.5f};
        this->shape[3].position =
            {this->shape[3].position.x - sizeDiff.x * 0.5f,
             this->shape[3].position.y - sizeDiff.y * 0.5f};
        this->shape[4].position =
            {this->shape[4].position.x - sizeDiff.x * 0.5f,
             this->shape[4].position.y + sizeDiff.y * 0.5f};
        this->shape[5].position =
            {this->shape[5].position.x + sizeDiff.x * 0.5f,
             this->shape[5].position.y + sizeDiff.y * 0.5f};
        return *this;
    }
};

struct Buttons
{
    std::vector<Button> buttons;
    Buttons(const std::vector<Button> &buttons)
        : buttons{buttons}
    {}

    Button& operator [](const std::string &name)
    {
        for(Button& button : this->buttons)
            if(button.name == name)
                return button;
        st::msg_err("ERROR: couldn't find the button " + name);
        exit(-1);
    }

    Button& operator [](int index) {return this->buttons[index];}
};
// QUADS ///////////////////////////////////////////////////////////////////////
class Quads : public sf::Drawable
{
    sf::VertexArray verts;
    std::map<int, std::string> index_name;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        target.draw(this->verts, states);
    }

public:
    Quads(std::size_t numberOfQuads = 1)
        : verts{sf::PrimitiveType::Triangles, numberOfQuads * 6}
    {}

    Quads& assign(const int &index, const std::string &name,
        const sf::Vector2f &position, const sf::Vector2f &size,
        const sf::Color &color)
    {
        // guard against out-of-reach assignment
        if(index * 6 - 5 > this->verts.getVertexCount() - 5)
        {
            st::msg_err("ERROR: in file " + std::string(__FILE__) +
                " on line " + std::to_string(__LINE__) +
                " in function '" + std::string(__func__) +
                "' an attempt to assign a quad out of range of existing" +
                " sf::VertexArray.\nEntered quad number " +
                std::to_string(index) + " whereas there are " +
                std::to_string(this->getQuadsCount()) +
                " quads in the container.\nTerminating.");
            exit(-1);
        }

        this->index_name[index] = name;
        // set vertices positions
        int firstIndex = index * 6 - 6;
        this->verts[firstIndex].position = position;
        this->verts[firstIndex + 1].position =
            {position.x + size.x, position.y};
        this->verts[firstIndex + 2].position =
            {position.x, position.y + size.y};
        this->verts[firstIndex + 3].position =
            {position.x, position.y + size.y};
        this->verts[firstIndex + 4].position =
            {position.x + size.x, position.y};
        this->verts[firstIndex + 5].position =
            {position.x + size.x, position.y + size.y};
        // set vertices colors
        for(int i{firstIndex}; i < firstIndex + 6; i++)
            this->verts[i].color = color;

        return *this;
    }

    Quads& append(const std::string &name, const sf::Vector2f &position,
        const sf::Vector2f &size, const sf::Color &color)
    {
        sf::Vertex topLeft{position, color};
        sf::Vertex topRight{{position.x + size.x, position.y}, color};
        sf::Vertex bottomLeft{{position.x, position.y + size.y}, color};
        sf::Vertex bottomRight{{position.x + size.x, position.y + size.y},
            color};

        this->verts.append(topLeft);
        this->verts.append(topRight);
        this->verts.append(bottomLeft);
        this->verts.append(bottomLeft);
        this->verts.append(topRight);
        this->verts.append(bottomRight);

        this->index_name.insert({this->verts.getVertexCount() / 6, name});
        return *this;
    }

    sf::VertexArray& getVertexArray() {return this->verts;}

    int getQuadsCount() const {return this->index_name.size();}

    int getVertexCount() const {return this->index_name.size() * 6;}

    std::optional<sf::Vector2f> getQuadCenter(const int &index)
    {
        if(auto iter = this->findQuad(index) != this->index_name.end())
        {
            return this->getQuadPosition(index).value() +
                this->getQuadSize(index).value() / 2.f;
        }
        return std::nullopt;
    }

    std::optional<sf::Vector2f> getQuadCenter(const std::string &name)
    {
        if(auto iter = this->findQuad(name) != this->index_name.end())
        {
            return this->getQuadPosition(name).value() +
                this->getQuadSize(name).value() / 2.f;
        }
        return std::nullopt;
    }

    std::optional<sf::Vector2f> getQuadPosition(const std::string &name)
    {
        auto iter = this->findQuad(name);
        if(iter != this->index_name.end())
            return this->verts[iter->first * 6 - 6].position;
        return std::nullopt;
    }

    std::optional<sf::Vector2f> getQuadPosition(const int &index)
    {
        auto iter = this->findQuad(index);
        if(iter != this->index_name.end())
            return this->verts[iter->first * 6 - 6].position;
        return std::nullopt;
    }

    Quads& setQuadPosition(const int &index, const sf::Vector2f &position)
    {
        auto iter = this->findQuad(index);
        if(iter == this->index_name.end())
            return *this;

        int firstIndex = iter->first * 6 - 6;
        sf::Vector2f position_difference(
            position.x - this->verts[firstIndex].position.x,
            position.y - this->verts[firstIndex].position.y);
        for(int i = firstIndex; i < firstIndex + 6; i++)
        {
            this->verts[i].position.x += position_difference.x;
            this->verts[i].position.y += position_difference.y;
        }
        return *this;
    }

    Quads& setQuadPosition(const std::string &name,
                           const sf::Vector2f &position)
    {
        auto iter = this->findQuad(name);
        if(iter == this->index_name.end())
            return *this;

        int firstIndex = iter->first * 6 - 6;
        sf::Vector2f position_difference(
            position.x - this->verts[firstIndex].position.x,
            position.y - this->verts[firstIndex].position.y);
        for(int i = firstIndex; i < firstIndex + 6; i++)
        {
            this->verts[i].position.x += position_difference.x;
            this->verts[i].position.y += position_difference.y;
        }
        return *this;
    }

    std::optional<sf::Vector2f> getQuadSize(const int &index)
    {
        auto iter = this->findQuad(index);
        if(iter == this->index_name.end())
            return std::nullopt;

        int firstIndex = iter->first * 6 - 6;
        float width = this->verts[firstIndex + 1].position.x -
            this->verts[firstIndex].position.x;
        float height = this->verts[firstIndex + 2].position.y -
            this->verts[firstIndex].position.y;
        return sf::Vector2f{width, height};
    }

    std::optional<sf::Vector2f> getQuadSize(const std::string &name)
    {
        auto iter = this->findQuad(name);
        if(iter == this->index_name.end())
            return std::nullopt;

        int firstIndex = iter->first * 6 - 6;
        float width = this->verts[firstIndex + 1].position.x -
            this->verts[firstIndex].position.x;
        float height = this->verts[firstIndex + 2].position.y -
            this->verts[firstIndex].position.y;
        return sf::Vector2f(width, height);
    }

    Quads& setQuadSize(const int &index, const sf::Vector2f &size)
    {
        auto iter = this->findQuad(index);
        if(iter == this->index_name.end())
            return *this;

        int firstIndex = iter->first * 6 - 6;
        sf::Vertex firstVert = this->verts[firstIndex];
        
        this->verts[firstIndex + 1].position.x = firstVert.position.x + size.x;
        this->verts[firstIndex + 2].position.y = firstVert.position.y + size.y;
        this->verts[firstIndex + 3].position.y = firstVert.position.y + size.y;
        this->verts[firstIndex + 4].position.x = firstVert.position.x + size.x;
        this->verts[firstIndex + 5].position = firstVert.position + size;
        return *this;
    }

    Quads& setQuadSize(const std::string &name, const sf::Vector2f &size)
    {
        auto iter = this->findQuad(name);
        if(iter == this->index_name.end())
            return *this;

        int firstIndex = iter->first * 6 - 6;
        sf::Vertex firstVert = this->verts[firstIndex];
        
        this->verts[firstIndex + 1].position.x = firstVert.position.x + size.x;
        this->verts[firstIndex + 2].position.y = firstVert.position.y + size.y;
        this->verts[firstIndex + 3].position.y = firstVert.position.y + size.y;
        this->verts[firstIndex + 4].position.x = firstVert.position.x + size.x;
        this->verts[firstIndex + 5].position = firstVert.position + size;
        return *this;
    }

    std::map<int,std::string>::iterator findQuad(const int &index)
    {
        auto iter = this->index_name.find(index);
        // guard against searching nonexisting quad
        if(iter == this->index_name.end())
            st::msg_warn("Warning: In file " + std::string(__FILE__) +
                " on line " + std::to_string(__LINE__) +
                " in function '" + std::string(__func__) +
                "' a quad with number '" + std::to_string(index) +
                "' was not found!");
        return iter;
    }

    std::map<int,std::string>::iterator findQuad(const std::string &name)
    {
        auto iter = std::find_if(
            this->index_name.begin(), this->index_name.end(),
            [&name](const std::pair<int, std::string> &pair)
                {return pair.second == name;});
        // guard against searching nonexisting quad
        if(iter == this->index_name.end())
            st::msg_warn("Warning: In file " + std::string(__FILE__) +
                " on line " + std::to_string(__LINE__) +
                " in function '" + std::string(__func__) +
                "' a quad with name '" + name + "' was not found!");
        return iter;
    }
};

// TYPE FIELD //////////////////////////////////////////////////////////////////
// Display small box with a message and a field for user input ////////////////
class InputBox : public sf::Drawable
{
    sf::VertexArray shapes;
    sf::Text message;
    sf::Text input;
    sf::Text hint;
    Button accept;
    Button cancel;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        target.draw(this->shapes, states);
        target.draw(this->message, states);
        target.draw(this->hint, states);
        target.draw(this->input, states);
        target.draw(this->accept, states);
        target.draw(this->cancel, states);
    }

public:
    InputBox(sf::RenderWindow       &window,
             const sf::Font         &font,
             const std::string      &message,
             const sf::Color        &frontColor = sf::Color::Black,
             const sf::Color        &backColor = sf::Color::White,
             const sf::Color        &acceptColor = sf::Color::Green,
             const sf::Color        &cancelColor = sf::Color::Red,
             const unsigned int     &textSize = 20,
             const std::string      &hint = "Enter input here")
        : shapes{[&]() -> sf::VertexArray
            {
                sf::VertexArray temp{sf::PrimitiveType::Triangles, 18};
                // greyed-out background
                temp[0].position = {0.f, 0.f};
                temp[1].position = {(float)window.getSize().x, 0.f};
                temp[2].position = {0.f, (float)window.getSize().y};
                temp[3].position = {(float)window.getSize().x,
                    (float)window.getSize().y};
                temp[4].position = {temp[1].position};
                temp[5].position = {temp[2].position};
                // box shape
                sf::Vector2f shapeSize{300.f, 225.f};
                sf::Text msgTemp{font, st::String{message, 27}.getString(), 20};
                if(msgTemp.getLocalBounds().size.y > 75.f)
                    shapeSize.y += msgTemp.getLocalBounds().size.y - 75.f;
                temp[6].position =
                    {window.getSize().x * 0.5f - shapeSize.x * 0.5f,
                    window.getSize().y * 0.5f - shapeSize.y * 0.5f};
                temp[7].position =
                    {window.getSize().x * 0.5f + shapeSize.x * 0.5f,
                    window.getSize().y * 0.5f - shapeSize.y * 0.5f};
                temp[8].position =
                    {window.getSize().x * 0.5f - shapeSize.x * 0.5f,
                    window.getSize().y * 0.5f + shapeSize.y * 0.5f};
                temp[9].position =
                    {window.getSize().x * 0.5f + shapeSize.x * 0.5f,
                    window.getSize().y * 0.5f + shapeSize.y * 0.5f};
                temp[10].position = {temp[7].position};
                temp[11].position = {temp[8].position};
                // input field shape
                temp[12].position =
                    {temp[8].position.x + 25.f, temp[8].position.y - 100.f};
                temp[13].position =
                    {temp[7].position.x - 25.f, temp[12].position.y};
                temp[14].position =
                    {temp[12].position.x, temp[12].position.y + 30.f};
                temp[15].position = {temp[13].position.x, temp[14].position.y};
                temp[16].position = {temp[13].position};
                temp[17].position = {temp[14].position};
                // set shapes colors
                for(int i{0}; i < temp.getVertexCount(); ++i)
                {
                    if(i > 11)
                        temp[i].color = frontColor;
                    else if(i > 5)
                        temp[i].color = backColor;
                    else
                        temp[i].color = sf::Color{0, 0, 0, 150};
                }
                return temp;
            }()}
        , message{[&]()-> sf::Text
            {
                sf::Text temp{font, st::String{message, 27}.getString(), 20};
                temp.setFillColor(frontColor);
                temp.setPosition({this->shapes[6].position.x + 25.f,
                    this->shapes[6].position.y + 25.f});
                return temp;
            }()}
        , input{[&]()-> sf::Text
            {
                sf::Text temp{font, "", textSize};
                temp.setFillColor(backColor);
                float centeredY{temp.getGlobalBounds().size.y * 0.5f +
                    temp.getLocalBounds().position.y};
                temp.setOrigin({temp.getOrigin().x, std::round(centeredY)});
                temp.setPosition({this->shapes[12].position.x + 18.f,
                    this->shapes[12].position.y + (this->shapes[14].position.y -
                    this->shapes[12].position.y) * 0.5f});
                return temp;
            }()}
        , hint{[&]()-> sf::Text
            {
                sf::Text temp{font, hint, textSize};
                temp.setFillColor({backColor.r, backColor.g, backColor.b,
                    static_cast<uint8_t>(backColor.a * 0.5)});
                float centeredY{temp.getGlobalBounds().size.y * 0.5f +
                    temp.getLocalBounds().position.y};
                temp.setOrigin({temp.getOrigin().x, std::round(centeredY)});
                temp.setPosition(this->input.getPosition());
                return temp;
            }()}
        , accept{"accept", {shapes[14].position.x + 50.f,
            shapes[14].position.y + 30.f},
            {100.f, 30.f}, font, "accept",
            textSize, backColor, frontColor}
        , cancel{"cancel", {shapes[15].position.x - 50.f,
            shapes[14].position.y + 30.f},
            {100.f, 30.f}, font, "cancel",
            textSize, backColor, frontColor}
    {
        sf::Texture screenshot{window.getSize()};
        screenshot.update(window);
        sf::Sprite previousGraphics{screenshot};
        while(window.isOpen())
        {
            sf::Vector2i mousePos{sf::Mouse::getPosition(window)};
            while(const std::optional event = window.pollEvent())
            {
                if(event->is<sf::Event::Closed>())
                    window.close();
                if(this->accept.contains({(float)mousePos.x,
                    (float)mousePos.y}))
                {
                    this->accept.setColor(acceptColor);
                }
                else
                    this->accept.setColor(frontColor);
                if(this->cancel.contains({(float)mousePos.x,
                    (float)mousePos.y}))
                {
                    this->cancel.setColor(cancelColor);
                }
                else
                {
                    this->cancel.setColor(frontColor);
                }
            }
            window.clear(sf::Color::Green);
            window.draw(previousGraphics);
            window.draw(*this);
            window.display();
        }
    }
};

}
#endif
