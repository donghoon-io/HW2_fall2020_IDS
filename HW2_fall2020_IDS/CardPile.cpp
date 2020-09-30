//
//  CardPile.cpp
//  HW2_fall2020_IDS
//
//  Created by HoonieMac on 2020/09/28.
//

#include <iostream>
#include "Card.cpp"
using namespace std;

class CardPile {
private:
    Card* arr;
    int capacity;
    int top;
    
public:
    CardPile(int capacity = 10) {
        this->arr = new Card [capacity];
        this->capacity = capacity;
    };
    ~CardPile() { delete [] arr; };
    
    void push(const Card card) {
        if (capacity == top+1) {
            doubleSize1D();
            capacity = capacity*2;
        }
        arr[++top] = card;
    }
    void pop() {
        if (isEmpty()) throw "Bag is empty";
        arr[top--].~Card();
    }
    bool isEmpty() {
        return (top == -1);
    }
    void doubleSize1D() {
        if (capacity == 0) {
            arr = new Card [1];
            capacity = 1;
            return;
        }
        
        Card* temp = new Card[capacity*2];
        for (int i = 0; i < capacity; i++) {
            temp[i] = arr[i];
        }
        delete [] arr;
        arr = temp;
    }
};

