#include "Grid.hpp"
#include <mutex>

Grid::Grid(int width, int height, int size) 
    : windowWidth(width), windowHeight(height), cellSize(size) {
    
    rows = windowHeight / cellSize;
    cols = windowWidth / cellSize;

    nodes.resize(rows, std::vector<Node>(cols));

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            nodes[r][c].row = r;
            nodes[r][c].col = c;
            nodes[r][c].state = NodeState::EMPTY;

            // SFML 3 expects size inside braces {}
            nodes[r][c].shape.setSize(sf::Vector2f({static_cast<float>(cellSize - 1), static_cast<float>(cellSize - 1)}));
            nodes[r][c].shape.setPosition(sf::Vector2f({static_cast<float>(c * cellSize), static_cast<float>(r * cellSize)}));
            nodes[r][c].shape.setFillColor(sf::Color(45, 45, 45)); // Dark grid tiles
        }
    }

    // Set Default Start and End Nodes
    startRow = rows / 2;   startCol = cols / 4;
    endRow = rows / 2;     endCol = (cols * 3) / 4;

    nodes[startRow][startCol].state = NodeState::START;
    nodes[startRow][startCol].shape.setFillColor(sf::Color::Green);

    nodes[endRow][endCol].state = NodeState::END;
    nodes[endRow][endCol].shape.setFillColor(sf::Color::Red);
}

void Grid::addWall(int mouseX, int mouseY) {
    std::lock_guard<std::mutex> lock(gridMutex);

    // Map pixel coordinates to Row and Column indices
    int c = mouseX / cellSize;
    int r = mouseY / cellSize;

    // Bounds checking
    if (r >= 0 && r < rows && c >= 0 && c < cols) {
        // Protect Start and End positions from being overwritten
        if (nodes[r][c].state == NodeState::EMPTY) {
            nodes[r][c].state = NodeState::WALL;
            nodes[r][c].shape.setFillColor(sf::Color::Black); // Wall color
        }
    }
}

void Grid::draw(sf::RenderWindow& window) {
    std::lock_guard<std::mutex> lock(gridMutex);
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            window.draw(nodes[r][c].shape);
        }
    }
}



void Grid::reset() {
    std::lock_guard<std::mutex> lock(gridMutex);
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (nodes[r][c].state != NodeState::START && nodes[r][c].state != NodeState::END) {
                nodes[r][c].state = NodeState::EMPTY;
                nodes[r][c].shape.setFillColor(sf::Color(45, 45, 45));
                nodes[r][c].parent = nullptr;
                nodes[r][c].gScore = 1e9;
                nodes[r][c].fScore = 1e9;
            }
        }
    }
}

#include <queue>
#include <thread>
#include <chrono>

void Grid::run() {
    runDijkstra();
}

void Grid::runDijkstra() {
    std::priority_queue<Node*, std::vector<Node*>, CompareGScore> pq;

    // Initialize Start Node
    nodes[startRow][startCol].gScore = 0;
    pq.push(&nodes[startRow][startCol]);

    // Direction vectors: Up, Right, Down, Left
    int dr[] = {-1, 0, 1, 0};
    int dc[] = {0, 1, 0, -1};

    while (!pq.empty()) {
        Node* current = pq.top();
        pq.pop();

        // If we reached the target, trigger the path drawing animation
        if (current->state == NodeState::END) {
            reconstructPath(current);
            return; 
        }

        // Color the current node Cyan (Visited)
        if (current->state != NodeState::START) {
            std::lock_guard<std::mutex> lock(gridMutex);
            current->state = NodeState::VISITED;
            current->shape.setFillColor(sf::Color::Cyan); 
        }

        // The Animation Delay
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        // Check all 4 neighbors
        for (int i = 0; i < 4; ++i) {
            int nr = current->row + dr[i];
            int nc = current->col + dc[i];

            // Bounds checking
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                Node* neighbor = &nodes[nr][nc];

                // Ignore walls and already visited nodes
                if (neighbor->state != NodeState::WALL && neighbor->state != NodeState::VISITED) {
                    
                    // In a simple grid, distance between adjacent nodes is always 1
                    int tentativeGScore = current->gScore + 1;

                    if (tentativeGScore < neighbor->gScore) {
                        neighbor->parent = current;
                        neighbor->gScore = tentativeGScore;
                        pq.push(neighbor);

                        // Color the neighbor Yellow (Visiting / In Queue)
                        if (neighbor->state != NodeState::END && neighbor->state != NodeState::START) {
                            std::lock_guard<std::mutex> lock(gridMutex);
                            neighbor->state = NodeState::VISITING;
                            neighbor->shape.setFillColor(sf::Color::Yellow);
                        }
                    }
                }
            }
        }
    }
}

void Grid::reconstructPath(Node* endNode) {
    Node* current = endNode->parent;
    
    // Trace back the parent pointers until we hit the Start node
    while (current != nullptr && current->state != NodeState::START) {
        {
            std::lock_guard<std::mutex> lock(gridMutex);
            current->state = NodeState::PATH;
            current->shape.setFillColor(sf::Color::Magenta); // Draw final path in Magenta
        }
        current = current->parent;
        
        // Slower delay for a dramatic path reveal
        std::this_thread::sleep_for(std::chrono::milliseconds(20)); 
    }
}

#include <cmath> // Needed for std::abs

int Grid::heuristic(int r1, int c1, int r2, int c2) {
    // Manhattan distance: perfect for grid movement where you can't move diagonally
    return std::abs(r1 - r2) + std::abs(c1 - c2);
}

void Grid::runAStar() {
    std::priority_queue<Node*, std::vector<Node*>, CompareFScore> pq;

    nodes[startRow][startCol].gScore = 0;
    nodes[startRow][startCol].fScore = heuristic(startRow, startCol, endRow, endCol);
    pq.push(&nodes[startRow][startCol]);

    int dr[] = {-1, 0, 1, 0};
    int dc[] = {0, 1, 0, -1};

    while (!pq.empty()) {
        Node* current = pq.top();
        pq.pop();

        if (current->state == NodeState::END) {
            reconstructPath(current);
            return; 
        }

        if (current->state != NodeState::START) {
            std::lock_guard<std::mutex> lock(gridMutex);
            current->state = NodeState::VISITED;
            current->shape.setFillColor(sf::Color::Cyan); 
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        for (int i = 0; i < 4; ++i) {
            int nr = current->row + dr[i];
            int nc = current->col + dc[i];

            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                Node* neighbor = &nodes[nr][nc];

                if (neighbor->state != NodeState::WALL && neighbor->state != NodeState::VISITED) {
                    
                    int tentativeGScore = current->gScore + 1;

                    if (tentativeGScore < neighbor->gScore) {
                        neighbor->parent = current;
                        neighbor->gScore = tentativeGScore;
                        
                        // A* Magic: The fScore calculates total estimated path length
                        neighbor->fScore = tentativeGScore + heuristic(nr, nc, endRow, endCol);
                        pq.push(neighbor);

                        if (neighbor->state != NodeState::END && neighbor->state != NodeState::START) {
                            std::lock_guard<std::mutex> lock(gridMutex);
                            neighbor->state = NodeState::VISITING;
                            // Let's color A*'s frontier Orange to distinguish it from Dijkstra's Yellow
                            neighbor->shape.setFillColor(sf::Color(255, 165, 0)); 
                        }
                    }
                }
            }
        }
    }
}