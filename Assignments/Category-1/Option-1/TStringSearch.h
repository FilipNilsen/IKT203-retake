#pragma once
#include <stdexcept>

template <typename T>
class TStringSearchNode {
    private: TStringSearchNode* data['~'-' ' + 1] = {0}; //!< ASCII printable range long array of T.
    private: T end = nullptr;

    private: bool InRange(char c) {
        if (c == 0) {
            return true;
        }
        else if (c < ' ' || c > '~') {
            throw std::out_of_range("");
            return false;
        }
        else {
            return true;
        }
    }

    public: T Get(const char* p_c) {
        InRange(*p_c);
        if (*p_c == 0) {
            if (end == nullptr) {
                throw std::out_of_range("");
            }
            return end;
        }
        else {
            char c = *p_c;
            c -= ' ';
            p_c++;
            if (data[c] == nullptr) {
                throw std::out_of_range("");
            }
            return data[c]->Get(p_c);
        }
    }

    public: void Insert(T end, const char* p_c) {
        InRange(*p_c);
        if (*p_c == 0) {
            this->end = end;
        }
        else {
            char c = *p_c;
            c -= ' ';
            if (data[c] == nullptr) {
                data[c] = new TStringSearchNode();
            }
            p_c++;
            data[c]->Insert(end, p_c);
        }
    }
};

template <typename T>
class TStringSearch {
    private: TStringSearchNode<T>* root;
    public: TStringSearch() {
        root = new TStringSearchNode<T>();
    }
    public: T Get(const char* p_c) {
        return root->Get(p_c);
    }
    public: void Insert(T end, const char* p_c) {
        root->Insert(end, p_c);
    }
};