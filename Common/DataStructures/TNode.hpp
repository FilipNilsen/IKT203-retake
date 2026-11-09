/**
* @file TNode.hpp
* @brief  Dear student, this file contains the definition of the TNode class template, which serves as a node in a doubly-linked list implementation for storing elements of type T.
*					You will create this as a template class typically holding data of type T and pointers to other nodes in the list. (you must choose if this is singel or double linked list)
*					Please ask for help in Discord if you have any questions or need assistance with the implementation.
*/


/**
@brief Node for elements stored in the double linked list.
@see TDLinkedList
*/
class TDLinkedListNode {
private:
    RefCounted<>* content;
    TDLinkedListNode* pointerToNextNode;
    TDLinkedListNode* pointerToPrevNode;
    bool ownsData = true;

public:
    TDLinkedListNode(RefCounted<>* pointerToContent, TDLinkedListNode* pointerToNextNode = nullptr, TDLinkedListNode* pointerToPreviousNode = nullptr, bool ownsData = true) {
        this->content = std::move(pointerToContent);
        if (this->content != nullptr) {
            this->content->Inc();
        }
        this->pointerToNextNode = pointerToNextNode;
        this->pointerToPrevNode = pointerToPreviousNode;
        this->ownsData = ownsData;
    }

    ~TDLinkedListNode() {
        if (this->content != nullptr)
            content->Dec();
    }

    TDLinkedListNode* GetNextElement() { return this->pointerToNextNode; }
    void SetNextElementTo(TDLinkedListNode* aTMovie) { this->pointerToNextNode = aTMovie; }
    RefCounted<>* GetContent() { return this->content; }
    void SetPreviousElementTo(TDLinkedListNode* aTMovie) { this->pointerToPrevNode = aTMovie; }
    TDLinkedListNode* GetPreviousElement() { return this->pointerToPrevNode; }
};