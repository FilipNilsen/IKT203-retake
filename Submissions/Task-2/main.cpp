#include <iostream>



enum EMovieGenreType {
    ACTION =      0b00001,
    ROMANCE =     0b00010,
    THRILLER =    0b00100,
    COMEDY =      0b01000,
    DOCUMENTARY = 0b10000,
};



class TMovie {
private:
    std::string title;
    std::string director;
    uint16_t year;
    int genre;
    float score;
public:
    TMovie(std::string aTitle, std::string aDirector, uint16_t aYear, uint8_t aGenre, float aScore) {
        this->title = std::move(aTitle);
        this->director = std::move(aDirector);
        this->year = aYear;
        this->score = aScore;
        this->genre = aGenre;
    };
    std::string GetTitle(){return this->title;};
    std::string GetDirector(){return this->director;};
    uint16_t GetYear(){return this->year;};
    int GetGenre(){return this->genre;};
    float GetScore(){return this->score;};
    void Dump() {
        std::cout
        <<"TMovie @ " <<this<<"\n"
        <<title<<"\n"
        <<director<<"\n"
        <<year<<"\n"
        <<genre<<"\n"
        <<score<<std::endl;
    }
   //friend std::ostream& operator<<(std::ostream& os, const TMovie& aTMovie);std::cout<<
};

// std::ostream& operator<<(std::ostream& os, const TMovie& aTMovie) {
//     os
//     <<"TMovie @ " <<&aTMovie<<"\n"
//     <<aTMovie.title<<"\n"
//     <<aTMovie.director<<"\n"
//     <<aTMovie.year<<"\n"
//     <<aTMovie.genre<<"\n"
//     <<aTMovie.score;
//     return os;
// }



class TMovieNode {
private:
    TMovie* pTMovie;
    TMovieNode* pNextTMovieNode;
    TMovieNode* pPreviousTMovieNode;
public:
    TMovieNode(TMovie* aPointerToTMovie, TMovieNode* aPointerToNextTMovieNode=nullptr, TMovieNode* aPointerToPreviousTMovieNode = nullptr) {
        this->pTMovie = std::move(aPointerToTMovie);
        this->pNextTMovieNode = aPointerToNextTMovieNode;
        this->pPreviousTMovieNode = aPointerToPreviousTMovieNode;
    }
    ~TMovieNode() {
        delete this->pTMovie;
    }
    TMovieNode* GetNextElement(){return this->pNextTMovieNode;}
    void SetNextElementTo(TMovieNode* aTMovie) {this->pNextTMovieNode = aTMovie;}
    TMovie* GetContent(){return this->pTMovie;}
    void SetPreviousElementTo(TMovieNode* aTMovie) {this->pPreviousTMovieNode = aTMovie;}
    TMovieNode* GetPreviousElement(){return this->pPreviousTMovieNode;}
    void Dump() {
        std::cout<<"TMovieNode @ "<<this
        <<"\nPrevious node @ "<<pPreviousTMovieNode<<std::endl;
        if (GetContent() != nullptr)
            GetContent()->Dump();
        else if (GetContent()==nullptr)
            std::cout<<"This node points to a nullptr\n";
        std::cout<<"Next node @ "<<GetNextElement()<<"\n\n";
    }
};



typedef bool (*FCheckMovie)(TMovie* aTMovie, void* aSearchCriteria);



class TMovieList {
private:
    TMovieNode* head;
    TMovieNode* tail;
public:
    TMovieList() {
        head = new TMovieNode(nullptr);
        tail = new TMovieNode(nullptr, nullptr, this->head);
        head->SetNextElementTo(this->tail);
    }
    ~TMovieList() {
        TMovieNode* pNext = nullptr;
        do {
            pNext = head->GetNextElement();
            delete head;
            //^Will pNext be deleted??
            head = pNext;
        } while(head != nullptr);
    }
    void Append(TMovie* aMovie) {
        // //Go to last element
        // TMovieNode* tail = head;
        // while (tail->GetNextElement() != nullptr) {
        //     tail = tail->GetNextElement();
        // }
        //
        // auto newTail = new TMovieNode(aMovie);
        //
        // tail->SetNextElementTo(newTail);
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
        //Refactoring makes this O(1) instead of O(n).
        auto newElement = new TMovieNode(aMovie, tail);
        newElement->SetPreviousElementTo(tail->GetPreviousElement());
        tail->SetPreviousElementTo(newElement);
        newElement->GetPreviousElement()->SetNextElementTo(newElement);
    }
    void Prepend(TMovie* aTMovie) {
        // auto currentFirstElement = head->GetNextElement();
        // auto newFirstElement = new TMovieNode(aTMovie, currentFirstElement);
        // head->SetNextElementTo(newFirstElement);

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
        auto newElement= new TMovieNode(aTMovie, head->GetNextElement(), head);
        head->SetNextElementTo(newElement);
        newElement->GetNextElement()->SetPreviousElementTo(newElement);
    }
    TMovie* GetAtIndex(size_t aIndex) {
        auto a = GetTmovieNodeAtIndex(aIndex);
        if (a==nullptr)
            return nullptr;
        else
            return a->GetContent();
    }
    TMovieNode* GetTmovieNodeAtIndex(size_t aIndex) {
        auto currentIndex = this->head->GetNextElement();
        for (size_t i = 0; i < aIndex; ++i) {
            if (currentIndex->GetContent() == nullptr)
                return nullptr;
            currentIndex = currentIndex->GetNextElement();
        }
        return currentIndex;
    }
    void Remove(size_t aIndex) {
        auto toDelete = GetTmovieNodeAtIndex(aIndex);
        toDelete->GetPreviousElement()->SetNextElementTo(toDelete->GetNextElement());
        toDelete->GetNextElement()->SetPreviousElementTo(toDelete->GetPreviousElement());
        delete toDelete;
    }
    void Dump() {
        std::cout
        <<"TMovieList @ "<<this<<std::endl;
        auto nodeToDump = head;
        do {
            nodeToDump->Dump();
            nodeToDump = nodeToDump->GetNextElement();
        } while (nodeToDump != nullptr);
    }
    TMovie* SearchFor(FCheckMovie aSearchFunction, void* aSearchCriteria) {
        auto currentIndex = this->head;
        bool a;
        do {
            currentIndex = currentIndex->GetNextElement();
            if (currentIndex->GetContent()==nullptr)
                return nullptr;
            a = aSearchFunction(currentIndex->GetContent(), aSearchCriteria);
        } while(!a);
        return currentIndex->GetContent();
    }
};

namespace SearchBy {
    bool Title(TMovie* aTMovie, void* aTitle) {
        char* aTitleCharArray = static_cast<char *>(aTitle);
        auto aTitleString = std::string(aTitleCharArray);
        //std::string aTitleStr = *aTitle;
        if (aTMovie->GetTitle() == aTitleString)
            return true;
        else
            return false;
    }

    bool Director(TMovie* aTMovie, void* aName) {
        char* aNameCharArray = static_cast<char*>(aName);
        auto aNameString = std::string(aNameCharArray);
        if (aTMovie->GetDirector() == aNameString)
            return true;
        else
            return false;
    }

    //Exact match only.
    bool Genre(TMovie* aTmovie, void* aGenre) {
        int* aGenreInt = (int*) aGenre;
        if (aTmovie->GetGenre() == *aGenreInt)
            return true;
        else
            return false;
    }
}

int main() {
    auto list = new TMovieList();
    list->Append(new TMovie("1293jh41932g12nd.webm", "Unknown Director", 2016, COMEDY, 100.0));
    list->Append(new TMovie("Alien", "Ridley Scott", 1979, THRILLER, 80.0));
    list->Prepend(new TMovie("Love Actually", "Richard Curtis", 2003, ROMANCE|COMEDY, 100.0));

    std::cout<<"==================================================\nList with three movies\n";
    list->Dump();

    std::cout<<"==================================================\nAt index 1 we have:\n";
    list->GetAtIndex(1)->Dump();

    std::cout<<"==================================================\nFind alien\n";
    char a[]="Alien";
    list->SearchFor(&SearchBy::Title, &a)->Dump();

    std::cout<<"==================================================\nFind film directed by Ridley Scott\n";
    char b[]="Ridley Scott";
    list->SearchFor(&SearchBy::Director, &b)->Dump();

    std::cout<<"==================================================\nFind comedy\n";
    int c = COMEDY;
    list->SearchFor(&SearchBy::Genre, &c)->Dump();

    std::cout<<"==================================================\nRemove at index 1\n";
    list->Remove(1);
    list->Dump();
    return 0;
}
