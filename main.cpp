//
// Created by William on 15/04/2020.
//

#include <iostream>
#include <fstream>
#include <sstream>

template <class T>
class sharedObject {
private:
    T **storedObject;
public:
    bool checkObjectExists() {
        return (*storedObject != nullptr);
    }

    T getObject() {
        if (this->checkObjectExists()) {
            return **storedObject;
        }
        else {
            return -1;
        }
    }

    T operator == (const T & otherValue) {
        if (**storedObject == otherValue) {
            return true;
        }
    }

    void operator = (const T toSet) {
        **storedObject = toSet;
    }

    void destroy() {
        delete *storedObject;
        *storedObject = nullptr;
    }

    sharedObject() {
        storedObject = new T*;
        *storedObject = new T;
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
    T *arr;
    bool isScott;
    unsigned long maxSize;
    unsigned long currentSize;

     T getPriority(T input) {
        if (isScott) {
            return input;
        }
        else {
            T sum = 0;
            while (input != 0) {
                sum += input % 10;
                input /= 10;
            }
            return sum;
        }
    }

public:
    //Children of a[n] at a[(2*n)+1] and a[(2*n)+2]
    //Parent of a[n] at a[n/2]
    void push(T input) {
        unsigned long location = currentSize;
        arr[currentSize] = input;
        currentSize++;
        while (getPriority(arr[location/2]) > this->getPriority(arr[location])) {
            T temp = arr[location];
            arr[location] = arr[location/2];
            arr[location/2] = temp;
            location /= 2;
        }
    }

    T pop() {
         T returnValue = arr[0];
         arr[0] = 0;
         unsigned long location = 0;
         while (arr[(2*location)+1] != 0 && arr[(2*location)+2] != 0) {
             T leftChildPriority = this->getPriority(arr[(2*location)+1]);
             T rightChildPriority = this->getPriority(arr[2*location]+2);

             if (leftChildPriority < rightChildPriority) {
                 arr[location] = arr[(2*location)+2];
                 arr[(2*location)+2] = 0;
                 location = (2*location)+2;
             }
             else {
                 arr[location] = arr[(2*location)+1];
                 arr[(2*location)+1] = 0;
                 location = (2*location)+1;
             }
         }
         return returnValue;
    }

    newPriorityQueue() = delete;

    newPriorityQueue(const bool isScottsQueue, const long n) {
        maxSize = n;
        currentSize = 0;
        isScott = isScottsQueue;
        arr = new T [n]{0};
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

int main() {

    sharedObject<int> newObj;

    newObj = 4;

    std::cout << newObj.getObject() << std::endl;

    sharedObject<int> secondObj(newObj);

    std::cout << secondObj.getObject() << std::endl;

    secondObj = 3;

    std::cout << newObj.getObject() << std::endl;

    newObj.destroy();

    std::cout << secondObj.getObject() << std::endl;

    return 0;
}
