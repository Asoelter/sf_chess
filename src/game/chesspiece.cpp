#include "chesspiece.h"

#include <cstdio>

ChessPiece::ChessPiece(Color color)
    : color_(color)
{

}

void ChessPiece::moveTo(unsigned rowOnBoard, unsigned columnOnBoard)
{
    rowOnBoard_ = rowOnBoard;
    columnOnBoard_ = columnOnBoard;
}

bool ChessPiece::occupies(int x, int y) const noexcept
{
#if 0
    auto const bounds = sprite().getLocalBounds();

    printf("x: %i left: %f right  %f\n", x, bounds.left, bounds.left + bounds.width);
    printf("y: %i top : %f bottom %f\n", y, bounds.top, bounds.top - bounds.height);

    auto const occupiesX = (x > bounds.left && x < bounds.left + bounds.width);
    auto const occupiesY = (y < bounds.top && y > bounds.top - bounds.height);

    if (occupiesX || occupiesY) {
        printf("occupies: ");
        if (occupiesX) {
            printf("x");
        }
        if(occupiesY) {
            printf(" y");
        }
        printf("\n");
    }
#endif
    auto const occupiesX = (x > (columnOnBoard_ * tileWidth_) && x < ((columnOnBoard_ + 1) * tileWidth_));
    auto const occupiesY = (y > (rowOnBoard_ * tileHeight_)   && y < ((rowOnBoard_ + 1) * tileHeight_));

    if (occupiesX && occupiesY) {
        return true;
    }

    return false;
}

ChessPiece::Color ChessPiece::color() const noexcept
{
    return color_;
}

void ChessPiece::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // since the same sprite is shared
    // across multiple objects we
    // must set the sprite's state every
    // time we draw
    auto& s = sprite();

    //s.setScale(scale_);

    auto const bounds = s.getGlobalBounds();

    s.scale({tileWidth_ / bounds.width, tileHeight_ / bounds.height});

    // this assumes that all pieces have the
    // same dimensions as a tile
    s.setPosition(columnOnBoard_ * bounds.width, rowOnBoard_ * bounds.height);

    target.draw(s, states);
}

/*static*/ void ChessPiece::setTileSize(float tileWidth, float tileHeight)
{
    tileWidth_ = tileWidth;
    tileHeight_ = tileHeight;
}

/*static*/ void ChessPiece::setScale(sf::Vector2f const & scale)
{
    scale_ = scale;
}

/*static*/ void ChessPiece::initialize()
{
    setTextureAndSprite<Color::White, Type::Pawn>  ("../src/res/w_pawn_1x_ns.png");
    setTextureAndSprite<Color::White, Type::Knight>("../src/res/w_knight_1x_ns.png");
    setTextureAndSprite<Color::White, Type::Bishop>("../src/res/w_bishop_1x_ns.png");
    setTextureAndSprite<Color::White, Type::Rook>  ("../src/res/w_rook_1x_ns.png");
    setTextureAndSprite<Color::White, Type::Queen> ("../src/res/w_queen_1x_ns.png");
    setTextureAndSprite<Color::White, Type::King>  ("../src/res/w_king_1x_ns.png");

    setTextureAndSprite<Color::Black, Type::Pawn>  ("../src/res/b_pawn_1x_ns.png");
    setTextureAndSprite<Color::Black, Type::Knight>("../src/res/b_knight_1x_ns.png");
    setTextureAndSprite<Color::Black, Type::Bishop>("../src/res/b_bishop_1x_ns.png");
    setTextureAndSprite<Color::Black, Type::Rook>  ("../src/res/b_rook_1x_ns.png");
    setTextureAndSprite<Color::Black, Type::Queen> ("../src/res/b_queen_1x_ns.png");
    //setTextureAndSprite<Color::Black, Type::King>  ("../src/res/b_king_1x_ns.png");
}

sf::Sprite& ChessPiece::sprite()
{
    if (!initialized_) {
        initialize();
        initialized_ = true;
    }

    auto const t = type();

    if (color() == Color::White) {
        return whiteSprites_[static_cast<size_t>(t)];
    }

    return blackSprites_[static_cast<size_t>(t)];
}

sf::Sprite& ChessPiece::sprite() const
{
    if (!initialized_) {
        initialize();
        initialized_ = true;
    }

    auto const t = type();

    if (color() == Color::White) {
        return whiteSprites_[static_cast<size_t>(t)];
    }

    return blackSprites_[static_cast<size_t>(t)];
}

template<ChessPiece::Color C, ChessPiece::Type T> 
/*static*/ void ChessPiece::setTextureAndSprite(std::string const & fileName)
{
    // catch future new enum values
    static_assert(C == Color::White || C == Color::Black, "unhandled color");

    if constexpr(C == Color::White) {
        setTextureAndSpriteImpl<T>(whiteTextures_, whiteSprites_, fileName);
    }

    setTextureAndSpriteImpl<T>(blackTextures_, blackSprites_, fileName);
}

template<ChessPiece::Type T> 
/*static*/ void ChessPiece::setTextureAndSpriteImpl(std::array<sf::Texture, 8>& textures, 
                                                    std::array<sf::Sprite, 8>& sprites,
                                                    std::string const & fileName)
{
    static constexpr auto index = static_cast<size_t>(T);
    auto const result = textures[index].loadFromFile(fileName);
    
    sprites[index].setTexture(textures[index]);

    assert(result);
}
