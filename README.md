# 🚀 Algorithm Visualizer

A multi-threaded, interactive visualization tool built with C++ and SFML 3. This application graphically renders the real-time execution of complex sorting and pathfinding algorithms, providing an intuitive way to understand data structures and algorithmic efficiency.

> **Visual Demo**
> 
> ![Quick Sort Demo]([Link to your Quick Sort GIF])
> *Quick Sort dividing and conquering a randomized array.*
> 
> ![A* Search Demo]([Link to your A* GIF])
> *A* Search algorithm using a Manhattan distance heuristic to navigate a custom-drawn maze.*

## ⚙️ Core Engineering Features

* **Multi-Threading:** Engineered a robust background-worker architecture using `std::thread` and `std::mutex`. This strictly separates the UI rendering loop from the algorithmic processing, guaranteeing smooth 60 FPS visual performance without frame drops or UI freezing.
* **Object-Oriented Architecture:** Applied Polymorphism and Inheritance to create modular base classes (`Algorithm`). This ensures a scalable state machine where algorithms can be hot-swapped without memory leaks or crashes.
* **Interactive 2D Grid:** Features a dynamic pathfinding environment where users can draw custom wall geometries in real-time using mouse coordinate mapping.

## 🧮 Implemented Algorithms

**Sorting (1D Array)**
* **Bubble Sort:** Demonstrates standard $O(N^2)$ execution.
* **Quick Sort:** Demonstrates $O(N \log N)$ execution using recursive partitioning.

**Pathfinding (2D Grid)**
* **Dijkstra's Algorithm:** Blind exploratory search guaranteeing the shortest path using a Min-Heap priority queue.
* **A* Search (A-Star):** Optimized heuristic-based search utilizing Manhattan distance to drastically reduce explored nodes.

## 🛠️ Tech Stack

* **Language:** C++17
* **Graphics Library:** SFML (Simple and Fast Multimedia Library) 3.0.2
* **Build System:** CMake

## 💻 Build Instructions (macOS / Linux)

### Prerequisites
Ensure you have a C++ compiler and CMake installed. You will also need SFML 3:
```bash
# For macOS (Homebrew)
brew install sfml
