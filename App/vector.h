#pragma once
#include "stdint.h"
// namespace Simple{
template<typename T>
class vector{
private:
    T* head;
    T* tail;
    T* cap;
public:
    vector() :head(nullptr),tail(nullptr),cap(nullptr){}
    ~vector(){delete [] head;}
    vector(const vector& other) {
        size_t n = other.size();
        if (n == 0) {
            head = tail = cap = nullptr;
        } else {
            head = new T[n];
            for (size_t i = 0; i < n; ++i)
                head[i] = other.head[i];
            tail = cap = head + n;
        }
    }
    vector& operator=(const vector& other) {
        if (this != &other) {
            delete[] head;
            size_t n = other.size();
            if (n == 0) {
                head = tail = cap = nullptr;
            } else {
                head = new T[n];
                for (size_t i = 0; i < n; ++i)
                    head[i] = other.head[i];
                tail = cap = head + n;
            }
        }
        return *this;
    }
    void clear(){tail=head;}
    void push_back(const T& data){
        if(tail==cap){
            size_t old_c=cap-head;
            size_t new_c=(old_c==0)?1:old_c*2;
            T* new_head=new T[new_c];
            for(size_t i=0;i<old_c;++i){
                new_head[i]=head[i];
            }
            delete [] head;
            head=new_head;
            tail=head+old_c;
            cap=head+new_c;
        }
        *tail=data;
        ++tail;
    }
    size_t size() const {
        return tail - head;
    }
    size_t capacity() const {
        return cap - head;
    }
    T& operator[](size_t index){
        return head[index];
    }
};
// }