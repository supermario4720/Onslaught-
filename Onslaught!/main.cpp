#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "TitleScreen.hpp"
#include "InputManager.hpp"
#include "GameStateManager.hpp"
#include "AudioManager.hpp"

#include "GameUI.hpp"
#include "PauseUI.hpp"
#include "HowToPlayUI.hpp"
#include "GameOverUI.hpp"
#include "VictoryUI.hpp"
//#include "ExpandedInventory.hpp"
//#include "CompactInventory.hpp"

#include "UI.hpp"

#include "Camera.hpp"


#include "Player.hpp"
#include "Enemy.hpp"
#include "Object.hpp"
#include "CollisionManager.hpp"
#include "EntityManager.hpp"
#include "ButtonTextureLoader.hpp"
#include "ItemTextures.hpp"
#include "BuildingTextures.hpp"

#include <vector>
#include <iostream>



int main() {
    AudioManager::getInstance().loadAllSounds();
    ButtonTextureLoader::getInstance().loadAllTextures();
    ItemTextures::getInstance().loadAllTextures();
    BuildingTextures::getInstance().loadAllTextures();
    // play noise at start so SFML loads sound properly
    AudioManager::getInstance().play("ButtonClick", 0.f);

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Game", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    //window.setMinimumSize(sf::Vector2u{ 400, 300 });
    //window.

    sf::Clock clock;

    sf::Font font;
    if (!font.openFromFile("resources/arial.ttf")) {
    }

    GameStateManager& gsManager = GameStateManager::getInstance();

    TitleScreen titleScreen(font, window);
    GameUI gameScreen(font, window);
    HowToPlayUI manual(font, window);
    PauseUI pauseScreen(font, window);
    GameOverUI gameOverScreen(font, window);
    VictoryUI winScreen(font, window);

    //InventoryUIExpanded expandedInv;
    //InventoryUICompact compactInv;
    // used to display manual on first play
    int firstPlay = 0;


    sf::Texture grassTexture("resources/grassTileTexture50.png");
    grassTexture.setRepeated(true);
    sf::Sprite background(grassTexture);
    background.setTextureRect(sf::IntRect({ 0, 0 }, { 800,600 }));

    Camera camera(window, 800.f, 600.f);
    EntityManager& entityManager = EntityManager::getInstance();
    entityManager.start(&camera);
    bool entitiesInitialized = false;

    CollisionManager& collisionManager = CollisionManager::getInstance();

    InputManager& inputManager = InputManager::getInstance();
    inputManager.setCamera(camera);

    // mark 0f, 0,f (center) of world
    sf::RectangleShape center;
    center.setSize({ 10.f, 10.f });
    center.setOrigin({ 5.f, 5.f });
    center.setFillColor(sf::Color::White);
    center.setPosition({ 0.f, 0.f });

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float dt = clock.restart().asSeconds();

        window.clear(sf::Color::Black);

        inputManager.update(window);

        GameStateManager::State gameState = gsManager.getState();

        switch (gameState)
        {
        case GameStateManager::State::Title:
            {
                titleScreen.update(window);
                window.draw(background);
                titleScreen.render(window);
                entitiesInitialized = false;
                break;
            }
            case GameStateManager::State::Playing:
            {
                // initialize (restart) entityManager on first frame of game
                if (!entitiesInitialized) {
                    entityManager.start(&camera);
                    entitiesInitialized = true;
                }
                // after first frame is loaded, show manual
                if (firstPlay == 1) {
                    // render surroundings in background
                    window.clear(sf::Color(10, 10, 10, 255));
                    window.draw(background);
                    window.setView(camera.getView());
                    entityManager.renderAlive(window);
                    window.setView(window.getDefaultView());
                    gameScreen.render(window);
                    sf::RectangleShape fadeOverlay(sf::Vector2f(window.getSize()));
                    fadeOverlay.setFillColor(sf::Color(0, 0, 0, 150)); // 150 = semi-transparent black
                    window.draw(fadeOverlay);
                    // check if close button is clicked
                    if (manual.update(window)) {
                        firstPlay = 2;
                    }
                    manual.render(window);
                    break;
                }

                // update game state
                entityManager.update(dt, window);
                collisionManager.update(dt);
                gameScreen.update(dt, window);
                camera.follow(entityManager.getPlayerPos());

                window.clear(sf::Color(10,10,10,255));
                window.draw(background);
                // adjusting background so it looks still
                const sf::Vector2u windowSize = window.getSize();
                background.setTextureRect(sf::IntRect(
                    { static_cast<int>(camera.getPos().x),
                      static_cast<int>(camera.getPos().y) },
                    { static_cast<int>(windowSize.x),
                     static_cast<int>(windowSize.y) }
                    )
                );
                background.setPosition({ 0.f, 0.f });

                // render player and surroundings
                window.setView(camera.getView());
                entityManager.renderAlive(window);
                ////window.draw(center);

                // render game UI
                window.setView(window.getDefaultView());
                gameScreen.render(window);
                //compactInv.Draw(window);

                // check for game over
                if (!entityManager.isPlayerAlive() || !entityManager.isTownAlive()) gameState = GameStateManager::State::GameOver;
                if (firstPlay == 0 ) firstPlay = 1;
                break;
            }
            case GameStateManager::State::Pause:
            {
                window.clear(sf::Color(10,10,10,255));
                window.draw(background);
                // render player and surroundings
                window.setView(camera.getView());
                entityManager.renderAlive(window);
                //window.draw(center);

                window.setView(window.getDefaultView());
                gameScreen.render(window);
                // transparent sheet over game objects
                sf::RectangleShape fadeOverlay(sf::Vector2f(window.getSize()));
                fadeOverlay.setFillColor(sf::Color(0, 0, 0, 150)); // 150 = semi-transparent black
                window.draw(fadeOverlay);

                pauseScreen.update(dt, window);
                pauseScreen.render(window);

                break;
            }
            case GameStateManager::State::GameOver:
            {
                // set to false so entities reset next play
                // dont reset yet, so entities are still drawn in background
                if(entitiesInitialized) entitiesInitialized = false;

                window.clear(sf::Color(10, 10, 10, 255));
                window.draw(background);
                // render player and surroundings
                window.setView(camera.getView());
                entityManager.renderAlive(window);
                //window.draw(center);

                window.setView(window.getDefaultView());
                gameScreen.render(window);
                // transparent sheet over game objects
                sf::RectangleShape fadeOverlay(sf::Vector2f(window.getSize()));
                fadeOverlay.setFillColor(sf::Color(0, 0, 0, 150)); // 150 = semi-transparent black
                window.draw(fadeOverlay);

                gameOverScreen.update(dt, window);
                gameOverScreen.render(window);

                break;
            }
            case GameStateManager::State::Victory:
            {
                entitiesInitialized = false;
                window.clear(sf::Color(10, 10, 10, 255));
                window.draw(background);
                // render player and surroundings
                window.setView(camera.getView());
                entityManager.renderAlive(window);
                //window.draw(center);

                window.setView(window.getDefaultView());
                gameScreen.render(window);
                // transparent sheet over game objects
                sf::RectangleShape fadeOverlay(sf::Vector2f(window.getSize()));
                fadeOverlay.setFillColor(sf::Color(0, 0, 0, 150)); // 150 = semi-transparent black
                window.draw(fadeOverlay);

                winScreen.update(dt, window);
                winScreen.render(window);

                break;
            }
            case GameStateManager::State::Exit:
            {
                entitiesInitialized = false;
                entityManager.reset();
                window.close();
                break;
            }
        }

        window.display();
    }
    AudioManager::getInstance().clearManager();
    ButtonTextureLoader::getInstance().clearManager();
    ItemTextures::getInstance().clearManager();
    BuildingTextures::getInstance().clearManager();
    entityManager.clear();
    inputManager.clear();
    return 0;
}
