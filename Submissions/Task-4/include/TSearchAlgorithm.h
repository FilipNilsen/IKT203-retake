#pragma once
#include "TCoordinate.h"
#include "TStack.h"

template <typename T>
class TSearchAlgorithmBase {
protected:
    TCoordinate pos;
    TGrid grid;
    bool inVisitList[100][100]={false};
    bool visited[100][100]={false};
    T storage;

    void CallibrateScreen() {
        for (size_t i = 0; i <= MAX_SIZE; ++i) {
            std::cout<< "\n" << i ;
        }
        printf(" Zoom out so that you can see all the numbers form 1 to %i.", MAX_SIZE);
        std::cin.get();
    }

    bool IsVisited() {
        return IsVisited(pos);
    }
    bool IsVisited(TCoordinate pos) {
        if (inVisitList[pos.GetX()][pos.GetY()]==true) {
            return true;
        }
        return false;
    }

    bool IsZero() {
        return IsZero(pos);
    }
    bool IsZero(TCoordinate pos) const {
        if (grid.Get(pos)==0)
            return true;
        else if (grid.Get(pos)!=0)
            return false;
        else
            return false;
    }

    bool IsInside() {
        return IsInside(pos);
    }
    bool IsInside(TCoordinate pos) const {
        if (pos.GetX()<0)
            return false;
        else if (pos.GetY()<0)
            return false;
        else if (pos.GetX()>=100)
            return false;
        else if (pos.GetY()>=100)
            return false;
        else
            return true;
    }
    int8_t Add(TCoordinate coordinate) {
        if (!IsInside(coordinate)) {
            return -1;
        }
        if (IsVisited(coordinate)) {
            return -2;
        }
        if (!storage.Add(coordinate)) {
            return -3;
        }
        inVisitList[coordinate.GetX()][coordinate.GetY()]=true;
        return 0;
    }
    void VisalizeVisited() {
        for (auto &i : visited) {
            for (bool &j : i) {
                if(j) {
                    std::cout<<"X";
                }
                else if (!j) {
                    std::cout<<" ";
                }
            }
            std::cout<<"\n";
        }
        std::cout.flush();
    }
public:
    /**
     * Time: O(n), Space: o(n)
     */
    TSearchAlgorithmBase() : pos((rand() % 99) , (rand() % 99)) {
        //CallibrateScreen();
        //printf("\033[2J\033[100U");
        //grid.Dump();
        while (!IsZero()) {
            std::cout<<pos<<" = "<<grid.Get(pos)<<std::endl;
            visited[pos.GetX()][pos.GetY()]=true;
            //storage.Dump();
            //std::cout<<std::endl<<std::endl;
            //printf("\033[%i;%iH\033[104m%i\033[0m ",pos.GetX()+1,pos.GetY()*2+1,grid.Get(pos));
            Add(pos.Above());
            Add(pos.Right());
            Add(pos.Below());
            if (Add(pos.Left()) == -3) {
                //std::cerr<<"Ran out of stack space."<<std::endl;
                VisalizeVisited();
                return;
            }

            pos = storage.Take();

            storage.Dump();
            std::cout<<std::endl;
        }
        if (IsZero()) {
            //printf("\033[%i;%iH\033[105m%i\033[0m ", pos.GetX() + 1, pos.GetY()*2 + 1, grid.Get(pos));
            std::cout<<pos<<" = "<<grid.Get(pos)<<std::endl;
        }
        VisalizeVisited();
    }
};


/*
class TDfs : public TSearchAlgorithmBase {
private:
    TStack<TCoordinate> stack;
    int8_t Push(TCoordinate coordinate) {
        if (!IsInside(coordinate)) {
            return -1;
        }
        if (IsVisited(coordinate)) {
            return -2;
        }
        if (!stack.Push(coordinate)) {
            std::cerr<<"Ran out of stack space!"<<std::endl;
            return -3;
        }
        added_to_visit_list[coordinate.GetX()][coordinate.GetY()]=true;
        return 0;
    }
public:
    TDfs(): TSearchAlgorithmBase() {
        //CallibrateScreen();
        //printf("\033[2J\033[100U");
        //grid.Dump();
        while (!IsZero()) {
            if (IsInside()) {
                std::cout<<pos<<" = "<<grid.Get(pos)<<std::endl;
                stack.Dump();
                std::cout<<std::endl<<std::endl;
                //printf("\033[%i;%iH\033[104m%i\033[0m ",pos.GetX()+1,pos.GetY()*2+1,grid.Get(pos));
                Push(pos.Above());
                Push(pos.Right());
                Push(pos.Below());
                if (Push(pos.Left()) == -3) {
                    //std::cerr<<"Ran out of stack space."<<std::endl;
                    return;
                }

                do {
                    pos = stack.Pop();
                } while (!IsInside());
            }
        }
        if (IsZero()) {
            //printf("\033[%i;%iH\033[105m%i\033[0m ", pos.GetX() + 1, pos.GetY()*2 + 1, grid.Get(pos));
            std::cout<<pos<<" = "<<grid.Get(pos)<<std::endl;
        }
    }
};



class TBfs : public TSearchAlgorithmBase {
private:
    TQueue<TCoordinate> queue;
    int8_t Enqueue(TCoordinate coordinate) {
        if (!IsInside(coordinate)) {
            return -1;
        }
        if (IsVisited(coordinate)) {
            return -2;
        }
        if (!queue.Enqueue(coordinate)) {
            std::cerr<<"Ran out of queue space!"<<std::endl;
            return -3;
        }
        added_to_visit_list[coordinate.GetX()][coordinate.GetY()]=true;
        return 0;
    }
public:
    TBfs() : TSearchAlgorithmBase() {
        //printf("\033[2J\033[100U");
        //grid.Dump();
        while (!IsZero()) {
            if (IsInside()) {
                //printf("\033[%i;%iH\033[104m%i\033[0m ", pos.GetX() + 1, pos.GetY() * 2 + 1, grid.Get(pos));
                std::cout << pos << " = " << grid.Get(pos) << std::endl;
                Enqueue(pos.Above());
                Enqueue(pos.Right());
                Enqueue(pos.Below());
                if (Enqueue(pos.Left()) == -3) {
                    return;
                }
                queue.Dump();
                std::cout<<std::endl<<std::endl;

                do {
                    pos = queue.Dequeue();
                } while (!IsInside());
            }
        }
        if (IsZero()) {
            //printf("\033[%i;%iH\033[105m%i\033[0m ", pos.GetX() + 1, pos.GetY() * 2 + 1, grid.Get(pos));
            std::cout << pos << " = " << grid.Get(pos) << std::endl;
            return;
        }
    }
};
*/