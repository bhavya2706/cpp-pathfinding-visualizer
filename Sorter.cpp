#include "Sorter.hpp"
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

Sorter::Sorter(int width, int height, int numElements) 
    : windowWidth(width), windowHeight(height), arraySize(numElements), currentState(State::IDLE)
{

    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Calculate how wide each bar should be based on the screen width
    float barWidth = static_cast<float>(windowWidth) / numElements;

    for (int i = 0; i < numElements; i++)
    {
        // Generate a random height (value) for the array element
        int value = std::rand() % (windowHeight - 50) + 20;
        data.push_back(value);

        // Create the visual rectangle for SFML 3
        sf::RectangleShape bar(sf::Vector2f({barWidth - 1.0f, static_cast<float>(value)}));

        // Position it at the bottom of the screen
        bar.setPosition(sf::Vector2f({i * barWidth, static_cast<float>(windowHeight - value)}));
        bar.setFillColor(sf::Color::White);

        rectangles.push_back(bar);
    }
}

void Sorter::draw(sf::RenderWindow &window)
{
    // Lock the mutex so we don't draw while a sorting thread is modifying the array
    std::lock_guard<std::mutex> lock(dataMutex);

    for (const auto &rect : rectangles)
    {
        window.draw(rect);
    }
}

void Sorter::swap(int i, int j)
{
    // 1. Lock the mutex! This stops the main thread from drawing while we move data
    std::lock_guard<std::mutex> lock(dataMutex);

    // 2. Swap the actual integer data
    std::swap(data[i], data[j]);

    // 3. Update the visual rectangles to match the new data
    float heightI = static_cast<float>(data[i]);
    float heightJ = static_cast<float>(data[j]);

    rectangles[i].setSize(sf::Vector2f({rectangles[i].getSize().x, heightI}));
    rectangles[i].setPosition(sf::Vector2f({rectangles[i].getPosition().x, windowHeight - heightI}));

    rectangles[j].setSize(sf::Vector2f({rectangles[j].getSize().x, heightJ}));
    rectangles[j].setPosition(sf::Vector2f({rectangles[j].getPosition().x, windowHeight - heightJ}));

    // 4. Sleep this background thread for 5 milliseconds to create the animation effect
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

// Change the name of your old run() method:
void Sorter::runBubbleSort() {
    currentState = State::SORTING;
    
    int n = data.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (data[j] > data[j + 1]) {
                swap(j, j + 1); 
            }
        }
    }
    
    currentState = State::FINISHED; 
}

// Add these three new functions for Quick Sort:
void Sorter::runQuickSort() {
    currentState = State::SORTING;
    quickSortHelper(0, data.size() - 1);
    currentState = State::FINISHED;
}

void Sorter::quickSortHelper(int low, int high) {
    if (low < high) {
        int pi = partition(low, high);
        quickSortHelper(low, pi - 1);
        quickSortHelper(pi + 1, high);
    }
}

int Sorter::partition(int low, int high) {
    int pivot = data[high]; // Choosing the last element as pivot
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        // If current element is smaller than the pivot
        if (data[j] < pivot) {
            i++;
            swap(i, j); // This triggers our animation!
        }
    }
    swap(i + 1, high);
    return (i + 1);
}
void Sorter::reset() {
    // Lock the mutex so we don't clear the data while drawing
    std::lock_guard<std::mutex> lock(dataMutex);
    
    data.clear();
    rectangles.clear();

    float barWidth = static_cast<float>(windowWidth) / arraySize;

    for (int i = 0; i < arraySize; i++) {
        int value = std::rand() % (windowHeight - 50) + 20;
        data.push_back(value);

        sf::RectangleShape bar(sf::Vector2f({barWidth - 1.0f, static_cast<float>(value)}));
        bar.setPosition(sf::Vector2f({i * barWidth, static_cast<float>(windowHeight - value)}));
        bar.setFillColor(sf::Color::White);
        
        rectangles.push_back(bar);
    }
    
    currentState = State::IDLE; // Reset the state so we can sort again
}