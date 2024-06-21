#ifndef SELFORGL_H
#define SELFORGL_H

#include <list>
#include <map> //should be unordered but c++11

template <typename T>
class self_organizing_list {
protected:
    std::list<T>& lst;
public:
    self_organizing_list(std::list<T>& _lst) : lst(_lst) {}
    virtual bool contains(T elem) = 0;
    //virtual bool contains_if(Func pred) = 0;  these should be implemented
    virtual ~self_organizing_list() {}
};

template <typename T>
class move_to_front : public self_organizing_list<T> {
private:
    void bring_elem_forward(typename std::list<T>::iterator& iter){
        this->lst.insert(this->lst.begin(), *iter);
        this->lst.erase(iter);
    }
public:
    move_to_front(std::list<T>& _lst) : self_organizing_list<T>(_lst) { }
    bool contains(T elem) { //override
        typename std::list<T>::iterator iter = this->lst.begin();
        while(iter != this->lst.end()){
            if(*iter == elem){
                bring_elem_forward(iter);
                return true;
            } else {
                iter++;
            }
        }
        return false;
    }
    template <typename Func>
    bool contains_if(Func pred) {
        typename std::list<T>::iterator iter = this->lst.begin();
        while(iter != this->lst.end()){
            if( pred(*iter) ){
                bring_elem_forward(iter);
                return true;
            } else {
                iter++;
            }
        }
        return false;
    }
};

template <typename T>
class frequency_list : public self_organizing_list<T> {
private:
    std::map<T, int> frequencies;
    void elem_increase_prio (typename std::list<T>::iterator& iter){
        //initialize if element doesn't exist
        if(frequencies.find(*iter)==frequencies.end()) frequencies[*iter] = 0;
        //increase priority
        frequencies[*iter]++;
        //find first priority lesser than ours
        typename std::list<T>::iterator i = this->lst.begin();
        while(i != this->lst.end()) {
            if(frequencies[*i]<frequencies[*iter]){ //itt döntünk, hogy az azonosakon belül hova tesszük: a legvégére
                this->lst.insert(i, *iter);
                this->lst.erase(iter);
                return;
            }
            i++;
        }
        //ha nincsen kisebb, nincsen dolgunk, return
    }
public:
    frequency_list(std::list<T>& _lst) : self_organizing_list<T>(_lst) { }
    bool contains (T elem) { //override
        typename std::list<T>::iterator iter = this->lst.begin();
        while(iter!=this->lst.end()){
            if(*iter == elem) {
                elem_increase_prio(iter);
                return true;
            }
            else {
                iter++;
            }
        }
        return false;
    }
    template <typename Func>
    bool contains_if (Func pred) {
        typename std::list<T>::iterator iter = this->lst.begin();
        while(iter!=this->lst.end()){
            if( pred(*iter) ) {
                elem_increase_prio(iter);
                return true;
            }
            else {
                iter++;
            }
        }
        return false;
    }
};

#endif
