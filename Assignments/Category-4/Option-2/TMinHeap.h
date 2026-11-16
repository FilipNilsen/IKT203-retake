#pragma once
#include <cstdint>
#include <cstring>
#include <cstddef>

/// @brief An array based min-heap.
/// @pre T is a pointer type.
/// @note There is no destructor because current estimated lifetime == application lifetime.
template <typename T>
class TMinHeap {
    private: T* heap = nullptr;
    private: size_t heap_size = 0;
    private: size_t heap_elements = 0;
    
    public: TMinHeap()
    {

    }

    /// @brief Find the parents index of element at index i of heap.
    /// @warning Can return negative indexes.
    private: static int64_t Parent(int64_t i)
    {
        return (i - 1) / 2;
    }

    /// @brief Find left childs index of parent element at index i of heap.
    /// @warning Can return an out of bounds index.
    private: static size_t Left(size_t i)
    {
        return 2 * i + 1;
    }

    /// @brief Find right childs index of parent element at index i of heap.
    /// @warning Can return an out of bounds index.
    private: static size_t Right(size_t i)
    {
        return 2 * i + 2;
    }

    private: void SiftUp(size_t i)
    {
        auto p = this->Parent(i);
        
        if (p < 0)
        {
            return;
        }

        if (*heap[p] > *heap[i])
        {
            auto tmp = heap[p];
            heap[p] = heap[i];
            heap[i] = tmp;
            SiftUp(p);
        }
    }

    private: void SiftDown(size_t i)
    {
        size_t smallest = i;
        size_t left = Left(i);
        size_t right = Right(i);

        // Check if left child exists and is smaller than current smallest
        if (left < heap_elements && *heap[left] < *heap[smallest]) {
            smallest = left;
        }

        // Check if right child exists and is smaller than current smallest
        if (right < heap_elements && *heap[right] < *heap[smallest]) {
            smallest = right;
        }

        // If a child was smaller, swap and keep sifting down
        if (smallest != i) {
            auto tmp = heap[i];
            heap[i] = heap[smallest];
            heap[smallest] = tmp;
            SiftDown(smallest);
        }
    }

    public: void Insert(T new_element)
    {
        if (new_element == nullptr)
            return;
            
        ++heap_elements;
        if (heap_elements > heap_size)
        {
            if (heap_size == 0)
                heap_size = 1;
            heap_size *= 2;
            auto new_heap = new T[heap_size];
            if (heap != nullptr)
            {
                memcpy(new_heap, heap, (heap_elements -1 ) * sizeof(T));
                delete[] heap;
            }
            heap = new_heap;
        }
        heap[heap_elements - 1] = new_element;
        SiftUp(heap_elements - 1);
    }

    public: T Get()
    {
        if (heap_elements <= 0)
            return nullptr;
        --heap_elements;
        auto r = heap[0];

        if (heap_elements > 0)
        {
            heap[0] = heap[heap_elements];
            heap[heap_elements] = nullptr;
            SiftDown(0);
        }
        else
        {
            heap[0] = nullptr;
        }
        return r;
    }
};