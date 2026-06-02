#ifndef SORTER_HPP
#define SORTER_HPP

#include "Algorithm.hpp"
#include <vector>
#include <mutex>

class Sorter : public Algorithm {
public:
    enum class State { IDLE, SORTING, FINISHED };

private:
    std::vector<int> data;                      
    std::vector<sf::RectangleShape> rectangles; 
    std::mutex dataMutex;                       
    
    int windowWidth;
    int windowHeight;
    int arraySize;
    State currentState; 

    void swap(int i, int j);

    // Quick Sort helper functions
    void quickSortHelper(int low, int high);
    int partition(int low, int high);

public:
    Sorter(int width, int height, int numElements);
    
    // Replace the old run() method with these specific ones
    void runBubbleSort(); 
    void runQuickSort();
    
    // We must still declare run() because it is a pure virtual function in Algorithm.hpp
    void run() override { runBubbleSort(); } // Default fallback

    void draw(sf::RenderWindow& window) override;
    void reset() override;

    State getState() const { return currentState; }
};

#endif