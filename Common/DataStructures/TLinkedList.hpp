#pragma once
#include <stdexcept> // For std::runtime_error
#include <string>    // For std::to_string
#include <functional> // For std::function
#include "../DebugUtils.h" // For debugText function
#include "TCollection.hpp" // Your base collection class
#include "TNode.hpp" // Your node class

/**
@brief Double linked list
@author Filip
*/
template <typename T>
class TDLinkedList {
private:
    TDLinkedListNode<T>* head;
    TDLinkedListNode<T>* tail;
    bool ownsData = true;

public:
    auto GetHead() { return this->head; }
    auto GetTail() { return this->tail; }

    TDLinkedList(bool ownsData = true) {
        head = new TDLinkedListNode<T>(nullptr);
        tail = new TDLinkedListNode<T>(nullptr, nullptr, this->head);
        head->SetNextElementTo(this->tail);
        this->ownsData = ownsData;
    }

    ~TDLinkedList() {
        TDLinkedListNode<T>* pNext = nullptr;
        do {
            pNext = head->GetNextElement();
            delete head;
            head = pNext;
        } while (head != nullptr);
    }

    void Append(T aMovie) {
        /*
         ┌────────┐                                          ┌────────┐
         │-1:     │                           3              │tail:   │
         │previous│<──────────────────────────x──────────────│previous│
         │next    │─────────────────x─────────┼─────────────>│next    │
         └────────┘                4│         │              └────────┘
                 ∧                  │         │               ∧
                 │                  ∨         ∨               │
                 │                 ┌───────────┐              │
                 │                2│newElement:│              │
                 └─────────────────│previous   │1             │
                                   │next       │──────────────┘
                                   └───────────┘
        1. Create newElement with next=tail
        2. Set newElement.previous to the same as tail.previous
        3. Set tail.previous to newElement
        4. Set newElement.previous.next to newElement
        */

        auto newElement = new TDLinkedListNode<T>(aMovie, tail);
        newElement->ownsData = this->ownsData;
        newElement->SetPreviousElementTo(tail->GetPreviousElement());
        tail->SetPreviousElementTo(newElement);
        newElement->GetPreviousElement()->SetNextElementTo(newElement);
    }

    void Prepend(T aTMovie) {
        /*
        ┌╶╶╶╶╶╶╶╶┐                                                  ┌╶╶╶╶╶╶╶╶┐
        ╎head:   ╎                    2                             ╎0:      ╎
        ╎next    ╎╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶x╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶>╎next    ╎
        ╎previous╎<╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶┼╶╶╶╶╶╶╶╶╶x╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╎previous╎
        └╶╶╶╶╶╶╶╶┘                    ╎         ╎3                  └╶╶╶╶╶╶╶╶┘
            ∧                         ╎         ╎                       ∧
            ╎                         ∨         ∨                       ╎
            ╎                        ┌╶╶╶╶╶╶╶╶╶╶╶┐                      ╎
            ╎                        ╎newElement:╎1                     ╎
            ╎                       1╎next       ╎╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶┘
            └╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╶╎previous   ╎
                                     └╶╶╶╶╶╶╶╶╶╶╶┘
        1. Create newElement with previous=head and next=head.next
        2. head.next=newElement
        3. newElement.next.previous=newElement
         */

        auto newElement = new TDLinkedListNode<T>(aTMovie, head->GetNextElement(), head, this->ownsData);
        head->SetNextElementTo(newElement);
        newElement->GetNextElement()->SetPreviousElementTo(newElement);
    }

    void InsertAfter(size_t anIndex, T aNewElement) {
        // A x B insert x inbetween A and B.
        auto A = GetNodeAtIndex(anIndex);
        if (A==nullptr) {
            throw std::out_of_range("");
        }
        auto B = A->GetNextElement();
        auto x = new TDLinkedListNode<T>(aNewElement, B, A, this->ownsData);
        A->SetNextElementTo(x);  
        B->SetPreviousElementTo(x);
    }

    void InsertBefore(size_t anIndex, T aNewElement) {
        // A x B insert x.
        auto B = GetNodeAtIndex(anIndex);
        if (B==nullptr) {
            throw std::out_of_range("");
        }
        auto A = B->GetPreviousElement();
        auto x = new TDLinkedListNode<T>(aNewElement, B, A, this->ownsData);
        B->SetPreviousElementTo(x);
        A->SetNextElementTo(x);
    }

    /**
     * @returns The content of the node at aIndex.
     */
    T GetAtIndex(size_t aIndex) {
        auto a = GetNodeAtIndex(aIndex);
        if (a == nullptr)
            return nullptr;
        else
            return a->GetContent();
    }

    /**
     * @returns The TDLinkedListNode<T>* at aIndex, or nullptr if out of range.
     */
    TDLinkedListNode<T>* GetNodeAtIndex(size_t aIndex) {
        auto currentIndex = this->head->GetNextElement();
        for (size_t i = 0; i < aIndex; ++i) {
            if (currentIndex->GetContent() == nullptr)
                return nullptr;
            currentIndex = currentIndex->GetNextElement();
        }
        if (currentIndex == GetTail()) {
            return nullptr;
        }
        return currentIndex;
    }

    bool Remove(size_t aIndex) {
        auto toDelete = GetNodeAtIndex(aIndex);
        if (toDelete == nullptr) {
            return false;
        }
        toDelete->GetPreviousElement()->SetNextElementTo(toDelete->GetNextElement());
        toDelete->GetNextElement()->SetPreviousElementTo(toDelete->GetPreviousElement());
        delete toDelete;
        return true;
    }

    /** @brief  Removes T at `aIndex` from this TDlinkedList and returns it. Ownership of the data at aIndex is removed from the list.
     * @param[in] aIndex
     * @returns The T at `aIndex`. You take ownership of this, otherwise it will be dangling.
     */
    T GetAndRemoveAtIndex(size_t aIndex){
        auto toDelete = GetNodeAtIndex(aIndex);
        if (toDelete==nullptr) {
            throw std::out_of_range("");
        }
        toDelete->GetPreviousElement()->SetNextElementTo(toDelete->GetNextElement());
        toDelete->GetNextElement()->SetPreviousElementTo(toDelete->GetPreviousElement());
        auto content = toDelete->GetContent();
        toDelete->ownsData = false;
        delete toDelete;
        return content;
    }

    bool RemoveLast() {
        auto toDelete = tail->GetPreviousElement();
        if (toDelete != head) {
            toDelete->GetPreviousElement()->SetNextElementTo(tail);
            toDelete->GetNextElement()->SetPreviousElementTo(toDelete->GetPreviousElement());
            delete toDelete;
            return true;
        }
        return false;
    }

    unsigned int Length() {
        auto i = GetHead();
        int counter = 0;
        while (true) {
            i = i->GetNextElement();
            if (i == GetTail()) {
                return counter;
            }
            ++counter;
        }
    }
};