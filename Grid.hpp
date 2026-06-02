#ifndef GRID_HPP
#define GRID_HPP

#include "Algorithm.hpp"
#include "Node.hpp"
#include <vector>
#include <mutex>
#include <queue> // Added for priority_queue

// Custom comparator to make the priority queue a Min-Heap based on gScore
struct CompareGScore {
    bool operator()(Node* const& n1, Node* const& n2) {
        return n1->gScore > n2->gScore;
    }
};

struct CompareFScore {
    bool operator()(Node* const& n1, Node* const& n2) {
        return n1->fScore > n2->fScore;
    }
};

class Grid : public Algorithm {
private:
    int windowWidth;
    int windowHeight;
    int cellSize;
    int rows;
    int cols;

    std::vector<std::vector<Node>> nodes;
    std::mutex gridMutex;

    int startRow, startCol;
    int endRow, endCol;

    void reconstructPath(Node* endNode);
    int heuristic(int r1, int c1, int r2, int c2); 

public:
    Grid(int width, int height, int size);

    void run() override; 
    void draw(sf::RenderWindow& window) override;
    void reset() override;
    void addWall(int mouseX, int mouseY);

    // Moved these here so main.cpp can use them!
    void runDijkstra();
    void runAStar(); 
};

#endif