#pragma once

template <typename T>
class IStorage{
    virtual bool Add(T item)=0;
    virtual T Take()=0;
    virtual T Peek() const =0;
    virtual bool IsEmpty() const =0;
    virtual void Dump() const =0;
};