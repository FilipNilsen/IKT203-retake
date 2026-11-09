#pragma once
#include "TCollection.hpp" // This is your base class for collections
#include "TLinkedList.hpp" // This is your linked list implementation
#include "DebugUtils.h" // This is the provided debug utility for logging
#include <stdexcept> // This is for throwing exceptions when needed


/**
* @file   TStack.hpp
* @brief  Dear student, this file contains the definition of the TStack class template, which serves as a stack implementation for storing elements of type T.
*         Be aware that this stack is implemented using a linked list, and it does not own the data it stores.
*         You will create this as a template class typically holding methods for pushing, popping, peeking, and checking the size of the stack.
*         Please ask for help in Discord if you have any questions or need assistance with the implementation.
*/

#pragma once
#include "IStorage.h"

template <typename T>
class TStack: public IStorage<T> {
private:
    T storedData[MAX_SIZE];
    T* top = storedData-1;  //Top points to the top element in the stack.

public:
    /**
     * Time: O(1), Space: O(1)
     * @param item Item to add to TStack
     * @return false if the TStack is full
     */
    bool Push(T item) {
        if (top >= storedData + MAX_SIZE-1) {
            std::cerr<<"Stack is full"<<std::endl;
            return false;
        }
        ++top;
        *top = item;
        return true;
    }
    bool Add(T item) override {
        return Push(item);
    }

    /**
     * Time: O(1), Space: O(1)
     * @return Item that was popped from the stack
     */
    T Pop() {
        T return_value = *top;
        --top;
        return return_value;
    }
    T Take() override {
        return Pop();
    }

    T Peek() const override {
        return *top;
    }

    bool IsEmpty() const override {
        if (top < storedData) {
            return true;
        }
        if (top >= storedData) {
            return false;
        }
        else
            return true;
    }

    void Dump() const override {
        const T* i = storedData;
        while (i <= top) {
            //printf("%i ", *i);
            std::cout<<*i<<" ";
            //std::cout << "\033[210G" << *i << std::endl;
            ++i;
        }
        //printf("\n");
        std::cout<<std::endl;
    }

    /**
    @note Deletes every item currently in the stack if T is a pointer type.
    */
    ~TStack() {
        if (std::is_pointer<T>()) {
            while (!IsEmpty()) {
                delete Pop();
            }
        }
    }
};
