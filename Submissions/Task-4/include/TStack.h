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
};
