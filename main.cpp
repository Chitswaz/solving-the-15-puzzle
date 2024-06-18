#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

// Struct to represent a search node
struct Node
{
    vector<int> state; // The current state
    Node *parent;      // Parent node
    int cost;          // Cost from the start node
    int heuristic;     // Heuristic value
    int totalCost;     // Total estimated cost (cost + heuristic)
    char direction;    // Direction of move from parent node

    // Constructor
    Node(vector<int> state, Node *parent, int cost, int heuristic, char direction)
        : state(state), parent(parent), cost(cost), heuristic(heuristic), direction(direction)
    {
        totalCost = cost + heuristic;
    }
};

// Comparator for priority queue based on totalCost
struct CompareNode
{
    bool operator()(const Node *a, const Node *b)
    {
        return a->totalCost > b->totalCost;
    }
};

class PuzzleUtils
{
public:
    static int findBlankIndex(const vector<int> &state)
    {
        // Implementation of the findBlankIndex function
        for (int i = 0; i < 16; ++i)
        {
            if (state[i] == -1)
            {
                return i;
            }
        }
        return -1; // If the blank tile is not found
    }

    static int manhattanDistance(const vector<int> &state, const vector<int> &goalState)
    {
        // Implementation of the manhattanDistance function
        int distance = 0;
        for (int i = 0; i < state.size(); ++i)
        {
            if (state[i] != -1)
            {
                int targetIndex = find(goalState.begin(), goalState.end(), state[i]) - goalState.begin();
                distance += abs(i / 4 - targetIndex / 4) + abs(i % 4 - targetIndex % 4);
            }
        }
        return distance;
    }

    static int heuristic(const vector<int> &state, const vector<int> &goalState)
    {
        return manhattanDistance(state, goalState);
    }

    // Function to reconstruct the path from the start node to the goal node
    static vector<vector<int>> getPath(Node *goalNode)
    {
        vector<vector<int>> path;
        Node *current = goalNode;
        while (current != nullptr)
        {
            path.push_back(current->state);
            current = current->parent;
        }
        reverse(path.begin(), path.end());
        return path;
    }
};

class PuzzleMoveGenerator
{
public:
    static vector<int> generateUpState(const vector<int> &currentState)
    {
        // Implementation of the generateUpState function
        vector<int> newState = currentState;
        int blankIndex = PuzzleUtils::findBlankIndex(newState);
        if (blankIndex >= 4)
        {
            swap(newState[blankIndex], newState[blankIndex - 4]);
        }
        return newState;
    }

    static vector<int> generateDownState(const vector<int> &currentState)
    {
        // Implementation of the generateDownState function
        vector<int> newState = currentState;
        int blankIndex = PuzzleUtils::findBlankIndex(newState);
        if (blankIndex < 12)
        {
            swap(newState[blankIndex], newState[blankIndex + 4]);
        }
        return newState;
    }

    static vector<int> generateLeftState(const vector<int> &currentState)
    {
        // Implementation of the generateLeftState function
        vector<int> newState = currentState;
        int blankIndex = PuzzleUtils::findBlankIndex(newState);
        if (blankIndex % 4 != 0)
        {
            swap(newState[blankIndex], newState[blankIndex - 1]);
        }
        return newState;
    }

    static vector<int> generateRightState(const vector<int> &currentState)
    {
        // Implementation of the generateRightState function
        vector<int> newState = currentState;
        int blankIndex = PuzzleUtils::findBlankIndex(newState);
        if (blankIndex % 4 != 3)
        {
            swap(newState[blankIndex], newState[blankIndex + 1]);
        }
        return newState;
    }
};

// A* search algorithm
// A* search algorithm
vector<vector<int>> AStarSearch(const vector<int> &startState, const vector<int> &goalState, ofstream &outFile, int puzzleNumber)
{
    priority_queue<Node *, vector<Node *>, CompareNode> openSet;

    openSet.push(new Node(startState, nullptr, 0, PuzzleUtils::heuristic(startState, goalState), ' '));

    while (!openSet.empty())
    {
        Node *current = openSet.top();
        openSet.pop();

        if (current->state == goalState)
        {
            // Goal state reached, reconstruct the path and return
            vector<vector<int>> path = PuzzleUtils::getPath(current);
            // Print the moves
            outFile << puzzleNumber << ", solved , " << current->cost << ":" << endl;
            for (size_t i = 1; i < path.size(); ++i)
            {
                int prevBlankIndex = -1;
                int currBlankIndex = -1;
                for (size_t j = 0; j < path[i].size(); ++j)
                {
                    if (path[i][j] == -1)
                    {
                        currBlankIndex = j;
                    }
                    if (path[i - 1][j] == -1)
                    {
                        prevBlankIndex = j;
                    }
                }
                char direction;
                if (currBlankIndex == prevBlankIndex - 1)
                {
                    direction = 'L';
                }
                else if (currBlankIndex == prevBlankIndex + 1)
                {
                    direction = 'R';
                }
                else if (currBlankIndex == prevBlankIndex - 4)
                {
                    direction = 'U';
                }
                else if (currBlankIndex == prevBlankIndex + 4)
                {
                    direction = 'D';
                }
                outFile << (prevBlankIndex / 4) << " " << (prevBlankIndex % 4) << " " << direction << endl;
            }
            outFile << endl; // Add a newline between puzzles
            return path;
        }

        // Generate successor states
        vector<vector<int>> successors = {
            PuzzleMoveGenerator::generateUpState(current->state),
            PuzzleMoveGenerator::generateDownState(current->state),
            PuzzleMoveGenerator::generateLeftState(current->state),
            PuzzleMoveGenerator::generateRightState(current->state)};

        for (int i = 0; i < successors.size(); ++i)
        {
            const vector<int> &successor = successors[i];
            int newCost = current->cost + 1;
            int heuristic = PuzzleUtils::heuristic(successor, goalState);
            Node *newNode = new Node(successor, current, newCost, heuristic, ' ');
            openSet.push(newNode);
        }
    }

    // If no path found
    return {};
}

int main()
{
    ifstream inFile("15_puzzles.txt");
    ofstream outFile("output.txt");

    int puzzleNumber = 1;
    while (inFile)
    {
        // Read the start state of the puzzle
        vector<int> startState;
        int tile;
        for (int i = 0; i < 16; ++i)
        {
            inFile >> tile;
            startState.push_back(tile);
        }

        // Define the goal state
        vector<int> goalState = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, -1};

        // Perform A* search
        vector<vector<int>> path = AStarSearch(startState, goalState, outFile, puzzleNumber);

        // Output the puzzle number
        ++puzzleNumber;
    }

    inFile.close();
    outFile.close();

    return 0;
}
