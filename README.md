# 15-Puzzle Solver

This repository contains a C++ implementation of the A* search algorithm to solve the 15-puzzle problem. The 15-puzzle is a sliding puzzle that consists of a frame of numbered square tiles in random order with one tile missing. The goal is to rearrange the tiles to their original order by sliding the tiles into the empty space.

## Features

- **A* Search Algorithm**: Uses the A* search algorithm to find the optimal solution.
- **Heuristic Calculation**: Utilizes the Manhattan distance heuristic to estimate the cost to the goal state.
- **Move Generation**: Generates possible moves (up, down, left, right) for the blank tile.
- **Path Reconstruction**: Reconstructs the path from the start state to the goal state.
- **File I/O**: Reads initial puzzle states from a file and writes the solution moves to an output file.

## Code Structure

- **Node Struct**: Represents a search node with state, parent, cost, heuristic, total cost, and direction of the move.
- **PuzzleUtils Class**: Contains utility functions for the puzzle including finding the blank tile, calculating Manhattan distance, and reconstructing the path.
- **PuzzleMoveGenerator Class**: Generates new states by moving the blank tile up, down, left, or right.
- **AStarSearch Function**: Implements the A* search algorithm to find the solution path.
- **Main Function**: Reads input puzzles from a file, solves each puzzle using A* search, and writes the solutions to an output file.

## Usage

### Prerequisites

- C++ compiler

### Compilation

To compile the program, use the following command:

```sh
g++ -o puzzle_solver puzzle_solver.cpp
