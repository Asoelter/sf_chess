#include "chessboard.h"

ChessBoard::ChessBoard(sf::Window const & window)
    : tileWidth_ (static_cast<float>(window.getSize().x / static_cast<float>(rows)))
    , tileHeight_(static_cast<float>(window.getSize().y / static_cast<float>(columns)))
    , tile_({tileWidth_, tileHeight_})
{

}

void ChessBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    auto useWhiteColor = false;

    // draw board
    for (unsigned y = 0; y < 8; ++y) {
        useWhiteColor = !useWhiteColor; // negate at start of loop to stagger

        for (unsigned x = 0; x < 8; ++x) {
            tile_.setPosition({x * tileWidth(), y * tileHeight()});

            if (useWhiteColor) {
                tile_.setFillColor(sf::Color::Black);
            }
            else {
                tile_.setFillColor(sf::Color::Red);
            }

            target.draw(tile_);
            useWhiteColor = !useWhiteColor;
        }
    }

    tile_.setPosition({0, 0});
}

float ChessBoard::tileWidth() const noexcept
{
    return tileWidth_;
}

float ChessBoard::tileHeight() const noexcept
{
    return tileHeight_;
}
