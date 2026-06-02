#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <SFML/Graphics.hpp>

class Algorithm {
public:
    // Virtual destructor is essential for base classes
    virtual ~Algorithm() = default;

    // The method that will contain your actual sorting/pathfinding logic
    virtual void run() = 0;

    // The method that tells the SFML window how to draw this specific algorithm
    virtual void draw(sf::RenderWindow& window) = 0;

    // Resets the data to its initial state
    virtual void reset() = 0;
};

#endif