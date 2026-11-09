#pragma once
#include "constants.h"
#include "IStorage.h"

/**
 * @tparam T Data type to store
 */
template <typename T>
class TQueue: public IStorage<T> {
private:
    T storedData[MAX_SIZE] = {0};
    T* front = storedData;
    T* back = storedData; //Back will point to the address after the last element.
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
};
