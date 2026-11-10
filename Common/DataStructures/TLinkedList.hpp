#pragma once
#include <stdexcept> // For std::runtime_error
#include <string>    // For std::to_string
#include <functional> // For std::function
#include "../DebugUtils.h" // For debugText function
#include "TCollection.hpp" // Your base collection class
#include "TNode.hpp" // Your node class

/**
* @file   TLinkedList.hpp
* @brief  Dear student, this file contains the definition of the TLinkedList class template, which serves as a doubly-linked list implementation for storing elements of type T.
*					You will create this as a template class typically holding methods for appending, prepending, searching, and iterating over elements.
*					Type of linked list is op to you, you must clean (delete) your data yourself so be wize about the type of T you choose to use.
*         I recommend to implenet a "isOwner" boolean variable to determine if the list is responsible for deleting the data or not.
*					Please ask for help in Discord if you have any questions or need assistance with the implementation.
* @tparam T The type of elements stored in the linked list.
*/


/**
@brief Double linked list

The text editor is highly coupeled to this class.
While writing this I also had issues with templates so i wrote this with high coupling to RefCounted.

Some functions return RefCounted*, others return a TDLinkedListNode*, while others return the data reference counted by RefCounted.
The last case is problematic because you can't then do any modifications to the reference count.

@todo Rewrite with templating and decouple from RefCounted.
@todo Undeprecate ownsData. I'm planning to write a RefCounted wrapper class whoose destructor calls RefCounted::Dec() anyway.
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
            //^Will pNext be deleted??
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

        auto newElement = new TDLinkedListNode<T>(aTMovie, head->GetNextElement(), head, ownsData);
        head->SetNextElementTo(newElement);
        newElement->GetNextElement()->SetPreviousElementTo(newElement);
    }

    void InsertAfter(size_t anIndex, T aNewElement) {
        //Started as copy of function above
        auto after = GetNodeAtIndex(anIndex);
        auto newElement = new TDLinkedListNode<T>(aNewElement, after->GetNextElement(), after, ownsData);
        newElement->GetPreviousElement()->SetNextElementTo(newElement);  
        after->SetPreviousElementTo(newElement);
    }

    void InsertBefore(size_t anIndex, T aNewElement) {
        //Started as copy of function above
        auto before = GetNodeAtIndex(anIndex);
        auto newElement = new TDLinkedListNode<T>(aNewElement, before, before->GetPreviousElement(), ownsData);
        before->SetPreviousElementTo(newElement);
        newElement->GetPreviousElement()->SetNextElementTo(newElement);
    }

    /**
     * @returns The content of the RefCount object at `aIndex`
     */
    void* GetAtIndex(size_t aIndex) {
        auto a = GetNodeAtIndex(aIndex);
        if (a == nullptr)
            return nullptr;
        else
            return a->GetContent();
    }

    /**
     * @returns The TDLinkedListNode<T>* at `aIndex`
     */
    TDLinkedListNode<T>* GetNodeAtIndex(size_t aIndex) {
        auto currentIndex = this->head->GetNextElement();
        for (size_t i = 0; i < aIndex; ++i) {
            if (currentIndex->GetContent() == nullptr)
                return nullptr;
            currentIndex = currentIndex->GetNextElement();
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

    /** @brief  Removes RefCounted* at `aIndex` from this TDlinkedList and returns it.
     * 
     * @returns The RefCounted* at `aIndex`
     */
    T GetAndRemoveAtIndex(size_t aIndex){
        auto toDelete = GetNodeAtIndex(aIndex);
        toDelete->GetPreviousElement()->SetNextElementTo(toDelete->GetNextElement());
        toDelete->GetNextElement()->SetPreviousElementTo(toDelete->GetPreviousElement());
        auto content = toDelete->GetContent();
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
};