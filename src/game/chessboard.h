#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class ChessBoard : public sf::Drawable
{
public:
    static constexpr auto rows      = 8;
    static constexpr auto columns   = 8;

    ChessBoard(sf::Window const & window);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    float tileWidth() const noexcept;
    float tileHeight() const noexcept;

private:
    sf::Color whiteColor_; // color of "white" tiles. Probably not the actual color white
    sf::Color blackColor_; // color of "black" tiles. Probably not the actual color black
    float     tileWidth_;
    float     tileHeight_;

    // the tile is used for drawing the board. The draw method is const so
    // we would either need to make the tile funciton-local to the draw
    // method, or allocate it here and make it mutable to avoid having
    // to allocate it every draw call
    mutable sf::RectangleShape tile_;
};

#endif // CHESSBOARD_H
