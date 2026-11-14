#pragma once
/**
* @brief Node for elements stored in the double linked list.
* @see TDLinkedList
*/
template <typename T>
class TDLinkedListNode {
private:
    T content;
    TDLinkedListNode* pointerToNextNode;
    TDLinkedListNode* pointerToPrevNode;

public:
    bool ownsData = true;
    
    TDLinkedListNode(T pointerToContent, TDLinkedListNode* pointerToNextNode = nullptr, TDLinkedListNode* pointerToPreviousNode = nullptr, bool ownsData = true) {
        this->content = std::move(pointerToContent);
        this->pointerToNextNode = pointerToNextNode;
        this->pointerToPrevNode = pointerToPreviousNode;
        this->ownsData = ownsData;
    }

    ~TDLinkedListNode() {
        if (ownsData) {
            delete content;
        }
    }

    TDLinkedListNode* GetNextElement() { return this->pointerToNextNode; }
    void SetNextElementTo(TDLinkedListNode* aTMovie) { this->pointerToNextNode = aTMovie; }
    T GetContent() { return this->content; }
    void SetPreviousElementTo(TDLinkedListNode* aTMovie) { this->pointerToPrevNode = aTMovie; }
    TDLinkedListNode* GetPreviousElement() { return this->pointerToPrevNode; }
};