#ifndef NODE_HPP
#define NODE_HPP

#include <SFML/Graphics.hpp>

enum class NodeState { EMPTY, WALL, START, END, VISITING, VISITED, PATH };

struct Node {
    int row;
    int col;
    NodeState state = NodeState::EMPTY;
    sf::RectangleShape shape;

    // For Pathfinding (Dijkstra/A*)
    int gScore = 1e9; 
    int fScore = 1e9; // Add this for A* Search
    Node* parent = nullptr; 
};

#endif