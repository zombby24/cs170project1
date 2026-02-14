#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <cstdlib>
#include <functional>
#include <queue>
#include <set>

using namespace std;

//puzzle board is a 2d vector
using Board = vector<vector<int>>;

//goal state
const Board goal {
        {1,2,3},
        {4,5,6},
        {7,8,0}
        };

// === Node struct ==========================================================
struct Node {
public:
    Board puzzle;
    int gCost; // g(n)
    int hCost; // h(n)
    Node* parent;

    Node(Board board, int g, int h, Node* p) 
    {
        puzzle = board;
        gCost = g;
        hCost = h;
        parent = p;
    }

    // f(n) = g + h
    bool operator<(const Node& other) const 
    {
        return (gCost + hCost) > (other.gCost + other.hCost);
    }
};


// === Heuristic Functions =========================================================
int uniform_cost_search(const Board& state) 
{
    return 0;
}

int misplaced_tile_heuristic(const Board& state) 
{
    int incorrect_tiles = 0;
    const int n = state.size();

    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < state[i].size(); j++) 
        {
            int tileNum = state[i][j];
            if (tileNum != 0 && tileNum != goal[i][j]) 
            {
                incorrect_tiles++;
            }
        }
    }
    return incorrect_tiles;
}

int manhattan_distance_heuristic(const Board& state) 
{
    int total = 0;
    const int n = (int)state.size();
    unordered_map<int, pair<int,int>> pos;

    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < (int)goal[i].size(); j++) 
        {
            pos[goal[i][j]] = {i, j};
        }
    }

    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < (int)state[i].size(); j++) 
        {
            int tileNum = state[i][j];
            if (tileNum == 0) continue;

            auto iterator = pos.find(tileNum);
            if (iterator != pos.end()) 
            {
                int gr = iterator->second.first;
                int gc = iterator->second.second;
                total += abs(i - gr) + abs(j - gc);
            }
        }
    }
    return total;
}

// === Helper Functions ======================================================
//check if puzzle has been solved
bool goalTest(const Board& puzzle) 
{
    if(puzzle == goal)
        return true;
    else
        return false;
}

//allow user to enter their own puzzle
Board createPuzzle()
{
    int num;
    Board newPuzzle(3, vector<int>(3));

    cout << "Enter your puzzle, using a zero to represent the blank. Please only enter  valid 8-puzzles."
     << "Enter the puzzle demilimiting the numbers with a space. Hit en only when finished.\n";
    for (int i = 0; i < 3; i++)
    {
        cout << "Enter the ";
        if (i == 0)
            cout << "first";
        else if (i == 1)
            cout << "second";
        else
            cout << "third";
        
        cout << " row: ";

        for (int j = 0; j < 3; j++)
        {
            cin >> newPuzzle[i][j];
        }
    }
    return newPuzzle;
}

//prints current state of puzzle (match output from instructions!)
void print_status(Node* node, int expandedCount, int maxQueueSize, bool goalReached)
{
    cout << "The best state to expand with a g(n) = " << node->gCost << " and h(n) = " << node->hCost << " is:\n";

    // print puzzle grid
    for (int i = 0; i < node->puzzle.size(); i++) 
    {
        cout << "[";
        for (int j = 0; j < node->puzzle[i].size(); j++) 
        {
            cout << node->puzzle[i][j];
            if (j != node->puzzle[i].size() - 1)
                cout << ", ";
        }
        cout << "]\n";
    }
    cout << "\n";

    if (goalReached) 
    {
        cout << "Goal state reached!\nSolution depth was " << node->gCost << ".\nNumber of nodes expanded: "
             << expandedCount << ".\nMax queue size: " << maxQueueSize << ".\n";
    }
}

//expand node - used in general_search
vector<Board> expand(const Board& state)
{
    vector<Board> children;
    int row = -1;
    int col = -1;

    //find 0
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (state[i][j] == 0)
            {
                row = i;
                col = j;
            }
        }
    }

    //possible moves
    int moves[4][2] = {
        {1, 0},
        {-1, 0},
        {0, 1},
        {0, -1}
    };

    //push possible moves
    for (int k = 0; k < 4; k++)
    {
        int newRow = row + moves[k][0];
        int newCol = col + moves[k][1];

        if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3)
        {
            Board newBoard = state;
            swap(newBoard[row][col], newBoard[newRow][newCol]);
            children.push_back(newBoard);
        }
    }
    return children;
}


// === General Search Function ====================================================
/* --- given psudocode ---
function general-search(problem, QUEUEING-FUNCTION)  
nodes = MAKE-QUEUE(MAKE-NODE(problem.INITIAL-STATE))  
loop do 
if EMPTY(nodes) then return "failure"  
 node = REMOVE-FRONT(nodes)  
if problem.GOAL-TEST(node.STATE) succeeds then return node 
 nodes = QUEUEING-FUNCTION(nodes, EXPAND(node, problem.OPERATORS))  
end
*/
Node* general_search(const Board& problem, function<int(const Board&)> heuristic)
{
    int expandedCount = 0;
    int maxQueueSize = 1;

    //change to min heap
    auto cmp = [](Node* a, Node* b) 
    {
        return (a->gCost + a->hCost) > (b->gCost + b->hCost);
    };
    priority_queue<Node*, vector<Node*>, decltype(cmp)> nodes(cmp);

    Node* start = new Node(problem, 0, heuristic(problem), nullptr);
    nodes.push(start);

    //used to check which nodes have already been visited
    set<Board> visited;

    //loop
    while (!nodes.empty())
    {
        Node* current = nodes.top();
        nodes.pop();

        if (visited.count(current->puzzle))
            continue;

        visited.insert(current->puzzle);

        // check if goal has been reached, stop if true!
        if (goalTest(current->puzzle))
        {
            print_status(current, expandedCount, maxQueueSize, true);
            return current;
        }

        //expand + check child nodes
        expandedCount++;
        vector<Board> children = expand(current->puzzle);
        print_status(current, expandedCount, maxQueueSize, false);

        for (int i = 0; i < children.size(); i++)
        {
            if (!visited.count(children[i]))
            {
                Node* child = new Node(children[i], current->gCost + 1, heuristic(children[i]), current);
                nodes.push(child);
            }
        }

        //update max queue size
        if ((int)nodes.size() > maxQueueSize)
            maxQueueSize = nodes.size();
    }

    //all nodes have been expanded without reaching goal state
    cout << "Puzzle is unsolvable!!\n";
    return nullptr;
}

// === Main ===========================================================
int main()
{
    //given puzzles
    Board trivial 
    {
    {1,2,3},
    {4,5,6},
    {7,8,0}
    };

    Board veryEasy 
    {
    {1, 2, 3}, 
    {4, 5, 6}, 
    {7, 0, 8}
    };

    Board easy 
    {
    {1, 2, 0}, 
    {4, 5, 3}, 
    {7, 8, 6}
    };

    Board doable 
    {
    {0, 1, 2}, 
    {4, 5, 3}, 
    {7, 8, 6}
    };

    Board ohBoy 
    {
    {8, 7, 1}, 
    {6, 0, 2}, 
    {5, 4, 3}
    };

    //unsolvable!
    Board impossible 
    {
    {2,1,3},
    {4,5,6},
    {7,8,0}
    };

    //initialize puzzle
    Board puzzle;

    //menu
    int puzzleChoice;
    cout << "Welcome to an 8-Puzzle Solver. Type '1' to use a default puzzle, or '2' to create your  own.\n";
    cin >> puzzleChoice;
    if(puzzleChoice==2)
    {
        puzzle = createPuzzle();
    }
    else if (puzzleChoice==1)
    {
        cout << "You wish to use a default puzzle. Please enter a desired difficulty on a scale from 0 to 5. \n";
        cin >> puzzleChoice;
        switch (puzzleChoice)
        {
        case 0:
            cout << "Difficulty of 'Trivial' selected.";
            puzzle = trivial;
            break;
        case 1:
            cout << "Difficulty of 'Very Easy' selected.";
            puzzle = veryEasy;
            break;
        case 2:
            cout << "Difficulty of 'Easy' selected.";
            puzzle = easy;
            break;
        case 3:
            cout << "Difficulty of 'Doable' selected.";
            puzzle = doable;
            break;
        case 4:
            cout << "Difficulty of 'Oh Boy' selected.";
            puzzle = ohBoy;
            break;
        case 5:
            cout << "Difficulty of 'Impossible' selected.";
            puzzle = impossible;
            break;
        default:
            cout << "\nError: Invalid selection. Exiting program...\n";
            exit(0);
            break;
        }
    }
    else
    {
        cout << "\nError: Invalid selection. Exiting program...\n";
        exit(0);
    }

    cout << "\n\nSelect algorithm. (1) for Uniform Cost Search, (2) for the Misplaced Tile Heuristic, or (3) the Manhattan Distance Heuristic.\n";
    int algChoice;
    cin >> algChoice;

    switch (algChoice)
    {
    case 1:
        cout << "\nYou chose Uniform Cost Search\n";
        general_search(puzzle, uniform_cost_search);
        break;
    case 2:
        cout << "\nYou chose Misplaced Tile Heuristic\n";
        general_search(puzzle, misplaced_tile_heuristic);
        break;
    case 3:
        cout << "\nYou chose Manhattan Distance Heuristic\n";
        general_search(puzzle, manhattan_distance_heuristic);
        break;
    
    default:
        cout << "\nError: Invalid selection. Exiting program...\n";
        exit(0);
        break;
    }
    return 0;
}