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
    int number = -1; // from 1 to 13
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
        if (isEmpty()) throw "Stack is empty";
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
        if (top != -1) {
            this->arr[top].revealCard();
        }
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
    CardPile *plays[7] = {play1, play2, play3, play4, play5, play6, play7};
    CardPile *output1 = new CardPile(), *output2 = new CardPile(), *output3 = new CardPile(), *output4 = new CardPile();
    CardPile *outputs[4] = {output1, output2, output3, output4};
    
    int temp1[4] = {130,230,410,340};
    int temp2[4] = {120,840,20,310};
    
    bool stockAvailable = true;
    int point = -52;
    bool isDone = false;
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
            if (i == 0 || i == 2 || i == 5 || i == 9 || i == 14 || i == 20 || i == 27 || i == 51) a[i].revealCard();
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
    }
    std::tuple<bool, int> checkMobility(CardPile* from, CardPile* to, bool isSingle = false) {
        if (isSingle) {
            if (from->getCardNum() == 0) {
                return std::make_tuple(false, 0);
            }
            else if (to->getCardNum() == 0 && from->getTopCard().getNumber() == 13) {
                return std::make_tuple(true, 0);
            }
            else if (to->getCardNum() == 0) {
                return std::make_tuple(false, 0);
            }
            return std::make_tuple(from->getArray()[from->getCardNum()-1].getNumber() + 1 == to->getTopCard().getNumber() && from->getArray()[from->getCardNum()-1].isBlack() != to->getTopCard().isBlack(), 1);
        } else {
            Card* possibles = std::get<0>(from->checkConsecutives());
            int possibleNum = std::get<1>(from->checkConsecutives());
            
            if (from->getCardNum() == 0) {
                return std::make_tuple(false, 0);
            }
            else if (to->getCardNum() == 0 && possibles[possibleNum-1].getNumber() == 13) {
                if (from->getArray()[0].getNumber() == 13 && from->getArray()[0].isRevealed()) return std::make_tuple(false, 0);
                return std::make_tuple(true, possibleNum);
            }
            else if (to->getCardNum() == 0) {
                return std::make_tuple(false, 0);
            }
            
            if (to->getTopCard().getNumber() == possibles[possibleNum-1].getNumber() + 1 && to->getTopCard().isBlack() != possibles[possibleNum-1].isBlack()) {
                return std::make_tuple(true, possibleNum);
            }
            return std::make_tuple(false, 0);
        }
    }
    bool checkMobilityToOutput(CardPile* from) {
        if (from->getCardNum() == 0) return false;
        else {
            if (from->getTopCard().getNumber() == 1 && outputs[from->getTopCard().getType()]->getCardNum()==0) {
                return true;
            }
            else if (outputs[from->getTopCard().getType()]->getCardNum() != 0) {
                if (outputs[from->getTopCard().getType()]->getTopCard().getNumber()+1 == from->getTopCard().getNumber()) {
                    return true;
                }
            }
        }
        return false;
    }
    void move(CardPile* from, CardPile* to, int cardNum) {
        for (int i = from->getCardNum()-cardNum; i < from->getCardNum(); i++) {
            to->push(from->getArray()[i]);
        }
        from->removeCards(cardNum);
    }
    void checkMove() {
        CardPile* from = new CardPile();
        from->push(Card(13, 1, false));
        from->push(Card(12, 3, false));
        from->push(Card(11, 1, false));
        CardPile* to = new CardPile();
        
        printPile(from);
        printPile(to);
        
        cout<<std::get<1>(checkMobility(from, to, false))<<endl;
        
    }
    int getPoint() {
        return point;
    }
    void addPoint(int point = 5) {
        this->point += point;
    }
    bool shouldBeTerminated() {
        return isDone;
    }
    void printPiles() {
        for (int i=0; i<7; i++) {
            cout << "Play " << i << ": ";
            printPile(plays[i]);
        }
        for (int i=0; i<4; i++) {
            cout << "Output " << i << ": ";
            printPile(outputs[i]);
        }
        cout << "Stock: ";
        printPile(stock);
        cout << "Waste: ";
        printPile(waste);
        cout << endl << "===========================" << endl << endl;
    }
    
    bool checkFromStockPile() {
        if (checkMobilityToOutput(stock)) {
            int type = stock->getTopCard().getType();
            move(stock, outputs[type], 1);
            addPoint();
            
            return true;
        }
        for (int i=0; i<7; i++) {
            if (std::get<0>(checkMobility(stock, plays[i], true))) {
                move(stock, plays[i], 1);
                return true;
            }
        }
        return false;
    }
    bool checkFromWastePile() {
        if (waste->isEmpty()) return false;
        if (checkMobilityToOutput(waste)) {
            int type = waste->getTopCard().getType();
            move(waste, outputs[type], 1);
            addPoint();
            return true;
        }
        for (int i=0; i<7; i++) {
            if (std::get<0>(checkMobility(waste, plays[i], true))) {
                move(waste, plays[i], 1);
                return true;
            }
        }
        return false;
    }
    void outTemp(int* temp) {
        cout << temp[0] <<temp[1] << temp[2] << temp[3] << endl;
    }
    bool checkFromPlayingPiles() {
        for (int i=0; i<7; i++) {
            if (checkMobilityToOutput(plays[i])) {
                int type = plays[i]->getTopCard().getType();
                move(plays[i], outputs[type], 1);
                addPoint();
                return true;
            }
            for (int j=0; j<7; j++) {
                if (std::get<0>(checkMobility(plays[i], plays[j], false)) && (i != j)) {
                    if (plays[j]->getCardNum() != 0 &&
                        std::get<0>(plays[i]->checkConsecutives())[std::get<1>(plays[i]->checkConsecutives())-1].getNumber() == std::get<0>(plays[j]->checkConsecutives())[std::get<1>(plays[j]->checkConsecutives())-1].getNumber() &&
                        std::get<0>(plays[i]->checkConsecutives())[std::get<1>(plays[i]->checkConsecutives())-1].isBlack() == std::get<0>(plays[j]->checkConsecutives())[std::get<1>(plays[j]->checkConsecutives())-1].isBlack()) {
                    } else {
                        temp1[0] = temp2[0];
                        temp1[1] = temp2[1];
                        temp1[2] = temp2[2];
                        temp1[3] = temp2[3];
                        temp2[0] = plays[i]->getCardNum();
                        temp2[1] = i;
                        temp2[2] = plays[j]->getCardNum();
                        temp2[3] = j;
                        
                        move(plays[i], plays[j], std::get<1>(checkMobility(plays[i], plays[j], false)));
                        
                        return true;
                    }
                }
            }
        }
        return false;
    }
    void cycle() {
        if (stockAvailable && checkFromStockPile()) {
        } else if (checkFromWastePile()) {
        } else if (checkFromPlayingPiles()) {
        } else {
            if (stock->getCardNum() == 0) {
                stockAvailable = false;
                cout << "Point: " << getPoint() << endl;
                isDone = true;
            } else {
                move(stock, waste, 1);
            }
        }
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
    
    
    while (!solitaire.shouldBeTerminated()) {
        solitaire.cycle();
    }
    
    return 0;
}

