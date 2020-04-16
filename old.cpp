#include <iostream>
#include <fstream>
#include <sstream>

std::string cleanGetline(std::ifstream & input) {
    std::string returnString;
    getline(input, returnString);
    if (returnString.back() == '\r' || returnString.back() == '\n') {
        returnString.pop_back();
    }
    return returnString;
}

unsigned long *initialiseQueue(const unsigned long n) {
    auto *queue = new unsigned long [n]{0};

    return queue;
}

void enqueue(unsigned long value, unsigned long priority, unsigned long *queue) {

    int position = 0;

    while (queue[position] != 0) {
        if (priority < queue[position]) {
            position = position*2+1;
        }
        else {
            position = position*2+2;
        }
    }

    queue[position] = value;

}

unsigned long dequeue(unsigned long *queue, const int n) {
    unsigned long position = 0;

    while (queue[position*2+2] != 0 && position*2+2 < n) {
        position = position*2+2;
    }

    unsigned long returnNum = queue[position];
    queue[position] = 0;

    return returnNum;
}

unsigned long digitSum(unsigned long value) {
    unsigned long digitSum = 0;
    while (value != 0) {
        digitSum += value % 10;
        value /= 10;
    }

    return digitSum;
}

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cout << "Incorrect number of arguments" << std::endl;
        exit(1);
    }

    std::ifstream input(argv[1]);
    std::ofstream output("output.txt");

    int testCases;

    std::stringstream testCasesStream(cleanGetline(input));
    testCasesStream >> testCases;

    while (testCases-- > 0) {
        std::string line;
        int n, k;
        std::stringstream ssNK(cleanGetline(input));
        ssNK >> n;
        ssNK >> k;
        std::stringstream ssNums(cleanGetline(input));

        std::string flip;
        flip = cleanGetline(input);
        bool scottsTurn; //Bool is smallest type to use for this
        scottsTurn = (flip == "HEADS");

        unsigned long *queue = initialiseQueue(n);
        unsigned long nextNum, scottScore = 0, rustyScore = 0;

        if (scottsTurn) {
            while (!ssNums.eof()) {
                ssNums >> nextNum;
                enqueue(nextNum, nextNum, queue);
            }
        }
        else {
            while (!ssNums.eof()) {
                ssNums >> nextNum;
                enqueue(nextNum, digitSum(nextNum), queue);
            }
        }

        while (n > 0) {
            for (int i=k;i>0 && n>0;i--) {
                nextNum = dequeue(queue, n);
                if (scottsTurn) {
                    scottScore += nextNum;
                }
                else {
                    rustyScore += nextNum;
                }
                n--;
            }
            if (n == 0) {
                break;
            }
            scottsTurn = !scottsTurn;
            if (scottsTurn) {
                unsigned long *newQueue = initialiseQueue(n);
                nextNum = dequeue(queue, n);
                while (nextNum != 0) {
                    enqueue(nextNum, nextNum, newQueue);
                }
                delete [] queue;
                queue = newQueue;
                newQueue = nullptr;
            }
            else {
                unsigned long *newQueue = initialiseQueue(n);
                nextNum = dequeue(queue, n);
                while (nextNum != 0) {
                    enqueue(nextNum, digitSum(nextNum), newQueue);
                }
                delete [] queue;
                queue = newQueue;
                newQueue = nullptr;
            }
        }

        output << scottScore << " " << rustyScore << std::endl;

    }

    return 0;
}
