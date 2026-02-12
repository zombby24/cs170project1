#include <stdio.h>
#include <iostream>
#include <vector>
#include <deque>
#include <functional>
#include <cmath>

using namespace std;

#define N 8;

class Problem {
public:
    vector<int> initialState;

    Problem(vector<int> init) : initialState(init) {}

    bool goalTest(const vector<int>& state) {
        vector<int> goal {1,2,3,4,5,6,7,8,0};
        if(state == goal)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    vector<vector<int>> expand(const vector<int>& state) {
        // Return child states
        return {};
    }
};

struct Node {
    vector<int> state;
    Node* parent;
    int cost;

    Node(vector<int> s, Node* p = nullptr, int c = 0)
        : state(s), parent(p), cost(c) {}
};

vector<int> createPuzzle();
void displayPuzzle(vector<int> puzzle);

int main()
{
    //given test puzzles 
    vector<int> depth0 {1,2,3,4,5,6,7,8,0};
    vector<int> depth2 {1,2,3,4,5,6,0,7,8};
    vector<int> depth4 {1,2,3,5,0,6,4,7,8};
    vector<int> depth8 {1,3,6,5,0,2,4,7,8};
    vector<int> depth12 {1,3,6,5,0,7,4,8,2};
    vector<int> depth16 {1,6,7,5,0,3,4,8,2};
    vector<int> depth20 {7,1,2,4,8,5,6,3,0};
    vector<int> depth24 {0,7,2,4,6,1,3,5,8};

    //initialize puzzle
    vector<int> puzzle;

    //menu
    int puzzleChoice;
    cout << "Welcome! Choose a test puzzle (1-8), or enter 0 to create your own puzzle: ";
    cin >> puzzleChoice;

    switch (puzzleChoice)
    {
    case 0:
        puzzle = createPuzzle();
        break;
    case 1:
        puzzle = depth0;
        break;
    case 2:
        puzzle = depth2;
        break;
    case 3:
        puzzle = depth4;
        break;
    case 4:
        puzzle = depth8;
        break;
    case 5:
        puzzle = depth12;
        break;
    case 6:
        puzzle = depth16;
        break;
    case 7:
        puzzle = depth20;
        break;
    case 8:
        puzzle = depth24;
        break;
    
    default:
        cout << "Error: input failed.\n";
        exit(0);
        break;
    }
    displayPuzzle(puzzle);

    return 0;
}


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
Node* generalSearch(Problem& problem, function<void(deque<Node*>&, const vector<Node*>&)> queueingFunction)
{
    // nodes = MAKE-QUEUE(MAKE-NODE(problem.INITIAL-STATE))  
    deque<Node*> nodes;
    nodes.push_back(new Node(problem.initialState));

    while (true) {
        // if EMPTY(nodes) then return failure
        if (nodes.empty())
        {
            return nullptr;
        }

        // node = REMOVE-FRONT(nodes)  
        Node* node = nodes.front();
        nodes.pop_front();

        // if problem.GOAL-TEST(node.STATE) succeeds then return node 
        if (problem.goalTest(node->state))
        {
            return node;
        }

        // EXPAND
        vector<vector<int>> childrenStates = problem.expand(node->state);
        vector<Node*> childrenNodes;

        for (auto& state : childrenStates) {
            childrenNodes.push_back(new Node(state, node, node->cost + 1));
        }

        // nodes = QUEUEING-FUNCTION(nodes, children)
        queueingFunction(nodes, childrenNodes);
    }
}


void uniform_cost_search()
{

}

void misplaced_tile()
{

}

void manhattan_distance()
{

}


vector<int> createPuzzle()
{
    int num;
    int vectorSize = N + 1;
    vector<int> newPuzzle;

    cout << "You chose to create your own puzzle.\n";
    for (int i = 0; i < vectorSize; i++)
    {
        cout << "Enter a number: ";
        cin >> num;
        newPuzzle.push_back(num);
    } 
    return newPuzzle;
}

// change display?
void displayPuzzle(vector<int> puzzle)
{
    cout << "[ ";
    for (int i = 0; i < puzzle.size(); i++) 
    {
        cout << puzzle[i] << " ";
    }
    cout << "]\n";
}