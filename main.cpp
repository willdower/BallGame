//
// Created by William on 18/04/2020.
//

#include <iostream>
#include <fstream>
#include <sstream>

struct node {
    unsigned long ballValue;
    unsigned long scottQueuePos;
    unsigned long rustyQueuePos;
};

class newPriorityQueue {
private:
    node **arr;
    bool isScott;
    unsigned long maxSize;
    long long currentSize; //Must be signed

    unsigned long getPriority(const unsigned long location) {
        // Check if outside array or nothing is there
        // Will not segfault due to optimised if checking on OR statement
        if (location >= maxSize || arr[location] == nullptr) {
            return 0;
        }
        unsigned long input = arr[location]->ballValue;
        if (isScott) {
            return input;
        }
        else {
            unsigned long sum = 0;
            while (input != 0) {
                sum += input % 10;
                input /= 10;
            }
            return sum;
        }
    }

    unsigned long getBallValue(const unsigned long location) {
        if (location >= maxSize || arr[location] == nullptr) {
            return 0;
        }
        else {
            return arr[location]->ballValue;
        }
    }

    void swap(const unsigned long locationOne, const unsigned long locationTwo) {
        node *temp = arr[locationOne];
        arr[locationOne] = arr[locationTwo];
        arr[locationTwo] = temp;
        setCurrentPos(locationOne);
        setCurrentPos(locationTwo);
    }

    //Children of a[n] at a[(2*n)+1] and a[(2*n)+2]
    //Parent of a[n] at a[n/2]
    static unsigned long parent(unsigned long input) {
        if (input == 1) {
            return 1;
        }
        else {
            return input / 2;
        }
    }

    static unsigned long leftChild(unsigned long input) {
        return (2*input);
    }

    static unsigned long rightChild(unsigned long input) {
        return (2*input)+1;
    }

    bool isLeaf(const unsigned long location) {
        return (arr[leftChild(location)] == nullptr && arr[rightChild(location)]);
    }

    void setCurrentPos(const unsigned long location) {
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

    unsigned long findNextSwap(unsigned long location) {
        auto parentPriority = getPriority(parent(location));
        auto currentPriority = getPriority(location);
        auto leftChildPriority = getPriority(leftChild(location));
        auto rightChildPriority = getPriority(rightChild(location));

        if (parentPriority < currentPriority) {
            return parent(location);
        }

        if (parentPriority == currentPriority && currentPriority != 0 && getBallValue(location) != getBallValue(parent(location))) {
            if (getBallValue(parent(location)) < getBallValue(location)) {
                return parent(location);
            }
            else {
                return location;
            }
        }

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

    void maxHeapify(unsigned long location) {
        auto nextLocation = findNextSwap(location);
        while (location != nextLocation) {
            swap(location, nextLocation);
            location = nextLocation;
            nextLocation = findNextSwap(location);
        }

        /*while (true) {
            auto parentPriority = getPriority(parent(location));
            auto currentPriority = getPriority(location);
            auto leftChildPriority = getPriority(leftChild(location));
            auto rightChildPriority = getPriority(rightChild(location));

            if (parentPriority < currentPriority) {
                swap(location, parent(location));
                location = parent(location);
                continue;
            }
            else if (currentPriority == parentPriority && currentPriority != 0 && getBallValue(location) != getBallValue(parent(location))) {
                if (getBallValue(parent(location)) < getBallValue(location)) {
                    swap(location, parent(location));
                    location = parent(location);
                }
                else {
                    break;
                }
            }
            else if (leftChildPriority > currentPriority || rightChildPriority > currentPriority) {
                if (leftChildPriority > rightChildPriority) {
                    swap(location, leftChild(location));
                    location = leftChild(location);
                    continue;
                }
                else {
                    swap(location, rightChild(location));
                    location = rightChild(location);
                    continue;
                }
            }
            else {
                break;
            }
        }*/
    }

public:

    void push(node *newNode) {
        unsigned long location = currentSize+1;
        arr[location] = newNode;
        currentSize++;
        while (getPriority(parent(location)) < getPriority(location)) {
            swap(location, parent(location));
            location = parent(location);
        }
        if (getPriority(parent(location)) == getPriority(location)) {
            if (getBallValue(parent(location)) < getBallValue(location)) {
                swap(location, parent(location));
                location = parent(location);
            }
        }
        if (isScott) {
            arr[location]->scottQueuePos = location;
        }
        else {
            arr[location]->rustyQueuePos = location;
        }
    }

    unsigned long pop(unsigned long & otherPos) {

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
        unsigned long location = 1;

        // Finish heapifying when:
        // a) Location has higher priority than children
        // b) It is a leaf node

        maxHeapify(location);

        /*while (true) {
            if (isLeaf(location)) {
                break;
            }

            unsigned long currentPriority = getPriority(location);
            unsigned long leftChildPriority = getPriority(leftChild(location));
            unsigned long rightChildPriority = getPriority(rightChild(location));

            if (currentPriority >= leftChildPriority && currentPriority >= rightChildPriority) {
                break;
            }
            else if (leftChildPriority > rightChildPriority) {
                swap(location, leftChild(location));
                location = leftChild(location);
            }
            else {
                swap(location, rightChild(location));
                location = rightChild(location);
            }
        }*/
        return returnValue;
    }

    void repair(unsigned long location) {
        swap(location, currentSize);
        delete arr[currentSize];
        arr[currentSize] = nullptr;
        currentSize--;

        // Finish repairing when:
        // a) It's priority is greater than its children AND its priority is less than its parent
        // b) It's priority is greater than its children AND it is root
        // c) It's priority is less than its parent AND it is leaf

        maxHeapify(location);
    }

        /*while (true) {

            auto parentPriority = getPriority(parent(location));
            auto currentPriority = getPriority(location);
            auto leftChildPriority = getPriority(leftChild(location));
            auto rightChildPriority = getPriority(rightChild(location));

            // If current location is the root node
            if (location == 0 && currentPriority >= leftChildPriority && currentPriority >= rightChildPriority) {
                break;
            }
            else if (location == 0 && leftChildPriority > rightChildPriority) {
                swap(location, leftChild(location));
                location = leftChild(location);
                continue;
            }
            else if (location == 0 && rightChildPriority > leftChildPriority) {
                swap(location, rightChild(location));
                location = rightChild(location);
                continue;
            }

            // If current location is a leaf node
            if (isLeaf(location) && currentPriority <= parentPriority) {
                break;
            }
            else if (isLeaf(location) && currentPriority > parentPriority) {
                swap(location, parent(location));
                location = parent(location);
                continue;
            }

            // If it is NOT root OR leaf
            if (parentPriority < currentPriority) {
                swap(location, parent(location));
                location = parent(location);
                continue;
            }
            else if (leftChildPriority > currentPriority || rightChildPriority > currentPriority) {
                if (leftChildPriority > rightChildPriority) {
                    swap(location, leftChild(location));
                    location = leftChild(location);
                    continue;
                }
                else {
                    swap(location, rightChild(location));
                    location = rightChild(location);
                    continue;
                }
            }
            else {
                break;
            }
        }

    }*/

    newPriorityQueue() = delete;

    newPriorityQueue(const bool isScottsQueue, const long n) {
        maxSize = n+1;
        currentSize = 0;
        isScott = isScottsQueue;
        arr = new node* [n+1];
        for (int i=0;i<n;i++) {
            arr[i] = nullptr;
        }

    }
    ~newPriorityQueue() {
        delete [] arr;
    }
};

std::string cleanGetline(std::ifstream & input) {
    std::string returnString;
    getline(input, returnString);
    if (returnString.back() == '\r' || returnString.back() == '\n') {
        returnString.pop_back();
    }
    return returnString;
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

        int n, k;
        getDetailsLine >> n >> k;

        newPriorityQueue scottQueue(true, n);
        newPriorityQueue rustyQueue(false, n);

        for (int i=0;i<n;i++) {
            auto *nextNode = new node;
            getBallsLine >> nextNode->ballValue;

            scottQueue.push(nextNode);
            rustyQueue.push(nextNode);
        }

        unsigned long ballsLeft = n;
        bool scottsTurn = (flipResult == "HEADS");
        unsigned long long scottScore = 0, rustyScore = 0;
        unsigned long otherPos;
        while (ballsLeft > 0) {
            if (scottsTurn) {
                unsigned long leftThisTurn = k;
                while (leftThisTurn > 0 && ballsLeft > 0) {
                    scottScore += scottQueue.pop(otherPos);
                    rustyQueue.repair(otherPos);
                    ballsLeft--;
                    leftThisTurn--;
                }
                scottsTurn = !scottsTurn;
            }
            else {
                unsigned long leftThisTurn = k;
                while (leftThisTurn > 0 && ballsLeft > 0) {
                    rustyScore += rustyQueue.pop(otherPos);
                    scottQueue.repair(otherPos);
                    ballsLeft--;
                    leftThisTurn--;
                }
                scottsTurn = !scottsTurn;
            }
        }
        output << scottScore << " " << rustyScore << std::endl;
    }

    input.close();
    output.close();

    return 0;
}