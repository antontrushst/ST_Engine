#ifndef LAG_GRID_HPP
#define LAG_GRID_HPP

#include <SFML/Graphics.hpp>

class Grid : public sf::Drawable
{
    float step;
    float reach;
    sf::Color color;
    sf::VertexArray lines;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    Grid(const sf::Vector2f &center, float step = 10.f, float reach = 1000.f,
        const sf::Color &color = sf::Color::White, std::uint8_t opacity = 255);

    Grid& setCenter(const sf::Vector2f &center);
    Grid& setColor(const sf::Color &color);
    Grid& setOpacity(std::uint8_t opacity);
    Grid& setStep(float step);
    Grid& setReach(float reach);

    float getStep() const {return this->step;}
    float getReach() const {return this->reach;}
    sf::Color getColor() const {return this->color;}
    std::uint8_t getOpacity() const {return this->color.a;}
};

#endif
