#include <iostream>
#include <memory>
#include "include/TQueue.h"
#include "include/constants.h"
#include "include/TCoordinate.h"
#include "include/TGrid.h"
#include "include/TSearchAlgorithm.h"
#include "include/TStack.h"

/**
 * @brief Reverses a string using a stack.
 *
 * Using a stack for string reversal works very well because when you add elements in one order they will be taken out in the opposite order.
 * So if you add a,b,c to the stack you will first pop c, then b, and then a.
 * So adding every character in a string to a stack and then popping it and printing it until it's empty will naturally result in the reversing of the string.
 *
 * Time: O(n), Space: O(n)
 *
 * @param in String to reverse
 */
void ReverseString(const char* in) {
    const char* c = in;
    TStack<char> stack;
    while (*c!=0) {
        stack.Push(*c);
        c++;
    }
    while (!stack.IsEmpty()) {
        printf("%c", stack.Pop());
    }
}

/**
 * Time: O(n), Space: O(n)
 * @param n Integer to find factorial of.
 * @return n!
 */
int CalculateFactorial(int n) {
    //printf("%i\n",n);
    TStack<int> stack;
    while (n>1) {
        //printf("%i ",n);
        stack.Push(n);
        --n;
    }
    int r = 1;
    while (!stack.IsEmpty()) {
        r = r * stack.Pop();
        //printf("%i ", r);
    }
    return r;
}

void QueueSimulator() {
    TQueue<int> queue;
    std::cout << "Add 1 to queue\n";
    queue.Enqueue(1);
    std::cout << "Add 2 to queue\n";
    queue.Enqueue(2);
    std::cout << "Add 3 to queue\n";
    queue.Enqueue(3);
    std::cout<<"Remove "<<queue.Dequeue() << " from queue\n";
    std::cout << "Add 4 to queue\n";
    queue.Enqueue(4);
    std::cout << "Add 5 to queue\n";
    queue.Enqueue(5);
    std::cout << "Remove " << queue.Dequeue() << " from queue\n";
}


int main() {
    srand(time(0));

    std::cout << "`Hello World!` in reverse is ";
    ReverseString("Hello World!");

    std::cout<<std::endl<<"The factorial of 5 is " << CalculateFactorial(5) << std::endl;

    QueueSimulator();

    std::cout<<"Press enter to run DFS";
    std::cin.get();
    //DFS
    TSearchAlgorithmBase<TStack<TCoordinate>>();
    /* The algorithm will add the neighbouring nodes of the node it's at to a stack.
    Then it will pop the stack for a node and go to it and repeat.
    The top element of a stack will be the last one added.
    Therefore the top element of the stack will always be one of the neighbouring nodes of the current node.
    This causes the algorithm to go as far as possible in one direction before backtracking. */

    std::cout<<"Press enter to run BFS";
    std::cin.get();
    //BFS
    TSearchAlgorithmBase<TQueue<TCoordinate>>();
    /* The algorithm will add the neighbouring nodes of the node it's at to a queue.
    Then it will dequeue a node and go to it and repeat.
    Because of the first come first serve nature of a queue the first node to be dequeued will be the first one that was added.
    So the nodes that are dequeued will be closer to the starting point than the nodes that are enqueued.
    This causes the serach to focus around the starting point and slowly go outwards.
    */

    /* The visualization of DFS looks like a snake,
    while the visualisation of BFS looks like a diamond.

    DFS will go as deep as possible before backtracking towards the start,
    while BFS will go through all nodes at one level before going down another level.*/

    //std::cout << "\033[101H"<<std::endl;
    return 0;
}