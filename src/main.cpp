#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "game/chessboard.h"
#include "game/chesspiece.h"

#include <array>
#include <cstdio>

template<ChessPiece::Color color>
void initPieces(std::array<std::unique_ptr<ChessPiece>, 16>& pieces);

int main()
{
    // init window
    auto window = sf::RenderWindow(sf::VideoMode(800, 600), "sfml_chess");
    window.setVerticalSyncEnabled(true);

    // init board
    auto board = ChessBoard(window);

    // init pieces (pawn)
    ChessPiece::setTileSize(board.tileWidth(), board.tileHeight());

    auto whitePieces = std::array<std::unique_ptr<ChessPiece>, 16>();
    auto blackPieces = std::array<std::unique_ptr<ChessPiece>, 16>();

    ChessPiece* movingPiece = nullptr; // non owning

    initPieces<ChessPiece::Color::White>(/*out*/ whitePieces);
    initPieces<ChessPiece::Color::Black>(/*out*/ blackPieces);

    while(window.isOpen()) {
        auto event = sf::Event();

        while (window.pollEvent(/*out*/ event)) {
            switch (event.type) {
                case sf::Event::Closed:
                {
                    window.close();
                } break;
                case sf::Event::MouseButtonPressed:
                {
                    auto const mousePosInWorldCoords = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

                    if (movingPiece) {
                        auto const closestColumn = static_cast<unsigned>(mousePosInWorldCoords.x / board.tileWidth());
                        auto const closestRow = static_cast<unsigned>(mousePosInWorldCoords.y / board.tileHeight());

                        if (movingPiece->canMoveTo(closestRow, closestColumn)) {
                            movingPiece->moveTo(closestRow, closestColumn);
                        }

                        movingPiece->endMove();
                        movingPiece = nullptr; // stop moving piece
                        break; // avoid clicking the piece we're dropping
                    }

                    for (auto& pawn : whitePieces) {
                        if (pawn && pawn->occupies(mousePosInWorldCoords.x, mousePosInWorldCoords.y)) {
                            movingPiece = pawn.get();
                            movingPiece->beginMove();
                            break;
                        }
                    }

                    for (auto& pawn : blackPieces) {
                        if (pawn && pawn->occupies(mousePosInWorldCoords.x, mousePosInWorldCoords.y)) {
                            movingPiece = pawn.get();
                            movingPiece->beginMove();
                            break;
                        }
                    }
                } break;
                case sf::Event::MouseMoved:
                {
                    if (movingPiece) {
                        // quick moving functionality to always move draggin
                        // piece to closest tile
                        auto const mousePosInWorldCoords = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});

                        auto const closestX = static_cast<unsigned>(mousePosInWorldCoords.x / board.tileWidth());
                        auto const closestY = static_cast<unsigned>(mousePosInWorldCoords.y / board.tileHeight());

                        movingPiece->drawAt(closestY, closestX);
                    }
                } break;
                default: break;
            }
        }

        // begin frame
        window.clear(sf::Color(111, 111, 111));

        window.draw(board);

        for (auto const & pawn : whitePieces) {
            if (pawn) {
                window.draw(*pawn);
            }
        }

        for (auto const & pawn : blackPieces) {
            if (pawn) {
                window.draw(*pawn);
            }
        }

        // end frame
        window.display();
    }

    return 0;
}

template<ChessPiece::Color color>
void initPieces(std::array<std::unique_ptr<ChessPiece>, 16>& pieces)
{
    static constexpr auto pawnRow = color == ChessPiece::Color::White ? 6 : 1;
    static constexpr auto kingRow = color == ChessPiece::Color::White ? 7 : 0;

    std::unique_ptr<ChessPiece> tmp2;
    unsigned index = 0;

#define CREATE_PIECE(type, row, column)     \
    tmp2 = std::make_unique<type>(color);   \
    tmp2->moveTo(row, column);              \
    pieces[index++] = std::move(tmp2)

    for (auto i = 0; i < 8; ++i) {
        CREATE_PIECE(Pawn, pawnRow, i);
    }

    CREATE_PIECE(Rook,   kingRow, 0);
    CREATE_PIECE(Knight, kingRow, 1);
    CREATE_PIECE(Bishop, kingRow, 2);

    if constexpr (color == ChessPiece::Color::White) {
        CREATE_PIECE(Queen,  kingRow, 3);
        CREATE_PIECE(King,   kingRow, 4);
    }
    else {
        CREATE_PIECE(King,   kingRow, 4);
        CREATE_PIECE(Queen,  kingRow, 3);
    }

    CREATE_PIECE(Bishop, kingRow, 5);
    CREATE_PIECE(Knight, kingRow, 6);
    CREATE_PIECE(Rook,   kingRow, 7);

#undef CREATE_PIECE
}
