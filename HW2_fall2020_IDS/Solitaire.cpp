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
    bool isRevealed() { return !isHidden; }
    bool isBlack() { return (type == 0 || type == 1); }
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
    
    bool isEmpty() { return (top == -1); }
    Card* getArray() { return arr; }
    int getCardNum() { return top + 1; }
    void push(Card card) {
        if (capacity == top+1) {
            doubleSize1D();
            capacity = capacity*2;
        }
        arr[++top] = card;
    }
    void pop() {
        if (isEmpty()) throw "Stack is empty";
        arr[top--].~Card();
    }
    Card getTopCard() {
        return arr[top];
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
    std::tuple<Card*, int> checkConsecutives() {
        Card* temp = new Card [13];
        int topIdx = top;
        int addIdx = 0;
        while (arr[topIdx].isRevealed() && topIdx > -1) {
            if (addIdx == 0) temp[addIdx++] = arr[topIdx--];
            else if (arr[topIdx+1].isBlack() != arr[topIdx].isBlack() && arr[topIdx].getNumber() - arr[topIdx+1].getNumber() == 1) temp[addIdx++] = arr[topIdx--];
            else break;
        }
        return std::make_tuple(temp, addIdx);
    }
    void revealLast() {
        this->arr[top].revealCard();
    }
    void removeCards(int number) {
        for (int i=0; i<number; i++) {
            this->pop();
        }
        this->revealLast();
    }
};









class Solitaire {
private:
    CardPile *stock = new CardPile(), *waste = new CardPile();
    CardPile *play1 = new CardPile(), *play2 = new CardPile(), *play3 = new CardPile(), *play4 = new CardPile(), *play5 = new CardPile(), *play6 = new CardPile(), *play7 = new CardPile();
    CardPile *output1 = new CardPile(), *output2 = new CardPile();
    
public:
    Solitaire() {};
    ~Solitaire() {};
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
            if (i == 0 || i == 2 || i == 5 || i == 9 || i == 14 || i == 20 || i == 27) a[i].revealCard();
            if (i<1) play1->push(a[i]);
            else if (i<3) play2->push(a[i]);
            else if (i<6) play3->push(a[i]);
            else if (i<10) play4->push(a[i]);
            else if (i<15) play5->push(a[i]);
            else if (i<21) play6->push(a[i]);
            else if (i<28) play7->push(a[i]);
            else stock->push(a[i]);
        }
        delete [] a;
        printPiles();
    }
    bool checkMobility(CardPile* from, CardPile* to, int cardNum) {
        return (from->getArray()[from->getCardNum()-cardNum].getNumber() + 1 == to->getTopCard().getNumber() && from->getArray()[from->getCardNum()-cardNum].getType() != to->getTopCard().getType());
    }
    void move(CardPile* from, CardPile* to, int cardNum) {
        for (int i = from->getCardNum()-cardNum; i < from->getCardNum(); i++) {
            to->push(from->getArray()[i]);
        }
        from->removeCards(cardNum);
    }
    void printPiles() {
        printPile(play1);
        printPile(play2);
        printPile(play3);
        printPile(play4);
        printPile(play5);
        printPile(play6);
        printPile(play7);
        printPile(stock);
        printConsecutives();
    }
    void printConsecutives() {
        CardPile* temp = new CardPile();
        temp->push(Card(5, 1, false));
        temp->push(Card(4, 3, false));
        temp->push(Card(3, 1, false));
        temp->push(Card(2, 3, false));
        temp->push(Card(1, 0, false));
        int tempNum = std::get<1>(temp->checkConsecutives());
        cout << tempNum << endl;
    }
    void printPile(CardPile* pile) {
        for (int i = 0; i < pile->getCardNum(); i++) {
            if (pile->getArray()[i].getType() == 0) cout << "♠" << pile->getArray()[i].getNumber();
            else if (pile->getArray()[i].getType() == 1) cout << "♣" << pile->getArray()[i].getNumber();
            else if (pile->getArray()[i].getType() == 2) cout << "♡" << pile->getArray()[i].getNumber();
            else cout << "◇" << pile->getArray()[i].getNumber();
            if (pile->getArray()[i].isRevealed()) {
                cout << "R ";
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
};











int main(int argc, const char * argv[]) {
    Solitaire solitaire = Solitaire();
    solitaire.initialize();
    
    return 0;
}

