#ifndef ST_SFML_HPP
#define ST_SFML_HPP

#include <SFML\Graphics.hpp>
#include <string>
#include <map>
#include <algorithm>
#include <optional>
#include <filesystem>
#include "st_system.hpp"

namespace st_sfml
{
// BUTTON //////////////////////////////////////////////////////////////////////
class Button : public sf::Drawable
{
    std::string name;
    sf::Texture *texture = nullptr;
    sf::Sprite sprite;
    sf::FloatRect collider;
    sf::VertexArray collider_visuals;

    inline void draw(sf::RenderTarget &target,
        sf::RenderStates states) const override
    {
        target.draw(this->sprite, states);
        if(this->show_collider)
            target.draw(this->collider_visuals, states);
    }

public:
    Button(std::string name, std::filesystem::path imagePath,
        sf::Vector2f position, sf::Vector2f size)
        : name{name}
        , texture{new sf::Texture{imagePath}}
        , sprite{*this->texture}
        , collider{position, size}
        , collider_visuals{sf::VertexArray{sf::PrimitiveType::LineStrip, 5}}
    {
        this->sprite.setPosition(position);
        // compose collider
        this->collider_visuals[0].position = position;
        this->collider_visuals[1].position = {position.x + size.x, position.y};
        this->collider_visuals[2].position =
            {position.x + size.x, position.y + size.y};
        this->collider_visuals[3].position = {position.x, position.y + size.y};
        this->collider_visuals[4].position = position;
        // set texture size
        float scaleFactor_x = size.x / float(this->texture->getSize().x);
        float scaleFactor_y = size.y / float(this->texture->getSize().y);
        this->sprite.scale({scaleFactor_x, scaleFactor_y});
    }
    ~Button()
    {
        delete this->texture;
    }

    inline void setColor(sf::Color color) {this->sprite.setColor(color);}
    bool show_collider = false;

    inline void setColliderColor(sf::Color color)
    {
        for(int i{0}; i < this->collider_visuals.getVertexCount(); i++)
            this->collider_visuals[i].color = color;
    }

    inline bool contains(sf::Vector2i point_position) const
    {
        return this->collider.contains({(float)point_position.x,
            (float)point_position.y});
    }

    inline void setPosition(sf::Vector2f position)
    {
        sf::Vector2f position_diff = position - this->collider.position;
        this->sprite.setPosition(position);
        this->collider.position = position;
        for(int i{0}; i < this->collider_visuals.getVertexCount(); i++)
        {
            this->collider_visuals[i].position += position_diff;
        }
    }

    inline sf::Vector2f getPosition() {return this->sprite.getPosition();}
};
// QUADS ///////////////////////////////////////////////////////////////////////
class Quads : public sf::Drawable
{
    sf::VertexArray verts;
    std::map<int, std::string> numbers_names;

    inline void draw(sf::RenderTarget &target,
        sf::RenderStates states) const override
    {
        target.draw(this->verts, states);
    }

public:
    Quads(int numberOfQuads)
        : verts(sf::VertexArray(sf::PrimitiveType::Triangles,
            numberOfQuads * 6))
    {}
    ~Quads(){}

    inline void assign(int quadNumber, std::string name,
        sf::Vector2f position, sf::Vector2f size, sf::Color color)
    {
        // guard against out-of-reach assignment
        if(quadNumber * 6 - 5 > this->verts.getVertexCount() - 5)
        {
            st::msg_err("ERROR: in file " + std::string(__FILE__) +
                " on line " + std::to_string(__LINE__) +
                " in function '" + std::string(__func__) +
                "' an attempt to assign a quad out of range of existing" +
                " sf::VertexArray.\nEntered quad number " +
                std::to_string(quadNumber) + " whereas there are " +
                std::to_string(this->getQuadsCount()) +
                " quads in the container.\nTerminating.");
            exit(-1);
        }

        // check if the key already exists
        if(this->numbers_names.find(quadNumber) !=
            this->numbers_names.end())
            this->numbers_names[quadNumber] = name;
        else
            this->numbers_names.insert({quadNumber, name});

        int firstIndex = quadNumber * 6 - 6;
        this->verts[firstIndex].position = position;
        this->verts[firstIndex + 1].position = sf::Vector2f(
            position.x + size.x, position.y);
        this->verts[firstIndex + 2].position = sf::Vector2f(
            position.x, position.y + size.y);
        this->verts[firstIndex + 3].position = sf::Vector2f(
            position.x, position.y + size.y);
        this->verts[firstIndex + 4].position = sf::Vector2f(
            position.x + size.x, position.y);
        this->verts[firstIndex + 5].position = sf::Vector2f(
            position.x + size.x, position.y + size.y);

        for(int i = firstIndex; i < firstIndex + 6; i++)
            this->verts[i].color = color;
    }

    inline void append(std::string name, sf::Vector2f position,
        sf::Vector2f size, sf::Color color)
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

        this->numbers_names.insert({this->verts.getVertexCount() / 6, name});
    }

    inline sf::VertexArray& getVertexArray() {return this->verts;}

    inline std::optional<sf::Vector2f> getQuadCenter(int number)
    {
        if(auto iter = this->findQuad(number) != this->numbers_names.end())
        {
            return this->getQuadPosition(number).value() +
                this->getQuadSize(number).value() / 2.f;
        }
        return std::nullopt;
    }

    inline std::optional<sf::Vector2f> getQuadCenter(std::string name)
    {
        if(auto iter = this->findQuad(name) != this->numbers_names.end())
        {
            return this->getQuadPosition(name).value() +
                this->getQuadSize(name).value() / 2.f;
        }
        return std::nullopt;
    }

    inline int getQuadsCount() const
        {return this->numbers_names.size();}

    inline int getVertexCount() const
        {return this->numbers_names.size() * 6;}

    inline std::optional<sf::Vector2f> getQuadPosition(std::string name)
    {
        auto iter = this->findQuad(name);
        if(iter != this->numbers_names.end())
            return this->verts[iter->first * 6 - 6].position;
        return std::nullopt;
    }

    inline std::optional<sf::Vector2f> getQuadPosition(int number)
    {
        auto iter = this->findQuad(number);
        if(iter != this->numbers_names.end())
            return this->verts[iter->first * 6 - 6].position;
        return std::nullopt;
    }

    inline void setQuadPosition(int number, sf::Vector2f position)
    {
        auto iter = this->findQuad(number);
        if(iter == this->numbers_names.end())
            return;

        int firstIndex = iter->first * 6 - 6;
        sf::Vector2f position_difference(
            position.x - this->verts[firstIndex].position.x,
            position.y - this->verts[firstIndex].position.y);
        for(int i = firstIndex; i < firstIndex + 6; i++)
        {
            this->verts[i].position.x += position_difference.x;
            this->verts[i].position.y += position_difference.y;
        }
    }

    inline void setQuadPosition(std::string name, sf::Vector2f position)
    {
        auto iter = this->findQuad(name);
        if(iter == this->numbers_names.end())
            return;

        int firstIndex = iter->first * 6 - 6;
        sf::Vector2f position_difference(
            position.x - this->verts[firstIndex].position.x,
            position.y - this->verts[firstIndex].position.y);
        for(int i = firstIndex; i < firstIndex + 6; i++)
        {
            this->verts[i].position.x += position_difference.x;
            this->verts[i].position.y += position_difference.y;
        }
    }

    inline std::optional<sf::Vector2f> getQuadSize(int number)
    {
        auto iter = this->findQuad(number);
        if(iter == this->numbers_names.end())
            return std::nullopt;

        int index = iter->first * 6 - 6;
        float width = this->verts[index + 1].position.x -
            this->verts[index].position.x;
        float height = this->verts[index + 2].position.y -
            this->verts[index].position.y;
        return sf::Vector2f(width, height);
    }

    inline std::optional<sf::Vector2f> getQuadSize(std::string name)
    {
        auto iter = this->findQuad(name);
        if(iter == this->numbers_names.end())
            return std::nullopt;

        int index = iter->first * 6 - 6;
        float width = this->verts[index + 1].position.x -
            this->verts[index].position.x;
        float height = this->verts[index + 2].position.y -
            this->verts[index].position.y;
        return sf::Vector2f(width, height);
    }

    inline void setQuadSize(int number, sf::Vector2f size)
    {
        auto iter = this->findQuad(number);
        if(iter == this->numbers_names.end())
            return;

        int index = iter->first * 6 - 6;
        sf::Vertex firstVert = this->verts[index];
        
        this->verts[index + 1].position.x = firstVert.position.x + size.x;
        this->verts[index + 2].position.y = firstVert.position.y + size.y;
        this->verts[index + 3].position.y = firstVert.position.y + size.y;
        this->verts[index + 4].position.x = firstVert.position.x + size.x;
        this->verts[index + 5].position = firstVert.position + size;
    }

    inline void setQuadSize(std::string name, sf::Vector2f size)
    {
        auto iter = this->findQuad(name);
        if(iter == this->numbers_names.end())
            return;

        int index = iter->first * 6 - 6;
        sf::Vertex firstVert = this->verts[index];
        
        this->verts[index + 1].position.x = firstVert.position.x + size.x;
        this->verts[index + 2].position.y = firstVert.position.y + size.y;
        this->verts[index + 3].position.y = firstVert.position.y + size.y;
        this->verts[index + 4].position.x = firstVert.position.x + size.x;
        this->verts[index + 5].position = firstVert.position + size;
    }

    inline std::map<int,std::string>::iterator findQuad(int number)
    {
        auto iter = this->numbers_names.find(number);
        // guard against searching nonexisting quad
        if(iter == this->numbers_names.end())
            st::msg_warn("Warning: In file " + std::string(__FILE__) +
                " on line " + std::to_string(__LINE__) +
                " in function '" + std::string(__func__) +
                "' a quad with number '" + std::to_string(number) +
                "' was not found!");
        return iter;            
    }

    inline std::map<int,std::string>::iterator findQuad(
        std::string name)
    {
        auto iter = std::find_if(
            this->numbers_names.begin(), this->numbers_names.end(),
            [&name](const std::pair<int, std::string> &pair)
                {return pair.second == name;});
        // guard against searching nonexisting quad
        if(iter == this->numbers_names.end())
            st::msg_warn("Warning: In file " + std::string(__FILE__) +
                " on line " + std::to_string(__LINE__) +
                " in function '" + std::string(__func__) +
                "' a quad with name '" + name + "' was not found!");
        return iter;
    }
};

}

#endif
