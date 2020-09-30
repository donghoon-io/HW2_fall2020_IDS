//
//  Solitaire.cpp
//  HW2_fall2020_IDS
//
//  Created by HoonieMac on 2020/09/30.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;



class Card {
private:
    int number = 0; // from 1 to 13
    int type = 0; //spade: 0; clova: 1; heart: 2, diamond: 3
    bool isHidden = true;
public:
    Card() {};
    Card(const int number, const int type) {
        this->number = number;
        this->type = type;
    }
    Card(const int number, const int type, bool isHidden) {
        this->number = number;
        this->type = type;
        this->isHidden = isHidden;
    }
    ~Card() {};
    
    int getNumber() { return number; }
    int getType() { return type; }
    void revealCard() { isHidden = false; }
};

class CardPile {
private:
    Card* arr;
    int capacity;
    int top = -1;
    
public:
    CardPile(int capacity = 10) {
        this->arr = new Card [capacity];
        this->capacity = capacity;
    };
    ~CardPile() {};
    
    void push(Card card) {
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
    Card* getArray() {
        return arr;
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









class Solitaire {
private:
    CardPile *stock = new CardPile(), *waste = new CardPile();
    CardPile *play1 = new CardPile(), *play2 = new CardPile(), *play3 = new CardPile(), *play4 = new CardPile(), *play5 = new CardPile(), *play6 = new CardPile(), *play7 = new CardPile();
    CardPile *output1 = new CardPile(), *output2 = new CardPile();
    
public:
    Solitaire() {};
    ~Solitaire() {delete [] stock;};
    Card* generate52Cards() {
        Card* arr = new Card [52];
        int idx = 0;
        for (int i=1; i<14; i++) {
            for (int j=0; j<4; j++) {
                arr[idx++] = Card(i, j);
            }
        }
        randomize(arr, 52);
        return arr;
    }
    void swapCard(Card *input1, Card *input2) {
        Card temp = *input1;
        *input1 = *input2;
        *input2 = temp;
    }
    void randomize(Card arr[], int n) {
        srand(time(NULL));
        
        for (int i = n - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            swapCard(&arr[i], &arr[j]);
        }
    }
    void initialize() {
        Card *a;
        a = generate52Cards();
        for (int i=0; i<52; i++) {
            if (i < 1) play1->push(a[i]);
            else if (i<3) play2->push(a[i]);
            else if (i<6) play3->push(a[i]);
            else if (i<10) play4->push(a[i]);
            else if (i<15) play5->push(a[i]);
            else if (i<21) play6->push(a[i]);
            else if (i<28) play7->push(a[i]);
            else stock->push(a[i-28]);
        }
        printPiles();
    }
    void printPiles() {
        printPile(play1->getArray(), 1);
        printPile(play2->getArray(), 2);
        printPile(play3->getArray(), 3);
        printPile(play4->getArray(), 4);
        printPile(play5->getArray(), 5);
        printPile(play6->getArray(), 6);
        printPile(play7->getArray(), 7);
        printPile(stock->getArray(), 24);
    }
    void printPile(Card arr[], int n) {
        for (int i = 0; i < n; i++) {
            if (arr[i].getType() == 0) cout << "Spade " << arr[i].getNumber() << " ";
            else if (arr[i].getType() == 1) cout << "Clova " << arr[i].getNumber() << " ";
            else if (arr[i].getType() == 2) cout << "Heart " << arr[i].getNumber() << " ";
            else cout << "Diamond " << arr[i].getNumber() << " ";
        }
        cout << "\n";
    }
};


int main(int argc, const char * argv[]) {
    Solitaire solitaire = Solitaire();
    solitaire.initialize();
    
    return 0;
}

