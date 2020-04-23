//
// Created by William on 18/04/2020.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

/// Node is a struct that represents a ball, and is shared between both queues
struct node {
    unsigned long ballValue;
    int scottQueuePos;
    int rustyQueuePos;
    int digitSum;
};

/// Priority queue keeps the node with the highest priority at its head, implemented using a maxHeap in an array
class newPriorityQueue {
private:
    node **arr; //Is an array of pointers as the nodes are shared between queues
    bool isScott; // Bool smaller than string indicating owner
    int maxSize; // n
    int currentSize;

    unsigned long getPriority(const int location) {
        // Check if outside array or nothing is there
        // Will not segfault due to optimised if checking on OR statement
        if (location >= maxSize || arr[location] == nullptr) {
            return 0;
        }
        if (isScott) {
            return arr[location]->ballValue;
        }
        else {
            return arr[location]->digitSum;
        }
    }

    unsigned long getBallValue(const int location) {
        if (location >= maxSize || arr[location] == nullptr) {
            return 0;
        }
        else {
            return arr[location]->ballValue;
        }
    }

    void swap(const int locationOne, const int locationTwo) {
        node *temp = arr[locationOne];
        arr[locationOne] = arr[locationTwo];
        arr[locationTwo] = temp;
        setCurrentPos(locationOne);
        setCurrentPos(locationTwo);
    }

    //Children of a[n] at a[(2*n)+1] and a[(2*n)+2]
    //Parent of a[n] at a[n/2]
    static int parent(const int input) {
        if (input == 1) {
            return 1;
        }
        else {
            return input / 2;
        }
    }

    static int leftChild(const int input) {
        return (2*input);
    }

    static int rightChild(const int input) {
        return (2*input)+1;
    }

    // Set the position variable of a node
    void setCurrentPos(const int location) {
        if (arr[location] == nullptr) {
            return;
        }
        if (isScott) {
            arr[location]->scottQueuePos = location;
        }
        else {
            arr[location]->rustyQueuePos = location;
        }
    }

    /// Finds the next node to swap with when heapifying
    int findNextSwap(const int location) {
        // Get neighbour priorities
        auto parentPriority = getPriority(parent(location));
        auto currentPriority = getPriority(location);
        auto leftChildPriority = getPriority(leftChild(location));
        auto rightChildPriority = getPriority(rightChild(location));

        if (parentPriority < currentPriority) {
            return parent(location);
        }

        // For Rusty, deal with conflicts where parent's digit sum is the same but ball value is not
        if (parentPriority == currentPriority && currentPriority != 0 && getBallValue(location) != getBallValue(parent(location))) {
            if (getBallValue(parent(location)) < getBallValue(location)) {
                return parent(location);
            }
            else {
                return location;
            }
        }

        // If either child's priority is higher than current, find it and swap
        if (leftChildPriority > currentPriority || rightChildPriority > currentPriority) {
            if (leftChildPriority > rightChildPriority) {
                return leftChild(location);
            }
            else if (rightChildPriority > leftChildPriority) {
                return rightChild(location);
            }
            else if (getBallValue(leftChild(location)) > getBallValue(rightChild(location))) {
                return leftChild(location);
            }
            else {
                return rightChild(location);
            }
        }

        // If either child has equal priority, find the child with higher value and swap
        if ((leftChildPriority == currentPriority || rightChildPriority == currentPriority) && currentPriority != 0) {
            if (leftChildPriority > rightChildPriority) {
                if (getBallValue(leftChild(location)) > getBallValue(location)) {
                    return leftChild(location);
                }
                else {
                    return location;
                }
            }
            else if (rightChildPriority > leftChildPriority) {
                if (getBallValue(rightChild(location)) > getBallValue(location)) {
                    return rightChild(location);
                }
                else {
                    return location;
                }
            }
            else {
                if (getBallValue(location) > getBallValue(leftChild(location)) && getBallValue(location) > getBallValue(rightChild(location))) {
                    return location;
                }
                else if (getBallValue(leftChild(location)) > getBallValue(rightChild(location))) {
                    return leftChild(location);
                }
                else {
                    return rightChild(location);
                }
            }
        }

        return location;
    }


    /// Loops findNextSwap until node at location is in correct place
    void maxHeapify(int location) {
        auto nextLocation = findNextSwap(location);
        while (location != nextLocation) {
            swap(location, nextLocation);
            location = nextLocation;
            nextLocation = findNextSwap(location);
        }
    }

public:

    /// Push a node into the heap and heapify
    void push(node *newNode) {
        int location = currentSize + 1;
        arr[location] = newNode;
        currentSize++;

        maxHeapify(location);

        setCurrentPos(location);
    }

    /// Pop the root node from the heap and heapify
    unsigned long pop(int & otherPos) {

        unsigned long returnValue = arr[1]->ballValue;
        if (isScott) {
            otherPos = arr[1]->rustyQueuePos;
        }
        else {
            otherPos = arr[1]->scottQueuePos;
        }
        arr[1] = nullptr;
        swap(1, currentSize);
        currentSize--;
        int location = 1;

        maxHeapify(location);

        return returnValue;
    }

    /// Remove node at location and heapify
    void repair(const int location) {
        swap(location, currentSize);
        delete arr[currentSize];
        arr[currentSize] = nullptr;
        currentSize--;

        maxHeapify(location);
    }

    newPriorityQueue() = delete;

    newPriorityQueue(const bool isScottsQueue, const int n) {
        maxSize = n+1;
        currentSize = 0;
        isScott = isScottsQueue;
        arr = new node* [n+1];
        for (int i=0;i<n+1;i++) {
            arr[i] = nullptr;
        }

    }
    ~newPriorityQueue() {
        delete [] arr;
    }
};

/// Strips line modifiers such as \r or \n for clean input
std::string cleanGetline(std::ifstream & input) {
    std::string returnString;
    getline(input, returnString);
    if (returnString.back() == '\r' || returnString.back() == '\n') {
        returnString.pop_back();
    }
    return returnString;
}

int getDigitSum(unsigned long n) {
    int sum = 0;
    while (n != 0) {
        sum += static_cast<int>(n % 10); // Narrowing conversion always safe as result of n % 10 can never be > 9
        n /= 10;
    }
    return sum;
}

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cout << "Incorrect number of arguments" << std::endl;
        exit(1);
    }

    std::ifstream input(argv[1]);
    std::ofstream output("output.txt");

    std::stringstream testCasesLine(cleanGetline(input));
    int testCases;
    testCasesLine >> testCases;

    for (int testCase=1;testCase<=testCases;testCase++) {
        std::stringstream getDetailsLine(cleanGetline(input));
        std::stringstream getBallsLine(cleanGetline(input));
        std::string flipResult = cleanGetline(input);

        auto startExecution = std::chrono::system_clock::now();

        int n, k;
        getDetailsLine >> n >> k;

        newPriorityQueue scottQueue(true, n);
        newPriorityQueue rustyQueue(false, n);

        // Push all nodes into the queues
        for (int i=0;i<n;i++) {
            auto *nextNode = new node;
            getBallsLine >> nextNode->ballValue;
            nextNode->digitSum = getDigitSum(nextNode->ballValue);

            scottQueue.push(nextNode);
            rustyQueue.push(nextNode);
        }

        // Loop through all of the turns
        int ballsLeft = n;
        bool scottsTurn = (flipResult == "HEADS");
        unsigned long long scottScore = 0, rustyScore = 0;
        int otherPos;
        while (ballsLeft > 0) {
            if (scottsTurn) {
                unsigned long leftThisTurn = k;
                while (leftThisTurn > 0 && ballsLeft > 0) {
                    scottScore += scottQueue.pop(otherPos); // otherPos will be set to the position of the popped node in the opposite queue
                    rustyQueue.repair(otherPos); // Remove the previously popped node from this queue as it's no longer choosable
                    ballsLeft--;
                    leftThisTurn--;
                }
                scottsTurn = !scottsTurn; // Change whose turn it is
            }
            else {
                unsigned long leftThisTurn = k;
                while (leftThisTurn > 0 && ballsLeft > 0) {
                    rustyScore += rustyQueue.pop(otherPos); // otherPos will be set to the position of the popped node in the opposite queue
                    scottQueue.repair(otherPos); // Remove the previously popped node from this queue as it's no longer choosable
                    ballsLeft--;
                    leftThisTurn--;
                }
                scottsTurn = !scottsTurn; // Change whose turn it is
            }
        }

        auto endExecution = std::chrono::system_clock::now();
        std::chrono::duration<double> timeTaken = endExecution - startExecution;
        std::cout << "Input " << testCase << " Time: " << timeTaken.count() << " seconds." << std::endl;

        output << scottScore << " " << rustyScore << std::endl;
    }

    input.close();
    output.close();

    return 0;
}