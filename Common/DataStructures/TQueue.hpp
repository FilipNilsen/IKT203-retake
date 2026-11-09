#pragma once
#include "TCollection.hpp" // This is your base class for collections
#include "TLinkedList.hpp" // This is your linked list implementation
#include "DebugUtils.h" // This is the provided debug utility for logging
#include <stdexcept> // This is for throwing exceptions when needed

/**
* @file   TQueue.hpp
* @brief  Dear student, this file contains the definition of the TQueue class template, which serves as a queue implementation for storing elements of type T.
*         Be aware that this queue is implemented using a linked list, and it does not own the data it stores.
*         You will create this as a template class typically holding methods for enqueueing, dequeueing, peeking, and checking the size of the queue.
*         Please ask for help in Discord if you have any questions or need assistance with the implementation.
*/

#pragma once
#include "constants.h"
#include "IStorage.h"

/**
 * A queue
 * @param T Data type to store. Must be a pointer.
 */
template <typename T>
class TQueue: public IStorage<T> {
private:
    T storedData[MAX_SIZE] = {0};
    T* front = storedData;
    T* back = storedData; //!< Back will point to the address after the last element.
    uint16_t numberOfElements = 0;

public:
    /**
     * Time: O(1), Space: O(1)
     * @param item Item to enqueue
     * @return Whether it failed or succeeded
     */
    bool Enqueue(T item) {
        //Wraparound
        if (back >= storedData + MAX_SIZE) {
            back = storedData;
        }
        if (numberOfElements>=MAX_SIZE) {
            std::cerr<<"Queue is full."<<std::endl;
            return false;
        }
        *back = item;
        ++back;
        ++numberOfElements;
        return true;
    }
    bool Add(T item) override{
        return Enqueue(item);
    }

    /**
     * Time: O(1), Space: O(1)
     * @return Element that was dequeued
     */
    T Dequeue() {
        //Wraparound
        if (front >= storedData + MAX_SIZE) {
            front = storedData;
        }
        T return_value = *front;
        ++front;
        --numberOfElements;
        return return_value;
    }
    T Take() {
        return Dequeue();
    }

    T Peek() const override {
        return *front;
    }

    bool IsEmpty() const override {
        if (numberOfElements<=0) {
            return true;
        }
        else if (numberOfElements>0) {
            return false;
        }
        return false;
    }

    /**
    Print out all elements in queue to the terminal.
    Uses `<<` so overloading `<<` for the object you're storing can make this more readable. */
    void Dump() const override{
        const T* i = storedData;
        while (i <= &storedData[MAX_SIZE-1]) {
            //printf("%i ", *i);
            if (i == front)
                std::cout<<" \033[32mfront* ->\033[0m ";
            std::cout<<*i<<" ";
            if (i == back)
                std::cout<<" \033[31m<- back*\033[0m " ;
            //std::cout << "\033[210G" << *i << std::endl;
            ++i;
        }
        //printf("\n");
        std::cout<<std::endl;
    }

    /**
    @note Deletes every item currently in the queue if T is a pointer type.
    */
    ~TQueue() {
        if (std::is_pointer<T>::value) {
            while (!IsEmpty()) {
                delete Dequeue();
            }
        }
    }
};
