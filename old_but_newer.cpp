//
// Created by William on 15/04/2020.
//

#include <iostream>
#include <stack>
#include <fstream>
#include <sstream>

class sharedObject {
private:
    unsigned long **storedObject;
public:
    bool checkObjectExists() {
        return (*storedObject != nullptr);
    }

    unsigned long getObject() {
        if (this->checkObjectExists()) {
            return **storedObject;
        }
        else {
            return 0;
        }
    }

    T operator == (const T & otherValue) {
        if (**storedObject == otherValue) {
            return true;
        }
    }

    void operator = (const T & toSet) {
        **storedObject = toSet;
    }

    bool operator < (const T & toCompare) {
        return (**storedObject < toCompare);
    }

    bool operator > (const T & toCompare) {
        return (**storedObject > toCompare);
    }

    void destroy() {
        delete *storedObject;
        *storedObject = nullptr;
    }

    sharedObject() {
        storedObject = new T*;
        *storedObject = new T;
        **storedObject = 0;
    }
    explicit sharedObject(const T & input) {
        storedObject = new T*;
        *storedObject = new T;
        **storedObject = input;
    }
    sharedObject(sharedObject<T> & existingObject) {
        storedObject = existingObject.storedObject;
    }

    ~sharedObject() {
        delete storedObject;
    }
};

template <class T>
class newPriorityQueue {
private:
    sharedObject<T> *arr;
    bool isScott;
    unsigned long maxSize;
    unsigned long currentSize;

     unsigned long getPriority(sharedObject<T> input) {
        unsigned long num = input.getObject();
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

    void push(sharedObject<T> input) {
        unsigned long location = currentSize;
        arr[currentSize] = input;
        currentSize++;
        while (getPriority(arr[parent(location)]) > this->getPriority(arr[location])) {
            sharedObject<T> temp = arr[location];
            arr[location] = arr[parent(location)];
            arr[parent(location)] = temp;
            location = parent(location);
        }
    }

    T pop() {
         T returnValue = arr[0];
         arr[0] = 0;
         unsigned long location = 0;
         while (arr[leftChild(location)] != 0 && arr[rightChild(location)] != 0) {
             T leftChildPriority = this->getPriority(arr[leftChild(location)]);
             T rightChildPriority = this->getPriority(arr[rightChild(location)]);

             if (leftChildPriority < rightChildPriority) {
                 arr[location] = arr[rightChild(location)];
                 arr[rightChild(location)] = 0;
                 location = rightChild(location);
             }
             else {
                 arr[location] = arr[leftChild(location)];
                 arr[leftChild(location)] = 0;
                 location = leftChild(location);
             }
         }
         return returnValue;
    }

    void repair() {
         for (int i=currentSize-1;i>0;i--) {
             if (arr[i] != 0) {
                 currentSize = i+1;
                 break;
             }
         }
        for (int i=currentSize-1;i>0;i--) {
            if (arr[i] == 0) {
                arr[i] = arr[currentSize-1];
                arr[currentSize-1] = 0;
                currentSize--;
                unsigned long location = currentSize-1;
                while (location != 0) {
                    T parentPriority = this->getPriority(arr[parent(location)]);
                    T currentPriority = this->getPriority(arr[location]);
                    T leftChildPriority = this->getPriority(arr[leftChild(location)]);
                    T rightChildPriority = this->getPriority(arr[rightChild(location)]);

                    if (parentPriority < currentPriority) {
                        T temp;
                        temp = arr[parent(location)];
                        arr[parent(location)] = arr[location];
                        arr[location] = temp;
                        location = parent(location);
                        continue;
                    }

                    if (leftChildPriority > currentPriority || rightChildPriority > currentPriority){
                        if (leftChildPriority < rightChildPriority) {
                            arr[location] = arr[rightChild(location)];
                            arr[rightChild(location)] = 0;
                            location = rightChild(location);
                        }
                        else {
                            arr[location] = arr[leftChild(location)];
                            arr[leftChild(location)] = 0;
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
        arr = new sharedObject<T> [n];
        for (int i=0;i<n;i++) {
            arr[i] = T();
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

/*int main(int argc, char **argv) {

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

        newPriorityQueue<unsigned long> scottQueue(true, n);
        newPriorityQueue<unsigned long> rustyQueue(false, n);

        unsigned long nextNum;
        for (int i=0;i<n;i++) {
            getBallsLine >> nextNum;

            sharedObject<unsigned long> nextNumShared(nextNum);

            scottQueue.push(nextNumShared);
            rustyQueue.push(nextNumShared);
        }
    }

    return 0;
}*/

/*int main() {

    sharedObject<int> newObj;

    newObj = 4;

    std::cout << newObj.getObject() << std::endl;

    sharedObject<int> secondObj(newObj);

    std::cout << secondObj.getObject() << std::endl;

    secondObj = 3;

    std::cout << (secondObj < 4) << " " << (secondObj > 4) << std::endl;

    std::cout << newObj.getObject() << std::endl;

    newObj.destroy();

    std::cout << secondObj.getObject() << std::endl;

    return 0;
}*/

class testStack {
private:
    int **arr;
public:

    void push(int* & pointer) {
        arr[0] = pointer;
    }

    testStack() {
        arr = new int* [1];
    }
};

int main() {

    std::stack<int*> one;
    std::stack<int*> two;

    int *test = new int;

    *test = 4;

    one.push(test);


    return 0;
}
