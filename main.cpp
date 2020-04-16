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
