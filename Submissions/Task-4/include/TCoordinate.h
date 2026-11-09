#pragma once
class TCoordinate {
private:
    int _x=0;
    int _y=0;
public:
    TCoordinate(int x=0, int y=0): _x(x), _y(y){}
    int GetX() const { return _x; }
    int GetY() const { return _y; }
    void SetX(int x){ this->_x = x; }
    void SetY(int y){ this->_y = y; }
    TCoordinate Above() const {return TCoordinate(GetX()-1, GetY());}
    TCoordinate Below() const {return TCoordinate(GetX()+1, GetY());}
    TCoordinate Left() const {return TCoordinate(GetX(), GetY()-1);}
    TCoordinate Right() const {return TCoordinate(GetX(), GetY()+1);}
    void Display() const {
        std::cout<<"("<<GetX()<<", "<<GetY()<<")"<<std::endl;
    }
};

std::ostream& operator<<(std::ostream& os, const TCoordinate& coordinate) {
    os<<"("<<coordinate.GetX()<<", "<<coordinate.GetY()<<")";
    return os;
}