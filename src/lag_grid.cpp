#include "lag_grid.hpp"
#include <cmath>
#include <iostream>

Grid::Grid(const sf::Vector2f &center, float step, float reach,
           const sf::Color &color, std::uint8_t opacity,
           const sf::Color &colorX, const sf::Color &colorY)
    : step{step}
    , reach{reach}
    , color{sf::Color{color.r, color.g, color.b, opacity}}
    , colorX{sf::Color{colorX.r, colorX.g, colorX.b, opacity}}
    , colorY{sf::Color{colorY.r, colorY.g, colorY.b, opacity}}
    , lines{[&]()-> sf::VertexArray
        {
            sf::Vector2f min{center.x - reach, center.y - reach};
            sf::Vector2f max{center.x + reach, center.y + reach};
            min.x -= std::fmod(min.x, step);
            min.y -= std::fmod(min.y, step);
            max.x -= std::fmod(max.x, step);
            max.y -= std::fmod(max.y, step);
            std::size_t numberOfLines =
                {static_cast<std::size_t>(((max.x - min.x) / step) * 2)};
            std::size_t vertexCount{numberOfLines * 2 + 4};
            sf::VertexArray temp{sf::PrimitiveType::Lines, vertexCount};
            for(int i{0}, c{0}; i < vertexCount; i+=4, ++c)
            {
                temp[i].position   = {min.x + step * c, min.y};
                temp[i+1].position = {min.x + step * c, max.y};
                temp[i+2].position = {min.x, min.y + step * c};
                temp[i+3].position = {max.x, min.y + step * c};

                temp[i].color   = this->color;
                temp[i+1].color = this->color;
                temp[i+2].color = this->color;
                temp[i+3].color = this->color;
            }

            if(min.x < 0 && max.x > 0)
            {
                int i{static_cast<int>(std::ceil(std::abs(min.x) / step))};
                temp[i*4].color = this->colorY;
                temp[i*4+1].color = this->colorY;
            }
            if(min.y < 0 && max.y > 0)
            {
                int i{static_cast<int>(std::ceil(std::abs(min.y) / step))};
                temp[i*4+2].color = this->colorX;
                temp[i*4+3].color = this->colorX;
            }
            return temp;
        }()}
{}

void Grid::draw(sf::RenderTarget &target,
    sf::RenderStates states) const
{
    target.draw(this->lines);
}

Grid& Grid::setCenter(const sf::Vector2f &center)
{
    std::size_t vertexCount{this->lines.getVertexCount()};
    sf::Vector2f currentCenter{
        (this->lines[vertexCount-1].position - this->lines[0].position) * 0.5f};

    if(currentCenter == center)
        return *this;


    sf::Vector2f posDiff{center - currentCenter};
    for(int i{0}; i < vertexCount; ++i)
        lines[i].position += posDiff;

    return *this;
}

Grid& Grid::setColor(const sf::Color &color)
{
    if(this->color == color)
        return *this;

    std::size_t vertexCount{this->lines.getVertexCount()};
    for(int i{0}; i < vertexCount; ++i)
        lines[i].color = color;

    this->color = color;
    return *this;
}

Grid& Grid::setOpacity(std::uint8_t opacity)
{
    if(this->color.a == opacity)
        return *this;

    std::size_t vertexCount{this->lines.getVertexCount()};
    for(int i{0}; i < vertexCount; ++i)
        this->lines[i].color.a = opacity;

    this->color.a = opacity;
    return *this;
}

Grid& Grid::setStep(float step)
{
    if(this->step == step)
        return *this;

    std::size_t vertexCount{this->lines.getVertexCount()};
    sf::Vector2f center{
        (this->lines[vertexCount-1].position - this->lines[0].position) * 0.5f};

    sf::Vector2f min{center.x - this->reach, center.y - this->reach};
    sf::Vector2f max{center.x + this->reach, center.y + this->reach};
    min.x -= std::fmod(min.x, step);
    min.y -= std::fmod(min.y, step);
    max.x -= std::fmod(max.x, step);
    max.y -= std::fmod(max.y, step);
    std::size_t numberOfLines =
        {static_cast<std::size_t>(((max.x - min.x) / step) * 2)};
    std::size_t newVertexCount{numberOfLines * 2 + 4};
    sf::VertexArray newLines{sf::PrimitiveType::Lines, newVertexCount};

    for(int i{0}, c{0}; i < newVertexCount; i+=4, ++c)
    {
        newLines[i].position   = {min.x + step * c, min.y};
        newLines[i+1].position = {min.x + step * c, max.y};
        newLines[i+2].position = {min.x, min.y + step * c};
        newLines[i+3].position = {max.x, min.y + step * c};

        newLines[i].color   = this->color;
        newLines[i+1].color = this->color;
        newLines[i+2].color = this->color;
        newLines[i+3].color = this->color;
    }
    this->lines = newLines;
    this->step = step;

    return *this;
}

Grid& Grid::setReach(float reach)
{
    if(this->reach == reach)
        return *this;

    std::size_t vertexCount{this->lines.getVertexCount()};
    sf::Vector2f center{
        (this->lines[vertexCount-1].position - this->lines[0].position) * 0.5f};

    sf::Vector2f min{center.x - reach, center.y - reach};
    sf::Vector2f max{center.x + reach, center.y + reach};
    min.x -= std::fmod(min.x, this->step);
    min.y -= std::fmod(min.y, this->step);
    max.x -= std::fmod(max.x, this->step);
    max.y -= std::fmod(max.y, this->step);
    std::size_t numberOfLines =
        {static_cast<std::size_t>(((max.x - min.x) / this->step) * 2)};
    std::size_t newVertexCount{numberOfLines * 2 + 4};
    sf::VertexArray newLines{sf::PrimitiveType::Lines, newVertexCount};

    for(int i{0}, c{0}; i < newVertexCount; i+=4, ++c)
    {
        newLines[i].position   = {min.x + this->step * c, min.y};
        newLines[i+1].position = {min.x + this->step * c, max.y};
        newLines[i+2].position = {min.x, min.y + this->step * c};
        newLines[i+3].position = {max.x, min.y + this->step * c};

        newLines[i].color   = this->color;
        newLines[i+1].color = this->color;
        newLines[i+2].color = this->color;
        newLines[i+3].color = this->color;
    }
    this->lines = newLines;
    this->reach = reach;

    return *this;    
}
