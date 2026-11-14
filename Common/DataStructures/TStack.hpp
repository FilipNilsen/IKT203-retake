#pragma once
#include "TCollection.hpp" // This is your base class for collections
#include "TLinkedList.hpp" // This is your linked list implementation
#include "DebugUtils.h" // This is the provided debug utility for logging
#include <stdexcept> // This is for throwing exceptions when needed
#include <iostream>

/// @brief A stack.
/// @tparam T Type to store on stack.
/// @tparam MAX_SIZE Size of the stack in number of T-elements.
/// @pre T is a pointer type.
template <typename T, int MAX_SIZE=100>
class TStack {
private:
    T storedData[MAX_SIZE];
    T* top = storedData;  //Top points to the top element in the stack.

public:
    /**
     * Time: O(1), Space: O(1).
     * @param item Item to add to TStack.
     * @return false if the TStack is full.
     */
    bool Push(T item) {
        if (top >= storedData + MAX_SIZE) {
            std::cerr<<"Stack is full"<<std::endl;
            return false;
        }
        *top = item;
        ++top;
        return true;
    }

    bool Add(T item) {
        return Push(item);
    }

    /**
     * Time: O(1), Space: O(1).
     * @return Item that was popped from the stack. Or nullptr if it's empty.
     */
    T Pop() {
        if (this->IsEmpty()) {
            return nullptr;
        }
        --top;
        T return_value = *top;
        return return_value;
    }

    T Take() {
        return Pop();
    }

    /// @brief Take a look at the item at the top of the stack.
    /// @return The item at the top, or nullptr if the stack is empty.
    T Peek() const {
        if (this->IsEmpty()) {
            return nullptr;
        }
        return *(top-1);
    }

    bool IsEmpty() const {
        if (top <= storedData) {
            return true;
        }
        if (top > storedData) {
            return false;
        }
        else
            return true;
    }

    void Dump() const {
        const T* i = storedData;
        while (i < top) {
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
        if constexpr (std::is_pointer_v<T>) {
            while (!IsEmpty()) {
                delete Pop();
            }
        }
    }
};
