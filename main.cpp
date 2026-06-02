#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Sorter.hpp"
#include "Grid.hpp"
#include <thread>

enum class AppMode { SORTING, PATHFINDING };

int main() {
    int windowWidth = 800;
    int windowHeight = 600;
    
    sf::RenderWindow window(sf::VideoMode({(unsigned int)windowWidth, (unsigned int)windowHeight}), "Algorithm Visualizer");
    window.setFramerateLimit(60);

    Sorter mySorter(windowWidth, windowHeight, 100);
    Grid myGrid(windowWidth, windowHeight, 20); 

    AppMode currentMode = AppMode::SORTING;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                // Global Mode Toggles
                if (keyPressed->code == sf::Keyboard::Key::Num1) currentMode = AppMode::SORTING;
                if (keyPressed->code == sf::Keyboard::Key::Num2) currentMode = AppMode::PATHFINDING;

                // Mode Specific Keyboard Controls
                if (currentMode == AppMode::SORTING) {
                    if (keyPressed->code == sf::Keyboard::Key::B && mySorter.getState() == Sorter::State::IDLE) {
                        std::thread t(&Sorter::runBubbleSort, &mySorter); t.detach();
                    }
                    if (keyPressed->code == sf::Keyboard::Key::Q && mySorter.getState() == Sorter::State::IDLE) {
                        std::thread t(&Sorter::runQuickSort, &mySorter); t.detach();
                    }
                    if (keyPressed->code == sf::Keyboard::Key::R && mySorter.getState() != Sorter::State::SORTING) {
                        mySorter.reset();
                    }
                } 
                else if (currentMode == AppMode::PATHFINDING) {
                    if (keyPressed->code == sf::Keyboard::Key::D) {
                        std::thread t(&Grid::runDijkstra, &myGrid); 
                        t.detach();
                    }
                    if (keyPressed->code == sf::Keyboard::Key::A) {
                        std::thread t(&Grid::runAStar, &myGrid); 
                        t.detach();
                    }
                    if (keyPressed->code == sf::Keyboard::Key::R) {
                        myGrid.reset();
                    }
                }
            }
        }

        // Mouse Drag Interaction outside pollEvents
        if (currentMode == AppMode::PATHFINDING && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            myGrid.addWall(mousePos.x, mousePos.y);
        }

        window.clear(sf::Color(30, 30, 30));

        if (currentMode == AppMode::SORTING) {
            mySorter.draw(window);
        } else {
            myGrid.draw(window);
        }

        window.display();
    }

    return 0;
}