#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <SFML/Graphics.hpp>

#include <array>

class ChessPiece : public sf::Drawable
{
public:
    enum class Color
    {
        White,
        Black
    };

    enum class Type
    {
        Pawn,
        Knight,
        Bishop,
        Rook,
        Queen,
        King
    };

    ChessPiece(Color color);
    ~ChessPiece() = default;

    void moveTo(unsigned rowOnBoard, unsigned columnOnBoard);
    bool occupies(int x, int y) const noexcept;

    Color color() const noexcept;
    virtual Type type() const noexcept = 0;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    static void setTileSize(float tileWidth, float tileHeight);
    static void setScale(sf::Vector2f const & scale);

protected:
    sf::Sprite& sprite();
    sf::Sprite& sprite() const;

private:
    static void initialize();

    template<Color C, Type T> 
    static void setTextureAndSprite(std::string const & fileName);

    template<Type T> 
    static void setTextureAndSpriteImpl(std::array<sf::Texture, 8>& textures,
                                        std::array<sf::Sprite, 8>& sprites,
                                        std::string const & fileName);

private:
    inline static auto initialized_ = false;;

    inline static std::array<sf::Texture, 8> whiteTextures_;
    inline static std::array<sf::Texture, 8> blackTextures_;

    inline static std::array<sf::Sprite, 8> whiteSprites_;
    inline static std::array<sf::Sprite, 8> blackSprites_;

    inline static auto scale_       = sf::Vector2f(1, 1);
    inline static auto tileWidth_   = 0.0f;
    inline static auto tileHeight_  = 0.0f;

private:
    Color color_;
    unsigned rowOnBoard_    = 0;
    unsigned columnOnBoard_ = 0;
};

class Pawn : public ChessPiece
{
public:
    Pawn(Color c) : ChessPiece(c) {}
    ~Pawn() = default;

    Type type() const noexcept override {return Type::Pawn;}
};

#endif // CHESSPIECE_H
