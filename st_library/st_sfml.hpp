#ifndef ST_SFML_HPP
#define ST_SFML_HPP

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

    void draw(sf::RenderTarget &target,
        sf::RenderStates states) const override
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
    Button(std::string name, std::filesystem::path iconPath,
        sf::Vector2f position, sf::Vector2f size)
        : name{name}
        , texture{new sf::Texture{iconPath}}
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

}

#endif
