#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "game/chesspiece.h"

#include <array>
#include <cstdio>

int main()
{
    // init window
    auto window = sf::RenderWindow(sf::VideoMode(800, 600), "sfml_chess");
    window.setVerticalSyncEnabled(true);

    // init board
    auto sfmlTileWidth = static_cast<float>(window.getSize().x / 8.0f);  // sfml appears to have an internal
    auto sfmlTileHeight = static_cast<float>(window.getSize().y / 8.0f); // representation of sizes that aren't
    auto realTileWidth = sfmlTileWidth;                                  // affected by window resizes. So we have
    auto realTileHeight = sfmlTileHeight;                                // to track sfml and real dimensions

    auto square = sf::RectangleShape({sfmlTileWidth, sfmlTileHeight});

    // init pieces (pawn)
    auto pawnPositions = std::array<sf::Vector2u, 8>();
    pawnPositions[0] = {0, 1};
    pawnPositions[1] = {1, 1};
    pawnPositions[2] = {2, 1};
    pawnPositions[3] = {3, 1};
    pawnPositions[4] = {4, 1};
    pawnPositions[5] = {5, 1};
    pawnPositions[6] = {6, 1};
    pawnPositions[7] = {7, 1};
    auto pawnTexture = sf::Texture();;
    pawnTexture.loadFromFile("../src/res/b_pawn_1x_ns.png");

    auto pawn = sf::Sprite(pawnTexture);

    auto const pawnScaleX = sfmlTileWidth  / pawnTexture.getSize().x;
    auto const pawnScaleY = sfmlTileHeight / pawnTexture.getSize().y;

    ChessPiece::setTileSize(sfmlTileWidth, sfmlTileHeight);

    auto pawns = std::array<Pawn, 8>{Pawn::Color::White, Pawn::Color::White, Pawn::Color::White, Pawn::Color::White, Pawn::Color::White, Pawn::Color::White, Pawn::Color::White, Pawn::Color::White};
    pawns[0].moveTo(6, 0);
    pawns[1].moveTo(6, 1);
    pawns[2].moveTo(6, 2);
    pawns[3].moveTo(6, 3);
    pawns[4].moveTo(6, 4);
    pawns[5].moveTo(6, 5);
    pawns[6].moveTo(6, 6);
    pawns[7].moveTo(6, 7);

    pawn.scale({pawnScaleX, pawnScaleY});

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
                    for (auto& pos : pawnPositions) {
                        auto const mouseX = event.mouseButton.x;
                        auto const mouseY = event.mouseButton.y;
                        auto const inXRange = (mouseX > (pos.x * realTileWidth)) && (mouseX < ((pos.x + 1) * realTileWidth));
                        auto const inYRange = (mouseY > (pos.y * realTileHeight)) && (mouseY < ((pos.y + 1) * realTileHeight));

                        if (inXRange && inYRange) {
                            printf("Pawn was hit!\n");
                        }
                    }

                    for (auto& pawn : pawns) {
                        if (pawn.occupies(event.mouseButton.x, event.mouseButton.y)) {
                            printf("Pawn was hit (cool mode)\n");
                        }
                    }
                } break;
                case sf::Event::Resized:
                {
                    realTileWidth = static_cast<float>(window.getSize().x / 8.0f);
                    realTileHeight = static_cast<float>(window.getSize().y / 8.0f);
                } break;
                default: break;
            }
        }

        // begin frame
        window.clear(sf::Color(111, 111, 111));

        auto usePrimaryColor = false;

        // draw board
        for (unsigned y = 0; y < 8; ++y) {
            usePrimaryColor = !usePrimaryColor; // negate at start of loop to stagger

            for (unsigned x = 0; x < 8; ++x) {
                square.setPosition({x * sfmlTileWidth, y * sfmlTileHeight});

                if (usePrimaryColor) {
                    square.setFillColor(sf::Color::Black);
                }
                else {
                    square.setFillColor(sf::Color::Red);
                }

                window.draw(square);
                usePrimaryColor = !usePrimaryColor;
            }
        }

        square.setPosition({0, 0});

        // draw pieces (pawns)
        for (auto const & position : pawnPositions) {
            pawn.setPosition({position.x * sfmlTileWidth, position.y * sfmlTileHeight});
            window.draw(pawn);
        }

        for (auto const & pawn : pawns) {
            window.draw(pawn);
        }

        // end frame
        window.display();
    }

    return 0;
}
