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

    void beginMove();
    void endMove();
    void moveTo(unsigned rowOnBoard, unsigned columnOnBoard);
    void drawAt(unsigned rowOnBoard, unsigned columnOnBoard);
    bool occupies(int x, int y) const noexcept;
    unsigned rowOnBoard() const noexcept;
    unsigned columnOnBoard() const noexcept;
    virtual bool canMoveTo(unsigned newRow, unsigned newColumn) = 0;

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
    static void setTextureAndSpriteImpl(std::array<sf::Texture, 6>& textures,
                                        std::array<sf::Sprite, 6>& sprites,
                                        std::string const & fileName);

private:
    inline static auto initialized_ = false;;

    inline static std::array<sf::Texture, 6> whiteTextures_;
    inline static std::array<sf::Texture, 6> blackTextures_;

    inline static std::array<sf::Sprite, 6> whiteSprites_;
    inline static std::array<sf::Sprite, 6> blackSprites_;

    inline static auto scale_       = sf::Vector2f(1, 1);
    inline static auto tileWidth_   = 0.0f;
    inline static auto tileHeight_  = 0.0f;

private:
    Color    color_;
    unsigned rowToDrawAt_       = 0; // we keep drawing positions and
    unsigned columnToDrawAt_    = 0; // actual positions because during
    unsigned rowOnBoard_        = 0; // moves we want to draw pieces
    unsigned columnOnBoard_     = 0; // at locations they don't occupy
    bool     isMoving_          = false;
};

#define DECLARE_PIECE(PieceType)                                        \
    class PieceType : public ChessPiece                                 \
    {                                                                   \
    public:                                                             \
        PieceType(Color c) : ChessPiece(c) {}                           \
        ~PieceType() = default;                                         \
                                                                        \
        bool canMoveTo(unsigned newRow, unsigned newColumn) override;   \
                                                                        \
        Type type() const noexcept override {return Type::PieceType;}   \
    }

DECLARE_PIECE(Pawn);
DECLARE_PIECE(Knight);
DECLARE_PIECE(Bishop);
DECLARE_PIECE(Rook);
DECLARE_PIECE(Queen);
DECLARE_PIECE(King);

#endif // CHESSPIECE_H
