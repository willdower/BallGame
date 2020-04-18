//
// Created by William on 17/04/2020.
//

#include <iostream>
#include <fstream>
#include <sstream>

class newPriorityQueue {
private:
    unsigned long ***arr;
    bool isScott;
    unsigned long maxSize;
    long long currentSize; //Must be signed

    unsigned long getPriority(unsigned long **input) {
        if (input == nullptr) {
            return 0;
        }
        if (*input == nullptr) {
            return 0;
        }

        unsigned long num = **input;
        if (isScott) {
            return num;
        }
        else {
            unsigned long sum = 0;
            while (num != 0) {
                sum += num % 10;
                num /= 10;
            }
            return sum;
        }
    }

public:
    //Children of a[n] at a[(2*n)+1] and a[(2*n)+2]
    //Parent of a[n] at a[n/2]
    static unsigned long parent(unsigned long input) {
        return input / 2;
    }

    static unsigned long leftChild(unsigned long input) {
        return (2*input)+1;
    }

    static unsigned long rightChild(unsigned long input) {
        return (2*input)+2;
    }

    void push(unsigned long **input) {
        unsigned long location = currentSize;
        arr[currentSize] = input;
        currentSize++;
        while (this->getPriority(arr[parent(location)]) < this->getPriority(arr[location])) {
            unsigned long **temp = arr[location];
            arr[location] = arr[parent(location)];
            arr[parent(location)] = temp;
            location = parent(location);
        }
    }

    unsigned long pop() {
        unsigned long returnValue = **arr[0];
        delete *arr[0];
        *arr[0] = nullptr;
        arr[0] = nullptr;
        currentSize--;
        unsigned long location = 0;
        while (1) {
            unsigned long leftChildPriority;
            unsigned long rightChildPriority;

            if ((static_cast<long long>(leftChild(location))) > currentSize-1 || arr[leftChild(location)] == nullptr) {
                leftChildPriority = 0;
            }
            else {
                leftChildPriority = this->getPriority(arr[leftChild(location)]);
            }

            if (static_cast<long long>((rightChild(location))) > currentSize-1 || arr[leftChild(location)] == nullptr) {
                rightChildPriority = 0;
            }
            else {
                rightChildPriority = this->getPriority(arr[leftChild(location)]);
            }

            if (leftChildPriority == 0 && rightChildPriority == 0) {
                break;
            }

            if (leftChildPriority < rightChildPriority) {
                arr[location] = arr[rightChild(location)];
                arr[rightChild(location)] = nullptr;
                location = rightChild(location);
            }
            else {
                arr[location] = arr[leftChild(location)];
                arr[leftChild(location)] = nullptr;
                location = leftChild(location);
            }
        }
        return returnValue;
    }

    void repair() {
        for (unsigned long i=currentSize-1;i>0;i--) {
            if (*arr[i] != nullptr) {
                break;
            }
            else {
                arr[i] = nullptr;
                currentSize--;
            }
        }
        for (long long i=currentSize-1;i>=0;i--) {
            if (*arr[i] == nullptr) {
                arr[i] = nullptr;
                arr[i] = arr[currentSize-1];
                arr[currentSize-1] = nullptr;
                currentSize--;
                unsigned long location = i;
                while (location != 0) {
                    unsigned long parentPriority = this->getPriority(arr[parent(location)]);
                    unsigned long currentPriority = this->getPriority(arr[location]);
                    unsigned long leftChildPriority = this->getPriority(arr[leftChild(location)]);
                    unsigned long rightChildPriority = this->getPriority(arr[rightChild(location)]);

                    if (parentPriority < currentPriority) {
                        unsigned long  **temp;
                        temp = arr[parent(location)];
                        arr[parent(location)] = arr[location];
                        arr[location] = temp;
                        location = parent(location);
                        continue;
                    }

                    if (leftChildPriority > currentPriority || rightChildPriority > currentPriority){
                        if (leftChildPriority < rightChildPriority) {
                            arr[location] = arr[rightChild(location)];
                            arr[rightChild(location)] = nullptr;
                            location = rightChild(location);
                        }
                        else {
                            arr[location] = arr[leftChild(location)];
                            arr[leftChild(location)] = nullptr;
                            location = leftChild(location);
                        }
                        continue;
                    }
                    break;
                }
            }
        }
    }

    newPriorityQueue() = delete;

    newPriorityQueue(const bool isScottsQueue, const long n) {
        maxSize = n;
        currentSize = 0;
        isScott = isScottsQueue;
        arr = new unsigned long ** [n];
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


        unsigned long nextNum;
        for (int i=0;i<n;i++) {
            auto ** nextNumPtr = new unsigned long *;
            *nextNumPtr = new unsigned long;
            getBallsLine >> **nextNumPtr;

            scottQueue.push(nextNumPtr);
            rustyQueue.push(nextNumPtr);
        }

        unsigned long ballsLeft = n;
        bool scottsTurn = (flipResult == "HEADS");
        unsigned long long scottScore = 0, rustyScore = 0;
        while (ballsLeft > 0) {
            if (scottsTurn) {
                unsigned long leftThisTurn = k;
                scottQueue.repair();
                while (leftThisTurn > 0 && ballsLeft > 0) {
                    scottScore += scottQueue.pop();
                    ballsLeft--;
                    leftThisTurn--;
                }
                scottsTurn = !scottsTurn;
            }
            else {
                unsigned long leftThisTurn = k;
                rustyQueue.repair();
                while (leftThisTurn > 0 && ballsLeft > 0) {
                    rustyScore += rustyQueue.pop();
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